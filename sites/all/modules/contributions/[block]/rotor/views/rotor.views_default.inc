<?php
//$Id: rotor.views_default.inc,v 1.1.2.2 2008/12/21 14:40:54 mrfelton Exp $
function rotor_views_default_views() {
   
  $view = new view;
  $view->name = 'rotor';
  $view->description = 'Default view for the Botor Banner module - A block that rotates through all Rotor Items';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'body' => array(
      'label' => 'Body',
      'exclude' => 0,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'rotor_item' => 'rotor_item',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('style_plugin', 'rotor');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'width' => '200',
    'height' => '200',
    'seconds' => '5',
    'speed' => '2',
    'effect' => 'fade',
    'pause' => 1,
  ));
  $handler->override_option('row_plugin', 'unformatted');
  $handler->override_option('row_options', NULL);
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  

  $views[$view->name] = $view;
  return $views;
}