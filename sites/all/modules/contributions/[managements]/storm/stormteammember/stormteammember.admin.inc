<?php
// $Id: stormteammember.admin.inc,v 1.1.2.2 2009/01/07 08:40:36 robertogerola Exp $

/**
 * @file
 */

function stormteammember_list() {
  if (array_key_exists('organization_nid', $_GET)) {
    if ($_SESSION['stormteammember_list_filter']['organization_nid'] != $_GET['organization_nid']) {
      $_SESSION['stormteammember_list_filter']['organization_nid'] = $_GET['organization_nid'];
    }
    unset($_SESSION['stormteammember_list_filter']['project_nid']);
    unset($_SESSION['stormteammember_list_filter']['task_nid']);
    unset($_SESSION['stormteammember_list_filter']['ticket_nid']);
  }

  if (array_key_exists('project_nid', $_GET)) {
    if ($_SESSION['stormteammember_list_filter']['project_nid'] != $_GET['project_nid']) {
      $_SESSION['stormteammember_list_filter']['project_nid'] = $_GET['project_nid'];
    }
    $p = node_load($_GET['project_nid']);
    $_SESSION['stormteammember_list_filter']['organization_nid'] = $p->organization_nid;

    unset($_SESSION['stormteammember_list_filter']['task_nid']);
    unset($_SESSION['stormteammember_list_filter']['ticket_nid']);
  }

  if (array_key_exists('task_nid', $_GET)) {
    if ($_SESSION['stormteammember_list_filter']['task_nid'] != $_GET['task_nid']) {
      $_SESSION['stormteammember_list_filter']['task_nid'] = $_GET['task_nid'];
    }

    $t = node_load($_GET['task_nid']);
    $_SESSION['stormteammember_list_filter']['organization_nid'] = $t->organization_nid;
    $_SESSION['stormteammember_list_filter']['project_nid'] = $t->project_nid;
    unset($_SESSION['stormteammember_list_filter']['ticket_nid']);
  }

  if (array_key_exists('ticket_nid', $_GET)) {
    if ($_SESSION['stormteammember_list_filter']['ticket_nid'] != $_GET['ticket_nid']) {
      $_SESSION['stormteammember_list_filter']['ticket_nid'] = $_GET['ticket_nid'];
    }

    $ti = node_load($_GET['ticket_nid']);
    $_SESSION['stormteammember_list_filter']['organization_nid'] = $ti->organization_nid;
    $_SESSION['stormteammember_list_filter']['project_nid'] = $ti->project_nid;
    $_SESSION['stormteammember_list_filter']['task_nid'] = $ti->task_nid;
  }

  $o = drupal_get_form('stormteammember_list_filter');

  $i = new stdClass();
  $i->type = 'stormteammember';

  $header = array(
    array(
      'data' => storm_t('Organization', 'teammember'),
      'field' => 'stm.organization_title',
    ),
    array(
      'data' => storm_t('Project', 'teammember'),
      'field' => 'stm.project_title',
    ),
    array(
      'data' => storm_t('Task', 'teammember'),
      'field' => 'stm.task_title',
    ),
    array(
      'data' => storm_t('Ticket', 'teammember'),
      'field' => 'stm.ticket_title',
    ),
    array(
      'data' => storm_icon_add('storm/teammembers/add', $i, $_GET),
      'class' => 'storm_list_operations',
    ),
  );

  $s = "SELECT stm.* FROM {stormteammember} stm";

  if ($_SESSION['stormteammember_list_filter']['organization_nid']) {
    $where[] = 'stm.organization_nid='. $_SESSION['stormteammember_list_filter']['organization_nid'];
  }
  if ($_SESSION['stormteammember_list_filter']['project_nid']) {
    $where[] = 'stm.project_nid='. $_SESSION['stormteammember_list_filter']['project_nid'];
  }
  if ($_SESSION['stormteammember_list_filter']['task_nid']) {
    $where[] = 'stm.task_nid='. $_SESSION['stormteammember_list_filter']['task_nid'];
  }
  if ($_SESSION['stormteammember_list_filter']['ticket_nid']) {
    $where[] = 'stm.ticket_nid='. $_SESSION['stormteammember_list_filter']['ticket_nid'];
  }

  $itemsperpage = $_SESSION['stormteammember_list_filter']['itemsperpage'];

  $s = stormteammember_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = pager_query($s . $tablesort, $itemsperpage, 0, NULL);

  $_SESSION['stormteammember_list_filter']['sql'] = $s . $tablesort;

  $teammembers = array();
  while ($item = db_fetch_object($r)) {
    $teammembers[] = $item;
  }

  $o .= theme('stormteammember_list', $header, $teammembers);
  $o .= theme('pager', NULL, $itemsperpage, 0);
  print theme('page', $o);
}

function stormteammember_list_filter() {
  $organization_nid = $_SESSION['stormteammember_list_filter']['organization_nid'];
  $project_nid = $_SESSION['stormteammember_list_filter']['project_nid'];
  $task_nid = $_SESSION['stormteammember_list_filter']['task_nid'];
  $ticket_nid = $_SESSION['stormteammember_list_filter']['ticket_nid'];
  $itemsperpage = $_SESSION['stormteammember_list_filter']['itemsperpage'];
  if (!$itemsperpage) {
    $itemsperpage = 10;
    $_SESSION['stormteammember_list_filter']['itemsperpage'] = $itemsperpage;
  }

  $form = array();

  $form['filter'] = array(
        '#type' => 'fieldset',
        '#title' => storm_t('Filter', 'teammember'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#theme' => 'stormteammember_list_filter',
  );

  $form['filter']['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -20,
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormorganization} AS sor ON sor.vid=n.vid
  WHERE n.status=1 AND n.type='stormorganization' ORDER BY n.title";
  $s = stormorganization_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
  }
  $organizations = array(0 => storm_t('All', 'teammember')) + $organizations;
  $form['filter']['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Organization', 'teammember'),
    '#default_value' => $organization_nid,
    '#options' => $organizations,
    '#attributes' => array('onchange' => "stormticket_organization_project_task_tickets(this, 'edit-project-nid', 'edit-task-nid', 'edit-ticket-nid', true, 'All')"),
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
  $projects = array(0 => storm_t('All', 'teammember')) + $projects;
  $form['filter']['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Project', 'teammember'),
    '#default_value' => $project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormticket_project_task_tickets(this, 'edit-organization-nid', 'edit-task-nid', 'edit-ticket-nid', true, 'All')"),
  );

  $tree = _stormtask_get_tree($project_nid);
  $tasks = _stormtask_plain_tree($tree);
  $tasks = array(0 => storm_t('All', 'teammember')) + $tasks;
  $form['filter']['group1']['task_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Task', 'teammember'),
    '#default_value' => $task_nid,
    '#options' => $tasks,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormticket_task_tickets(this, 'edit-organization-nid', 'edit-project-nid', 'edit-ticket-nid', true, 'All')"),
  );

  $s = "SELECT n.nid, n.title FROM {node} AS n INNER JOIN {stormticket} AS sti
  ON sti.vid=n.vid WHERE n.status=1 AND n.type='stormticket' AND sti.organization_nid=%d AND sti.project_nid=%d AND sti.task_nid=%d ORDER BY title ";
  $s = stormticket_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $organization_nid, $project_nid, $task_nid);
  $tickets = array();
  while ($ticket = db_fetch_object($r)) {
    $tickets[$ticket->nid] = $ticket->title;
  }
  $tickets = array(0 => storm_t('All', 'teammember')) + $tickets;
  $form['filter']['group1']['ticket_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Ticket', 'teammember'),
    '#default_value' => $ticket_nid,
    '#process' => array('storm_dependent_select_process'),
    '#options' => $tickets,
  );

  $form['filter']['group3'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
  );

  $form['filter']['group3']['submit'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Filter', 'teammember'),
    '#submit' => array('stormteammember_list_filter_filter'),
  );

  $form['filter']['group3']['reset'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Reset', 'teammember'),
    '#submit' => array('stormteammember_list_filter_reset'),
  );

  $form['filter']['group3']['itemsperpage'] = array(
    '#type' => 'textfield',
    '#title' => storm_t('Items', 'teammember'),
    '#size' => 10,
    '#default_value' => $itemsperpage,
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );

  return $form;
}

function stormteammember_list_filter_filter($form, &$form_state) {
  $_SESSION['stormteammember_list_filter']['organization_nid'] = $form_state['values']['organization_nid'];
  $_SESSION['stormteammember_list_filter']['project_nid'] = $form_state['values']['project_nid'];
  $_SESSION['stormteammember_list_filter']['task_nid'] = $form_state['values']['task_nid'];
  $_SESSION['stormteammember_list_filter']['ticket_nid'] = $form_state['values']['ticket_nid'];
  $_SESSION['stormteammember_list_filter']['itemsperpage'] = $form_state['values']['itemsperpage'];
}

function stormteammember_list_filter_reset($form, &$form_state) {
  unset($_SESSION['stormteammember_list_filter']);
}

function stormteammember_add() {
  $teammember = new stdClass();
  return stormteammember_form($teammember);
}

function stormteammember_add_submit($form, &$form_state) {
  $teammember = new stdClass();
  $teammember->organization_nid = $form_state['values']['organization_nid'];
  $teammember->project_nid = $form_state['values']['project_nid'];
  $teammember->task_nid = $form_state['values']['task_nid'];
  $teammember->ticket_nid = $form_state['values']['ticket_nid'];

  _stormteammember_beforesave($teammember);

  db_query("INSERT INTO {stormteammember}
            (organization_nid, organization_title,
            project_nid, project_title,
            task_nid, task_title, task_stepno,
            ticket_nid, ticket_title,
            person_organization_nid, person_organization_title,
            person_nid, person_title
            ) VALUES
            (%d, '%s',
            %d, '%s',
            %d, '%s', '%s',
            %d, '%s', 
            %d, '%s',
            %d, '%s'
            )",
            $teammember->organization_nid, $teammember->organization_title,
            $teammember->project_nid, $teammember->project_title,
            $teammember->task_nid, $teammember->task_title, $teammember->task_stepno,
            $teammember->ticket_nid, $teammember->ticket_title,
            $teammember->person_organization_nid, $teammember->person_organization_title,
            $teammember->person_nid, $teammember->person_title
            );
}

function stormteammember_edit($form_state, $tid) {
  $teammember = new stdClass();
  if ($tid) {
    $r = db_query("SELECT * FROM {stormteammember} WHERE tid=%d", $tid);
    $teammember = db_fetch_object($r);
  }
  return stormteammember_form($teammember);
}

function stormteammember_edit_submit($form, &$form_state) {
  $teammember = new stdClass();
  $teammember->organization_nid = $form_state['values']['organization_nid'];
  $teammember->project_nid = $form_state['values']['project_nid'];
  $teammember->task_nid = $form_state['values']['task_nid'];
  $teammember->ticket_nid = $form_state['values']['ticket_nid'];
  $teammember->person_organization_nid = $form_state['values']['person_organization_nid'];
  $teammember->person_nid = $form_state['values']['person_nid'];
  $teammember->tid = $form_state['values']['tid'];

  _stormteammember_beforesave($teammember);

  db_query("UPDATE {stormteammember} SET
            organization_nid=%d, organization_title='%s',
            project_nid=%d, project_title='%s',
            task_nid=%d, task_title='%s', task_stepno='%s',
            ticket_nid=%d, ticket_title='%s', 
            person_organization_nid=%d, person_organization_title='%s', 
            person_nid=%d, person_title='%s' 
            WHERE tid = %d",
            $teammember->organization_nid, $teammember->organization_title,
            $teammember->project_nid, $teammember->project_title,
            $teammember->task_nid, $teammember->task_title, $teammember->task_stepno,
            $teammember->ticket_nid, $teammember->ticket_title, 
            $teammember->person_organization_nid, $teammember->person_organization_title, 
            $teammember->person_nid, $teammember->person_title, 
            $teammember->tid);
}

function _stormteammember_beforesave(&$teammember) {
  $s = "SELECT n.title FROM {node} n INNER JOIN {stormorganization} o ON n.nid=o.nid
        WHERE type='stormorganization' AND n.nid=%d";
  $r = db_query($s, $teammember->organization_nid);
  $o = db_fetch_object($r);
  $teammember->organization_title = $o->title;

  $s = "SELECT n.title, p.organization_title FROM {node} n INNER JOIN {stormproject} p ON n.nid=p.nid
        WHERE type='stormproject' AND n.nid=%d";
  $r = db_query($s, $teammember->project_nid);
  $p = db_fetch_object($r);
  $teammember->project_title = $p->title;

  $s = "SELECT title, stepno FROM {node} n INNER JOIN {stormtask} t ON n.nid=t.nid WHERE n.type='stormtask' AND n.nid=%d";
  $r = db_query($s, $teammember->task_nid);
  $ta = db_fetch_object($r);
  $teammember->task_title = $ta->title;
  $teammember->task_stepno = $ta->stepno;

  $s = "SELECT title FROM {node} n INNER JOIN {stormticket} t ON n.nid=t.nid WHERE n.type='stormticket' AND n.nid=%d";
  $r = db_query($s, $teammember->ticket_nid);
  $ti = db_fetch_object($r);
  $teammember->ticket_title = $ti->title;

  $s = "SELECT title FROM {node} n INNER JOIN {stormorganization} t ON n.nid=t.nid WHERE n.type='stormorganization' AND n.nid=%d";
  $r = db_query($s, $teammember->person_organization_nid);
  $ti = db_fetch_object($r);
  $teammember->person_organization_title = $ti->title;

  $s = "SELECT title FROM {node} n INNER JOIN {stormperson} t ON n.nid=t.nid WHERE n.type='stormperson' AND n.nid=%d";
  $r = db_query($s, $teammember->person_nid);
  $ti = db_fetch_object($r);
  $teammember->person_title = $ti->title;
}

function stormteammember_form(&$teammember) {
  if (arg(2)=='add') {
    if (array_key_exists('organization_nid', $_GET) && !$teammember->organization_nid) {
      $teammember->organization_nid = $_GET['organization_nid'];
    }
    if (array_key_exists('project_nid', $_GET) && !$teammember->project_nid) {
      $teammember->project_nid = $_GET['project_nid'];
      $p = node_load($teammember->project_nid);
      $teammember->organization_nid = $p->organization_nid;
    }
    if (array_key_exists('task_nid', $_GET) && !$teammember->task_nid) {
      $teammember->task_nid = $_GET['task_nid'];
      $t = node_load($teammember->task_nid);
      $teammember->organization_nid = $t->organization_nid;
      $teammember->project_nid = $t->project_nid;
    }
    if (array_key_exists('ticket_nid', $_GET) && !$teammember->ticket_nid) {
      $teammember->ticket_nid = $_GET['ticket_nid'];
      $t = node_load($teammember->ticket_nid);
      $teammember->organization_nid = $t->organization_nid;
      $teammember->project_nid = $t->project_nid;
      $teammember->task_nid = $t->task_nid;
    }

    if ($_SESSION['stormteammember_list_filter']['organization_nid'] && !$teammember->organization_nid) {
      $teammember->organization_nid = $_SESSION['stormteammember_list_filter']['organization_nid'];
    }
    if ($_SESSION['stormteammember_list_filter']['project_nid'] && !$teammember->project_nid) {
      $teammember->project_nid = $_SESSION['stormteammember_list_filter']['project_nid'];
    }
    if ($_SESSION['stormteammember_list_filter']['task_nid'] && !$teammember->task_nid) {
      $teammember->task_nid = $_SESSION['stormteammember_list_filter']['task_nid'];
    }
    if ($_SESSION['stormteammember_list_filter']['ticket_nid'] && !$teammember->ticket_nid) {
      $teammember->ticket_nid = $_SESSION['stormteammember_list_filter']['ticket_nid'];
    }
    if (array_key_exists('organization_nid', $_GET)) $teammember->organization_nid = $_GET['organization_nid'];
    if (array_key_exists('project_nid', $_GET)) $teammember->project_nid = $_GET['project_nid'];
    if (array_key_exists('task_nid', $_GET)) $teammember->task_nid = $_GET['task_nid'];
    if (array_key_exists('ticket_nid', $_GET)) $teammember->ticket_nid = $_GET['ticket_nid'];

  }
  $s_org = "SELECT n.nid, n.title FROM {stormorganization} so INNER JOIN {node} n 
              ON so.nid=n.nid WHERE n.status=1 AND (so.isactive=1 OR n.nid=%d) AND so.iscustomer=1 AND n.type='stormorganization' ORDER BY n.title";

  $form['group1'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -19,
  );

  $s_org = stormorganization_access_sql($s_org);
  $s_org = db_rewrite_sql($s_org);
  $r = db_query($s_org, $teammember->organization_nid);
  $organizations = array();
  while ($organization = db_fetch_object($r)) {
    $organizations[$organization->nid] = $organization->title;
    if (!$teammember->organization_nid) $teammember->organization_nid = $organization->nid;
    if (!$teammember->person_organization_nid) $teammember->person_organization_nid = $organization->nid;
  }
  $form['group1']['organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Organization', 'teammember'),
    '#default_value' => $teammember->organization_nid,
    '#options' => $organizations,
    '#required' => true,
    '#attributes' => array('onchange' => "stormticket_organization_project_task_tickets(this, 'edit-project-nid', 'edit-task-nid', 'edit-ticket-nid', true, '-')"),
  );

  $s = "SELECT n.nid, n.title FROM {stormproject} spr INNER JOIN {node} n ON spr.nid=n.nid WHERE spr.organization_nid=%d AND n.status=1 AND n.type='stormproject' ORDER BY n.title";
  $s = stormproject_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $teammember->organization_nid);
  $projects = array();
  while ($project = db_fetch_object($r)) {
    $projects[$project->nid] = $project->title;
  }
  $projects = array(0 => '-') + $projects;
  $form['group1']['project_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Project', 'teammember'),
    '#default_value' => $teammember->project_nid,
    '#options' => $projects,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormticket_project_task_tickets(this, 'edit-organization-nid', 'edit-task-nid', 'edit-ticket-nid', true, '-')"),
  );

  $tree = _stormtask_get_tree($teammember->project_nid);
  $tasks = _stormtask_plain_tree($tree);
  $tasks = array(0 => '-') + $tasks;
  $form['group1']['task_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Task', 'teammember'),
    '#default_value' => $teammember->task_nid,
    '#options' => $tasks,
    '#process' => array('storm_dependent_select_process'),
    '#attributes' => array('onchange' => "stormticket_task_tickets(this, 'edit-organization-nid', 'edit-project-nid', 'edit-ticket-nid', true, '-')"),
  );

  $tickets = array();
  $s = "SELECT n.nid, n.title FROM {stormticket} sti INNER JOIN {node} n ON sti.nid=n.nid WHERE n.status=1 AND n.type='stormticket' AND sti.organization_nid=%d AND sti.project_nid=%d AND sti.task_nid=%d ORDER BY n.title";
  $s = stormticket_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $teammember->organization_nid, $teammember->project_nid, $teammember->task_nid);
  while ($ticket = db_fetch_object($r)) {
    $tickets[$ticket->nid] = $ticket->title;
  }
  $form['group1']['ticket_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Ticket', 'teammember'),
    '#default_value' => $teammember->ticket_nid,
    '#options' => array(0 => '-') + $tickets,
    '#process' => array('storm_dependent_select_process'),
  );

  $form['group2'] = array(
    '#type' => 'markup',
    '#theme' => 'storm_form_group',
    '#weight' => -18,
  );

  $form['group2']['person_organization_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Person organization', 'teammember'),
    '#default_value' => $teammember->person_organization_nid,
    '#options' => $organizations,
    '#required' => true,
    '#attributes' => array('onchange' => "stormperson_organization_people(this, 'edit-person-nid', true, '-')"),
  );

  $s = "SELECT n.nid, n.title FROM {stormperson} spe INNER JOIN {node} n 
              ON spe.vid=n.vid WHERE n.status=1 AND spe.organization_nid=%d AND n.type='stormperson' ORDER BY n.title";
  $s = stormperson_access_sql($s);
  $s = db_rewrite_sql($s);
  $r = db_query($s, $teammember->person_organization_nid);
  $people = array();
  while ($person = db_fetch_object($r)) {
    $people[$person->nid] = $person->title;
  }
  $people = array(0 => '-') + $people;

  $form['group2']['person_nid'] = array(
    '#type' => 'select',
    '#title' => storm_t('Person', 'teammember'),
    '#default_value' => $teammember->person_nid,
    '#options' => $people,
    '#required' => true,
    '#process' => array('storm_dependent_select_process'),
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Save')
  );

  if ($teammember->tid) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => storm_t('Delete'),
      '#submit' => array('stormteammember_form_delete'),
    );
  }
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => storm_t('Cancel'),
    '#submit' => array('stormteammember_cancel_submit'),
  );

  if ($teammember->tid) {
    $form['tid'] = array(
      '#type' => 'value',
      '#value' => $teammember->tid,
    );
  }

  return $form;
}

