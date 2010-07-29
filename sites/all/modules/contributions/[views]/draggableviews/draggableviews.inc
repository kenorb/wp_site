<?php
// $Id: draggableviews.inc,v 1.7.2.6 2009/01/11 21:06:44 sevi Exp $

/**
 * @file
 * Draggableviews processing functions.
 */

include_once('draggableviews_theme.inc');

/**
 * Build the form
 */
function draggableviews_view_draggabletable_form($form_state, $view) {
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}


/**
 * Implementing hook_submit
 */
function draggableviews_view_draggabletable_form_submit($vars) {
  // check permissions
  if (!user_access('Allow Reordering')) {
    drupal_set_message(t('You are not allowed to reorder nodes.'), 'error');
    return;
  }

  // gather all needed information
  $view = $vars['#parameters'][2]->view;
  $results = $view->result;
  // get input
  $input = $vars['submit']['#post'];

  $info = _draggableviews_info($view);

  if (!isset($info['order'])) return;


  // loop through all resulting nodes
  foreach ($results as $row) {

    if (isset($info['order']['fields'])) {
      $info['input'][$row->nid]['order'][] = $input[$info['order']['fields'][0]['field_name'] .'_'. $row->nid];
    }

    if (isset($info['hierarchy'])) {
      $info['input'][$row->nid]['parent'] = $input[$info['hierarchy']['field']['field_name'] .'_'. $row->nid];
    }
  }

  // build hierarchy
  _draggableviews_build_hierarchy($info);

  // save structure
  _draggableviews_save_hierarchy($info);

  if (isset($info['hierarchy'])) {
    // save expanded/collapsed state
    global $user;

    foreach ($vars['submit']['#post'] AS $key => $val) {
      if (ereg('draggableviews_collapsed_', $key)) {
        $parent_nid = drupal_substr($key, 25);
        db_query("DELETE FROM {draggableviews_collapsed}
                  WHERE uid=%d AND parent_nid=%d",
                  $user->uid, $parent_nid);

        db_query("INSERT INTO {draggableviews_collapsed}
                  (uid, parent_nid, collapsed) VALUES (%d, %d, %d)",
                  $user->uid, $parent_nid, $val);
      }
    }
  }
}

/**
 * Collect all known information in a handy array
 *
 * @param $view
 *   The views object
 *
 * @return
 *   An structured array containt the extracted draggableviews settings
 *   and additional field information.
 *   array(
 *     'order' => array(
 *       'fields' => array(
 *         0 => array(
 *           'field_name'  => field_name
 *           'field_alias' => field_alias
 *           'handler'     => handler,
 *         ),
 *         ..
 *       ),
 *       'visible' => True/False
 *       'minimum_value' => value
 *     ),
 *     'hierarchy' => array(
 *       'field' => array(
 *         'field_name'  => field_name,
 *         'field_alias' => field_alias,
 *         'handler'     => handler,
 *       ),
 *       'visible' => TRUE/FALSE,
 *     ),
 *
 *     'types' = array(
 *       node_type => "root"/"leaf",
 *     ),
 *
 *     'expand_links' = array(
 *       'show'              => TRUE/FALSE,
 *       'default_collapsed' => TRUE/FALSE,
 *     ),
 *
 *     'nodes' => array(
 *       nid1 => array(
 *         field_name1 => value
 *         field_name2 => value
 *          ..
 *       ),
 *       ..
 *     ),
 *   );
 */
function _draggableviews_info(&$view, $prepare_fields = TRUE, $prepare_options = FALSE, $prepare_results = TRUE) {
  $options = &$view->style_plugin->options;
  $fields  = &$view->field;
  $results = &$view->result;

  // build information array
  $info = array();
  if ($prepare_fields && isset($options['tabledrag_order'])) {
    foreach ($options['tabledrag_order'] as $field) {
      $info['order']['fields'][] = array(
        'handler' => _draggableviews_init_handler($field, $view),
        'field_name' => $field['field'],
        'field_alias' => $fields[$field['field']]->field_alias,
      );
    }
    $info['order']['visible'] = strcmp($options['tabledrag_order_visible']['visible'], 'visible') == 0 ? TRUE : FALSE;
    $info['order']['minimum_value'] = $info['order']['fields'][0]['handler']->get_minimum_value();

    if (count($info['order']['fields']) >= 2 && $options['tabledrag_hierarchy']['field'] != 'none') {
      $info['hierarchy'] = array(
        'field' => array(
           'handler' => _draggableviews_init_handler($options['tabledrag_hierarchy'], $view),
           'field_name' => $options['tabledrag_hierarchy']['field'],
           'field_alias' => $fields[$options['tabledrag_hierarchy']['field']]->field_alias,
        ),
        'visible' => strcmp($options['tabledrag_hierarchy_visible']['visible'], 'visible') == 0 ? TRUE : FALSE,
      );
    }
  }
  if ($prepare_fields && isset($options['tabledrag_types'])) {
    foreach ($options['tabledrag_types'] as $type) {
      $info['types'][$type['node_type']] = $type['type'];
    }
  }
  if ($prepare_options && $info['hierarchy']) {
    $info['expand_links'] = array(
      'show'              => strcmp($options['tabledrag_expand']['expand_links'], 'expand_links') == 0 ? TRUE : FALSE,
      'default_collapsed' => strcmp($options['tabledrag_expand']['collapsed'], 'collapsed') == 0 ? TRUE : FALSE,
    );
  }

  $info['nodes'] = array();

  if ($prepare_results && $prepare_fields && $options['tabledrag_order']) {
    // loop through all resulting nodes
    foreach ($results as $row) {
      foreach ($info['order']['fields'] as $field) {
        $info['nodes'][$row->nid]['order'][] = $row->{$field['field_alias']};
      }
      if (isset($info['hierarchy'])) {
        $info['nodes'][$row->nid]['parent'] = $row->{$info['hierarchy']['field']['field_alias']};
      }
    }
  }

  return $info;
}

/**
 * Get instance from handler
 *
 * @param field
 *   The field options specified in the style plugin
 * @param view
 *   The view object
 *
 * @return
 *   An instance of the handler
 */
function _draggableviews_init_handler($field, &$view) {
  $handler_info = draggableviews_discover_handlers($field['handler']);
  require_once($handler_info['path'] .'/'. $handler_info['file']);
  $handler = new $handler_info['handler'];
  $handler->init($field['field'], $view);

  return $handler;
}

/**
 * Analyze structure
 *
 * @param info
 *   The structured information array
 *
 * @return
 *   FALSE if structure is broken
 */
function _draggableviews_analyze_structure(&$info) {
  // calculate depth of all nodes.
  // If one of the parents of a node is broken,
  // the return will be FALSE
  if (!_draggableviews_calculate_depths($info)) {
    //drupal_set_message(t('Draggableviews: Detected broken parents.'), 'error');
    return FALSE;
  }

  // Detect order collisions and solve them
  // If one order of the same depth is found
  // twice the return will be FALSE.
  if (!_draggableviews_detect_order_collisions($info)) {
    //drupal_set_message(t('Draggableviews: Detected order collisions.'), 'error');
    return FALSE;
  }

  foreach ($info['nodes'] as $nid => $prop) {
    // Detect ordering errors
    // The nodes order value has to equal with the
    // parents order value. Otherwise the return will be FALSE.
    if (_draggableviews_check_order($nid, $info) == FALSE) {
      //drupal_set_message(t('Draggableviews: Detected ordering errors.'), 'error');
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Build hierarchy
 *
 * This function also handles broken structures
 *
 * @param info
 *   The structured information array
 */
function _draggableviews_build_hierarchy(&$info) {
  $nodes = &$info['nodes'];
  $input = &$info['input'];

  foreach ($nodes as $nid => $prop) {
    // get depth
    if (($depth = _draggableviews_get_hierarchy_depth($nid, $input, $info)) === FALSE) {
      // Error! The hierarchy structure is broken and could
      // look like the following: (we're currently processing X)
      // A
      //   --X
      // --D
      //
      // The next steps:
      //  1) bring it down to the root level
      //  2) Set order fields to the minimum

      $nodes[$nid]['parent'] = 0;

      // We gracefully sidestep the order-loop
      $depth = -1;

      drupal_set_message("Draggableviews: An error was detected. The structure has been repaired.");
    }

    // Let's take a look at the following expample, to understand
    // what is beeing done.
    //
    // A
    // --B
    // --C
    //   --X
    // --D
    // E
    // Imagine we're currently processing X:
    //
    // We know that X is in depth=3, so we save the received
    // weight value in the 3rd order field of node X.
    //
    // The 2nd order field must inherit the received weight of
    // node C (the next parent). And the 1st order field must
    // inherit the received weight of node A (the parent of C).
    //
    // When we finally order the view by weight1, weight2, weight3 then
    // weight1 and weight2 from node X will always equal with
    // those from node A and B, and weight3 defines the order of the 3rd level.

    $temp_nid = $nid;

    for ($i = $depth; $i >= 0; $i--) {
      // we're operating top-down, so we determine the parents nid by the way

      $nodes[$nid]['order'][$i] = $input[$temp_nid]['order'][0];

      if (isset($info['hierarchy']) && $i > 0) {
        if (!($temp_nid = $input[$temp_nid]['parent'])) {
          // this line should never be reached assumed the depth
          // was calculated correctly.
          drupal_set_message(t('Undefined State called in draggableviews_build_hierarchy(..)'), 'error');
          break;
        }
      }
    }

    if (isset($info['hierarchy']) && $depth > -1) {
      // Simply set the parent value

      $nodes[$nid]['parent'] = $input[$nid]['parent'];
    }

    // Now set all unused weights to a minimum value. Otherwise
    // it could happen that a child appears above its parent.
    // The ? can be anything, unfortunately also > 5
    //
    // --A (3,5)
    // B   (3,?)
    //
    // To guaranteer that the ? is always the lowest, we choose
    // the minimum.
    // Due to this it's recommended that all order fields have
    // the same minimum value!

    $depth = ($depth == -1) ? 0 : $depth;

    for ($i = $depth + 1; $i < count($info['order']['fields']); $i++) {
      $info['nodes'][$nid]['order'][$i] = $info['order']['fields'][$i]['handler']->get_minimum_value();
    }
  }
}

/**
 * Rebuild hierarchy
 *
 * This function is called when the structure is broken
 *
 * @param info
 *   The structures information array
 */
function _draggableviews_rebuild_hierarchy(&$info) {
  // count recursions
  static $recursion_counter;
  if (!isset($recursion_counter)) $recursion_counter = 0;
  $recursion_counter++;

  //drupal_set_message("Draggableviews: Rebuilding structure..");

  $nodes         = &$info['nodes'];
  $info['input'] = array();
  $input         = &$info['input'];

  // Build an input-array to simulate the form data we would receive on submit

  // loop through all nodes
  foreach ($nodes as $nid => $prop) {
    $depth = $prop['depth'] ? $prop['depth'] : 0;

    // use order weight of the hierarchy level the field is situated in
    $input[$nid][0] = $prop['order'][$depth];
    $input[$nid]['order'][0] = $prop['order'][$depth];

    if (isset($info['hierarchy'])) {
      // set parent
      $input[$nid]['parent'] = $prop['parent'];
    }
  }

  // build hierarchy
  _draggableviews_build_hierarchy($info);

  // save hierarchy
  _draggableviews_save_hierarchy($info);

  // analyze structure (also calculates depths)
  if (!_draggableviews_analyze_structure($info)) {
    // the structure is broken

    if ($recursion_counter <= 10) {
      // start recursion
      return _draggableviews_rebuild_hierarchy($info);
    }
    else {
      //drupal_set_message(t('Reached limit of '. $recursion_counter .' recursions'));
      return FALSE;
    }
  }
  else {
    //drupal_set_message(t('Solved all problems after '. $recursion_counter .' recursions'));
    // redirect here
    return TRUE;
  }
}

/**
 * Detect and repair order collisions
 *
 * @return
 *   TRUE if no collision detected
 */
function _draggableviews_detect_order_collisions(&$info) {

  $nodes = &$info['nodes'];

  $collision = FALSE;

  // Detect order collisions
  // Check for the following:
  //  1) The minimum value should not be used as it
  //     should be possible for new nodes to default on top
  //     without order collisions
  //  2) @todo The last value should not be used...
  //  3) An order value should only be used once per depth/parent

  // array(
  //   parent => array(order1, ..),
  //   ..
  // )
  $order = array();

  $min_value = $info['order']['minimum_value'];

  // loop through all nodes
  foreach ($nodes as $nid => $prop) {
    if (isset($info['hierarchy'])) {
      $parent = $prop['parent'];
      // if no parent set use 0
      if (!isset($parent)) $parent = 0;
    }
    else {
      $parent = 0;
    }

    $order_field_value = &$nodes[$nid]['order'][$prop['depth']];

    $begin_search = TRUE;

    // make sure that the minimum value cannot be used
    if (!is_array($order[$parent])) $order[$parent] = array($min_value);

    // first try to keep current value
    $tmp_order = $order_field_value;

    while (is_numeric(array_search($tmp_order, $order[$parent]))) {
      // if there already exists an order value for this depth/parent
      // we have to find another one.
      // Try to find a free order:

      if ($begin_search == TRUE) {
        $tmp_order = $min_value + 1;
        $begin_search = FALSE;
      }
      else {
        $tmp_order++;
      }

      $collision = TRUE;
    }

    $order[$parent] = array_merge(array($tmp_order), $order[$parent]);

    $order_field_value = $tmp_order;
  }

  return !$collision;
}

/**
 * Set values and save nodes
 *
 * @param $info
 *   The structured information array
 */
function _draggableviews_save_hierarchy($info) {

  // loop through all nodes
  foreach ($info['nodes'] as $nid => $prop) {

    if (isset($info['hierarchy'])) {
      $value   = $prop['parent'];
      $handler = $info['hierarchy']['field']['handler'];
      $handler->save($nid, $value);
    }

    for ($i = 0; $i < count($info['order']['fields']); $i++) {
      // loop through all order fields
      $value   = $prop['order'][$i];
      $handler = $info['order']['fields'][$i]['handler'];
      $handler->save($nid, $value);
    }
  }
}

/**
 * Check order settings
 *
 * @param nid
 *   The node id to check
 * @param info
 *   The structured information array
 *
 * @return
 *   TRUE if no errors were found
 */
function _draggableviews_check_order($nid, &$info) {
  $nodes = &$info['nodes'];

  $temp_nid = $nid;

  for ($i = $nodes[$nid]['depth']; $i >= 0; $i--) {
    // we're operating top-down, so we determine the parents nid by the way

    if ($nodes[$nid]['order'][$i] != $nodes[$temp_nid]['order'][$i]) {
      return FALSE;
    }

    if (isset($info['hierarchy']) && $i > 0) {
      if (!($temp_nid = $nodes[$temp_nid]['parent'])) {
        // this line should never be reached assumed the depth
        // was calculated correctly.
        drupal_set_message(t('Undefined State called in draggableviews_build_hierarchy(..)'), 'error');
        return FALSE;
      }
    }
  }

  return TRUE;
}

/**
 * Calculate depth of all nodes
 *
 * @param info
 *   The structured information array
 *
 * @return
 *   TRUE if no errors were found
 */
function _draggableviews_calculate_depths(&$info) {
  $error = FALSE;

  // loop through all rows the view returns
  foreach ($info['nodes'] as $nid => $prop) {

    // determine hierarchy depth of current row
    $info['nodes'][$nid]['depth'] = _draggableviews_get_hierarchy_depth($nid, $info['nodes'], $info);

    if ($info['nodes'][$nid]['depth'] === FALSE) $error = TRUE;
  }

  return !$error;
}

/**
 * Get Hierarchy depth
 *
 * This function detects cycles,
 * broken hierarchy structures
 * and wrong weight settings
 *
 * @param $node
 *   The node from wich we want to know the depth
 * @param $info
 *   The structured information array
 * return
 *   The hierarchy depth,
 *   on error FALSE.
 */
function _draggableviews_get_hierarchy_depth($nid, &$nodes, &$info) {

  if (!isset($info['hierarchy'])) return 0;

  $depth = 0;
  $error = FALSE;
  $temp_nid = $nid;
  $used_nids = array();
  $used_nids[] = $temp_nid;

  while (!($error = !isset($nodes[$temp_nid])) && ($temp_nid = $nodes[$temp_nid]['parent']) > 0) {
    // In order to detect cycles we use an array,
    // where all used node ids are saved in. If any node id
    // occurs twice -> return FALSE
    $cycle_found = array_search($temp_nid, $used_nids);

    if (isset($cycle_found) && $cycle_found !== FALSE) {
      drupal_set_message(t("Draggableviews: A cycle was found."));
      return FALSE;
    }

    $used_nids[] = $temp_nid;
    $depth++;
  }

  if ($error) {
    // If loop breaked caused by an error
    return FALSE;
  }

  return $depth;
}

/*
 * filter handlers by type
 *
 * @param $type
 *   the field type
 * @param $fields
 *   the fields array
 * return
 *   the available fields
 */
function _draggableviews_filter_fields($types = array(''), $handlers) {
  $available_fields = array();

  foreach ($handlers as $field => $handler) {
    if ($label = $handler->label()) {
      $available_fields[$field] = $label;
    }
    else {
      $available_fields[$field] = $handler->ui_name();
    }
  }
  return $available_fields;
}

function draggableviews_views_pre_render(&$view) {
  $info = _draggableviews_info($view);

  if (!isset($info['order'])) return;

  // analyze structure (also calculates depths)
  if (!_draggableviews_analyze_structure($info)) {
    // the structure is broken
    if (_draggableviews_rebuild_hierarchy($info)) {
      drupal_set_message('The structure was broken. It has been repaired successfully');
      // the structure has been repaired
      $view->executed = FALSE;
      $view->execute();
    }
    else {
      drupal_set_message('The struture is broken. It could not be repaired', 'error');
      // the structure could not be repaired
      return;
    }
  }
}
