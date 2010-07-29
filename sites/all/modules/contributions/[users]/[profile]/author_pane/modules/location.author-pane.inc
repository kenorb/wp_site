<?php
// $Id: location.author-pane.inc,v 1.1.2.2 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the location module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function location_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  if ($account_id != 0) {
    $account = $variables['account'];
    if (variable_get('location_display_location', 1) && isset($account->locations) && count($account->locations)) {
      $settings = variable_get('location_settings_user', array());
      $location = location_display($settings, $account->locations);
      $variables['location_user_location'] = $location['#value'];
    }
  }
}