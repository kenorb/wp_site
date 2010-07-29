<?php
// $Id: finder_wizard.taxonomy.inc,v 1.1.2.3 2008/11/08 05:37:06 danielb Exp $

/**
 * Get an array of vocabularies for use in select forms
 * parameters - $vocab_list: array of vocabularies to reject from the list
 */

function finder_wizard_taxonomy_get_fields() { 
  $vocabs = taxonomy_get_vocabularies();
  if (array_filter($vocabs) == null) {
    // no vocabs;
    return FALSE;
  }
  $vocab_array = array();
  foreach ((array)$vocabs as $vocab) {
    $vocab_array['taxonomy.'. $vocab->vid] = $vocab->name;
  }
  return $vocab_array;
}

/**
 * Get the form element for this step.
 * to do: change this so it builds onto the form instead of creating it
 */
function finder_wizard_taxonomy_get_element(&$form_state) {
  $finder_wizard = &$form_state['storage']['finder_wizard'];
  $step = &$form_state['storage']['step'];
  $field_name = &$finder_wizard->steps[$step]['field_name'];
  $field_display = &$finder_wizard->steps[$step]['field_display'];
  $element = &$finder_wizard->steps[$step]['element'];
  $field = taxonomy_vocabulary_load($field_name);
  $form_element =  array(
    '#type' => $element,
    '#title' => t($field_display),
    '#prefix' => theme('finder_wizard_form_description', $field->description),
    '#description' => $field->help,
  );
  if (in_array($element, array('radios'))) {
    $form_element['#options'] = $form_state['storage']['answers'][$step];
  }
  return $form_element;
}

/**
 * Get the default value for this step.
 */
function finder_wizard_taxonomy_get_default(&$form_state) {
  $finder_wizard = &$form_state['storage']['finder_wizard'];
  $step = &$form_state['storage']['step'];
  $current_default = &$form_state['storage']['values'][$step][$step];
  $element = &$finder_wizard->steps[$step]['element'];
  if (in_array($element, array('radios'))) {
    $default_value = $current_default;
  }
  else {
    $term = taxonomy_get_term($current_default);
    if ($term->name) {
      $default_value = $term->name;
    }
  }
  return $default_value;
}

/**
 * Get the options for the form element of this step.
 */
function finder_wizard_taxonomy_get_answers(&$form_state, $restrict_to_nids) {
  $field_options = array();
  $finder_wizard = &$form_state['storage']['finder_wizard'];
  $step = &$form_state['storage']['step'];
  $field_name = &$finder_wizard->steps[$step]['field_name'];
  $types = &$finder_wizard->types;
  $args = array();
  $wheres = " WHERE td.vid = %d ";
  $args[] = $field_name;
  if ($types) {
    $wheres .= " AND n.type IN ('". implode("','", $types) ."') ";
  }
  if ($restrict_to_nids) {
    $wheres .= " AND n.nid IN ('". implode("','", $restrict_to_nids) ."') ";
  }
  $sql = "
    SELECT td.tid, td.name, td.description FROM {term_data} td 
    INNER JOIN {term_node} tn ON td.tid = tn.tid 
    INNER JOIN {node} n ON tn.vid = n.vid 
    ". $wheres ." 
    ORDER BY td.weight 
  ";
  $result = db_query($sql, $args);
  while ($term = db_fetch_object($result)) {
    $field_options[$term->tid] = "<span class='term-option'>";
    $field_options[$term->tid] .= "<span class='term-name'>".$term->name."</span>";
    $field_options[$term->tid] .= $term->description ? " <span class='term-desc'>".$term->description."</span>" : '';
    $field_options[$term->tid] .= "</span>";
  }
  return $field_options;
}

/**
 * Get the results for one step of the wizard.
 */
function finder_wizard_taxonomy_get_results(&$form_state, $step) {
  $results = array();
  $finder_wizard = &$form_state['storage']['finder_wizard'];
  $field_name = &$finder_wizard->steps[$step]['field_name'];
  $element = &$finder_wizard->steps[$step]['element'];
  $tids = array();
  if (!empty($form_state['storage']['values'])) {
    $tids[] = &$form_state['storage']['values'][$step][$step];
  }
  $result = finder_wizard_taxonomy_select_nodes($tids, 'all', 'n.sticky DESC, n.created DESC', $types);
  while ($node = db_fetch_object($result)) {
    $results[$node->nid] = $node;
  }
  return $results;
}

/**
 * Alter the answer for this step.
 */
function finder_wizard_taxonomy_alter_answer(&$form_state) {
  $finder_wizard = &$form_state['storage']['finder_wizard'];
  $step = &$form_state['storage']['step'];
  $element = &$finder_wizard->steps[$step]['element'];
  $answer = &$form_state['values'][$step];
  if (!in_array($element, array('radios'))) {
    $terms = taxonomy_get_term_by_name($answer);
    $answer = $terms[0]->tid;
  }
  return $answer;
}

/**
 * Finds all nodes that match selected taxonomy and types conditions.
 */
function finder_wizard_taxonomy_select_nodes($tids, $depth = 0, $order = 'n.sticky DESC, n.created DESC', $types = NULL) {
  if (count($tids) > 0) {
    // For each term ID, generate an array of descendant term IDs to the right depth.
    $descendant_tids = array();
    if ($depth === 'all') {
      $depth = NULL;
    }
    foreach ($tids as $index => $tid) {
      $term = taxonomy_get_term($tid);
      $tree = taxonomy_get_tree($term->vid, $tid, -1, $depth);
      $descendant_tids[] = array_merge(array($tid), array_map('_taxonomy_get_tid_from_term', $tree));
    }
    $joins = '';
    $wheres = '';
    $args = array();
    foreach ($descendant_tids as $index => $tids) {
      $joins .= ' INNER JOIN {term_node} tn'. $index .' ON n.vid = tn'. $index .'.vid';
      $wheres .= ' AND tn'. $index .'.tid IN ('. db_placeholders($tids, 'int') .')';
      $args = array_merge($args, $tids);
    }
    if ($types) {
      $wheres .= " AND n.type IN ('". implode("','", $types) ."') ";
    }
    $sql = 'SELECT DISTINCT(n.nid), n.sticky, n.title, n.created FROM {node} n '. $joins .' WHERE n.status = 1 '. $wheres .' ORDER BY '. $order;
    $sql = db_rewrite_sql($sql);
    $result = db_query($sql, $args);
  }
  return $result;
}