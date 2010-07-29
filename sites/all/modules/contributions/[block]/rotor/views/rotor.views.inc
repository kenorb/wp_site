<?php
//$Id: rotor.views.inc,v 1.1.2.1 2008/12/20 17:53:55 mrfelton Exp $

/**
 * Implementation of hook_views_plugins
 */
function rotor_views_plugins() {
  $path = drupal_get_path('module', 'rotor');
  $views_path = drupal_get_path('module', 'views');
  require_once "./$path/theme/theme.inc";
  return array(
    'module' => 'rotor',
    'style' => array(
      'rotor' => array(
        'title' => t('Rotor'),
        'help' => t('Displays nodes as a rotoating banner.'),
        'handler' => 'rotor_plugin_style_rotor',
        'path' => "$path/views",
        'theme' => 'rotor_view_rotor',
        'theme file' => 'theme.inc', 
        'theme path' => "$path/theme",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
    'row' => array(
      'unformatted' => array(
        'title' => t('Rotor'),
        'help' => t('(Displays a Rotor Item ready to be used in a Rotor block'),
        'handler' => 'rotor_plugin_row_rotor',
        'path' => "$path/views",
        'theme' => 'rotor_row_rotor',
        'theme file' => 'theme.inc',
        'theme path' => "$path/theme",
        'uses fields' => FALSE,
        'uses options' => FALSE,
        'type' => 'normal',
      ),
    )
  );
}

/**
 * Implementation of hook_views_data().
 */
/*
function rotor_views_data() {
  $data['rotor']['image'] = array(
    'title' => t('Rotor image'),
    'help' => t('The Rotor image'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  return $data;
}
*/
