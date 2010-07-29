<?php

/**
* Implementation of hook_views_plugins().
*
* Define charts style for Views.
*/
function charts_views_plugins() {
  return array(
    'module' => 'charts',
    'style'  => array(     // Declare the charts style plugin
      'chart' => array(
        'path'            => drupal_get_path('module', 'charts') .'/views',
        'title'           => t('Chart'),
        //'theme'         => 'views_view_chart',
        'help'            => t('Displays the content in several Chart styles.'),
        'handler'         => 'charts_plugin_style_chart',
        'uses row plugin' => FALSE,
        'uses fields'     => TRUE,
        'uses options'    => TRUE,
        'type'            => 'normal',
        'help topic'      => 'style-chart',
      ),
    )
  );
}

