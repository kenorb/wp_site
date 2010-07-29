<?php
// $Id: graphstat.system.inc,v 1.1.2.1 2009/01/25 21:08:34 profix898 Exp $

/**
 * Implementation of hook_graphstat().
 */
function graphstat_system_graphstat() {
  $graphs = array();
  $graphs['history'] = array(
    '#title' => t('History'),
    'graph_nodes' => array(
      '#title' => t('Number of Nodes'),
      '#xlabel' => t('Date (MM/DD/YYYY)'),
      '#ylabel' => t('# Nodes')
    ),
    'graph_users' => array(
      '#title' => t('Number of Users'),
      '#xlabel' => t('Date (MM/DD/YYYY)'),
      '#ylabel' => t('# Users')
    )
  );
  
  if (module_exists('comment')) {
    $graphs['history']['graph_comments'] = array(
      '#title' => t('Number of Comments'),
      '#xlabel' => t('Date (MM/DD/YYYY)'),
      '#ylabel' => t('# Comments')
    );
  }
  
  return $graphs;
}

/**
 * Implementation of hook_graphstat_data().
 */
function graphstat_system_graphstat_data() {
  $data = array();
  $data['history']['graph_nodes'] = graphstat_statistics_tablehistory();
  $data['history']['graph_users'] = graphstat_statistics_tablehistory('users', 'uid');
  
  if (module_exists('comment')) {
    $data['history']['graph_comments'] = graphstat_statistics_tablehistory('comments', 'cid');
  }
  
  return $data;
}

/**
 * Function graphstat_statistics_tablehistory().
 */
function graphstat_statistics_tablehistory($table = 'node', $id = 'nid', $fragment = '', $points = 0, $timestamp = 0) {
  $data = array();
  
  $key = in_array($table, array('node', 'users')) ? 'created' : 'timestamp';
  $timestamp = $timestamp ? $timestamp : db_result(db_query('SELECT MIN(%s) FROM {%s} WHERE %s != 0', $key, $table, $id));
  if ($timestamp) {
    // One point per week (at least 15 points)
    if (!$points) {
      $points = max((time() - $timestamp) / (60*60*24*7), 15);
      while ($points > 30) {
        $points /= 2;
      }
    }
    $interval = round((time() - $timestamp) / ($points - 1), 0);
    $time = $timestamp;
    for ($i = 0; $i < $points; $i++) {
      $data[format_date($time, 'custom', 'm/d/Y')] = db_result(db_query('SELECT COUNT(DISTINCT(%s)) FROM {%s} WHERE %s <= %d AND %s <> 0 %s', $id, $table, $key, $time, $id, $fragment));
      $time += $interval;
    }
  }

  return $data;
}
