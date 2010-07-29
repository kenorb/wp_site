<?php
// $Id: gallery_block.inc,v 1.8.2.13 2009/01/25 14:21:32 profix898 Exp $

/**
 * gallery.module : gallery_block.inc
 * Block functions
 */
 
/**
 * Implementation of hook_block().
 */
function _gallery_block($op = 'list', $delta = 0, $edit = array()) {
  if ($op != 'list') {
    list( , $delta) = explode('-', $delta);
    $delta = is_numeric($delta) ? $delta : 0;
  }
  switch ($op) {
    case 'list':
      $block_num = variable_get('gallery_block_num', 2);
      for ($i=0; $i<$block_num+1; $i++) {
        $id = variable_get('gallery_block_'. $i .'_blockid', '');
        $blocks['block-'. $i]['info'] = $i ? t('Gallery Block @id', array('@id' => '['. (empty($id) ? $i : $id) .']'))
                                           : t('Gallery Navigation');
        $blocks['block-'. $i]['cache'] = variable_get('gallery_block_'. $i .'_cache', BLOCK_NO_CACHE);
      }
      return $blocks;
    case 'view':
      if (user_access('access gallery') && _gallery_init(TRUE) && $_GET['q'] != 'admin/settings/gallery/install') {
        if ($delta) {
          $block = _gallery_block_block($delta);
        }
        else if ((arg(0) == variable_get('gallery_base', 'gallery')) && !empty($GLOBALS['gallery_sidebar'])) {
          $block['subject'] = t('Gallery Navigation');
          $block['content'] = '<div id="gsSidebar" class="gcBorder1">'. implode('', $GLOBALS['gallery_sidebar']) .'</div>';
        }
        GalleryEmbed::done();
        return isset($block) ? $block : FALSE;
      }
      break;
    case 'configure':
      require_once(drupal_get_path('module', 'gallery') .'/gallery_block_admin.inc');
      return $delta ? _gallery_block_admin_block($delta) : _gallery_block_admin_navigation();
    case 'save':
      require_once(drupal_get_path('module', 'gallery') .'/gallery_block_admin.inc');
      _gallery_block_admin_save($edit);
      break;
  }
}

/**
 * Function _gallery_block_map().
 */
function _gallery_block_map($refresh = FALSE) {
  // Get block map from cache (or rebuild map)
  $cache = cache_get('gallery:blockmap', 'cache');
  if (!$cache || $refresh) {
    // Rebuild block map
    $block_map = array();
    $g2_blocks = gallery_list_blocks();
    foreach ($g2_blocks as $plugin => $blocks) {
      foreach (array_keys($blocks) as $block) {
        switch ($block) {
          // Blocks with no required parameter
          case 'AlbumSelect':
          case 'AlbumTree':
          case 'ShowCart':
          case 'LanguageSelector':
          case 'MembersSummary':
          case 'QuotasSummary':
          case 'RssBlock':
            $params = array();
            break;
          // Parameter defaults to 'item'
          // (required by 'ViewComments', 'AddComment', 'ItemInfo', 'ItemLinks',
          //  'PeerList', 'CustomFields', 'ExifInfo', 'KeywordLinks', 'Rating', ...)
          default:
            $params = array('item');
        }
        //
        $block_map[strtolower($block)] = array(
          'plugin'  => $plugin,
          'block'   => $block,
          'params'  => $params
        );
      }
    }
    cache_set('gallery:blockmap', $block_map, 'cache', CACHE_PERMANENT);
  }
  else {
    $block_map = $cache->data;
  }
  
  return $block_map;
}

/**
 * Function _gallery_block_options().
 */
function _gallery_block_options(&$type_map, &$param_map) {
  $g2_blocks = gallery_list_blocks();
  $type_map = $g2_blocks['imageblock']['ImageBlock']['vars']['blocks']['choices'];
  $param_map = array(
    'heading' => t('Heading'),
    'title'   => t('Title'),
    'date'    => t('Date'),
    'views'   => t('View Count'),
    'owner'   => t('Item owner')
  );
}

/**
 * Function _gallery_block_block().
 */
function _gallery_block_block($delta) {
  $block_map = _gallery_block_map();
  $block_type = variable_get('gallery_block_'. $delta .'_type', 'imageblock');
  $plugin = $block_map[$block_type]['plugin'];
  $plugin_block = $block_map[$block_type]['block'];
  $params = $block_map[$block_type]['params'];
  
  // ImageBlock needs special handling
  if ($plugin_block == 'ImageBlock') {
    return _gallery_block_image_block($delta, $plugin, $plugin_block);
  }
  
  // Check for required parameters
  if (!gallery_context($params)) {
    return FALSE;
  }
  // Add custom parameters
  $g2_blocks = gallery_list_blocks();
  if (isset($g2_blocks[$plugin][$plugin_block]['vars'])) {
    foreach ($g2_blocks[$plugin][$plugin_block]['vars'] as $parameter => $options) {
      $block = strtolower($plugin_block .'_'. $parameter);
      $element = 'gallery_block_'. $delta .'_'. $block;
      $params[$parameter] = variable_get($element, $options['default']);
    }
  }
  
  return _gallery_block_get_block($plugin, $plugin_block, $params);
}

/**
 * Function _gallery_block_image_block().
 */
function _gallery_block_image_block($delta, $plugin, $plugin_block) {
  $element = 'gallery_block_'. $delta .'_'. strtolower($plugin_block);
  $num_cols = variable_get($element .'_num_cols', 2);
  $num_rows = variable_get($element .'_num_rows', 2);
  $num_images = $num_cols * $num_rows;
  
  $param_blocks_array = array_filter(variable_get($element .'_block_block', array('randomImage')));
  // Ensure $param_blocks_array contains $num_images elements (auto-append if necessary)
  $count = count($param_blocks_array);
  if (($num = $num_images - $count) > 0) {
    $param_blocks_array += array_fill($count, $num, end($param_blocks_array));
  }
  $params['blocks'] = implode('|', $param_blocks_array);
  $params['itemId'] = variable_get($element .'_item_id', '');
  $param_show_array = variable_get($element .'_block_show', array());
  $params['show'] = empty($param_show_array) ? 'none' : implode('|', $param_show_array);
  if (variable_get($element .'_size_method', GALLERY_IMAGEBLOCK_SIZE_METHOD_DEFAULT) == 'maxsize') {
    $params['maxSize'] = variable_get($element .'_size', GALLERY_IMAGEBLOCK_SIZE_DEFAULT);
  } 
  else {
    $params['exactSize'] = variable_get($element .'_size', GALLERY_IMAGEBLOCK_SIZE_DEFAULT);
  }
  $params['albumFrame'] = variable_get($element .'_album_frame', 'none');
  $params['itemFrame'] = variable_get($element .'_item_frame', 'none');
  $params['linkTarget'] = variable_get($element .'_link_target', '');
  $params['link'] = variable_get($element .'_link', '');
  
  $extra = ($num_cols > 1) ? array('num_cols' => $num_cols) : array();
  return _gallery_block_get_block($plugin, $plugin_block, $params, $extra);
}

/**
 * Function _gallery_block_get_block().
 */
function _gallery_block_get_block($plugin, $plugin_block, $params, $extra = array()) {
  // Handle useralbum feature (for ImageBlocks)
  if ($plugin_block == 'ImageBlock') {
    if (preg_match('/user(:([\d]+))?/i', $params['itemId'], $param_uid)) {
      require_once(drupal_get_path('module', 'gallery') .'/gallery_user.inc');
      $params['itemId'] = gallery_user_useralbum(isset($param_uid[2]) ? $param_uid[2] : NULL, FALSE);
    }
    if (empty($params['itemId']) || $params['itemId'] === FALSE) {
      unset($params['itemId']);
    }
  }
  
  gallery_debug($params, t('Block parameters'));
  // Get the block from G2
  list($ret, $content, $head) = GalleryEmbed::getBlock($plugin, $plugin_block, $params);
  if ($ret) {
    gallery_error(t('Error trying to get block.'), $ret);
    return array();
  }
  $content = trim($content);
  // Allow other modules to alter the block output
  drupal_alter('gallery_block', $content, array('params' => $params, 'extra' => $extra));
  // Format the block content
  $block = array();
  if (!empty($content)) {
    $block['subject'] = t('Gallery');
    $block['content'] = $content;
    //
    if ($plugin_block == 'ImageBlock') {
      if (isset($extra['num_cols'])) {
        $class = isset($extra['class']) ? $extra['class'] : 'gallery-grid-block';
        $block['content'] = theme('gallery_block_grid_block', $content, $extra['num_cols'], $class);
      }
      else {
        $class = isset($extra['class']) ? $extra['class'] : 'g2image_centered';
        $block['content'] = theme('gallery_block_image_block', $content, $class);
      }
    }
    // Add css/js to the page
    if ($head) {
      gallery_set_head($head);
    }
  }

  return $block;
}

/**
 * Function _gallery_block_split_imageblock().
 * (split an image block result into individual images)
 */
function _gallery_block_split_imageblock($html) {
  // From http://uk.php.net/manual/en/function.preg-split.php
  // Split the html from image block into <...> parts
  $pattern = '/(<(?:[^<>]+(?:"[^"]*"|\'[^\']*\')?)+>)/';
  $html_array = preg_split($pattern, trim($html), -1, PREG_SPLIT_DELIM_CAPTURE | PREG_SPLIT_NO_EMPTY);
  
  $images = array();
  $image_html = '';
  $ndx = $open_divs = 0;
  $found_image_block_class = FALSE;
  $imageblock_class = '<div class="block-imageblock-ImageBlock';

    // Iterate through this array and combine again, but on a per-image basis
  foreach ($html_array as $value) {
    $value = trim($value);
    if (!strncmp($value, $imageblock_class, strlen($imageblock_class))) {
      // Found the opening <div> for the image block (G2.3). We want to remove this.
      $found_image_block_class = TRUE;
    }
    else {
      $image_html .= $value;
    }
    if (!strcmp($value, '<div class="one-image">')) {
      // Found the opening <div> for the image
      $open_divs = 0;
    }
    elseif (!strncmp($value, '<div', 4)) {
      // Found a <div> but not the main image one (eg a frame)
      $open_divs++;
    }
    elseif (!strcmp($value, '</div>')) {
      // Found a </div> but check if it's for the main image or a subcomponent (eg frame)
      if ($open_divs > 0) {
        $open_divs--;
      }
      else {
        // This must be the closing div for "one-image" so move to next image
        $images[] = $image_html;
        $image_html = '';
      }
    }
  }
  // If using G2.3 there is a final <div> from imageblock that must be removed.
  if ($found_image_block_class) {
    unset($images[count($images)-1]);
  }

  return $images;  
}

/**
 * Template function : template_preprocess_gallery_block_image_block().
 */
function template_preprocess_gallery_block_image_block(&$variables) {
  $variables['class'] = check_plain($variables['class']);
  $variables['content'] = filter_xss_admin($variables['content']);
} 


/**
 * Template function : template_preprocess_gallery_block_grid_block().
 */
function template_preprocess_gallery_block_grid_block(&$variables) {
  $images = _gallery_block_split_imageblock($variables['content']);
  // Filter the images (and split columns)
  array_walk($images, 'filter_xss_admin');
  $images = array_chunk($images, $variables['num_cols']);
  // Leave the whole content in in case themes want to do more formatting
  $variables['content'] = filter_xss_admin($variables['content']);
  $variables['images'] = array();
  $row = 0;
  foreach ($images as $image_row) {
    $variables['images'][] = array(
      'data' => $image_row,
      'zebra' => ($row % 2 == 0) ? 'odd' : 'even'
    );
    $row++;
  }
  $variables['class'] = check_plain($variables['class']);
  $variables['num_cols'] = check_plain($variables['num_cols']);
}
