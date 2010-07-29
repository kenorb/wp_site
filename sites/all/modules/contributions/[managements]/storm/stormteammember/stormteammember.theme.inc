<?php
// $Id: stormteammember.theme.inc,v 1.1.2.1 2008/12/15 13:23:53 robertogerola Exp $

/**
 * @file
 */

function theme_stormteammember_list($header, $teammembers) {
  $i = new stdClass();
  $i->type = 'stormteammember';

  $rows = array();
  foreach ($teammembers as $teammember) {
    $rows[] = array(
      l($teammember->organization_title, 'node/'. $teammember->organization_nid),
      l($teammember->project_title, 'node/'. $teammember->project_nid),
      l($teammember->task_title, 'node/'. $teammember->task_nid),
      l($teammember->ticket_title, 'node/'. $teammember->ticket_nid),
      array(
        'data' => storm_icon_edit('storm/teammembers/edit/'. $teammember->tid, $i, $_GET) .'&nbsp;'.  storm_icon_delete('storm/teammembers/delete/'. $teammember->tid, $i, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormteammember_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormteammember'] = array(
    '#prefix' => '<div id="stormteammember">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormteammember']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'teammember'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormteammember']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'teammember'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormteammember']['task'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Task', 'teammember'), l($node->task_title, 'node/'. $node->task_nid)),
    '#weight' => $w++,
  );

  return $node;
}

function theme_stormteammember_list_form_report_reports() {
  $reports = array(
      'std' => t('Standard'),
      'for_organization' => t('For an organization'),
      'for_organization_w_task' => t('For an org. with tasks'),
      'for_project' => t('For a project'),
  );

  return $reports;
}

function theme_stormteammember_list_report($report, $language, $teammembers) {
  switch ($report) {
    case 'std':
      $header = array(
        t('Organization', array(), $language),
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($teammembers as $teammember) {
        $rows[] = array(
          $teammember->organization_title,
          $teammember->project_title,
          $teammember->title,
          format_date($teammember->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $teammember->duration), 'align' => 'right'),
        );
        $total_duration += $teammember->duration;
      }
      $title  = '<h2>'. t('teammembers report', array(), $language) .'</h2><br />';
    break;
    case 'for_organization':
      $organization = node_load($_SESSION['stormteammember_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($teammembers as $teammember) {
        $rows[] = array(
          $teammember->project_title,
          $teammember->title,
          format_date($teammember->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $teammember->duration), 'align' => 'right'),
        );
        $total_duration += $teammember->duration;
      }
      $title  = '<h2>'. t('teammembers report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_organization_w_task':
      $organization = node_load($_SESSION['stormteammember_list_filter']['organization_nid']);
      $header = array(
        t('Project', array(), $language),
        t('Task', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($teammembers as $teammember) {
        $rows[] = array(
          $teammember->project_title,
          $teammember->task_title,
          $teammember->title,
          format_date($teammember->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $teammember->duration), 'align' => 'right'),
        );
        $total_duration += $teammember->duration;
      }
      $title  = '<h2>'. t('teammembers report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_project':
      $organization = node_load($_SESSION['stormteammember_list_filter']['organization_nid']);
      $project = node_load($_SESSION['stormteammember_list_filter']['project_nid']);

      $header = array(
        t('Task', array(), $language),
        t('Title', array(), $language),
        t('Date', array(), $language),
        t('Duration (hours)', array(), $language));
      $total_duration = 0;
      foreach ($teammembers as $teammember) {
        $rows[] = array(
          $teammember->task_title,
          $teammember->title,
          format_date($teammember->trackingdate, 'small'),
          array('data' => sprintf('%.2f', $teammember->duration), 'align' => 'right'),
        );
        $total_duration += $teammember->duration;
      }
      $title  = '<h2>'. t('teammembers report', array(), $language) .'</h2><br />';
      $title .= t('Organization : @organization', array('@organization' => $organization->fullname), $language) .'<br />';
      $title .= t('Project : @project', array('@project' => $project->title), $language) .'<br />';
    break;
  }
  $footer = '<h3>'. t('Total duration : %total_duration', array('%total_duration' => format_plural($total_duration, '1 hour', '@count hours', array(), $language)), $language) .'</h3>';
  return theme('storm_list_report', $header, $rows, $title, $footer);
}
