<?php
// $Id: book.inc,v 1.1.4.6 2008/12/16 13:55:06 robrechtj Exp $

/**
 * @file
 * Support file for the core book module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function book_node_import_fields($type) {
  $fields = array();
  return $fields; //TODO: reenable this

  if (($node_type = node_import_type_is_node($type)) !== FALSE && book_type_is_allowed($node_type)) {
    $fields['book'] = array(
      'title' => t('Book outline'),
      'group' => t('Book outline'),
      'is_mappable' => user_access('administer book outlines') || user_access('add content to books'),
      'columns' => array(
        'bid' => array(
          'title' => t('Book'),
        ),
        'plid' => array(
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
function book_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();
  return $form; //TODO: reenable this

  if (($node_type = node_import_type_is_node($type)) !== FALSE && book_type_is_allowed($node_type)) {
    if (user_access('administer book outlines') || user_access('add content to books')) {
      $book_options = array(
        '' => '<'. t('create a new book') .'>',
        0 => '<'. t('none') .'>',
      );
      foreach (book_get_books() as $book) {
        $book_options[$book['nid']] = $book['title'];
      }
      $form['book'] = array(
        '#title' => t('Book outline'),
        'bid' => array(
          '#title' => t('Book'),
          '#type' => 'select',
          '#options' => $book_options,
          '#default_value' => isset($defaults['book']['bid']) ? $defaults['book']['bid'] : '',
        ),
        //TODO: can we default the parent item too?
        'weight' => array(
          '#title' => t('Weight'),
          '#type' => 'weight',
          '#default_value' => isset($defaults['book']['weight']) ? $defaults['book']['weight'] : 0,
        ),
      );
    }
  }

  return $form;
}

