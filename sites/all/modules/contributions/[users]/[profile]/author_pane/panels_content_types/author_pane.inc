<?php
// $Id: author_pane.inc,v 1.1.2.3 2009/01/05 04:38:07 michellec Exp $

/**
 * @file
 *   This file provides a panels content type containing the author pane.
 */

/**
 * Implementation of hook_panels_content_types().
 */
function author_pane_author_pane_panels_content_types() {
  $items['author_pane'] = array(
    'title' => t('Author Pane'),
    'title callback' => 'author_pane_content_type_title',
    'content_types' => 'author_pane_content_type',
    'single' => TRUE,
    'render callback' => 'author_pane_content_type_content',
    'add callback' => 'author_pane_content_type_edit',
    'edit callback' => 'author_pane_content_type_edit',
  );

  return $items;
}

/**
 * Return "author pane" content type.
 */
function author_pane_content_type() {
  return array(
    'description' => array(
      'title' => t('Author Pane'),
      'icon' => 'icon_user.png',
      'path' => panels_get_path('content_types/user'),
      'description' => t('Author related variables gathered from helper modules.'),
      'required context' => new panels_required_context(t('User'), 'user'),
      'category' => array(t('Advanced Profile Kit'), -9),
    ),
  );
}

/**
 * Output function for the 'author pane' content type.
 */
function author_pane_content_type_content($subtype, $conf, $panel_args, $context) {
  $account = isset($context->data) ? drupal_clone($context->data) : NULL;
  $block->module = 'author_pane';
  $block->title = "Author Pane";

  if ($account) {
    $block->content = theme('author_pane', $account, $conf['image_path'], $conf['template_file']);
  }
  else {
    $block->content = "User information not available";
  }

  return $block;
}

function author_pane_content_type_title($conf, $context) {
  return t('"@s" author pane', array('@s' => $context->identifier));
}

/**
 * 'Edit' callback for the author pane content type.
 */
function author_pane_content_type_edit($id, $parents, $conf = array()) {
  // Apply defaults
  if (empty($conf)) {
    $conf = array('title' => '', 'imagepath' => '', 'template' => '');
  }

  $form['image_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Image directory'),
    '#size' => 50,
    '#description' => t('Path from active theme to image directory. If left blank the images in the module directory will be used.'),
    '#default_value' => $conf['image_path'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  $form['template_file'] = array(
    '#type' => 'textfield',
    '#title' => t('Template file'),
    '#size' => 50,
    '#description' => t('Template file to use for the author pane.'),
    '#default_value' => $conf['template_file'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  return $form;
}

