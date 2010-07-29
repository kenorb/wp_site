<?php
// $Id: l10n_drupalorg.admin.inc,v 1.1.2.1 2008/10/15 15:04:09 goba Exp $

/**
 * @file
 *   Administration pages for localization community for drupal.org.
 */

// = Settings ==================================================================

/**
 * Settings form callback.
 */
function l10n_drupalorg_settings_form() {
  $form = array();
  $form['l10n_drupalorg_limit'] = array(
    '#title' => t('Number of projects to scan at once'),
    '#description' => t('The number of projects to scan on a manual or cron run. Scanning is synchronous, so you need to wait for the download to complete and Drupal to extract and parse the tarball content. If everything goes well, detailed data about the project and its releases, files, and translatable strings will be inserted into the database and become available for translation.'),
    '#type' => 'select',
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 18, 20)),
    '#default_value' => variable_get('l10n_drupalorg_limit', 1),
  );
  $form['l10n_drupalorg_cron'] = array(
    '#title' => t('Scan new projects on every cron run'),
    '#type' => 'radios',
    '#options' => array(t('Disabled'), t('Enabled')),
    '#default_value' => variable_get('l10n_drupalorg_cron', 0),
  );
  return system_settings_form($form);
}

// = Project synchronization ===================================================

/**
 * Scans files of a project and release picked.
 *
 * @param $automated
 *   TRUE if the execution was automated, so user feedback should
 *   not be provided. FALSE otherwise.
 *
 * @todo
 *   Do less HTTP queries! Look at ARCHITECTURE.txt for more information.
 */
function l10n_drupalorg_scan($automated = FALSE) {
  include_once 'projects.inc';
  include_once drupal_get_path('module', 'l10n_community') .'/extractor.inc';
  
  // Always update full project list.
  l10n_drupalorg_sync_projects();
  
  // Pick the projects we did not update for the longest period and sync fresh release list.
  $result = db_query_range("SELECT * FROM {l10n_community_project} WHERE connector_module = 'l10n_drupalorg' AND status = 1 ORDER BY last_parsed ASC", 0, variable_get('l10n_drupalorg_limit', 1));
  while ($project = db_fetch_object($result)) {
    l10n_drupalorg_sync_releases($project);
    
    // Pick the release we did not update for the longest period, and sync the tarball.
    // We should only look for releases with downloads. Placeholder releases for later
    // versions should not be considered.
    $release = db_fetch_object(db_query_range("SELECT r.* FROM {l10n_community_project} p LEFT JOIN {l10n_community_release} r ON p.pid = r.pid WHERE p.pid = %d AND r.download_link != '' ORDER BY r.last_parsed ASC", $project->pid, 0, 1));
    $result = l10n_drupalorg_sync_files($release);
    
    if (!$automated) {
      // User feedback, if not automated. Log messages are already done.
      if (isset($result['error'])) {
        drupal_set_message($result['error'], 'error');
      }
      if (isset($result['message'])) {
        drupal_set_message($result['message']);
      }
    }
  }
  
  // Ensure that a Drupal page will be displayed with the messages.
  return '';
}
