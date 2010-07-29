<?php
// $Id: nicemap.theme.inc,v 1.1.2.7 2009/02/11 18:49:15 tmcw Exp $

/**
 * @file
 * All theme functions for the nicemap module.
 */

/**
 * Wrapper theme function allows for easy overriding of map style
 *
 * @param $points
 *   Array An array of points in the form:
 *       array('title' => 'Chester',
 *           'content' => 'Test',
 *           'lat' => 40.47,
 *           'lon' => -74.41,
 *           'weight' => 5,
 *           ),
 *  @param $map
 *   nicemap_map object
 *  @param $options 
 *    array of options: valid keys are
 *      js: include js on map (boolean, default true)
 *      width: in px
 *      height: in px
 *      layers: array
 *      styles: array
 *      bbox: array
 *
 *  @return
 *   A fully themed map
 */
function theme_nicemap_map($points = array(), $map, $options) {
  global $base_url;

  drupal_add_css(drupal_get_path('module', 'nicemap') .'/nicemap.css');

  /* This is necessary to get IE < 7 to correctly display
   * alpha-transparent png backgrounds.
   */
  drupal_set_html_head("<!--[if lt IE 7]>
<style type='text/css' media='screen'>
/* Since IE doesn't do alpha PNGs... */
body .nicemap-map a.geopoint span {
  background: url(\"". $base_url .'/'. 
    drupal_get_path('module', 'nicemap') 
    ."/images/label.gif\") no-repeat;
}
body .nicemap-map a.geopoint {
  zoom:1;
}
body .nicemap-map a.hover span.label {
  display:block;
}
body .nicemap-map a.nohover span.label {
  display:none;
}
body .nicemap-map a.clone {
  filter: none;
  background:transparent !important;
}
</style>
    <![endif]-->");

  if (!isset($options['js']) || $options['js']) {
    drupal_add_js(drupal_get_path('module', 'nicemap') .'/nicemap.js');
  }

  try {
    $points = $map->process($points, 
      array(
        'width' => $options['width'],
        'height' => $options['height']),
      variable_get('nicemap_wms_crs', '')
    );
  }
  catch( Exception $e ) {
    $points = array();
    watchdog('nicemap', $e->getMessage());
  }

  return _theme_nicemap_map_full($points, $map, $options);
}

/**
 * Theme a point on a map
 *
 * @param $point 
 *   an array which, by default requires the keys
 *   title, weight, x, y and an optional array of attributes
 * 
 */
function theme_nicemap_point($point) {
  $weight = $point['weight'] ? ' weight-'. $point['weight'] : '';
  $title = "<span>". truncate_utf8(strip_tags($point['title']), 18, FALSE) ."</span>";
  $point['attributes']['class'] = $point['attributes']['class'] ?
    $point['attributes']['class'] ." geopoint $weight" :
    "geopoint $weight";

  // sorry, we're gonna bulldoze your style attributes
  $point['attributes']['style'] = 'left: '. $point['x'] .'%; top: '. $point['y'] .'%;';
  // and your id attributes
  $point['attributes']['id'] = "geopoint-". $point['i'];
  return l($title, $point['href'], array('attributes' => $point['attributes'], 'query' => NULL, 'fragment' => NULL, 'absolute' => TRUE, 'html' => TRUE));
}

function theme_nicemap_content($point) {
  $content = $point['content'];
  $close = "<span class='close'>". t('Close') ."</span>";
  return "<div class='geoitem' id='geoitem-". $point['i'] ."'>$close $content</div>";
}

function _theme_nicemap_map_full($points, $map, $options) {
  $w = $options['width']  .'px';
  $h = $options['height'] .'px';
  $defaults = variable_get('nicemap_defaults', 
    array(
      'layers' => array(),
      'styles' => array(),
      'weights' => array()
    )
  );
  $options['srs'] = variable_get('nicemap_wms_crs', 'EPSG:900913');
  $options['bgcolor'] = variable_get('nicemap_wms_bgcolor', 'ffffff');

  if(!$options['layers']) {
    $defaults  = variable_get('nicemap_defaults', '');

    // sort by weights
    $layers = array_combine(array_keys($defaults['layers']), $defaults['weights']);
    asort($layers);

    foreach($layers as $l => $w) {
      if($defaults['layers'][$l]) { // layer enabled
        $options['layers'][] = $l;
        $options['styles'][] = $defaults['styles'][$l];
      }
    }
  }

  $map_url = $map->url($options);

  // This hook allows other modules to modify the 
  // URL for authentication or other reasons
  $map_url_options = module_invoke_all('nicemap_prepare_url', $map_url, $map);
  $map_url = $map_url_options[0];

  if ($map_url) {
    $map_bg = "background-image:url('$map_url');";
  }

  // use a hardcoded index to associate point to item
  $i = 0;
  if($points) {
    foreach ($points as $point) {
      $point['i'] = $i;
      $geopoints  .= theme('nicemap_point', $point);
      $geocontent .= theme('nicemap_content', $point);
      $i++;
    }
  }

  $attr = array(
    "class" => "nicemap-map",
    "style" => "$map_bg width:$w; height:$h;",
  );

  $attr = drupal_attributes($attr);

  return "
    <div $attr>
      $geopoints
      <div class='hidden'>$geocontent</div>
    </div>
  ";
}
