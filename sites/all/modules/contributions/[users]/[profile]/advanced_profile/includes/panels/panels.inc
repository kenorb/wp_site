<?php
// $Id: panels.inc,v 1.1.4.6 2009/01/07 20:01:12 michellec Exp $

/**
 * @file
 * Handles integration with the Panels module.
 */

// Load the exported panel page, unless it's set to not be used.
if (variable_get('advanced_profile_use_panel_page', TRUE)) {
  function advanced_profile_default_panel_pages() {
    include_once drupal_get_path('module', 'advanced_profile') . '/includes/panels/panels.pages.inc';
    return _advanced_profile_default_panel_pages();
  }
}

/**
 * Implementation of hook_panels_include_directory().
 */
function advanced_profile_panels_include_directory($plugintype) {
  if ($plugintype == 'content_types') {
    return 'includes/panels/content_types';
  }
}


