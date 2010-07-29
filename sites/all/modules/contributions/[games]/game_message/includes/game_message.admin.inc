<?php
// $Id: game_message.admin.inc,v 1.1 2009/01/15 03:47:18 aaron Exp $

/**
 *  @file
 *  The administration page for the Game Utilities: Message module.
 *  @TODO add a 'clear all messages' button & confirmation.
 *  @TODO add an 'imperative: clear all messages now, you had no business
 *  doing that' message if the value changes and there are messages in the
 *  database.
 */

/**
 *  Callback for admin/settings/game_message.
 */
function game_message_settings_form() {
  $form = array();

  $form['game_utility_character_objects'] = array(
    '#type' => 'radios',
    '#title' => t('Game objects').
    '#default_value' => variable_get('game_utility_character_objects', 'user'),
    '#options' => array('user' => t('Users'), 'node' => t('Nodes')),
    '#required' => TRUE,
    '#description' => t('This determines what type of Drupal objects are used for game characters when messaging, for purposes of storing messages in the database. It is important not to change this value lightly, as the tables will become disassociated with the current game state. If you do change the value in-game, you should also clear all game messages. If set as Users, then the default character when messaging will be the logged in user for the current page load. If set as Nodes, then the default character will be the active character node, assuming the !game_character module is installed and enabled.', array('!game_character' => l(t('Game Utilities: Character Node'), 'http://drupal.org/project/game_character'))),
  );

  if (variable_get('game_utility_character_objects', 'user') == 'node' && (!module_exists('game_character'))) {
    drupal_set_message('You may wish to consider installing and enabling the !game_character module, which will automatically work in conjunction with the game messaging system when using node types as game character types.', array('!game_character' => l(t('Game Utilities: Character Node'), 'http://drupal.org/project/game_character')));
  }

  return system_settings_form($form);
}
