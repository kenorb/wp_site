<?php
// $Id: export.inc,v 1.1.2.15.2.8 2008/12/30 16:41:41 goba Exp $

/**
 * @file
 *   Gettext export for localization community.
 */

/**
 * User interface for the translation export screen.
 */
function l10n_community_export_page($uri = NULL, $langcode = NULL) {
  $breadcrumb = array(
    l(t('Home'), NULL),
    l(t('Translate'), 'translate'),
  );
  
  // Set a matching title with the translation page.
  $project = l10n_community_get_projects($uri);
  if (isset($langcode)) {
    $languages = l10n_community_get_languages();
    drupal_set_title(t('Export @language translations', array('@language' => $languages[$langcode]->name)));
    $breadcrumb[] = l(t('Explore languages'), 'translate/languages');
  }
  else {
    $project = l10n_community_get_projects($uri);
    drupal_set_title(t('Export @project translation templates', array('@project' => $project->title)));
    $breadcrumb[] = l(t('Explore projects'), 'translate/projects');
  }
  // Add missing breadcrumb.
  drupal_set_breadcrumb($breadcrumb);

  return drupal_get_form('l10n_community_export_form', $uri, $langcode);
}

/**
 * Translation export form. This is multi-step with the project selection.
 */
function l10n_community_export_form(&$form_state, $uri = NULL, $langcode = NULL) {

  $hide_projects = FALSE;
  if (!isset($uri)) {
    $uri = !empty($form_state['values']['project']) ? $form_state['values']['project'] : (!empty($_GET['project']) ? $_GET['project'] : NULL);
  }
  else {
    // When project was preset from URL, disable the selector.
    $hide_projects = TRUE;
  }
  
  $projects = l10n_community_get_projects();
  $form['data'] = array(
    '#type' => 'fieldset',
    '#title' => t('Source data'),
  );
  if ($hide_projects) {
    $form['data']['project'] = array(
      '#type' => 'value',
      // Value of $uri already verified in menu path handler.
      '#value' => $projects[$uri]->title,
    );
  }
  else {
    $form['data']['project'] = array(
      '#title' => t('Project'),
    );
    if (($count = count($projects)) <= 30) {
      // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
      $form['data']['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
      $form['data']['project']['#options'] = array();
      foreach ($projects as $project) {
        // Title used to conform to the autocomplete behavior.
        $form['data']['project']['#options'][$project->title] = $project->title;
      }
    }
    else {
      // Autocomplete field for more then 30 projects.
      $form['data']['project'] += array(
        '#type' => 'textfield',
        '#autocomplete_path' => 'translate/projects/autocomplete',
      );
    }
  }

  if (isset($projects[$uri])) {
    // Set this project as default value if identified.
    $form['data']['project']['#default_value'] = $projects[$uri]->title;
  
    $releases = l10n_community_get_releases($uri);
    $release_options = array('all' => t('All'));
    foreach ($releases as $rid => $this_release) {
      $release_options[$rid] = $this_release->title;
    }
    $form['data']['release'] = array(
      '#title' => t('Release'),
      '#type' => count($release_options) <= 3 ? 'radios' : 'select',
      '#options' => $release_options,
      '#default_value' => isset($release) ? $release : 'all',
      '#description' => t('Exporting with all releases is useful for translators, but is not optimal to use for end users because unused data would clutter up their database, when files get imported. Export for all releases if you would like to provide a complete translation, and you work with a desktop tool.'),
    );
  }
  if (isset($langcode) && isset($form['data']['release'])) {
    $form['langcode'] = array(
      '#type' => 'value',
      '#value' => $langcode,
    );

    $form['format'] = array(
      '#type' => 'fieldset',
      '#title' => t('Format'),
    );
    // Only include the type selector if we are not presenting
    // a template export page (which does not have a language).
    $form['format']['type'] = array(
      '#title' => t('Type'),
      '#type' => 'radios',
      '#options' => array('translation' => t('Translation'), 'template' => t('Translation template')),
      '#default_value' => 'translation',
      '#description' => t('Templates only contain the source (English) strings, translations are represented by empty placeholders.')
    );
    $form['format']['version'] = array(
      '#title' => t('Format'),
      '#type' => 'radios',
      '#options' => array('drupal-6' => t('Drupal 6 package format'), 'drupal-5' => t('Drupal 5 package format for autolocale module'), 'flat-package' => t('Flat package for CVS commit'), 'all-in-one' => t('All in one file')),
      '#default_value' => 'drupal-6',
      '#description' => t("Drupal 5's autolocale module and Drupal 6 use different conventions for directory naming in packages. Temporarily, as long as you need to use CVS to distribute translations the flat package helps you generate a package for committing to Drupal.org. Additionally, for testing, spell checking and smaller modules, you can also export in one file without packaging as well. It is not advised to use big .po files for importing in Drupal though, as it might lead to incomplete imports. Use packages for usage with Drupal importing."),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => isset($form['data']['release']) ? t('Export') : t('Choose project'),
  );
  return $form;
}

/**
 * Release field is mandatory.
 */
function l10n_community_export_form_validate($form, &$form_state) {
  if (empty($form_state['values']['project'])) {
    form_set_error('project', t('You should choose a project, submit the form and choose a release.'));
  }
  else if (!$project = l10n_community_project_uri_by_title($form_state['values']['project'])) {
    form_set_error('project', t('Invalid project chosen.'));
    $form_state['values']['project'] = '';
  }
}

/**
 * Generate translation or template package on the fly based on
 * all details available and return the output via HTTP.
 */
function l10n_community_export_form_submit($form, &$form_state) {

  // This was validated to work in the validation code.
  $uri = !empty($form_state['values']['project']) ? l10n_community_project_uri_by_title($form_state['values']['project']) : NULL;
  if (empty($uri)) {
    form_set_error('project', t('Please select a project first.'));
    return;
  }

  if (empty($form_state['values']['release'])) {
    form_set_error('release', t('Please choose a release or opt to export for all releases.'));
    $form_state['redirect'] = array($_GET['q'], array('project' => $uri));
    return;
  }
  elseif ($form_state['values']['release'] != 'all') {
    $releases = l10n_community_get_releases($uri);
    if (!isset($releases[$form_state['values']['release']])) {
      form_set_error('release', t('Invalid release chosen.'));
      return;
    }
  }
  
  $language = NULL;
  if (isset($form_state['values']['langcode'])) {
    $languages = l10n_community_get_languages();
    $language = $languages[$form_state['values']['langcode']];
  }
  $type = (isset($form_state['values']['type']) ? $form_state['values']['type'] : 'template');

  // Generate tarball or PO file and get file name.
  $export_result = l10n_community_export(
    $uri,
    ($form_state['values']['release'] == 'all' ? NULL : $form_state['values']['release']),
    $language,
    ($type != 'translation'),
    // If not set (exporting a template for a module), stick to all-in-one.
    isset($form_state['values']['version']) ? $form_state['values']['version'] : 'all-in-one'
  );
  
  if (isset($export_result) && is_array($export_result)) {
    // If we got an array back from the export build, tear that into pieces.
    list($mime_type, $file_name, $serve_name) = $export_result;
    // Return compressed archive to user.
    header('Content-Disposition: attachment; filename='. $serve_name);
    header('Content-Type: '. $mime_type);
    echo file_get_contents($file_name);
    unlink($file_name);
    die();  
  }
  else {
    // Messages should already be recorded about any build errors.
    return;
  }
}

/**
 * Helper function to store the export string.
 */
function _l10n_community_export_string_files(&$string_files, $uri, $language, $template, $version, $export_string) {
  $po_folder = ($version == 'drupal-5' ? 'po' : 'translations');

  if (!empty($export_string)) {
    
    // Location comments are constructed in fileone:1,2,5; filetwo:123,537
    // format, where the numbers represent the line numbers of source
    // occurances in the respective source files.
    $comment = array();
    foreach ($export_string['comment'] as $path => $lines) {
      $comment[] = preg_replace('!(^[^/]+/)!', '', $path) .':'. join(',', $lines);
    }
    $comment = '#: '. join('; ', $comment) ."\n";

    // File location is dependent on export format and string location.
    if ($version == 'all-in-one') {
      // Fold all strings into this one file.
      $filename = 'general';
    }
    elseif ((strpos($comment, '.info') && $uri == 'drupal') || count(array_keys($export_string['comment'])) > 1) {
      // An .info file string in Drupal core (which is folded into
      // general.po, so that later the module screen has all module info
      // translated for the admin). Or appeared in more then one file, so
      // goes to general.po for that reason.
      
      // Note that some modules like ubercart might not have their
      // root module in the root folder, so this needs to be rethought.
      $filename = ($version != 'flat-package' ? $po_folder .'/' : '') .'general';
    }
    else {
      // Putting into a file named after the specific directory.
      $filename = dirname(preg_replace('!(^[^/]+/)!', '', array_shift(array_keys($export_string['comment']))));
      if (empty($filename) || $filename == '.') {
        $filename = ($version != 'flat-package' ? ($po_folder .'/') : '') .'root';
      }
      elseif ($version != 'flat-package') {
        $filename .= '/'. $po_folder .'/'. str_replace('/', '-', $filename);
      }
      else {
        $filename = str_replace('/', '-', $filename);
      }
    }
    
    // Temporary hack to make the core exports put the system module
    // files to the right place. See http://drupal.org/node/212594
    // This should be solved more elegantly with a knowledge on the
    // non-module files (non-info file based directories in Drupal 6)
    // and a default location for each project.
    if (!in_array($version, array('flat-package', 'all-in-one')) && ($uri == 'drupal')) {
      $system_files = array(
        $po_folder .'/general',
        "includes/$po_folder/includes",
        "misc/$po_folder/misc"
      );
      if (in_array($filename, $system_files)) {
        $filename = "modules/system/$po_folder/". basename($filename);
      }
    }
    
    // Append extension.
    $filename .= ($template ? '.pot' : ($version != 'flat-package' ? ('.'. $language->language) : '') .'.po');
    
    if (strpos($export_string['value'], "\0") !== FALSE) {
      // This is a string with plural variants.
      list($singular, $plural) = explode("\0", $export_string['value']); 
      $output = $comment .'msgid '. _l10n_community_export_string($singular) .'msgid_plural '. _l10n_community_export_string($plural);
      if (!$template && !empty($export_string['translation'])) {
        // Export translations we have.
        foreach (explode("\0", $export_string['translation']) as $id => $value) {
          $output .= 'msgstr['. $id .'] '. _l10n_community_export_string($value);
        }
      }
      elseif (isset($language)) {
        // Empty msgstrs based on plural formula for language. Could be
        // a plural without translation or a template generated for a
        // specific language.
        for ($pi = 0; $pi < $language->plurals; $pi++) {
          $output .= 'msgstr['. $pi .'] ""'."\n";
        }
      }
      else {
        // Translation template without language, assume two msgstrs.
        $output .= 'msgstr[0] ""'."\n";
        $output .= 'msgstr[1] ""'."\n";
      }
    }
    else {
      // Simple string (and possibly translation pair).
      $output = $comment .'msgid '. _l10n_community_export_string($export_string['value']);
      if (!empty($export_string['translation'])) {
        $output .= 'msgstr '. _l10n_community_export_string($export_string['translation']);
      }
      else {
        $output .= 'msgstr ""'."\n";
      }
    }

    $file_outputs = array($filename);
    if ($export_string['type'] != 2 /* POTX_STRING_RUNTIME */ && $version != 'all-in-one') {
      // Not runtime-only, so make sure we try to have an installer entry.
      $file_outputs[] = ($uri == 'drupal' ?
        (($version == 'flat-package') ?
          ('installer'. ($template ? '.pot' : '.po')) :
          ('profiles/default/'. $po_folder .'/'. ($template ? 'installer.pot' : ($language->language .'.po')))) :
        ($po_folder .'/installer'. ($template ? '.pot' : ('.'. $language->language .'.po')))
      );
      if ($export_string['type'] == 1 /* POTX_STRING_INSTALLER */) {
        // Installer-only, remove runtime entry.
        $file_outputs = array($file_outputs[1]);
      }
    }
    foreach ($file_outputs as $filename) {
      if (!isset($string_files[$filename])) {
        // Prepare new file storage for use.
        $string_files[$filename] = array('file' => '', 'changed' => 0, 'revisions' => array());
      }
      // Add to existing file storage.
      $string_files[$filename]['file'] .= $output ."\n";
      if (!$template) {
        $string_files[$filename]['changed'] = max($string_files[$filename]['changed'], $export_string['changed']);
      }
      $string_files[$filename]['revisions'] = array_unique(array_merge($string_files[$filename]['revisions'], $export_string['revisions']));
    }
  }
}

/**
 * Generates the PO(T) files contents and wrap them in a tarball for a given
 * project.
 *
 * @param $uri
 *   Project URI.
 * @param $release
 *   Release number (rid) to generate tarball for, or NULL to generate
 *   with all releases considered.
 * @param $language
 *   Language object.
 * @param $template
 *   TRUE if templates should be exported, FALSE if translations.
 * @param $version
 *   Version to export with: 'drupal-6', 'drupal-5', 'flat-package' and
 *   'all-in-one'. 'all-in-one' means one .po file, 'flat-package' means no
 *   directory structure generated, the others are suitable packages for the
 *   given Drupal version.
 *
 * @todo
 *   Look into possibly exporting suggestions as fuzzy translations.
 */
function l10n_community_export($uri, $release = NULL, $language = NULL, $template = TRUE, $version = NULL) {
  // l10n_community_requirements() makes sure there is a status
  // error if this is not installed.
  include_once 'Archive/Tar.php';
  
  $project = l10n_community_get_projects($uri);
  if ($template) {
    // We are exporting a template explicitly.
    $sql = 'SELECT s.sid, s.value, f.location, f.revision, l.lineno, l.type FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid WHERE r.pid = %d';
    $sql_args = array($project->pid);
  }
  else {
    // We only export active translations, not suggestions.
    $sql = "SELECT s.sid, s.value, f.location, f.revision, l.lineno, l.type, t.translation, t.uid_approved, t.time_approved FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid  LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid AND t.language = '%s' AND is_active = 1 AND is_suggestion = 0 WHERE r.pid = %d";
    $sql_args = array($language->language, $project->pid);
  }
  
  if (isset($release)) {
    // Release restriction.
    $sql_args[] = $release;
    $sql .= ' AND r.rid = %d';
    $releases = l10n_community_get_releases($uri, TRUE);
    $release = $releases[$release];
  }
  
  // Source strings will be repeated as many times as they appear, so to generate
  // the export file properly, order by the source id.
  $sql .= ' ORDER BY s.sid';
  
  $result = db_query($sql, $sql_args);
  $previous_sid = 0;
  $export_string = $string_files = array();
    
  while ($string = db_fetch_object($result)) {
    if ($string->sid != $previous_sid) {
      // New string in the stream.
      _l10n_community_export_string_files($string_files, $uri, $language, $template, $version, $export_string);

      // Now fill in the new string values.
      $previous_sid = $string->sid;
      $export_string = array(
        'comment'     => array($string->location => array($string->lineno)),
        'value'       => $string->value,
        'translation' => isset($string->translation) ? $string->translation : '',
        'revisions'   => array($string->revision),
        'changed'     => isset($string->time_approved) ? $string->time_approved : 0,
        'type'        => $string->type,
      );
    }
    else {
      // Existing string but with new location information.
      $export_string['comment'][$string->location][] = $string->lineno;
      $export_string['revisions'][] = $string->revision;
      if ($export_string['type'] != 0 && $export_string['type'] != $string->type) {
        // Elevate string type if it is not already 0 (POTX_STRING_BOTH), and
        // the currently found string type is different to the previously found.
        $export_string['type'] = 0;
      }
    }
  }
  if ($previous_sid > 0) {
    // Store the last string.
    _l10n_community_export_string_files($string_files, $uri, $language, $template, $version, $export_string);
  }
  
  if (empty($string_files)) {
    // No strings were found.
    if (isset($release)) {
      $message = t('There are no strings in the %release release of %project to export.', array('%project' => $project->title, '%release' => $release->title));
    }
    else {
      $message = t('There are no strings in any releases of %project to export.', array('%project' => $project->title));
    }
    // Message to the user.
    drupal_set_message($message);
    // Message to watchdog for possible automated packaging. 
    watchdog('l10n_community', $message);
    return NULL;
  }

  // Generate a 'unique' temporary filename for this package.
  $tempfile = tempnam(file_directory_temp(), 'l10n_community-'. $uri);
  
  if ($version != 'all-in-one') {
    // Generate tgz file with all files added.
    $tar = new Archive_Tar($tempfile, 'gz');
  }
  foreach ($string_files as $filename => $fileinfo) {
    if (count($fileinfo['revisions']) == 1) {
      $file_list = '# Generated from file: '. $fileinfo['revisions'][0] ."\n";
    }
    else {
      $file_list = '# Generated from files:'."\n#  ". join("\n#  ", $fileinfo['revisions']) ."\n";
    }
    
    $release_title = $project->title .' ('. (isset($release) ? $release->title : 'all releases') .')';
    if (!$template) {
      $header = '# '. $language->name .' translation of '. $release_title ."\n";
      $header .= "# Copyright (c) ". date('Y') .' by the '. $language->name ." translation team\n";
      $header .= $file_list;
      $header .= "#\n";
      $header .= "msgid \"\"\n";
      $header .= "msgstr \"\"\n";
      $header .= "\"Project-Id-Version: ". $release_title ."\\n\"\n";
      $header .= "\"POT-Creation-Date: ". date("Y-m-d H:iO") ."\\n\"\n";
      // Use date placeholder, if we have no date information (no translation here yet).
      $header .= "\"PO-Revision-Date: ". (!empty($fileinfo['changed']) ? date("Y-m-d H:iO", $fileinfo['changed']) : 'YYYY-mm-DD HH:MM+ZZZZ') ."\\n\"\n";
      $header .= "\"Language-Team: ". $language->name ."\\n\"\n";
      $header .= "\"MIME-Version: 1.0\\n\"\n";
      $header .= "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
      $header .= "\"Content-Transfer-Encoding: 8bit\\n\"\n";
      if ($language->formula && $language->plurals) {
        $header .= "\"Plural-Forms: nplurals=". $language->plurals ."; plural=". strtr($language->formula, array('$' => '')) .";\\n\"\n";
      }
    }
    else {
      $language_title = (isset($language) ? $language->name : 'LANGUAGE');
      $header = "# ". $language_title ." translation of ". $release_title ."\n";
      $header .= "# Copyright (c) ". date('Y') ."\n";
      $header .= $file_list;
      $header .= "#\n";
      $header .= "msgid \"\"\n";
      $header .= "msgstr \"\"\n";
      $header .= "\"Project-Id-Version: ". $release_title ."\\n\"\n";
      $header .= "\"POT-Creation-Date: ". date("Y-m-d H:iO") ."\\n\"\n";
      $header .= "\"PO-Revision-Date: YYYY-mm-DD HH:MM+ZZZZ\\n\"\n";
      $header .= "\"Language-Team: ". $language_title ."\\n\"\n";
      $header .= "\"MIME-Version: 1.0\\n\"\n";
      $header .= "\"Content-Type: text/plain; charset=utf-8\\n\"\n";
      $header .= "\"Content-Transfer-Encoding: 8bit\\n\"\n";
      if (isset($language) && $language->formula && $language->plurals) {
        $header .= "\"Plural-Forms: nplurals=". $language->plurals ."; plural=". strtr($language->formula, array('$' => '')) .";\\n\"\n";
      }
      else {
        $header .= "\"Plural-Forms: nplurals=INTEGER; plural=EXPRESSION;\\n\"\n";
      }
    }
    if ($version == 'all-in-one') {
      // Write to file directly. We should only do this once.
      $fh = fopen($tempfile, 'w');
      fwrite($fh, $header ."\n". $fileinfo['file']);
      fclose($fh);
    }
    else {
      // Add file to the tgz.
      $tar->addString($filename, $header ."\n". $fileinfo['file']);
    }
  }

  if ($version == 'all-in-one') {
    // Output a single PO(T) file in this case.
    return array('text/plain', $tempfile, $uri .'-'. (isset($release) ? $release->title : 'all') . (isset($language) ? '-'. $language->language : '') . ($template ? '.pot' : '.po'));
  }
  else {
    // Output a package in this case.
    return array('application/x-compressed', $tempfile, $uri .'-'. (isset($release) ? $release->title : 'all') . (isset($language) ? '-'. $language->language : '') . ($template ? '-templates' : '-translations') .'.tgz');
  }
}

/**
 * Print out a string on multiple lines
 */
function _l10n_community_export_string($str) {
  $stri = addcslashes($str, "\0..\37\\\"");
  $parts = array();

  // Cut text into several lines
  while ($stri != "") {
    $i = strpos($stri, "\\n");
    if ($i === FALSE) {
      $curstr = $stri;
      $stri = "";
    }
    else {
      $curstr = substr($stri, 0, $i + 2);
      $stri = substr($stri, $i + 2);
    }
    $curparts = explode("\n", _l10n_community_export_wrap($curstr, 70));
    $parts = array_merge($parts, $curparts);
  }

  // Multiline string
  if (count($parts) > 1) {
    return "\"\"\n\"". implode("\"\n\"", $parts) ."\"\n";
  }
  // Single line string
  elseif (count($parts) == 1) {
    return "\"$parts[0]\"\n";
  }
  // No translation
  else {
    return "\"\"\n";
  }
}

/**
 * Custom word wrapping for Portable Object (Template) files.
 */
function _l10n_community_export_wrap($str, $len) {
  $words = explode(' ', $str);
  $ret = array();

  $cur = "";
  $nstr = 1;
  while (count($words)) {
    $word = array_shift($words);
    if ($nstr) {
      $cur = $word;
      $nstr = 0;
    }
    elseif (strlen("$cur $word") > $len) {
      $ret[] = $cur ." ";
      $cur = $word;
    }
    else {
      $cur = "$cur $word";
    }
  }
  $ret[] = $cur;

  return implode("\n", $ret);
}
