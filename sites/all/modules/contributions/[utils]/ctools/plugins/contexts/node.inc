<?php
// $Id: node.inc,v 1.1 2008/12/04 22:22:41 merlinofchaos Exp $

/**
 * @file
 *
 * Plugin to provide a node context. A node context is a node wrapped in a
 * context object that can be utilized by anything that accepts contexts.
 */

/**
 * Implementation of specially named hook_ctools_contexts().
 */
function ctools_node_ctools_contexts() {
  $args['node'] = array(
    'title' => t("Node"),
    'description' => t('A node object.'),
    'context' => 'ctools_context_create_node',
    'settings form' => 'ctools_context_node_settings_form',
    'settings form validate' => 'ctools_context_node_settings_form_validate',
    'settings form submit' => 'ctools_context_node_settings_form_submit',
    'keyword' => 'node',
    'context name' => 'node',
  );
  return $args;
}

/**
 * It's important to remember that $conf is optional here, because contexts
 * are not always created from the UI.
 */
function ctools_context_create_node($empty, $data = NULL, $conf = FALSE) {
  $types = array('node');
  // FIXME this is broken when called from ctools_page's render process, through ctools_context_load_contexts(); it passes in $conf as TRUE.
  if (!empty($conf['types'])) {
    foreach ($conf['types'] as $type) {
      if ($type) {
        $types[] = 'node-' . $type;
      }
    }
  }
  $context = new ctools_context($types);
  $context->plugin = 'node';

  if ($empty) {
    return $context;
  }

  if ($conf) {
    $nid = is_array($data) && isset($data['nid']) ? $data['nid'] : (is_object($data) ? $data->nid : 0);

    if (module_exists('translation')) {
       if ($translation = module_invoke('translation', 'node_nid', $nid, $GLOBALS['language']->language)) {
        $nid = $translation;
        $reload = TRUE;
      }
    }

    if (is_array($data) || !empty($reload)) {
      $data = node_load($nid);
    }
  }

  if (!empty($data)) {
    $context->data     = $data;
    $context->title    = $data->title;
    $context->argument = $data->nid;
    if (is_array($context->type)) {
      $context->type[] = 'node-' . $data->type;
    }
    else {
      $context->type = array($context->type, 'node-' . $data->type);
    }
    return $context;
  }
}

function ctools_context_node_settings_form($conf, $external = FALSE) {
  if (empty($conf)) {
    $conf = array(
      'nid' => '',
    );
  }

  if ($external) {
    $form['external'] = array(
      '#type' => 'checkbox',
      '#default_value' => $conf['external'],
      '#title' => t('Require this context from an external source (such as a containing panel page).'),
      '#description' => t('If selected, node selection (below) will be ignored.'),
    );
  }

  $form['node'] = array(
    '#prefix' => '<div class="no-float">',
    '#suffix' => '</div>',
    '#title' => t('Enter the title or NID of a post'),
    '#type' => 'textfield',
    '#maxlength' => 512,
    '#autocomplete_path' => 'ctools/node/autocomplete',
    '#weight' => -10,
  );

  if (!empty($conf['nid'])) {
    $info = db_fetch_object(db_query("SELECT * FROM {node} n WHERE n.nid = %d", $conf['nid']));
    if ($info) {
      $link = l(t("'%title' [node id %nid]", array('%title' => $info->title, '%nid' => $info->nid)), "node/$info->nid", array('target' => '_blank', 'title' => t('Open in new window')));
      $form['node']['#description'] = t('Currently set to !link', array('!link' => $link));
    }
  }

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $conf['nid'],
  );

  $form['set_identifier'] = array(
    '#type' => 'checkbox',
    '#default_value' => FALSE,
    '#title' => t('Reset identifier to node title'),
    '#description' => t('If checked, the identifier will be reset to the node title of the selected node.'),
  );

  return $form;
}

/**
 * Validate a node.
 */
function ctools_context_node_settings_form_validate($form, &$form_values, &$form_state) {
  // Validate the autocomplete
  if (empty($form_values['external']) && empty($form_values['nid']) && empty($form_values['node'])) {
    form_error($form['node'], t('You must select a node.'));
    return;
  }

  if (empty($form_values['node'])) {
    return;
  }

  $nid = $form_values['node'];
  $preg_matches = array();
  $match = preg_match('/\[nid: (\d+)\]/', $nid, $preg_matches);
  if (!$match) {
    $match = preg_match('/^nid: (\d+)/', $nid, $preg_matches);
  }

  if ($match) {
    $nid = $preg_matches[1];
  }
  if (is_numeric($nid)) {
    $node = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid FROM {node} n WHERE n.nid = %d"), $nid));
  }
  else {
    $node = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid FROM {node} n WHERE LOWER(n.title) = LOWER('%s')"), $nid));
  }

  if (!$node) {
    form_error($form['node'], t('Invalid node selected.'));
  }
  else {
    form_set_value($form['nid'], $node->nid, $form_state);
    // $form_values['nid'] = $node->nid;
  }
}

function ctools_context_node_settings_form_submit($form, &$form_values, &$form_state) {
  if ($form_values['set_identifier']) {
    $node = node_load($form_values['nid']);
    $form_state['values']['context']['identifier'] = $node->title;
  }

  // Don't let this be stored.
  unset($form_values['set_identifier']);
}
