<?php
// $Id: gallery_groups.inc,v 1.3.2.2 2007/11/27 18:10:32 profix898 Exp $

/**
 * gallery.module : gallery_groups.inc
 * Group/Role Functions (sync groups, ...)
 */

/**
 * Function _gallery_groups_user().
 * (sync Drupal roles and Gallery groups for a specific user)
 */
function _gallery_groups_user($user, $groups = FALSE) {
  // Sync the Drupal roles and Gallery groups
  if ($groups) {
    _gallery_groups_sync();
  }
  
  // Get the Gallery groups for this user
  // First get the G2 Id from the Drupal uid
  list($ret, $g2_user) = GalleryCoreApi::loadEntityByExternalId($user->uid, 'GalleryUser');
  if ($ret) {
    gallery_error(t('Error loading Gallery user from Drupal user id (:uid)',
      array(':uid' => $user->uid)), $ret);
    return;
  }
  // Then get the groups for this user currently set in G2
  list($ret, $g2_user_groups) = GalleryCoreApi::fetchGroupsForUser($g2_user->id);
  if ($ret) {
    gallery_error(t('Error getting Gallery group info for user (:uid)',
      array(':uid' => $g2_user->id)), $ret);
    return;
  }
  gallery_debug($g2_user_groups, t('G2 groups for G2 user (uid: :g2uid)', array(':g2uid' => $g2_user->id)));
  
  // Convert the Drupal role Ids into Gallery Group Ids
  $user->roles[DRUPAL_ANONYMOUS_RID] = DRUPAL_ANONYMOUS_RID;
  $user->roles[DRUPAL_AUTHENTICATED_RID] = DRUPAL_AUTHENTICATED_RID;
  gallery_debug($user->roles, t('Drupal roles for Drupal user (uid: :uid)', array(':uid' => $user->uid)));
  if (($g2_rid_map = _gallery_groups_map(array_keys($user->roles), TRUE)) === FALSE) {
    return;
  }
  gallery_debug($g2_rid_map, t('Drupal roles <> G2 groups map (for Drupal user)'));
  if (($g2_groups_map = array_flip(_gallery_groups_map(array_keys($g2_user_groups), FALSE))) === FALSE) {
    return;
  }
  gallery_debug($g2_groups_map, t('Drupal roles <> G2 groups map (for G2 user)'));
  
  // Find if the user needs to be deleted from any G2 groups (only mapped groups)
  $delete_list = array_diff($g2_groups_map, $g2_rid_map);
  gallery_debug($delete_list, t('Remove user from these groups'));
  foreach ($delete_list as $rid => $gid) {
    $ret = GalleryCoreApi::removeUserFromGroup($g2_user->id, $gid);
    if ($ret) {
      gallery_error(t('Error removing user from Gallery group (Gallery Group Id: :gid)',
        array(':gid' => $gid)), $ret);
      return;
    }
  }
  
  // Find if the user needs to be added to any G2 groups
  $add_list = array_diff($g2_rid_map, $g2_groups_map);
  gallery_debug($add_list, t('Add user to these groups'));
  foreach ($add_list as $rid => $gid) {
    $ret = GalleryCoreApi::addUserToGroup($g2_user->id, $gid);
    if ($ret) {
      gallery_error(t('Error adding user to Gallery group (:gid)',
        array(':gid' => $gid)), $ret);
    return;
    }
  }
}

/**
 * Function _gallery_groups_sync().
 * (sync Drupal roles and Gallery groups)
 */ 
function _gallery_groups_sync() {
  static $sync_groups = TRUE;
  // Sync groups only once
  if (!$sync_groups) {
    return;
  }
  $sync_groups = FALSE;
  // Check if the Drupal role <> G2 group mapping exists
  $roles = user_roles();
  $admin_role = variable_get('gallery_user_admin_role', 0);
  foreach ($roles as $rid => $role_name) {
    // Add Drupal <> G2 mapping if needed
    $ret = GalleryEmbed::isExternalIdMapped($rid, 'GalleryGroup');
    if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
      switch ($rid) {
        case DRUPAL_ANONYMOUS_RID:
          list($ret, $g2_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.everybodyGroup');
          if ($ret) {
            gallery_error(t('Error retrieving Gallery group Id for \'Everybody\' group'), $ret);
            return;
          }
          $ret = GalleryEmbed::addExternalIdMapEntry($rid, $g2_gid, 'GalleryGroup');
          if ($ret) {
              gallery_error(t('Error creating Drupal role <> Gallery group mapping for \'anonymous user\' role (Drupal Role Id: :rid, Gallery Group Id: :gid)',
                array(':rid' => $rid, ':gid' => $g2_gid)), $ret);
              return;
          }
          break;
        case DRUPAL_AUTHENTICATED_RID:
          list($ret, $g2_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.allUserGroup');
          if ($ret) {
            gallery_error(t('Error retrieving Gallery group Id for \'Registered Users\' group'), $ret);
            return;
          }
          $ret = GalleryEmbed::addExternalIdMapEntry($rid, $g2_gid, 'GalleryGroup');
          if ($ret) {
            gallery_error(t('Error creating Drupal role <> Gallery group mapping for \'authenticated user\' role (Drupal Role Id: :rid, Gallery Group Id: :gid)',
              array(':rid' => $rid, ':gid' => $g2_gid)), $ret);
            return;
          }
          break;
        default:
          // Special handling of the 'admin' role
          if ($rid == $admin_role) {
            // Get G2 admin group id
            list($ret, $g2_admin_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.adminGroup');
            if ($ret) {
              gallery_error(t('Error getting \'adminGroup\' id'), $ret);
              return;
            }
            $ret = GalleryEmbed::addExternalIdMapEntry($rid, $g2_admin_gid, 'GalleryGroup');
            if ($ret) {
              gallery_error(t('Error creating Drupal role <> Gallery \'Site Admin\' group mapping (Drupal Role Id: :rid, Gallery Group Id: :gid)',
                array(':rid' => $rid, ':gid' => $g2_gid)), $ret);
              return;
            }
          }
          else {
            // Is there a group with this name already?
            list($ret, $g2_group) = GalleryCoreApi::fetchGroupByGroupName($role_name);
            if ($ret && ($ret->getErrorCode() & ERROR_MISSING_OBJECT)) {
              $ret = GalleryEmbed::createGroup($rid, $role_name);
              if ($ret) {
                gallery_error(t('Error creating Gallery group (Drupal Role Id: :rid, Drupal Role Name: :rname)',
                  array(':rid' => $rid, ':rname' => $role_name)), $ret);
                return;
              }
            }
            else {
              $ret = GalleryEmbed::addExternalIdMapEntry($rid, $g2_group->id, 'GalleryGroup');
              if ($ret) {
                gallery_error(t('Error creating Drupal role <> Gallery group mapping (Drupal Role Id: :rid, Gallery Group Id: :gid)',
                  array(':rid' => $rid, ':gid' => $g2_group->id)), $ret);
                return;
              }
            }
          }
          break;
      }
    }
    else {
      // Update group name if needed
      list($ret, $g2_group) = GalleryCoreApi::loadEntityByExternalId($rid, 'GalleryGroup');
      if ($ret) {
        gallery_error(t('Error retrieving Gallery Group Id from Drupal Role Id (Drupal Role Id: :rid)',
          array(':rid' => $rid)), $ret);
        return;
      }
      if (!in_array($rid, array(DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID, $admin_role)) && ($role_name != $g2_group->getGroupName())) {
        $ret = GalleryEmbed::updateGroup($rid, array('groupname' => $role_name));
        if ($ret) {
          gallery_error(t('Error updating Gallery group (Drupal Role Id: :rid, Drupal Role Name: :rname)',
            array(':rid' => $rid, ':rname' => $role_name)), $ret);
          return;
        }
      }
    }
  }
  // Now check for any deleted Drupal roles. Only delete those G2 groups that were mapped to Drupal roles
  // (just in case other groups have been defined which are not meant to be sync'd with Drupal)
  if (($g2_groups_map = _gallery_groups_map(array(), TRUE)) === FALSE) {
    return;
  }
  foreach ($g2_groups_map as $rid => $g2_gid) {
    if (!isset($roles[$rid])) {
      $ret = GalleryEmbed::deleteGroup($rid);
      if ($ret) {
        gallery_error(t('Error deleting Gallery group (Gallery Group Id: :gid)',
          array(':gid' => $g2_gid)), $ret);
        return;
      }        
    }
  }
}

/**
 * Function _gallery_groups_map().
 * (fetch 'GalleryGroup' entries from G2 'ExternalIdMap')
 */
function _gallery_groups_map($ids = array(), $inverse = FALSE) {
  // g2Id => externalId (default)
  $match = array('entityType' => 'GalleryGroup');
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
    gallery_error(t('Error fetching \'GalleryGroup\' entries from \'ExternalIdMap\''), $ret);
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
 * Function gallery_groups_map_info().
 * (get info about groups map status)
 */
function gallery_groups_map_info($g2_user, $user) {
  // Get the groups for this G2 user
  list($ret, $g2_user_groups) = GalleryCoreApi::fetchGroupsForUser($g2_user->id);
  if ($ret) {
    gallery_error(t('Error getting Gallery group info for user (:uid)',
      array(':uid' => $g2_user->id)), $ret);
    return;
  }
  gallery_debug($g2_user_groups, t('G2 groups for G2 user (uid: :g2uid)', array(':g2uid' => $g2_user->id)));
  if (($g2_user_groups = _gallery_groups_map(array_keys($g2_user_groups))) === FALSE) {
    return;
  }
  gallery_debug($g2_user_groups, t('Mapped Drupal roles for G2 user (uid: :g2uid)', array(':g2uid' => $g2_user->id)));
  
  $user->roles[DRUPAL_ANONYMOUS_RID] = DRUPAL_ANONYMOUS_RID;
  $user->roles[DRUPAL_AUTHENTICATED_RID] = DRUPAL_AUTHENTICATED_RID;
  gallery_debug($user->roles, t('Drupal roles for Drupal user (uid: :uid)', array(':uid' => $user->uid)));
  
  // Compare number of G2 groups and Drupal roles (of the user)
  $count_g2_groups = count($g2_user_groups);
  $count_roles = variable_get('gallery_user_admin_role', 0) ? count($user->roles) : count($user->roles) + 1;
  return ($count_g2_groups == $count_roles);
}

/**
 * Function _gallery_groups_import().
 * (import Gallery groups into Drupal)
 */
function _gallery_groups_import() {
  // Fetch G2 album names
  list($ret, $g2_groups) = GalleryCoreApi::fetchGroupNames();
  if ($ret) {
    gallery_error(t('Error fetching Gallery Group names'), $ret);
    return FALSE;
  }
  // Exlude 'Everybody' and 'Registered Users' groups
  list($ret, $g2_everybody_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.everybodyGroup');
  if ($ret) {
    gallery_error(t('Error retrieving Gallery group Id for \'Everybody\' group'), $ret);
    return;
  }
  list($ret, $g2_users_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.allUserGroup');
  if ($ret) {
    gallery_error(t('Error retrieving Gallery group Id for \'Registered Users\' group'), $ret);
    return;
  }
  unset($g2_groups[$g2_everybody_gid], $g2_groups[$g2_users_gid]);
  // Check for admin roles mapping
  if (variable_get('gallery_user_admin_role', 0)) {
    list($ret, $g2_admin_gid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.adminGroup');
    if ($ret) {
      gallery_error(t('Error getting \'adminGroup\' id'), $ret);
      return FALSE;
    }
    unset($g2_groups[$g2_admin_gid]);
  }
  // Create missing Drupal roles (using the G2 groupname)
  $roles = user_roles();
  $g2_import_groups = array_diff($g2_groups, $roles);
  foreach ($g2_import_groups as $g2_groupname) {
    db_query("INSERT INTO {role} (name) VALUES ('%s')", $g2_groupname);
  }
  // Map Drupal roles <> Gallery2 group
  _gallery_groups_sync();
    
  return TRUE;
}

/**
 * Function _gallery_groups_submit().
 * (sync Drupal role <> G2 group when Drupal role has changed)
 */
function _gallery_groups_submit($form_id, &$form_values) {
  if (!_gallery_init(TRUE)) {
    return;
  }
  // Drupal roles have changed => resync 
  _gallery_groups_sync();
  
  GalleryEmbed::done();
}
