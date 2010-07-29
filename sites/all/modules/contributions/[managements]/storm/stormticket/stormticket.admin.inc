<?php
// $Id: stormticket.admin.inc,v 1.8.4.10 2008/12/18 09:15:46 robertogerola Exp $

/**
 * @file
 */

function stormticket_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormticket_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormticket_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormticket_list_filter']['project_nid']);
    unset($_SESSION['stormticket_list_filter']['task_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormticket_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormticket_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormticket_list_filter']['organization_nid'] = $p->organization_nid;

    unset($_SESSION['stormticket_list_filter']['task_nid']);
  }

  if (array_key_exists('task_nid', $_GET)) {
    if ($_SESSION['stormticket_list_filter']['task_nid'] != $_GET['task_nid']) {
      $_SESSION['stormticket_list_filter']['task_nid'] = $_GET['task_nid'];
    }

    $t = node_load($_GET['task_nid']);
    $_SESSION['stormticket_list_filter']['organization_nid'] = $t->organization_nid;
    $_SESSION['stormticket_list_filter']['project_nid'] = $t->project_nid;
  }

  $o = drupal_get_form('stormticket_list_filter');

  $i = new stdClass();
  $i->type = 'stormticket';

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => storm_t('Organization', 'ticket'),
      'field' => 'sti.organization_title',
    ),
    array(
      'data' => storm_t('Project', 'ticket'),
      'field' => 'sti.project_title',
    ),
    array(
      'data' => storm_t('Title', 'ticket'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_t('Date', 'ticket'),
      'field' => 'n.created',
      'sort' => 'desc',
    ),
    array(
      'data' => storm_t('Status', 'ticket'),
      'field' => 'sti.ticketstatus',
    ),
    array(
      'data' => storm_t('Priority', 'ticket'),
      'field' => 'sti.ticketpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  $s = "SELECT n.*, sti.* FROM  {node} AS n INNER JOIN {stormticket} AS sti ON n.vid=sti.vid WHERE n.status=1 AND n.type='stormticket' ";
  if ($_SESSION['stormticket_list_filter']['organization_nid']) {
    $where[] = 'sti.organization_nid=%d';
    $args[] = $_SESSION['stormticket_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['project_nid']) {
    $where[] = 'sti.project_nid=%d';
    $args[] = $_SESSION['stormticket_list_filter']['project_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['task_nid']) {
    $where[] = 'sti.task_nid=%d';
    $args[] = $_SESSION['stormticket_list_filter']['task_nid'];
  }
  if ($_SESSION['stormticket_list_filter']['ticketstatus'] && $_SESSION['stormticket_list_filter']['ticketstatus'] != '-') {
    $status = split(',', $_SESSION['stormticket_list_filter']['ticketstatus']);
    $v = array();
    foreach($status as $item) $v[] = '%s';
    $where[] = "sti.ticketstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormticket_list_filter']['ticketpriority'] && $_SESSION['stormticket_list_filter']['ticketpriority'] != '-') {
    $priority = split(',', $_SESSION['stormticket_list_filter']['ticketpriority']);
    $v = array();
    foreach($priority as $item) $v[] = '%s';
    $where[] = "sti.ticketpriority IN ('". implode("','",$v) ."')";
    foreach ($priority as $key=>$value) {
      $args[] = $value;
    }
  }

  $datebeginfrom = $_SESSION['stormticket_list_filter']['datebeginfrom'];
  if ($datebeginfrom) {
    $datebeginfrom['hour'] = 0;
    $datebeginfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($datebeginfrom);
    if ($datebeginfrom['year']>0 && $t>=0) {
      $where[] = 'sti.datebegin>=%d';
      $args[] = $t;
    }
  }

  $datebeginto = $_SESSION['stormticket_list_filter']['datebeginto'];
  if ($datebeginto) {
    $datebeginto['hour'] = 23;
    $datebeginto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($datebeginto);
    if ($datebeginto['year']>0 && $t>=0) {
      $where[] = 'sti.datebegin<=%d';
      $args[] = $t;
    }
  }

  $dateendfrom = $_SESSION['stormticket_list_filter']['dateendfrom'];
  if ($dateendfrom) {
    $dateendfrom['hour'] = 0;
    $dateendfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($dateendfrom);
    if ($dateendfrom['year']>0 && $t>=0) {
      $where[] = 'sti.dateend>=%d';
      $args[] = $t;
    }
  }

  $dateendto = $_SESSION['stormticket_list_filter']['dateendto'];
  if ($dateendto) {
    $dateendto['hour'] = 23;
    $dateendto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($dateendto);
    if ($dateendto['year']>0 && $t>=0) {
      $where[] = 'sti.dateend<=%d';
      $args[] = $t;
    }
  }

  $itemsperpage = $_SESSION['stormticket_list_filter']['itemsperpage'];

  $s = stormticket_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $tickets = array();
  while ($item = db_fetch_object($r)) {
    $tickets[] = $item;
  }

  $o .= theme('stormticket_list', $header, $tickets);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormticket_list_filter() {
  $status_list = stormattribute_attributes_bydomain('ticket status search');

  $organization_nid = $_SESSION['stormticket_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormticket_list_filter']['project_nid'];
  $task_nid = $_SESSION['stormticket_list_filter']['task_nid'];
  $ticketstatus = $_SESSION['stormticket_list_filter']['ticketstatus'];
  if (!$ticketstatus) {
    $rstatus_list = array_flip($status_list);
    $ticketstatus = $rstatus_list['open'];
    $_SESSION['stormticket_list_filter']['ticketstatus'] = $ticketstatus;
  }
  $ticketpriority = $_SESSION['stormticket_list_filter']['ticketpriority'];

  $itemsperpage = $_SESSION['stormticket_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormticket_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $datebeginfrom = $_SESSION['stormticket_list_filter']['datebeginfrom'];
  $datebeginto = $_SESSION['stormticket_list_filter']['datebeginto'];
  $dateendfrom = $_SESSION['stormticket_list_filter']['dateendfrom'];
  $dateendto = $_SESSION['stormticket_list_filter']['dateendto'];

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => storm_t('Filter', 'ticket'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#theme' => 'stormticket_list_filter',
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => storm_t('All', 'ticket')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Organization', 'ticket'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormtask_organization_project_tasks(this, 'edit-project-nid', 'edit-task-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormproject} AS spr ON spr.vid=n.vid
  WHERE n.status=1 AND spr.organization_nid=%d AND n.type='stormproject' ORDER BY n.title";
  $s = stormproject_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid);
  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[$project->nid] = $project->title;
  }
  $projects = array(0 => storm_t('All', 'ticket')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Project', 'ticket'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormtask_project_tasks(this, 'edit-task-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormtask} AS sta
  ON sta.vid=n.vid WHERE n.status=1 AND type='stormtask' AND sta.project_nid=%d ORDER BY title ";
  $s = stormtask_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $project_nid);
  $tasks = array();
  while ($task = db_fetch_object($r)) {
    $tasks[$task->nid] = $task->title;
  }
  $tasks = array(0 => storm_t('All', 'ticket')) + $tasks;
  $form['filter']['group1']['task_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Task', 'ticket'),
    '#default_value' => $task_nid,
    '#options' => $tasks,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['ticketstatus'] = array(
    '#type' => 'select',
    '#title' => storm_t('Status', 'ticket'),
    '#default_value' => $ticketstatus,
    '#options' => array('-' => storm_t('all', 'ticket')) + $status_list,
  );

  $form['filter']['group2']['ticketpriority'] = array(
    '#type' => 'select',
    '#title' => storm_t('Priority', 'ticket'),
    '#default_value' => $ticketpriority,
    '#options' => array('-' => storm_t('all', 'ticket')) + stormattribute_attributes_bydomain('ticket priority search'),
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['datebeginfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date begin from', 'ticket'),
    '#default_value' => $datebeginfrom,
  );

  $form['filter']['group3']['datebeginto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date begin to', 'ticket'),
    '#default_value' => $datebeginto,
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group4']['dateendfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date end from', 'ticket'),
    '#default_value' => $dateendfrom,
  );

  $form['filter']['group4']['dateendto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date end to', 'ticket'),
    '#default_value' => $dateendto,
  );

  $form['filter']['group5'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group5']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'ticket'),
    '#submit' => array('stormticket_list_filter_filter'),
  );

  $form['filter']['group5']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'ticket'),
    '#submit' => array('stormticket_list_filter_reset'),
  );

  $form['filter']['group5']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'ticket'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormticket_list_filter_filter($form, &$form_state) {
  $_SESSION['stormticket_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormticket_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormticket_list_filter']['task_nid'] = $form_state['values']['task_nid'];
  $_SESSION['stormticket_list_filter']['ticketstatus'] = $form_state['values']['ticketstatus'];
  $_SESSION['stormticket_list_filter']['ticketpriority'] = $form_state['values']['ticketpriority'];
  $_SESSION['stormticket_list_filter']['datebeginfrom'] = $form_state['values']['datebeginfrom'];
  $_SESSION['stormticket_list_filter']['datebeginto'] = $form_state['values']['datebeginto'];
  $_SESSION['stormticket_list_filter']['dateendfrom'] = $form_state['values']['dateendfrom'];
  $_SESSION['stormticket_list_filter']['dateendto'] = $form_state['values']['dateendto'];
  $_SESSION['stormticket_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormticket_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormticket_list_filter']);
}

function _stormticket_task_tickets_js($organization_nid=0, $project_nid=0, $task_nid=0) {
  $tickets = array();

  $s = "SELECT n.nid, n.title FROM {node} n INNER JOIN {stormticket} sti
  ON n.vid=sti.vid WHERE n.status=1 AND n.type='stormticket' AND sti.organization_nid=%d AND sti.project_nid=%d
  AND sti.task_nid=%d ORDER BY n.title";

  $s = stormticket_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid, $project_nid, $task_nid);
  while ($item = db_fetch_object($r)) {
    $nid = $item->nid;
    $tickets[$nid] = $item->title;
  }
  print drupal_to_js($tickets);
  exit();
}
