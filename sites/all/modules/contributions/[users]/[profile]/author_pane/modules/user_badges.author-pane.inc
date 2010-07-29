<?php
// $Id: user_badges.author-pane.inc,v 1.1.2.2 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the user badges module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_badges_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    $variables['user_badges'] = user_badges_for_uid($account_id);
  }
}