<?php
// $Id: l10n_community.admin.inc,v 1.1.2.2 2008/10/15 23:56:38 goba Exp $

/**
 * @file
 *   Administrative functions for the Localization Server.
 */

define('L10N_COMMUNITY_ADMIN_PROJECTS_PER_PAGE', 30);

// = Admin index page for the l10n_* family ====================================

/**
 * This is the same admin block display function as in system.module.
 *
 * We need to write this wrapper code so that the right include file is
 * loaded to run that function.
 */
function l10n_community_settings_overview() {
  include_once drupal_get_path('module', 'system') .'/system.admin.inc';
  return system_admin_menu_block_page();
}

// = Settings form callback ====================================================

/**
 * Settings form callback.
 */
function l10n_community_settings_form() {
  $form = array();
  $form['l10n_community_project_per_page'] = array(
    '#title' => t('Number of projects to display per page'),
    '#description' => t('Number of projects displayed per page in overviews. It is not advised to change this setting on live sites, as it changes what is displayed on each page, so links will break to existing project listings.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(4, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40)),
    '#default_value' => variable_get('l10n_community_project_per_page', 10),
  );
  return system_settings_form($form);
}

// = Project list administration ===============================================

/**
 * Menu callback for projects administration listing.
 */
function l10n_community_admin_projects() {
  if (!$projects = l10n_community_get_projects(NULL, L10N_COMMUNITY_ADMIN_PROJECTS_PER_PAGE)) {
    drupal_set_message(t('No projects found.'), 'error');
    return '';
  }
  return drupal_get_form('l10n_community_admin_projects_form', $projects);
}

/**
 * Form builder for projects administration listing.
 */
function l10n_community_admin_projects_form(&$form_state, $projects) {
  $form = array();
  $form['pager'] = array(
    '#value' => theme('pager', NULL, L10N_COMMUNITY_ADMIN_PROJECTS_PER_PAGE, 0),
  );
  $form['orphans'] = array(
    '#type' => 'value',
    '#value' => ($orphans = l10n_community_orphan_strings()),
  );
  $form['projects'] = array(
    '#tree' => TRUE,
  );
  
  $table = array();
  foreach ($projects as $project) {
    $form['projects'][$project->pid] = array(
      'title' => array(
        '#value' => $project->title .'<br /><small>'. (!empty($project->home_link) ? '<a href="'. $project->home_link .'">'. $project->home_link .'</a>' : t('No link available')) .'</small>',
      ),
      'status' => array(
        '#type' => 'checkbox',
        '#default_value' => $project->status,
      ),
      'reset' => array(
        '#value' => l(t('Start over'), 'admin/l10n_server/projects/reset/'. $project->uri),
      ),
      'delete' => array(
        '#value' => $orphans ? '' : l(t('Delete'), 'admin/l10n_server/projects/delete/'. $project->uri),
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes')
  );
  
  $form['#theme'] = 'l10n_community_admin_projects_form';
  return $form;
}

/**
 * Form theme for projects administration listing.
 */
function theme_l10n_community_admin_projects_form($form) {
  $output = ($pager = drupal_render($form['pager']));
  $table = array();
  foreach ($form['projects'] as $pid => &$project_element) {
    if (!is_int($pid)) {
      // Skip non-project elements.
      continue;
    }
    $table[] = array(
      drupal_render($project_element['title']),
      drupal_render($project_element['status']),
      drupal_render($project_element['reset']),
      drupal_render($project_element['delete'])
    );
  }
  $headers = array(t('Project'), t('Enabled'), array('data' => t('Operations'), 'colspan' => 2));
  $output .= theme('table', $headers, $table);
  $output .= $pager;
  $output .= drupal_render($form);
  
  if ($form['orphans']['#value']) {
    drupal_set_message(format_plural($form['orphans']['#value'], 'There is @count source string orphaned.', 'There are @count source strings orphaned.') .' '. t('Deleting projects will not be available until you <a href="@cleanup">clean up the database</a> or rescan projects (within connectors you use) if you have started over with some of them.', array('@cleanup' => url('admin/l10n_server/projects/cleanup'))));
  }
  return $output;
}

/**
 * Submission handler for l10n_community_admin_projects_form().
 */
function l10n_community_admin_projects_form_submit($form, &$form_state) {
  $disabled = array();
  foreach ($form_state['values']['projects'] as $pid => $project) {
    db_query('UPDATE {l10n_community_project} SET status = %d WHERE pid = %d', $project['status'], $pid);
  }
  drupal_set_message(t('Localization Server project changes saved.'));
}

// = Delete a project from the database ========================================

/**
 * Form callback to display a confirm form for deleting projects.
 */
function l10n_community_admin_projects_delete(&$form_state, $project_uri) {
  $project = l10n_community_get_projects($project_uri);
  $form = array(
    'project' => array(
      '#type' => 'value',
      '#value' => $project,
    ),
  );
  return confirm_form($form, t('Are you sure you would like to delete project %project from the Localization Server?', array('%project' => $project->title)), 'admin/l10n_server/projects', t('This would remove all data associated with the project, including releases, files, parsing errors, and all source strings and translations which are not applicable to remaining projects. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for deleting projects.
 */
function l10n_community_admin_projects_delete_submit($form, &$form_state) {
  l10n_community_delete_project($form_state['values']['project']->pid);
  drupal_set_message(t('The %project was deleted from the Localization Server.', array('%project' => $form_state['values']['project']->title)));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = Reset a project in the database ===========================================

/**
 * Form callback to display a confirm form for resetting projects.
 */
function l10n_community_admin_projects_reset(&$form_state, $project_uri) {
  $project = l10n_community_get_projects($project_uri);
  $form = array(
    'project' => array(
      '#type' => 'value',
      '#value' => $project,
    ),
  );
  return confirm_form($form, t('Are you sure you would like to start over with the project %project from the Localization Server?', array('%project' => $project->title)), 'admin/l10n_server/projects', t('This would remove almost all data associated with the project: releases, files, line and parsing error information. Source strings found within this project and translations for them are temporarily kept. If you clean up the database before this project is parsed again, source strings and translations will be lost. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for resetting projects.
 */
function l10n_community_admin_projects_reset_submit($form, &$form_state) {
  l10n_community_delete_project($form_state['values']['project']->pid, TRUE);
  drupal_set_message(t('Project data for %project was deleted from the Localization Server. Make sure to parse this project again before deleting a project or cleaning up the database, or you will loose existing translation data.', array('%project' => $form_state['values']['project']->title)));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = Source strings cleanup ====================================================

function l10n_community_admin_projects_cleanup() {
  if ($orphans = l10n_community_orphan_strings()) {
    return drupal_get_form('l10n_community_admin_projects_cleanup_form', $orphans);
  }
  drupal_set_message(t('You have no orphan source strings in your database to clean up.'));
  return '';
}

/**
 * Form callback to display a confirm form for cleaning up strings.
 */
function l10n_community_admin_projects_cleanup_form(&$form_state, $orphans) {
  return confirm_form(array(), format_plural($orphans, 'Are you sure you would like to clean up @count orphan?', 'Are you sure you would like to clean up @count orphans?'), 'admin/l10n_server/projects', t('There is at least one orphan source string in your database. Orphan source strings are not tied to any release of a project managed on your Localization Server. It might be safe to remove these if you are not in the middle of rescanning some projects. Always make database backups. This action cannot be undone.'));
}

/**
 * Form submission callback for cleaning up strings.
 */
function l10n_community_admin_projects_cleanup_form_submit($form, &$form_state) {
  l10n_community_delete_orphans();
  drupal_set_message(t('Orphan strings and their translations were removed.'));
  $form_state['redirect'] = 'admin/l10n_server/projects';
}

// = API functions =============================================================

/**
 * API function to delete or "reset" projects.
 *
 * Deleting removes all data associated with the project and cleans up any
 * orphan strings in the database. Resetting a project almost the same, but
 * orphaned strings are purposefully kept.
 *
 * @param $pid
 *   Project identifier.
 * @param $skip_strings
 *   Boolean. If TRUE, source strings and translations are kept. Used for the
 *   "Start over" feature which lets regeneration of project data without
 *   loosing the actual translations.
 */
function l10n_community_delete_project($pid, $skip_strings = FALSE) {

  // Drop errors stored for releases of this project.
  db_query(
    'DELETE e FROM {l10n_community_error} e
     LEFT JOIN {l10n_community_release} r ON e.rid = r.rid
     WHERE r.pid = %d',
    $pid
  );

  // Cascade delete all related entries in project, release, file and line.
  db_query(
    'DELETE p, r, f, l FROM {l10n_community_project} p
     INNER JOIN {l10n_community_release} r ON p.pid = r.pid
     INNER JOIN {l10n_community_file} f ON r.rid = f.rid
     INNER JOIN {l10n_community_line} l ON f.fid = l.fid
     WHERE p.pid = %d',
    $pid
  );

  if (!$skip_strings) {
    l10n_community_delete_orphans();
  }
}

/**
 * Delete all orphan source strings and their associated translations.
 */
function l10n_community_delete_orphans() {
  // Drop all stale source strings and translations.
  db_query(
    'DELETE s, t FROM {l10n_community_string} s
     LEFT JOIN {l10n_community_translation} t ON t.sid = s.sid
     WHERE s.sid NOT IN (SELECT sid FROM {l10n_community_line})'
  );
}

/**
 * Detect the count of orphan strings in the database, if any.
 */
function l10n_community_orphan_strings() {
  return db_result(db_query('SELECT COUNT(*) FROM {l10n_community_string} s LEFT JOIN {l10n_community_line} l ON s.sid = l.sid WHERE l.sid IS NULL'));
}
