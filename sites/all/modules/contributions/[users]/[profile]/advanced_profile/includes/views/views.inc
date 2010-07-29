<?php
// $Id: views.inc,v 1.1.4.6 2009/01/05 21:16:18 michellec Exp $

/**
 * @file
 * Handles integration with the views module.
 */

/**
 * Loads the included views
 */
function advanced_profile_views_default_views() {

  $views_dir = drupal_get_path('module', 'advanced_profile') . '/includes/views/';
  $views = array();

  include_once $views_dir . 'views.user-tracker.inc';
  
  if (module_exists("user_relationships_api")) {
    include_once $views_dir . 'views.user-relationships-grid.inc';
  }
  
  //include_once $views_dir . 'views.user-list.inc';

  return $views;
}


/**
 * Themes the user_list view.
 */
function phptemplate_views_view_list_user_list($view, $nodes, $type) {
  $fields = _views_get_fields();

  $taken = array();

  // Set up the fields in nicely named chunks.
  foreach ($view->field as $id => $field) {
    $field_name = $field['field'];
    if (isset($taken[$field_name])) {
      $field_name = $field['queryname'];
    }
    $taken[$field_name] = TRUE;
    $field_names[$id] = $field_name;
  }

  // Set up some variables that won't change.
  $base_vars = array(
    'view' => $view,
    'view_type' => $type,
  );

  foreach ($nodes as $i => $node) {
    $vars = $base_vars;
    $vars['node'] = $node;
    $vars['count'] = $i;
    $vars['stripe'] = $i % 2 ? 'even' : 'odd';
    foreach ($view->field as $id => $field) {
      $name = $field_names[$id];
      $vars[$name] = views_theme_field('views_handle_field', $field['queryname'], $fields, $field, $node, $view);
      if (isset($field['label'])) {
        $vars[$name . '_label'] = $field['label'];
      }
    }
    $items[] = _phptemplate_callback('advanced_profile_view_user-list', $vars);
  }
  if ($items) {
    advanced_profile_add_css();
    return implode("", $items);
  }
}