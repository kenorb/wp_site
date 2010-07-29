<?php
// $Id: nicemap_views.theme.inc,v 1.1.2.3 2009/02/11 18:49:15 tmcw Exp $

/**
 * @file
 * This file contains a single function which just wraps the 
 * views-generated display code in the theme functions provided
 * by nicemap.theme.inc
 */

/**
 * Theme preporocess for nicemap view style.
 */
function template_preprocess_views_view_nicemap_map(&$vars) {
  $view = $vars['view'];
  $options = $view->style_plugin->options;
  $points = $view->style_plugin->map_points($vars['rows']);

  $map = nicemap_cache(variable_get('nicemap_wms_url', ''));

  $map_options = array(
        'width'  => $options['width'], 
        'height' => $options['height'],
        'js' => TRUE);

  $map_options['bgcolor'] = $options['bgcolor'];

  // This seems like it shouldn't need to be set before doing map process(),
  // it should just be passed as an argument. Will need more looking.
  $map->bounds =  array(
      'miny' => $options['miny'],
      'maxy' => $options['maxy'],
      'minx' => $options['minx'],
      'maxx' => $options['maxx'],
  );

  if ($options['custom']) {
    /*
     * If you checked 'custom', we need to sort & determine 
     * enabled layers quickly here. Otherwise it'll be done by
     * the nicemap_map object itself.
     */
    $options['layers'] = array_filter($options['layers']);
    asort($options['weights']);
    foreach($options['weights'] as $l => $w) {
      if (isset($options['layers'][$l])) {
        $map_options['layers'][] = $l;
        $map_options['styles'][] = $options['styles'][$l];
      }
    }
  }

  $vars['map'] = theme('nicemap_map', $points, 
    $map, 
    $map_options
  );
}
