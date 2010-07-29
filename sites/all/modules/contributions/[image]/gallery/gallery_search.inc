<?php
// $Id: gallery_search.inc,v 1.4.2.3 2008/02/18 23:07:15 profix898 Exp $

/**
 * gallery.module search functions
 */
 
/**
 * Implementation of hook_search().
 */
function _gallery_search($op = 'search', $keys = NULL) {
  switch ($op) {
    case 'name':
      return t('Gallery');
    case 'search':
      $results = array();
      $head_array = array();
      $items_per_row = variable_get('gallery_search_num_per_row', 3);
      $rows_per_pager = variable_get('gallery_search_rows_per_pager', 4);
      $matches = _gallery_search_pager_search($keys, $items_per_row * $rows_per_pager);
      if ($matches['count']) {
        $results['count'] = $matches['count'];
        // Parameters for the search results
        $params = array();
        $params['blocks'] = 'specificItem';
        $param_show_array = variable_get('gallery_search_block_show', array());
        $params['show'] = is_array($param_show_array) ? implode('|', $param_show_array) : '';
        if (variable_get('gallery_search_size_method', GALLERY_SEARCH_SIZE_METHOD_DEFAULT) == 'maxsize') {
          $params['maxSize'] = variable_get('gallery_search_size', GALLERY_SEARCH_SIZE_DEFAULT);
        } 
        else {
          $params['exactSize'] = variable_get('gallery_search_size', GALLERY_SEARCH_SIZE_DEFAULT);
        }
        $params['albumFrame'] =  variable_get('gallery_search_album_frame', 'none');
        $params['itemFrame'] =  variable_get('gallery_search_item_frame', 'none');
        $params['linkTarget'] =  variable_get('gallery_search_link_target', '');
        $params['link'] = variable_get('gallery_search_link', '');
       
        $show_thumbs = variable_get('gallery_search_show_thumbs', 1);
        // Loop over the results
        foreach ($matches['results'] as $item) {  
          $excerpt = array();
          // Get a thumbnail for this item
          if ($show_thumbs) {
            $params['itemId'] = $item['itemId'];
            list($ret, $thumbnail, $head) = GalleryEmbed::getBlock('imageblock', 'ImageBlock', $params);
            if ($ret) {
              $thumbnail = t('n/a');
            }
            if ($head) {
              $head_array[] = $head;
            }
          }
          // Generate a snippet with highlighted search keys
          foreach ($item['fields'] as $field) {
            if (preg_match("/$keys/i", $field['value'])) {
              $excerpt[] = '<em>'. $field['key'] .':</em> '. search_excerpt($keys, $field['value']);
            }
          }
          // Put everything into the $results array
          $title = reset($item['fields']);
          $results[] = array(
            'link' => gallery_generate_url(array('itemId' => $item['itemId']), FALSE),
            'title' => empty($title['value']) ? t('Gallery item: Untitled') : $title['value'],
            'snippet' => implode('<br />', $excerpt),
            'thumbnail' => $thumbnail,
          );
        }
      }
      // Add html head items and css
      if (count($head_array)) {
        gallery_set_head(implode("\n", array_unique($head_array)));
      }
      return $results;
  }
}

/**
 * Function _gallery_search_pager_search().
 */
function _gallery_search_pager_search(&$keys, $limit = 10, $element = 0) {
  // Adapted version of the pager_query() function (from /includes/pager.inc)
  // for use with the Gallery2 search() function
  //
  global $pager_page_array, $pager_total, $pager_total_items;
  $page = isset($_GET['page']) ? $_GET['page'] : '';

  // Convert comma-separated $page to an array, used by other functions.
  $pager_page_array = explode(',', $page);

  // We calculate the total of pages as ceil(items / limit).
  $count = _gallery_search_perform($keys);
  $pager_total_items[$element] = $count['count'];
  $pager_total[$element] = ceil($pager_total_items[$element] / $limit);
  $pager_page_array[$element] = max(0, min((int)$pager_page_array[$element], ((int)$pager_total[$element]) - 1));
  
  return _gallery_search_perform($keys, $pager_page_array[$element] * $limit, $limit);
}

/**
 * Function _gallery_search_perform().
 */
function _gallery_search_perform($keys, $offset = 0, $limit = -1) {
  list($search_interface, $options) = _gallery_search_init();
  if (!isset($search_interface)) {
    return array();
  }
  // Extract query parameters
  if ($fields = search_query_extract($keys, 'fields')) {
    $keys = trim(preg_replace('/\s+fields:[\w,]*/', '', $keys));
  }
  $fields = $fields ? array_flip(explode(',', $fields)) : $options;
  foreach ($fields as $key => $value) {
    $fields[$key] = $key;
  }
  // Perform the actual search
  list($ret, $matches) = $search_interface->search($fields, $keys, $offset, $limit);
  if ($ret) {
    gallery_error(t('Error invoking search() method.'), $ret);
    return array();
  }
  
  return $matches;
}

/**
 * Function _gallery_search_form().
 */
function _gallery_search_form(&$form, $form_state) {
  list($search_interface, $options) = _gallery_search_init();
  if (!count($options)) {
    return;
  }
  // Extend search form
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced search'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array('class' => 'search-advanced'),
  );
  
  $form['advanced']['fields'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Only the following fields'),
    '#prefix' => '<div class="criterion">',
    '#suffix' => '</div>',
    '#options' => $options,
  );
  
  $form['advanced']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Advanced search'),
    '#prefix' => '<div class="action clear-block">',
    '#suffix' => '</div>',
  );
  
  $form['#validate'][] = '_gallery_search_validate';
}

/**
 * Function _gallery_search_validate().
 */
function _gallery_search_validate($form, &$form_state) {
  $keys = $form_state['values']['processed_keys'];
  // Append field options to query
  if (isset($form_state['values']['fields']) && is_array($form_state['values']['fields'])) {
    $form_state['values']['fields'] = array_filter($form_state['values']['fields']);
    if (count($form_state['values']['fields'])) {
      $keys = search_query_insert($keys, 'fields', implode(',', array_keys($form_state['values']['fields'])));
      form_set_value($form['basic']['inline']['processed_keys'], trim($keys), $form_state);
    }
  }
}

/**
 * Function _gallery_search_init().
 */
function _gallery_search_init() {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  // Create instance of search interface
  list($ret, $search_interface) = GalleryCoreApi::newFactoryInstance('GallerySearchInterface_1_0');
  if ($ret) {
    gallery_error(t('Error creating instance of GallerySearchInterface. Make sure the \'Search\' plugin is enabled in Gallery2.'), $ret);
    return array(NULL, array());
  }
  // Get search module info
  list($ret, $module_info) = $search_interface->getSearchModuleInfo();
  if ($ret) {
    gallery_error(t('Error getting \'Search\' module options.'), $ret);
    return array(NULL, array());
  }
  $options = array();
  foreach ($module_info['options'] as $module => $info) {
    if ($info['enabled']) {
      $options[$module] = $info['description'];
    } 
  }
  
  return array($search_interface, $options);
}

/**
 * Function _gallery_search_page().
 */
function _gallery_search_page($results) {
  $items_per_row = variable_get('gallery_search_num_per_row', 3);
  $rows_per_pager = variable_get('gallery_search_rows_per_pager', 4);
  
  $output  = '<dl class="search-results">';
  $output .= t('<p>Total Number of Matches: @count</p>', array('@count' => $results['count']));
  unset($results['count']);
  // Arrange items as table
  $rows = array();
  $results = array_chunk($results, $items_per_row);
  foreach ($results as $item_row) {
    $row = array();
    foreach ($item_row as $item) {
      $row[] = array('data' => theme('gallery_search_item', $item));
    }
    $rows[] = $row;
  }
  $output .= theme('table', array(), $rows);
  $output .= '</dl>';
  $output .= theme('pager', NULL, $items_per_row * $rows_per_pager, 0);

  return $output;
}

/**
 * Theme function : theme_gallery_search_item().
 */
function theme_gallery_search_item($item) {
  $output  = ' <dt class="title"><a href="'. check_url($item['link']) .'">'. check_plain($item['title']) .'</a></dt>';
  $output .= '<div class="g2image_centered">'. $item['thumbnail'] .'</div>'; 
  $output .= ' <dd>'. ($item['snippet'] ? '<p>'. $item['snippet'] .'</p>' : '') .'</dd>';
  
  return $output;
}
