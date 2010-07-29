<?php
// $Id: taxonomy_dhtml.admin.inc,v 1.2 2008/12/12 21:54:10 darthclue Exp $

/**
 * @file
 * Administrative page callbacks for the taxonomy DHTML module.
 */

/**
 * Form Builder to manage settings for taxonomy DHTML.
 *
 * @ingroup forms
 * @see taxonomy_dhtml_admin_submit()
 * @see theme_taxonomy_dhtml_admin()
 */
function taxonomy_dhtml_admin() {
  $vocabularies = taxonomy_dhtml_get_vocabularies();
  $form = array('#tree' => TRUE);
  foreach ($vocabularies as $vocabulary) {
    $types = array();
    $form[$vocabulary->vid]['#vocabulary'] = (array)$vocabulary;
    $form[$vocabulary->vid]['name'] = array('#value' => check_plain($vocabulary->name));
    $form[$vocabulary->vid]['noi'] = array('#type' => 'textfield', '#size' => 3, '#maxlength' => 3, '#default_value' => (is_null($vocabulary->noi) ? 25 : $vocabulary->noi));
    $form[$vocabulary->vid]['exposeBlock'] = array('#type' => 'checkbox', '#default_value' => is_null($vocabulary->expblock) ? FALSE : $vocabulary->expblock);
    $form[$vocabulary->vid]['depth'] = array('#type' => 'select', '#default_value' => is_null($vocabulary->depth) ? 0 : $vocabulary->depth, '#options' => array('0' => t('None'), '1' => '1', '2' => '2', '3' => '3', '4' => '4', '5' => '5', '6' => '6', '7' => '7', '8' => '8', '9' => '9', '10' => 'All'));
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Theme the taxonomy dhtml admin as a sortable list of vocabularies.
 *
 * @ingroup themeable
 * @see taxonomy_dhtml_admin()
 */
function theme_taxonomy_dhtml_admin($form) {
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['name'])) {
      $vocabulary = &$form[$key];

      $row = array();
      $row[] = drupal_render($vocabulary['name']);
      $row[] = drupal_render($vocabulary['noi']);
      $row[] = drupal_render($vocabulary['depth']);
      $row[] = drupal_render($vocabulary['exposeBlock']);
      $rows[] = array('data' => $row, 'class' => 'draggable');
    }
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No vocabularies available.'), 'colspan' => '3'));
  }

  $header = array(t('Name'), t('Number of items to display'), t('Depth'), t('Expose Block'));
  return theme('table', $header, $rows, array('id' => 'taxonomy')) . drupal_render($form);
}

/**
 * Submit handler for the taxonomy dhtml settings form.
 *
 *
 * @see taxonomy_dhtml_admin()
 */
function taxonomy_dhtml_admin_submit($form, &$form_state) {
  foreach (array_keys($form_state['values']) as $ak) {
    if (is_array($form_state['values'][$ak])) {
      db_query("DELETE FROM {taxonomy_dhtml} WHERE vid=%d", $ak);
      db_query("INSERT INTO {taxonomy_dhtml} (vid, noi, depth, expblock) VALUES (%d, %d, %d, %d)", $ak, $form_state['values'][$ak]['noi'], $form_state['values'][$ak]['depth'], $form_state['values'][$ak]['exposeBlock']);
    }
  }
  cache_clear_all();
  drupal_set_message('Taxonomy DHTML settings saved.');
}
