<?php
// $Id: imagebrowser.views.inc,v 1.1.2.4 2008/12/03 22:18:51 starnox Exp $

/**
 * @file
 * Image Browser's views plugins
 */


/**
 * Implementation of hook_views_plugins().
 *
 * Define a row style plugin to display a thumbnail in the image browser.
 */
function imagebrowser_views_plugins() {
  return array(
    'row' => array(
      'imagebrowser_thumbnail' => array(
        'title' => t('Image Browser thumbnail'),
        'handler' => 'views_plugin_row',
        'theme' => 'imagebrowser_thumbnail',
        'theme path' => drupal_get_path('module', 'imagebrowser') .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}