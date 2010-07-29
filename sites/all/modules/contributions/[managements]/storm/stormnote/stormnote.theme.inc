<?php
// $Id: stormnote.theme.inc,v 1.1.4.2 2009/01/06 15:16:22 robertogerola Exp $

/**
 * @file
 */

function theme_stormnote_list($header, $notes) {
  $rows = array();

  foreach ($notes as $note) {
    $n = new stdClass();
    $n->nid = $note->nid;
    $n->uid = $note->uid;
    $n->organization_nid = $note->organization_nid;
    $n->project_nid = $note->project_nid;
    $n->task_nid = $note->task_nid;
    $n->type = 'stormnote';
    $rows[] = array(
      l($note->organization_title, 'node/'. $note->organization_nid),
      l($note->project_title, 'node/'. $note->project_nid),
      l($note->task_title, 'node/'. $note->task_nid),
      l($note->title, 'node/'. $note->nid),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormnote_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;

  $node->content['stormnote'] = array(
    '#prefix' => '<div id="stormnote" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormnote']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormnote']['fields']['title'] = array(
    '#prefix' => '<div class="title">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Title', 'note'), $node->title),
    '#weight' => $w++,
  );

  $node->content['stormnote']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'note'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormnote']['fields']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'note'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormnote']['fields']['task'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Task', 'note'), l($node->task_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormnote']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'note'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormnote']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
    '#weight' => $w++,
  );

  return $node;
}

