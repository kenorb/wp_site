<?php
/*
	version: $Id: wsod_emergency.php,v 1.1.2.6 2009/06/22 12:18:40 kenorb Exp $
	author: kenorb@gmail.com
*/

$verbose = TRUE;
$fix_on_fly  = TRUE;
$output = '';

require_once './wsod.module'; // include functions

$drupal_path = wsod_detect_drupal_path($output);
if (!empty($drupal_path)) {
    chdir($drupal_path); // change dir to Drupal path
} else {
    $verbose ? print "Fatal error: Couldn't detect your Drupal installation in:<br>\n".$output : NULL;
    $verbose ? print "Move this module inside your Drupal installation!": NULL;
    exit;
}

/* simulate Drupal boot */
require_once './includes/bootstrap.inc'; // load bootstrap file
require_once './includes/menu.inc'; // load menu file (needed for Menu flags)
wsod_drupal_bootstrap_run(DRUPAL_BOOTSTRAP_FULL); // execute WSOD version of Drupal bootstrap
drupal_page_footer();

/* enable wsod module */
if (!module_exists('wsod')) {
    module_enable(array('wsod')); // enable wsod module if it's not enabled
}
if (!function_exists('wsod_check_wsod')) { // if wsod testing function doesn't exist, there is some path problem
    module_rebuild_cache(); // so we need to rebuild path of modules
    drupal_load('module', 'wsod'); // load module files again
} // if this will not help, please check manually in your filesystem if you don't have duplicated wsod modules

wsod_check_wsod($verbose, $fix_on_fly, TRUE); // run diagnostic

/**
 * WSOD version of Drupal drupal_bootstrap()
 */
function wsod_drupal_bootstrap_run($phase) {
  drupal_bootstrap(DRUPAL_BOOTSTRAP_CONFIGURATION);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_ACCESS);
  wsod_drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION); // replace handler sess_close() by WSOD
  drupal_bootstrap(DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_LANGUAGE);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_PATH);
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
}

/**
 * WSOD version of Drupal _drupal_bootstrap()
 *
 * Modifications:
 * - replace handler sess_close() by WSOD
 *   Some fatal errors can't be handled by set_error_handler(), but can be handled by session_set_save_handler()
 *
 * @param $phase
 *   A constant. Allowed values are:
 *     DRUPAL_BOOTSTRAP_CONFIGURATION: initialize configuration.
 *     DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE: try to call a non-database cache fetch routine.
 *     DRUPAL_BOOTSTRAP_DATABASE: initialize database layer.
 *     DRUPAL_BOOTSTRAP_ACCESS: identify and reject banned hosts.
 *     DRUPAL_BOOTSTRAP_SESSION: initialize session handling.
 *     DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE: load bootstrap.inc and module.inc, start
 *       the variable system and try to serve a page from the cache.
 *     DRUPAL_BOOTSTRAP_LANGUAGE: identify the language used on the page.
 *     DRUPAL_BOOTSTRAP_PATH: set $_GET['q'] to Drupal path of request.
 *     DRUPAL_BOOTSTRAP_FULL: Drupal is fully loaded, validate and fix input data.
 */
function wsod_drupal_bootstrap($phase) {
  global $conf;

  switch ($phase) {

    case DRUPAL_BOOTSTRAP_CONFIGURATION:
      drupal_unset_globals();
      // Start a page timer:
      timer_start('page');
      // Initialize the configuration
      conf_init();
      break;

    case DRUPAL_BOOTSTRAP_EARLY_PAGE_CACHE:
      // Allow specifying special cache handlers in settings.php, like
      // using memcached or files for storing cache information.
      require_once variable_get('cache_inc', './includes/cache.inc');
      // If the page_cache_fastpath is set to TRUE in settings.php and
      // page_cache_fastpath (implemented in the special implementation of
      // cache.inc) printed the page and indicated this with a returned TRUE
      // then we are done.
      if (variable_get('page_cache_fastpath', FALSE) && page_cache_fastpath()) {
        exit;
      }
      break;

    case DRUPAL_BOOTSTRAP_DATABASE:
      // Initialize the default database.
      require_once './includes/database.inc';
      db_set_active();
      break;

    case DRUPAL_BOOTSTRAP_ACCESS:
      // Deny access to hosts which were banned - t() is not yet available.
      if (drupal_is_denied('host', ip_address())) {
        header('HTTP/1.1 403 Forbidden');
        print 'Sorry, '. check_plain(ip_address()) .' has been banned.';
        exit();
      }
      break;

    case DRUPAL_BOOTSTRAP_SESSION:
      require_once variable_get('session_inc', './includes/session.inc');
      session_set_save_handler('sess_open', 'wsod_sess_close', 'sess_read', 'sess_write', 'sess_destroy_sid', 'sess_gc'); // WSOD: replace wsod handle for sess_close()
      if(!isset($_SESSION)){ // WSOD: additional condition if session has been already started
          session_start();
      }
      break;

    case DRUPAL_BOOTSTRAP_LATE_PAGE_CACHE:
      // Initialize configuration variables, using values from settings.php if available.
      $conf = variable_init(isset($conf) ? $conf : array());
      $cache_mode = variable_get('cache', CACHE_DISABLED);
      // Get the page from the cache.
      $cache = $cache_mode == CACHE_DISABLED ? '' : page_get_cache();
      // If the skipping of the bootstrap hooks is not enforced, call hook_boot.
      if (!$cache || $cache_mode != CACHE_AGGRESSIVE) {
        // Load module handling.
        require_once './includes/module.inc';
        bootstrap_invoke_all('boot');
      }
      // If there is a cached page, display it.
      if ($cache) {
        drupal_page_cache_header($cache);
        // If the skipping of the bootstrap hooks is not enforced, call hook_exit.
        if ($cache_mode != CACHE_AGGRESSIVE) {
          bootstrap_invoke_all('exit');
        }
        // We are done.
        exit;
      }
      // Prepare for non-cached page workflow.
      drupal_page_header();
      break;

    case DRUPAL_BOOTSTRAP_LANGUAGE:
      drupal_init_language();
      break;

    case DRUPAL_BOOTSTRAP_PATH:
      require_once './includes/path.inc';
      // Initialize $_GET['q'] prior to loading modules and invoking hook_init().
      drupal_init_path();
      break;

    case DRUPAL_BOOTSTRAP_FULL:
      require_once './includes/common.inc';
      _drupal_bootstrap_full();
      break;
  }
}

/** 
 * Detect Drupal path by scanning each level of current path
 * 
 */ 
function wsod_detect_drupal_path(&$output, $start_dir = __FILE__) {
    $drupal_path = FALSE; // set init variable
    $path_arr = wsod_pathposall(dirname($start_dir));
    $bootstrap_file = '/includes/bootstrap.inc';
    $buff_output = '';
    foreach ($path_arr as $path) {
        if (file_exists($path.$bootstrap_file)) {
            $drupal_path = $path;
            return $drupal_path;
        } else {
            $buff_output .= "Couldn't find $path$bootstrap_file!<br>\n";
        }
        // FIXME: file_exists() [function.file-exists]: open_basedir restriction in effect. File(//includes/bootstrap.inc) is not within the allowed path(s)
    }
    $output .= $buff_output;
    return NULL;
}

/** 
 * pathposall 
 * 
 * Find all occurrences of a subdirs in a path
 * 
 * @param string $dir
 * @return array
 */ 
function wsod_pathposall($dir){ 
    $slash = wsod_detect_filesystem_slash();
    $pos = wsod_strposall($dir,$slash);
    foreach ($pos as $no) {
        $subdir[] = substr($dir, 0, $no);
    }
    if (is_array($subdir)) {
        rsort($subdir); // make list in reverse
        return $subdir; // return array of available subdir paths
    } else {
        return array();
    }
} 

/** 
 * Detect slash type of filesystem
 * 
 */ 
function wsod_detect_filesystem_slash() {
    if (strpos(__FILE__, '/') !== FALSE) { // detect filesystem
        return '/'; // Unix style
    } else {
        return '\\'; // Windows style
    }
}

/** 
 * Find all occurrences of a needle in a haystack 
 * 
 * @param string $haystack 
 * @param string $needle 
 * @return array or false 
 */ 
function wsod_strposall($haystack,$needle){ 
    $s=0; $i=0; 
    while (is_integer($i)){ 
        $i = strpos($haystack,$needle,$s); 
        if (is_integer($i)) { 
            $aStrPos[] = $i; 
            $s = $i+strlen($needle); 
        } 
    } 
    if (isset($aStrPos)) { 
        return $aStrPos; 
    } else { 
        return false; 
    } 
} 

/**
 * Get content type of page header 
 * 
 * @return string
 * 
 */
function wsod_sess_close() {
  wsod_check_wsod(TRUE);
  return TRUE;
}
