<?php
// $Id: menu.inc,v 1.1.2.6 2008/12/16 13:55:06 robrechtj Exp $

/**
 * @file
 * Support file for the core menu module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function menu_node_import_fields($type) {
  $fields = array();
  return $fields; //TODO: reenable this

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $fields['menu'] = array(
      'title' => t('Menu settings'),
      'group' => t('Menu settings'),
      'is_mappable' => user_access('administer menu'),
      'columns' => array(
        'link_title' => array(
          'title' => t('Menu link title'),
        ),
        'parent' => array(
          'title' => t('Parent item'),
        ),
        'weight' => array(
          'title' => t('Weight'),
        ),
      ),
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function menu_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();
  return $form; //TODO: reenable this

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (user_access('administer menu')) {
      $menu_options = menu_parent_options(menu_get_menus(), array('mlid' => 0));
      $form['menu'] = array(
        '#title' => t('Menu settings'),
        'parent' => array(
          '#title' => t('Parent'),
          '#type' => 'select',
          '#options' => $menu_options,
          '#default_value' => isset($defaults['menu']['parent']) ? $defaults['menu']['parent'] : 'primary-links:0',
        ),
        'weight' => array(
          '#title' => t('Weight'),
          '#type' => 'weight',
          '#default_value' => isset($defaults['menu']['weight']) ? $defaults['menu']['weight'] : 0,
        ),
      );
    }
  }

  return $form;
}

