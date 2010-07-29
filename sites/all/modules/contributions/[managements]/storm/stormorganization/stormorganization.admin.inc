<?php
// $Id: stormorganization.admin.inc,v 1.7.4.7 2008/12/18 09:15:46 robertogerola Exp $

/**
 * @file
 */

function stormorganization_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('name', $_GET)) {
    if ($_SESSION['stormorganization_list_filter']['name'] != $_GET['name']) {
      $_SESSION['stormorganization_list_filter']['name'] = $_GET['name'];
    }
  }

  $o = drupal_get_form('stormorganization_list_filter');

  $i = new stdClass();
  $i->type = 'stormorganization';

  $header = array(
    array(
      'data' => storm_t('Name', 'organization'),
      'field' => 'n.title',
      'sort' => 'ASC',
    ),
    array(
      'data' => storm_t('Country', 'organization'),
      'field' => 'sor.country',
      'sort' => '',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s = "SELECT n.*, sor.* FROM {node} AS n INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid WHERE n.status=1 AND n.type='stormorganization'";

  $where = array();
  $args = array();

  if ($_SESSION['stormorganization_list_filter']['country']) {
    $where[] = "sor.country='%s'";
    $args[] = $_SESSION['stormorganization_list_filter']['country'];
  }
  if ($_SESSION['stormorganization_list_filter']['name']) {
    $where[] = "LOWER(n.title) LIKE LOWER('%s')";
    $args[] = $_SESSION['stormorganization_list_filter']['name'];
  }

  $iscustomer = $_SESSION['stormorganization_list_filter']['iscustomer'];
  if ($iscustomer && $iscustomer!='-') {
    if ($iscustomer=='yes') {
      $where[] = "sor.iscustomer=1";
    }
    else if ($iscustomer=='no') {
      $where[] = "sor.iscustomer=0";
    }
  }

  $isprovider = $_SESSION['stormorganization_list_filter']['isprovider'];
  if ($isprovider && $isprovider!='-') {
    if ($isprovider=='yes') {
      $where[] = "sor.isprovider=1";
    }
    else if ($isprovider=='no') {
      $where[] = "sor.isprovider=0";
    }
  }

  $isactive = $_SESSION['stormorganization_list_filter']['isactive'];
  if ($isactive && $isactive!='-') {
    if ($isactive=='yes') {
      $where[] = "sor.isactive=1";
    }
    else if ($isactive=='no') {
      $where[] = "sor.isactive=0";
    }
  }

  $itemsperpage = $_SESSION['stormorganization_list_filter']['itemsperpage'];

  $s = stormorganization_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[] = $organization;
  }

  $o .= theme('stormorganization_list', $header, $organizations);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormorganization_list_filter() {
  $country = $_SESSION['stormorganization_list_filter']['country'];
  $name = $_SESSION['stormorganization_list_filter']['name'];
  $iscustomer = $_SESSION['stormorganization_list_filter']['iscustomer'];
  $isprovider = $_SESSION['stormorganization_list_filter']['isprovider'];
  $isactive = $_SESSION['stormorganization_list_filter']['isactive'];
  if (!$isactive) {
    $isactive = 'yes';
    $_SESSION['stormorganization_list_filter']['isactive'] = $isactive;
  }
  $itemsperpage = $_SESSION['stormorganization_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormorganization_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => storm_t('Filter', 'organization'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => -20,
  );

  $form['filter']['country'] = array(
    '#type' => 'select',
    '#title' => storm_t('Country', 'organization'),
    '#default_value' => $country,
    '#options' => array(0 => storm_t('All', 'organization')) + stormattribute_attributes_bydomain('Country'),
  );

  $form['filter']['name'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Name', 'organization'),
    '#default_value' => $name,
    '#autocomplete_path' => 'storm/organizations/autocomplete',
  );

  $qs = '<div class="storm_quick_shortcuts"><ul class="storm_quick_shortcuts">';
  foreach (range(0, 9) as $n) {
    $qs .= '<li>'. l($n, 'storm/organizations', array('query' => array('name' => $n .'%'))) .'</li>';
  }
  $qs .= '</ul></div>';
  $form['filter']['name_quick_shortcuts_numbers'] = array(
    '#type' => 'markup',
    '#value' => $qs,
  );
  $qs = '<div class="storm_quick_shortcuts"><ul class="storm_quick_shortcuts">';
  foreach (range('A', 'Z') as $l) {
    $qs .= '<li>'. l($l, 'storm/organizations', array('query' => array('name' => $l .'%'))) .'</li>';
  }
  $qs .= '</ul></div>';
  $form['filter']['name_quick_shortcuts_letters'] = array(
    '#type' => 'markup',
    '#value' => $qs,
  );

  $form['filter']['group0'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $options = array (
    '-' => storm_t('-', 'organization'),
    'no' => storm_t('no', 'organization'),
    'yes' => storm_t('yes', 'organization')
  );
  $form['filter']['group0']['iscustomer'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => storm_t('Is customer', 'organization'),
    '#default_value' => $iscustomer,
  );

  $form['filter']['group0']['isprovider'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => storm_t('Is provider', 'organization'),
    '#default_value' => $isprovider,
  );

  $form['filter']['group0']['isactive'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => storm_t('Is active', 'organization'),
    '#default_value' => $isactive,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'organization'),
    '#submit' => array('stormorganization_list_filter_filter'),
  );

  $form['filter']['group1']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'organization'),
    '#submit' => array('stormorganization_list_filter_reset'),
  );

  $form['filter']['group1']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'organization'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormorganization_list_filter_filter($form, &$form_state) {
  $_SESSION['stormorganization_list_filter']['country'] = $form_state['values']['country'];
  $_SESSION['stormorganization_list_filter']['name'] = $form_state['values']['name'];
  $_SESSION['stormorganization_list_filter']['iscustomer'] = $form_state['values']['iscustomer'];
  $_SESSION['stormorganization_list_filter']['isprovider'] = $form_state['values']['isprovider'];
  $_SESSION['stormorganization_list_filter']['isactive'] = $form_state['values']['isactive'];
  $_SESSION['stormorganization_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormorganization_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormorganization_list_filter']);
}

function stormorganization_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $s = "SELECT title FROM {node} AS n WHERE n.type='stormorganization' AND LOWER(title) LIKE LOWER('%s%%')";
    $s = stormorganization_access_sql($s);
    $s = db_rewrite_sql($s);

    $result = db_query_range($s, $string, 0, 10);
    while ($org = db_fetch_object($result)) {
      $matches[$org->title] = check_plain($org->title);
    }
  }

  drupal_json($matches);
}

