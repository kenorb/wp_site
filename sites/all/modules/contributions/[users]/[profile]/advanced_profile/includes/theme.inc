<?php
// $Id: theme.inc,v 1.2.4.7 2009/01/05 05:19:40 michellec Exp $

/**
 * @file
 * Holds theme and preprocess functions.
 */

function template_preprocess_advanced_profile_visitors(&$variables) {
  $visitors = advanced_profile_visitors($variables['account']->uid);

  if (!empty($visitors)) {
    $variables['visitors'] = theme_item_list($visitors);
  }
  else {
    $variables['visitors'] = t("No recent visitors.");
  }
}

function advanced_profile_preprocess_user_profile(&$variables) {
  $image_path = advanced_profile_path_to_images();
  $variables['author_pane'] = theme('author_pane', $variables['account'], $image_path, 'advanced_profile_author-pane');
  $variables['profile_visitors'] = theme('advanced_profile_visitors', $variables['account']);
}
