<?php
// $Id: gallery_block_admin.inc,v 1.1.2.9 2009/01/25 14:21:32 profix898 Exp $

require_once(drupal_get_path('module', 'gallery') .'/gallery_settings.inc');

/**
 * gallery.module : gallery_block_admin.inc
 * Gallery block settings
 */

/**
 * Function _gallery_block_admin_navigation().
 */
function _gallery_block_admin_navigation() {
  $form['gallery_block_navigation_cache'] = array(
    '#type' => 'select',
    '#title' => t('Block cache'),
    '#default_value' => variable_get('gallery_block_navigation_cache', BLOCK_NO_CACHE),
    '#options' => array(
      BLOCK_NO_CACHE => t('Cache disabled'),
      BLOCK_CACHE_PER_USER | BLOCK_CACHE_PER_PAGE => t('Cache per user and page'),
      BLOCK_CACHE_PER_ROLE | BLOCK_CACHE_PER_PAGE => t('Cache per role and page')
    ),
    '#description' => t('Configure cache settings for this block. It\'s usually safe to set this to \'Cache per user and page\'.')
  );

  $form['element'] = array('#type' => 'value', '#value' => 'gallery_block_navigation');
  return $form;
}

/**
 * Function _gallery_block_admin_block().
 */
function _gallery_block_admin_block($delta) {
  $block_map = _gallery_block_map(TRUE);
  $block_type = variable_get('gallery_block_'. $delta .'_type', 'imageblock');
  $plugin = $block_map[$block_type]['plugin'];
  $plugin_block = $block_map[$block_type]['block'];
  
  // General block settings (common for all G2 block types)
  $form['gallery_block_'. $delta .'_blockid'] = array(
    '#type' => 'textfield',
    '#title' => t('Block Identifier [Block @id]', array('@id' => $delta)),
    '#default_value' => variable_get('gallery_block_'. $delta .'_blockid', ''),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t('A short identifier to distinguish blocks of the same type. (Blocks are numbered by default)')
  );
  $cache_options = array(
      BLOCK_NO_CACHE => t('Cache disabled'),
      BLOCK_CACHE_PER_USER | BLOCK_CACHE_PER_PAGE => t('Cache per user and page'),
      BLOCK_CACHE_PER_ROLE | BLOCK_CACHE_PER_PAGE => t('Cache per role and page'),
      BLOCK_CACHE_PER_USER => t('Cache per user'),
      BLOCK_CACHE_PER_ROLE => t('Cache per role'),
      BLOCK_CACHE_GLOBAL => t('Cache globally (use carefully)')
  );
  // Make sure the cache options are compatible with the block parameters
  if (count($block_map[$block_type]['params'])) {
    unset($cache_options[BLOCK_CACHE_GLOBAL]);
    // If the block is item-specific we must (at least) cache per page
    if (in_array('item', $block_map[$block_type]['params'])) {
      unset($cache_options[BLOCK_CACHE_PER_ROLE], $cache_options[BLOCK_CACHE_PER_USER]);
    }
  }
  $form['gallery_block_'. $delta .'_cache'] = array(
    '#type' => 'select',
    '#title' => t('Block cache'),
    '#default_value' => variable_get('gallery_block_'. $delta .'_cache', BLOCK_NO_CACHE),
    '#options' => $cache_options,
    '#description' => _gallery_block_admin_description($block_type .'_cache'),
  );
  
  // Block type selector
  $block_list = array();
  foreach ($block_map as $type => $details) {
    $block_list[$type] = $details['block'];
  }
  $form['gallery_block_'. $delta .'_type'] = array(
    '#type' => 'select',
    '#title' => t('Block type'),
    '#default_value' => $block_type,
    '#options' => $block_list,
    '#description' => t('Select the Gallery2 block to be displayed in this Drupal block. In previous versions only
                         \'ImageBlock\' was available, but now you can embed any Gallery2 block in Drupal.'),
  );
  
  // Block specific settings
  if ($param_form = _gallery_block_admin_block_params($delta, $plugin, $plugin_block)) {
    $form[$block_type] = array(
      '#type' => 'fieldset',
      '#title' => t('@block settings', array('@block' => $plugin_block)),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE
    );
    $form[$block_type][] = $param_form;
  }
  
  // Save/Edit button
  $form['element'] = array('#type' => 'value', '#value' => 'gallery_block_'. $delta);
  $form['update_btn'] = array(
    '#type' => 'submit',
    '#value' => t('Save and Edit'),
    '#submit' => array('_gallery_block_admin_update')
  );
  
  return $form;
}

/**
 * Function _gallery_block_admin_description().
 */
function _gallery_block_admin_description($element) {
  switch ($element) {
    case 'imageblock_cache':
      return t('Configure cache settings for this block. Unless random images are used you can safely keep this to
                \'Cache per user and page\'. If you dont have per-user permissions configured in your gallery you
                can improve performance by setting to \'Cache per role and page\'.');
    default:
      return '';
  }
}

/**
 * Function _gallery_block_admin_block_params().
 */
function _gallery_block_admin_block_params($delta, $plugin, $plugin_block) {
  $g2_blocks = gallery_list_blocks();
  
  // Image block specific settings
  if ($plugin_block == 'ImageBlock') {
    $element = 'gallery_block_'. $delta .'_'. strtolower($plugin_block);
    return _gallery_block_admin_imageblock($element);
  }
  
  // Block specific settings
  if (isset($g2_blocks[$plugin][$plugin_block]['vars'])) {
    // Generate a Drupal form element for each G2 block variable
    foreach ($g2_blocks[$plugin][$plugin_block]['vars'] as $parameter => $options) {
      $block = strtolower($plugin_block .'_'. $parameter);
      $element = 'gallery_block_'. $delta .'_'. $block;
      // Common form element properties
      $form[$element] = array(
        '#title' => $options['description'],
        '#default_value' => variable_get($element, $options['default']),
        '#description' => _gallery_block_admin_description($block),
      );
      // Form element type specific properties
      switch ($options['type']) {
        case 'boolean':
          $form[$element]['#type'] = 'checkbox';
          break;
        case 'text':
          $form[$element]['#type'] = 'textfield';
          $form[$element]['#size'] = max(10, min(80, strlen($options['default'])));
          $form[$element]['#maxlength'] = 255;
          break;
        case 'choice':
          $form[$element]['#type'] = 'select';
          $form[$element]['#options'] = $options['choices'];
          break;
        default:
      }
    }
    return $form;
  }

  return FALSE;
}

/**
 * Function _gallery_block_admin_imageblock().
 */
function _gallery_block_admin_imageblock($element, $title = 'Gallery Image/Grid Block', $hide_options = NULL) {
  $plugin_status =  gallery_plugin_status(array('imageblock', 'imageframe'));
  $form['#description'] = t('The @title requires the Gallery2 Image Block plugin (!imageblock_status) and optionally
                             the Gallery2 Image Frame plugin (!imageframe_status).', array(
                                '@title' => $title,
                                '!imageblock_status' => theme('gallery_plugin_status_message', $plugin_status['imageblock']),
                                '!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']))
                           );
  
  if ($plugin_status['imageblock'] != GALLERY_PLUGIN_ENABLED) {
    $form['#description'] .= t(' However the Image Block plugin is unavailable.');
    return $form;
  }
  
  $form['imageblock'] = array('#tree' => TRUE);
  $form['imageblock']['element'] = array('#type' => 'value', '#value' => $element);
  
  $num_cols = variable_get($element .'_num_cols', 2);
  $num_rows = variable_get($element .'_num_rows', 2);
  $numimages = $num_cols * $num_rows;
  
  $form['imageblock'][$element .'_num_cols'] = array(
    '#type' => 'select',
    '#title' => t('Number of columns'),
    '#default_value' => variable_get($element .'_num_cols', 2),
    '#options' => _gallery_range_array(1, 10),
    '#description' => t('Select the number of columns in the grid.'),
  );
  
  $form['imageblock'][$element .'_num_rows'] = array(
    '#type' => 'select',
    '#title' => t('Number of rows'),
    '#default_value' => variable_get($element .'_num_rows', 2),
    '#options' => _gallery_range_array(1, 10),
    '#description' => t('Select the number of rows in the grid.'),
  );
  
  _gallery_block_options($type_map, $param_map);
  $form['imageblock'][$element .'_block_block'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image types'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#description' => t('Pick the type of images you would like to see. You can select the same type more than once.'),
  );
  $gallery_block_block = variable_get($element .'_block_block', array('randomImage'));
  for ($i=0; $i<$numimages; $i++) {
    $form['imageblock'][$element .'_block_block'][$i] = array(
      '#type' => 'select',
      '#title' => '',
      '#default_value' => isset($gallery_block_block[$i]) ? $gallery_block_block[$i] : NULL,
      '#options' => array_merge(array(NULL => t('None')), $type_map),
    );
  }
  
  $form['imageblock'][$element .'_item_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Album or Item ID'),
    '#default_value' => variable_get($element .'_item_id', ''),
    '#size' => 20,
    '#maxlength' => 20,
    '#description' => t('Enter the Gallery image or album ID (or blank). If an album or item ID is specified, random
                         images will only be selected from that album and its sub-albums. If \'user\' (or \'user:123\')
                         is entered, items will be taken from the current (or specified) user\'s useralbum.')
  );
  
  $form['imageblock'][$element .'_block_show'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Image data'),
    '#default_value' => variable_get($element .'_block_show', array('title', 'heading')),
    '#options' => $param_map,
    '#description' => t('Choose the item metadata you would like to display.'),
  );
  
  $form['imageblock'][$element .'_size_method'] = array(
    '#type' => 'select',
    '#title' => t('Image size method'),
    '#default_value' => variable_get($element .'_size_method', GALLERY_IMAGEBLOCK_SIZE_METHOD_DEFAULT),
    '#options' => array(
      'maxsize' => t('Max Size'),
      'exactsize' => t('Exact Size'),
    ),
    '#description' => t('\'Max Size\' gives faster image downloading, but the image size
                         may be smaller than the size defined below. <br />\'Exact Size\' may be slower
                         (as a larger image is downloaded and then scaled by the browser) but the image
                         will be guaranteed to be the size defined below.'),
  );
  
  $form['imageblock'][$element .'_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Image size'),
    '#default_value' => variable_get($element .'_size', GALLERY_IMAGEBLOCK_SIZE_DEFAULT),
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t('Sets the size (in pixels) of the longest side of the image according to the method defined above.'),
  );
  
  $imageframe_desc = ($plugin_status['imageframe'] != GALLERY_PLUGIN_ENABLED) ?
      t('Requires the Gallery2 Image Frame plugin (!imageframe_status).',
      array('!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']))) : '';

  $image_frames = gallery_list_image_frames();
  
  $form['imageblock'][$element .'_album_frame'] = array(
    '#type' => 'select',
    '#title' => t('Album frame'),
    '#default_value' => variable_get($element .'_album_frame', 'none'),
    '#options' => $image_frames,
    '#description' => $imageframe_desc,
  );
  
  $form['imageblock'][$element .'_item_frame'] = array(
    '#type' => 'select',
    '#title' => t('Item frame'),
    '#default_value' => variable_get($element .'_item_frame', 'none'),
    '#options' => $image_frames,
    '#description' => $imageframe_desc,
  );
  
  $form['imageblock'][$element .'_link_target'] = array(
    '#type' => 'textfield',
    '#title' => t('Link target'),
    '#default_value' => variable_get($element .'_link_target', ''),
    '#size' => 20,
    '#maxlength' => 20,
    '#description' => t('Enter a link target (e.g. \'_blank\' to open in a new window).'),
  );
  
  $form['imageblock'][$element .'_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Link'),
    '#default_value' => variable_get($element .'_link', ''),
    '#size' => 60,
    '#maxlength' => 255,
    '#description' => t('By default the image has a link to the item in the Gallery. This
      can be overridden here (or leave empty for the default). Use \'none\' for no link, or a URL
      to link somewhere else instead.'),
  );
  
  // Remove form elements that should not be available
  if (isset($hide_options) && is_array($hide_options)) {
    foreach ($hide_options as $option) {
      unset($form['imageblock'][$element .'_'. $option]);
    }
  }
  
  return $form;
}

/**
 * Function _gallery_block_admin_imageblock_submit().
 */
function _gallery_block_admin_imageblock_submit($form, &$form_state) {
  _gallery_block_admin_save($form_state['values']);
}

/**
 * Function _gallery_block_admin_imageblock_save().
 */
function _gallery_block_admin_imageblock_save(&$values) {
  $element = isset($values['element']) ? $values['element'] : FALSE;
  if ($element) {
    unset($values['element']);
    // Validate _size variable
    if (isset($values[$element .'_imageblock_size'])) {
      if (!is_numeric($values[$element .'_imageblock_size']) || $values[$element .'_imageblock_size'] < 10) {
        $values[$element .'_imageblock_size'] = 10;
        drupal_set_message(t('Image size must be a number greater than ten pixels.
                              (The value has been updated to \'10\' for your convenience.)'), 'error');
      }
    }
  }
  // Save imageblock variables
  foreach ($values['imageblock'] as $key => $value) {
    if (is_array($value)) {
      $value = array_values(array_filter($value));
    }
    variable_set($key, $value);
  }
  unset($values['imageblock']);
}

/**
 * Function _gallery_block_admin_update().
 */
function _gallery_block_admin_update($form, &$form_state) {
  _gallery_block_admin_save($form_state['values']);
  db_query("UPDATE {blocks} SET title = '%s' WHERE module = '%s' AND delta = '%s'", $form_state['values']['title'], $form_state['values']['module'], $form_state['values']['delta']);
}

/**
 * Function _gallery_block_admin_save().
 */
function _gallery_block_admin_save(&$values) {
  $element = isset($values['element']) ? $values['element'] : FALSE;
  // Save imageblock values (if present)
  if (isset($values['imageblock'])) {
    _gallery_block_admin_imageblock_save($values);
  }
  // Save variables
  if ($element) {
    unset($values['element']);
    foreach ($values as $key => $value) {
      $pos = strpos($key, $element);
      if ($pos !== FALSE && $pos == 0) {
        if (is_array($value)) {
          $value = array_values(array_filter($value));
        }
        variable_set($key, $value);
      }
    }
  }
}
