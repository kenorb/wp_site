<?php
// $Id: timeline.theme.inc,v 1.13 2008/09/18 02:23:27 jmiccolis Exp $
/**
 * @file
 * Theme functions for timeline.
 */

/**
 * Implementation of template_process for views-view-timeline.
 */
function template_preprocess_views_view_timeline(&$vars) {

  // Establish initial focus.
  $processed_fields = timeline_process_fields($vars['view']->query->fields);
  $field = $processed_fields['start'];

  switch ($vars['options']['display']['focus']) {
    case 'today':
      $initial_date = time();
      break;
    case 'first':
      $initial_date = $vars['view']->result[0]->{$field['table'] .'_'. $field['field']};
      break;
    case 'last':
      $initial_date = $vars['view']->result[count($vars['view']->result)-1]->{$field['table'] .'_'. $field['field']};
      break;
  }

  if (!is_numeric($initial_date)) {
    $initial_date = _timeline_strtotime($initial_date);
  }

  $timeline = new stdClass();
  $timeline->id = $vars['view']->name;
  $timeline->class = $vars['view']->name;
  $timeline->width = $vars['options']['display']['width'];
  $timeline->height = $vars['options']['display']['height'];
  $timeline->align = '';
  $timeline->orientation = ($vars['options']['type']);
  $timeline->initial_date = timeline_format_initial_date($initial_date);
  $timeline->controls = $vars['options']['misc']['controls'];
  $timeline->view = $vars['view'];
  $timeline->bands = array();
  foreach ($vars['rows'] as $row) {
    $timeline->bands[] = strtoupper($vars['options']['bands']['band1_unit']);
  }
  $timeline->summary = strtoupper($vars['options']['bands']['band2_unit']);

  timeline_add_js($timeline);

  $style = array('width' => _timeline_to_dim($timeline->width), 'height' => _timeline_to_dim($timeline->height));
  switch (strtolower($timeline->align)) {
    case 'left':
    case 'right':
      $style['float']  = strtolower($timeline->align);
      break;
    case 'center':
      $style += array('margin-left' => 'auto', 'margin-right' => 'auto');
      break;
  }
  $vars['style'] = implode('; ', array_map_assoc(create_function('$k, $v', 'return "$k: $v";'), $style));
  $vars['class'] = $vars['view']->name;
  $vars['id'] = $vars['view']->name;
  $vars['controls'] = $vars['options']['misc']['controls'];

}

/**
 * Adds the timeline js to the page.
 *
 * @param
 *   $timeline an object with the following attributes set:
 *
 *   id - the XHTML identifier of the timeline element
 *   width - the width of the timeline widget
 *   height - the height of the timeline widget
 *   class - CSS class of the timeline element (default 'timeline')
 *   align - left, right or center (default '')
 *   orientation - HORIZONTAL or VERTICAL
 *   initial_date - the initial focus date of the timeline
 *   timezone - the timezone represented by # of hours difference from GMT
 *   controls - whether to show the filtering controls
 *   bands -
 *   summary -
 *   view_name - the name of the Drupal view to render in the timeline
 *   view_args - an array of additional view arguments
 *
 */
function timeline_add_js($timeline) {
  $widget_url = variable_get('timeline_widget_url', 'sites/all/modules/timeline/api/timeline-api.js');

  if (strpos($widget_url, '://') === FALSE) { // relative URL
    drupal_add_js($widget_url);
  }
  else { // absolute URL
    // Can i use drupal_add_js here? - jm
    drupal_set_html_head('<script type="text/javascript" src="'. $widget_url .'"></script>');
  }

  drupal_add_css(drupal_get_path('module', 'timeline') .'/timeline.css');
  drupal_add_js(drupal_get_path('module', 'timeline') .'/timeline.js');

  // FIXME: timezone fiddling due to bug in the SIMILE widget; see timeline_format_iso8601_date().
  //if (!isset($timeline->timezone)) $timeline->timezone = timeline_get_timezone() / 3600;
  if (!isset($timeline->timezone)) {
    $timeline->timezone = 0;
  }

  $query = 'view='. $timeline->view->name . (empty($timeline->view->args) ? '' : '&args='. urlencode(serialize($timeline->view->args)));
  $url = url('timeline/'. variable_get('timeline_feed_type', 'json'), array('query' => $query));

  $args = array(
    $timeline->id,
    $timeline->orientation,
    $timeline->initial_date,
    $timeline->timezone,
    $timeline->bands,
    $timeline->summary,
    $url,
    variable_get('timeline_feed_type', 'json'),
  );

  $script = 'Drupal.timeline.createWidget('. implode(', ', array_map('drupal_to_js', $args)) .');';

  if ($timeline->controls) {
    $texts = implode(', ', array_map('drupal_to_js', array(t('Filter:'), t('Highlight:'), t('Clear All'))));
    $script = 'var timeline = '. $script ." Drupal.timeline.controls.setup('{$timeline->id}-controls', timeline, $texts);";
  }

  $script = 'if (Drupal.jsEnabled) { $(document).ready(function() { '. $script .' }); }';

  // Not sure if I can get away with this in all browsers -jm
  drupal_add_js($script, 'inline');
}

//////////////////////////////////////////////////////////////////////////////
// TIMELINE HELPERS
// TODO: REMOVE THESE - jm

if (!function_exists('array_map_assoc')) {
  function array_map_assoc($callback, $array) {
    return array_map($callback, array_keys($array), array_values($array));
  }
}

function _timeline_to_dim($string) {
  return intval($string) . (strpos($string, '%') !== FALSE ? '%' : 'px');
}
