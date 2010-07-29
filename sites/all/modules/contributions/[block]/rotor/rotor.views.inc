<?php
//$Id: rotor.views.inc,v 1.1.2.2 2008/12/17 08:35:25 mrfelton Exp $

/**
 * Implementation of hook_views_plugins
 */
function rotor_views_plugins() {
  return array(
    'module' => 'rotor',
    'style' => array(
      'rotor' => array(
        'title' => t('Rotor'),
        'help' => t('Displays nodes as a rotoating banner.'),
        'handler' => 'rotor_plugin_style_rotor',
        'theme' => 'rotor_view_rotor',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

function template_preprocess_rotor_view_rotor(&$vars) {
  $view = $vars['view'];
  $vars['name'] = $view->name;
  $vars['display_id'] = $view->current_display;
}
