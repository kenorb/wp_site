<?php
// $Id: node_gallery.admin.inc,v 1.3 2008/11/17 01:46:40 wilson98 Exp $

/**
 * @file
 * Node gallery admin file. 
 *
 */

function node_gallery_config_list() {
  $items = GalleryConfigGateway::getTypes('gallery');
  $node_types = node_get_types();
  
  if (!empty($items)) {
    $headers = array(t('Gallery Name'), t('Gallery type'), t('Image type'), t('Operations'));
    foreach ($items as $item) {
      $rows[] = array($item->name, $node_types[$item->gallery_type]->name,
        $node_types[$item->image_type]->name, theme('links', node_gallery_config_operations($item)));
    }
    return theme('table', $headers, $rows, array('class' => 'node-gallery-config-list'));
  }
  else {
    return t("There's no gallery type now.");
  }
}

function node_gallery_config_operations($config) {
  $items[] = array('title' => t('Edit'), 'href' => 'admin/build/node_gallery/edit/'. $config->gallery_type);
  $items[] = array('title' => t('Delete'), 'href' => 'admin/build/node_gallery/delete/'. $config->gallery_type);
  
  return $items;
}

function node_gallery_config_form($form_state, $gallery_config = NULL) {
  $form = array();
  static $node_types, $image_types, $imagecaches;
  
  if (empty($imagecaches)) {
    foreach (imagecache_presets() as $id => $imagecache) {
      $imagecaches[$imagecache['presetname']] = $imagecache['presetname'];
    }
  }
  if (empty($imagecaches)) {
    form_set_error('Node gallery config', t('No imagecache preset now. You must set at least one.'));
  }
  
  if (empty($gallery_config)) {
    drupal_set_title(t('Add Gallery Config'));
  }
  else {
    drupal_set_title(t('Edit %name', array('%name' => $gallery_config->name)));
  }
  $gallery_config = empty($gallery_config) ? array() : (array) $gallery_config;
  $gallery_config = empty($form_state['storage']) ? $gallery_config : array_merge($gallery_config, $form_state['storage']);
  $gallery_config = empty($form_state['values']) ? $gallery_config : array_merge($gallery_config, $form_state['values']);
  
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Gallery Type Name'),
    '#description' => t('The human-readable gallery type name. It may be up to 32 characters long and my only contain lowercase letters, underscores, and numbers.'),
    '#maxlength' => 32,
    '#required' => TRUE,
    '#default_value' => $gallery_config['name']
  );
  
  $step = empty($form_state['values']['step']) ? 1 : $form_state['values']['step'] + 1;
  
  $form['step'] = array(
    '#type' => 'hidden',
    '#value' => $step,
  );
  
  if ($step == 1) {
    if (empty($node_types)) {
      foreach (node_get_types() as $node_type) {
        $node_types[$node_type->type] = $node_type->name;
      }
    }
  
    $form['gallery_type'] = array(
      '#type' => 'radios',
      '#title' => t('Gallery Type'),
      '#options' => $node_types,
      '#description' => t('Select which content type should be used as gallery type.'),
      '#default_value' => $gallery_config['gallery_type'],
      '#required' => TRUE,
    );
    $form['image_type'] = array(
      '#type' => 'radios',
      '#title' => t('Image Type'),
      '#options' => $node_types,
      '#description' => t('Select which content type should be used as image type.'),
      '#default_value' => $gallery_config['image_type'],
      '#required' => TRUE,
    );
    
    $form['next'] = array(
      '#type' => 'submit',
      '#value' => t('Next'),
      '#submit' => array('node_gallery_config_form_next_submit'),
    );
  }
  else {
    /*cck support*/
    if (module_exists('content')) {
      content_clear_type_cache();
      $image_type = content_types($form_state['storage']['image_type']);
      //non-cck fields
      foreach ($image_type['extra'] as $name => $field) {
        if ($name != 'menu' && $name != 'attachments') {
          $image_type_fields[$name] = $field['label'];
        }
      }
      //cck fields
      foreach ($image_type['fields'] as $name => $field) {
        $image_type_fields[$name] = $field['widget']['label'];
      }
    }
    else {
      $node_fields = node_gallery_get_type_fields($form_state['storage']['image_type']);
      foreach ($node_fields as $name => $field) {
        $image_type_fields[$name] = $field['label'];
      }
    }
    
    $form['data'] = array(
      '#tree' => TRUE,
    );
    
    $form['data']['display_fields'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Fields'),
      '#options' => $image_type_fields,
      '#description' => t('Specify what field should be displayed.'),
      '#required' => TRUE,
      '#default_value' => empty($gallery_config['display_fields']) ? array() : $gallery_config['display_fields'],
    );
    
    $form['data']['gallery_directory'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery Directory'),
      '#description' => t('Specify the directory of the gallery in the file system. The root is \'files\'.
      Use %uid for user_id, %username for username, %gid for gallery_id, %gallery_name for gallery_name.'),
      '#default_value' => $gallery_config['gallery_directory']
    );
    
    $form['data']['default_cover'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Cover Image'),
      '#description' => t('Specify the default cover image to show when there is no image in gallery. The path starts with your drupal root.'),
      '#default_value' => $gallery_config['default_cover'],
    );

    $form['data']['image_size'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image Sizes'),
      '#description' => t('Specify what imagecache preset you wish to use to display the image.'),
      '#tree' => TRUE,
    );
    $form['data']['image_size']['cover'] = array(
      '#type' => 'select',
      '#title' => t('Cover'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['image_size']['cover'],
    );
    $form['data']['image_size']['thumbnail'] = array(
      '#type' => 'select',
      '#title' => t('Thumbnail'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['image_size']['thumbnail'],
    );
    $form['data']['image_size']['preview'] = array(
      '#type' => 'select',
      '#title' => t('Preview'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['image_size']['preview'],
    );
    
    $form['data']['teaser'] = array(
      '#type' => 'fieldset',
      '#title' => t('Teaser Setting'),
      '#description' => t('Specify the display of gallery and image when teaser.'),
    );
    $form['data']['teaser']['gallery_display_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display Type'),
      '#options' => array(t('Cover'), t('Thumbnails')),
      '#default_value' => empty($gallery_config['teaser']['gallery_display_type']) ? 0 : $gallery_config['teaser']['gallery_display_type'],
    );
    $form['data']['teaser']['thumbnails_num'] = array(
      '#type' => "textfield",
      '#title' => t('Display Thumbnails Number'),
      '#description' => t('If you select gallery display type is thumbnails above, enter the number you want to display when teaser.'),
      '#default_value' => $gallery_config['teaser']['thumbnails_num'],
    );
    $form['data']['teaser']['image'] = array(
      '#type' => 'select',
      '#title' => t('Image Size'),
      '#options' => $imagecaches,
      '#default_value' => $gallery_config['teaser']['image_size'],
    );
    
    $form['data']['view_original'] = array(
      '#type' => 'radios',
      '#title' => t('View Original Image'),
      '#options' => array(t('Disable'), t('Enable')),
      '#description' => t('Specify whether allow or not to view original image.'),
      '#default_value' => empty($gallery_config['view_original']) ? 0 : $gallery_config['view_original'],
    );
    
    $form['data']['content_display'] = array(
      '#type' => 'radios',
      '#title' => t('Choose the content you want to display'),
      '#options' => array('gallery' => t('Gallery'), 'image' => t('Images')),
      '#description' => t("If you select 'Gallery', the content of the Gallery will be used. If you select 'Images', the 
      content of the Images will be used. This let you choose if you want the content to be different for each image or not."),
      '#default_value' => empty($gallery_config['content_display']) ? 'image' : $gallery_config['content_display'],
    );
    
    $form['data']['image_comment'] = array(
      '#type' => 'radios',
      '#title' => t('Comment to image'),
      '#options' => array(t('Disabled'), t('Read only'), t('Read/Write')),
      '#description' => t('Choose whether or not to allow comment to images.'),
      '#default_value' => empty($gallery_config['image_comment']) ? 2 : $gallery_config['image_comment'],
    );
    
    $form['previous'] = array(
      '#type' => 'submit',
      '#value' => t('Previous'),
      '#submit' => array('node_gallery_config_form_pre_submit'),
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }
  
  return $form;
}

function node_gallery_config_form_validate($form, &$form_state) {
  if ($form_state['values']['next'] == t('Next')) {
    if ($form_state['values']['gallery_type'] == $form_state['values']['image_type']) {
      unset($form_state['values']['step']);
      form_set_error('Type', t('The gallery type and image type can\'t be the same.'));
    }
  }
}

function node_gallery_config_form_next_submit($form, &$form_state) {
  $form_state['storage'] = empty($form_state['storage']) ? $form_state['values'] : array_merge($form_state['storage'], $form_state['values']);  
}

function node_gallery_config_form_pre_submit($form, &$form_state) {
  $form_state['storage'] = empty($form_state['storage']) ? $form_state['values'] : array_merge($form_state['storage'], $form_state['values']);
  unset($form_state['values']['step']);  
}

function node_gallery_config_form_submit($form, &$form_state) {
  $submit_values = array_merge($form_state['storage'], $form_state['values']);
  
  $gallery_config = new GalleryConfig($submit_values);
  $gallery_config->save();
  
  unset($form_state['storage']);
  $form_state['rebuild'] = FALSE;
  $form_state['redirect'] = 'admin/build/node_gallery';
}

function node_gallery_config_delete_form($form_state, $gallery_config) {
  $form['gallery_type'] = array(
    '#type' => 'value',
    '#value' => $gallery_config->gallery_type,
  );
  
  return confirm_form($form, t('Are you sure you want to delete gallery type config %name?', array('%name' => $gallery_config->gallery_type)),
   'admin/build/node_gallery', NULL, t('Delete'), t('Back'));
}

function node_gallery_config_delete_form_submit($form, &$form_state) {
  $gallery_config = new GalleryConfig($form_state['values']);
  
  if ($gallery_config->delete()) {
    drupal_set_message(t('Gallery type config %name has been deleted.', array('%name' => $gallery_config->gallery_type)));
    watchdog(
      'node_gallery',
      t('Image gallery delete form: %name deleted.', array('%name' => $gallery_config->gallery_type)),
      WATCHDOG_NOTICE
    );
  
    $form_state['redirect'] = 'admin/build/node_gallery';
  }
}

/**
 * copy from cck module.
 *
 * @param unknown_type $type_name
 * @return unknown
 */
function node_gallery_get_type_fields($type_name) {
  $type = node_get_types('type', $type_name);
  $extra = array();

  if ($type->has_title) {
    $extra['title'] = array(
      'label' => $type->title_label,
      'description' => t('Node module form.'),
      'weight' => -5
    );
  }
  if ($type->has_body) {
    $extra['body_field'] = array(
      'label' => $type->body_label,
      'description' => t('Node module form.'),
      'weight' => 0,
      'view' => 'body'
    );
  }
  if (module_exists('locale') && variable_get("language_content_type_$type_name", 0)) {
    $extra['language'] = array(
      'label' => t('Language'),
      'description' => t('Locale module form.'),
      'weight' => 0
    );
  }
  if (module_exists('taxonomy') && taxonomy_get_vocabularies($type_name)) {
    $extra['taxonomy'] = array(
      'label' => t('Taxonomy'),
      'description' => t('Taxonomy module form.'),
      'weight' => -3
    );
  }

  return $extra;
}