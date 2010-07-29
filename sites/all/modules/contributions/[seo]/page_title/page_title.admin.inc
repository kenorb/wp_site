<?php
// $Id: page_title.admin.inc,v 1.1.2.3 2008/12/16 17:46:56 njt1982 Exp $

/**
 * @file
 * Admin include file.
 */

/**
 * Displays the form for the standard settings tab.
 *
 * @return
 *   array A structured array for use with Forms API.
 */
function page_title_admin_settings() {
  //Define the titles
  $form['title']['page_title_default'] = array('#type' => 'markup', '#value' => 'Default Pattern');
  $form['title']['page_title_front']   = array('#type' => 'markup', '#value' => 'Frontpage Pattern');

  //Define the 'default' token patterns
  $form['pattern']['page_title_default'] = array('#type' => 'textfield', '#default_value' => variable_get('page_title_default', '[page-title] | [site-name]'), '#size' => 30, '#maxlength' => 256, '#description' => t('This is the pattern used in a situation where a pattern is not defined specifically for a content type below.'), '#required' => TRUE, );
  $form['pattern']['page_title_front']   = array('#type' => 'textfield', '#default_value' => variable_get('page_title_front', '[site-name] | [site-slogan]'),  '#size' => 30, '#maxlength' => 256, '#description' => t('This is the frontpage pattern.'), );

  //Definate the patterns per-node-type
  $types = node_get_types();
  foreach ($types as $type) {
    $key = 'page_title_type_'. $type->type;
    $form['title'][$key]   = array('#type' => 'markup', '#value' => t('Pattern for %type', array('%type' => $type->name)), );
    $form['pattern'][$key] = array('#type' => 'textfield', '#default_value' => variable_get($key, ''), '#size' => 30, '#maxlength' => 256, '#description' => t('If left blank, will inherit from default settings.'), );
  }

  //Add the system buttons to the form
  $form = system_settings_form($form);

  //Overide the theme function back to our own one
  $form['#theme'] = 'page_title_admin_settings';
  return $form;
}


/**
 * Displays the form for the "Content creation types" tab.
 *
 * @return
 *   array A structured form array for use with Forms API.
 */
function page_title_admin_types() {
  $form['page_title_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content creation forms'),
    '#description' => t('Display a "Page title" field for these content types.'),
  );

  $form['page_title_fieldset']['page_title_display'] = array(
    '#type'          => 'checkboxes',
    '#default_value' => variable_get('page_title_display', array()),
    '#options'       => node_get_types('names'),
  );

  // Add save button, etc to the form.
  return system_settings_form($form);
}

