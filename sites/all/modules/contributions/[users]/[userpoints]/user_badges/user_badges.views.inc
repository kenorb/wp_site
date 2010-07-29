<?php
// $Id: user_badges.views.inc,v 1.1 2008/11/20 17:12:09 heine Exp $

/**
 *  @file
 *  This defines views hooks for the User_badges module. It will be loaded automatically as needed by the Views module.
 */

/**
 * Implementation of hook_views_handlers().
 */
function user_badges_views_handlers() {
  return array(
    'handlers' => array(

      // Field handlers.
      'views_handler_field_user_badges_user_uid' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data()
 */
function user_badges_views_data() {
  // ----------------------------------------------------------------
  // user_badges table

  // Describe the user_badges table.
  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['user_badges_user']['table']['group']  = t('User Badges');

  $data['user_badges_user']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('User Badges'),
    'help' => t('Badges held by users on your site.'),
  );

  $data['user_badges_user']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'node' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    // This goes to the node so that we have consistent authorship.
    'node_revisions' => array(
      'left_table' => 'node',
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Describe the uid column of the user_badges table.
  $data['user_badges_user']['uid'] = array(
    'title' => t('Badges'),
    'help' => t("A User's current badges."), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_user_badges_user_uid',
    ),

  );

  return $data;
}
