<?php
// $Id: graphstat.node.inc,v 1.1.2.2 2009/01/25 22:13:45 profix898 Exp $

require_once(drupal_get_path('module', 'graphstat') .'/modules/graphstat.system.inc');

/**
 * Implementation of hook_graphstat().
 */
function graphstat_node_graphstat() {
  $graphs = array();
  
  $graphs['nodes'] = array(
    '#title' => t('Nodes'),
    'graph_node_types' => array(
      '#type' => 'pie',
      '#title' => t('Node Type Distribution'),
      '#description' => t('Number of nodes for each node type.')
    ),
    'graph_nodes_details' => array(
      '#type' => 'bars',
      '#title' => t('Number of Nodes (last 2 weeks)'),
      '#xlabel' => t('Date (MM/DD/YYYY)'),
      '#ylabel' => t('# Nodes'),
      '#legend' => array(t('total'), t('published'), t('queued')),
      '#description' => t('Number of total/published/queued nodes in the last two weeks.')
    )
  );

  return $graphs;
}

/**
 * Implementation of hook_graphstat_data().
 */
function graphstat_node_graphstat_data() {
  $data = array();
  
  // Node type distribution
  $node_types = array();
  $data['nodes']['graph_node_types'] = array(0 => array(''));
  $result = db_query("SELECT type, COUNT(nid) AS nodes FROM {node} WHERE nid <> 0 GROUP BY type ORDER BY nodes DESC", $_SERVER['HTTP_HOST']);
  while ($object = db_fetch_object($result)) {
    $data['nodes']['graph_node_types'][0][] = $object->nodes;
    $node_types[] = db_result(db_query("SELECT name FROM {node_type} WHERE type = '%s'", $object->type));
  }
  $data['nodes']['graph_node_types']['#legend'] = $node_types;
  
  // Node details (total, published, queued) of the last two weeks
  $start_date = time() - (60*60*24*14);
  $nodes_details_total = graphstat_statistics_tablehistory('node', 'nid', '', 14, $start_date);
  $nodes_details_published = graphstat_statistics_tablehistory('node', 'nid', ' AND status=1', 14, $start_date);
  $nodes_details_queued = graphstat_statistics_tablehistory('node', 'nid', ' AND moderate=1', 14, $start_date);
  $nodes_details_all = array();
  foreach (array_keys($nodes_details_total) as $date) {
    $data['nodes']['graph_nodes_details'][] = array(
      $date,
      $nodes_details_total[$date],
      $nodes_details_published[$date],
      $nodes_details_queued[$date]
    );
  }
  
  return $data;
}
