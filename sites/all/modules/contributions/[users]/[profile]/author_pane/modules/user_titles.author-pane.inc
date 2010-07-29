<?php
// $Id: user_titles.author-pane.inc,v 1.1.2.2 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the user titles module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_titles_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    $variables['user_title'] = user_titles_get_user_title($account_id);
  }
}
