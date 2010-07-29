<?php
// $Id$
/**
 * @file
 * Processing page callback file for the imcontrol module.
 */

/**
 * Menu callback; Process results from the IM Control sign up.
 */
function imcontrol_process() {
  // Session is read from cookie to solve Firefox losing the Drupal login
  sess_read($_COOKIE["imcontrol_live"]);

  // Only proceed with install if user has permission to embed messenger
  if (user_access('embed messenger')) {
    if ($_GET["result"] == "Accepted") {
      db_query(
        'INSERT INTO {imcontrol} (uid, pid) '
          ."VALUES (%d, '%s') "
          ."ON DUPLICATE KEY UPDATE pid = '%s'",
        $GLOBALS['user']->uid,
        $_GET["id"],
        $_GET["id"]
      );

      drupal_set_message(t('Windows Live Messenger IM Control successfully installed.'), 'status');
    }
    else {
      if ($_GET["result"] == "Declined") {
        drupal_set_message(t('Windows Live Messenger IM Control failed to install.'), 'status');
      }
    }
    drupal_goto('user/'. $GLOBALS['user']->uid .'/im');
  }
  drupal_access_denied();
}