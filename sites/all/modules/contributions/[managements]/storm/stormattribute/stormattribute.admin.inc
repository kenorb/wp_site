<?php
// $Id: stormattribute.admin.inc,v 1.5.4.5 2008/12/03 18:30:13 litwol Exp $

/**
 * @file
 */

function stormattribute_list() {
  $o = drupal_get_form('stormattribute_list_filter');

  $i = new stdClass();
  $i->type = 'stormattribute';

  $header = array(
    array(
      'data' => storm_t('Domain'),
      'field' => 'domain',
      'sort' => 'asc',
    ),
    array(
      'data' => storm_t('Key'),
      'field' => 'akey',
    ),
    array(
      'data' => storm_t('Value'),
      'field' => 'avalue',
      'sort' => '',
    ),
    array(
      'data' => storm_icon_add('storm/attributes/add', $i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s = "SELECT * FROM {stormattribute}";
  $where = array();
  $args = array();

  if ($_SESSION['stormattribute_list_filter']['domain']) {
    $where[] = "domain='%s'";
    $args[] = $_SESSION['stormattribute_list_filter']['domain'];
  }

  if ($_SESSION['stormattribute_list_filter']['akey']) {
    $where[] = "LOWER(akey) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormattribute_list_filter']['akey'];
  }

  if ($_SESSION['stormattribute_list_filter']['avalue']) {
    $where[] = "LOWER(avalue) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormattribute_list_filter']['avalue'];
  }

  $itemsperpage = $_SESSION['stormattribute_list_filter']['itemsperpage'];

  $tablesort = tablesort_sql($header);

  $s = db_rewrite_sql($s, 'stormattribute', 'aid');
  $s = storm_rewrite_sql($s, $where) . $tablesort;
  $r = pager_query($s, $itemsperpage, 0, NULL, $args);

  $attributes = array();
  while ($attribute = db_fetch_object($r)) {
    $attributes[] = $attribute;
  }

  $o .= theme('stormattribute_list', $header, $attributes);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormattribute_list_filter() {
  $domain = $_SESSION['stormattribute_list_filter']['domain'];
  $akey = $_SESSION['stormattribute_list_filter']['akey'];
  $avalue = $_SESSION['stormattribute_list_filter']['avalue'];

  $itemsperpage = $_SESSION['stormattribute_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormattribute_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => storm_t('Filter'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['domain'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Domain'),
    '#default_value' => $domain,
    '#size' => 30,
    '#autocomplete_path' => 'storm/attributes/domain/autocomplete',
  );

  $form['filter']['group1']['akey'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Key'),
    '#default_value' => $akey,
    '#size' => 20,
  );

  $form['filter']['group1']['avalue'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Value'),
    '#default_value' => $avalue,
    '#size' => 20,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter'),
    '#submit' => array('stormattribute_list_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset'),
    '#submit' => array('stormattribute_list_filter_reset'),
  );

  $form['filter']['group2']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormattribute_list_filter_filter($form, &$form_state) {
  $_SESSION['stormattribute_list_filter']['domain'] = $form_state['values']['domain'];
  $_SESSION['stormattribute_list_filter']['akey'] = $form_state['values']['akey'];
  $_SESSION['stormattribute_list_filter']['avalue'] = $form_state['values']['avalue'];
  $_SESSION['stormattribute_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormattribute_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormattribute_list_filter']);
}

function stormattribute_add() {
  $attribute = array(
    'domain' => '',
    'akey' => '',
    'avalue' => '',
    'weight' => 0,
  );
  return stormattribute_form($attribute);
}

function stormattribute_add_submit($form, &$form_state) {
  db_query("INSERT INTO {stormattribute} (domain, akey, avalue, weight) VALUES ('%s', '%s', '%s', %d)", $form_state['values']['domain'], $form_state['values']['akey'], $form_state['values']['avalue'], $form_state['values']['weight']);

  if ($_REQUEST['destination']) {
    $destination = urldecode($_REQUEST['destination']);
  }
  else {
    $destination = 'storm/attributes';
  }
  drupal_goto($destination);
}

function stormattribute_edit($form_state, $aid) {
  $attribute = array();
  if ($aid) {
    $r = db_query("SELECT * FROM {stormattribute} WHERE aid=%d", $aid);
    $attribute = db_fetch_object($r);
  }
  return stormattribute_form($attribute);
}

function stormattribute_edit_submit($form, &$form_state) {
  db_query("UPDATE {stormattribute} SET domain='%s', akey='%s', avalue='%s', weight=%d WHERE aid=%d", $form_state['values']['domain'], $form_state['values']['akey'], $form_state['values']['avalue'], $form_state['values']['weight'], $form_state['values']['aid']);

  if ($_REQUEST['destination']) {
    $destination = urldecode($_REQUEST['destination']);
  }
  else {
    $destination = 'storm/attributes';
  }
  drupal_goto($destination);
}

function stormattribute_form_delete($form, &$form_state) {
  drupal_goto('storm/attributes/delete/'. $form_state['values']['aid']);
}

function stormattribute_delete($form_state, $aid) {
  $form['aid'] = array('#type' => 'value', '#value' => $aid);
  $r = db_query("SELECT * FROM {stormattribute} WHERE aid=%d", $aid);
  $a = db_fetch_object($r);
  $title = $a->domain .' : '. $a->avalue;

  return confirm_form($form,
  storm_t('Are you sure you want to delete the attribute %title?', array('%title' => $title)),
  'storm/attributes/edit/'. $aid,
  storm_t('This action cannot be undone.'),
  storm_t('Delete'), storm_t('Cancel'));
}

function stormattribute_delete_submit($form, &$form_state) {
  if ($form_state['values']['aid']) {
    db_query('DELETE FROM {stormattribute} WHERE aid=%d', $form_state['values']['aid']);
    drupal_set_message('Storm attribute deleted');
    if ($_REQUEST['destination']) {
      $destination = urldecode($_REQUEST['destination']);
    }
    else {
      $destination = 'storm/attributes';
    }
    drupal_goto($destination);
  }
}

function stormattribute_form($attribute = NULL) {
  $form = array();
  if ($attribute->aid) {
    $form['aid'] = array(
      '#type' => 'value',
      '#value' => $attribute->aid,
    );
  }

  $form['domain'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Domain'),
    '#required' => TRUE,
    '#default_value' => $attribute->domain,
    '#autocomplete_path' => 'storm/attributes/domain/autocomplete',
    '#weight' => -10
  );

  $form['akey'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Key'),
    '#required' => TRUE,
    '#default_value' => $attribute->akey,
    '#weight' => -9
  );

  $form['avalue'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Value'),
    '#required' => TRUE,
    '#default_value' => $attribute->avalue,
    '#weight' => -8
  );

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => storm_t('Weight'),
    '#default_value' => $attribute->weight,
    '#weight' => -7
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Save')
  );

  if ($attribute->aid) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => storm_t('Delete'),
      '#submit' => array('stormattribute_form_delete'),
    );
  }
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Cancel'),
    '#submit' => array('stormattribute_cancel_submit'),
  );

  return $form;
}

function _stormattibute_domain_options() {
  static $options;
  if (!$options) {
    $r = db_query("SELECT DISTINCT(domain) AS domain_name FROM {stormattribute} ORDER BY domain");
    $options = array();
    while ($d = db_fetch_object($r)) {
      $options[$d->domain_name] = $d->domain_name;
    }
  }
  return $options;
}

function _stormattribute_domain_autocomplete($string = '') {
  static $default_domains;
  if (!$default_domains) {
    $default_domains = array (
      'Country'=>'Country',
      'Currency'=>'Currency',
      'Task status'=>'Task status',
      'Project category'=>'Project category',
      'Project status search'=>'Project status search',
      'Project status'=>'Project status',
      'Ticket priority search'=>'Ticket priority search',
      'Ticket priority'=>'Ticket priority',
      'Ticket category'=>'Ticket category',
      'Ticket status search'=>'Ticket status search',
      'Ticket status'=>'Ticket status',
      'Task status search'=>'Task status search',
      'Price mode'=>'Price mode',
      'Project priority'=>'Project priority',
      'Project priority search'=>'Project priority search',
      'Task category'=>'Task category',
      'Task priority'=>'Task priority',
      'Task priority search'=>'Task priority search',
      'Duration unit'=>'Duration unit',
    );
  }

  $matches = array();
  if ($string) {
    $s = "SELECT DISTINCT(domain) AS domain FROM {stormattribute} WHERE LOWER(domain) LIKE LOWER('%s%%')";
    $s = db_rewrite_sql($s);
    $result = db_query($s, $string);
    while ($a = db_fetch_object($result)) {
      $matches[$a->domain] = check_plain($a->domain);
    }
  }

  foreach ($default_domains as $domain) {
    if (strpos(strtoupper($domain), strtoupper($string))===false) {
    }
    else {
      $matches[$domain] = $domain;
    }
  }

  $matches = array_slice($matches, 0, 10);

  drupal_json($matches);
}

