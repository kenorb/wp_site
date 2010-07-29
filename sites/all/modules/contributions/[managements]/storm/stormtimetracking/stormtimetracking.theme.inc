<?php
// $Id: stormtimetracking.theme.inc,v 1.5.4.3 2009/01/06 15:16:23 robertogerola Exp $

/**
 * @file
 */

function theme_stormtimetracking_list($header, $timetrackings, $duration) {
  $rows = array();
  foreach ($timetrackings as $timetracking) {
    $n = new stdClass();
    $n->nid = $timetracking->nid;
    $n->uid = $timetracking->uid;
    $n->organization_nid = $timetracking->organization_nid;
    $n->project_nid = $timetracking->project_nid;
    $n->task_nid = $timetracking->task_nid;
    $n->ticket_nid = $timetracking->ticket_nid;
    $n->type = 'stormtimetracking';
    $rows[] = array(
      l($timetracking->organization_title, 'node/'. $timetracking->organization_nid),
      l($timetracking->project_title, 'node/'. $timetracking->project_nid),
      l($timetracking->title, 'node/'. $timetracking->nid),
      format_date($timetracking->trackingdate, 'small'),
      array('data' => sprintf('%.2f', $timetracking->duration), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  $o .= '<span style="font-weight: bold;">'. t('Total duration') .' : '. sprintf('%.2f', $duration) .'</span> (hours)';
  return $o;
}

function theme_stormtimetracking_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormtimetracking'] = array(
    '#prefix' => '<div id="stormtimetracking" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormtimetracking']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormtimetracking']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'timetracking'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormtimetracking']['fields']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'timetracking'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormtimetracking']['fields']['task'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Task', 'timetracking'), l($node->task_title, 'node/'. $node->task_nid)),
    '#weight' => $w++,
  );

  $node->content['stormtimetracking']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'timetracking'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormtimetracking']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
    '#weight' => $w++,
  );

  $w = 0;

  return $node;
}

function theme_stormtimetracking_list_form_report_reports() {
  $reports = array(
      'std' => t('Standard'),
      'for_organization' => t('For an organization'),
      'for_organization_w_task' => t('For an org. with tasks'),
      'for_project' => t('For a project'),
  );

  return $reports;
}

function theme_stormtimetracking_list_report($report, $language, $timetrackings) {
  switch ($report) {
    case 'std':
      $header = array(
        t('Organization', array(), $language),
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          $timetracking->organization_title,
          $timetracking->project_title,
          $timetracking->title,
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->duration), 'align' => 'right'),
        );
        $total_duration += $timetracking->duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
    break;
    case 'for_organization':
      $organization = node_load($_SESSION['stormtimetracking_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          $timetracking->project_title,
          $timetracking->title,
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->duration), 'align' => 'right'),
        );
        $total_duration += $timetracking->duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_organization_w_task':
      $organization = node_load($_SESSION['stormtimetracking_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Task', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          $timetracking->project_title,
          $timetracking->task_title,
          $timetracking->title,
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->duration), 'align' => 'right'),
        );
        $total_duration += $timetracking->duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_project':
      $organization = node_load($_SESSION['stormtimetracking_list_filter']['organization_nid']);
      $project = node_load($_SESSION['stormtimetracking_list_filter']['project_nid']);

      $header = array(
        t('Task', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($timetrackings as $timetracking) {
        $rows[] = array(
          $timetracking->task_title,
          $timetracking->title,
          format_date($timetracking->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $timetracking->duration), 'align' => 'right'),
        );
        $total_duration += $timetracking->duration;
      }
      $title  = '<h2>'. t('Timetrackings report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
      $title .= t('Project : @project', array('@project' => $project->title), $language) .'<br />';
    break;
  }
  $footer = '<h3>'. t('Total duration : %total_duration', array('%total_duration' => format_plural($total_duration, '1 hour', '@count hours', array(), $language)), $language) .'</h3>';
  return theme('storm_list_report', $header, $rows, $title, $footer);
}
