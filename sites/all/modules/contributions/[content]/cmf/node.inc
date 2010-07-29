<?php
// $Id: node.inc,v 1.2.2.3 2008/12/03 18:43:45 nunoveloso18 Exp $

/**
 * @file
 * @brief Content management filter node operations file
 * 
 * This file contains all the node functions used by the module.
 *
 * @attention Copyright (C) 2008 Nuno Veloso <nunoveloso18 (at) gmail (dot) com>
 * @author Nuno Andr Veloso Matias (http://www.nunoveloso.org)
 *
 * @warning For more information on licensing, read the LICENCE.txt file.
 */


/**
 * Defines the form for nodes administration filter results.
 *
 * @ingroup forms
 * @see cmf_admin_nodes_form_validate()
 * @see cmf_admin_nodes_form_submit()
 * 
 * @return array with forms properties 
 */
function cmf_admin_nodes_form() {

  $destination = drupal_get_destination();

  // build an 'Update options' form
  if (user_access('filter and manage site content')) {
    $form['options'] = array(
      '#type' => 'fieldset', '#title' => t('Update options'),
      '#prefix' => '<div class="container-inline">', '#suffix' => '</div>',
    );
    $options = array();
    foreach (module_invoke_all('node_operations') as $operation => $array) {
       $options[$operation] = $array['label'];
    }
    $form['options']['operation'] = array(
      '#type' => 'select', 
      '#options' => $options,  
      '#default_value' => 'approve'
    );
    $form['options']['submit'] = array('#type' => 'submit', '#value' => t('Update'));
  }

  // load the nodes that we want to display
  $form['header'] = array(
    '#type'  => 'value', 
    '#value' => cmf_build_header(),
  );
  
  $result = cmf_perform_query($form['header']['#value']);
  // build a table listing the appropriate nodes
  while ($node = db_fetch_object($result)) {
    $nodes[$node->nid] = '';
    $form['title'][$node->nid] = array('#value' => l($node->title, 'node/'. $node->nid) .' '.
      theme('mark', node_mark($node->nid, $node->changed)));
    $form['kind'][$node->nid] = array('#value' => _cmf_get_img('node', t('node')));
    $form['type'][$node->nid] =  array('#value' => check_plain(node_get_types('name', $node)));
    if (!(arg(0) == 'user' && is_numeric(arg(1)) && arg(1) > 0)) {
      $form['username'][$node->nid] = array('#value' => theme('cmf_user', $node->uid));
    }
    $form['status'][$node->nid] =  array('#value' =>  ($node->status ? t('published') : 
      t('not published')));
    $form['created'][$node->nid] = array('#value' => format_date($node->created, 'small'));
    if (user_access('filter and manage site content')) {
      $form['operations'][$node->nid] = array('#value' => l(_cmf_get_img('edit', t('edit')) .' '. 
        t('edit'), 'node/'. $node->nid .'/edit', array('html' => TRUE)));
    }
  }

  if (user_access('filter and manage site content')) {
    $form['nodes'] = array('#type' => 'checkboxes', '#options' => $nodes);
  }
  $form['pager'] = array('#value' => theme('pager', NULL, $_SESSION['cmf_max_rows'], 0));
  
  return $form;
}


/** 
 * Form validation before submit. \n
 * We can't execute any 'Update options' if no nodes were selected.
 *
 * @ingroup forms
 * @see cmf_admin_nodes_form()
 * @see cmf_admin_nodes_form_submit()
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */ 
function cmf_admin_nodes_form_validate($form, &$form_state) {
  $nodes = array_filter($form_state['values']['nodes']);
  if (count($nodes) == 0) {
    form_set_error('', t('No items selected.'));
  }
}


/** 
 * Handle post-validation form submission. \n
 * Execute the chosen 'Update option' on the selected nodes, such as
 * publishing, unpublishing, promotion and stickness status or deleting.
 *
 * @ingroup forms
 * @see cmf_admin_nodes_form()
 * @see cmf_admin_nodes_form_validate()
 *
 * @param the ID of the passed form
 * @param array with the form properties values
 */ 
function cmf_admin_nodes_form_submit($form, &$form_state) {
  $operations = module_invoke_all('node_operations');
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked nodes
  $nodes = array_filter($form_state['values']['nodes']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($nodes), $operation['callback arguments']);
    }
    else {
      $args = array($nodes);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
    drupal_set_message(t('The update has been performed.'));

    if (arg(0) == 'user' && is_numeric(arg(1)) && arg(1) > 0) {
      $form_state['redirect'] = 'user/'. arg(1) .'/user-content';
    }
    else {
      $form_state['redirect'] = 'admin/content/filter';
    }
  }
}


/** 
 * Theme results table.
 *
 * @ingroup themable
 *
 * @return table with filter results
 */ 
function theme_cmf_admin_nodes_form($form) {
  $output = drupal_render($form['options']);
  if (isset($form['title']) && is_array($form['title'])) {
    foreach (element_children($form['title']) as $key) {
      $row = array();
      if (user_access('filter and manage site content')) {
        $row[] = drupal_render($form['nodes'][$key]);
      }
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['kind'][$key]);
      $row[] = drupal_render($form['type'][$key]);
      if (!(arg(0) == 'user' && is_numeric(arg(1)) && arg(1) > 0)) {
        $row[] = drupal_render($form['username'][$key]);
      }
      $row[] = drupal_render($form['status'][$key]);
      $row[] = drupal_render($form['created'][$key]);
      if (user_access('filter and manage site content')) {
        $row[] = drupal_render($form['operations'][$key]);
      }
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('Filter returned no results.'), 'colspan' => '7'));
  }

  $output .= theme('table', $form['header']['#value'], $rows, array());
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}