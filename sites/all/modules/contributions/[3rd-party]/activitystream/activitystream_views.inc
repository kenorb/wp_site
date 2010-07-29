<?php
function _activitystream_views_default_views() {
  $view = new stdClass();
  $view->name = 'activity_stream_delicious';
  $view->description = t('Displays the activity stream for Delicious only.');
  $view->access = array();
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Delicious Activity';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'activitystream';
  $view->url = 'users/$arg/delicious';
  $view->use_pager = FALSE;
  $view->nodes_per_page = '50';
  $view->sort = array (
  );
  $view->argument = array (
    array (
      'type' => 'uid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'activitystream',
      ),
    ),
    array (
      'tablename' => 'activitystream',
      'field' => 'module',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'activitystream_delicious',
      ),
    ),
  );
  $view->exposed_filter = array();
  $view->requires = array('node', 'activitystream');
  $views[$view->name] = $view;

  $view = new stdClass();
  $view->name = 'activity_stream_digg';
  $view->description = t('Displays the activity stream for Digg only.');
  $view->access = array();
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Digg Activity';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'activitystream';
  $view->url = 'users/$arg/digg';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->sort = array();
  $view->argument = array (
    array (
      'type' => 'uid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'activitystream',
      ),
    ),
    array (
      'tablename' => 'activitystream',
      'field' => 'module',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'activitystream_digg',
      ),
    ),
  );
  $view->exposed_filter = array();
  $view->requires = array('node', 'activitystream');
  $views[$view->name] = $view;

  $view = new stdClass();
  $view->name = 'activity_stream_twitter';
  $view->description = t('Displays the activity stream for Twitter only.');
  $view->access = array();
  $view->view_args_php = '';
  $view->page = TRUE;
  $view->page_title = 'Twitter Activity';
  $view->page_header = '';
  $view->page_header_format = '1';
  $view->page_footer = '';
  $view->page_footer_format = '1';
  $view->page_empty = '';
  $view->page_empty_format = '1';
  $view->page_type = 'activitystream';
  $view->url = 'users/$arg/twitter';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '50';
  $view->sort = array();
  $view->argument = array (
    array (
      'type' => 'uid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
      'wildcard' => '',
      'wildcard_substitution' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => '',
      'handler' => 'views_handler_field_nodelink',
      'options' => 'link',
    ),
    array (
      'tablename' => 'node',
      'field' => 'created',
      'label' => '',
      'handler' => 'views_handler_field_since',
    ),
    array (
      'tablename' => 'activitystream',
      'field' => 'data',
      'label' => '',
      'handler' => 'activitystream_views_handler_field_feed_icon',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'status',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'type',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'activitystream',
      ),
    ),
    array (
      'tablename' => 'activitystream',
      'field' => 'module',
      'operator' => 'OR',
      'options' => '',
      'value' => array (
        0 => 'activitystream_twitter',
      ),
    ),
  );
  $view->exposed_filter = array();
  $view->requires = array('node', 'activitystream');
  $views[$view->name] = $view;

  return $views;
}

function _activitystream_views_tables() {
  $tables = array();
  $tables['activitystream'] = array(
    'name' => 'activitystream',
    'provider' => 'internal',
    'join' => array(
      'left' => array(
        'table' => 'node',
        'field' => 'nid'
      ),
      'right' => array(
        'table' => 'activitystream',
        'field' => 'nid'
      )
    ),
    'fields' => array(
      'source' => array(
        'name' => t('Activity Stream: Source'),
        'handler' => array(
          'views_handler_field_profile_default' => t('Source')
        )
      ),
      'data' => array(
        'name' => t('Activity Stream: Icon'),
        'handler' => array(
          'activitystream_views_handler_field_icon' => t('Icon')
        )
      ),
      'link' => array(
        'name' => t('Activity Stream: Link'),
        'handler' => array(
          'activitystream_views_handler_field_link' => t('Link')
        )
      )
    ),
    'filters' => array(
      'module' => array(
        'name' => t('Activity Stream: Type'),
        'list' => '_activitystream_views_handler_filter_type',
        'list-type' => 'list',
        'operator' => 'views_handler_operator_or',
        'value-type' => 'array',
        'help' => t('Include or exclude streams of the selected types.')
      )
    )
  );
  
  return $tables;
}