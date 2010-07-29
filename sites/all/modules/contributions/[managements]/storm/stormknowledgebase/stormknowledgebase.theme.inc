<?php
// $Id: stormknowledgebase.theme.inc,v 1.1.4.2 2009/01/06 15:16:22 robertogerola Exp $

/**
 * @file
 */

function theme_stormknowledgebase_list($header, $kbs) {
  $rows = array();
  $topics = stormattribute_attributes_bydomain('Knowledge base topic');

  foreach ($kbs as $kb) {
    $n = new stdClass();
    $n->nid = $note->nid;
    $n->uid = $note->uid;
    $n->type = 'stormknowledgebase';
    $rows[] = array(
      l($kb->title, 'node/'. $kb->nid),
      $topics[$kb->topic],
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);
  return $o;
}

function theme_stormknowledgebase_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);
  $topics = stormattribute_attributes_bydomain('Knowledge base topic');

  $w = 0;
  $node->content['stormknowledgebase'] = array(
    '#prefix' => '<div id="stormknowledgebase" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormknowledgebase']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormknowledgebase']['fields']['topic'] = array(
    '#prefix' => '<div class="topic">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Topic', 'knowledgebase'), $topics[$node->topic]),
    '#weight' => $w++,
  );

  $node->content['stormknowledgebase']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'expense'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormknowledgebase']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
    '#weight' => $w++,
  );

  return $node;
}

