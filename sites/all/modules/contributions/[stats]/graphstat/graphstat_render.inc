<?php
// $Id: graphstat_render.inc,v 1.1.2.2 2009/01/25 22:13:45 profix898 Exp $

function graphstat_render_graph($group, $key) {
  global $user;
  
  $filter = isset($_SESSION['graphstat_filter'][$group]) ? $_SESSION['graphstat_filter'][$group] : NULL;
  $cache = cache_get('graphstat:'. $user->uid);
  $width = $cache ? $cache->data['width'] : 500;
  $height = $cache ? $cache->data['height'] : 300;
  $time = format_date(time(), 'custom', 'H');

  $cid = 'graphstat:'. md5($group . $key . $filter) .':'. $width .':'. $time;
  if ($cache = cache_get($cid)) {
    $image = $cache->data;
  }
  else {
    // Cache Miss: we need to generate the graphs from scratch
    $graphs = module_invoke_all('graphstat');
    $graphs = $graphs[$group];
    $data = module_invoke_all('graphstat_data');
    $data = isset($data[$group]) ? $data[$group] : array();
    // If a filter is defined then invoke the callback function
    // to fetch the filtered graph data
    if (isset($graphs['#filter'])) {
      $function = $graphs['#filter']['#callback'];
      if (function_exists($function)) {
        $function($data, $filter);
      }
    }
    $data = isset($data[$key]) ? $data[$key] : array();
    // If no graph data available then render a placeholder image
    if (empty($data)) {
      if (function_exists('imagepng')) {
        $img = imagecreate($width, $height);
        imagefill($img, 0, 0, imagecolorallocate($img, 240, 240, 240));
        imageline($img, 0, 0, $width, $height, imagecolorallocate($img, 128, 128, 128));
        imageline($img, 0, $height, $width, 0, imagecolorallocate($img, 128, 128, 128));
        imagepng($img);
        imagedestroy($img);
      }
      exit();
    }
    
    //
    $renderer = variable_get('graphstat_renderer', 'graphstat');
    $graph = module_invoke($renderer, 'graphstat_renderer', $graphs[$key], $data, $width, $height);
    
    //
    if (is_array($graph)) {
      $image = $graph['image'];
      if (isset($graph['#type'])) {
        drupal_set_header($graph['#type']);
      }
    }
    else {
      $image = $graph;
      drupal_set_header('Content-type: image/png');
    }
    
    // Put $image into the cache
    cache_set($cid, $image, 'cache', CACHE_TEMPORARY);
  }

  // Print out the image and make sure to exit immediatly
  print $image;
  exit();
}

/**
 * Implementation of hook_graphstat_renderer().
 * (renders the actual graph images, in this case with the PHPLOT library)
 */
function graphstat_graphstat_renderer($graph, $data, $width, $height) {
  require_once(drupal_get_path('module', 'graphstat') .'/phplot.php');
  
  // Create instance of PHPLOT and configure graph paramters
  $plot =& new PHPlot($width, $height);
  $graph['#type'] = isset($graph['#type']) ? $graph['#type'] : 'linepoints';
  $plot->SetPlotType($graph['#type']);
  $plot->SetTitle(isset($graph['#title']) ? $graph['#title'] : '');
  $plot->SetXTitle(isset($graph['#xlabel']) ? $graph['#xlabel'] : '');
  $plot->SetXTickLabelPos('none');
  $plot->SetYTitle(isset($graph['#ylabel']) ? $graph['#ylabel'] : '');
  $plot->SetXLabelAngle(90);
  if (isset($data['#legend'])) {
    $plot->SetLegend($data['#legend']);
    unset($data['#legend']);
  }
  else if (isset($graph['#legend'])) {
    $plot->SetLegend($graph['#legend']);
  }
  // Push graph data into the plot renderer
  if (!is_array(current($data))) {
    $series = $data;
    $data = array();
    foreach ($series as $x => $y) {
      $data[] = array($x, $y);
    }
  }
  $plot->SetDataValues($data);
  // Draw the graph into a buffer $image
  ob_start();
  $plot->DrawGraph();
  $image = ob_get_contents();
  ob_end_clean();
  
  return $image;
}
