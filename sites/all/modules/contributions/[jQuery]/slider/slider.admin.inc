<?php
// $Id: slider.admin.inc,v 1.1 2008/10/15 10:04:25 marktheunissen Exp $

/**
 * @file
 * Administration callbacks for the slider module.
 */

/**
 * Form builder. Configure slider.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function slider_admin_settings() {
  // get an array of node types with internal names as keys
  // and friendly names as values
  $options = node_get_types('names');

  // remove the node types that don't have a cck field called slider_content.
  foreach ($options as $key => $val) {
    $field = content_fields('field_slider_content', $key);
    if ($field['type'] != 'nodereference') {
      unset($options[$key]);
    }
  }

  $form['slider_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('The following nodes will be converted to sliders.'),
    '#options' => $options,
    '#default_value' => variable_get('slider_node_types', array()),
    '#description' => t('The node must have a nodereference CCK multifield called field_slider_content.'),
  );

  $form['slider_tabs_position'] = array(
    '#type' => 'radios',
    '#title' => t("The location of the tabs."),
    '#options' => array(t('Above'), t('Below')),
    '#default_value' => variable_get("slider_tabs_position", 0),
    '#description' => t('Should the tabs display above or below the content?.'),
  );

  return system_settings_form($form);
}
