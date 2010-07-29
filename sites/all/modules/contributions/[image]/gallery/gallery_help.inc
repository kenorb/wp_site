<?php
// $Id: gallery_help.inc,v 1.6.2.6 2008/06/10 15:46:19 profix898 Exp $

/**
 * gallery.module : gallery_help.inc
 * Help and documentation
 */

/**
 * Implementation of hook_help().
 */
function _gallery_help($section) {
  switch ($section) {
    case 'filter#short-tip' :
      return gallery_help_filter_short_tip();
    case 'filter#long-tip' :
      return gallery_help_filter_long_tip();
    default:
      return '';
  }
}

/**
 * Function gallery_help_filter_short_tip().
 */
function gallery_help_filter_short_tip() {
  return t('You may link to Gallery2 items on this site <a href="@url">using a special syntax</a>.',
            array('@url' => url('filter/tips', array('fragment' => 'filter-gallery-0'))));
}

/**
 * Function gallery_help_filter_long_tip().
 */
function gallery_help_filter_long_tip() {
  $prefix = trim(variable_get('gallery_filter_prefix', 'G2')) .':';
  $path = drupal_get_path('module', 'gallery') .'/help/filter-gallery.html';
  
  $output  = '<p><strong>Gallery2 Filter:</strong> </p>';
  $output .= str_replace('PREFIX:', $prefix, file_get_contents($path));
  
  return $output;
}

/**
 * Function _gallery_report_help().
 */
function _gallery_report_help() {
  return t('<p><strong>A serious error has occured. This can happen due to an incorrect configuration or a bug in the
            gallery module. If you plan to submit a bug report to the issue queue at <a href="@gallery-issues">drupal.org</a>
            consider to include the <a href="@report">pre-generated report file</a>.</strong></p>',
            array(
              '@gallery-issues' => 'http://drupal.org/project/issues/gallery',
              '@report' => url('admin/settings/gallery/report/download'))
          );
}

/**
 * Function _gallery_advanced_help_get_form_help().
 */
function _gallery_advanced_help_get_form_help($form_id, $module = 'gallery') {
  static $form_help = NULL;

  if (!isset($form_help)) {
    $module_path = drupal_get_path('module', $module);
    if (file_exists("$module_path/help/$module.forms_help.ini")) {
      $info = parse_ini_file("./$module_path/help/$module.forms_help.ini", TRUE);
      foreach ($info["$module-forms"] as $form => $topic) {
        $form = explode('@', $form);
        $form_help[$form[1]][$form[0]] = $topic;
      }
    }
  }

  return isset($form_help[$form_id]) ? $form_help[$form_id] : array();
}
