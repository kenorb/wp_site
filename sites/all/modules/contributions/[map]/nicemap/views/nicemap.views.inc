<?php
// $Id: nicemap.views.inc,v 1.1.2.2 2008/12/18 22:50:11 tmcw Exp $

/**
 * @file
 * Views plugins definition for nicemap.
 */

/**
 * Implementation of hook_views_style_plugins(). Adds view types to views UI interface.
 */
function nicemap_views_plugins() {
  return array(
    'style' => array(
      'nicemap' => array(
        'title' => t('Nice map'),
        'help' => t('Displays content on a WMS map.'),
        'handler' => 'nicemap_views_plugin',
        'theme' => 'views_view_nicemap_map',
        'theme file' => 'nicemap_views.theme.inc',
        'theme path' => drupal_get_path('module', 'nicemap') .'/views',
        'path' => drupal_get_path('module', 'nicemap') .'/views',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'even empty' => TRUE,
      ),
    ),
  );
}
