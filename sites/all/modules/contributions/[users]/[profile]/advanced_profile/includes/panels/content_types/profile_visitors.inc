<?php
// $Id: profile_visitors.inc,v 1.1.2.3 2009/01/07 20:01:12 michellec Exp $

/**
 * @file
 * Provides a panels content type containing visitors to the profile page.
 */

/**
 * Implementation of hook_panels_content_types().
 */
function advanced_profile_profile_visitors_panels_content_types() {
  $items['profile_visitors'] = array(
    'title' => t('Profile visitors'),
    'title callback' => 'advanced_profile_profile_visitors_title',
    'content_types' => 'advanced_profile_profile_visitors_content_type',
    'single' => TRUE,
    'render callback' => 'advanced_profile_profile_visitors_content',
  );

  return $items;
}

/**
 * Return "profile visitors" content type.
 */
function advanced_profile_profile_visitors_content_type() {
  return array(
    'description' => array(
      'title' => t('Profile visitors'),
      'icon' => 'icon_user.png',
      'path' => panels_get_path('content_types/user'),
      'description' => t('List of visitors to a profile page.'),
      'required context' => new panels_required_context(t('User'), 'user'),
      'category' => array(t('Advanced Profile Kit'), -9),
    ),
  );
}

/**
 * Output function for the 'profile visitors' content type.
 */
function advanced_profile_profile_visitors_content($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block->module = 'profile_visitors';
  $block->title = "Profile visitors";

  if ($account) {
    $block->content = theme('advanced_profile_visitors', $account);
  }
  else {
    $block->content = t("User information not available");
  }

  return $block;
}

function advanced_profile_profile_visitors_title($conf, $context) {
  return t('"@s" profile visitors', array('@s' => $context->identifier));
}
