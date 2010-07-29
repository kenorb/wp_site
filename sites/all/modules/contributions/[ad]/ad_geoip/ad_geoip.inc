<?php

/**
 * If called from a cache, Drupal wasn't bootstrapped and the following aren't
 * defined, so we define them.
 */
if (!defined('AD_GEOIP_ALL')) {
  define('AD_GEOIP_ALL', 0);
  define('AD_GEOIP_SELECTED', 1);
  define('AD_GEOIP_NOT_SELECTED', 2);
}

/**
 * Select a geoip appropriate advertisement to display, from database.
 */
function ad_geoip_adserve($ads, $quantity) {
  _debug_echo('ad_geoip_cache: adserve_select.');

  $valid = array();
  foreach ($ads as $aid) {
    $display = TRUE;
    $targets = array('country', 'region', 'city');

    foreach ($targets as $target) {
      // stop processing if we already know we shouldn't display this ad
      if ($display !== TRUE) break;
      if ($target == 'city') {
        $result = db_query("SELECT city as code, format FROM {ad_geoip_ads_$target} WHERE aid = %d", $aid);
      }
      else {
        $result = db_query("SELECT code, format FROM {ad_geoip_ads_$target} WHERE aid = %d", $aid);
      }
      _debug_echo("ad_geoip_cache: aid($aid) target($target)");
      while ($geoip = db_fetch_object($result)) {
        $display = ad_geoip_display($target, $geoip->format, $geoip->code);
        if ($geoip->format == AD_GEOIP_ALL) {
          _debug_echo("ad_geoip_cache: $target: display aid($aid)");
          break;
        }
        else if ($display === FALSE) {
          _debug_echo("ad_geoip_cache: do not display aid($aid)");
          break;
        }
        // break out if we already know we should display this ad
        else if ($geoip->format == AD_GEOIP_SELECTED && $display === TRUE) {
          _debug_echo("ad_geoip_cache: $target: display aid($aid)");
          break;
        }
      }
    }
    if ($display === TRUE) {
      _debug_echo("ad_geoip_cache: aid($aid) is valid");
      $valid[] = $aid;
    }
    else {
      _debug_echo("ad_geoip_cache: aid($aid) is not valid");
    }
  }

  $total = sizeof($valid);
  _debug_echo("ad_geoip_cache: selecting from $total ads: ". implode(', ', $valid));
  if ($total) {
    $id = $total > 1 ? $valid[mt_rand(0, $total - 1)] : $valid[0];
    _debug_echo("ad_geoip: randomly selected ID: $id.");
    return $id;
  }
  else {
    return (-1);
  }
}

/**
 * Select a geoip appropriate advertisement to display, from cache.
 */
function ad_geoip_cache_select($ads, $invalid, $cache) {
  _debug_echo("ad_geoip_cache: adserve_cache_select");

  $valid = array();
  foreach ($ads as $aid) {
    if (in_array($aid, $invalid)) {
      _debug_echo("ad_geoip_cache: aid($aid) is invalid.");
    }

    $targets = array('country', 'region', 'city');

    $display = TRUE;
    foreach ($targets as $target) {
      // stop processing if we already know we shouldn't display this ad
      if ($display !== TRUE) break;
      $format = $cache['geoip'][$target]['format'][$aid] ? $cache['geoip'][$target]['format'][$aid] : AD_GEOIP_ALL;
      _debug_echo("ad_geoip_cache: aid($aid) $target format($format)");

      foreach ($cache['geoip'][$aid][$target] as $code) {
        $display = ad_geoip_display($target, $format, $code);
        // break out if we already know we shouldn't display this ad
        if ($format == AD_GEOIP_ALL) {
          _debug_echo("ad_geoip_cache: $target: display aid($aid)");
          break;
        }
        else if ($display === FALSE) {
          _debug_echo("ad_geoip_cache: do not display aid($aid)");
          break;
        }
        // break out if we already know we should display this ad
        else if ($format == AD_GEOIP_SELECTED && $display === TRUE) {
          _debug_echo("ad_geoip_cache: $target: display aid($aid)");
          break;
        }
      }
    }

    if ($display === TRUE) {
      _debug_echo("ad_geoip_cache: aid($aid) is valid");
      $valid[] = $aid;
    }
    else {
      _debug_echo("ad_geoip_cache: aid($aid) is not valid");
    }
  }

  $total = sizeof($valid);
  _debug_echo("ad_geoip_cache: selecting from $total ads: ". implode(', ', $valid));
  if ($total) {
    $id = $total > 1 ? $valid[mt_rand(0, $total - 1)] : $valid[0];
    _debug_echo("ad_geoip_cache: randomly selected ID: $id.");
    return array($id);
  }
  else {
    // no more ads available
    return array(-1);
  }
}

/**
 * Determine if the advertisement should be displayed.
 */
function ad_geoip_display($target, $format, $code) {
  switch ($format) {
    // display to all
    case AD_GEOIP_ALL: {
      _debug_echo("ad_geoip_cache: $format: all valid.");
      $display = TRUE;
      break;
    }

    // display to selected
    case AD_GEOIP_SELECTED: {
      $lookup = ad_geoip_lookup($target);
      if (strcasecmp($code, $lookup) == 0) {
        _debug_echo("ad_geoip_cache: $format:  current visitor from $code.");
        $display = TRUE;
        break;
      }
      else if ($lookup == 'UNKNOWN') {
        _debug_echo("ad_geoip_cache: $format: current visitor from UNKOWN locale.");
        $display = FALSE;
      }
      else {
        if ($target == 'city' && strpos($code, '*') !== FALSE) {
          // city with wildcard
          if (ad_geoip_search($code, $lookup)) {
            _debug_echo("ad_geoip_cache: $format:  current visitor from $lookup, matches $code.");
            $display = TRUE;
            break;
          }
        }
        _debug_echo("ad_geoip_cache: $format:  current visitor from $lookup not from $code.");
        $display = NULL;
      }
      break;
    }

    // display to not selected
    case AD_GEOIP_NOT_SELECTED: {
      $lookup = ad_geoip_lookup($target);
      if (strcasecmp($code, $lookup) == 0) {
        _debug_echo("ad_geoip_cache: current visitor from $code.");
        $display = FALSE;
      }
      else if ($lookup == 'UNKNOWN') {
        _debug_echo("ad_geoip_cache: $format: current visitor from UNKOWN locale.");
        $display = FALSE;
      }
      else {
        if ($target == 'city' && strpos($code, '*') !== FALSE) {
          // city with wildcard
          if (ad_geoip_search($code, $lookup)) {
            _debug_echo("ad_geoip_cache: $format:  current visitor from $lookup, matches $code.");
            $display = FALSE;
            break;
          }
        }
        _debug_echo("ad_geoip_cache: $format: current visitor from $lookup not from $code.");
        $display = TRUE;
      }
      break;
    }
  }

  return $display;
}

/**
 * Convert $needle to regex, replacing "*" with "(.)*", then perform search.
 */
function ad_geoip_search($needle, $haystack) {
  if (!strlen(trim($haystack))) return;
  $array = explode('*', $needle);
  $size = sizeof($array);
  $current = 0;
  foreach ($array as $string) {
    if (strlen(trim($string))) {
      if (++$current < $size) {
        $search .= $string ."(.)*";
      }
      else {
        $search .= $string;
      }
    }
    else {
      $current++;
    }
  }
  _debug_echo("ad_geoip_cache: searching /$search/i.");
  if (preg_match("/$search/i", $haystack)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Perform the appropriate GeoIP database lookup.
 */
function ad_geoip_lookup($target) {
  switch ($target) {
    case 'country':
      $lookup = ad_geoip_country_code();
      break;
    case 'region':
      $lookup = ad_geoip_region_code();
      break;
    case 'city':
      $lookup = ad_geoip_city_code();
      break;
  }
  return $lookup;
}

/**
 * Retrieve country code from IP address, cache for re-use.
 */
function ad_geoip_country_code() {
  static $code = NULL;

  if (is_null($code)) {
    $hostname = ip_address();
    $code = geoip_country_code_by_name($hostname);
    if (!$code) {
      _debug_echo("ad_geoip: no country code found for host from $hostname.");
      // do not translate, as we may call this without bootstrapping Drupal
      $code = 'UNKNOWN';
    }
  }
  return $code;
}

/**
 * Retrieve region code from IP address, cache for re-use.
 */
function ad_geoip_region_code() {
  static $code = NULL;

  if (is_null($code)) {
    $hostname = ip_address();
    $result = geoip_record_by_name($hostname);
    if (!is_array($result) || empty($result)) {
      _debug_echo("ad_geoip: no region code found for host from $hostname.");
      // do not translate, as we may call this without bootstrapping Drupal
      $code = 'UNKNOWN';
    }
    else {
      $code = $result['region'];
    }
  }
  return $code;
}

/**
 * Retrieve city code from IP address, cache for re-use.
 */
function ad_geoip_city_code() {
  static $code = NULL;

  if (is_null($code)) {
    $hostname = ip_address();
    $result = geoip_record_by_name($hostname);
    if (!is_array($result) || empty($result)) {
      _debug_echo("ad_geoip: no city found for host from $hostname.");
      // do not translate, as we may call this without bootstrapping Drupal
      $code = 'UNKNOWN';
    }
    else {
      $code = $result['city'];
    }
  }
  return $code;
}
