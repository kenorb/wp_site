<?php
// $Id: game_message.menu.inc,v 1.1 2009/01/15 03:47:18 aaron Exp $

/**
 *  @file
 *  Menu definition for the Game Queue module.
 */

/**
 *  Helper function for game_queue_menu().
 */
function _game_message_menu() {
  $items = array(
    'admin/settings/game_message' => array(
      'title' => 'Game Utilities: Message',
      'description' => 'Administer the game message log.',
      'page callback' => 'drupal_get_form',
      'page arguments' => array('game_message_settings_form'),
      'access arguments' => array('administer game messages'),
      'file' => 'includes/game_message.admin.inc',
    ),
  );
  return $items;
}
