<?php
// $Id: welcome.inc,v 1.1.2.7.2.3 2008/12/29 16:02:22 goba Exp $

/**
 * @file
 *   Introduction page for an l10n_community setup.
 */

/**
 * Introduction page for an l10n_community setup.
 *
 * All string are in PHP source code, so translators can translate
 * them to their own language, without resorting to i18n module or
 * some other solution for node translation.
 *
 * This page is based on the admin page layout from system.module,
 * so we are using classes and concepts from there. This might not
 * fit perfectly with all listings, so I bended some stuff more to comply.
 */
function l10n_community_welcome_page() {
  $output = '<div id="l10n-community-welcome" class="admin clear-block"><div class="left clear-block">';

  $block = array(
    'title' => t('About this interface'),
    'description' => t('Community for Drupal translations'),
    'content' => '<p class="info">'. t('This interface serves as a community hub for translating Drupal projects, so translators only need to use a browser to contribute. Our system knows about Drupal projects, such as modules, themes and install profiles; with their respective releases. This means that all text added to these packages are translatable. New projects and releases are scanned regularly. Languages are maintained by translation groups. Every group has a community space, where members can share information, discuss specific questions and document best practices. Finally, translations are packaged on drupal.org for each project release.') .'</p>'
  );
  $output .= theme('admin_block', $block);

  $stats = array();
  if (module_exists('l10n_groups')) {
    $groups = db_result(db_query('SELECT COUNT(*) FROM {l10n_groups_group}'));
    $stats = array(
      format_plural($groups, '1 translation group', '@count translation groups'),
    );
  }
  $stats_numbers = l10n_community_get_stats();
  $block = array(
    'title' => t('Quick stats'),
    'description' => t('Some facts about this community'),
    'content' => theme('item_list', array_merge($stats, array(
      format_plural($stats_numbers['users'], '1 contributor', '@count contributors'),
      format_plural($stats_numbers['projects'], '1 project managed', '@count projects managed'),
      format_plural($stats_numbers['files'], '1 file scanned', '@count files scanned'),
      format_plural($stats_numbers['strings'], '1 string to translate', '@count strings to translate'),
      format_plural($stats_numbers['translations'], '1 translation recorded', '@count translations recorded'),
      format_plural($stats_numbers['suggestions'], '1 suggestion awaiting approval', '@count suggestions awaiting approval'),
    ))),
  );
  $output .= theme('admin_block', $block);

  $output .= '</div><div class="right clear-block">';
  $content = '';
  if (!$GLOBALS['user']->uid) {
    // Only display account creation task if not logged in.
    $content = '<p>'. t('You can view the work being done here, but to participate, first you need to <a href="@register">create an account and/or log in</a>', array('@register' => url('user'))) .'</p>';
  }
  
  $content .= drupal_get_form('l10n_community_pick_go') .'<div class="clearing"></div>';

  $block = array(
    'title' => t('Contribute'),
    'description' => t('Suggest, translate, review'),
    'content' => '<dl class="admin-list">'. $content .'</dl>'
  );
  $output .= str_replace('class="admin-panel"', 'class="admin-panel admin-panel-contribute"', theme('admin_block', $block));

  $items = array(
    array('title' => t('Translator\'s guide'), 'path' => 'http://drupal.org/translators', 'description' => t('Documentation for translators on drupal.org.')),
    array('title' => t('Translations group'), 'path' => 'http://groups.drupal.org/translations', 'description' => t('Translations group at groups.drupal.org.')),
    array('title' => t('Mailing list'), 'path' => 'http://lists.drupal.org/listinfo/translations', 'description' => t('Translators mailing list hosted at drupal.org.')),
  );
  $content = '';
  foreach ($items as $item) {
    $content .= '<dt>'. l($item['title'], $item['path']) .'</dt>';
    $content .= '<dd>'. $item['description'] .'</dd>';
  }
  $block = array(
    'title' => t('Handy links'),
    'description' => t('Documentation, discussion spaces'),
    'content' => '<dl class="admin-list">'. $content .'</dl>'
  );
  $output .= theme('admin_block', $block);
  $output .= '</div></div>';
  return $output;
}

function l10n_community_pick_go() {
  $form = array();
  $languages = l10n_community_get_languages();
  $projects = l10n_community_get_projects();
  
  // Drop languages without a plural formula from the options.
  $language_list = array();
  foreach ($languages as $langcode => $language) {
    if (!empty($language->plurals)) {
      $language_list[$langcode] = $language->name;
    }
  }

  if (!count($language_list) || !count($projects)) {
    $form['dummy'] = array('#type' => 'markup', '#value' => t('No languages with plural formulas defined or no projects scanned yet.'));
    return $form;
  }
  
  if (count($language_list) > 1) {
    // If more then one language.
    $form['langcode'] = array(
      // For three or less languages, display radio buttons, select otherwise.
      '#type' => (count($language_list) <= 3) ? 'radios' : 'select',
      '#title' => t('Pick a language'),
      '#options' => array('' => t('All')) + $language_list,
      '#description' => t('Alternatively you can <a href="@explore">explore languages</a>.', array('@explore' => url('translate/languages')))
    );
  }
  else {
    // Only one language, store as form value.
    list($language_code, $language_name) = each($language_list);
    $form['langcode'] = array(
      '#type' => 'value',
      '#value' => $language_code
    );
  }
  $form['project'] = array(
    '#title' => (count($language_list) > 1) ? t('And/or pick a project') : t('Pick a project for @language translation', array('@language' => $language_name)),
    '#description' => t('Alternatively you can <a href="@explore">explore projects</a>.', array('@explore' => url('translate/projects')))
  );
  if (($count = count($projects)) <= 30) {
    // Radio box widget for as much as 5 projects, select widget for 5-30 projects.
    $form['project']['#type'] = ($count <= 5 ? 'radios' : 'select');
    $form['project']['#options'] = array('' => t('All'));
    foreach ($projects as $project) {
      // title used to conform to the autocomplete behavior.
      $form['project']['#options'][$project->title] = $project->title;
    }
  }
  else {
    // Autocomplete field for more then 30 projects.
    $form['project'] += array(
      '#type' => 'textfield',
      '#autocomplete_path' => 'translate/projects/autocomplete',
    );
  }
  
  // Either we have languages or projects, so we can jump.
  $form['submit'] = array(
    '#type' => 'submit',
    '#prefix' => '<div class="clearing"></div>',
    '#value' => t('Choose'),
  );
  return $form;
}

function l10n_community_pick_go_validate($form, &$form_state) {
  if (empty($form_state['values']['project']) && empty($form_state['values']['langcode'])) {
    // We require at least one to be selected to be effective.
    form_set_error('langcode', t('Select a language and/or a project.'));
  }
}

/**
 * Submission handler for l10n_community_pick_go().
 *
 * Redirects the user to the selected project page with language selection
 * if both are provided, or to the project page with project only or the
 * language page with language only.
 */
function l10n_community_pick_go_submit($form, &$form_state) {
  if (!empty($form_state['values']['project']) && ($uri = l10n_community_project_uri_by_title($form_state['values']['project']))) {
    // Project surely selected, possibly with language too.
    if (!empty($form_state['values']['langcode'])) {
      drupal_goto('translate/languages/'. $form_state['values']['langcode'], 'project='. $uri);
    }
    else {
      drupal_goto('translate/projects/'. $uri);
    }
  }
  elseif (!empty($form_state['values']['langcode'])) {
    // Only language selected.
    drupal_goto('translate/languages/'. $form_state['values']['langcode']);
  }
}
