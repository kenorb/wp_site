<?php
// $Id: gallery_base.inc,v 1.8.2.19 2009/01/25 14:21:32 profix898 Exp $

/**
 * gallery.module : gallery_base.inc
 * Base functions
 */

define('GALLERY_BASE_API', 1);

define('GALLERY_ERROR_WATCHDOG',  1);
define('GALLERY_ERROR_BROWSER',   2);
define('GALLERY_ERROR_VERBOSE',   3);

define('GALLERY_SEVERITY_SUCCESS',       1);
define('GALLERY_SEVERITY_ERROR',        -1);
define('GALLERY_SEVERITY_WARNING',      -2);
define('GALLERY_SEVERITY_ADVISE',       -3);
define('GALLERY_SEVERITY_UNKNOWN',      -4);

define('GALLERY_PLUGIN_ENABLED',         1);
define('GALLERY_PLUGIN_DISABLED',        0);
define('GALLERY_PLUGIN_STATUS_UNKNOWN', -1);
define('GALLERY_PLUGIN_NOT_ACTIVE',     -2);
define('GALLERY_PLUGIN_NOT_INSTALLED',  -3);
define('GALLERY_PLUGIN_MISSING',        -4);

define('GALLERY_PLUGIN_WANTED',          1);
define('GALLERY_PLUGIN_UNWANTED',        2);
define('GALLERY_PLUGIN_DRUPAL',          3);

define('GALLERY_DEBUGTRACE', TRUE);

/**
 * Function gallery_login().
 * (login user into embedded gallery)
 */
function gallery_login() {
  _gallery_init();
}

/**
 * Function gallery_logout().
 * (end user session)
 */
function gallery_logout() {
  if (variable_get('gallery_valid', FALSE)) {
    $embed_path = variable_get('gallery_dir', './gallery2/') .'embed.php';
    if (is_readable($embed_path)) {
      require_once($embed_path);
      GalleryEmbed::logout();
    }
  }
}

/**
 * Function _gallery_init().
 * (initialize embedded gallery)
 */
function _gallery_init($full = FALSE, $vars = NULL, $report_error = TRUE) {
  global $user, $language;
  static $ready = array('half' => FALSE, 'full' => FALSE);
    
  // Initialize only once
  if ($ready[$full ? 'full' : 'half']) {
    return TRUE;
  }
  
  $init_err_msg = t('Unable to initialize embedded Gallery. You need to <a href="@link"> configure your embedded Gallery</a>.',
                     array('@link' => url('admin/settings/gallery/install')));
  
  $g2_dir = isset($vars['gallery_dir']) ? $vars['gallery_dir'] : variable_get('gallery_dir', './gallery2/');
  $g2_uri = isset($vars['gallery_uri']) ? $vars['gallery_uri'] : variable_get('gallery_uri', '/gallery2/');
  $embed_uri = isset($vars['gallery_embed_uri']) ? $vars['gallery_embed_uri'] : variable_get('gallery_embed_uri', '?q=gallery');
  
  $gallery_valid = isset($vars) ? (isset($vars['gallery_valid']) ? $vars['gallery_valid'] : TRUE) : variable_get('gallery_valid', 0);
  $uid = isset($vars) ? (isset($vars['uid']) ? $vars['uid'] : NULL) : (($user->uid > 0) ? $user->uid : NULL);
  
  // Language-prefix url rewrite
  if (function_exists('language_url_rewrite') && variable_get('language_count', 1) > 1 && !empty($language->prefix)) {
    switch (variable_get('language_negotiation', LANGUAGE_NEGOTIATION_NONE)) {
      case LANGUAGE_NEGOTIATION_PATH_DEFAULT:
        $default = language_default();
        if ($language->language == $default->language) {
          break;
        }
      case LANGUAGE_NEGOTIATION_PATH:
        $embed_uri = str_replace('index.php?q=', 'index.php?q='. $language->prefix .'/', $embed_uri);
        break;
      default:
    }
  }
  
  // Prepare array with init parameters
  $debug = variable_get('gallery_g2debug', 0) && user_access('administer site configuration');
  $params = array('g2Dir' => $g2_dir,
                  'g2Uri' => $g2_uri,
                  'embedUri' => $embed_uri,
                  'loginRedirect' => url('user/login', array('query' => drupal_get_destination(), 'absolute' => TRUE)),
                  'activeUserId' => $uid,
                  'activeLanguage' => gallery_get_language($language->language),
                  'apiVersion' => array(1, 4),
                  'debug' => $debug ? 'buffered' : FALSE,
                  'eventListeners' => array());

  // Add theme to init parameters
  if (($g2_theme = variable_get('gallery_embed_theme', 'default')) != 'default') {
    $params['themeId'] = $g2_theme;
  }
  
  // Add root album to init parameters (but only for the gallery pages)
  $g2_root = variable_get('gallery_root_album', 'default');
  if ($g2_root != 'default' && arg(0) == variable_get('gallery_base', 'gallery')) {
    $params['defaultAlbumId'] = $params['breadcrumbRootId'] = $g2_root;
  }
  
  // Allow modules to alter the init parameters
  drupal_alter('gallery_init', $params, array('fullInit' => $full, 'ready' => $ready));
  $params['fullInit'] = $full;
  
  // Debug output of init parameters
  gallery_debug($params, t('Init parameters'));
  
  // Check for existence of 'embed.php'
  if (!$gallery_valid || !is_readable($params['g2Dir'] .'embed.php')) {
    if ($report_error) {
      gallery_error($init_err_msg);
    }
    return FALSE;
  }
  
  // Initialize Gallery2
  include_once($params['g2Dir'] .'embed.php');
  $ret = GalleryEmbed::init($params);
  if ($ret) {
    if ($ret->getErrorCode() & ERROR_PLUGIN_VERSION_MISMATCH) {
      $err_msg = t('Embedding API version is incompatible.');
      if ($vars) {
        drupal_set_message($err_msg, 'error');
      }
      gallery_error($vars ? $err_msg : $init_err_msg, $ret, !$vars);
    }
    else if ($report_error) {
      gallery_error($init_err_msg, $ret, TRUE);
    }
    return FALSE;
  }
      
  // Check if initialization was successful
  if (!class_exists('GalleryEmbed') || !class_exists('GalleryCoreApi')) {
    if ($report_error) {
      gallery_error(t('Initialization seemed successful, but classes \'GalleryEmbed\'
                       and/or \'GalleryCoreApi\' are still not available.'));
    }
    return FALSE;
  }
  
  // Multiroot: 'Virtual Root' (or 'Alternate guest user') view requested
  if (isset($params['defaultAlbumId'])) {
    $GLOBALS['gallery']->setConfig('defaultAlbumId', $params['defaultAlbumId']);
  }
  if (isset($params['breadcrumbRootId'])) {
    $GLOBALS['gallery']->setConfig('breadcrumbRootId', $params['breadcrumbRootId']);
  }
  if (isset($params['anonymousUserId'])) {
    $GLOBALS['gallery']->setConfig('anonymousUserId', $params['anonymousUserId']);
  }
    
  // Register event listeners
  if (count($params['eventListeners'])) {
    require_once(drupal_get_path('module', 'gallery') .'/G2DrupalHelper.class');
    $ret = G2DrupalHelper::registerEventListeners($params['eventListeners']);
    if ($ret && $report_error) {
      gallery_error(t('Failed to register event listeners.'), $ret);
    }
  }
  
  // Session handling
  $g2_session =& $GLOBALS['gallery']->getSession();
  $g2_session->doNotUseTempId();
  
  $ready['half'] = $params['fullInit'] ? ($ready['full'] = TRUE) : TRUE;
  
  return TRUE;
}

/**
 * Function gallery_handle_request().
 * (handleRequest extension with error handling)
 */
function gallery_handle_request() {
  global $language;
  
  // Remove the language-prefix for G2
  if (!empty($_GET['g2_path']) && function_exists('language_url_rewrite') && variable_get('language_count', 1) > 1) {
    $args = explode('/', $_GET['g2_path']);
    $prefix = array_shift($args);
    if (!empty($language->prefix) && $language->prefix == $prefix) {
      $_GET['g2_path'] = implode('/', $args);
    }
  }
  
  // Handle the request
  ob_start();
  $result = GalleryEmbed::handleRequest();
  $output = ob_get_contents();
  ob_end_clean();
  
  if ($output) {
    if (!preg_match('%<h2>\sError\s</h2>%', $output)) {
      // If $output does not contain '<h2>Error</h2>', this is an AJAX/Image callback
      print $output;
      exit();
    }
    else {
      // Otherwise (on regular pages) $output means that an error occured
      if (variable_get('gallery_error_redirect', 0) && user_access('search content')) {
        drupal_set_message('The requested Gallery URL does not exist. The item may have been
                            moved or deleted. You can search for it below.', 'notice');
        drupal_goto('search/gallery');
      }
      preg_match('%<div id="giStackTrace" [^>]*>(.*?)</div>%is', $output, $matches);
      gallery_error(t('Error handling request (invalid request)<br />or the requested Gallery URL does not exist.'), $matches[1], TRUE);
      return NULL;
    }
  }
  
  // Include theme-specific css overrides (gallery-themeid.css)
  if (isset($result['themeData']['themeId'])) {
    $stylesheet = drupal_get_path('module', 'gallery') .'/gallery-'. $result['themeData']['themeId'] .'.css';
    if (file_exists($stylesheet)) {
      drupal_add_css($stylesheet);
    }
  }
  
  // Catch debug output from G2
  if (($pos = strpos($result['bodyHtml'], '<div id="gpDebug">')) !== FALSE) {
    list($result['bodyHtml'], $suffix) = explode('<div id="gpDebug">', $result['bodyHtml']);
    if ($suffix) {
      list($debug, $suffix) = explode('</div>', $suffix, 2);
      $result['bodyHtml'] .= $suffix;
      gallery_debug($debug, t('G2 Debug Buffer'), TRUE);
    }    
  }
  
  return $result;
}

/**
 * Function gallery_get_language().
 */
function gallery_get_language($lang = '', $user = NULL) {
  global $language;
  
  // Get language from $user object
  $lang = (isset($user) && $user->uid > 0 && !empty($user->language)) ? $user->language : $lang;
  // Use current global $language, if no other language is specified
  $lang = empty($lang) ? $language->language : $lang;
  // Convert certain language codes, e.g. 'esl/spa es' => 'es' or 'fr-ca' => 'fr'
  $lang = preg_replace(array('/([\w\/]+) ([a-z]{2,3})/i', '/([a-z]{2,3})-(\w+)/i'), array('${2}', '${1}'), $lang);

  return $lang;
}

/**
 * Function gallery_list_themes().
 * (retrieve all (active) themes from Gallery2)
 */
function gallery_list_themes() {
  if (!_gallery_init()) {
    return array();
  }
  // List of themes
  list($ret, $g2_themes) = GalleryCoreApi::fetchPluginStatus('theme', TRUE);
  if ($ret) {
    gallery_error(t('Error retrieving theme list'), $ret);
    return array();
  }
  
  $themes = array();
  foreach (array_keys($g2_themes) as $themeid) {
    // Only active themes
    if (!empty($g2_themes[$themeid]['active'])) {
      // Get theme details
      list($ret, $theme) = GalleryCoreApi::loadPlugin('theme', $themeid, TRUE);
      if ($ret) {
        gallery_error(t('Error getting theme (:themeid) details',
          array(':themeid' => $themeid)), $ret);
        return array();
      }
      $themes[$themeid] = $theme->getName();
    }
  }
  
  return $themes;
}

/**
 * Function gallery_list_image_frames().
 * (retrieve all image frames from Gallery2)
 */
function gallery_list_image_frames() {
  if (!_gallery_init()) {
    return array('none' => t('None'));
  }
  // List of available image frames
  list($ret, $imageframe) = GalleryCoreApi::newFactoryInstance('ImageFrameInterface_1_1');
  if ($ret || !isset($imageframe)) {
      return array('none' => t('None'));
  }
  list($ret, $list) = $imageframe->getImageFrameList();
  if ($ret) {
      return array('none' => t('None'));
  }

  return $list;
}

/**
 * Function gallery_check_path2id(). 
 */
function gallery_check_path2id($path) {
  if (!_gallery_init(TRUE)) {
    return FALSE;
  }
  // Cache the request items here to save G2 API calls
  // (the menu system invokes _load repeatedly with the same arguments) 
  static $items = NULL;
  if (!isset($items[$path])) {
    $url_generator =& $GLOBALS['gallery']->getUrlGenerator();
    if (isset($url_generator->_shortUrls)) {
      // Fetch item id for this path (to check existence) and return id
      list($ret, $id) = GalleryCoreApi::fetchItemIdByPath($path);
      $items[$path] = $ret ? FALSE : $id;
    }
    else if (is_numeric($path) && ($id = intval($path))) {
      // Fetch item by id (to check existence) and return id
      list($ret, $entity) = GalleryCoreApi::loadEntitiesById($id);
      $items[$path] = $ret ? FALSE : $id;
    }
  }
  
  return $items[$path];
}

/**
 * Function gallery_generate_url().
 */
function gallery_generate_url($params, $html = TRUE, $full = TRUE) {
  if (!_gallery_init(TRUE)) {
    return '';
  }
  // Get URL generator and generate the url
  $url_generator =& $GLOBALS['gallery']->getUrlGenerator();
  if (!$url_generator) {
    gallery_error(t('Error: UrlGenerator not available'));
    return '';
  }

  $options = array();
  $options['forceFullUrl'] = $full;
  $options['htmlEntities'] = $html;

  if (!isset($params['view'])) {
    $params['view'] = 'core.ShowItem';
  }

  return $url_generator->generateUrl($params, $options);
}

/**
 * Function gallery_album_tree().
 */
function gallery_album_tree($root = NULL, $depth = NULL, $uid = NULL) {
  if (!_gallery_init(TRUE)) {
    return array();
  }
  // If this is called for the Drupal guest user, pass in G2
  // anonymous user id (otherwise fetchAlbumTree() returns nothing)
  global $user;
  if (!$uid && !$user->uid) {
    list($ret, $uid) = GalleryCoreApi::getPluginParameter('module', 'core', 'id.anonymousUser');
    if ($ret) {
      return array();
    }
  }
  // Fetch G2 album tree
  list($ret, $tree) = GalleryCoreApi::fetchAlbumTree($root, $depth, $uid);
  if ($ret) {
    gallery_error(t('Error fetching album tree'), $ret);
    return array();
  }

  return $tree;
}

/**
 * Function gallery_item_details().
 */
function gallery_item_details($id, $verbose = FALSE) {
  $details = array();
  if (!_gallery_init(TRUE)) {
    return $details;
  }
  // Load entity
  list($ret, $entity) = GalleryCoreApi::loadEntitiesById($id);
  if ($ret) {
    gallery_error(t('Error fetching item details (entityId: :id)',
      array(':id' => $id)), $ret);
    return $details;
  }
  // Extract details
  $details['g2id'] = $id;
  $details['g2type'] = $entity->entityType;
  $details['g2owner'] = $entity->ownerId;
  $details['g2parent'] = $entity->parentId;
  // Drupal node properties (without g2 prefix)
  $details['title'] = _gallery_htmlcharsdecode($entity->title);
  $details['created'] = $entity->creationTimestamp;
  $details['changed'] = $entity->modificationTimestamp;
  // Gallery2 item properties (with g2 prefix)
  if ($verbose) {
    $details['g2description'] = _gallery_htmlcharsdecode($entity->description);
    $details['g2summary'] = _gallery_htmlcharsdecode($entity->summary);
    $details['g2keywords'] = $entity->keywords;
    $details['g2theme'] = isset($entity->theme) ? $entity->theme : NULL;
  }

  return $details;
}

/**
 * Function gallery_item_access().
 */
function gallery_item_access($id, $permission = 'core.view', $user = NULL) {
  if (!_gallery_init(TRUE) || !is_numeric($id)) {
    return FALSE;
  }
  // Check access permissions
  list($ret, $access) = GalleryCoreApi::hasItemPermission($id, $permission, $user);
  if ($ret) {
    gallery_error(t('Error calling hasItemPermission()'), $ret);
    return FALSE;
  }

  return $access;
}

/**
 * Function gallery_db_query().
 */
function gallery_db_query($query, $data = NULL) {
  if (!_gallery_init(TRUE)) {
    return FALSE;
  }
  // Perform query
  list ($ret, $search) = $GLOBALS['gallery']->search($query, $data);
  if ($ret) {
    return FALSE;
  }
  $results = array();
  while ($result = $search->nextResult()) {
    $results += $result;
  }

  return $results;
}

/**
 * Function gallery_flush_entity_cache().
 */
function gallery_flush_entity_cache() {
  if (!_gallery_init()) {
    return FALSE;
  }
  // Flush entity cache
  $platform =& $GLOBALS['gallery']->getPlatform();
  $cache_basedir = $GLOBALS['gallery']->getConfig('data.gallery.cache');
  $cache_dir = $cache_basedir .'entity';

  if ($platform->file_exists($cache_dir)) {
    if (!$platform->recursiveRmDir($cache_dir)) {
      return FALSE;
    }
  }

  return TRUE;
}

/**
 * Function gallery_set_head().
 */
function gallery_set_head($html, $settitle = FALSE) {
  if (!empty($html)) {
    list($title, $css, $javascript, $meta) = GalleryEmbed::parseHead($html);
    if ($settitle) {
      drupal_set_title($title);
    }
    gallery_set_meta($meta);
    gallery_set_css($css);
    gallery_set_javascript($javascript);
  }
}

/**
 * Function gallery_set_meta().
 */
function gallery_set_meta($meta) {
  static $meta_memory = array();
  
  // Debug output
  gallery_debug($meta, t('Head [Meta]'));
  // Drupal adds these item by default already
  if (empty($meta_memory)) {
    $meta_memory[] = md5('<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>');
  }
  // Handle head/meta data
  foreach ($meta as $include) {
    if (!in_array(($md5 = md5($include)), $meta_memory)) {
      $meta_memory[] = $md5;
      drupal_set_html_head($include);
    }
  }
}

/**
 * Function gallery_set_css().
 */
function gallery_set_css($css) {
  static $css_memory = array();
  
  if (count($css)) {
    // Debug output
    gallery_debug($css, t('Head [CSS]'));
    // Handle css styles and includes
    foreach ($css as $include) {
      if (!in_array(($md5 = md5($include)), $css_memory)) {
        $css_memory[] = $md5;
        drupal_set_html_head($include);
      }
    }
  }
}

/**
 * Function gallery_set_javascript().
 * (druapl_add_js() ensures proper cascading of included G2 javascript)  
 */
function gallery_set_javascript($javascript) {
  static $js_memory = array();
  
  if (!empty($javascript)) {
    // Debug output
    gallery_debug($javascript, t('Head [Javascript]'));
    // Handle inline javascript and includes
    $files = preg_grep('/<script(.*?)src=/i', $javascript);
    // Inline Javascript
    $inline = array_diff($javascript, $files);
    $inline = preg_replace('/<script([^>]*)>(.*?)<\/script>/is', '\2', $inline);
    drupal_add_js(implode("\n", $inline), 'inline');
    // Javascript files
    foreach ($files as $include) {
      if (!in_array(($md5 = md5($include)), $js_memory)) {
        $js_memory[] = $md5;
        drupal_set_html_head($include);
      }
    }
  }
}

/**
 * Function gallery_context().
 * (store and/or access the context of the current gallery page) 
 */
function gallery_context(&$params, $setcontext = FALSE) {
  static $context_cache = NULL;
  
  if ($setcontext) {
    // Store new context
    $context_cache = (isset($params) && count($params)) ? $params : array();
    gallery_debug($context_cache, t('Context cache'));
  }
  else if (isset($context_cache)) {
    // Get context parameters
    if (isset($params)) {
      // Return requested context parameters
      $params = is_array($params) ? $params : array($params);
      foreach ($params as $param) {
        if (!isset($context_cache[$param])) {
          return FALSE;
        }
        $context[$param] = $context_cache[$param];
      }
      $params = $context;
    }
    else {
      // Return all stored context parameters
      $params = $context_cache;
    }
    
    return TRUE;
  }
  
  return FALSE;
}

/**
 * Function gallery_list_blocks().
 */
function gallery_list_blocks($plugin_names = NULL) {
  if (!_gallery_init()) {
    return array();
  }
  // List of available blocks
  list($ret, $g2_blocks) = GalleryCoreApi::loadAvailableBlocks();
  if ($ret) {
    gallery_error(t('Error retrieving available blocks list'), $ret);
    return array();
  }
  // Filter plugins
  if (isset($plugin_names)) {
    $plugin_names = is_array($plugin_names) ? $plugin_names : array($plugin_names);
    foreach ($g2_blocks as $plugin => $blocks) {
      if (!in_array($plugin, $plugin_names)) {
        unset($g2_blocks[$plugin]);
      }
    }
  }
  
  return $g2_blocks;
}

/**
 * Function gallery_get_block().
 *
 * This function allows developers to fetch an image or grid block with certain
 * parameters from Gallery2.
 *
 * @param $params
 *   An array of parameters to be passed to getImageBlock().
 *   (http://gallery.menalto.com/apidoc/GalleryCore/Classes/GalleryEmbed.html#methodgetImageBlock)
 *   If 'itemId' is set to 'user' (or 'user:uid') items are be taken from the
 *   current (or specified) user's useralbum.
 * @param $block
 *   Name of the block to get from Gallery2 (e.g. 'ImageBlock', etc.)
 * @param $extra
 *   $extra['class']:
 *     Optional class to apply to the block (if customized css styles are required).
 *   $extra['num_cols']:
 *     If given for a 'ImageBlock' a grid block with 'num_cols' columns is generated.
 *     Otherwise (default) a 'normal' image block is returned.
 */
function gallery_get_block($params, $block = 'ImageBlock', $extra = array()) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_block.inc');
  if (!_gallery_init(TRUE)) {
    return array('content' => '');
  }
  $block_map = _gallery_block_map();
  $plugin = $block_map[strtolower($block)]['plugin'];
  $plugin_block = $block_map[strtolower($block)]['block'];
  
  return _gallery_block_get_block($plugin, $plugin_block, $params, $extra);
}

/**
 * Function _gallery_htmlcharsdecode().
 * (recover special character's HTML entities, see htmlspecialchars_decode() for php5)
 */
function _gallery_htmlcharsdecode($string) {
  return strtr($string, array_flip(get_html_translation_table(HTML_SPECIALCHARS)));
}

/* --------------------------------------------------------------------------
 * Error, Debug and Status Functions
 * -------------------------------------------------------------------------- */

/**
 * Function gallery_error().
 */
function gallery_error($msg, $ret = NULL, $serious = FALSE) {
  $error_mode = variable_get('gallery_error_mode', array(GALLERY_ERROR_WATCHDOG));
  $admin = user_access('administer site configuration');
  $report = $admin && variable_get('gallery_report', 1);
  
  // Verbose error messages (include backtrace if available)
  $debug_info = array();
  if (in_array(GALLERY_ERROR_VERBOSE, $error_mode) || variable_get('gallery_debug', 0) || $admin) {
    $msg = $ret ? (is_object($ret) ? ($msg .'<br />'. $ret->getAsHtml()) : $ret) : $msg;
    if (GALLERY_DEBUGTRACE && function_exists('debug_backtrace')) {
      $trace = debug_backtrace();
      $source = t('Error in function \':func()\' (:file::line):<br />',
        array(':func' => $trace[1]['function'], ':file' => basename($trace[0]['file']), ':line' => $trace[0]['line']));
      $message  = $source .'<ul><li>'. $msg .'</li></ul>';
      $debug_info = array('Debug Trace' => $trace);
    }
  }
  $message = !empty($message) ? $message : $msg;
  
  // Debug output (do not log to watchdog)
  if (variable_get('gallery_debug', 0) && $admin) {
    if ($report) {
      _gallery_report_error($debug_info);
    }
    drupal_set_message($message, 'error');
    return;
  }
  
  // Error output to browser
  if (in_array(GALLERY_ERROR_BROWSER, $error_mode)) {
    if ($report) {
      _gallery_report_error($debug_info);
    }
    drupal_set_message($message, 'error');
  }
  elseif ($serious) {
    if ($report) {
      _gallery_report_error($debug_info);
    }
    drupal_set_message($admin ? $message : t('Embedded Gallery2 is not available or requested Gallery URL does not exist.'), 'error');
  }
  
  // Error output to watchdog
  if (in_array(GALLERY_ERROR_WATCHDOG, $error_mode)) {
    watchdog('gallery', $message, array(), WATCHDOG_ERROR);
  }
}

/**
 * Function _gallery_report_error().
 */
function _gallery_report_error($report = array()) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_help.inc');
  require_once(drupal_get_path('module', 'gallery') .'/gallery_report.inc');

  drupal_set_message(_gallery_report_help(), 'error');
  _gallery_report(FALSE, $report, TRUE);
}

/**
 * Function gallery_debug().
 */
function gallery_debug($output, $label = NULL, $html = FALSE) {
  if (variable_get('gallery_debug', 0) && user_access('administer site configuration')) {
    // Output debug log into a collapsible fieldset
    $fieldset = array(
      '#title' => t('Gallery Debug') . (isset($label) ? (': '. check_plain($label)) : ''),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#value' => '<pre>'. ($html ? $output : htmlspecialchars(print_r($output, TRUE))) .'</pre>',
    );
    drupal_set_content('content', theme('fieldset', $fieldset));
  }
}

/**
 * Function gallery_plugin_status().
 */
function gallery_plugin_status($plugin_names) {
  static $all_plugins_status = array();

  $plugins_status = array();
  if (!_gallery_init()) {
    foreach ($plugin_names as $plugin) {
      $plugins_status[$plugin] = GALLERY_PLUGIN_STATUS_UNKNOWN;
    }
    
    return $plugins_status;
  }
  // Fetch status of G2 modules
  if (empty($plugins_status_cache)) {
    list($ret, $plugins_status_cache) = GalleryCoreApi::fetchPluginStatus('module');
    if ($ret) {
      gallery_error(t('Unable to get Gallery2 module status.'), $ret);
      foreach ($plugin_names as $plugin) {
        $plugins_status[$plugin] = GALLERY_PLUGIN_STATUS_UNKNOWN;
      }
      
      return $plugins_status;
    }
  }
  // Generate array containing module status
  foreach ($plugin_names as $plugin) {
    if (isset($plugins_status_cache[$plugin])) {
      if (isset($plugins_status_cache[$plugin]['active']) && $plugins_status_cache[$plugin]['available']) {
        $plugins_status[$plugin] = GALLERY_PLUGIN_ENABLED;
      }
      elseif (!isset($plugins_status_cache[$plugin]['active']) && $plugins_status_cache[$plugin]['available']) {
        $plugins_status[$plugin] = GALLERY_PLUGIN_NOT_INSTALLED;
      }
      elseif (($plugins_status_cache[$plugin]['active'] == FALSE) && $plugins_status_cache[$plugin]['available']) {
        $plugins_status[$plugin] = GALLERY_PLUGIN_NOT_ACTIVE;
      }
      else {
        $plugins_status[$plugin] = GALLERY_PLUGIN_DISABLED;
      }
    }
    else {
      $plugins_status[$plugin] = GALLERY_PLUGIN_MISSING;
    }
  }
  
  return $plugins_status;
}

/**
 * Function gallery_single_plugin_status().
 */
function gallery_single_plugin_status($plugin_name) {
  $status = gallery_plugin_status(array($plugin_name));
  return $status[$plugin_name];
}

/**
 * Function gallery_set_status().
 */
function gallery_set_status($status = array(), $reset = FALSE) {
  $status_array = $status;
  if (!$reset) {
    $status_array = unserialize(variable_get('gallery_status', serialize(array())));
    foreach ($status as $key => $value) {
      if (!empty($value)) {
        $status_array[$key] = $value;
      }
      elseif (isset($status_array[$key])) {
        unset($status_array[$key]);
      }
    }
  }
  
  variable_set('gallery_status', serialize($status_array));
}

/**
 * Function gallery_get_status().
 */
function gallery_get_status() {
  return unserialize(variable_get('gallery_status', serialize(array())));
}

/**
 * Function gallery_version().
 */
function gallery_version() {
  if (!_gallery_init()) {
    return array();
  }
  // Get API version
  list($core_major, $core_minor) = GalleryCoreApi::getApiVersion();
  list($embed_major, $embed_minor) = GalleryEmbed::getApiVersion();
  $version = array(
    'core' => array('major' => $core_major, 'minor' => $core_minor),
    'embed' => array('major' => $embed_major, 'minor' => $embed_minor)
  );

  // Update version in status messages
  $status = array('version' => array(
    'title' => t('Gallery2 API version'),
    'info' => "$core_major.$core_minor / $embed_major.$embed_minor")
  );
  gallery_set_status($status);
      
  return $version;
}
