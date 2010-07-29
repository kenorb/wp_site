<?php
// $Id: gallery_service_core.inc,v 1.1 2008/03/15 18:41:54 profix898 Exp $

/**
 * Function gallery_service_core_service().
 */
function gallery_service_core_service() {
  return array(
    // gallery.core.get_api_version
    array(
      '#method'   => 'gallery.core.get_api_version',
      '#callback' => 'gallery_service_core_get_api_version',
      '#auth'     => false,
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Returns the API version.')
    ),
    // gallery.core.get_plugin_parameter
    array(
      '#method'   => 'gallery.core.get_plugin_parameter',
      '#callback' => 'gallery_service_core_get_plugin_parameter',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'pluginType',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'pluginId',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'parameterName',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Retrieves a plugin parameter.')
    ),
    // gallery.core.fetch_all_plugin_parameters
    array(
      '#method'   => 'gallery.core.fetch_all_plugin_parameters',
      '#callback' => 'gallery_service_core_fetch_all_plugin_parameters',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'pluginType',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'pluginId',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns all the parameters for a plugin.')
    ),
    // gallery.core.fetch_plugin_status
    array(
      '#method'   => 'gallery.core.fetch_plugin_status',
      '#callback' => 'gallery_service_core_fetch_plugin_status',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'pluginType',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the status of all plugins of a given type.')
    ),
    // gallery.core.fetch_plugin_list
    array(
      '#method'   => 'gallery.core.fetch_plugin_list',
      '#callback' => 'gallery_service_core_fetch_plugin_list',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'pluginType',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns a plugin list by plugin type of all installed plugins.')
    ),
    // gallery.core.fetch_access_list_ids
    array(
      '#method'   => 'gallery.core.fetch_access_list_ids',
      '#callback' => 'gallery_service_core_fetch_access_list_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'permission',
          '#type'         => 'array',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'userId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'sessionPermissions',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches all the access list ids that grant the given permission to the given user.')
    ),
    // gallery.core.fetch_access_list_id
    array(
      '#method'   => 'gallery.core.fetch_access_list_id',
      '#callback' => 'gallery_service_core_fetch_access_list_id',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns true if the current user has the specific permission for the target item.')
    ),
    // gallery.core.assert_has_item_permission
    array(
      '#method'   => 'gallery.core.assert_has_item_permission',
      '#callback' => 'gallery_service_core_assert_has_item_permission',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Asserts that the current user has the specific permission for the target item.')
    ),
    // gallery.core.has_item_permission
    array(
      '#method'   => 'gallery.core.has_item_permission',
      '#callback' => 'gallery_service_core_has_item_permission',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'userId',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'sessionPermissions',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns true if the current user has the specific permission for the target item.')
    ),
    // gallery.core.assert_user_is_administrator
    array(
      '#method'   => 'gallery.core.assert_user_is_administrator',
      '#callback' => 'gallery_service_core_assert_user_is_administrator',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Asserts that the active user is a site administrator.')
    ),
    // gallery.core.fetch_permissions_for_items
    array(
      '#method'   => 'gallery.core.fetch_permissions_for_items',
      '#callback' => 'gallery_service_core_fetch_permissions_for_items',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemIds',
          '#type'         => 'array',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'userId',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'sessionPermissions',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns a list of permissions for the given items.')
    ),
    // gallery.core.get_permissions
    array(
      '#method'   => 'gallery.core.get_permissions',
      '#callback' => 'gallery_service_core_get_permissions',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'userId',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'sessionPermissions',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns all the permissions that the given user has for the given item.')
    ),
    // gallery.core.fetch_all_permissions_for_item
    array(
      '#method'   => 'gallery.core.fetch_all_permissions_for_item',
      '#callback' => 'gallery_service_core_fetch_all_permissions_for_item',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns a list of permissions for the given item id.')
    ),
    // gallery.core.fetch_preferred_source
    array(
      '#method'   => 'gallery.core.fetch_preferred_source',
      '#callback' => 'gallery_service_core_fetch_preferred_source',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the thumbnail for an item id.')
    ),
    // gallery.core.fetch_thumbnails_by_item_ids
    array(
      '#method'   => 'gallery.core.fetch_thumbnails_by_item_ids',
      '#callback' => 'gallery_service_core_fetch_thumbnails_by_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ids',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the thumbnail for an item id.')
    ),
    // gallery.core.fetch_preferreds_by_item_ids
    array(
      '#method'   => 'gallery.core.fetch_preferreds_by_item_ids',
      '#callback' => 'gallery_service_core_fetch_preferreds_by_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ids',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the preferred for an item id.')
    ),
    // gallery.core.fetch_resizes_by_item_ids
    array(
      '#method'   => 'gallery.core.fetch_resizes_by_item_ids',
      '#callback' => 'gallery_service_core_fetch_resizes_by_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ids',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the resizes for an item id.')
    ),
    // gallery.core.fetch_derivatives_by_source_ids
    array(
      '#method'   => 'gallery.core.fetch_derivatives_by_source_ids',
      '#callback' => 'gallery_service_core_fetch_derivatives_by_source_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ids',
          '#type'         => 'array',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'types',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Loads the derivative images that have the specified source id(s) with the type(s) specified.')
    ),
    // gallery.core.fetch_derivatives_by_item_ids
    array(
      '#method'   => 'gallery.core.fetch_derivatives_by_item_ids',
      '#callback' => 'gallery_service_core_fetch_derivatives_by_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ids',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches all derivatives for a given item id.')
    ),
    // gallery.core.update_modification_timestamp
    array(
      '#method'   => 'gallery.core.update_modification_timestamp',
      '#callback' => 'gallery_service_core_update_modification_timestamp',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'entityId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Sets modification timestamp for the given entity id to the current time.')
    ),
    // gallery.core.increment_item_view_count
    array(
      '#method'   => 'gallery.core.increment_item_view_count',
      '#callback' => 'gallery_service_core_increment_item_view_count',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'step',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Updates the view count for this item id.')
    ),
    // gallery.core.set_item_view_count
    array(
      '#method'   => 'gallery.core.set_item_view_count',
      '#callback' => 'gallery_service_core_set_item_view_count',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'count',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Sets the view count for this item id.')
    ),
    // gallery.core.fetch_item_view_count
    array(
      '#method'   => 'gallery.core.fetch_item_view_count',
      '#callback' => 'gallery_service_core_fetch_item_view_count',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the view count for this item id.')
    ),
    // gallery.core.fetch_item_view_counts
    array(
      '#method'   => 'gallery.core.fetch_item_view_counts',
      '#callback' => 'gallery_service_core_fetch_item_view_counts',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemIds',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the view counts for multiple item ids.')
    ),
    // gallery.core.fetch_parent_sequence
    array(
      '#method'   => 'gallery.core.fetch_parent_sequence',
      '#callback' => 'gallery_service_core_fetch_parent_sequence',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the parent sequence for this item id.')
    ),
    // gallery.core.fetch_theme_id
    array(
      '#method'   => 'gallery.core.fetch_theme_id',
      '#callback' => 'gallery_service_core_fetch_theme_id',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the correct theme for this item.')
    ),
    // gallery.core.fetch_child_counts
    array(
      '#method'   => 'gallery.core.fetch_child_counts',
      '#callback' => 'gallery_service_core_fetch_child_counts',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemIds',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Return the number of children for the items specified.')
    ),
    // gallery.core.fetch_descendents_counts
    array(
      '#method'   => 'gallery.core.fetch_descendents_counts',
      '#callback' => 'gallery_service_core_fetch_descendents_counts',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemIds',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Return the number of descendents for the items specified.')
    ),
    // gallery.core.fetch_all_item_ids
    array(
      '#method'   => 'gallery.core.fetch_all_item_ids',
      '#callback' => 'gallery_service_core_fetch_all_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemType',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Return the number of descendents for the items specified.')
    ),
    // gallery.core.fetch_all_item_ids_by_owner_id
    array(
      '#method'   => 'gallery.core.fetch_all_item_ids_by_owner_id',
      '#callback' => 'gallery_service_core_fetch_all_item_ids_by_owner_id',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ownerId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Return the number of descendents for the items specified.')
    ),
    // gallery.core.fetch_album_tree
    array(
      '#method'   => 'gallery.core.fetch_album_tree',
      '#callback' => 'gallery_service_core_fetch_album_tree',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'itemId',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'depth',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Fetches the album tree visible to current user.')
    ),
    // gallery.core.fetch_item_id_by_path
    array(
      '#method'   => 'gallery.core.fetch_item_id_by_path',
      '#callback' => 'gallery_service_core_fetch_item_id_by_path',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'path',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Given a complete logical path, returns the item id that it refers to.')
    ),
    // gallery.core.load_entities_by_id
    array(
      '#method'   => 'gallery.core.load_entities_by_id',
      '#callback' => 'gallery_service_core_load_entities_by_id',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'ids',
          '#type'         => 'array',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Loads the GalleryEntities with the ids specified.')
    ),
    // gallery.core.load_entities_by_external_id
    array(
      '#method'   => 'gallery.core.load_entities_by_external_id',
      '#callback' => 'gallery_service_core_load_entities_by_external_id',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'externalId',
          '#type'         => 'int',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'entityType',
          '#type'         => 'string',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Maps external id to G2 id and then loads the entity.')
    ),
    // gallery.core.fetch_descendent_item_ids
    array(
      '#method'   => 'gallery.core.fetch_descendent_item_ids',
      '#callback' => 'gallery_service_core_fetch_descendent_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'offset',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'count',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'string',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the ids of the descendents of this entity.')
    ),
    // gallery.core.fetch_child_item_ids
    array(
      '#method'   => 'gallery.core.fetch_child_item_ids',
      '#callback' => 'gallery_service_core_fetch_child_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'offset',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'count',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the ids of the children of this entity.')
    ),
    // gallery.core.fetch_descendent_album_item_ids
    array(
      '#method'   => 'gallery.core.fetch_descendent_album_item_ids',
      '#callback' => 'gallery_service_core_fetch_descendent_album_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'offset',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'count',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'string',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the album ids of the descendents of this entity.')
    ),
    // gallery.core.fetch_child_album_item_ids
    array(
      '#method'   => 'gallery.core.fetch_child_album_item_ids',
      '#callback' => 'gallery_service_core_fetch_child_album_item_ids',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'offset',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'count',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Returns the album ids of the children of this entity.')
    ),
    // gallery.core.fetch_parents
    array(
      '#method'   => 'gallery.core.fetch_parents',
      '#callback' => 'gallery_service_core_fetch_parents',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'item',
          '#type'         => 'struct',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('')
        ),
        array(
          '#name'         => 'filterBreadcrumb',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('')
        )
      ),
      '#help'     => t('Loads all the ancestors of this item.')
    ),
    // gallery.core.build_item_query
    array(
      '#method'   => 'gallery.core.build_item_query',
      '#callback' => 'gallery_service_core_build_item_query',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'baseTable',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'baseIdColumn',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'baseQuery',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'orderBy',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'orderDirection',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'class',
          '#type'         => 'string',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'requiredPermission',
          '#type'         => 'array',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'linkableOnly',
          '#type'         => 'boolean',
          '#description'  => t('')
        ),
        array(
          '#name'         => 'userId',
          '#type'         => 'int',
          '#description'  => t('')
        )
      ),
      '#help'     => t('Builds the query to select items in a given sort order.')
    ),
    // gallery.core.get_default_album_id
    array(
      '#method'   => 'gallery.core.get_default_album_id',
      '#callback' => 'gallery_service_core_get_default_album_id',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Returns the id of the album to display by default.')
    )
  );
}

/* Services wrappers for GalleryCoreApi methods */

/**
 * Function gallery_service_core_get_api_version().
 */
function gallery_service_core_get_api_version() {
  if (!_gallery_init()) {
    return array();
  }
  
  return GalleryCoreApi::getApiVersion();
}

/**
 * Function gallery_service_core_get_plugin_parameter().
 */
function gallery_service_core_get_plugin_parameter($pluginType, $pluginId, $parameterName, $itemId = 0) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::getPluginParameter($pluginType, $pluginId, $parameterName, $itemId);
}

/**
 * Function gallery_service_core_fetch_all_plugin_parameters().
 */
function gallery_service_core_fetch_all_plugin_parameters($pluginType, $pluginId, $itemId = 0) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAllPluginParameters($pluginType, $pluginId, $itemId);
}

/**
 * Function gallery_service_core_fetch_plugin_status().
 */
function gallery_service_core_fetch_plugin_status($pluginType) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchPluginStatus($pluginType);
}

/**
 * Function gallery_service_core_fetch_plugin_list().
 */
function gallery_service_core_fetch_plugin_list($pluginType) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchPluginList($pluginType);
}

/**
 * Function gallery_service_core_fetch_access_list_ids().
 */
function gallery_service_core_fetch_access_list_ids($permission, $userId, $sessionPermissions = TRUE) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAccessListIds($permission, $userId, $sessionPermissions);
}

/**
 * Function gallery_service_core_fetch_access_list_id().
 */
function gallery_service_core_fetch_access_list_id($itemId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAccessListId($itemId);
}

/**
 * Function gallery_service_core_assert_has_item_permission().
 */
function gallery_service_core_assert_has_item_permission($itemId, $permission) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::assertHasItemPermission($itemId, $permission);
}

/**
 * Function gallery_service_core_has_item_permission().
 */
function gallery_service_core_has_item_permission($itemId, $permission, $userId = NULL, $sessionPermissions = TRUE) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::hasItemPermission($itemId, $permission, $userId, $sessionPermissions);
}

/**
 * Function gallery_service_core_assert_user_is_administrator().
 */
function gallery_service_core_assert_user_is_administrator() {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::assertUserIsSiteAdministrator();
}

/**
 * Function gallery_service_core_fetch_permissions_for_items().
 */
function gallery_service_core_fetch_permissions_for_items($itemId, $userId = NULL, $sessionPermissions = TRUE) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchPermissionsForItems($itemId, $userId, $sessionPermissions);
}

/**
 * Function gallery_service_core_get_permissions().
 */
function gallery_service_core_get_permissions($itemId, $userId = NULL, $sessionPermissions = TRUE) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::getPermissions($itemId, $userId, $sessionPermissions);
}

/**
 * Function gallery_service_core_fetch_all_permissions_for_item().
 */
function gallery_service_core_fetch_all_permissions_for_item($itemId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAllPermissionsForItem($itemId);
}

/**
 * Function gallery_service_core_fetch_preferred_source().
 */
function gallery_service_core_fetch_preferred_source($item) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchPreferredSource($item);
}

/**
 * Function gallery_service_core_fetch_thumbnails_by_item_ids().
 */
function gallery_service_core_fetch_thumbnails_by_item_ids($ids) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchThumbnailsByItemIds($ids);
}

/**
 * Function gallery_service_core_fetch_preferreds_by_item_ids().
 */
function gallery_service_core_fetch_preferreds_by_item_ids($ids) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchPreferredsByItemIds($ids);
}

/**
 * Function gallery_service_core_fetch_resizes_by_item_ids().
 */
function gallery_service_core_fetch_resizes_by_item_ids($ids) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchResizesByItemIds($ids);
}

/**
 * Function gallery_service_core_fetch_derivatives_by_source_ids().
 */
function gallery_service_core_fetch_derivatives_by_source_ids($ids, $types = array()) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchDerivativesBySourceIds($ids, $types);
}

/**
 * Function gallery_service_core_fetch_derivatives_by_item_ids().
 */
function gallery_service_core_fetch_derivatives_by_item_ids($ids) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchDerivativesByItemIds($ids);
}

/**
 * Function gallery_service_core_update_modification_timestamp().
 */
function gallery_service_core_update_modification_timestamp($entityId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::updateModificationTimestamp($entityId);
}

/**
 * Function gallery_service_core_increment_item_view_count().
 */
function gallery_service_core_increment_item_view_count($itemId, $step = 1) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::incrementItemViewCount($itemId, $step);
}

/**
 * Function gallery_service_core_set_item_view_count().
 */
function gallery_service_core_set_item_view_count($itemId, $count) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::setItemViewCount($itemId, $count);
}

/**
 * Function gallery_service_core_fetch_item_view_count().
 */
function gallery_service_core_fetch_item_view_count($itemId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchItemViewCount($itemId);
}

/**
 * Function gallery_service_core_fetch_item_view_counts().
 */
function gallery_service_core_fetch_item_view_counts($itemIds) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchItemViewCounts($itemIds);
}

/**
 * Function gallery_service_core_fetch_parent_sequence().
 */
function gallery_service_core_fetch_parent_sequence($itemId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchParentSequence($itemId);
}

/**
 * Function gallery_service_core_fetch_theme_id().
 */
function gallery_service_core_fetch_theme_id($item) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchThemeId($item);
}

/**
 * Function gallery_service_core_fetch_child_counts().
 */
function gallery_service_core_fetch_child_counts($itemIds) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchChildCounts($itemIds);
}

/**
 * Function gallery_service_core_fetch_descendents_counts().
 */
function gallery_service_core_fetch_descendents_counts($itemIds) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchDescendentCounts($itemIds);
}

/**
 * Function gallery_service_core_fetch_all_item_ids().
 */
function gallery_service_core_fetch_all_item_ids($itemType, $permission = 'core.view') {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAllItemIds($itemType, $permission);
}

/**
 * Function gallery_service_core_fetch_all_item_ids_by_owner_id().
 */
function gallery_service_core_fetch_all_item_ids_by_owner_id($ownerId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAllItemIdsByOwnerId($ownerId);
}

/**
 * Function gallery_service_core_fetch_album_tree().
 */
function gallery_service_core_fetch_album_tree($itemId = NULL, $depth = NULL) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchAlbumTree($itemId, $depth);
}

/**
 * Function gallery_service_core_fetch_item_id_by_path().
 */
function gallery_service_core_fetch_item_id_by_path($path) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchItemIdByPath($path);
}

/**
 * Function gallery_service_core_load_entities_by_id().
 */
function gallery_service_core_load_entities_by_id($ids) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::loadEntitiesById($ids);
}

/**
 * Function gallery_service_core_load_entities_by_external_id().
 */
function gallery_service_core_load_entities_by_external_id($externalId, $entityType) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::loadEntityByExternalId($externalId, $entityType);
}

/**
 * Function gallery_service_core_fetch_descendent_item_ids().
 */
function gallery_service_core_fetch_descendent_item_ids($item, $offset = NULL, $count = 0, $permission = 'core.view') {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchDescendentItemIds($item, $offset, $count, $permission);
}

/**
 * Function gallery_service_core_fetch_child_item_ids().
 */
function gallery_service_core_fetch_child_item_ids($item, $offset = NULL, $count = 0) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchChildItemIds($item, $offset, $count);
}

/**
 * Function gallery_service_core_fetch_descendent_album_item_ids().
 */
function gallery_service_core_fetch_descendent_album_item_ids($item, $offset = NULL, $count = 0, $permission = 'core.view') {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchDescendentAlbumItemIds($item, $offset, $count, $permission);
}

/**
 * Function gallery_service_core_fetch_child_album_item_ids().
 */
function gallery_service_core_fetch_child_album_item_ids($item, $offset = NULL, $count = 0) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchChildAlbumItemIds($item, $offset, $count);
}

/**
 * Function gallery_service_core_fetch_parents().
 */
function gallery_service_core_fetch_parents($item, $permission = NULL, $filterBreadcrumb = FALSE) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::fetchParents($item, $permission, $filterBreadcrumb);
}

/**
 * Function gallery_service_core_build_item_query().
 */
function gallery_service_core_build_item_query($baseTable, $baseIdColumn, $baseQuery, $orderBy, $orderDirection, $class, $requiredPermission, $linkableOnly, $userId) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::buildItemQuery($baseTable, $baseIdColumn, $baseQuery, $orderBy, $orderDirection, $class, $requiredPermission, $linkableOnly, $userId);
}

/**
 * Function gallery_service_core_get_default_album_id().
 */
function gallery_service_core_get_default_album_id() {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  
  return GalleryCoreApi::getDefaultAlbumId();
}
