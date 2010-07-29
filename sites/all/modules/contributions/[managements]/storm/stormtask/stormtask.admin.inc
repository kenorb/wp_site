<?php
// $Id: stormtask.admin.inc,v 1.8.4.11 2009/01/02 08:49:14 robertogerola Exp $

/**
 * @file
 */

function stormtask_tasks($project) {
  $node_invoice = node_load($invoice->nid);
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  $breadcrumb[] = l(t('Projects'), 'storm/projects');
  $breadcrumb[] = l(t('Project : '. $project->title), 'node/'. $project->nid);
  drupal_set_breadcrumb($breadcrumb);

  $i = new stdClass();
  $i->type = 'stormtask';

  $o = drupal_get_form('stormtask_tasks_filter');

  $params = $_GET;
  $params['organization_nid'] = $project->organization_nid;
  $params['project_nid'] = $project->nid;

  $header = array(
    array(
      'data' => storm_t('Title', 'task'),
    ),
    array(
      'data' => storm_t('Category', 'task'),
    ),
    array(
      'data' => storm_t('Status', 'task'),
    ),
    array(
      'data' => storm_t('Priority', 'task'),
    ),
    array(
      'data' => storm_t('Parent', 'task'),
    ),
    array(
      'data' => storm_t('Weight', 'task'),
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  if ($_SESSION['stormtask_tasks_filter']['taskcategory'] && $_SESSION['stormtask_tasks_filter']['taskcategory'] != '-') {
    $category = split(',', $_SESSION['stormtask_tasks_filter']['taskcategory']);
    $v = array();
    foreach($category as $item) $v[] = '%s';
    $where[] = "sta.taskcategory IN ('". implode("','",$v) ."')";
    foreach ($category as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormtask_tasks_filter']['taskstatus'] && $_SESSION['stormtask_tasks_filter']['taskstatus'] != '-') {
    $status = split(',', $_SESSION['stormtask_tasks_filter']['taskstatus']);
    $v = array();
    foreach($status as $item) $v[] = '%s';
    $where[] = "sta.taskstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormtask_tasks_filter']['taskpriority'] && $_SESSION['stormtask_tasks_filter']['taskpriority'] != '-') {
    $priority = split(',', $_SESSION['stormtask_tasks_filter']['taskpriority']);
    $v = array();
    foreach($priority as $item) $v[] = '%s';
    $where[] = "sta.taskpriority IN ('". implode("','",$v) ."')";
    foreach ($priority as $key=>$value) {
      $args[] = $value;
    }
  }

  $taskstree = _stormtask_get_tree($project->nid, 0, -1, NULL, $where, $args);
  $o .= drupal_get_form('stormtask_tasks_form', $header, $taskstree, $project);
  return $o;
}

function stormtask_tasks_form($form_id, $header, $tasks, $project) {
  $form = array();

  $form['tasks'] = array(
    '#theme' => 'stormtask_tasks',
  );

  $form['tasks']['header'] = array (
    '#value' => $header,
  );

  $tree = _stormtask_get_tree($project->nid);
  $parent_tasks = _stormtask_plain_tree($tree);

  $params = $_GET;
  $params['project_nid'] = $project->nid;

  foreach ($tasks as $task) {
    $n = new stdClass();
    $n->nid = $task->nid;
    $n->uid = $task->uid;
    $n->type = 'stormtask';

    $form['tasks']['tasks'][$task->nid]['task_depth_'. $task->nid] = array (
      '#value' => $task->depth,
    );

    $form['tasks']['tasks'][$task->nid]['task_description_'. $task->nid] = array (
      '#value' => l($task->stepno .' '. $task->title, 'node/'. $task->nid, array('html' => TRUE)),
    );

    $form['tasks']['tasks'][$task->nid]['task_category_'. $task->nid] = array (
      '#value' => storm_icon('category_'. $task->taskcategory, $task->taskcategory),
    );

    $form['tasks']['tasks'][$task->nid]['task_status_'. $task->nid] = array (
      '#value' => storm_icon('status_'. $task->taskstatus, $task->taskstatus),
    );

    $form['tasks']['tasks'][$task->nid]['task_priority_'. $task->nid] = array (
      '#value' => storm_icon('priority_'. $task->taskpriority, $task->taskpriority),
    );

    $form['tasks']['tasks'][$task->nid]['task_parent-nid_'. $task->nid] = array (
      '#type' => 'select',
      '#options' => array(0 => '-') + $parent_tasks,
      '#default_value' => $task->parent_nid,
    );

    $form['tasks']['tasks'][$task->nid]['task_nid_'. $task->nid] = array (
      '#type' => 'hidden',
      '#default_value' => $task->nid,
    );

    $form['tasks']['tasks'][$task->nid]['task_weight_'. $task->nid] = array (
      '#type' => 'weight',
      '#default_value' => $task->weight,
    );

    $form['tasks']['tasks'][$task->nid]['task_operations_'. $task->nid] = array (
      '#value' => storm_icon_edit_node($n, $params) .'&nbsp;'. storm_icon_delete_node($n, $params),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('stormtask_tasks_submit'),
    '#value' => storm_t('Save', 'task'),
  );

  return $form;
}

function stormtask_tasks_submit($form, &$form_state) {
  $tasks = array();
  foreach($form_state['values'] as $key=>$value) {
    $ar = explode('_', $key);
    if($ar[0]=='task') {
      if($ar[1]=='weight') $tasks[$ar[2]]['weight'] = $value;
      if($ar[1]=='parent-nid') $tasks[$ar[2]]['parent-nid'] = $value;
    }
  }

  $s = "UPDATE {stormtask} SET weight=%d, parent_nid=%d WHERE nid=%d";  
  foreach($tasks as $nid=>$values) {
    db_query($s, $values['weight'], $values['parent-nid'], $nid);
  }

  drupal_set_message(storm_t('Tasks saved', 'task'));
}

function stormtask_tasks_filter() {
  $category_list = stormattribute_attributes_bydomain('Task category');
  $status_list = stormattribute_attributes_bydomain('Task status search');
  $priority_list = stormattribute_attributes_bydomain('Task priority search');

  $taskcategory = $_SESSION['stormtask_tasks_filter']['taskcategory'];
  $taskstatus = $_SESSION['stormtask_tasks_filter']['taskstatus'];
  $taskpriority = $_SESSION['stormtask_tasks_filter']['taskpriority'];

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => storm_t('Filter', 'task'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group1']['taskcategory'] = array(
    '#type' => 'select',
    '#title' => storm_t('Category', 'task'),
    '#default_value' => $taskcategory,
    '#options' => array('-' => storm_t('all', 'task')) + $category_list,
  );

  $form['filter']['group1']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => storm_t('Status', 'task'),
    '#default_value' => $taskstatus,
    '#options' => array('-' => storm_t('all', 'task')) + $status_list,
  );

  $form['filter']['group1']['taskpriority'] = array(
    '#type' => 'select',
    '#title' => storm_t('Priority', 'task'),
    '#default_value' => $taskpriority,
    '#options' => array('-' => storm_t('all', 'task')) + $priority_list,
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'task'),
    '#submit' => array('stormtask_tasks_filter_filter'),
  );

  $form['filter']['group2']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'task'),
    '#submit' => array('stormtask_tasks_filter_reset'),
  );

  return $form;
}

function stormtask_tasks_filter_filter($form, &$form_state) {
  $_SESSION['stormtask_tasks_filter']['taskcategory'] = $form_state['values']['taskcategory'];
  $_SESSION['stormtask_tasks_filter']['taskstatus'] = $form_state['values']['taskstatus'];
  $_SESSION['stormtask_tasks_filter']['taskpriority'] = $form_state['values']['taskpriority'];
}

function stormtask_tasks_filter_reset($form, &$form_state) {
  unset($_SESSION['stormtask_tasks_filter']);
}

function _stormtask_project_tasks_js($project_nid) {
  $tree = _stormtask_get_tree($project_nid);
  $tasks = _stormtask_plain_tree($tree);
  print drupal_to_js($tasks);
  exit();
}

function stormtask_list() {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  drupal_set_breadcrumb($breadcrumb);

  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormtask_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormtask_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormtask_list_filter']['project_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormtask_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormtask_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormtask_list_filter']['organization_nid'] = $p->organization_nid;

  }

  $o = drupal_get_form('stormtask_list_filter');

  $i = new stdClass();
  $i->type = 'stormtask';

  $header = array(
    array(
      'data' => storm_t('Organization', 'task'),
      'field' => 'sta.organization_title',
    ),
    array(
      'data' => storm_t('Project', 'task'),
      'field' => 'sta.project_title',
    ),
    array(
      'data' => storm_t('Title', 'task'),
      'field' => 'n.title',
    ),
    array(
      'data' => storm_t('Category', 'task'),
      'field' => 'sta.taskcategory',
    ),
    array(
      'data' => storm_t('Status', 'task'),
      'field' => 'sta.taskstatus',
    ),
    array(
      'data' => storm_t('Priority', 'task'),
      'field' => 'sta.taskpriority',
    ),
    array(
      'data' => storm_icon_add_node($i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $args = array();

  $s  = "SELECT n.*, sta.* FROM {node} AS n INNER JOIN {stormtask} AS sta ON n.vid=sta.vid WHERE n.status=1 AND n.type='stormtask' ";
  if ($_SESSION['stormtask_list_filter']['organization_nid']) {
    $where[] = 'sta.organization_nid=%d';
    $args[] = $_SESSION['stormtask_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormtask_list_filter']['project_nid']) {
    $where[] = 'sta.project_nid=%d';
    $args[] = $_SESSION['stormtask_list_filter']['project_nid'];
  }
  if ($_SESSION['stormtask_list_filter']['taskcategory'] && $_SESSION['stormtask_list_filter']['taskcategory'] != '-') {
    $category = split(',', $_SESSION['stormtask_list_filter']['taskcategory']);
    $v = array();
    foreach($category as $item) $v[] = '%s';
    $where[] = "sta.taskcategory IN ('". implode("','",$v) ."')";
    foreach ($category as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormtask_list_filter']['taskstatus'] && $_SESSION['stormtask_list_filter']['taskstatus'] != '-') {
    $status = split(',', $_SESSION['stormtask_list_filter']['taskstatus']);
    $v = array();
    foreach($status as $item) $v[] = '%s';
    $where[] = "sta.taskstatus IN ('". implode("','",$v) ."')";
    foreach ($status as $key=>$value) {
      $args[] = $value;
    }
  }
  if ($_SESSION['stormtask_list_filter']['taskpriority'] && $_SESSION['stormtask_list_filter']['taskpriority'] != '-') {
    $priority = split(',', $_SESSION['stormtask_list_filter']['taskpriority']);
    $v = array();
    foreach($priority as $item) $v[] = '%s';
    $where[] = "sta.taskpriority IN ('". implode("','",$v) ."')";
    foreach ($priority as $key=>$value) {
      $args[] = $value;
    }
  }

  $datebeginfrom = $_SESSION['stormtask_list_filter']['datebeginfrom'];
  if ($datebeginfrom) {
    $datebeginfrom['hour'] = 0;
    $datebeginfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($datebeginfrom);
    if ($datebeginfrom['year']>0 && $t>=0) {
      $where[] = 'sta.datebegin>=%d';
      $args[] = $t;
    }
  }

  $datebeginto = $_SESSION['stormtask_list_filter']['datebeginto'];
  if ($datebeginto) {
    $datebeginto['hour'] = 23;
    $datebeginto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($datebeginto);
    if ($datebeginto['year']>0 && $t>=0) {
      $where[] = 'sta.datebegin<=%d';
      $args[] = $t;
    }
  }

  $dateendfrom = $_SESSION['stormtask_list_filter']['dateendfrom'];
  if ($dateendfrom) {
    $dateendfrom['hour'] = 0;
    $dateendfrom['minute'] = 0;
    $t = _storm_datetime_to_gmtimestamp($dateendfrom);
    if ($dateendfrom['year']>0 && $t>=0) {
      $where[] = 'sta.dateend>=%d';
      $args[] = $t;
    }
  }

  $dateendto = $_SESSION['stormtask_list_filter']['dateendto'];
  if ($dateendto) {
    $dateendto['hour'] = 23;
    $dateendto['minute'] = 59;
    $t = _storm_datetime_to_gmtimestamp($dateendto);
    if ($dateendto['year']>0 && $t>=0) {
      $where[] = 'sta.dateend<=%d';
      $args[] = $t;
    }
  }

  $itemsperpage = $_SESSION['stormtask_list_filter']['itemsperpage'];

  $s = stormtask_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL, $args);

  $tasks = array();
  while ($item = db_fetch_object($r)) {
    $tasks[] = $item;
  }

  $o .= theme('stormtask_list', $header, $tasks);
  $o .= theme('pager', NULL, $itemperpage, 0);
  print theme('page', $o);
}

function stormtask_list_filter() {
  $category_list = stormattribute_attributes_bydomain('Task category');
  $status_list = stormattribute_attributes_bydomain('Task status search');
  $priority_list = stormattribute_attributes_bydomain('Task priority search');

  $organization_nid = $_SESSION['stormtask_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormtask_list_filter']['project_nid'];
  $taskcategory = $_SESSION['stormtask_list_filter']['taskcategory'];
  $taskstatus = $_SESSION['stormtask_list_filter']['taskstatus'];
  $taskpriority = $_SESSION['stormtask_list_filter']['taskpriority'];

  if (!$taskcategory) {
    $rcategory_list = array_flip($category_list);
    $taskcategory = $rcategory_list['open'];
    $_SESSION['stormtask_list_filter']['taskcategory'] = $taskcategory;
  }

  if (!$taskstatus) {
    $rstatus_list = array_flip($status_list);
    $taskstatus = $rstatus_list['open'];
    $_SESSION['stormtask_list_filter']['taskstatus'] = $taskstatus;
  }

  if (!$taskpriority) {
    $rpriority_list = array_flip($priority_list);
    $taskpriority = $priority_list['open'];
    $_SESSION['stormtask_list_filter']['taskpriority'] = $taskpriority;
  }

  $datebeginfrom = $_SESSION['stormtask_list_filter']['datebeginfrom'];
  $datebeginto = $_SESSION['stormtask_list_filter']['datebeginto'];
  $dateendfrom = $_SESSION['stormtask_list_filter']['dateendfrom'];
  $dateendto = $_SESSION['stormtask_list_filter']['dateendto'];

  $itemsperpage = $_SESSION['stormtask_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormtask_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => storm_t('Filter', 'task'),
    '#collapsible' => true,
    '#collapsed' => true,
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => storm_t('All', 'task')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Organization', 'task'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormproject_organization_projects(this, 'edit-project-nid', true, 'All')"),
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
  $projects = array(0 => storm_t('All', 'task')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Project', 'task'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
  );

  $form['filter']['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group2']['taskcategory'] = array(
    '#type' => 'select',
    '#title' => storm_t('Category', 'task'),
    '#default_value' => $taskcategory,
    '#options' => array('-' => storm_t('all', 'task')) + $category_list,
  );

  $form['filter']['group2']['taskstatus'] = array(
    '#type' => 'select',
    '#title' => storm_t('Status', 'task'),
    '#default_value' => $taskstatus,
    '#options' => array('-' => storm_t('all', 'task')) + $status_list,
  );

  $form['filter']['group2']['taskpriority'] = array(
    '#type' => 'select',
    '#title' => storm_t('Priority', 'task'),
    '#default_value' => $taskpriority,
    '#options' => array('-' => storm_t('all', 'task')) + $priority_list,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['datebeginfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date begin from', 'task'),
    '#default_value' => $datebeginfrom,
  );

  $form['filter']['group3']['datebeginto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date begin to', 'task'),
    '#default_value' => $datebeginto,
  );

  $form['filter']['group4'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group4']['dateendfrom'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date end from', 'task'),
    '#default_value' => $dateendfrom,
  );

  $form['filter']['group4']['dateendto'] = array(
    '#type' => 'dateext',
    '#withnull' => 'true',
    '#title' => storm_t('Date end to', 'task'),
    '#default_value' => $dateendto,
  );

  $form['filter']['group5'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group5']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'task'),
    '#submit' => array('stormtask_list_filter_filter'),
  );

  $form['filter']['group5']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'task'),
    '#submit' => array('stormtask_list_filter_reset'),
  );

  $form['filter']['group5']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'task'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormtask_list_filter_filter($form, &$form_state) {
  $_SESSION['stormtask_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormtask_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormtask_list_filter']['taskcategory'] = $form_state['values']['taskcategory'];
  $_SESSION['stormtask_list_filter']['taskstatus'] = $form_state['values']['taskstatus'];
  $_SESSION['stormtask_list_filter']['taskpriority'] = $form_state['values']['taskpriority'];
  $_SESSION['stormtask_list_filter']['datebeginfrom'] = $form_state['values']['datebeginfrom'];
  $_SESSION['stormtask_list_filter']['datebeginto'] = $form_state['values']['datebeginto'];
  $_SESSION['stormtask_list_filter']['dateendfrom'] = $form_state['values']['dateendfrom'];
  $_SESSION['stormtask_list_filter']['dateendto'] = $form_state['values']['dateendto'];
  $_SESSION['stormtask_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormtask_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormtask_list_filter']);
}

