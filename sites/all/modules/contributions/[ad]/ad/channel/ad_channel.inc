<?php

/**
 * @file
 * Ad Channel include file.
 *
 * Copyright (c) 2008-2009.
 *   Jeremy Andrews <jeremy@tag1consulting.com>.
 */

/**
 * Filter advertisements not in an appropriate channel, from cache.
 */
function ad_channel_cache_filter($ads) {
  _debug_echo("ad_channel_cache: adserve_cache_filter");

  $channels = adserve_cache('get_cache', 'channel');
  $valid = array();
  $nochannel = array();
  $nochannel_weight = array();
  $matched_channel = array();
  foreach ($ads as $aid) {
    _debug_echo("ad_channel_cache: checking aid($aid)");
    if (is_array($channels['ads']) && isset($channels['ads'][$aid]) &&
        is_array($channels['ads'][$aid])) {
      foreach ($channels['ads'][$aid] as $chid) {
        $channel = $channels['channels'][$chid];
        $display = $channel->display;
        $urls = unserialize($channel->urls);
        $frontpage = adserve_variable('site_frontpage') ? adserve_variable('site_frontpage') : 'node';
        $regexp = '/^('. preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/', '/(^|\|)\\\\<front\\\\>($|\|)/'), array('|', '.*', '\1'. preg_quote($frontpage, '/') .'\2'), preg_quote($urls, '/')) .')$/';
        $match = preg_match($regexp, adserve_variable('url'));
        _debug_echo("ad_channel_cache: checking aid($aid) against channel($chid) path(". adserve_variable('url') .") regexp($regexp) match($match)");
        if ($display == 0) { // display on all except listed urls
          if (empty($urls) || !$match) {
            _debug_echo("ad_channel_cache: aid($aid) is valid");
            $valid[] = $aid;
            if ($display == 1) {
              $nochannel_weight[$chid] = $channel->no_channel_weight;
            }
            $matched_channel[$aid] = $chid;
            _debug_echo("ad_channel_cache: channel($channel->chid) no_channel_weight(". $nochannel_weight[$chid] .')');
            break;
          }
        }
        else { // display only on listed urls
          if (!empty($urls) && $match) {
            _debug_echo("ad_channel_cache: aid($aid) is valid");
            $valid[] = $aid;
            if ($display == 1) {
              $nochannel_weight[$chid] = $channel->no_channel_weight;
            }
            $matched_channel[$aid] = $chid;
            break;
          }
        }
        _debug_echo("ad_channel_cache: aid($aid) is not valid");
      }
    }
    else {
      // no channel information for ad, it's valid
      $display = $channels['display'];
      _debug_echo("ad_channel_cache: aid($aid) has no channel info [$display]");
      switch ($display) {
        case 0:
          $nochannel[] = $aid;
          _debug_echo("ad_channel_cache: aid($aid) is valid if no valid ads found in current channel");
          break;
        case 1:
          $valid[] = $aid;
          _debug_echo("ad_channel_cache: aid($aid) is valid");
          break;
        case 2:
          _debug_echo("ad_channel_cache: aid($aid) is not valid");
          break;
      }
    }
  }

  // Apply weights, applicable for advertisements that are not assigned to any
  // channel.
  if (!empty($valid) && !empty($nochannel_weight)) {
    $weights = array();
    foreach ($valid as $aid) {
      if (isset($matched_channel[$aid])) {
        $chid = $matched_channel[$aid];
        if (isset($nochannel_weight[$chid])) {
          $weights[$aid] = $nochannel_weight[$chid];
          _debug_echo("ad_channel_cache: ad $aid in channel $chid with weight ". $nochannel_weight[$chid]);
        }
        else {
          // by default, ads are assigned a weight of 100%
          $weights[$aid] = 100;
          _debug_echo("ad_channel_cache: ad $aid in channel $chid with default weight of 100");
        }
      }
      else {
        // by default, ads are assigned a weight of 100%
        $weights[$aid] = 100;
        _debug_echo("ad_channel_cache: ad $aid in no channel with default weight of 100");
      }
    }
    if (!empty($weights)) {
      $gcd = ad_channel_probability_gcd($weights);
      $display = array();
      _debug_echo("ad_channel_cache: adjusting channel weights, gcd ($gcd)");
      foreach ($valid as $aid) {
        $weight = $weights[$aid] / $gcd;
        for ($i = 1; $i <= $weight; $i++) {
          $display[] = $aid;
        }
      }
      $valid = $display;
    }
  }
  else if (empty($valid) && !empty($nochannel)) {
    _debug_echo("ad_channel_cache: using ads with no channel info");
    $valid = $nochannel;
  }

  $premiere = adserve_cache('get_cache', 'premiere');
  $premieres = array();
  if (is_array($premiere)) {
    foreach ($valid as $aid) {
      if (in_array($aid, $premiere)) {
        _debug_echo("ad_channel_cache: aid($aid) is premiere advertisement");
        $premieres[$aid] = $aid;
      }
      else {
        _debug_echo("ad_channel_cache: aid($aid) is not a premiere advertisement");
      }
    }
    if (!empty($premieres)) {
      _debug_echo("ad_channel_cache: returning premiere advertisements");
      return $premieres;
    }
  }
  _debug_echo("ad_channel_cache: returning non-premiere advertisements");
  return $valid;
}

/**
 * Returns the greatest common divisor of an array of integers.
 */
function ad_channel_probability_gcd($integers) {
  $gcd = array_shift($integers);

  while (!empty($integers)) {
    $gcd = _ad_channel_probability_gcd($gcd, array_shift($integers));
  }
  return $gcd;
}

/**
 * Helper function to calculate the greatest common divisor using the Euclidean
 * algorithm (http://en.wikipedia.org/wiki/Euclidean_algorithm).
 */
function _ad_channel_probability_gcd($a, $b) {
  if ($b == 0) {
    return $a;
  }
  else {
    return _ad_channel_probability_gcd($b, $a % $b);
  }
}
