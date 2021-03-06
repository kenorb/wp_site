<?php
// $Id: domain_views.views.inc,v 1.6 2008/11/06 16:32:07 agentken Exp $

/**
 * @file
 * Provides the views data and handlers for domain.module.
 *
 * Original code by mrichar1.  Port to Drupal 6 by nonsie.
 *
 * @ingroup domain_views
 */

/**
 * Implements hook_views_data().
 */
function domain_views_views_data() {
  $data = array();
  // Define the {domain} table.
  $data['domain']['table']['group'] = t('Domain');
  // We can use this as a base table for Views.
  $data['domain']['table']['base'] = array(
    'field' => 'domain_id',
    'title' => t('Domain'),
    'help' => t('Domain Access uses domains to divide and display content on a group of related sites.'),
  );
  // Define the joins.
  $data['domain']['table']['join']['node'] = array(
    'left_table' => 'domain_access',
    'left_field' => 'gid',
    'field' => 'domain_id',
    'type' => 'INNER',
    'extra' => "domain_access.realm = 'domain_id'",
  );
  // Define the fields.
  $data['domain']['domain_id'] = array(
    'title' => t('Domain ID'),
    'help' => t('The unique id key for the active domain.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'domain_views_handler_filter_domain_domain_id',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['domain']['subdomain'] = array(
    'title' => t('Domain'),
    'help' => t('The URL string that identifies a domain.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['domain']['sitename'] = array(
    'title' => t('Site name'),
    'help' => t('The display string that identifies a domain.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['domain']['scheme'] = array(
    'title' => t('Scheme'),
    'help' => t('The http protocol used by the domain.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['domain']['valid'] = array(
    'title' => t('Valid'),
    'help' => t('A boolean flag indicating the domain can be seen by site visitors.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Valid'),
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Define the {domain_access} table.
  $data['domain_access']['table']['group'] = t('Domain');
  // Define the joins.
  $data['domain_access']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );
  // Define the fields.
  $data['domain_access']['nid'] = array(
    'title' => t('Node ID'),
    'help' => t('The node id.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'title' => t('Domain node'),
      'help' => t("Bring in information about the domain this node is assigned to."),
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Domain node'),
    ),
  );
  // Define the fields.
  $data['domain_access']['gid'] = array(
    'title' => t('Group ID'),
    'help' => t('The group id of the record.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'domain_views_handler_filter_domain_access_gid',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Define the fields.
  $data['domain_access']['realm'] = array(
    'title' => t('Access Type'),
    'help' => t('The domain realm.'),
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a domain_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Information for sorting on a domain_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
   // Define available on current domain field
  $data['domain_access']['current_all'] = array(
    'title' => t('Available on current domain'),
    'help' => t('Filters out nodes not available on current domain (published to current domain or all affiliates'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'field' => 'gid',
      'handler' => 'domain_views_handler_filter_domain_access_current_all',
      'label' => t('Available on current domain'),
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers() to register all of the basic handlers
 * views uses.
 */
function domain_views_views_handlers() {
  $handlers = array(
    'info' => array(
    'path' => drupal_get_path('module', 'domain_views') .'/includes',
    ),
    'handlers' => array(
    // filter handlers
    'domain_views_handler_filter_domain_access_current_all' => array(
        'parent' => 'views_handler_filter_boolean_operator',
    ),
    'domain_views_handler_filter_domain_access_gid' => array(
      'parent' => 'views_handler_filter_in_operator',
    ),
    'domain_views_handler_filter_domain_domain_id' => array(
      'parent' => 'views_handler_filter_in_operator',
    ),
  ),
  );
  return $handlers;
}

/**
 * Implementation of hook_views_query_substitutions ()
 */
function domain_views_query_substitutions($view) {
  return array('***CURRENT_DOMAIN***' => $GLOBALS['_domain']['domain_id']);
}

