<?php
// $Id: gallery_service_base.inc,v 1.1 2008/03/15 18:41:54 profix898 Exp $

define('GALLERY_SERVICE_API', 1);

/**
 * Function gallery_service_base_service().
 */
function gallery_service_base_service() {
  return array(
    // gallery.get_api_version
    array(
      '#method'   => 'gallery.get_api_version',
      '#callback' => 'gallery_service_base_get_api_version',
      '#auth'     => false,
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Returns the gallery module API version.')
    ),
    // gallery.gallery_version
    array(
      '#method'   => 'gallery.gallery_version',
      '#callback' => 'gallery_version',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Returns an array containing the version information of GalleryCoreApi and GalleryEmbed.')
    ),
    // gallery.gallery_get_themes
    array(
      '#method'   => 'gallery.gallery_get_themes',
      '#callback' => 'gallery_get_themes',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Retrieves a list of available themes.')
    ),
    // gallery.gallery_get_image_frames
    array(
      '#method'   => 'gallery.gallery_get_image_frames',
      '#callback' => 'gallery_get_image_frames',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Retrieves a list of available image frames.')
    ),
    // gallery.gallery_check_path2id
    array(
      '#method'   => 'gallery.gallery_check_path2id',
      '#callback' => 'gallery_check_path2id',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'path',
          '#type'         => 'string',
          '#description'  => t('An internal Drupal path (or G2 path fragment)')
        )
      ),
      '#help'     => t('Verifies the path existing and returns the associated entity id.')
    ),
    // gallery.gallery_generate_url
    array(
      '#method'   => 'gallery.gallery_generate_url',
      '#callback' => 'gallery_generate_url',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'params',
          '#type'         => 'array',
          '#description'  => t('An array of parameters, i.e. the itemId, etc.')
        ),
        array(
          '#name'         => 'html',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('Escape html entities')
        ),
        array(
          '#name'         => 'full',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('Force absolute url')
        )
      ),
      '#help'     => t('Returns a embedded G2 url based on the provided parameters.')
    ),
    // gallery.gallery_album_tree
    array(
      '#method'   => 'gallery.gallery_album_tree',
      '#callback' => 'gallery_album_tree',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'root',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('Album id of the album tree root to fetch')
        ),
        array(
          '#name'         => 'depth',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('Depth of album tree')
        ),
        array(
          '#name'         => 'uid',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('Visible to uid only (instead of current user)')
        )
      ),
      '#help'     => t('Returns the album tree (of specified depth and user id).')
    ),
    // gallery.gallery_item_details
    array(
      '#method'   => 'gallery.gallery_item_details',
      '#callback' => 'gallery_item_details',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'id',
          '#type'         => 'int',
          '#description'  => t('Entity id to fetch details for')
        ),
        array(
          '#name'         => 'verbose',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('Return a larger amount of details')
        )
      ),
      '#help'     => t('Returns details of the specified item.')
    ),
    // gallery.gallery_item_access
    array(
      '#method'   => 'gallery.gallery_item_access',
      '#callback' => 'gallery_item_access',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'id',
          '#type'         => 'int',
          '#description'  => t('Entity id to check access to')
        ),
        array(
          '#name'         => 'permission',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('Array of permissions to check for')
        ),
        array(
          '#name'         => 'user',
          '#type'         => 'string',
          '#optional'     => TRUE,
          '#description'  => t('Accessible to this user (instead of current user)')
        )
      ),
      '#help'     => t('Checks the access permissions for an item.')
    ),
    // gallery.gallery_db_query
    /*array(
      '#method'   => 'gallery.gallery_db_query',
      '#callback' => 'gallery_db_query',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'query',
          '#type'         => 'string',
          '#description'  => t('Query string to run against db')
        ),
        array(
          '#name'         => 'data',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('Data required for query')
        )
      ),
      '#help'     => t('Runs the query against the G2 db and returns the result(s).')
    ),*/
    // gallery.gallery_get_blocks
    array(
      '#method'   => 'gallery.gallery_get_blocks',
      '#callback' => 'gallery_get_blocks',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'plugin_names',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('List of plugins to fetch block list for')
        )
      ),
      '#help'     => t('Retrieves a list of available blocks.')
    ),
    // gallery.gallery_get_blocks
    array(
      '#method'   => 'gallery.gallery_get_block',
      '#callback' => 'gallery_get_block',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'params',
          '#type'         => 'array',
          '#description'  => t('Parameters passed to getBlock() method')
        ),
        array(
          '#name'         => 'block',
          '#type'         => 'string',
          '#optional'     => TRUE,
          '#description'  => t('Name of the block to fetch')
        ),
        array(
          '#name'         => 'extra',
          '#type'         => 'array',
          '#optional'     => TRUE,
          '#description'  => t('Additional parameters')
        )
      ),
      '#help'     => t('Fetches a G2 block, e.g. an ImageBlock.')
    ),
    // gallery.gallery_plugin_status
    array(
      '#method'   => 'gallery.gallery_plugin_status',
      '#callback' => 'gallery_plugin_status',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'plugin_names',
          '#type'         => 'array',
          '#description'  => t('List of plugins to retrieve status for')
        )
      ),
      '#help'     => t('Returns the status of the specified plugins.')
    ),
    // gallery.gallery_get_blocks
    array(
      '#method'   => 'gallery.gallery_single_plugin_status',
      '#callback' => 'gallery_single_plugin_status',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'plugin_name',
          '#type'         => 'string',
          '#description'  => t('Name of plugin to retrieve status for')
        )
      ),
      '#help'     => t('Returns the status of a single plugin.')
    ),
    // gallery.gallery_get_status
    array(
      '#method'   => 'gallery.gallery_get_status',
      '#callback' => 'gallery_get_status',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Returns the gallery module status array.')
    ),
    // gallery.gallery_search
    array(
      '#method'   => 'gallery.gallery_search',
      '#callback' => 'gallery_service_base_search',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'keys',
          '#type'         => 'string',
          '#description'  => t('Comma-separated list of keys to search for')
        ),
        array(
          '#name'         => 'offset',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('Offset in matches to return')
        ),
        array(
          '#name'         => 'limit',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('Max. number of matches to return')
        )
      ),
      '#help'     => t('Searches Gallery 2 for keys and returns the results.')
    ),
    // gallery.gallery_filter
    array(
      '#method'   => 'gallery.gallery_filter',
      '#callback' => 'gallery_service_base_filter',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'text',
          '#type'         => 'string',
          '#description'  => t('Text to be processed')
        )
      ),
      '#help'     => t('Runs the Gallery2 input filter on a piece of text.')
    ),
    // gallery.gallery_xmlsitemap
    array(
      '#method'   => 'gallery.gallery_xmlsitemap',
      '#callback' => 'gallery_service_base_xmlsitemap',
      '#return'   => 'struct',
      '#args'     => array(),
      '#help'     => t('Retrieves a xml sitemap from Gallery2.')
    ),
    // gallery.gallery_user_useralbum
    array(
      '#method'   => 'gallery.gallery_user_useralbum',
      '#callback' => 'gallery_service_base_user_useralbum',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'uid',
          '#type'         => 'int',
          '#optional'     => TRUE,
          '#description'  => t('User id to get useralbum for (instead of current user)')
        ),
        array(
          '#name'         => 'link',
          '#type'         => 'boolean',
          '#optional'     => TRUE,
          '#description'  => t('Return a link to the useralbum instead of the album id')
        )
      ),
      '#help'     => t('Gets the album id or url of a user\'s user album.')
    ),
    // gallery.gallery_user_map_status
    array(
      '#method'   => 'gallery.gallery_user_map_status',
      '#callback' => 'gallery_service_base_user_map_status',
      '#return'   => 'struct',
      '#args'     => array(
        array(
          '#name'         => 'uid',
          '#type'         => 'int',
          '#description'  => t('User id to fetch map status for (instead of current user)')
        )
      ),
      '#help'     => t('Returns the user map status for the specified user.')
    )
  );
}

/* Services wrappers for basic Drupal-embedded Gallery2 features */

/**
 * Function gallery_service_base_get_api_version().
 */
function gallery_service_base_get_api_version() {
  return array(GALLERY_BASE_API, GALLERY_SERVICE_API);
}

/**
 * Function gallery_service_base_search().
 */
function gallery_service_base_search($keys, $offset = 0, $limit = -1) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_search.inc');
  return _gallery_search_perform($keys, $offset, $limit);
}

/**
 * Function gallery_service_base_filter().
 */
function gallery_service_base_filter($text) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_filter.inc');
  return gallery_filter_process($text);
}

/**
 * Function gallery_service_base_xmlsitemap().
 */
function gallery_service_base_xmlsitemap() {
  return gallery_xmlsitemap_links('xml', array());
}

/**
 * Function gallery_service_base_user_useralbum().
 */
function gallery_service_base_user_useralbum($uid = NULL, $link = TRUE) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_user.inc');
  return gallery_user_useralbum($uid, $link);
}

/**
 * Function gallery_service_base_user_map_status().
 */
function gallery_service_base_user_map_status($uid) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_user.inc');
  $user = user_load(array('uid' => $uid));
  $userinfo = gallery_user_map_info($user, TRUE);
  
  return gallery_user_map_info_status($userinfo, TRUE);
}
