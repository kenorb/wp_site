<?php
// $Id: ad_cache_memcache.inc,v 1.1.2.6.2.6 2009/02/16 23:12:28 jeremy Exp $

/**
 * @file
 * Memcache include.
 *
 * Copyright (c) 2005-2009.
 *   Jeremy Andrews <jeremy@tag1consulting.com>.
 */

/**
 * TODO: Debug Raw and IFrame display methods, neither currently seem to work
 *       with this cache type.
 */

/**
 * Called by adserve.inc, display an ad from memcache.
 */
function ad_cache_memcache() {
  _debug_echo('Memcache: entering ad_cache_memcache().');

  // TODO: Move the meat of this function into adserve.php, simplifying what
  // cache plugins have to do and removing duplicated logic.
  $init_cache = array();
  $init_func = ad_cache_memcache_hook($init_cache, 'include_file_init', 'include_func_init');

  $hostid = adserve_variable('hostid') ? adserve_variable('hostid') : 'none';
  if ($hostid == 'none' || ad_memcache_get("ad-hostid-$hostid")) {
    if (function_exists($init_func)) {
      $init = $init_func($init_cache, $hostid);
    }
    if (!empty($init)) {
      if (adserve_variable('debug')) {
        echo "Memcache: initialized externally:<pre>\n";
        print_r($init);
        echo '</pre>';
      }
      $type = $init['type'];
      $id = $init['id'];
      $group = $init['group'];
      $aids = explode(',', $id);
      adserve_variable('quantity', $init['quantity']);
    }
    else {
      if ($id = adserve_variable('nids')) {
        $type = 'node';
      }
      else if ($id = adserve_variable('tids')) {
        $type = 'taxonomy';
      }
      else {
        $type = 'default';
        $id = 0;
      }
      $aids = ad_cache_memcache_get_ids($type, $id);
      $group = $id;
    }
    adserve_variable('group', $group);

    if (adserve_variable('debug')) {
      echo 'Memcache: selecting from the following ad id(s): ';
      if (empty($aids)) {
        echo 'none.<br />';
      }
      else {
        echo implode(', ', $aids) .'.<br />';
      }
    }

    $ids = adserve_variable("$type-ids");
    if ($ids == NULL) {
      $ids = array();
    }

    $output = '';
    $selected = adserve_select_ad($aids, adserve_variable('quantity'), $ids);
    adserve_variable("$type-ids", array_merge($selected, $ids));
    foreach ($selected as $aid) {
      if ($aid = (int)$aid) {
        $ad = ad_cache_memcache_get_ad($aid);

        if (!empty($output)) {
          $display_count++;
          $output .= "<div class=\"space\" id=\"$id-$displayed_count\"></div>";
        }

        $output .= $ad->display;
      }
      else {
        $ad = array();
      }

      _debug_echo("Displaying AID: $aid.");
      $action = $aid ? 'view' : 'count';
      ad_cache_memcache_increment($action, $aid, $group, $hostid, $ad);
    }
    if (empty($output)) {
      adserve_variable('error', TRUE);
      $output = 'No active ads were found in the '. (empty($nids) ? 'tids' : 'nids') ." '$id'.";
      _debug_echo("Memcache: {$output}");
    }
  }
  else {
    _debug_echo("Memcache: invalid hostid: '$hostid'.");
    $output = 'You do not have permission to display ads.';
  }

  return $output;
}

function ad_cache_memcache_hook(&$cache, $hook, $func) {

  if (empty($cache)) {
    _debug_echo('Memcache: retrieving hook info from cache.');
    $cache = ad_memcache_get('ad-cache-hook');
  }
  $include_func = NULL;
  if (is_array($cache) && !empty($cache)) {
    $include_file = adserve_variable('root_dir') .'/'. $cache[$hook];
    if (file_exists($include_file) && is_file($include_file)) {
      _debug_echo("Memcache: including external file: '$include_file'.");
      include_once($include_file);
    }
    else if (is_file($include_file)) {
      _debug_echo("Memcache: unable to find external file: '$include_file'.");
    }
    else {
      _debug_echo('Memcache: no include file defined in cache.');
    }
    $include_func = $cache[$func];
    if ($include_func) {
      _debug_echo("Memcache: returning requested func($func): '$include_func'.");
    }
  }
  return ($include_func);
}

function ad_cache_memcache_get_ids($op = 'default', $id = 0) {
  switch ($op) {

    case 'node': {
      $ids = explode(',', $id);
      break;
    }

    case 'taxonomy': {
      $ids = ad_memcache_get("ad-taxonomy-cache-$id");
      if (!$ids || empty($ids)) {
        $taxonomy = ad_memcache_get('ad-taxonomy');
        $cache = array();
        $ids = explode(',', $id);
        foreach ($ids as $tid) {
          if (is_array($taxonomy[$tid])) {
            $cache += $taxonomy[$tid];
          }
        }
        // Rebuild keys from 0, cache for quick re-use on next ad display.
        $ids = array_values($cache);
        ad_memcache_set("ad-taxonomy-cache-$id", $ids);
      }
      break;
    }

    default: {
      $taxonomy = ad_memcache_get('ad-taxonomy');
      $ids = $taxonomy[0];
      break;
    }

  }

  return $ids;
}

function ad_cache_memcache_get_ad($aid) {
  static $load = FALSE;

  $ad = ad_memcache_get("ad-aid-$aid");

  if (!$load && !is_object($ad)) {
    $load = TRUE;
    adserve_bootstrap();
    $ad_memcache_build = variable_get('ad_memcache_build', '');
    if ((time() - $ad_memcache_build) >= 60) {
      ad_cache_memcache_build();
    }
  }

  return $ad;
}

/**
 * Increment impressions counter in memcache.
 */
function ad_cache_memcache_increment($action, $aid, $group = '', $hostid = '', $ad = array()) {
  static $timestamp = NULL;

  _debug_echo("Memcache: increment action($action) aid($aid) group($group) hostid($hostid).");

  if ($aid && !is_object($ad)) {
    _debug_echo("Invalid ad id: $aid.");
    return (0);
  }

  if (!isset($timestamp)) {
    $timestamp = date('YmdH');
  }
  $counters = ad_memcache_get("ad-counters-$aid");

  $update = TRUE;
  if (!is_array($counters) || !isset($counters["$action:$group:$hostid:$timestamp"])) {
    _debug_echo("Memcache: adding map: action($action) aid($aid) group($group) hostid($hostid) timestamp($timestamp)");
    ad_memcache_increment_map($action, $aid, $group, $hostid, $timestamp);
  }

  $rc = ad_memcache_increment("ad-$action-$aid-$group-$hostid-$timestamp");
  _debug_echo("Memcache: incrementing ad-$action-$aid-$group-$hostid-$timestamp ($rc)");
}

/**
 * The maximum time any process can hold a given lock, in seconds.
 */
define('AD_MEMCACHE_LOCK_LIMIT', 2);

/**
 * Store a value in memcache.
 */
function ad_memcache_set($key, $value, $timeout = 86400) {
  $memcache = ad_memcache_init();

  return $memcache->set($key, $value, MEMCACHE_COMPRESSED, $timeout);
}

/**
 * Store a value in memcache.
 */
function ad_memcache_add($key, $value, $timeout = 86400) {
  $memcache = ad_memcache_init();

  return $memcache->add($key, $value, MEMCACHE_COMPRESSED, $timeout);
}

/**
 * Get a value from memcache.
 */
function ad_memcache_get($key) {
  $memcache = ad_memcache_init();

  return $memcache->get($key);
}

/**
 * Delete a value from memcache.
 */
function ad_memcache_delete($key) {
  $memcache = ad_memcache_init();

  return $memcache->delete($key);
}

/**
 * Get a lock in memcache.
 */
function ad_memcache_lock($key, $wait = TRUE) {
  $loop = 0;
  $lock = FALSE;
  while ($lock == FALSE) {
    $lock = ad_memcache_add("LOCK-$key-LOCK", TRUE, AD_MEMCACHE_LOCK_LIMIT);
    if (!$lock && $wait) {
      if ($loop++ > 50) {
        // Hard limit of 5 seconds, after which we fail to grab a lock.
        return FALSE;
      }
      // Wait 1/10th of a second and try again.
      usleep(100000);
    }
    else if (!$lock && !$wait) {
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Release a lock in memcache.
 */
function ad_memcache_unlock($key) {
  ad_memcache_delete("LOCK-$key-LOCK");
}

/**
 * Increment a numerical value in memcache.
 */
function ad_memcache_increment($key, $value = 1) {
  $memcache = ad_memcache_init();

  $rc = $memcache->increment($key, $value);
  if ($rc === FALSE) {
    // We tried incrementing a counter that hasn't yet been initialized.
    $rc = $memcache->set($key, $value);
    if ($rc === FALSE) {
      // Another process already initialized the counter, increment it.
      $rc = $memcache->increment($key);
    }
  }
  return $rc;
}

/**
 * Decrement a numerical value in memcache.
 */
function ad_memcache_decrement($key, $value = 1) {
  $memcache = ad_memcache_init();

  $rc = $memcache->decrement($key, $value);
  if ($rc === FALSE) {
    // We tried incrementing a counter that hasn't yet been initialized.
    $rc = $memcache->set($key, $value);
    if ($rc === FALSE) {
      // Another process already initialized the counter, increment it.
      $rc = $memcache->decrement($key);
    }
  }
  return $rc;
}

/**
 * Update mapping which allows us to quickly find stats in memcache when
 * feeding them into the database.
 */
function ad_memcache_increment_map($action, $aid, $group, $hostid, $timestamp) {
  $key = "ad-counters-$aid";
  if (ad_memcache_lock($key)) {
    $counters = ad_memcache_get($key);
    if (!is_array($counters) || !isset($counters["$action:$group:$hostid:$timestamp"])) {
      $counters["$action:$group:$hostid:$timestamp"] = "$action:$group:$hostid:$timestamp";
      ad_memcache_set($key, $counters);
    }
    ad_memcache_unlock($key);
  }
}

/**
 * Decrement a numerical value in memcache.
 * TODO: Use the same configuration style as Drupal's memcache module,
 * supporting multiple memcache servers, etc.
 */
function ad_memcache_init() {
  static $memcache = NULL;

  if (!$memcache) {
    $memcache = new Memcache;
    $memcache->addServer('localhost', 11211);
  }
  return $memcache;
}

/**
 * Allow external ad selection logic.
 */
function ad_cache_memcache_adserve_select($ads, $invalid) {
  $cache = array();
  if ($select_func = ad_cache_memcache_hook($cache, 'include_file_select', 'include_func_select')) {
    _debug_echo("Memcache: adserve_select: invoking '$select_func()'");
    if (function_exists($select_func)) {
      if (is_array($cache) && !empty($cache)) {
        return $select_func($ads, $invalid, $cache);
      }
      else {
        _debug_echo("Memcache: unexpected error: cache empty.");
      }
    }
    else {
      _debug_echo("Memcache: adserve_select: '$include_func_select()' not found");
    }
  }
  else {
    _debug_echo("Memcache: adserve_select: no select function defined");
  }
}

/**
 * Allow external exit text.
 */
function ad_cache_memcache_adserve_exit_text() {
  $cache = array();
  if ($exit_text_func = ad_cache_memcache_hook($cache, 'include_file_exit_text', 'include_func_exit_text')) {
    _debug_echo("Memcache: adserve_exit_text: invoking '$exit_text_func()'");
    if (function_exists($exit_text_func)) {
      return $exit_text_func();
    }
    else {
      _debug_echo("Memcache: adserve_exit_text: '$exit_text_func()' not found");
    }
  }
  else {
    _debug_echo("Memcache: adserve_exit_text: no exit_text function defined");
  }
}

