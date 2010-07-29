<?php
// $Id: stormproject.admin.inc,v 1.9.4.12 2009/01/07 08:40:36 robertogerola Exp $

/**
 * @file
 */

function stormproject_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormproject_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormproject_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
  }

  $o = drupal_get_form('stormproject_list_filter');

  $i = new stdClass();
  $i->type = 'stormproject';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => storm_t('Organization', 'project'),
      'field' => 'spr.organization_title',
      'sort' => 'asc',
    ),
    array(
      'data' => storm_t('Title', 'project'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_t('Status', 'project'),
      'field' => 'spr.projectstatus',
    ),
    array(
      'data' => storm_t('Priority', 'project'),
      'field' => 'spr.projectpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s  = "SELECT n.*, spr.* FROM {node} AS n INNER JOIN {stormproject} AS spr ON n.vid=spr.vid WHERE n.status=1 AND n.type='stormproject'";

  $where = array();
  $args = array();

  if ($_SESSION['stormproject_list_filter']['organization_nid']) {
    $where[] = 'spr.organization_nid=%d';
    $args[] = $_SESSION['stormproject_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormproject_list_filter']['projectcategory'] && $_SESSION['stormproject_list_filter']['projectcategory'] != '-') {
    $where[] = "spr.projectcategory='%s'";
    $args[] = $_SESSION['stormproject_list_filter']['projectcategory'];
  }
  if ($_SESSION['stormproject_list_filter']['projectstatus'] && $_SESSION['stormproject_list_filter']['projectstatus'] != '-') {
    $status = split(',', $_SESSION['stormproject_list_filter']['projectstatus']);
    $v = array();
    foreach($status as $item) $v[] = '%s';
    $where[] = "spr.projectstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormproject_list_filter']['projectpriority'] && $_SESSION['stormproject_list_filter']['projectpriority'] != '-') {
    $where[] = "spr.projectpriority='%s'";
    $args[] = $_SESSION['stormproject_list_filter']['projectpriority'];
  }

  $datebeginfrom = $_SESSION['stormproject_list_filter']['datebeginfrom'];
  if ($datebeginfrom) {
    $datebeginfrom['hour'] = 0;
    $datebeginfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($datebeginfrom);
    if ($datebeginfrom['year']>0 && $t>=0) {
      $where[] = 'spr.datebegin>=%d';
      $args[] = $t;
    }
  }

  $datebeginto = $_SESSION['stormproject_list_filter']['datebeginto'];
  if ($datebeginto) {
    $datebeginto['hour'] = 23;
    $datebeginto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($datebeginto);
    if ($datebeginto['year']>0 && $t>=0) {
      $where[] = 'spr.datebegin<=%d';
      $args[] = $t;
    }
  }

  $dateendfrom = $_SESSION['stormproject_list_filter']['dateendfrom'];
  if ($dateendfrom) {
    $dateendfrom['hour'] = 0;
    $dateendfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($dateendfrom);
    if ($dateendfrom['year']>0 && $t>=0) {
      $where[] = 'spr.dateend>=%d';
      $args[] = $t;
    }      
  }

  $dateendto = $_SESSION['stormproject_list_filter']['dateendto'];
  if ($dateendto) {
    $dateendto['hour'] = 23;
    $dateendto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($dateendto);
    if ($dateendto['year']>0 && $t>=0) {
      $where[] = 'spr.dateend<=%d';
      $args[] = $t;
    }
  }

  $itemsperpage = $_SESSION['stormproject_list_filter']['itemsperpage'];

  $tablesort = tablesort_sql($header);

  $s = stormproject_access_sql($s, $where);
  $s = db_rewrite_sql($s);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[] = $project;
  }

  $o .= theme('stormproject_list', $header, $projects);
  $o .= theme('pager', NULL, 10, 0);
  print theme('page', $o);
}

function stormproject_list_filter() {
  $organization_nid = $_SESSION['stormproject_list_filter']['organization_nid'];
  $projectcategory = $_SESSION['stormproject_list_filter']['projectcategory'];
  $status_list = stormattribute_attributes_bydomain('project status search');
  $projectstatus = $_SESSION['stormproject_list_filter']['projectstatus'];
  if (!$projectstatus) {
    $rstatus_list = array_flip($status_list);
    $projectstatus = $rstatus_list['open'];
    $_SESSION['stormproject_list_filter']['projectstatus'] = $projectstatus;
  }
  $priority_list = stormattribute_attributes_bydomain('project priority search');
  $projectpriority = $_SESSION['stormproject_list_filter']['projectpriority'];
  if (!$projectpriority) {
    $rpriority_list = array_flip($priority_list);
    $projectpriority = $rpriority_list['-'];
    $_SESSION['stormproject_list_filter']['projectpriority'] = $projectpriority;
  }

  $datebeginfrom = $_SESSION['stormproject_list_filter']['datebeginfrom'];
  $datebeginto = $_SESSION['stormproject_list_filter']['datebeginto'];
  $dateendfrom = $_SESSION['stormproject_list_filter']['dateendfrom'];
  $dateendto = $_SESSION['stormproject_list_filter']['dateendto'];

  $itemsperpage = $_SESSION['stormproject_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormproject_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => storm_t('Filter', 'project'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON n.vid=sor.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title ";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);

  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $form['filter']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Organization', 'project'),
    '#default_value' => $organization_nid,
    '#options' => array(0 => storm_t('All', 'project')) + $organizations,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['projectcategory'] = array(
    '#type' => 'select',
    '#title' => storm_t('Category', 'project'),
    '#default_value' => $projectcategory,
    '#options' => array('-' => storm_t('all', 'project')) + stormattribute_attributes_bydomain('Project category'),
  );

  $form['filter']['group1']['projectstatus'] = array(
    '#type' => 'select',
    '#title' => storm_t('Status', 'project'),
    '#default_value' => $projectstatus,
    '#options' => array('-' => storm_t('all', 'project')) + $status_list,
  );

  $form['filter']['group1']['projectpriority'] = array(
    '#type' => 'select',
    '#title' => storm_t('Priority', 'project'),
    '#default_value' => $projectpriority,
    '#options' => array('-' => storm_t('all', 'project')) + $priority_list,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['datebeginfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date begin from', 'task'),
    '#default_value' => $datebeginfrom,
  );

  $form['filter']['group2']['datebeginto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date begin to', 'task'),
    '#default_value' => $datebeginto,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['dateendfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date end from', 'task'),
    '#default_value' => $dateendfrom,
  );

  $form['filter']['group3']['dateendto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date end to', 'task'),
    '#default_value' => $dateendto,
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group4']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'project'),
    '#submit' => array('stormproject_list_filter_filter'),
  );

  $form['filter']['group4']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'project'),
    '#submit' => array('stormproject_list_filter_reset'),
  );

  $form['filter']['group4']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'project'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormproject_list_filter_filter($form, &$form_state) {
  $_SESSION['stormproject_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormproject_list_filter']['projectcategory'] = $form_state['values']['projectcategory'];
  $_SESSION['stormproject_list_filter']['projectstatus'] = $form_state['values']['projectstatus'];
  $_SESSION['stormproject_list_filter']['projectpriority'] = $form_state['values']['projectpriority'];
  $_SESSION['stormproject_list_filter']['datebeginfrom'] = $form_state['values']['datebeginfrom'];
  $_SESSION['stormproject_list_filter']['datebeginto'] = $form_state['values']['datebeginto'];
  $_SESSION['stormproject_list_filter']['dateendfrom'] = $form_state['values']['dateendfrom'];
  $_SESSION['stormproject_list_filter']['dateendto'] = $form_state['values']['dateendto'];
  $_SESSION['stormproject_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormproject_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormproject_list_filter']);
}

function _stormproject_organization_projects_js($organization_nid=0) {
  $projects = array();

  if ($organization_nid) {
    $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormproject} AS spr ON n.vid=spr.vid WHERE n.status=1 AND n.type='stormproject' AND spr.organization_nid=%d ORDER BY n.title";
    $s = stormproject_access_sql($s);
    $s = db_rewrite_sql($s);
    $r = db_query($s, $organization_nid);

    while ($item = db_fetch_object($r)) {
      $nid = $item->nid;
      $projects[$nid] = $item->title;
    }
  }
  print drupal_to_js($projects);
  exit();
}

