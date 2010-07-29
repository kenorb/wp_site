<?php
// $Id: gallery_filter.inc,v 1.5.2.4 2008/02/18 23:07:15 profix898 Exp $

/**
 * gallery.module : gallery_filter.inc
 * Gallery Filter functions
 */

/**
 * Function gallery_filter_process().
 * (implementation of the main filter routine)
 */
function gallery_filter_process($text) {
  $prefix = trim(variable_get('gallery_filter_prefix', 'G2'));
  preg_match_all("/\[$prefix:(\d+)\s*(.*?)\]/i", $text, $matches, PREG_SET_ORDER);
  
  // Initialize G2 and set default arguments
  if (count($matches) > 0) {
    if (!_gallery_init(TRUE)) {
      return $text;
    }
    $default['n'] = variable_get('gallery_filter_n_images', 1);
    $default['type'] = variable_get('gallery_filter_default_block_type', 'recentImage');
    $default['maxsize'] = variable_get('gallery_filter_default_maxsize', GALLERY_FILTER_MAXSIZE_DEFAULT); 
    $default['exactsize'] = variable_get('gallery_filter_default_exactsize', GALLERY_FILTER_EXACTSIZE_DEFAULT);
    $default['class'] = variable_get('gallery_filter_default_div_class', 'nowrap');
    $default['album_frame'] = variable_get('gallery_filter_default_album_frame', 'none');
    $default['item_frame'] = variable_get('gallery_filter_default_item_frame', 'none');
    $default['show'] = variable_get('gallery_filter_default_show', array('none'));
    $default['target'] = variable_get('gallery_filter_default_link_target', '');
    $default['link'] = variable_get('gallery_filter_default_link', '');
  }
  else {
    return $text;
  }

  $head_array = array();
  // Loop over all matches
  foreach ($matches as $match) {
    // First argument is numeric => valid G2 filter tag
    if (is_numeric($match[1])) {
      $args = array_filter(preg_split('/[\s,]+/', $match[2]));
      $params = array('itemId' => intval($match[1]));
      // If this item is not an album (e.g. photo, movie, ...) set block type to 'specificItem'
      $details = gallery_item_details($params['itemId']);
      if (isset($details['g2type']) && $details['g2type'] != 'GalleryAlbumItem') {
        $params['n'] = 1;
        $params['type'] = 'specificItem';
      }
      if (preg_match('/user(:([\d]+))?/i', $params['itemId'], $param_uid)) {
        require_once(drupal_get_path('module', 'gallery') .'/gallery_user.inc');
        $params['itemId'] = gallery_user_useralbum(isset($param_uid[2]) ? $param_uid[2] : NULL, FALSE);
      }
      // Loop over all (optional) arguments
      foreach ($args as $arg) {
        list($key, $value) = array_filter(explode('=', $arg));
        if (!empty($value)) {
          $key = strtolower(preg_replace('/\W/', '', $key));
          $params[$key] = _gallery_filter_sanitize($key, $value);
        }
      }
      // Treat 'maxsize' and 'size' as the same
      if (isset($params['size'])) {
        $params['maxsize'] = $params['size'];
        unset($params['size']);
      }
      // Carefully treat the default size method (cannot just merge them as the
      // entered value must take precedence over the default)
      if (isset($params['maxsize'])) {
        unset($default['exactsize']);
      }
      else if (isset($params['exactsize'])) {
        unset($default['maxsize']);
      }
      // Merge params with default values
      $params = array_merge($default, $params);
      // Transform 'type' into a valid parameter
      if ($params['n'] > 1 && $params['type'] == 'specificItem') {
        $params['type'] = $default['type'];
      }
      if (is_array($params['type'])) {
        // Ensure 'type' contains 'n' elements (auto-append if necessary)
        $count = count($params['type']);
        if (($num = $params['n'] - $count) > 0) {
          $params['type'] += array_fill($count, $num, end($params['type']));
        }
      }
      else {
        $params['type'] = array_fill(0, $params['n'], $params['type']);
      }
      // 'frame' overrides 'album_frame' and 'item_frame'
      if ($params['frame']) {
        $params['album_frame'] = $params['item_frame'] = $params['frame'];
      }
      // Convert into G2-compatible arguments
      $params['blocks'] = implode('|', $params['type']);
      if (isset($params['maxsize']) && !empty($params['maxsize'])) {
        $params['maxSize'] = $params['maxsize'];
      }
      else if (isset($params['exactsize']) && !empty($params['exactsize'])) {
        $params['exactSize'] = $params['exactsize'];
      }
      $params['albumFrame'] = $params['album_frame'];
      $params['itemFrame'] = $params['item_frame'];
      $params['show'] = implode('|', $params['show']);
      $params['linkTarget'] = $params['target'];
      // Unset redundant parameters
      unset(
        $params['n'],
        $params['type'],
        $params['exactsize'],
        $params['maxsize'],
        $params['frame'],
        $params['album_frame'],
        $params['item_frame'],
        $params['target']
      );
      gallery_debug($params, t('Filter parameters'));
      // Fetch the images and format output
      list($ret, $content, $head) = GalleryEmbed::getBlock('imageblock', 'ImageBlock', $params);
      if ($ret) {
        gallery_error(t('Error trying to get image block.'), $ret);
        continue;
      }
      $content = trim($content);
      if (!empty($content)) {
        $params['class'] = 'giImageBlock'. ($params['class'] ? ' '. $params['class'] : '');
        $content = '<div class ="'. $params['class'] .'">'. $content .'</div>';
        // Allow other modules to alter the filter output
        drupal_alter('gallery_filter', $content, array('params' => $params));
      }
      // Replace G2 filter tag with image block html (or at least remove filter tag)
      $text = str_replace($match[0], $content, $text);
      if ($head) {
        $head_array[] = trim($head);
      }
    }
  }
  
  // Add html head items and css
  if (count($head_array)) {
    gallery_set_head(implode("\n", array_unique($head_array)));
  }
  GalleryEmbed::done();
  
  return $text .'<br class="giImageBlock-clear-both" />';
}

/**
 * Function _gallery_filter_sanitize().
 * (sanitize filter parameters)
 */
function _gallery_filter_sanitize($key, $value) {
  switch ($key) {
    case 'n':
    case 'size':
    case 'maxsize':
    case 'exactsize':
      return intval(preg_replace('/\D/', '', $value));
    case 'class':
    case 'frame':
    case 'album_frame':
    case 'item_frame':
    case 'target':
    case 'link':
      return preg_replace('/\W/', '', $value);
    case 'type':
    case 'show':
      return explode('|', preg_replace('/[^\w\x7c]/', '', $value));
    default :
      return check_plain($value);
  }
  
  return $value;
}
