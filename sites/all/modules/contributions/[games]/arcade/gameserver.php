<?php

/**
 * @file FlashGames
 * Server script to talk to games
 */



// If we're accessing this file directly, bootstrap Drupal
if (!function_exists(drupal_get_path)) {
  $prefix = "";
  while(!file_exists($prefix ."includes/bootstrap.inc")) {
    $prefix .= "../";
  }
  if ($prefix != "") chdir($prefix);
  include_once("./includes/bootstrap.inc");
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
}
chdir(drupal_get_path('module','arcade'));

//watchdog('arcade','triggered: '. var_export($_POST, TRUE));

//cover for older pnFlashGames that may not use the pn_mods variable
if ($_POST['module']=='pnFlashGames') $_POST['game_protocol'] = 'pnFlashGames';

//Check if we've been sent any requests from a game.
if ($_POST['game_protocol']) {
  include_once("protocols/{$_POST[game_protocol]}.inc");
} elseif ($_POST) {
  include_once("protocols/default.inc");
}