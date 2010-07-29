<?php
// $Id: charts.inc,v 1.23 2008/11/13 01:29:02 brmassa Exp $
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Settings function for Cahrts module.
 */

/**
 * Subform that integartes Color module with Charts settings. Its called
 * inside the Charts settings page, by _charts_settings().
 *
 * @ingroup from
 */
function _charts_color_form(&$form, $form_state, $settings) {
  // The Color module integration starts with a field set
  $form['color'] = array(
    '#attributes'     => array('id' => 'charts_color', 'class' => ' clear-block'),
    '#collapsible'    => TRUE,
    '#type'           => 'fieldset',
    '#title'          => t('Color'),
  );

  // Load some color palettes
  $schemes = _charts_color_palette();
  if (empty($settings['#color_palette'])) {
    $settings['#color_palette'] = '0,0,0,0,0,0,0,0';
  }
  $form['color']['scheme'] = array(
    '#type'           => 'select',
    '#title'          => t('Color set'),
    '#options'        => $schemes,
    '#default_value'  => $settings['#color_palette'],
  );

  // Add palette fields. Since all
  $color_palette = explode(',', $settings['#color_palette']);

  $names = array(
    t('Color %number', array('%number' => 1)),
    t('Color %number', array('%number' => 2)),
    t('Color %number', array('%number' => 3)),
    t('Color %number', array('%number' => 4)),
    t('Color %number', array('%number' => 5)),
    t('Color %number', array('%number' => 6)),
    t('Color %number', array('%number' => 7)),
    t('Color %number', array('%number' => 8)),
  );
  $form['color']['palette'] = array(
    '#prefix'           => '<div id="preview"></div><div id="farbtastic"></div><div id="palette" class="clear-block">',
    '#suffix'           => '</div>',
    '#tree'             => TRUE,
  );
  foreach ($names as $color => $value) {
    $form['color']['palette']['color'. $color] = array(
      '#type'           => 'textfield',
      '#title'          => $value,
      '#default_value'  => empty($color_palette[$color]) ? '336699' : $color_palette[$color],
      '#size'           => 8,
    );
    $color_palette['color'. $color] = $color_palette[$color];
    unset($color_palette[$color]);
  }
  $form['color']['farbtastic'] = array(
    '#prefix'          => '<div id="farbtastic">',
    '#sufix'           => '</div>',
  );

  // Add the necessary JS data into the page
  drupal_add_js(array('charts_color' => array('reference' => $color_palette)), 'setting');
}

/**
 * List all preset color palette
 */
function _charts_color_palette() {
  return array(
    '' => t('Custom'),
    '#ff0000,#00cc00,#0066b3,#ff8000,#ffcc00,#330099,#990099,#ccff00' => t('Primary'),
    '#ff6600,#009999,#1919b3,#ffb200,#ffff00,#660099,#e60066,#33ff00' => t('Secondary'),
//     'ff0000,00cc00,0066b3,ff8000,ffcc00,330099,990099,ccff00' => t('Primary'),
//     'ff6600,009999,1919b3,ffb200,ffff00,660099,e60066,33ff00' => t('Secondary'),
  );
}

/**
 * Generate a generic chart example
 *
 * @param $data
 *   Array (optional). Chart data, following the Charts Schema
 * @return
 *   HTML. The generic Chart example
 */
function _charts_example($data = array()) {
  if (empty($data)) {
    // This will hold the example chart
    // Since the chart is an example, we should provide
    // and example data.
    $data[] = array(
      '#legend'     => 'Profit',
      array('#value' => 35, '#label' => t('Jan')),
      array('#value' => 25, '#label' => t('Feb')),
      array('#value' => 75, '#label' => t('Mar')),
      array('#value' => 50, '#label' => t('Apr')),
      array('#value' => 23, '#label' => t('May')),
      array('#value' => 12, '#label' => t('Jun')),
    );
    $data[] = array(
      '#legend'     => 'Revenue',
      10, 20, 55, 72, 45, 50
    );
    $data['#title'] = 'Testing Chart';
  }
  return charts_chart($data);
}

/**
 * Invoke a hook in all enabled modules that implement it.
 *
 * Its copy of module_invoke_all()
 * @see module_invoke_all()
 */
function _charts_module_invoke_all() {
  $args = func_get_args();
  $hook = $args[0];
  unset($args[0]);
  $return = array();
  foreach (module_implements($hook) as $module) {
    $function = $module .'_'. $hook;
    $result = call_user_func_array($function, $args);
    if (isset($result) && is_array($result)) {
      $return += $result;
    }
    else if (isset($result)) {
      $return[] = $result;
    }
  }

  return $return;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings($form_state) {
  // Get the previously saved data from Data Base
  $settings = variable_get('charts_settings', array());

  $form['chart_example'] = array(
    '#value'          => _charts_example(),
  );

  $charts_info = module_invoke_all('charts_info', 'list');
  foreach ($charts_info as $chart_code => $chart) {
    $chart_names[$chart_code]= $chart['name'];
  }
  asort($chart_names);
  $form['plugin'] = array(
    '#default_value'  => empty($settings['#plugin']) ? '' : $settings['#plugin'],
    '#options'        => $chart_names,
    '#type'           => 'select',
    '#title'          => t('Chart plugin'),
  );
  if (!empty($settings['#plugin'])) {
    asort($charts_info[$settings['#plugin']]['types']);
  }
  $form['type'] = array(
    '#default_value'  => empty($settings['#type']) ? '' : $settings['#type'],
    '#options'        => isset($settings['#plugin']) ? $charts_info[$settings['#plugin']]['types'] : array(),
    '#type'           => 'select',
    '#title'          => t('Chart type'),
  );

  $form['width'] = array(
    '#default_value'  => empty($settings['#width']) ? 400 : $settings['#width'],
    '#description'    => t('The chart width, in pixels'),
    '#size'           => 8,
    '#type'           => 'textfield',
    '#title'          => t('Width'),
  );
  $form['height'] = array(
    '#default_value'  => empty($settings['#height']) ? 200 : $settings['#height'],
    '#description'    => t('The chart height, in pixels'),
    '#size'           => 8,
    '#type'           => 'textfield',
    '#title'          => t('Height'),
  );

  _charts_color_form($form, $form_state, $settings);

  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => t('Save settings'),
  );

  return $form;
}

/**
 * Module settings page. Users can set the default layout
 * of their charts.
 *
 * @ingroup form
 */
function _charts_settings_submit(&$form, &$form_state) {
  $settings = $form_state['values'];

  // Discart all form values non related to charts
  unset($settings['submit']);
  unset($settings['form_id']);
  unset($settings['form_build_id']);
  unset($settings['form_token']);
  unset($settings['op']);

  // Add a '#' in all field names, as they will be used like a
  // Form API field attributes
  foreach ($settings as $index => $value) {
    $settings["#$index"] = $value;
    unset($settings[$index]);
  }

  // Save the Color palette. Firts, filter the data, cluster the results
  // into a single string. Finally, unset all field values, since they
  // are useless
  $settings['#color_palette'] = implode(',', $settings['#palette']);
  unset($settings['#palette']);
  unset($settings['#scheme']);

  // Save the data into database
  variable_set('charts_settings', $settings);
}

/**
 * Theme color form.
 *
 * @ingroup @themeable
 */
function theme__charts_settings($form) {
  // Preview
  $output = '<div id="preview"></div>';

  // The rest of the form
  $output .= drupal_render($form);

  // Add Farbtastic color picker JS
  drupal_add_css('misc/farbtastic/farbtastic.css', 'module', 'all', FALSE);
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'charts') .'/charts_color.js');

  // Add the necessary CSS
  drupal_add_css(drupal_get_path('module', 'charts') .'/charts_color.css');

  // Put everything inside given DIV. Its necessary for JS code
  return $output;
}
