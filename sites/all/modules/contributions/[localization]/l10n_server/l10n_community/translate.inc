<?php
// $Id: translate.inc,v 1.1.2.7.2.7 2008/12/30 16:57:39 goba Exp $

/**
 * @file
 *   Translation view and editing pages for localization community.
 */

// = Translation interface hub =================================================

/**
 * Menu callback for the translation pages.
 *
 * Displays a translation view or translation edit page depending
 * on permissions. If no strings are found, an error is printed.
 *
 * @param $langcode
 *   Language code, for example 'hu', 'pt-br', 'de', 'it'.
 */
function l10n_community_translate_page($langcode = NULL, $mode = 'view') {
  
  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate')
    )    
  );
  
  $languages = l10n_community_get_languages();
  $perm = l10n_community_get_permission($langcode);

  // Build filter values for the form generation.
  $project = $release = NULL;
  if (isset($_GET['project']) && ($project = l10n_community_get_projects($_GET['project']))) {
    if (isset($_GET['release']) && ($releases = l10n_community_get_releases($project->uri)) && isset($releases[$_GET['release']])) {
      // Allow to select this release, if belongs to current project.
      $release = $_GET['release'];
    }
  }
  // Thankfully L10N_STATUS_ALL is 0, so non-number bad data goes to no filter. 
  $status  = isset($_GET['status']) ? (int) $_GET['status'] : 0;
  $search  = isset($_GET['search']) ? (string) $_GET['search'] : '';
  
  $output = drupal_get_form('l10n_community_filter_form', $project, $status, $release, $search);
  
  $strings = l10n_community_get_strings($languages[$langcode]->language, $project, $status, $release, $search, 10);
  if (!count($strings)) {
    drupal_set_message(t('No strings found with this filter. Try adjusting the filter options.'));
  }
  elseif ($perm == L10N_PERM_NONE || $mode == 'view') {
    // For users without permission to translate or suggest, display the view.
    drupal_set_title(t('@language translations', array('@language' => $languages[$langcode]->name)));
    $output .= l10n_community_translate_view($strings, $languages[$langcode], $project);
  }
  else {
    // For users with some permission, display the form.
    drupal_add_js(drupal_get_path('module', 'l10n_community') .'/l10n_community.js');
    drupal_set_title(t('Translate to @language', array('@language' => $languages[$langcode]->name)));
    $output .= drupal_get_form('l10n_community_translate_form', $strings, $languages[$langcode], $project, $perm);
  }
  return $output;
}

// = Filter form handling ======================================================

/**
 * Translate form filter.
 */
function l10n_community_filter_form(&$form_state, $project = NULL, $status = L10N_STATUS_ALL, $release = 'all', $search  = '') {
  $projects = l10n_community_get_projects();

  $status_options = array(
    L10N_STATUS_ALL            => t('All'),
    L10N_STATUS_UNTRANSLATED   => t('Untranslated'),
    L10N_STATUS_HAS_SUGGESTION => t('Has suggestion'),
    L10N_STATUS_TRANSLATED     => t('Translated'),
  );
  
  $form['project'] = array(
    '#title' => t('Project'),
    '#default_value' => isset($project) ? $project->title : '',
  );
  if (($count = count($projects)) <= 30) {
    // Select widget for 1-30 projects.
    $form['project']['#type'] = 'select';
    $form['project']['#options'] = array('' => t('All'));
    foreach ($projects as $this_project) {
      // Title used to conform to the autocomplete behavior.
      $form['project']['#options'][$this_project->title] = $this_project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/projects/autocomplete',
    );
  }
  
  if (isset($project)) {
    $releases = l10n_community_get_releases($project->uri);
    $release_options = array('all' => t('All'));
    foreach ($releases as $rid => $this_release) {
      $release_options[$rid] = $this_release->title;
    }
    $form['release'] = array(
      '#title' => t('Release'),
      '#type' => 'select',
      '#options' => $release_options,
      '#default_value' => isset($release) ? $release : 'all',
    );
  }
  
  $form['status'] = array(
    '#title' => t('Status'),
    '#type' => 'select',
    '#options' => $status_options,
    '#default_value' => $status,
  );
  $form['search'] = array(
    '#title' => t('Contains'),
    '#type' => 'textfield',
    '#default_value' => $search,
  );
  $form['submit'] = array(
    '#value' => t('Filter'),
    '#type' => 'submit',
  );
  $form['reset'] = array(
    '#value' => t('Reset'),
    '#type' => 'submit',
  );
  $form['#theme'] = 'l10n_community_filter_form';
  return $form;
}

/**
 * Submission handler for filtering form.
 */
function l10n_community_filter_form_submit($form, &$form_state) {

  if ($form_state['values']['op'] == t('Reset')) {
    // Just go with the redirection flow => removes URL params.
    return;
  }
  
  if ($form_state['values']['op'] == t('Filter')) {
    // Validate incoming data. The project needs to be resolved by title,
    // so that we redirect with the proper uri value. Other validations 
    // are just to make sure that the URL keeps being short, and the user 
    // is not tricked into thinking a certain filter is applied, while 
    // being invalid. These checks are done on input as well.
    if (!empty($form_state['values']['project'])) {
      if ($uri = l10n_community_project_uri_by_title($form_state['values']['project'])) {
        // Remember proper internal name based on title.
        $form_state['values']['project'] = $uri;
        if (!empty($form_state['values']['release']) && !(($releases = l10n_community_get_releases($uri)) && isset($releases[$form_state['values']['release']]))) {
          // Such a release was not found for this project. Unset.
          unset($form_state['values']['release']);
        }
      }
      else {
        // If project not found, release data is cleared.
        unset($form_state['values']['release']);
      }
    }
    $form_state['values']['status'] = (int) $form_state['values']['status'];

    // Redirect keeping the relevant filters intact in the URL.
    $form_state['redirect'] = array(
      $_GET['q'], 
      l10n_community_build_filter_array($form_state['values'])
    );
  }
}

// = Translation viewer ========================================================

/**
 * Form for translations display.
 *
 * @param $strings
 *   Array of string objects to display on the page.
 * @param $language
 *   Language object corresponding to the page displayed.
 * @param $project
 *   Project object.
 */
function l10n_community_translate_view($strings = array(), $language = NULL, $project = NULL) {
  $output = '';
  $rows = array();
  foreach ($strings as $string) {
    $row = array();
    // Source display
    // Multiple source strings if we deal with plurals.
    if (strpos($string->value, "\0") !== FALSE) {
      $source = theme('item_list', array_map('l10n_community_format_text', explode(chr(0), $string->value)), '');
    }
    else {
      $source = l10n_community_format_text($string->value);
    }
    $row[] = array('data' => $source, 'class' => 'source');
    // Translation display.
    if (!empty($string->translation)) {
      if (strpos(chr(0), $string->value) !== FALSE) {
        $translations = explode(chr(0), l10n_community_format_text($string->translation));
        // Fill in any missing items, so it is shown that not all items are done.
        if (count($translations) < $language->plurals) {
          $translations = array_merge($translations, array_fill(0, count($translations) - $language->plurals, '')); 
        }
        $translation = theme('item_list', $translations);
      }
      else {
        $translation = l10n_community_format_text($string->translation);
      }
      $row[] = $translation;
    }
    else {
      $row[] = '';
    }
    $rows[] = $row;
  }
  $output .= theme('pager', NULL, 10, 0);
  $output .= theme('table', array(t('Source Text'), t('Translations')), $rows, array('class' => 'l10n-server-translate'));
  $output .= theme('pager', NULL, 10, 0);
  $output = "<div id='l10n-community-translate-view'>". $output ."</div>";
  return $output;
}

// = Translation editor ========================================================

/** 
 * Translation web interface.
 *
 * @param $strings
 *   Array of string objects to display.
 * @param $language
 *   Language object.
 * @param $project
 *   Project object.
 * @param $perm
 *   Community permission level of user watching the page.
 */
function l10n_community_translate_form(&$form_state, $strings = array(), $language = NULL, $project = NULL, $perm = L10N_PERM_SUGGEST) {
  
  $form = array(
    '#tree' => TRUE,
    '#redirect' => array(
      $_GET['q'],
      // Ensure that we keep all filter values, even the page number, so
      // after submission, the same page can be shown.
      l10n_community_build_filter_array($_GET, TRUE)
    )
  );
  $form['pager_top'] = array(
    '#weight' => -10,
    '#value' => theme('pager', NULL, 10, 0)
  );
  $form['pager_bottom'] = array(
    '#weight' => 10,
    '#value' => theme('pager', NULL, 10, 0)
  );
  // Keep language code and URI in form for further reference.
  $form['langcode'] = array(
    '#type' => 'value',
    '#value' => $language->language
  );
  $form['project'] = array(
    '#type' => 'value',
    '#value' => isset($project) ? $project->uri : NULL
  );

  foreach ($strings as $string) {
    $form[$string->sid] = array(
      '#tree' => TRUE,
    );

    // A toolbox which displays action icons on each string editor fieldset.
    $toolbox = theme('l10n_community_button', 'translate', 'l10n-translate active');
    $toolbox .= theme('l10n_community_button', 'lookup', 'l10n-lookup');
    $toolbox .= $string->has_suggestion ? theme('l10n_community_button', 'has-suggestion', 'l10n-suggestions') : "";
    $toolbox = "<div class='toolbox'>$toolbox</div>";    
    $form[$string->sid]['toolbox'] = array(
      '#type' => 'markup',
      '#value' => $toolbox,
    );
    $form[$string->sid]['messagebox'] = array(
      '#type' => 'markup',
      '#value' => "<div class='messagebox'></div>",
    );

    $is_plural = strpos($string->value, "\0");
    // Multiple source strings if we deal with plurals. The form item and
    // consequently the JavaScript strings identifiers are the sid and then
    // the index of the plural being displayed.
    $string_parts = explode(chr(0), $string->value);
    foreach ($string_parts as $delta => &$part) {
      $part = l10n_community_format_text($part, $string->sid, (count($string_parts) > 1) ? $delta : NULL);
    }
    $source = theme('l10n_community_strings', $string_parts);

    $form[$string->sid]['source'] = array(
      '#type' => 'item',
      '#value' => $source,
    );
    
    $translated = !empty($string->translation);
    $form[$string->sid]['translation'] = array(
      '#type' => 'item',
      // Hide editing controls of translated stuff to save some space and guide user eyes.
      '#prefix' => '<div id="l10n-community-wrapper-'. $string->sid .'"'. ($translated ? ' class="hidden"' : '') .'>',
      '#suffix' => '</div>',
    );

    if ($is_plural) {
      
      // Dealing with a string with plural versions.
      if ($translated) {
        // Add translation form element with all plural versions.
        $translations = explode("\0", $string->translation);
        $string_parts = array();
        for ($i = 0; $i < $language->plurals; $i++) {
          $target = $string->sid .'-'. $i;
          $string_parts[] = l10n_community_format_text($translations[$i], $string->sid, $i);
        }
        $form[$string->sid]['translation_existing'] = array(
          '#type' => 'item',
          '#value' => theme('l10n_community_strings', $string_parts),
        );
      }
      
      $string_parts = explode(chr(0), $string->value);
            
      for ($i = 0; $i < $language->plurals; $i++) {
        $target = $string->sid .'-'. $i;
        if ($translated) {
          // Already translated so we ask for new translation or suggestion.
          $description = ($perm == L10N_PERM_SUGGEST) ? t('New suggestion for variant #%d', array('%d' => $i)) : t('New translation for variant #%d', array('%d' => $i));
        }
        else {
          // Not translated yet, so we ask for initial translation or suggestion.
          $description = ($perm == L10N_PERM_SUGGEST) ? t('Suggestion for variant #%d', array('%d' => $i)) : t('Translation for variant #%d', array('%d' => $i));
        }
        
        // Include a textarea for each plural variant.
        $form[$string->sid]['translation']['value'][$i] = array(
          '#type' => strlen($string_parts[$i]) > 45 ? 'textarea' : 'textfield',
          '#description' => $description,
          '#rows' => 1,
          '#id' => 'l10n-community-translation-'. $target,
        );
      }
    }
    
    // Dealing with a simple string (no plurals).
    
    else {      
      if ($translated) {
        $form[$string->sid]['translation_existing'] = array(
          '#type' => 'item',
          '#value' => theme('l10n_community_strings', array(l10n_community_format_text($string->translation, $string->sid))),
        );
      }
      $form[$string->sid]['translation']['value'] = array(
        '#type' => strlen($string->value) > 45 ? 'textarea' : 'textfield',
        // Provide accurate title based on previous data and permission.
        '#description' => $translated ? (($perm == L10N_PERM_SUGGEST) ? t('Add a new suggestion') : t('Add a new translation')) : (($perm == L10N_PERM_SUGGEST) ? t('Suggestion') : ''),
        '#rows' => 4,
        '#resizable' => FALSE,
        '#cols' => NULL,
        '#size' => NULL,
        '#id' => 'l10n-community-translation-'. $string->sid,
      );
      if (strlen($string->value) > 200) {
        $form[$string->sid]['translation']['value']['#rows'] = floor(strlen($string->value) * .03);
        $form[$string->sid]['translation']['value']['#resizable'] = TRUE;
      }      
    }

    // Add AJAX saving buttons
    $form[$string->sid]['translation']['save'] = array(
      '#prefix' => "<span class='l10n-approval-buttons'>",
      '#value' => theme('l10n_community_button', 'save', 'l10n-save'),
      '#type' => 'markup',
    );
    $form[$string->sid]['translation']['clear'] = array(
      '#suffix' => "</span>",
      '#value' => theme('l10n_community_button', 'clear', 'l10n-clear'),
      '#type' => 'markup',
    );
    
    if ($perm == L10N_PERM_SUGGEST) {
      // User with suggestion capability only, record this.
      $form[$string->sid]['translation']['is_suggestion'] = array(
        '#type' => 'value',
        '#value' => TRUE
      );
    }
    else {
      // User with full privileges, offer option to submit suggestion.
      $form[$string->sid]['translation']['is_suggestion'] = array(
        '#title' => t('Suggestion for discussion'),
        '#type' => 'checkbox',
      );
    }
  }
  
  // Add all strings for copy-pasting and some helpers.
  drupal_add_js(
    array(
      'l10n_lookup_help'      => t('Show detailed information.'),
      'l10n_approve_error'    => t('There was an error approving this suggestion. You might not have permission or the suggestion id was invalid.'),
      'l10n_approve_confirm'  => t('!icon Suggestion approved.', array('!icon' => '&#10004;')),

      'l10n_decline_error'    => t('There was an error declining this suggestion. You might not have permission or the suggestion id was invalid.'),
      'l10n_decline_confirm'  => t('Suggestion declined.'),

      'l10n_details_callback' => url('translate/details/'. $language->language .'/'),
      'l10n_suggestions_callback' => url('translate/suggestions/'. $language->language .'/'),
      'l10n_approve_callback' => url('translate/approve/'),
      'l10n_decline_callback' => url('translate/decline/'),
      'l10n_form_token_path'  => variable_get('clean_url', '0') ? '?form_token=' : '&form_token=',
      'l10n_num_plurals'      => $language->plurals
    ),
    'setting'
  );
  
  // Let the user submit the form.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => ($perm == L10N_PERM_SUGGEST) ? t('Save suggestions') : t('Save translations')
  );
  
  $form['#theme'] = 'l10n_community_translate_form';
  
  return $form;
}

/**
 * Save translations entered in the web form.
 */
function l10n_community_translate_form_submit($form, &$form_state) {
  global $user;

  $inserted = $updated = $unchanged = $suggested = $duplicates = 0;
  
  foreach ($form_state['values'] as $sid => $item) {
    if (!is_array($item) || !isset($item['translation'])) {
      // Skip, if we don't have translations in this form item,
      // which means this is some other form value.
      continue;
    }
    
    $text = '';
    if (is_string($item['translation']['value'])) {
      // Single string representation: simple translation.
      $text = $item['translation']['value'];
    }
    if (is_array($item['translation']['value'])) {
      // Array -> plural variants are provided. Join them with a NULL separator.
      $text = join("\0", $item['translation']['value']);
      if (trim($text) == '') {
        // If the whole string only contains NULL bytes, empty the string, so
        // we don't save an empty translation. Otherwise the NULL bytes need
        // to be there, so we know plural variant indices.
        $text = '';
      }
    }
      
    if (!empty($text)) {
      // Check for duplicate translation or suggestion.
      if (l10n_community_is_duplicate($text, $sid, $form_state['values']['langcode'])) {
        $duplicates++;
        continue;
      } 

      // We have some string to save.
      l10n_community_target_save(
        $sid, $text, $form_state['values']['langcode'], $user->uid,
        ($item['translation']['is_suggestion'] == TRUE),
        $inserted, $updated, $unchanged, $suggested
      );
    }
  }
  
  // Inform user about changes made to the database.
  l10n_community_update_message($inserted, $updated, $unchanged, $suggested, $duplicates);
}

// = Theme functions ===========================================================

/**
 * Theme function for l10n_community_filter_form.
 * Arranges elements in a condensed fashion in a table.
 */
function theme_l10n_community_filter_form($form) {
  $row = array(); // table content
  $labels = array(); // table header labels
  // Only display these elements in distinct table cells
  $elements = array('project', 'release', 'status', 'search');
  foreach ($form as $id => &$element) {
    if (in_array($id, $elements)) {
      $labels[] = $element['#title'];
      unset($element['#title']);
      $row[] = drupal_render($element);
    }
  }
  // Fill in the rest of the header above the buttons.
  $labels[] = '';
  // Display the rest of the form in the last cell
  $row[] = drupal_render($form);
  return theme('table', $labels, array($row), array('class' => 'l10n-server-translate'));
}

/**
 * Theme function for l10n_community_translate_form.
 */
function theme_l10n_community_translate_form($form) {
  $rows = array();
  $output = '';
  
  foreach ($form as $id => &$element) {
    // if the form id is numeric, this form element is for editing a string
    if (is_numeric($id)) {
      $source_pane = drupal_render($element['source']);
      $translation_pane = "<div class='l10n-panes clear-block'>";
      $translation_pane .= drupal_render($element['toolbox']);
      $translation_pane .= "<div class='pane translate'>";
      $translation_pane .= !empty($element['translation_existing']) ? drupal_render($element['translation_existing']) : '';
      $translation_pane .= drupal_render($element['messagebox']);
      $translation_pane .= drupal_render($element['translation']) .'</div>';
      $translation_pane .= "<div class='pane suggestions'></div><div class='pane lookup'></div>";
      $translation_pane .= "</div>";
      $row = array(
        array(
          'data' => $source_pane,
          'class' => 'source',
          'id' => 'spane-'. $id,
        ),
        array(
          'data' => $translation_pane,
          'class' => 'translation',
          'id' => 'tpane-'. $id,
        ),        
      );
      $rows[] = $row;
      unset($form[$id]);
    }
  }
  $output .= drupal_render($form['pager_top']);
  $output .= theme('table', array(t('Source Text'), t('Translations')), $rows, array('class' => 'l10n-server-translate'));
  $output .= drupal_render($form);
  return $output;
}

// = API functions =============================================================

/**
 * Get strings under some conditions.
 *
 * @param $langcode
 *   Language code to use for the lookup.
 * @param $project
 *   Project object to look up strings for.
 * @param $status
 *   Filter strings by status. See L10N_STATUS_ALL,
 *   L10N_STATUS_UNTRANSLATED, L10N_STATUS_HAS_SUGGESTION and
 *   L10N_STATUS_TRANSLATED.
 * @param $release
 *   Release id of the particular project release to filter with.
 *   Use NULL to not filter on releases.
 * @param $search
 *   Substring to search for in all source and translation strings.
 * @param $pager
 *   Number of strings to be returned in a pager. Should be NULL if
 *   no pager should be used.
 * @return
 *   An array of string records from database.
 */
function l10n_community_get_strings($langcode, $project = NULL, $status = L10N_STATUS_ALL, $release = NULL, $search = NULL, $pager = NULL) {
  
  $sql = $sql_count = '';
  $sql_args = array();
  
  if (!isset($project)) {
    // No project based filtering.
    $sql = "SELECT DISTINCT s.sid, s.value, t.tid, t.language, t.translation, t.uid_entered, t.uid_approved, t.time_entered, t.time_approved, t.has_suggestion, t.is_suggestion, t.is_active FROM {l10n_community_string} s LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid AND t.language = '%s' AND t.is_active = 1 AND t.is_suggestion = 0 WHERE";
    $sql_count = "SELECT COUNT(DISTINCT(s.sid)) FROM {l10n_community_string} s LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid AND t.language = '%s' AND t.is_active = 1 AND t.is_suggestion = 0 WHERE";
    $sql_args = array($langcode);
  }
  else {
    // Project based filtering and language based filtering built in.
    $sql = "SELECT DISTINCT s.sid, s.value, t.tid, t.language, t.translation, t.uid_entered, t.uid_approved, t.time_entered, t.time_approved, t.has_suggestion, t.is_suggestion, t.is_active FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid AND t.language = '%s' AND t.is_active = 1 AND t.is_suggestion = 0 WHERE r.pid = %d";
    $sql_count = "SELECT COUNT(DISTINCT(s.sid)) FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid AND t.language = '%s' AND t.is_active = 1 AND t.is_suggestion = 0 WHERE r.pid = %d";
    $sql_args = array($langcode, $project->pid);
  }

  if (!empty($search)) {
    // Search in the source or target strings.
    $sql_args[] = $search;
    $sql_args[] = $search;
    $search_sql = " AND (s.value LIKE '%%%s%%' OR t.translation LIKE '%%%s%%')";
    $sql .= $search_sql;
    $sql_count .= $search_sql;
  }

  if (isset($release) && $release != 'all') {
    // Release restriction.
    $sql_args[] = $release;
    $sql_args[] = $release;
    $release_sql = ' AND r.rid = %d';
    $sql .= $release_sql;
    $sql_count .= $release_sql;
  }
    
  // Restriction based on string status.
  switch ($status) {
    case L10N_STATUS_UNTRANSLATED:
      // We are doing a LEFT JOIN especially to look into the case, when we have nothing 
      // to match in the translation table, but we still have the string. (We get our 
      // records in the result set in this case). The translation field is empty or
      // NULL in this case, as we are not allowing NULL there and only saving an empty
      // translation if there are suggestions but no translation yet.
      $status_sql = " AND (t.translation is NULL OR t.translation = '')";
      $sql .= $status_sql;
      $sql_count .= $status_sql;
      break;
    case L10N_STATUS_TRANSLATED:
      $status_sql = " AND t.translation != ''";
      $sql .= $status_sql;
      $sql_count .= $status_sql;
      break;
    case L10N_STATUS_HAS_SUGGESTION:
      // Note that we are not searching in the suggestions themselfs, only
      // the source and active translation values. The user interface underlines
      // that we are  looking for strings which have suggestions, not the
      // suggestions themselfs.
      $status_sql = " AND t.has_suggestion = 1";
      $sql .= $status_sql;
      $sql_count .= $status_sql;
      break;
    case L10N_STATUS_ALL:
    default:
      // Leave query alone on this case.
      break;
  }
  
  // Morph "WHERE AND" to "WHERE" and remove closing "WHERE". The first appears
  // if there was no project filtering but further filtering was made. The
  // second appears if only language filtering was used.
  $sql = str_replace('WHERE AND', 'WHERE', preg_replace('!(WHERE$)!', '', $sql));
  $sql_count = str_replace('WHERE AND', 'WHERE', preg_replace('!(WHERE$)!', '', $sql_count));

  // We either need a pager or a full result.
  if (isset($pager)) {
    $strings = pager_query($sql, $pager, 0, $sql_count, $sql_args);
  }
  else {
    $strings = db_query($sql, $sql_args);
  }
  $result = array();
  while ($string = db_fetch_object($strings)) {
    $result[] = $string;
  }
  return $result;
}

/**
 * Builds a simple array with only the keys allowed for filtering.
 *
 * @param $input
 *   An array from $_GET or $form_state['values']
 * @param $allow_pager
 *   Allow page number to be kept in the array.
 * @return
 *  Filtered array with only the allowed keys and their values.
 *  The values are not checked for accuracy.
 */
function l10n_community_build_filter_array($input, $allow_pager = FALSE) {
  // Redirect keeping the relevant filters intact in the URL.
  $filter = array();
  $allowed_keys = array('search', 'project', 'release', 'status');
  if ($allow_pager) {
    $allowed_keys[] = 'page';
  }
  foreach ($input as $key => $value) {
    if (in_array($key, $allowed_keys)) {
      $filter[$key] = $value;
    }
  }
  return $filter;
}
