<?php
// $Id: nicemap_admin.inc,v 1.1.2.8 2009/02/11 18:49:15 tmcw Exp $

/**
 * @file
 * Form for clearing the nicemap cache
 */
function nicemap_cache_clear() {
  $form = array();
  $form['info'] = array(
    '#type' => 'item',
    '#value' => "<p>". t('Nicemap stores information it retrieves from each WMS
    server in a cache to eliminate the need to poll the server for information 
    each time a map is generated. You should clear this cache when the server 
    updates its capabilities or the cached information is no longer 
    valid.') ."</p>",
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Clear nicemap cache'),
  );
  return $form;
}

/**
 * Submit handler for nicemap cache clearing.
 */
function nicemap_cache_clear_submit($form, &$form_state) {
  variable_set('nicemap_cache', array());
}

/**
 * Master settings form and testing page
 */
function nicemap_settings() {
  $form = array();
  $form = system_settings_form($form);
  $form['buttons']['#weight'] = 100;

  $form['wms'] = array(
    '#type' => 'fieldset',
    '#title' => t('WMS server'),
  );
  $form['wms']['nicemap_wms_url'] = array(
    '#title'       => t('Map server URL'),
    '#description' => t('The root URL of the map server you intend to use,
                        please include "http://". This may or may not include
                        certain arguments and filenames.'),
    '#default_value' => variable_get('nicemap_wms_url', ''),
    '#type' => 'textfield');
  if ($wms = variable_get('nicemap_wms_url', '')) {
    $spec = nicemap_cache($wms);
    $server_table =
      array(
        array(t('Title')   ,   $spec->identification['title']),
        array(t('Name')   ,    $spec->identification['name']),
        array(t('Abstract')   ,$spec->identification['abstract']),
        array(t('Contact')   , $spec->identification['contact']),
        array(t('URL')   ,     l($spec->identification['url'], $spec->identification['url'])),
        );
    $form['wms']['server_table'] = array(
      '#value' =>  theme_table(
        array(t('Property'), t('Value')),
        $server_table));
    // projections
    $crs = array();
    if (count($spec->crs)) {
      foreach ($spec->crs as $code) {
        $crs[$code] = $code;
      }
    }
    $form['wms']['nicemap_wms_crs'] = array(
      '#type' => 'select',
      '#title' => t('CRS code'),
      '#description' => t('Choose the projection mode.'),
      '#options' => $crs,
      '#default_value' => variable_get('nicemap_wms_crs', ''),
    );

    // layers + styles
    $l = _nicemap_get_layers($spec);
    $settings = variable_get('nicemap_defaults', 
      array('layers' => array(), 'styles' => array(), 'weights' => array()));
    $bgcolor = variable_get('nicemap_bgcolor', 'aabbcc');
    _nicemap_layer_style_form($form, $l, 
      $settings['layers'], $settings['styles'], $settings['weights'], $bgcolor);
  }
  return $form;
}

/**
 * Layer and style settings form, used also by the view form.
 */
function _nicemap_layer_style_form(&$form, $wms_layers, 
  $default_layers, $default_styles, $default_weights, $default_bg) {
  $form['display'] = array(
    '#title' => t('Display options'),
    '#type' => 'fieldset',
  );
  $form['display']['nicemap_bgcolor'] = array(
    '#title' => t('Background color'),
    '#type' => 'textfield',
    '#size' => 6,
    '#maxlength' => 6,
    '#description' => t('Enter an RGB hex value or leave blank to 
      use a transparent background.'),
    '#default_value' => $default_bg,
  );

  $layers = $styles = array();
  foreach ($wms_layers as $layer => $info) {
    if ($info['title']) {
      $layers[$layer] = $info['title'];
    }
    else {
      $layers[$layer] = $layer;
    }
    $styles[$layer] = $info['styles'];
  }

  $weight_options = array();
  $weight_range = max(10, count($wms_layers));
  /*
   * This loop generates weights for the dropdowns in the Views plugin
   * dialog. Drag n' drop is not used because reattaching to the DOM is difficult.
   * the preceding line guarantees that there will be enough weights to weight any
   * number of provided layers.
   */
  for ($i = -round($weight_range / 2); $i <= round($weight_range / 2); $i++) {
    $weight_options[$i] = $i;
  }

  $form['nicemap_defaults'] = array(
    '#tree' => TRUE,
    '#theme' => 'nicemap_settings_layers',
    '#type' => 'fieldset',
    '#title' => t('Layers display options'),
    'layers' => array(),
    'styles' => array(),
    'weights' => array(),
  );

  foreach ($layers as $layer => $name) {
    $form['nicemap_defaults']['layers'][$layer] = array(
      '#type' => 'checkbox',
      '#title' => $name,
      '#default_value' => isset($default_layers[$layer]) ? 
        $default_layers[$layer] : 0,
    );
    // Some WMS Servers don't provide style data.
    if($styles[$layer]) {
      $form['nicemap_defaults']['styles'][$layer] = array(
        '#type' => 'select',
        '#options' => $styles[$layer],
        '#default_value' => isset($default_styles[$layer]) ? 
          $default_styles[$layer] : $styles[$layer][0],
      );
    }
    $form['nicemap_defaults']['weights'][$layer] = array(
      '#type' => 'select',
      '#options' => $weight_options,
      '#default_value' => isset($default_weights[$layer]) ? 
        $default_weights[$layer] : 0,
      '#attributes' => array('class' => 'layer-weight'),
    );
  }
}

/**
 * Style layer settings into a table grid.
 */
function theme_nicemap_settings_layers($form) {
  uasort($form['weights'], 
    create_function('$a, $b', 
      'return $a["#default_value"] > $b["#default_value"];')
  );

  $rows = array();
  foreach (element_children($form['weights']) as $elem) {
    $row = array(
      drupal_render($form['layers'][$elem]),
      drupal_render($form['styles'][$elem]),
      drupal_render($form['weights'][$elem]),
    );
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', array(t('Layer'), 
    t('Style'), t('Weight')), $rows, array('id' => 'wms-layer-selector'));
  drupal_add_tabledrag('wms-layer-selector', 'order', 'self', 'layer-weight');
  $output .= drupal_render($form);
  return $output;
}
