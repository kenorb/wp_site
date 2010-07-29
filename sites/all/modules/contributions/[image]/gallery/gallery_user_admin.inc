<?php
// $Id: gallery_user_admin.inc,v 1.3.2.4 2009/01/25 14:21:32 profix898 Exp $

require_once(drupal_get_path('module', 'gallery') .'/gallery_user.inc');

/**
 * gallery.module : gallery_user_admin.inc
 * Gallery User Administration
 */
 
define('GALLERY_BATCH_INTERVAL', 10);

/**
 * Function _gallery_user_users().
 * (gallery users page - view list of users)
 */
function _gallery_user_users() {
  // Generate user status overview
  $header = array(
    array('data' => t('ID'), 'field' => 'u.uid', 'sort' => 'asc'),
    array('data' => t('G2ID')),
    array('data' => t('Username'), 'field' => 'u.name'),
    array('data' => t('Status'), 'field' => 'u.status'),
    array('data' => t('Sync Status')),
    t('Operations')
  );
  $query = 'SELECT u.uid, u.name, u.status FROM {users} u WHERE uid <> 0';

  $status = array(t('blocked'), t('active'));
  $destination = drupal_get_destination();
  $filter = isset($_SESSION['gallery_user_filter']) ? $_SESSION['gallery_user_filter'] : FALSE;

  if (!_gallery_init(TRUE)) {
    return '';
  }
  list($ret, $g2_admin) = GalleryCoreApi::isUserInSiteAdminGroup();
  if ($ret) {
    gallery_error(t('Error calling \'GalleryCoreApi::isUserInSiteAdminGroup\'.'), $ret);
  }
  
  if ($filter) {
    $result = db_query($query);
  }
  else {
    $query .= tablesort_sql($header);
    $result = pager_query($query, 50);
  }
  
  $rows = array();
  while ($user = db_fetch_object($result)) {
    $g2_userinfo = gallery_user_map_info(user_load(array('uid' => $user->uid)), FALSE);
    
    $g2_id = ($g2_userinfo['g2_id'] >= 0) ? $g2_userinfo['g2_id'] : t('N/A');

    $operations = array(l(t('edit'), "user/$user->uid/edit", array(), $destination));

    if ($g2_admin && ($g2_userinfo['g2_id'] > 0)) {
      $link_url = gallery_generate_url(array('view' => 'core.SiteAdmin',
                                               'subView' => 'core.AdminEditUser',
                                               'userId' => $g2_userinfo['g2_id']), FALSE);
      $operations[] = l(t('edit G2'), $link_url);
    }

    if (count($g2_userinfo['status'])) {
      $operations[] = l(t('sync'), 'admin/user/gallery/users/sync/'. $user->uid, array(), drupal_get_destination());
    }
    
    if ($filter) {
      if ($filter == GALLERY_USERINFO_ERROR) {
        if (!count($g2_userinfo['status'])) {
          continue;
        }
      }
      elseif (!in_array($filter, $g2_userinfo['status'])) {
        continue;
      }    
    }

    $rows[] = array($user->uid,
                    $g2_id,
                    theme_username($user),
                    $status[$user->status],
                    implode(',<br />', gallery_user_map_info_status($g2_userinfo['status'])),
                    implode(' | ', $operations));
  }
  
  if ($filter && !count($rows)) {
    $rows[] = array(array('data' => t('There are no users with the selected status.'), 'colspan' => '6', 'align' => 'center', 'class' => 'message'));
  }
  
  $output  = drupal_get_form('_gallery_user_filter_form', $filter);
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', array(), 50);

  GalleryEmbed::done();
  return $output;
}

/**
 * Function _gallery_user_filter_form().
 * (filter form for user status)
 */
function _gallery_user_filter_form($form_state, $filter) {
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter by status'),
    '#collapsible' => TRUE,
    '#collapsed' => !$filter,
  );
  $filter_options = gallery_user_map_info_status(array(), FALSE);
  unset($filter_options[GALLERY_USERINFO_NOERROR]);
  $form['filter']['filter_status'] = array(
    '#type' => 'select',
    '#title' => t('Show only users with status'),
    '#options' => $filter_options,
    '#default_value' => $filter,
  );
  $form['filter']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
    '#submit' => array('_gallery_user_filter_submit')
  );
  if ($filter) {
    $form['filter']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#submit' => array('_gallery_user_filter_reset')
    );
  }
  
  return $form;
}

/**
 * Function _gallery_user_filter_submit().
 */
function _gallery_user_filter_submit($form, &$form_state) {
  $_SESSION['gallery_user_filter'] = $form_state['values']['filter_status'];
}

/**
 * Function _gallery_user_filter_reset().
 */
function _gallery_user_filter_reset($form, &$form_state) {
  unset($_SESSION['gallery_user_filter']);
}

/**
 * Function _gallery_user_users_sync().
 */
function _gallery_user_users_sync($uid = NULL) {
  // Sync the selected user
  if (isset($uid) && is_numeric($uid)) {
    _gallery_user_sync($uid);
  }

  drupal_goto('admin/user/gallery/users');
}

/**
 * Function _gallery_user_advanced().
 * (advanced user administration)
 */
function _gallery_user_advanced() {
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Sync (Batch operations)'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['advanced']['gallery_user_advanced_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import users from Gallery2'),
    '#return_value' => 1,
    '#default_value' => FALSE,
    '#description' => t('Use this option to import users from an existing Gallery2 install into Drupal.'),
  );
  $form['advanced']['gallery_user_advanced_sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Synchronize (export) all existing users'),
    '#return_value' => 1,
    '#default_value' => FALSE,
    '#description' => t('Use this option to sync all users between Drupal and Gallery2.'),
  );
  
  $form['advanced']['gallery_user_advanced_offline'] = array(
    '#type' => 'checkbox',
    '#title' => t('Switch Drupal to \'offline mode\' for operation'),
    '#return_value' => 1,
    '#default_value' => FALSE,
    '#disabled' => (user_access('administer site configuration') && !variable_get('site_offline', 0)) ? FALSE : TRUE,
    '#prefix' => '<br />',
  );
  
  $form['buttons']['start'] = array('#type' => 'submit', '#value' => t('Start'));
  $form['#validate'] = array('_gallery_user_advanced_validate');
  $form['#submit'] = array('_gallery_user_advanced_submit');
  return $form;
}

/**
 * Function _gallery_user_advanced_validate().
 */
function _gallery_user_advanced_validate($form, &$form_state) {
  if (($form_state['values']['gallery_user_advanced_import'] + $form_state['values']['gallery_user_advanced_sync']) < 1) {
    form_set_error('', t('No option selected.'));
  }
}

/**
 * Function _gallery_user_advanced_submit().
 */
function _gallery_user_advanced_submit($form, &$form_state) {
  if ($form_state['values']['gallery_user_advanced_offline']) {
    variable_set('site_offline', 1);
    $_SESSION['gallery_user_batch_offline'] = TRUE;
  }

  $operations = array();
  if ($form_state['values']['gallery_user_advanced_import']) {
    $operations[] = array('_gallery_user_advanced_import', array());
  }
  if ($form_state['values']['gallery_user_advanced_sync']) {
    $operations[] = array('_gallery_user_advanced_sync', array());
  }
  
  $batch = array(
    'title' => t('User Synchronization'),
    'operations' => $operations,
    'file' => drupal_get_path('module', 'gallery') .'/gallery_user_admin.inc',
    'finished' => '_gallery_user_advanced_finished'
  );
  
  batch_set($batch);
}

/**
 * Function _gallery_user_advanced_import().
 */
function _gallery_user_advanced_import(&$context) {
  // Skip operation if an error occured
  if (isset($context['results']['error']) && $context['results']['error']) {
    $context['finished'] = $context['results']['error'] = 1;
    return;
  }
  // Initialize G2
  if (!_gallery_init(TRUE, NULL, FALSE)) {
    $context['finished'] = $context['results']['error'] = 1;
    return;
  }
  // First pass
  if (empty($context['sandbox'])) {
    // Initialize batch variables
    $context['finished'] = 0;
    $context['sandbox']['progress'] = 0;
    // Total number of G2 users
    list($ret, $context['sandbox']['max']) = GalleryCoreApi::fetchUserCount();
    if ($ret || !$context['sandbox']['max']) {
      gallery_error(t('Error getting number of G2 users'), $ret);
      $context['finished'] = $context['results']['error'] = 1;
      return;
    }
    // Flush entity cache
    gallery_flush_entity_cache();
    // Import Gallery2 groups
    if (variable_get('gallery_user_import_groups', 1)) {
      _gallery_groups_import();
    }
  }
  // Fetch a list of G2 users
  list($ret, $g2_users) = GalleryCoreApi::fetchUsernames(GALLERY_BATCH_INTERVAL, $context['sandbox']['progress']);
  if ($ret) {
    gallery_error(t('Error fetching G2 usernames'), $ret);
    $context['finished'] = $context['results']['error'] = 1;
    return;
  }
  else {
    if (!_gallery_user_import($g2_users, $context['results']['messages'])) {
      $context['finished'] = $context['results']['error'] = 1;
      return;
    }
  }
  $context['sandbox']['progress'] += GALLERY_BATCH_INTERVAL;
  $context['finished'] = 1;
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Function _gallery_user_advanced_sync().
 */
function _gallery_user_advanced_sync(&$context) {
  // Skip operation if an error occured
  if (isset($context['results']['error']) && $context['results']['error']) {
    $context['finished'] = $context['results']['error'] = 1;
    return;
  }
  // Initialize G2
  if (!_gallery_init(TRUE, NULL, FALSE)) {
    $context['finished'] = $context['results']['error'] = 1;
    return;
  }
  // First pass
  if (empty($context['sandbox'])) {
    // Initialize batch variables
    $context['finished'] = 0;
    $context['sandbox']['progress'] = 0;
    // Total number of users
    $total = db_fetch_object(db_query("SELECT COUNT(*) AS users FROM {users} WHERE uid > 0"));
    if (!$total->users) {
      $context['finished'] = $context['results']['error'] = 1;
      return;
    }
    $context['sandbox']['max'] = $total->users;
    // Empty externalIdMap in first pass
    if (variable_get('gallery_user_sync_remap', 0)) {
      $ret = GalleryCoreApi::removeAllMapEntries('ExternalIdMap');
      if ($ret) {
        gallery_error(t('Error emptying \'ExternalIdMap\''), $ret);
        $context['finished'] = $context['results']['error'] = 1;
        return;
      }
    }
  }
  // Sync users
  $result = db_query_range("SELECT uid FROM {users} WHERE uid > 0", $context['sandbox']['progress'], GALLERY_BATCH_INTERVAL);
  while ($user = db_fetch_object($result)) {
    if ($account = user_load(array('uid' => $user->uid))) {
      if (!gallery_user_modify($account, 'update', !$context['sandbox']['progress'])) {
        $context['finished'] = $context['results']['error'] = 1;
        return;
      }
    }
  }
  $context['sandbox']['progress'] += GALLERY_BATCH_INTERVAL;
  $context['finished'] = 1;
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Function _gallery_user_advanced_finished().
 */
function _gallery_user_advanced_finished($success, $results, $operations) {
  if (isset($_SESSION['gallery_user_batch_offline'])) {
    variable_set('site_offline', 0);
    unset($_SESSION['gallery_user_batch_offline']);
  }
  //
  if ($success) {
    if (isset($results['error']) && $results['error']) {
      drupal_set_message(t('User synchronization (partially) failed.'), 'error');
    }
    else {
      if (isset($results['messages']) && count($results['messages'])) {
        drupal_set_message(theme('item_list', $results['messages'], t('The following messages occured:')), 'notice');
        drupal_set_message('<strong>'. t('Invalid user items were skipped.') .'</strong>', 'notice');
        watchdog('gallery', theme('item_list', $results['messages'], t('The following messages occured:')));
      }
      else {
        drupal_set_message(t('User synchronization successfully completed.'));
      }
    }
  }
  else {
    $error_operation = reset($operations);
    drupal_set_message(t('An error occurred while processing @operation', array('@operation' => $error_operation[0])), 'error');
  }
}

/**
 * Function _gallery_user_settings().
 */
function _gallery_user_settings() {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_settings.inc');
  
  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );
  
  // General settings
  $form['user']['gallery_user_profile_hide'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide Gallery2 section in profiles'),
    '#default_value' => variable_get('gallery_user_profile_hide', 0),
    '#description' => t('Hide the Gallery2 section (i.e. Gallery2-Drupal Sync Status) on the user profile pages.'),
  );
  
  // Sync settings
  $form['user']['sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Synchronization'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  
  $roles  = array(0 => t('none'));
  $roles += user_roles(TRUE);
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  $form['user']['sync']['gallery_user_admin_role'] = array(
    '#type' => 'select',
    '#title' => t('Drupal \'admin\' role'),
    '#default_value' => variable_get('gallery_user_admin_role', 0),
    '#options' => $roles,
    '#description' => t('Select the Drupal role equivalent to Gallery2\'s \'Site Admin\' group (or \'none\' to disable this
                         feature). The roles \'anonymous\' and \'authenticated\' are not available for selection.'),
  );
  
  $form['user']['sync']['gallery_user_locked'] = array(
    '#type' => 'checkbox',
    '#title' => t('Lock G2 accounts'),
    '#default_value' => variable_get('gallery_user_locked', 0),
    '#description' => t('Locking G2 account prevents users from changing their details (password, email, ...) in G2.'),
  );
  
  // Fullname settings
  $profile_status = module_exists('profile');
  $profile_status_str = theme('gallery_module_status_message', $profile_status);
  $desc = t('Full names in Gallery2 can be supported by using the profile module
            (!profile_status) with a \'Full Name\' profile field as defined below.', 
            array('!profile_status' => $profile_status_str)
  );
  if (!$profile_status) {
    $desc .= t(' However the profile module is disabled, so this functionality is
      not available and the options are disabled.');
  }
  $form['user']['sync']['fullname'] = array(
    '#type' => 'fieldset',
    '#title' => t('Full Name settings'),
    '#collapsible' => TRUE,
    '#collapsed' => !$profile_status,
    '#description' => $desc,
  );
  if ($profile_status) {
    $usefullname = variable_get('gallery_use_fullname', 0);
    $form['user']['sync']['fullname']['gallery_use_fullname'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Full Name support'),
      '#return_value' => 1,
      '#default_value' => $usefullname,
      '#disabled' => !$profile_status,
      '#description' => t('Use full name from profile module in Gallery2 user data. Note that
                           enabling/disabling this only updates Gallery2 user data when the
                           Drupal user is updated or if a user sync is performed.'),
    );
    
    if ($usefullname) {
      $categories = array();
      $result = db_query('SELECT DISTINCT(category) FROM {profile_fields}');
      while ($category = db_fetch_object($result)) {
        $categories[$category->category] = $category->category;
      }
      $default_category = variable_get('gallery_profile_fullname_category', 'Personal Information');
      $form['user']['sync']['fullname']['gallery_profile_fullname_category'] = array(
        '#type' => 'select',
        '#title' => t('Full name profile category'),
        '#default_value' => $default_category,
        '#options' => $categories,
        '#description' => t('Name of the category containing the \'Full Name\' field in profile module.'),
      );
      $fields = array();
      $result = _profile_get_fields($default_category);
      while ($field = db_fetch_object($result)) {
        $fields[$field->name] = $field->title;
      }
      $form['user']['sync']['fullname']['gallery_profile_fullname_field'] = array(
        '#type' => 'select',
        '#title' => t('Full name profile field'),
        '#default_value' => variable_get('gallery_profile_fullname_field', 'profile_fullname'),
        '#options' => $fields,
        '#description' => t('Name of \'Full Name\' field in profile module.'),
      );
    }
  }
  
  // Import behaviour
  $form['user']['sync']['import'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Sync - Import'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['user']['sync']['import']['gallery_user_import_groups'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import Gallery2 groups'),
    '#default_value' => variable_get('gallery_user_import_groups', 1),
    '#description' => t('Import Gallery2 groups into Drupal roles (in addition to users).'),
  );
  
  $form['user']['sync']['import']['gallery_user_import_override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override existing Drupal users'),
    '#default_value' => variable_get('gallery_user_import_override', 0),
    '#description' => t('Replaces user details (password, email, groups, ...) of existing Drupal users with Gallery2 imported values.'),
  );
  
  $form['user']['sync']['import']['gallery_user_import_conflict'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Auto-resolve email address conflicts'),
    '#default_value' => variable_get('gallery_user_import_conflict', array()),
    '#options' => array(
      GALLERY_IMPORT_CONFLICT_DUPLICATE   => t('Duplicate e-mail addresses'),
      GALLERY_IMPORT_CONFLICT_INVALID     => t('Invalide e-mail addresses')
    ),
    '#description' => t('Renames duplicate/invalid e-mail addresses to username@drupaldomain.
                         If this option is disabled you get a list of users to handle conflicts yourself.')
  );
  
  // Export behaviour
  $form['user']['sync']['export'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Sync - Export (Synchronize)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  
  $form['user']['sync']['export']['gallery_user_sync_remap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remap all users'),
    '#default_value' => variable_get('gallery_user_sync_remap', 0),
    '#description' => t('Remaps all users instead of missing or mismatching ones only.
                         This will completely flush Gallery2\'s \'externalIdMap\'.'),
  );
  
  $form = system_settings_form($form);
  $form['#submit'] = array('_gallery_user_settings_submit', 'system_settings_form_submit');
  return $form;
}

/**
 * Function _gallery_user_settings_submit().
 */
function _gallery_user_settings_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Reset to defaults')) {
    $fullname_changed = variable_get('gallery_use_fullname', 0);
  } 
  else {
    $fullname_changed = (isset($form_state['values']['gallery_use_fullname'])
      && ($form_state['values']['gallery_use_fullname'] != variable_get('gallery_use_fullname', 0)));
  }
  if ($fullname_changed) {
      drupal_set_message(t('Full Name settings have changed. You should now synchronize
                            your users on the <a href="@user-admin">Gallery users</a> page.',
                            array('@user-admin' => url('admin/user/gallery'))));
  }
}
