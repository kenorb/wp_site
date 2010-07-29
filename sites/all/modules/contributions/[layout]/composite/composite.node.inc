<?php
// $Id: composite.node.inc,v 1.1.2.2 2008/11/19 15:10:07 bengtan Exp $


function composite_composite_node_api(&$reference, $op, $a3 = NULL, $a4 = NULL) {
  global $language;
  
  switch ($op) {
    // Derive additional fields 
    case 'load':
      // Note: can't do a node_load here, otherwise, we run the chance of an infinite loop. 
      list($unused, $reference['nid']) = explode('-', $reference['id'], 2);
      break;

    // Generate and insert an informative human-readable string into ['info']
    case 'info':
      $node = node_load($reference['nid']);
      $reference['info'] = check_plain($node->title);
      break;

    // Return a rendering of the reference item
    case 'view':
      $node = node_load($reference['nid']);
      
      // check if node has a translation, if yes, set the translation in the current language as $node
      if ($node->tnid) {
        $translations = translation_node_get_translations($node->tnid);
        $node = node_load($translations[$language->language]->nid);
      }
    
      if ($node->nid) {
        switch ($reference['data']['format']) {
          case 'title':
            $output = theme('composite_node_title', check_plain($node->title));
            break;

          case 'title-link':
            $output = theme('composite_node_title', l($node->title, 'node/' . $node->nid));
            break;

          case 'teaser':
            $output =  node_view($node, TRUE, FALSE);
            break;

          case 'full':
            if (_composite_nodes_recursion_check($node)) {
              drupal_set_message(t('You have created an infinite loop. One node is displaying a second node which in turn displays the first node.'), 'error');
              $output =  node_view($node, TRUE, FALSE);
            }
            else {
              $output =  node_view($node, FALSE, FALSE);
            }
            break;
        }
        return $output;
      }
      break;

    // Return form elements for additional settings
    case 'settings':
      $form['format'] = array(
        '#title' => t('Display as'),
        '#type' => 'select',
        '#options' => array(
          'title' => t('Title'),
          'title-link' => t('Title (link)'),
          'teaser' => t('Teaser'),
          'full' => t('Full node'),
        ),
        '#default_value' => $reference['data']['format'] ? $reference['data']['format'] : 'teaser',
      );
      return $form;
      break;
  }
}

function composite_composite_node_access($op, $node) {
  // Only show nodes tab if referenceable types or a view has been configured
  $types = variable_get('composite_referenceable_types_' . $node->type, array());
  $view_name = variable_get('composite_advanced_view_' . $node->type, '--');

  $access = FALSE;
  if ($types || $view_name != '--')
    $access = node_access($op, $node);
  return $access;
}

/**
 * Generates and returns a keyed array of potential composite references
 *   so composite.module can create a meaningful local task. 
 */
function composite_composite_node_potentials($node) {
  $node_list = variable_get('composite_advanced_view_' . $node->type, '--') == '--' ? _composite_nodes_form_get_potentials_standard($node) : _composite_nodes_form_get_potentials_views($node);

  foreach ($node_list as $nid => $v) {
    $options['node-' . $nid] = $v->title;
  }
  return $options;  
}

function _composite_nodes_form_get_potentials_standard($node) {
  $types = variable_get('composite_referenceable_types_' . $node->type, array());
  foreach ($types as $k => $type) {
    $types[$k] = "'". $types[$k] ."'";
  }

  $list = array();
  $result = db_query("SELECT nid, title FROM {node} WHERE type IN (". implode(', ', $types) . ") ORDER BY title, type, nid");
  while ($object = db_fetch_object($result)) {
    $list[$object->nid] = $object;
  }
  return $list;
}

function _composite_nodes_form_get_potentials_views($node) {
  $view_name = variable_get('composite_advanced_view_' . $node->type, '--');

  if (module_exists('views') && $view_name != '--' && $view = views_get_view($view_name)) {
    $view_args = variable_get('composite_advanced_view_args_' . $node->type, '');
    $view->set_arguments($view_args ? explode(',', $view_args) : array());
    $result = $view->execute_display();
    $results = $view->result;

    // We rely on the view returning objects with ->nid and ->node_title.
    //   Can we safely presume this will always be true? 
    $list = array();
    foreach ($results as $object) {
      $object->title = $object->node_title;
      $list[$object->nid] = $object;
    }
    return $list;
  }
  else {
    return array();
  }
}

// Returns true if we there is an infinite loop.
function _composite_nodes_recursion_check($node) {
  static $nodes = array();

  if (array_key_exists($node->nid, $nodes)) {
    return true;
  }
  else {  
    $nodes[$node->nid] = $node->nid;
    return false;
  }
}
