<?php
// $Id: views.user-relationships-grid.inc,v 1.1.2.2 2009/01/05 05:19:42 michellec Exp $

/**
 * @file
 * Holds user relationships grid view export.
 */

$view = new view;
$view->name = 'apk_user_relationships_grid';
$view->description = 'Shows someone\'s related users';
$view->tag = 'APK';
$view->view_php = '';
$view->base_table = 'user_relationships';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('relationships', array(
  'requestee_id' => array(
    'label' => 'requestee',
    'required' => 0,
    'id' => 'requestee_id',
    'table' => 'user_relationships',
    'field' => 'requestee_id',
    'relationship' => 'none',
  ),
  'requester_id' => array(
    'label' => 'requester',
    'required' => 0,
    'id' => 'requester_id',
    'table' => 'user_relationships',
    'field' => 'requester_id',
    'relationship' => 'none',
  ),
  'rtid' => array(
    'label' => 'rtid',
    'required' => 0,
    'id' => 'rtid',
    'table' => 'user_relationships',
    'field' => 'rtid',
    'relationship' => 'none',
  ),
));
$handler->override_option('fields', array(
  'name' => array(
    'label' => '',
    'link_to_user' => 1,
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'relationship' => 'requestee_id',
    'override' => array(
      'button' => 'Override',
    ),
  ),
  'picture' => array(
    'label' => '',
    'exclude' => 0,
    'id' => 'picture',
    'table' => 'users',
    'field' => 'picture',
    'relationship' => 'requestee_id',
  ),
));
$handler->override_option('sorts', array(
  'plural_name' => array(
    'order' => 'ASC',
    'id' => 'plural_name',
    'table' => 'user_relationship_types',
    'field' => 'plural_name',
    'relationship' => 'rtid',
  ),
));
$handler->override_option('arguments', array(
  'requester_id' => array(
    'default_action' => 'default',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'default_argument_type' => 'user',
    'default_argument' => '',
    'validate_type' => 'numeric',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'requester_id',
    'table' => 'user_relationships',
    'field' => 'requester_id',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'blog' => 0,
      'page' => 0,
      'story' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(),
    'validate_argument_type' => 'tid',
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'approved' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'approved',
    'table' => 'user_relationships',
    'field' => 'approved',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('title', 'Relationships');
$handler->override_option('items_per_page', 30);
$handler->override_option('style_plugin', 'grid');
$handler->override_option('style_options', array(
  'grouping' => '',
  'columns' => '4',
  'alignment' => 'horizontal',
));
$handler->override_option('row_options', array(
  'inline' => array(
    'name_1' => 'name_1',
  ),
  'separator' => ', ',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'relationships/%');
$handler->override_option('menu', array(
  'type' => 'normal',
  'title' => 'All Relationships',
  'weight' => '0',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler = $view->new_display('panel_pane', 'Panel pane', 'panel_pane_1');
$handler->override_option('pane_title', 'UR Grid');
$handler->override_option('pane_description', '');
$handler->override_option('pane_category', array(
  'name' => 'Advanced Profile Kit',
  'weight' => '0',
));
$handler->override_option('allow', array(
  'use_pager' => 'use_pager',
  'items_per_page' => 'items_per_page',
  'offset' => 0,
  'link_to_view' => 'link_to_view',
  'more_link' => 'more_link',
  'path_override' => 'path_override',
));
$handler->override_option('argument_input', array(
  'requester_id' => array(
    'type' => 'context',
    'context' => 'user',
    'panel' => '0',
    'fixed' => '',
    'label' => 'User relationships: Requester user',
  ),
));
$handler->override_option('link_to_view', 0);
$handler->override_option('inherit_panels_path', 0);

$views[$view->name] = $view;
