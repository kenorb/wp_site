<?php
// $Id: stormknowledgebase.admin.inc,v 1.2.4.6 2009/01/06 15:16:22 robertogerola Exp $

/**
 * @file
 */

function stormknowledgebase_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  $o = drupal_get_form('stormknowledgebase_list_filter');

  $i = new stdClass();
  $i->type = 'stormknowledgebase';

  $params = $_GET;

  $header = array(
    array(
      'data' => storm_t('Title', 'knowledgebase'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_t('Topic', 'knowledgebase'),
      'field' => 'skb.topic',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.nid, n.title, skb.* FROM {node} AS n INNER JOIN {stormknowledgebase} AS skb ON n.vid=skb.vid WHERE n.status=1 AND n.type='stormknowledgebase' ";

  $where = array();
  $args = array();

  if ($_SESSION['stormknowledgebase_list_filter']['topic'] && $_SESSION['stormknowledgebase_list_filter']['topic'] != '-') {
    $where[] = "skb.topic='%s'";
    $args[] = $_SESSION['stormknowledgebase_list_filter']['topic'];
  }

  $s = stormknowledgebase_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $tablesort = tablesort_sql($header);

  $itemsperpage = $_SESSION['stormknowledgebase_list_filter']['itemsperpage'];
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $notes = array();
  while ($item = db_fetch_object($r)) {
    $notes[] = $item;
  }

  $o .= theme('stormknowledgebase_list', $header, $notes);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormknowledgebase_list_filter() {
  $topic = $_SESSION['stormknowledgebase_list_filter']['topic'];

  $itemsperpage = $_SESSION['stormknowledgebase_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormknowledgebase_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => storm_t('Filter', 'knowledgebase'),
    '#collapsible' => true,
    '#collapsed' => true,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['topic'] = array(
    '#type' => 'select',
    '#title' => storm_t('Topic', 'knowledgebase'),
    '#default_value' => $topic,
    '#options' => array('-' => storm_t('all', 'knowledgebase')) + stormattribute_attributes_bydomain('Knowledge base topic'),
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'knowledgebase'),
    '#submit' => array('stormknowledgebase_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'knowledgebase'),
    '#submit' => array('stormknowledgebase_list_filter_reset'),
  );

  $form['filter']['group2']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'knowledgebase'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;

}

function stormknowledgebase_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormknowledgebase_list_filter']);
}

function stormknowledgebase_list_filter_filter($form, &$form_state) {
  $_SESSION['stormknowledgebase_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
  $_SESSION['stormknowledgebase_list_filter']['topic'] = $form_state['values']['topic'];
}


