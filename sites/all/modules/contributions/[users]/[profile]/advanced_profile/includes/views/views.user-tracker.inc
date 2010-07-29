<?php
// $Id: views.user-tracker.inc,v 1.1.4.5 2009/01/05 05:19:42 michellec Exp $

/**
 * @file
 * Holds user tracker view export.
 */

$view = new view;
$view->name = 'apk_user_tracker';
$view->description = 'Shows topics by or commented on by a given user.';
$view->tag = 'APK';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'label' => 'Title',
    'link_to_node' => TRUE,
  ),
  'comment_count' => array(
    'id' => 'comment_count',
    'table' => 'node_comment_statistics',
    'field' => 'comment_count',
    'label' => 'Replies',
    'set_precision' => FALSE,
    'precision' => 0,
    'decimal' => '.',
    'separator' => ',',
    'prefix' => '',
    'suffix' => '',
  ),
  'last_comment_timestamp' => array(
    'id' => 'last_comment_timestamp',
    'table' => 'node_comment_statistics',
    'field' => 'last_comment_timestamp',
    'label' => 'Last Post',
    'date_format' => 'small',
    'custom_date_format' => '',
  ),
  'timestamp' => array(
    'id' => 'timestamp',
    'table' => 'history_user',
    'field' => 'timestamp',
    'label' => '',
    'comments' => 1,
    'relationship' => 'none',
    'link_to_node' => 0,
    'comment' => 1,
  ),
  'new_comments' => array(
    'id' => 'new_comments',
    'table' => 'node',
    'field' => 'new_comments',
    'label' => '',
    'set_precision' => FALSE,
    'precision' => 0,
    'decimal' => '.',
    'separator' => ',',
    'prefix' => '',
    'suffix' => ' new',
    'link_to_comment' => 1,
    'no_empty' => 1,
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'last_comment_timestamp' => array(
    'id' => 'last_comment_timestamp',
    'table' => 'node_comment_statistics',
    'field' => 'last_comment_timestamp',
    'order' => 'ASC',
    'granularity' => 'second',
  ),
));
$handler->override_option('arguments', array(
  'uid_touch' => array(
    'default_action' => 'empty',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => 'Topics posted in by %1',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'not found',
    'id' => 'uid_touch',
    'table' => 'node',
    'field' => 'uid_touch',
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'blog' => 0,
      'poll' => 0,
      'forum' => 0,
      'group' => 0,
      'page' => 0,
      'profile' => 0,
      'story' => 0,
      'uprofile' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      '1' => 0,
      '2' => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_is_member' => 0,
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'status' => array(
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'operator' => '=',
    'value' => '1',
    'group' => 0,
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'status' => array(
      'id' => 'status',
      'table' => 'comments',
      'field' => 'status',
      'operator' => '=',
      'value' => 0,
      'group' => 0,
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'relationship' => 'none',
    ),
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
  'role' => array(),
  'perm' => '',
));
$handler->override_option('title', 'Topics I\'ve participated in');
$handler->override_option('empty', 'No posts.');
$handler->override_option('empty_format', '1');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('items_per_page', '25');
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'override' => 1,
  'order' => 'desc',
  'columns' => array(
    'type' => 'type',
    'title' => 'title',
    'name' => 'name',
    'comment_count' => 'comment_count',
    'last_comment_timestamp' => 'last_comment_timestamp',
    'timestamp' => 'title',
    'new_comments' => 'comment_count',
  ),
  'info' => array(
    'type' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'separator' => '&nbsp;',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'comment_count' => array(
      'sortable' => 1,
      'separator' => '<br />',
    ),
    'last_comment_timestamp' => array(
      'sortable' => 1,
      'separator' => '&nbsp;',
    ),
    'timestamp' => array(
      'separator' => '',
    ),
    'new_comments' => array(
      'separator' => '',
    ),
  ),
  'default' => 'last_comment_timestamp',
));
$handler = $view->new_display('panel_pane', 'Panel pane', 'panel_pane_1');
$handler->override_option('items_per_page', 10);
$handler->override_option('use_pager', TRUE);
$handler->override_option('pane_title', '');
$handler->override_option('pane_description', '');
$handler->override_option('pane_category', array(
  'name' => 'Advanced Profile Kit',
  'weight' => '0',
));
$handler->override_option('allow', array(
  'use_pager' => 'use_pager',
  'items_per_page' => 'items_per_page',
  'offset' => 0,
  'link_to_view' => 0,
  'more_link' => 'more_link',
  'path_override' => 0,
));
$handler->override_option('argument_input', array(
  'uid_touch' => array(
    'type' => 'context',
    'context' => 'user',
    'panel' => '0',
    'fixed' => '',
    'label' => 'Node: User posted or commented',
  ),
));
$handler->override_option('link_to_view', 0);
$handler->override_option('inherit_panels_path', 0);
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->override_option('items_per_page', 10);
$handler->override_option('use_pager', 'mini');
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

$views[$view->name] = $view;

