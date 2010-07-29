<?php
// $Id: pages.inc,v 1.1.2.20.2.5 2008/12/29 16:02:21 goba Exp $

/**
 * @file
 *   Overview pages for localization community.
 */

// = Overview screens ==========================================================

/**
 * Translation status overview for all languages.
 */
function l10n_community_explore_languages() {
  
  // Checking whether we have languages and strings.
  if (!$languages = l10n_community_get_languages()) {
    drupal_set_message(t('No languages to list.'), 'error');
    return '';
  }
  if (!$num_source = l10n_community_get_string_count('all')) {
    drupal_set_message(t('No strings to translate.'), 'error');
    return '';
  }
  
  // Generate listing of all languages with summaries. The list of languages
  // is relatively "short", compared to projects, so we don't need a pager here.
  $groups = array();
  if (module_exists('l10n_groups')) {
    $groups = l10n_groups_get_groups();
  }
  $table = array();
  $string_counts = l10n_community_get_string_count('languages');
  foreach ($languages as $langcode => $language) {
    $perm = l10n_community_get_permission($langcode);
    if (empty($language->plurals)) {
      $table[] = array(
        array('data' => $language->name .' ('. $langcode .')', 'class' => 'rowhead'),
        array('data' => t('Uninitialized plural formula. Please <a href="@import-url">import a valid interface translation</a> for Drupal in this language.', array('@import-url' => url('admin/build/translate/import'))), 'colspan' => '3', 'class' => 'error')
      );
    }
    else {
      $table[] = array_merge(
        array(
          array('data' => l($language->name .' ('. $langcode .')', 'translate/languages/'. $langcode), 'class' => 'rowhead'),
        ),
        isset($string_counts[$langcode]) ? theme('l10n_community_progress_columns', $num_source, $string_counts[$langcode][0], $string_counts[$langcode][1]) : theme('l10n_community_progress_columns', $num_source, 0, 0)
      );
    }
  }
  $header = theme('l10n_community_progress_headers', t('Language'));
  return theme('l10n_community_table', $header, $table);
}

/**
 * Translation status overview for all projects.
 *
 * Because projects are much more numerous then languages, we need
 * a pager on this screen.
 */
function l10n_community_explore_projects() {
  $pager_setting = variable_get('l10n_community_project_per_page', 10);

  $output = '';
  if (!$projects = l10n_community_get_projects(NULL, $pager_setting)) {
    drupal_set_message(t('No projects found.'), 'error');
    return '';
  }
  $languages = l10n_community_get_languages('name');
  $language_count = count($languages);

  $output .= theme('pager', NULL, $pager_setting, 0);
  $table = array();
  $string_counts = l10n_community_get_string_count('projects');
  foreach ($projects as $project) {
    if (isset($string_counts[$project->pid])) {
      list($title, $uri, $sum, $translated, $suggested) = $string_counts[$project->pid];
      $table[] = array_merge(
        array(
          array('data' => l($project->title, 'translate/projects/'. $uri), 'class' => 'rowhead'), 
        ),
        // Multiply summary count by languages handled, so we get an
        // accurate picture of completeness.
        theme('l10n_community_progress_columns', ($sum * $language_count), $translated, $suggested)
      );
    }
  }
  $output .= theme(
    'l10n_community_table',
    theme('l10n_community_progress_headers', t('Project')),
    $table,
    array('id' => 'l10n-community-overview')
  );
  $output .= theme('pager', NULL, $pager_setting, 0);
  return $output;
}

/**
 * Translation status page of all projects from a given language.
 *
 * @param $langcode
 *   Language code, for example 'hu', 'pt-br', 'de', 'it'.
 */
function l10n_community_overview_language($langcode = NULL) {
  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore languages'), 'translate/languages')
    )
  );

  if (!isset($langcode)) {
    drupal_set_message(t('No language selected.'), 'error');
    return '';
  }
  
  $languages = l10n_community_get_languages('name');
  drupal_set_title(t('@language overview', array('@language' => $languages[$langcode])));
  
  $output = '<div id="l10n-community-summary" class="admin clear-block"><div class="left clear-block">';

  $has_posts = FALSE;
  if (module_exists('taxonomy') && ($term = taxonomy_get_term_by_name('l10n-server-'. $langcode))) {
    $result = taxonomy_select_nodes(array($term[0]->tid), 'or', 0, TRUE);
    while ($node = db_fetch_object($result)) {
      $has_posts = TRUE;
      $output .= node_view(node_load($node->nid), 1);
    }
  }
  if (!$has_posts) {
    $output .= '<p>'. t('There are currently no posts set for display here. Mark posts to display here with the %tagname tag, using the taxonomy module.', array('%tagname' => 'l10n-server-'. $langcode)) .'</p>';
  }

  $output .= '</div><div class="right clear-block">';

  $group_help = '';
  if (module_exists('l10n_groups')) {
    // A little extra message if the groups module is enabled.
    $groups = l10n_groups_get_groups();
    if (isset($groups[$langcode])) {
      $group_help = '<p>'. t('The <a href="@group">%language group</a> pages might provide you with more information, translation suggestions and guidelines.', array('@group' => url('node/'. $groups[$langcode]->nid), '%language' => $languages[$langcode])) .'</p>';
    }
  }
  $block = array(
    'title' => t('Contribute'),
    'description' => t('Review, translate, import'),
    'content' => '<p class="info">'. t('Different tabs allow you to view existing translations, suggest new ones, import translations completed offline or export translations for import to Drupal or offline work.') .'</p>'. $group_help
  );
  $output .= str_replace('class="admin-panel"', 'class="admin-panel admin-panel-contribute"', theme('admin_block', $block));

  $stats_numbers = l10n_community_get_stats($langcode);

  $block = array(
    'title' => t('Progress status'),
    'description' => t('Status overview'),
    'content' => theme('item_list', array(
      format_plural($stats_numbers['users'], '1 contributor', '@count contributors'),
      format_plural($stats_numbers['strings'], '1 string to translate', '@count strings to translate'),
      format_plural($stats_numbers['translations'], '1 translation recorded', '@count translations recorded'),
      format_plural($stats_numbers['suggestions'], '1 suggestion awaiting approval', '@count suggestions awaiting approval'),
    ))
  );
  $output .= theme('admin_block', $block);

  $people = l10n_community_get_string_count('top-people', $langcode);
  $block = array(
    'title' => t('Top contributors'),
    'description' => t('People with most approved translations'),
  );
  $list = array();
  foreach ($people as $translator) {
    // @todo: This can be integrated into format_plural in D6.
    $list[] = str_replace('!name', l($translator->name, 'user/'. $translator->uid), format_plural($translator->sum, '!name - 1 translation', '!name - @count translations'));
  }
  if ($list) {
    $block['content'] = theme('item_list', $list);
  }
  else {
    $block['content'] = '<p>'. t('Nobody contributed to this translation yet.') .'</p>';
  }
  $output .= theme('admin_block', $block);


  $output .= '</div></div>';
  
  return $output;
}

/**
 * Translation status page of all languages for a given project.
 *
 * @param $uri
 *   Project URI.
 */
function l10n_community_overview_project($uri = NULL) {

  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate'), 'translate'),
      l(t('Explore projects'), 'translate/projects')
    )
  );

  if (!isset($uri)) {
    drupal_set_message(t('No project selected.'), 'error');
    return '';
  }
  drupal_set_title(t('@project project translations', array('@project' => drupal_get_title())));
  
  if (!$languages = l10n_community_get_languages()) {
    drupal_set_message(t('No languages to list.'), 'error');
    return '';
  }

  $output = '<div id="l10n-community-summary" class="admin clear-block"><div class="left clear-block">';

  $block = array(
    'title' => t('Translations overview'),
    'description' => t('Overall status of translations'),
  );
  $project = l10n_community_get_projects($uri);
  $num_source = l10n_community_get_string_count('project', $project->pid);
  $string_counts = l10n_community_get_string_count('languages', $project->pid);

  $table = array();
  foreach ($languages as $langcode => $language) {
    $perm = l10n_community_get_permission($langcode);
    if (empty($language->plurals)) {
      $table[] = array(
        array('data' => $language->name .' ('. $langcode .')', 'class' => 'rowhead'),
        array('data' => t('Uninitialized plural formula. Please <a href="@import-url">import a valid interface translation</a> for Drupal in this language.', array('@import-url' => url('admin/build/translate/import'))), 'colspan' => '3', 'class' => 'error')
      );
    }
    else {
      $table[] = array_merge(
        array(
          array('data' => l($language->name .' ('. $langcode .')', 'translate/languages/'. $langcode .'/view', array(), 'project='. $uri), 'class' => 'rowhead'),
        ),
        theme(
          'l10n_community_progress_columns',
          $num_source,
          !empty($string_counts[$langcode][0]) ? $string_counts[$langcode][0] : 0,
          !empty($string_counts[$langcode][1]) ? $string_counts[$langcode][1] : 0
        )
      );
    }
  }
  $block['content'] = theme(
    'table',
    theme('l10n_community_progress_headers', t('Language')),
    $table,
    array('id' => 'l10n-community-overview')
  ) .'<div class="clearing"></div>';
  $output .=  theme('admin_block', $block);

  $output .= '</div><div class="right clear-block">';

  $block = array(
    'title' => t('Contribute'),
    'description' => t('Export, translate, review'),
    'content' => '<p class="info">'. t('Select a language from the overview to review translations and contribute to the translation efforts. The export tab allows exporting of translation templates, but translations can also be exported on the language pages.') .'</p>'
  );
  $output .= str_replace('class="admin-panel"', 'class="admin-panel admin-panel-contribute"', theme('admin_block', $block));

  // Get information on releases, and the number of parsed ones.
  $releases = l10n_community_get_releases($uri);
  $num_parsed = 0;
  foreach ($releases as $release) {
    if ($release->last_parsed > 0) {
      $num_parsed++;
    }
  }
  
  // Get information on errors.
  $num_errors = db_result(db_query("SELECT COUNT(DISTINCT e.eid) FROM {l10n_community_project} p LEFT JOIN {l10n_community_release} r ON p.pid = r.pid LEFT JOIN {l10n_community_error} e ON r.rid = e.rid WHERE p.uri = '%s'", $uri));
  
  $block = array(
    'title' => t('@project summary', array('@project' => $project->title)),
    'description' => t('Some details we know about this project'),
    'content' => theme('item_list', array(
      t('Project home') .': '. (!empty($project->home_link) ? ('<a href="'. check_url($project->home_link) .'">'. check_plain($project->home_link) .'</a>') : t('Not available')),
      format_plural(count($releases), '1 release in total', '@count releases in total'),
      format_plural($num_parsed, '1 release parsed', '@count releases parsed'),
      format_plural($num_source, '1 source string in total', '@count source strings in total'),
      ($num_errors == 0 ? t('No marker error found') : format_plural($num_errors, '1 marker error found', '@count marker errors found')),
    ))
  );
  $output .= theme('admin_block', $block);
  $output .= '</div></div>';
  return $output;
}

/**
 * Displays a page with marker errors for a given project.
 *
 * @param $uri
 *   Project URI.
 */
function l10n_community_project_errors($uri = NULL) {
  if (!isset($uri)) {
    drupal_set_message(t('No project selected.'), 'error');
    return '';
  }
  drupal_set_title(t('@project project errors', array('@project' => drupal_get_title())));
  
  if (!$errors = l10n_community_get_errors($uri)) {
    drupal_set_message(t('No marker errors found in any of the parsed releases of this project.'));
    return '';
  }
  
  $output = '';
  $releases = l10n_community_get_releases($uri);  
  foreach ($releases as $release) {
    $output .= '<h3>'. $release->title .'</h3>';
    if (!isset($errors[$release->rid])) {
      $output .= '<p>'. t('No errors found in this release.') .'</p>';
    }
    else {
      $output .= theme('item_list', $errors[$release->rid]);
    }
  }
  return $output;
}


// = Theme functions ===========================================================

/**
 * Progress bar and table column display for translation status information.
 *
 * @param $sum
 *   The number of all translatable strings to count with.
 * @param $translated
 *   Number of strings translated (and without outstanding
 *   suggestions) out of the $sum.
 * @param $has_suggestion
 *   Number of strings which are not translated yet (or are
 *   translated) and have at least one suggestion attached.
 * @return
 *   An indexed array with four elements, first being the progress bar,
 *   and the other three being calculated and percentages.
 */
function theme_l10n_community_progress_columns($sum, $translated, $has_suggestion) {
  if ($sum == 0) {
    // Handle (sort of debug) special case when we handle a project without any
    // source strings available for translation.
    return array(
      t('n/a'),
      t('No data available.'),
    );
  }

  // Compute numbers, percentages and provide alternate text titles.
  $done_percent = round($translated / $sum * 100, 2);
  $status = array(
    'translated'     => array((int) $translated, $done_percent, t('!percent translated')),
    'untranslated'   => array($sum - $translated, 100 - $done_percent, t('!percent untranslated')),
    // suggestions are not in the bar as they overlap with both translated and
    // untranslated strings, so we cannot display them here.
  );
  
  // Visual summary with a progress bar.
  $bar = '<div class="l10n-community-progress">';
  foreach ($status as $key => $values) {
    if ($values[1] > 0) {
      $bar .= '<div class="l10n-community-progress-'. $key .'" style="width:'. $values[1] .'%;" title="'. strtr($values[2], array('!percent' => $values[1] .'%')) .'"></div>';
    }
  }
  $bar .= '</div>';
  
  return array(
    $bar,
    ($sum - $translated),
    (int) $has_suggestion
    //t('@num untranslated', array('@num' => ($sum - $translated))),
    //($has_suggestion ? format_plural($has_suggestion, '@count has suggestions', '@count have suggestions') : t('no suggestions'))
  );
}

/**
 * Header columns for the progress data.
 */
function theme_l10n_community_progress_headers($mainhead) {
  return array(
    array('data' => $mainhead, 'class' => 'rowhead'),
    t('Overall status'),
    t('Untranslated'),
    t('Suggestions'),
  );
}

/**
 * Generate a summary table for l10n_community screens.
 *
 * @param $headers
 *   An array of table headers.
 * @param $table
 *   An array of table rows.
 */
function theme_l10n_community_table($header, $table) {
  if (($row_count = count($table)) > 1) {
    $output = theme('table', $header, array_slice($table, 0, ceil($row_count/2)), array('id' => 'l10n-community-overview'));
    $output .= theme('table', $header, array_slice($table, ceil($row_count/2)), array('id' => 'l10n-community-overview'));
    return $output;
  }
  else {
    return theme('table', $header, $table, array('id' => 'l10n-community-overview'));
  }
}

// = API functions =============================================================

/**
 * Get string counts for summaries.
 *
 * @param $type
 *   Type of string count to return:
 *     - all:        count of all strings
 *     - project:    count of strings in one project (identified by $id)
 *     - languages:  array of the count of translated strings and suggestions by langcode
 *     - projects:   array of the count of translated strings and suggestions by pid
 *     - top-people: array of counts for top translators for a given language
 * @param $id
 *   The project id when the 'project' type is used. The result set can be
 *   restricted by an ID from the oppposing type for the later two types.
 *   Eg. 'projects' summaries can be restricted to one specific language, or
 *   'languages' summeries can be restricted to one specific project. This
 *   id represents the identifier (pid or langcode) of the restricting item.
 *   For the 'all' type, this value is discarded.
 *
 * @todo
 *   These queries are *slooow*. The query cache helps a lot with caching the
 *   result, so the slowness only shows for the first run, but still it would
 *   be good to look into optimizing these.
 */
function l10n_community_get_string_count($type, $id = NULL) {
  switch ($type) {
    case 'all':
      // Return a count of all strings.
      return db_result(db_query("SELECT COUNT(sid) FROM {l10n_community_string}"));
    
    case 'project':
      // Return a count of all strings in this project, id required.
      return db_result(db_query('SELECT COUNT(DISTINCT s.sid) FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid WHERE r.pid = %d', $id));
    
    case 'languages':
      // Summeries based on language codes, restricted to a specific project if $id is set.
      $sums = array();
      if (!isset($id)) {
        // Simple count query if we are not filtering by project.
        $count_sql = "SELECT COUNT(DISTINCT sid) AS translation_count, language, is_suggestion FROM {l10n_community_translation} WHERE is_active = 1 AND translation != '' GROUP BY language, is_suggestion";
      }
      else {
        // Rather complex join if we also need to factor the project in.
        $count_sql = "SELECT COUNT(DISTINCT t.sid) AS translation_count, t.language, t.is_suggestion FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid WHERE r.pid = %d AND t.is_active = 1 AND t.translation != '' GROUP BY t.language, t.is_suggestion";
      }
      $result = db_query($count_sql, $id);
      while ($row = db_fetch_object($result)) {
        if (!isset($sums[$row->language])) {
          // Set default zeroes for summaries.
          $sums[$row->language] = array(0, 0);
        }
        // Fill in the 0 or 1 element based on whether we have a result
        // for suggestions or translations.
        $sums[$row->language][(int) $row->is_suggestion] = $row->translation_count;
      }
      return $sums;
      break;
    
    case 'projects':
      // Get summaries by projects. Restricted to a specific language, if $id is set.
      
      // First get the count of strings available for translation.
      $sums = $count_args = array();
      $result = db_query("SELECT COUNT(DISTINCT s.sid) AS string_count, p.pid, p.title, p.uri FROM {l10n_community_project} p INNER JOIN {l10n_community_release} r ON p.pid = r.pid INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid GROUP BY p.pid;");
      while ($row = db_fetch_object($result)) {
        // Initialize remaining fields to zeroes too.
        $sums[$row->pid] = array($row->title, $row->uri, $row->string_count, 0, 0);
      }
      // Get the count of distinct strings translated and suggestions per project.
      $count_sql = "SELECT COUNT(DISTINCT t.sid) AS translation_count, r.pid, t.is_suggestion FROM {l10n_community_release} r INNER JOIN {l10n_community_file} f ON r.rid = f.rid INNER JOIN {l10n_community_line} l ON f.fid = l.fid INNER JOIN {l10n_community_string} s ON l.sid = s.sid LEFT JOIN {l10n_community_translation} t ON s.sid = t.sid WHERE t.is_active = 1 AND t.translation != '' ";
      if (isset($id)) {
        // Limit to language if desired.
        $count_sql .= "AND t.language = '%s' ";
        $count_args[] = $id;
      }
      $count_sql .= 'GROUP BY r.pid, t.is_suggestion ';
      $result = db_query($count_sql, $count_args);
      while ($row = db_fetch_object($result)) {
        // Fill up the zero spots we added above with real data.
        $sums[$row->pid][((int) $row->is_suggestion) + 3] = $row->translation_count;
      }
      return $sums;

    case 'top-people':
      // Get summaries of people having most active translations per language.
      $result = db_query_range("SELECT COUNT(DISTINCT t.sid) AS sum, u.name, u.uid FROM {l10n_community_translation} t LEFT JOIN {users} u ON t.uid_entered = u.uid WHERE t.is_active = 1 AND t.is_suggestion = 0 AND t.language = '%s' GROUP BY t.uid_entered ORDER BY sum DESC", $id, 0, 10);
      $accounts = array();
      while ($account = db_fetch_object($result)) {
        $accounts[] = $account;
      }
      return $accounts;
  }
}
