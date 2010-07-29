<?php
// $Id: stormperson.admin.inc,v 1.5.4.8 2009/01/07 08:40:36 robertogerola Exp $

/**
 * @file
 */

function stormperson_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET) &&
    ($_SESSION['stormperson_list_filter']['organization_nid'] != $_GET['organization_nid'])) {
    $_SESSION['stormperson_list_filter']['organization_nid'] = $_GET['organization_nid'];
  }

  $o = drupal_get_form('stormperson_list_filter');

  $i = new stdClass();
  $i->type = 'stormperson';

  $params = $_GET;
  if ($_SESSION['stormperson_list_filter']['organization_nid']) {
    $params['organization_nid'] = $_SESSION['stormperson_list_filter']['organization_nid'];
  }

  $header = array(
    array(
      'data' => storm_t('Organization', 'person'),
      'field' => 'spe.organization_title',
    ),
    array(
      'data' => storm_t('Name', 'person'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_t('Email', 'person'),
      'field' => 'spe.email',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  $s  = "SELECT n.*, spe.* FROM {node} AS n INNER JOIN {stormperson} AS spe ON n.vid=spe.vid WHERE n.status=1 AND n.type='stormperson' ";
  if ($_SESSION['stormperson_list_filter']['organization_nid']) {
    $where[] = 'spe.organization_nid=%d';
    $args[] = $_SESSION['stormperson_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormperson_list_filter']['name']) {
    $where[] = "LOWER(n.title) LIKE LOWER('%%%s%%')";
    $args[] = $_SESSION['stormperson_list_filter']['name'];
  }
  $itemsperpage = $_SESSION['stormperson_list_filter']['itemsperpage'];

  $s = stormperson_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $people = array();
  while ($person = db_fetch_object($r)) {
    $people[] = $person;
  }

  $o .= theme('stormperson_list', $header, $people);
  $o .= theme('pager', NULL, $itemperpage, 0);
  print theme('page', $o);
}

function stormperson_list_filter() {
  $organization_nid = $_SESSION['stormperson_list_filter']['organization_nid'];
  $name = $_SESSION['stormperson_list_filter']['name'];

  $itemsperpage = $_SESSION['stormperson_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormperson_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => storm_t('Filter', 'person'),
    '#collapsible' => true,
    '#collapsed' => true,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormorganization} sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => storm_t('All', 'person')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Organization', 'person'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
  );

  $form['filter']['group1']['name'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Name', 'person'),
    '#default_value' => $name,
    '#size' => 30,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'person'),
    '#submit' => array('stormperson_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'person'),
    '#submit' => array('stormperson_list_filter_reset'),
  );

  $form['filter']['group2']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'person'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormperson_list_filter_filter($form, &$form_state) {
  $_SESSION['stormperson_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormperson_list_filter']['name'] = $form_state['values']['name'];
  $_SESSION['stormperson_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormperson_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormperson_list_filter']);
}

function _stormperson_organization_people_js($organization_nid=0) {
  $people = array();

  if ($organization_nid) {
    $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormperson} AS spe ON n.vid=spe.vid WHERE n.status=1 AND n.type='stormperson' AND spe.organization_nid=%d ORDER BY n.title";    $s = stormperson_access_sql($s);
    $s = db_rewrite_sql($s);
    $r = db_query($s, $organization_nid);

    while ($item = db_fetch_object($r)) {
      $nid = $item->nid;
      $people[$nid] = $item->title;
    }
  }
  print drupal_to_js($people);
  exit();
}

