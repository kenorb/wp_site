<?php
// $Id: google_charts.inc,v 1.14 2008/08/10 16:23:45 brmassa Exp $
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Use Google Charts on your site.
 */

/**
 * Convert all Chart-level data.
 *
 * @param &$chart
 *   Array. The array that will be converted into a string for Google Charts
 * @param &$data
 *   Array. The raw data.
 * @return
 *   String. The string presentation of series data
 */
function _google_charts_chart(&$chart, &$data) {
  // Convert the chat TYPE into the Google Chart way.
  // Since its a requirement to build the chart on Google, if the value
  // was not found, return nothing and stop the execution.
  $options = array(
    'line2D'  => 'lc',
    'hbar2D'  => 'bhg',
    'vbar2D'  => 'bvg',
    'pie2D'   => 'p',
    'pie3D'   => 'p3',
    'venn'    => 'v',
    'scatter' => 's',
  );
  if (empty($options[$data['#type']])) {
    return t('This type is not possible using %chartplugin',
      array('%chartplugin' => 'Google Chart'));
  }
  $chart[] = 'cht='. $options[$data['#type']];

  // Convert the chat SIZE into the Google Chart way.
  // Since its a requirement to build the chart on Google, if the value
  // was not found, return nothing and stop the execution.
  if (empty($data['#width']) or empty($data['#height'])) {
    return t('Height and Width are required');
  }
  $chart[] = 'chs='. $data['#width'] .'x'. $data['#height'];

  // Add Title and Description to the chart
  if (!empty($data['#title'])) {
    $chart[] = 'chtt='. $data['#title'];
  }

  // Chart background color. Since the default color
  // is white (#ffffff), only different colors are considered
  if (!empty($data['#color']) and $data['#color'] != 'ffffff') {
    $chart[] = 'chf=bg,s,'. $data['#color'];
  }

  return;
}

/**
 * Encode the Chart data into a Alphanumeric code, follwing the
 * Google Chart API instruction. Its needed because there is a
 * size limmit to URL strings. So we reduce the amount of characters.
 *
 * It basicly uses a scale of 61 levels to represent each chart
 * value. If a more precise scale is needed, see
 * _google_charts_codingextended(), which produces a 4000 levels,
 * but also a bigger URL string.
 *
 * @param $data
 *   Array. A series of numeric data values
 * @return
 *   String. The string presentation of series data
 */
function _google_charts_data_codingsimple($value, $max) {
  // Set the list of characters and the size of the list
  $simple = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
  $simple_len = drupal_strlen($simple) - 1;

  if ($value >= 0) {
    return $simple[round($simple_len * $value / $max)];
  }
  else {
    return  '_';
  }
}

/**
 * Implementation of hook_charts_render().
 *
 * Its a Charts module hook. It transform the data into a HTML chart.
 *
 * @param &$data
 *   Array. The
 */
function _google_charts_render(&$data) {
  $chart = array();
  if ($message = _google_charts_chart($chart, $data)) {
    return $message;
  }

  // Convert the chat DATA into the Google Chart way.
  // Since its a requirement to build the chart on Google, if the value
  // was not found, return nothing and stop the execution.
  if ($message = _google_charts_series($chart, $data)) {
    return $message;
  }

  // If its all ok, build the HTML img tag
  return '<img src="http://chart.apis.google.com/chart?'. implode('&amp;', $chart) .'" />';
}

/**
 * Convert all Series-level data.
 *
 * @param &$chart
 *   Array. The array that will be converted into a string for Google Charts
 * @param &$data
 *   Array. The raw data.
 * @return
 *   String. The string presentation of series data
 */
function _google_charts_series(&$chart, &$data) {
  // The final output is going to be build
  $chart_data = '';

  $value_labels = array();

  // For each chart value, encode it
  // Note: Underscore represents a missing value
  foreach (element_children($data) as $series) {
    // Include a series separator
    if (!empty($chart_data)) {
      $chart_data .= ',';
    }

    // Get only the numeric values from the series
    $series_data = _charts_series_values($data[$series]);

    // Get the highest value on the series, to be a reference point
    $max = max($series_data);

    // Value labels: temporary array.
    $value_labels_temp = array();

    // For each series of data, scan it
    foreach (array_keys($series_data) as $value) {
      $chart_data .= _google_charts_data_codingsimple($series_data[$value], $max);

      $value_labels_temp[] = empty($data[$series][$value]['#label']) ? NULL : $data[$series][$value]['#label'];
    }

    // Value labels
    if (!empty($value_labels_temp)) {
      $value_labels += $value_labels_temp;
    }

    // Series legends
    $legends[] = empty($data[$series]['#legend']) ? NULL : $data[$series]['#legend'];

    // Series colors
    $colors[] = empty($data[$series]['#color']) ? NULL : $data[$series]['#color'];
  }

  // Return the series of data
  if (empty($chart_data)) {
    return t('No enough data to create a chart.');
  }

   // Insert data
  $chart[] = 'chd=s:'. $chart_data;

  // Insert series legend
  $legends = implode('|', $legends);
  if (str_replace('|', '', $legends)) {
    $chart[] = 'chdl='. $legends;
  }

  // Insert series legend
  $colors = implode(',', $colors);
  if (str_replace(',', '', $value_labels)) {
    $chart[] = 'chco='. $colors;
  }

  // Insert values legend
  $value_labels = implode('|', $value_labels);
  if (str_replace('|', '', $value_labels)) {
    $chart[] = 'chl='. $value_labels;
  }

  return;
}
