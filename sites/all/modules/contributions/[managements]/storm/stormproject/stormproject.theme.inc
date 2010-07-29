<?php
// $Id: stormproject.theme.inc,v 1.2.4.5 2009/01/06 15:16:23 robertogerola Exp $

/**
 * @file
 */

function theme_stormproject_list($header, $projects) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  foreach ($projects as $key => $project) {
    $n = new stdClass();
    $n->nid = $project->nid;
    $n->uid = $project->uid;
    $n->organization_nid = $project->organization_nid;
    $n->type = 'stormproject';

    $rows[] = array(
      storm_icon('projectcategory_'. $project->projectcategory, $project->projectcategory),
      l($project->organization_title, 'node/'. $project->organization_nid),
      l($project->title, 'node/'. $project->nid) . theme('mark', node_mark($project->nid, $project->changed)),
      storm_icon('status_'. $project->projectstatus, $project->projectstatus),
      storm_icon('priority_'. $project->projectpriority, $project->projectpriority),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormproject_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormproject'] = array(
    '#prefix' => '<div id="stormproject" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormproject']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormproject']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'project'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );
  $node->content['stormproject']['fields']['projectcategory'] = array(
    '#prefix' => '<div class="projectcategory">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Category', 'project'), stormattribute_value('Project category', $node->projectcategory)),
    '#weight' => $w++,
  );

  $node->content['stormproject']['fields']['projectstatus'] = array(
    '#prefix' => '<div class="projectstatus">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Status', 'project'), stormattribute_value('Project status', $node->projectstatus)),
    '#weight' => $w++,
  );

  $node->content['stormproject']['fields']['projectpriority'] = array(
    '#prefix' => '<div class="projectpriority">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Priority', 'project'), stormattribute_value('Project priority', $node->projectpriority)),
    '#weight' => $w++,
  );

  $node->content['stormproject']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'project'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormproject']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;

  if (module_exists('stormnote')) {
    $node->content['stormproject']['links']['notes']  = array(
      '#prefix' => '<div class="notes">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Notes', 'project'), 'storm/notes/', array('query' => array('project_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormticket')) {
    $node->content['stormproject']['links']['tickets'] = array(
      '#prefix' => '<div class="tickets">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Tickets', 'project'), 'storm/tickets/', array('query' => array('project_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormtimetracking')) {
    $node->content['stormproject']['links']['timetrackings'] = array(
      '#prefix' => '<div class="timetrackings">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Timetrackings', 'project'), 'storm/timetrackings/', array(), 'project_nid='. $node->nid),
      '#weight' => $w++,
    );
  }
  return $node;
}
