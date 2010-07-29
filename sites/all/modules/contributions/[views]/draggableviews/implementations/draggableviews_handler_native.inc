<?php
// $Id: draggableviews_handler_native.inc,v 1.1.2.1 2009/01/11 20:59:23 sevi Exp $

/**
 * @file
 * The default implementation of draggableviews handlers.
 */

/*
 * Default Implementation: storage done by draggableviews
 */
class draggableviews_handler_native {
  var $view;

  function init($field_name, &$view) {
    $this->field_name = $field_name;
    $this->view       = $view;

    $this->delta = $this->view->field[$field_name]->definition['draggableviews_delta'];
  }

  function save($nid, $value) {
    db_query('DELETE FROM {draggableviews_structure} WHERE vid=%d AND nid=%d AND delta=%d', $this->view->vid, $nid, $this->delta);
    db_query('INSERT INTO {draggableviews_structure}(vid, nid, delta, value) VALUES(%d, %d, %d, %d)', $this->view->vid, $nid, $this->delta, $value);
  }

  function get_form_element($value, $attributes = array()) {
    if (!isset($attributes['class'])) $attributes['class'] = $field['field_name'];

    $options = array();
    for ($i = 0; $i <= 40; $i++) $options[$i] = $i;

    switch ($this->field->content_field['widget']['type']) {
      default:
        return array(
          '#type'  => 'select',
          '#name'  => $attributes['field_name'],
          '#value' => $value,
          // slice out the first and the last element, so we make it possible
          // that new nodes may appear on the very top or the very bottom
          '#options' => $options,
          '#attributes' => array('class' => $attributes['class']),
        );
        break;

      case 'hidden':

        return array(
          '#type' => 'hidden',
          '#name' => $attributes['field_name'],
          '#value' => $value,
          '#attributes' => array('class' => $attributes['class']),
        );
    }
  }

  function get_minimum_value() {
    return -1;
  }
}
