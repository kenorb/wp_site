<?php
// $Id: timeline.views.inc,v 1.6.2.3 2008/12/07 23:36:38 jmiccolis Exp $
/**
 * @file
 * Views plugins definition for timeline.
 */

/**
 * Implementation of hook_views_style_plugins(). Adds view types to views UI interface.
 */
function timeline_views_plugins() {
  return array(
    'style' => array(
      'timeline' => array(
        'title' => t('Timeline'),
        'help' => t('Displays content on a SIMILE Timeline.'),
        'handler' => 'timeline_views_plugin',
        'theme' => 'views_view_timeline',
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_default_views().
 */
function timeline_views_default_views() {
  $views = array();

  // Add timeline_nodes view.
  $view = new view;
  $view->name = 'timeline_nodes';
  $view->description = 'Example timeline of nodes; accepts YYYY/MM arg';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'created' => array(
      'label' => 'Post date',
      'date_format' => 'small',
      'custom_date_format' => '',
      'exclude' => 0,
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Title',
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'body' => array(
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
    ),
  ));
  $handler->override_option('sorts', array(
    'created' => array(
      'order' => 'ASC',
      'granularity' => 'second',
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'created_year' => array(
      'id' => 'created_year',
      'table' => 'node',
      'field' => 'created_year',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'default_argument_type' => 'fixed',
      'relationship' => 'none',
      'title' => '',
      'default_action' => 'ignore',
      'wildcard' => '',
      'wildcard_substitution' => '',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'event' => 0,
        'image' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
    'created_month' => array(
      'id' => 'created_month',
      'table' => 'node',
      'field' => 'created_month',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'default_argument_type' => 'fixed',
      'relationship' => 'none',
      'title' => '',
      'default_action' => 'ignore',
      'wildcard' => '',
      'wildcard_substitution' => '',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'event' => 0,
        'image' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '1' => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 50);
  $handler->override_option('style_plugin', 'timeline');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => '0',
    'display' => array(
      'width' => '100%',
      'height' => '350px',
      'focus' => 'first',
    ),
    'bands' => array(
      'band1_unit' => 'week',
      'band2_unit' => 'year',
    ),
    'misc' => array(
      'controls' => '0',
    ),
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'timeline/nodes');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'weight' => 0,
  ));
  $views[$view->name] = $view;

  return $views;
}