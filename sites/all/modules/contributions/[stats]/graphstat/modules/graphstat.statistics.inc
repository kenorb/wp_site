<?php
// $Id: graphstat.statistics.inc,v 1.1.2.1 2009/01/25 21:08:34 profix898 Exp $

/**
 * Implementation of hook_graphstat().
 */
function graphstat_statistics_graphstat() {
  global $user;
  
  // Daily statistics are only available if 'access_log' is enabled
  if (!variable_get('statistics_enable_access_log', 0))
    return array();

  if (variable_get('configurable_timezones', 1) && $user->uid && strlen($user->timezone)) {
    $timezone = $user->timezone;
  }
  else {
    $timezone = variable_get('date_default_timezone', 0);
  }
  // Get oldest record and current date
  $min = db_result(db_query('SELECT MIN(timestamp) FROM {accesslog}'));
  if ($min) {
    $now = time();
    // Figure out which local dates correspond with these timestamps - can be different between timezones
    $min_date = format_date($min, 'custom', 'm/d/Y');
    $now_date = format_date($now, 'custom', 'm/d/Y');
    // Figure out the (UNIX Epoch) timestamp that corresponds to when the first day began locally
    // Figure out the (UNIX Epoch) timestamp that corresponds to when today ends locally
    $min_date_frag = explode('/', $min_date);
    $now_date_frag = explode('/', $now_date);
    $gm_timestamp_min = gmmktime(0, 0, 0, $min_date_frag[0], $min_date_frag[1], $min_date_frag[2]);
    $gm_timestamp_now = gmmktime(23, 59, 59, $now_date_frag[0], $now_date_frag[1], $now_date_frag[2]);
    if ($timezone > 0) {
      $local_timestamp_min = $gm_timestamp_min - $timezone;
      $local_timestamp_now = $gm_timestamp_now - $timezone;
    }
    elseif ($timezone < 0) {
      $local_timestamp_min = $gm_timestamp_min + abs($timezone);
      $local_timestamp_now = $gm_timestamp_now + abs($timezone);
    }
    else {
      $local_timestamp_min = $gm_timestamp_min;
      $local_timestamp_now = $gm_timestamp_now;
    }
    // Build the filter options array with all dates
    $options = array('<'. t('Select a date') .'>');
    while ($local_timestamp_min < $local_timestamp_now) {
      $options[$local_timestamp_min] = format_date($local_timestamp_min, 'custom', 'm/d/Y');
      $local_timestamp_min = $local_timestamp_min + 86400;
    }
  }
  else {
    $options = array(NULL => t('No items available'));
  }
  
  $graphs = array();
  $graphs['daily'] = array(
    '#title' => t('Daily'),
    '#filter' => array(
      '#options' => $options,
      '#callback' => 'graphstat_statistics_daily_filter'
    ),
    'graph_page_generation' => array(
      '#title' => t('Average page generation time'),
      '#xlabel' => t('Time (hh:mm)'),
      '#ylabel' => t('Page generation / ms'),
      '#legend' => array(t('avg'), t('avg + stddev'), t('avg - stddev')),
      '#description' => t('Average (avg) page generation time +/- standard deviation (stddev).')
    ),
    'graph_pages' => array(
      '#title' => t('Number of pages served (total)'),
      '#xlabel' => t('Time (hh:mm)'),
      '#ylabel' => t('# Pages')
    ),
    'graph_unique_pages' => array(
      '#title' => t('Number of unique pages served'),
      '#xlabel' => t('Time (hh:mm)'),
      '#ylabel' => t('# Pages')
    ),
    'graph_visitors' => array(
      '#title' => t('Number of unique visitors'),
      '#xlabel' => t('Time (hh:mm)'),
      '#ylabel' => t('# Visitors')
    )
  );

  return $graphs;
}

/**
 * Function graphstat_statistics_daily_filter().
 */
function graphstat_statistics_daily_filter(&$data, $start) {
  if (!$start) {
    return;
  }
  $stop = $start + 86400;
  // Average page generation time
  $loop = $start;
  while ($loop < $stop) {
    $stats = db_fetch_object(db_query('SELECT AVG(timer) AS avg, STDDEV(timer) AS dev FROM {accesslog} WHERE timestamp > %d AND timestamp <= %d', $loop, $loop + 3600));
    $avg = $stats->avg ? round($stats->avg , 0) : 0;
    $dev = $stats->dev ? round($stats->dev, 0) : 0;
    $data['graph_page_generation'][] = array(format_date($loop, 'custom', 'H:i'), $avg, $avg + $dev, max($avg - $dev, 0));
    $loop += 3600;
  }
  // Number of pages served (total)
  $loop = $start;
  while ($loop < $stop) {
    $stats = db_fetch_object(db_query('SELECT COUNT(aid) AS count FROM {accesslog} WHERE timestamp > %d AND timestamp <= %d', $loop, $loop + 3600));
    $data['graph_pages'][format_date($loop, 'custom', 'H:i')] = $stats->count ? $stats->count : 0;
    $loop += 3600;
  }
  // Number of unique pages served
  $loop = $start;
  while ($loop < $stop) {
    // TODO: optimize query (using an additional table to store statistics)
    $stats = db_fetch_object(db_query('SELECT COUNT(DISTINCT(path)) AS count FROM {accesslog} WHERE timestamp > %d AND timestamp <= %d', $loop, $loop + 3600));
    $data['graph_unique_pages'][format_date($loop, 'custom', 'H:i')] = $stats->count ? $stats->count : 0;
    $loop += 3600;
  }
  // Number of unique visitors
  $loop = $start;
  while ($loop < $stop) {
    $stats = db_fetch_object(db_query('SELECT COUNT(DISTINCT(hostname)) AS count FROM {accesslog} WHERE timestamp > %d AND timestamp <= %d', $loop, $loop + 3600));
    $data['graph_visitors'][format_date($loop, 'custom', 'H:i')] = $stats->count ? $stats->count : 0;
    $loop += 3600;
  }
}
