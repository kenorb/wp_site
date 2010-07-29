<?php
// $Id: profile.author-pane.inc,v 1.1.2.3 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the profile module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function profile_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    // As of D6, this just loads the content array and doesn't return anything.
    profile_view_profile($variables['account']);

    $variables['profile'] = $variables['account']->content;
  }
}