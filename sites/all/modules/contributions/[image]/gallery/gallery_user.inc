<?php
// $Id: gallery_user.inc,v 1.11.2.4 2009/01/25 14:21:32 profix898 Exp $

require_once(drupal_get_path('module', 'gallery') .'/gallery_groups.inc');

/**
 * gallery.module : gallery_user.inc
 * User Functions (insert, update, delete, view, details, ...)
 */
 
define('GALLERY_MAP_UNKNOWN',                             0);
define('GALLERY_MAP_USER_EXISTS',                         1);
define('GALLERY_MAP_USER_EXISTS_BUT_NEEDS_MAPPING',       2);
define('GALLERY_MAP_USER_DOES_NOT_EXIST_BUT_IS_MAPPED',   3);
define('GALLERY_MAP_USER_DOES_NOT_EXIST',                 4);

define('GALLERY_USERINFO_NOERROR',                        1);
define('GALLERY_USERINFO_ERROR',                          2);
define('GALLERY_USERINFO_ERROR_MISSING',                  3);
define('GALLERY_USERINFO_ERROR_USERNAME',                 4);
define('GALLERY_USERINFO_ERROR_FULLNAME',                 5);
define('GALLERY_USERINFO_ERROR_FULLNAME_MISSING',         6);
define('GALLERY_USERINFO_ERROR_EMAIL',                    7);
define('GALLERY_USERINFO_ERROR_PASSWORD',                 8);
define('GALLERY_USERINFO_ERROR_HASHMETHOD',               9);
define('GALLERY_USERINFO_ERROR_GROUPS',                  10);

define('GALLERY_IMPORT_CONFLICT_DUPLICATE',               1);
define('GALLERY_IMPORT_CONFLICT_INVALID',                 2);
 
/**
 * Function gallery_user_insert().
 * (insert new user)
 */
function gallery_user_insert(&$edit, $user) {
    $user->roles = isset($edit['roles']) ? $edit['roles'] : $user->roles;
    $user->status = isset($edit['status']) ? $edit['status'] : TRUE;
    
    gallery_user_modify($user, 'create');
}

/**
 * Function gallery_user_update().
 * (update a user with new information)
 */
function gallery_user_update(&$edit, $user) {
  $user->language = isset($edit['language']) ? $edit['language'] : gallery_get_language('', $user);
  $user->pass = !empty($edit['pass']) ? md5($edit['pass']) : $user->pass;
  $user->status = isset($edit['status']) ? $edit['status'] : $user->status;
  $user->mail = !empty($edit['mail']) ? $edit['mail'] : $user->mail;
  $user->roles = isset($edit['roles']) ? $edit['roles'] : $user->roles;

  // Fullname support
  if (module_exists('profile') && variable_get('gallery_use_fullname', 0)) {
    $fullname_field = variable_get('gallery_profile_fullname_field', 'profile_fullname');
    $user->$fullname_field = isset($edit[$fullname_field]) ? $edit[$fullname_field] : $user->$fullname_field;
  }
  
  // Username is about to change
  if ($namechange = (isset($edit['name']) && ($edit['name'] != $user->name))) {
    // Make sure the original user is up to date
    gallery_user_modify($user, 'update', TRUE);
  }
  
  $user->name = isset($edit['name']) ? $edit['name'] : $user->name;
  
  if ($namechange) {
    // Change username
    gallery_user_modify($user, 'username');
  }
  else {
    // Update user
    gallery_user_modify($user, 'update', TRUE);
  }
}

/**
 * Function gallery_user_delete().
 * (delete the user from the Gallery)
 */ 
function gallery_user_delete($user) {
  gallery_user_modify($user, 'delete');
}

/**
 * Function gallery_user_modify().
 * (modify (create/update/delete) a user)
 */
function gallery_user_modify($user, $action = 'create', $groups = FALSE, $vars = NULL) {
  if (!_gallery_init(TRUE, $vars)) {
    return FALSE;
  }

  // Check for fullname support
  $fullname_field = variable_get('gallery_profile_fullname_field', 'profile_fullname');
  $usefullname = module_exists('profile') && variable_get('gallery_use_fullname', 0);
  $fullname = $usefullname ? (isset($user->$fullname_field) ? $user->$fullname_field : '') : $user->name;

  // Generate random password for G2 if user is blocked
  $pass = ($user->status) ? $user->pass : user_password(20);

  switch ($action) {
    case 'username':
      $ret = GalleryEmbed::updateUser($user->uid,
              array('username' => $user->name,
                    'fullname' => $fullname,
                    'email' => $user->mail,
                    'language' => gallery_get_language('', $user),
                    'hashedpassword' => $pass,
                    'hashmethod' => 'md5'));
      if ($ret) {
        gallery_error(t('Error updating Gallery user (username changed)'), $ret);
        return FALSE;
      }
      break;
    case 'create' :
    case 'update' :
      // Get map state of the user
      list($g2_user_state, $g2_user, $ret) = gallery_user_map_state($user);
      if ($ret) {
        gallery_error(t('Error determining user map state'), $ret);
        return FALSE;
      }
      
      // Complete user mapping
      switch ($g2_user_state) {
        case GALLERY_MAP_USER_EXISTS_BUT_NEEDS_MAPPING:
          // create map entry for the user
          $ret = GalleryEmbed::addExternalIdMapEntry($user->uid, $g2_user->id, 'GalleryUser');
          if ($ret) {
            gallery_error(t('Error creating map entry (ExternlIdMapEntry)'), $ret);
            return FALSE;
          }
        case GALLERY_MAP_USER_EXISTS:
          // Update user (Drupal -> G2)
          $ret = GalleryEmbed::updateUser($user->uid,
                  array('username' => $user->name,
                        'fullname' => $fullname,
                        'email' => $user->mail,
                        'language' => gallery_get_language('', $user),
                        'hashedpassword' => $pass,
                        'hashmethod' => 'md5'));
          if ($ret) {
            gallery_error(t('Error updating Gallery user'), $ret);
            return FALSE;
          }
          break;
        case GALLERY_MAP_USER_DOES_NOT_EXIST_BUT_IS_MAPPED:
          // Remove mapping for non-existing user
          // (also happens if gallery_user_modify() is called with a changed username)
          $ret = GalleryCoreApi::removeMapEntry('ExternalIdMap', array('externalId' => $user->uid, 'entityType' => 'GalleryUser'));
          if ($ret) {
            gallery_error(t('Error removing map entry (ExternlIdMapEntry)'), $ret);
            return FALSE;
          }
        case GALLERY_MAP_USER_DOES_NOT_EXIST:
          // Create new user
          if (!$user->uid)
            return FALSE;
          $ret = GalleryEmbed::createUser($user->uid,
                  array('username' => $user->name,
                        'email' => $user->mail,
                        'fullname' => $fullname,
                        'language' => gallery_get_language('', $user),
                        'hashedpassword' => $pass,
                        'hashmethod' => 'md5'));
          if ($ret) {
            gallery_error(t('Error creating Gallery user'), $ret);
            return FALSE;
          }
          list($ret, $g2_user) = GalleryCoreApi::loadEntityByExternalId($user->uid, 'GalleryUser');
          if ($ret) {
            gallery_error(t('Error loading newly created Gallery user'), $ret);
            return FALSE;
          }
          break;
      }
      // Update group info
      _gallery_groups_user($user, $groups);
      // Admin role mapping
      $admin_role = variable_get('gallery_user_admin_role', 0);
      if (($admin_role && in_array($admin_role, array_keys($user->roles))) || ($user->uid == 1)) {
        // Get G2 admin group id
        list($ret, $g2_admin_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.adminGroup');
        if ($ret) {
          gallery_error(t('Error getting \'adminGroup\' id'), $ret);
          return FALSE;
        }
        // Add user to admin group
        $ret = GalleryCoreApi::addUserToGroup($g2_user->id, $g2_admin_gid);
        if ($ret) {
          gallery_error(t('Error adding user to Gallery group (:gid)',
            array(':gid' => $g2_admin_gid)), $ret);
          return FALSE;
        }
      }
      break;
    case 'delete' :
      $ret = GalleryEmbed::deleteUser($user->uid);
      if ($ret) {
        gallery_error(t('Error deleting Gallery user'), $ret);
        return FALSE;
      }
      break;
  }
  
  // Set the 'locked' property for the user
  if (variable_get('gallery_user_locked', 0) && ($action != 'delete') && ($user->uid > 1)) {
    list($ret, $g2_user) = GalleryCoreApi::loadEntityByExternalId($user->uid, 'GalleryUser');
    if ($ret) {
      gallery_error(t('Error loading Gallery user'), $ret);
      return FALSE;
    }
    if (!$g2_user->locked) {
      list($ret, $lock_id) = GalleryCoreApi::acquireWriteLock($g2_user->id);
      if ($ret) {
        gallery_error(t('Error acquiring write lock'), $ret);
        return FALSE;
      }
      $g2_user->setLocked(TRUE);
      if ($g2_user->save()) {
        gallery_error(t('Locking user account failed'), $ret);
        return FALSE;
      }
    }
  }

  GalleryEmbed::done();
  return TRUE;
}

/**
 * Function _gallery_user_sync().
 * (sync user info for $uid)
 */
function _gallery_user_sync($uid) {
  $user = user_load(array('uid' => $uid));
  gallery_user_modify($user, 'update', TRUE);
}

/**
 * Function gallery_user_view().
 * (view Gallery user details for a specific user)
 */
function gallery_user_view(&$user) {
  if (variable_get('gallery_user_profile_hide', 0) || !_gallery_init(TRUE)) {
    return;
  }
  
  // Profile section title
  $user->content['gallery2'] = array(
    '#type' => 'user_profile_category',
    '#title' => t('Gallery2')
  );
  // User album status
  if ($useralbum = gallery_user_useralbum($user->uid)) {
    $user->content['gallery2']['useralbum_link'] = array(
      '#type' => 'user_profile_item',
      '#value' => l(t('Album: @username', array('@username' => $user->name)), $useralbum)
    );
  }
  else {
    $user->content['gallery2']['useralbum_link'] = array(
      '#type' => 'user_profile_item',
      '#value' => t('User has not created an album yet')
    );
  }
  // Sync/Map status info
  $g2_userinfo = gallery_user_map_info($user);
  if (($g2_userinfo['status']) && (user_access('administer users'))) {
    $user->content['gallery2']['user_sync'] = array(
      '#type' => 'user_profile_item',
      '#title' => t('Gallery2-Drupal Sync Status'),
      '#value' => implode(',<br />', gallery_user_map_info_status($g2_userinfo['status']))
    );
  }
  
  GalleryEmbed::done();
}

/**
 * Function gallery_user_useralbum().
 * (create link to user album)
 */
function gallery_user_useralbum($uid = NULL, $link = TRUE) {
  if (!_gallery_init(TRUE)) {
    return FALSE;
  }
  global $user;
  $uid = isset($uid) ? $uid : $user->uid;
  $g2_user = _gallery_user_map($uid, TRUE);
  // User album status
  if ($g2_user && (gallery_single_plugin_status('useralbum') == GALLERY_PLUGIN_ENABLED)) {
    // Fetch user album id
    list($ret, $album_id) = GalleryCoreApi::getPluginParameter('module', 'useralbum', 'albumId', $g2_user[$uid]);
    if ($ret) {
      gallery_error(t('Error fetching user album id'), $ret);
      return FALSE;
    }
    // Generate link to user album
    if (is_numeric($album_id) && $album_id > 0) {
      return $link ? gallery_generate_url(array('itemId' => $album_id), FALSE) : $album_id;
    }
  }
  
  return FALSE;
}

/**
 * Function gallery_user_map_info().
 * (get info about user map status)
 */
function gallery_user_map_info($user, $noerror_status = TRUE) {
  $g2_userinfo = array('g2_id' => -1, 'status' => array());
  // User map entry
  $ret = GalleryEmbed::isExternalIdMapped($user->uid, 'GalleryUser');
  if ($ret && !($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
    $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_MISSING;
    return $g2_userinfo;
  }
  // But user not available
  list($ret, $g2_user) = GalleryCoreApi::loadEntityByExternalId($user->uid, 'GalleryUser');
  if ($ret) {
    $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_MISSING;
    return $g2_userinfo;
  }
  // Username
  $g2_userinfo['g2_id'] = $g2_user->id;
  if ($g2_user->userName != $user->name) {
    $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_USERNAME;
  }
  // Fullname
  if (module_exists('profile') && variable_get('gallery_use_fullname', 0)) {
    $fullname_field = variable_get('gallery_profile_fullname_field', 'profile_fullname');
    $fullname_result = db_query("SELECT v.value FROM {profile_values} v INNER JOIN {profile_fields} f ON v.fid = f.fid AND v.uid = %d WHERE f.name = '%s'", $user->uid, $fullname_field);
    $fullname = db_fetch_object($fullname_result);
    $fullname = $fullname->value;
    if ($g2_user->fullName != $fullname) {
      $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_FULLNAME;
    }
    elseif (!$fullname) {
      $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_FULLNAME_MISSING;
    }
  }
  // Email adress
  if ($g2_user->email != $user->mail) {
    $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_EMAIL;
  }
  // Password
  if ($user->status && ($g2_user->hashedPassword != $user->pass)) {
    if (strlen($g2_user->hashedPassword) != strlen($user->pass)) {
      $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_HASHMETHOD;
    }
    $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_PASSWORD;
  }
  // Roles/Groups
  if (!gallery_groups_map_info($g2_user, $user)) {
    $g2_userinfo['status'][] = GALLERY_USERINFO_ERROR_GROUPS;
  }
  // Overall sync status
  if ($noerror_status && !count($g2_userinfo['status'])) {
    $g2_userinfo['status'][] = GALLERY_USERINFO_NOERROR;
  }
  
  return $g2_userinfo;
}

/**
 * Function gallery_user_map_info_status().
 * (get string representation of the use map status)
 */
function gallery_user_map_info_status($info = array(), $format = TRUE) {
  $info_map = array(
    GALLERY_USERINFO_NOERROR                    => t('OK'),
    GALLERY_USERINFO_ERROR                      => t('Any Error'),
    GALLERY_USERINFO_ERROR_MISSING              => t('Missing from G2'),
    GALLERY_USERINFO_ERROR_USERNAME             => t('Different Usernames'),
    GALLERY_USERINFO_ERROR_FULLNAME             => t('Different Full Names'),
    GALLERY_USERINFO_ERROR_FULLNAME_MISSING     => t('G2 Full Name missing'),
    GALLERY_USERINFO_ERROR_EMAIL                => t('Different E-Mails'),
    GALLERY_USERINFO_ERROR_PASSWORD             => t('Different Passwords'),
    GALLERY_USERINFO_ERROR_HASHMETHOD           => t('Imported/Different hash method'),
    GALLERY_USERINFO_ERROR_GROUPS               => t('Roles <> Groups')
  );
  
  if ($format) {
    $status = array();
    if (!count($info)) {
      $info[] = GALLERY_USERINFO_NOERROR;
    }
    foreach ($info as $key => $value) {
      $status[] = $info_map[$value];
    }
    return $status;
  }
  
  return $info_map;
}

/**
 * Function gallery_user_map_state().
 * (get state of user mapping)
 */
function gallery_user_map_state($user) {
  // See if user already exists in G2
  list($ret, $g2_user) = GalleryCoreApi::fetchUserByUsername($user->name);
  if (!$ret) {
    // User is in G2, so map the user if needed
    $ret2 = GalleryEmbed::isExternalIdMapped($user->uid, 'GalleryUser');
    if ($ret2) {
      if ($ret2->getErrorCode() & ERROR_MISSING_OBJECT) {
        return array(GALLERY_MAP_USER_EXISTS_BUT_NEEDS_MAPPING, $g2_user, NULL);
      }
      else {
        // Some other error
        return array(GALLERY_MAP_UNKNOWN, $g2_user, $ret2);
      }
    }
    else {
        return array(GALLERY_MAP_USER_EXISTS, $g2_user, NULL);
    }
  }
  elseif ($ret->getErrorCode() & ERROR_MISSING_OBJECT) {
    // User does not yet exist in G2
    // Check if the externalID was mapped (it should not be)
    $ret2 = GalleryEmbed::isExternalIdMapped($user->uid, 'GalleryUser');
    if ($ret2) {
      if ($ret2->getErrorCode() & ERROR_MISSING_OBJECT) {
        return array(GALLERY_MAP_USER_DOES_NOT_EXIST, $g2_user, NULL);
      }
      else {
        // Some other error
        return array(GALLERY_MAP_UNKNOWN, $g2_user, $ret2);
      }
    }
    else {
      // User is mapped
      return array(GALLERY_MAP_USER_DOES_NOT_EXIST_BUT_IS_MAPPED, $g2_user, NULL);
    }
  }
  else {
    return array(NULL, $g2_user, $ret);
  }
}

/**
 * Function _gallery_user_import().
 * (import Gallery users into Drupal)
 */
function _gallery_user_import($g2_users, &$messages) {
  $resolve_conflict = variable_get('gallery_user_import_conflict', array());
  // Anonymous user id
  list($ret, $guest) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.anonymousUser');
  if (!$ret) {
    unset($g2_users[$guest]);
  }
  if (($g2_groups_map = _gallery_groups_map()) === FALSE) {
    return FALSE;
  }
  if (($g2_extIdMap = _gallery_user_map(array_keys($g2_users))) === FALSE) {
    return FALSE;
  }
  gallery_debug($g2_users, t('G2 Users'));
  gallery_debug($g2_extIdMap, t('G2 ExternalIdMap'));
  // Iterate over G2 users
  foreach ($g2_users as $g2_id => $g2_username) {
    $new_user = !array_key_exists($g2_id, $g2_extIdMap);
    list($ret, $g2_user) = GalleryCoreApi::fetchUserByUsername($g2_username);
    if ($ret) {
      gallery_error(t('Error fetching user by username (:name)',
        array(':name' => $g2_username)), $ret);
      return FALSE;
    }
    // Collect user details and validate the values (name, mail, ...)
    $values = array();
    $values['name'] = $g2_user->userName;
    if ($error = user_validate_name($values['name'])) {
      $messages[] = t('G2 User Import (uid: :uid, name: \':name\'): !error',
        array(':uid' => $g2_id, ':name' => $values['name'], '!error' => $error));
      continue;
    }
    $values['fullname'] = $g2_user->fullName;
    $values['pass'] = user_password(20);
    $password_hash = $g2_user->hashedPassword;
    $values['mail'] = $g2_user->email;
    if ($error = user_validate_mail($values['mail'])) {
      if ($resolve_conflict[GALLERY_IMPORT_CONFLICT_INVALID]) {
        _gallery_user_resolve_mail($values['mail'], $values['name'], $messages);
        $error = user_validate_mail($values['mail']) ? $error : FALSE;
      }
      if ($error) {
        $messages[] = t('G2 User Import (uid: :uid, name: \':name\'): !error',
          array(':uid' => $g2_id, ':name' => $values['name'], '!error' => $error));
        continue;
      }
    }
    else if ($new_user && db_result(db_query("SELECT COUNT(uid) FROM {users} WHERE uid <> 0 AND LOWER(mail) = LOWER('%s')", $values['mail'])) > 0) {
      $error = TRUE;
      if ($resolve_conflict[GALLERY_IMPORT_CONFLICT_DUPLICATE]) {
        _gallery_user_resolve_mail($values['mail'], $values['name'], $messages);
        $error = FALSE;
      }
      if ($error) {
        $messages[] = t('G2 User Import (uid: :uid, name: \':name\'): The e-mail address :mail is already registered.',
          array(':uid' => $g2_id, ':name' => $values['name'], ':mail' => $values['mail']));
        continue;
      }
    }
    $values['language'] = $g2_user->language;
    $values['created'] = $g2_user->creationTimestamp;
    $values['roles'] = array();
    list($ret, $g2_groups) = GalleryCoreApi::fetchGroupsForUser($g2_user->id);
    if ($ret) {
      gallery_error(t('Error fetching groups for user (uid: :uid)',
        array(':uid' => $g2_id)), $ret);
      return FALSE;
    }
    foreach ($g2_groups as $g2_gid => $g2_groupname) {
      if (isset($g2_groups_map[$g2_gid])) {
        $values['roles'][$g2_groups_map[$g2_gid]] = $g2_groupname;
      }
    }
    // Is the user blocked in G2
    list($ret, $g2_user_blocked) = GalleryCoreApi::isDisabledUsername($g2_username);
    if ($ret) {
      gallery_error(t('Error calling isDisabledUsername() for \':name\'',
        array(':name' => $g2_username)), $ret);
      return FALSE;
    }
    $values['status'] = !$g2_user_blocked;
    // Create new Drupal user (this will also override the G2 user
    //  during hook_user, but there is no 'clean' way to avoid this)
    if ($new_user) {
      $values['notify'] = FALSE;
      if (!($user = user_save('', $values))) {
        $messages[] = t('Error creating Drupal user for G2 user (uid: :uid)',
          array(':uid' => $g2_id));
        continue;
      }
    }
    else {
      $user = new stdClass();
      $user->uid = $g2_extIdMap[$g2_id];
    }
    // Override user details if requested (or for new users)
    if ($user->uid && ($new_user || variable_get('gallery_user_import_override', 0))) {
      // Fullname support
      if (module_exists('profile') && variable_get('gallery_use_fullname', 0)) {
        $fullname_category = variable_get('gallery_profile_fullname_category', 'Personal Information');
        $fullname_field = variable_get('gallery_profile_fullname_field', 'profile_fullname');
        $values[$fullname_field] = $values['fullname'];
        profile_save_profile($values, $user, $fullname_category);
      }
      // Do we have a md5 hash (unsalted hash)?
      if (strlen($password_hash) == 32) {
        db_query("UPDATE {users} SET pass = '%s' WHERE uid = %d", $password_hash, $user->uid);
        unset($values['pass']);
      }
      // Update Drupal user with G2 user details (invokes hook_user)
      user_save(user_load(array('uid' => $user->uid)), $values);
      // Override/Restore password in G2 (in case we have a salted G2 hash)
      // (Drupal hash will remain a dummy and user is authenticated in hook_auth against G2 directly)
      if (strlen($password_hash) > 32) {
        $ret = GalleryEmbed::updateUser($user->uid, array('hashedpassword' => $password_hash, 'hashmethod' => 'md5'));
        if ($ret) {
          gallery_error(t('Error updating Gallery user (password)'), $ret);
          return FALSE;
        }
      }
    }
  }
  
  return TRUE;
}

/**
 * Function _gallery_user_resolve_mail()
 */
function _gallery_user_resolve_mail(&$mail, $username, &$messages) {
  // Replace all invalid chars in the username
  $invalid_search = array(' ', '!', '#', '$', '%', '&', '*', '+', '/', '=', '?', '`', '|', '{', '}', '~', '\'');
  $invalid_replace = array_fill(0, count($invalid_search), '_');
  $user = str_replace($invalid_search, $invalid_replace, $username);
  // Generate a new mail address using a simple 'username_#@drupaldomain' naming schema
  // (=> username@drupaldomain, username_1@drupaldomain, username_2@drupaldomain, ...)
  $i = 0;
  do {
    $newmail = $user . (($i > 0) ? ('_'. $i) : '') .'@'. $_SERVER['HTTP_HOST'];
    $error = (db_result(db_query("SELECT COUNT(uid) FROM {users} WHERE uid <> 0 AND LOWER(mail) = LOWER('%s')", $newmail)) > 0);
  } while ($error && $i++ < 10);
  // Do we have a new mail address?
  if (!$error) {
    $messages[] = t('G2 User Import (name: \':name\'): e-mail address :oldmail changed to :newmail.',
      array(':name' => $username, ':oldmail' => $mail, ':newmail' => $newmail));
    $mail = $newmail;
  }
}

/**
 * Function _gallery_user_map().
 * (fetch 'GalleryUser' entries from G2 'ExternalIdMap')
 */
function _gallery_user_map($ids = array(), $inverse = FALSE) {
  // g2Id => externalId (default)
  $ids = is_array($ids) ? $ids : array($ids);
  $match = array('entityType' => 'GalleryUser');
  if (count($ids) > 0) {
    if ($inverse) {
      $match['externalId'] = $ids;
    }
    else {
      $match['entityId'] = $ids;
    }
  }
  // Fetch the map entries
  list($ret, $resultMap) = GalleryCoreApi::getMapEntry('ExternalIdMap', array('externalId', 'entityId'), $match);
  if ($ret) {
    gallery_error(t('Error fetching \'GalleryUser\' entries from \'ExternalIdMap\''), $ret);
    return FALSE;
  }
  // Iterate over the results
  $g2_extIdMap = array();
  while (($row = $resultMap->nextResult()) !== FALSE) {
    $g2_extIdMap[($inverse ? $row[0] : $row[1])] = ($inverse ? $row[1] : $row[0]);
  }
  
  return $g2_extIdMap;
}

/**
 * Function _gallery_user_drupal_users().
 * (fetch all existing Drupal users (uid => username))
 */
function _gallery_user_drupal_users() {
  $users = array();
  $result = db_query("SELECT uid, name FROM {users} WHERE uid > 0");
  while ($user = db_fetch_object($result)) {
    $users[$user->uid] = $user->name;
  }
  
  return $users;
}
