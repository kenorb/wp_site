<?php
// $Id: userpoints.author-pane.inc,v 1.1.2.1 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the userpoints module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function userpoints_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;

  if ($account_id != 0) {
    $variables['userpoints_points'] = userpoints_get_current_points($account_id);

    $categories = userpoints_get_categories();
    foreach ($categories AS $tid => $category) {
      $variables['userpoints_categories'][$category] = userpoints_get_current_points($account_id, $tid);
    }
  }
}

