<?php
// $Id: better_formats_settings.admin.inc,v 1.6 2008/12/26 03:33:17 dragonwize Exp $

/**
 * @file
 * Contains FAPI and theme functions for the format settings form.
 */

/**
 * FAPI form builder for admin/settings/filters/settings page.
 * 
 * @see better_formats_menu()
 */
function better_formats_settings_admin_form() {
  $form = array();
  
  $key = 'better_formats_settings_display';
  $form[$key] = array(
    '#type' => 'fieldset',
    '#title' => t('Display'),
  );
  $form[$key]['better_formats_fieldset_title'] = array(
    '#type'  => 'textfield',
    '#default_value' => variable_get('better_formats_fieldset_title', ''),
    '#title' => t('Selection title'),
    '#description' => t('Change the format selection title. Defaults to "Input format"'),
  );
  $form[$key]['better_formats_long_tips_link_text'] = array(
    '#type'  => 'textfield',
    '#default_value' => variable_get('better_formats_long_tips_link_text', ''),
    '#title' => t('More format tips link text'),
    '#description' => t('Change the format full tips link text. Defaults to "More information about formatting options"'),
  );
  
  
  $key = 'better_formats_settings_control';
  $form[$key] = array(
    '#type' => 'fieldset',
    '#title' => t('Control'),
  );
  $form[$key]['better_formats_per_node_type'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Control formats per node type'),
    '#description' => t('Control formats allowed and default formats per node type. Global settings will be used until a content type admin page is saved with different values.'),
    '#default_value' => variable_get('better_formats_per_node_type', 0),
  );
  
  $key = 'better_formats_settings_advanced';
  $form[$key] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form[$key]['better_formats_debug'] = array(
    '#type'  => 'checkbox',
    '#title' => t('Debug mode'),
    '#description' => t('Turn on debug mode. This will output information to the message area that should be filed with issue reports when asked. Do not leave this turned on.'),
    '#default_value' => variable_get('better_formats_debug', 0),
  );
  
  return system_settings_form($form);
}