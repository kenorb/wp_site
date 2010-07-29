<?php
// $Id: node_gallery.themes.inc,v 1.3 2008/11/17 01:46:40 wilson98 Exp $

/**
 * @file
 * Node gallery themes. 
 *
 */

function theme_gallery_list($items, $account = NULL) {
  $output = "<div class='gallery-list'>";
  $output .= node_gallery_operations('list', $account);
  if (empty($items)) {
    $output .= t("There's no gallery now.");
  }
  else {
    $output .= theme('item_list', $items, NULL, 'ul', array('class' => 'gallery-cover-list'));
  }
  $output .= "</div>";
  
  return $output;
}

function template_preprocess_gallery_cover_view(&$vars) {
  $gallery = $vars['gallery'];
  $config = $vars['config'];
  if (empty($gallery->cover)) {
    $gallery->cover->filepath = $config->default_cover;
  }
  $gallery->cover->title = $gallery->title;
  $vars['cover_image'] = theme('image_view', $config->image_size['cover'], $gallery->cover);
  $vars['meta_data'] = theme('item_list', node_gallery_meta($gallery));
  $vars['cover_operations'] = node_gallery_operations('cover', $gallery);
}

function node_gallery_meta($gallery) {
  $items[] = format_plural($gallery->image_count, '1 image', '@count images');
  $items[] = t('Created at: !date', array('!date' => format_date($gallery->created, 'custom', 'Y-m-d')));
  
  return $items;
}

function theme_gallery_image_view($type, $image, $config) {
  if ($type == 'cover') {
    if (empty($image)) {
      $image->filepath = $config->default_cover;
    }
  }

  return theme('imagecache', $config->image_size[$type], $image->filepath, $image->title, $image->title, array('class' => 'gallery-image-'. $type));
}

function theme_gallery_teaser($gallery, $config) {
  //cover display
  if ($config->teaser['gallery_display_type'] == 0) {
    foreach ($gallery->images as $image) {
      if ($image->is_cover) {
        $cover = $image;
        break;
      }
    }
    $cover->filepath = empty($cover->filepath) ? $config->default_cover : $cover->filepath;
    $cover->title = $gallery->title;
    $image_tag = theme('image_view', $config->image_size['cover'], $cover);
    return l($image_tag, 'node/'. $gallery->nid, array('html' => TRUE));
  }
  //thumbnails display
  else {
    $display_num = $config->teaser['thumbnails_num'];
    $i = 0;
    foreach ($gallery->images as $image) {
      if ($i < $display_num) {
        $image_tag = theme('image_view', $config->image_size['thumbnail'], $image);
        $items[] = l($image_tag, 'node/'. $gallery->nid, array('html' => TRUE));
      }
    }
    return theme('item_list', $items);
  }
}

function theme_image_view($imagecache, $image) {
  return theme('imagecache', $imagecache, $image->filepath, $image->title, $image->title);
}

function theme_gallery_edit_images_form(&$form) {
  drupal_add_tabledrag('upload-attachments', 'order', 'sibling', 'upload-weight');
  
  $header = array('', t('Delete'), t('Thumbnail'), t('Edit'), t('Weight'), t('Cover'));
  foreach (element_children($form['files']) as $key) {
    // Add class to group weight fields for drag and drop.
    $form['files'][$key]['weight']['#attributes']['class'] = 'upload-weight';

    $row = array('');
    $row[] = drupal_render($form['files'][$key]['remove']);
    $row[] = theme('imagecache', $form['#config']->image_size['thumbnail'], 
      $form['files'][$key]['filepath']['#value'], $form['files'][$key]['filename']['#value'], $form['files'][$key]['filename']['#value']);
    $row[] = drupal_render($form['files'][$key]['edit_form']);
    $row[] = drupal_render($form['files'][$key]['weight']);
    if ($form['is_cover']) {
      $row[] = drupal_render($form['is_cover'][$key]);
    }
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  $output = theme('table', $header, $rows, array('id' => 'upload-attachments'));
  $output .= drupal_render($form);
  return $output;
}

function theme_gallery_images_list($gallery, $config) {
  $output = "<div class='gallery-images-list'>";
  if (!count($gallery->images)) {
    $output .= t('There\'s no image now.');
  }
  else {
    foreach ($gallery->images as $nid => $image) {
      $items[] = theme('gallery_image_thumbnail', $image, $config);
    }
    $output .= theme('item_list', $items);    
  }
  $output .= "</div>";

  return $output;
}

function theme_gallery_image_thumbnail($image, $config) {
  $output = "<div class='image-thumbnail'>";
  $output .= l(theme('image_view', $config->image_size['thumbnail'], $image), 'node/'. $image->nid, array('html' => TRUE));
  $output .= l($image->title, 'node/'. $image->nid);
  $output .= "</div>";
  return $output;
}

function theme_gallery_image_navigator($navigator, $image) {
  $col1 = array('data' => t("Image %current/Total %total", array('%current' => $navigator['current'], '%total' => $navigator['total'])),
    'class' => 'image-navigator-left');
  $col2 = array('data' => l(t('Prev'), 'node/'. $navigator['prev_nid']) .'/'. l(t('Next'), 'node/'. $navigator['next_nid']),
    'class' => 'image-navigator-mid');
  $col3 = array('data' => node_gallery_operations('image', $image), 'class' => 'image-navigator-right');
  $rows[] = array($col1, $col2, $col3);

  return theme('table', NULL, $rows, array('class' => 'image-navigator')); 
}