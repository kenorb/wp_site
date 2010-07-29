<?php
// $Id: graphstat.user.inc,v 1.1.2.2 2009/01/25 22:13:45 profix898 Exp $

/**
 * Implementation of hook_graphstat().
 */
function graphstat_user_graphstat() {
  $graphs = array();
  
  $graphs['users'] = array(
    '#title' => t('Users'),
    'graph_user_roles' => array(
      '#type' => 'bars',
      '#title' => t('Users per role')
    ),
    'graph_user_hits' => array(
      '#type' => 'pie',
      '#title' => t('Most active users (page hits)')
    ),
    'graph_user_nodes' => array(
      '#type' => 'pie',
      '#title' => t('Most active users (number of nodes)')
    )
  );

  // Most active users (number of comments)
  if (module_exists('comment')) {
    $graphs['users']['graph_user_comments'] = array(
      '#type' => 'pie',
      '#title' => t('Most active users (number of comments)')
    );
  }

  return $graphs;
}

/**
 * Implementation of hook_graphstat_data().
 */
function graphstat_user_graphstat_data() {
  $data = array();
  
  // Users per role
  $data['users']['graph_user_roles'] = array(0 => array(0 => ''));
  foreach (user_roles(TRUE) as $rid => $role_name) {
    if ($rid != DRUPAL_AUTHENTICATED_RID) {
      $data['users']['graph_user_roles'][0][] = db_result(db_query('SELECT COUNT(uid) FROM {users_roles} WHERE rid = %d', $rid));
      $data['users']['graph_user_roles']['#legend'][] = $role_name;
    }
  }
  
  // Most active users (page hits)
  if (variable_get('statistics_enable_access_log', 0)) {
    $top_users_hits = array('');
    $top_users_hits_legend = array();
    $result = db_query_range("SELECT uid, COUNT(DISTINCT(url)) AS hits FROM {accesslog} WHERE url <> '' GROUP BY uid ORDER BY hits DESC", $_SERVER['HTTP_HOST'], 0, 10);
    while ($object = db_fetch_object($result)) {
      $top_users_hits[] = $object->hits;
      $top_users_hits_legend[] = graphstat_statistics_uid2username($object->uid);
    }
    $total_hits = max(array_sum($top_users_hits), 1);
    $data['users']['graph_user_hits'] = array(0 => array());
    foreach ($top_users_hits as $pos => $hits) {
      $data['users']['graph_user_hits'][0][$pos] = round($hits / $total_hits * 100, 1);
    }
    $data['users']['graph_user_hits']['#legend'] = $top_users_hits_legend;
  }
  
  // Most active users (number of nodes)
  $top_users_nodes = array('');
  $top_users_nodes_legend = array();
  $result = db_query_range("SELECT uid, COUNT(nid) AS nodes FROM {node} WHERE nid <> 0 GROUP BY uid ORDER BY nodes DESC", $_SERVER['HTTP_HOST'], 0, 10);
  while ($object = db_fetch_object($result)) {
    $top_users_nodes[] = $object->nodes;
    $top_users_nodes_legend[] = graphstat_statistics_uid2username($object->uid);
  }
  $total_nodes = max(array_sum($top_users_nodes), 1);
  $data['users']['graph_user_nodes'] = array(0 => array());
  foreach ($top_users_nodes as $pos => $nodes) {
    $data['users']['graph_user_nodes'][0][$pos] = round($nodes / $total_nodes * 100, 1);
  }
  $data['users']['graph_user_nodes']['#legend'] = $top_users_nodes_legend;
  
  // Most active users (number of comments)
  if (module_exists('comment')) {
    $top_users_comments = array('');
    $top_users_comments_legend = array();
    $result = db_query_range("SELECT uid, COUNT(cid) AS comments FROM {comments} WHERE cid <> 0 GROUP BY uid ORDER BY comments DESC", $_SERVER['HTTP_HOST'], 0, 10);
    while ($object = db_fetch_object($result)) {
      $top_users_comments[] = $object->comments;
      $top_users_comments_legend[] = graphstat_statistics_uid2username($object->uid);
    }
    $total_comments = max(array_sum($top_users_comments), 1);
    $data['users']['graph_user_comments'] = array(0 => array());
    foreach ($top_users_comments as $pos => $comments) {
      $data['users']['graph_user_comments'][0][$pos] = round($comments / $total_comments * 100, 1);
    }
    $data['users']['graph_user_comments']['#legend'] = $top_users_nodes_legend;
    if (count($data['users']['graph_user_comments'][0][$pos]) < 2) {
      $data['users']['graph_user_comments'] = NULL;
    }
  }
  
  return $data;
}

/**
 * Function graphstat_statistics_uid2username().
 */
function graphstat_statistics_uid2username($uid) {
  if ($uid && $user = user_load(array('uid' => $uid))) {
    return $user->name;
  }
  
  return variable_get('anonymous', t('anonymous'));
}
