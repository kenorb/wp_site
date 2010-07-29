<?php
// $Id: node_gallery.pages.inc,v 1.3 2008/11/17 01:46:40 wilson98 Exp $

/**
 * @file
 * Node gallery pages. 
 *
 */

function node_gallery_list($account = NULL) {
  $galleries = GalleryGateway::findBy($account->uid);
  
  if (!empty($galleries)) {
    $gids = array_keys($galleries);
    $image_counts = GalleryGateway::countImages($gids);
    $gallery_covers = GalleryGateway::findCovers($gids);
    if (!empty($gallery_covers)) {
      $images = ImageGateway::getFile(array_values($gallery_covers));
    }
    
    foreach ($galleries as $k => $gallery) {
      $galleries[$k]->cover = $images[$gallery_covers[$k]];
      $galleries[$k]->image_count = empty($image_counts[$k]) ? 0 :$image_counts[$k];
    }
    
    foreach ($galleries as $gallery) {
      $config = GalleryConfigGateway::getBy($gallery->type);
      $items[] = theme('gallery_cover_view', $gallery, $config);
    }
  }
  
  return theme('gallery_list', $items, $account);
}

function node_gallery_upload_form($form_state, $gallery) {
  //bad hack, this is for node_gallery_image item_form, since we use ahah in here;
  drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');
  drupal_add_js('misc/autocomplete.js');
  
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['#node'] = $gallery;
  
  $form['upload_wrapper'] = array(
    '#prefix' => "<div id='gallery-upload-wrapper'>",
    '#suffix' => "</div>",
  );
  for ($i = 1; $i <= 5; $i++) {
    $form['upload_wrapper']['uploads-'. $i] = array(
      '#type' => 'file',
      '#title' => t('Please select a file'),
    );
  }
  
  $form['upload_wrapper']['description'] = array(
    '#value' => node_gallery_upload_limits(),
  );
  $form['upload_wrapper']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Submit Files'),
    '#weight' => 20,
    '#ahah' => array(
      'path' => 'node_gallery/upload/js',
      'wrapper' => 'gallery-upload-wrapper',
      'progress' => array('type' => 'bar', 'message' => t('Please wait...')),
    ),
  );
  
  return $form;
}

function node_gallery_upload_form_validate($form, $form_state) {
  //bad hacks, since we use ahah, we have to validate the upload image nodes here.
  if (!empty($form_state['values']['files'])) {
    foreach ($form_state['values']['files'] as $key => $image_form_state) {
      node_validate($image_form_state['edit_form'], $form['files'][$key]['edit_form']);
    }    
  }
}

function node_gallery_upload_limits() {
  global $user;
  
  $limits = _upload_file_limits($user);
  $items[] = t('Your total disk size is %total MB and you have used %used MB',
   array('%total' => format_size($limits['user_size']), '%used' => format_size(file_space_used($user->uid))));
  if ($limits['resolution']) {
    $items[] = t('Images are larger than %resolution will be resized. ', array('%resolution' => $limits['resolution']));
  }
  $items[] = t('The maximum upload size is %filesize.', array('%filesize' => format_size($limits['file_size'])));
  $items[] = t('Only files with the following extensions may be uploaded: %extensions.', array('%extensions' => $limits['extensions']));
  
  return theme('item_list', $items, t('notes'), 'ul', array('class' => 'upload-notes'));
}

function node_gallery_upload_js() {
  $cached_form_state = array();
  $files = array();
  
  // Load the form from the Form API cache.
  if (!($cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state)) || !isset($cached_form['#node']) || !isset($cached_form['upload_wrapper'])) {
    form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    $output = theme('status_messages');
    print drupal_to_js(array('status' => TRUE, 'data' => $output));
    exit();
  }
  
  $form_state = array('values' => $_POST);
  
  //this is upload form, we don't want to show the node's existing images;
  unset($cached_form['#node']->images);
  // Handle new uploads, and merge tmp files into node-files.
  node_gallery_upload_images($cached_form, $form_state);
  
  //if upload failed.
  if (drupal_get_messages(NULL, FALSE)) {
    print drupal_to_js(array('status' => TRUE, 'data' => theme('status_messages').drupal_get_form('node_gallery_upload_form')));
    exit;
  }
  
  $form = node_gallery_edit_images_form($form_state, $cached_form['#node']);
  $cached_form['edit_images'] = $form;
  form_set_cache($_POST['form_build_id'], $cached_form, $cached_form_state);
  // Render the form for output.
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
    '#tree' => FALSE,
    '#parents' => array(),
  );
  //drupal_alter('form', $form, array(), 'upload_js');
  $form_state = array('submitted' => FALSE);
  $form = form_builder('node_gallery_upload_js', $form, $form_state);
  
  print drupal_to_js(array('status' => TRUE, 'data' => theme('status_messages').drupal_render($form)));
  exit;
}

function node_gallery_upload_images(&$form, &$form_state) {
  global $user;

  $limits = _upload_file_limits($user);
  $validators = array(
    'file_validate_extensions' => array($limits['extensions']),
    'file_validate_image_resolution' => array($limits['resolution']),
    'file_validate_size' => array($limits['file_size'], $limits['user_size']),
  );

  // Save new file uploads.
  if (($user->uid != 1 || user_access('upload files'))) {
    $directory = node_gallery_check_directory($form['#node']);
    foreach ($_FILES['files']['name'] as $id => $name) {
      if (!empty($name)) {
        $file = file_save_upload($id, $validators, $directory);
        if (!empty($file)) {
          $file->description = $file->filename;
          $file->weight = 0;
          $file->gid = $form['#node']->nid;
          $form['#node']->images[$file->fid] = $file;
          $form_state['values']['files'][$file->fid] = (array) $file;
        }
      }
    }
  }

  // Order the form according to the set file weight values.
  if (!empty($form_state['values']['files'])) {
    $microweight = 0.001;
    foreach ($form_state['values']['files'] as $fid => $file) {
      if (is_numeric($fid)) {
        $form_state['values']['files'][$fid]['#weight'] = $file['weight'] + $microweight;
        $microweight += 0.001;
      }
    }
    uasort($form_state['values']['files'], 'element_sort');
  }
}

function node_gallery_check_directory($gallery) {
  global $user;
  $gallery_directory = strtr($gallery->getConfig()->gallery_directory, 
    array('%uid' => $user->uid, '%username' => $user->name, '%gid' => $gallery->nid, '%gallery_name' => str_replace(' ', '_', $gallery->title)));
  $directory = rtrim(file_directory_path() .'/'. $gallery_directory, '/\\');
  //recursive mkdir;
  if (!is_dir($directory)) {
    mkdir($directory, 0777, TRUE);
  }
  file_check_directory($directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
  
  return $directory;
}

function node_gallery_edit_images_form($form_state, $gallery) {
  global $user;
   
  $form = array(
    '#theme' => 'gallery_edit_images_form',
    '#cache' => TRUE,
  );
  //if this is new upload, then fid is file id;
  //if this is edit images, then fid is node id;
  //but fid is just an identifier here, so needn't care it.
  foreach ($gallery->images as $fid => $f) {
    $options[$fid] = '';
    if ($f->is_cover) {
      $cover_fid = $fid;
    }
  }
  $form['is_cover'] = array(
    '#type' => 'radios',
    '#default_value' => $cover_fid,
    '#options' => $options,
  );
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $gallery->nid,
  );
  $form['#config'] = $gallery->getConfig();

  if (!empty($gallery->images) && is_array($gallery->images)) {
    $form['files']['#tree'] = TRUE;
    
    foreach ($gallery->images as $key => $file) {
      $file = (object)$file;
      
      $form['files'][$key]['remove'] = array('#type' => 'checkbox', '#default_value' => $file->remove);
      $form['files'][$key]['weight'] = array('#type' => 'weight', '#delta' => count($gallery->images), '#default_value' => $file->weight);
      $form['files'][$key]['filename'] = array('#type' => 'value',  '#value' => $file->filename);
      $form['files'][$key]['filepath'] = array('#type' => 'value',  '#value' => $file->filepath);
      $form['files'][$key]['filemime'] = array('#type' => 'value',  '#value' => $file->filemime);
      $form['files'][$key]['filesize'] = array('#type' => 'value',  '#value' => $file->filesize);
      $form['files'][$key]['fid'] = array('#type' => 'value',  '#value' => $file->fid);
      $form['files'][$key]['gid'] = array('#type' => 'value',  '#value' => $gallery->nid);
      
      //get the required fields for image node edit and saving.
      $file->type = empty($file->type) ? $gallery->getConfig()->image_type : $file->type;
      $file->gid = empty($file->gid) ? $gallery->nid : $file->gid;
      $form['files'][$key]['edit_form'] = node_gallery_image_item_edit_form($file, $gallery->getConfig());
    }
  }
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 20,
    //need this because this form is used in image upload ahah also.
    '#submit' => array('node_gallery_images_edit_submit'),
  );
  
  return $form;
}

function node_gallery_images_edit_submit($form, &$form_state) {
  
  foreach ($form_state['values']['files'] as $fid => $form_values) {
    $form_values += $form_values['edit_form'];
    unset($form_values['edit_form']);
    
    if ($form_values['remove']) {
      $operations[] = array('images_delete_process', array(new Image($form_values)));
    }
    else {
      if ($form_state['values']['is_cover'] == $fid) {
        $form_values['is_cover'] = 1;
      }
      $operations[] = array('images_upload_process', array($form['files'][$fid]['edit_form'], new Image($form_values)));
    }
  }
  $batch = array(
    'operations' => $operations,
    'finished' => 'images_process_finished',
    'title' => t('Processing Image Upload.'),
    'init_message' => t('Images Upload is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Images Upload has encountered an error.'),
  );
  
  batch_set($batch);
  
  $form_state['redirect'] = 'node/'. $form_state['values']['gid'];
}

function node_gallery_image_item_edit_form($image, $config) {
  
  module_load_include('inc', 'node', 'node.pages');
  $form_state = array();
  $display_fields = $config->display_fields;
  $form = drupal_retrieve_form($image->type .'_node_form', $form_state, $image);
  drupal_prepare_form($image->type ."_node_form", $form, $form_state);
  foreach (array_values($display_fields) as $field_name) {
    if (!empty($field_name)) {
      $item_form[$field_name] = (array)get_image_form_item_recursive($form, $field_name); 
    }
  }
  $item_form += (array)get_image_form_value_items($form);
  set_image_form_default_values($item_form, $image, $config);
  
  return $item_form;
}

function get_image_form_item_recursive($form, $field) {
  if ($form[$field]) {
    return $form[$field];
  }
  elseif ($children = element_children($form)) {
    foreach ($children as $child) {
      get_image_form_item_recursive($form[$child], $field);
    }
  }
  return ;
}

function get_image_form_value_items($form) {
  if ($children = element_children($form)) {
    foreach ($children as $child) {
      //todo: didn't recursive;
      if ($form[$child]['#type'] == 'value' || $form[$child]['#type'] == 'hidden') {
        $value_forms[$child] = $form[$child];
      }
    }
  }
  elseif ($form['#type'] == 'value' || $form['#type'] == 'hidden') {
    $value_forms[key($form)] = $form;
  }
  
  return $value_forms;
}

function set_image_form_default_values(&$form, $image, $config) {
  global $user;
  
  $form['title']['#default_value'] = empty($form['title']['#default_value']) ? $image->filename : $form['title']['#default_value'];
  if (!empty($form['body_field'])) {
    $tmp_item = $form['body_field']['body'];
    unset($form['body_field']);
    $form['body'] = $tmp_item;
    $form['body']['#rows'] = 2;
    $form['body']['#default_value'] = empty($form['body']['#default_value']) ? $image->filename : $form['body']['#default_value'];
  }
  if (!empty($form['changed']) && empty($form['changed']['#value'])) {
    $form['changed']['#value'] = time();
  }
  if (user_access('administer nodes')) {
    $form['name'] = array(
      '#type' => 'value',
      '#value' => $user->name,
    );
  }
  if (!empty($form['uid']) && empty($form['uid']['#value'])) {
    $form['uid']['#value'] = $user->uid;
  }
  $form['comment'] = array(
    '#type' => 'value',
    '#value' => $config->image_comment,
  );
}