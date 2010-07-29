<?php
// $Id: gallery_report.inc,v 1.3.2.6 2008/06/14 20:16:17 profix898 Exp $

/**
 * gallery.module : gallery_report.inc
 * Bug report assistance (system info)
 */

/**
 * Function _gallery_report().
 */
function _gallery_report($download = FALSE, $report = array(), $cache = FALSE) {
  // provide download of the cached report
  if ($download && ($content = cache_get('gallery:report_'. session_id()))) {
    _gallery_report_download($content->data);
  }
  
  //System
  global $db_url;
  $report['System']['PHP']['Version'] = phpversion();
  $report['System']['PHP']['Memory limit'] = ini_get('memory_limit');
  $report['System']['PHP']['Safe mode'] = ini_get('safe_mode') ? 'On' : 'Off';  
  $database = is_array($db_url) ? $db_url : array('default' => $db_url);
  foreach ($database as $key => $value) {
    db_set_active($key);
    $db_info = parse_url($value);
    $report['System']['Database'][drupal_ucfirst($key)]['Type'] =  urldecode(strtoupper($db_info['scheme']));
    $report['System']['Database'][drupal_ucfirst($key)]['Version'] = db_version();
  }
  db_set_active('default');
  
  //Request
  $report['Request']['URI'] = request_uri();
  $report['Request']['Query'] = isset($_GET['q']) ? $_GET['q'] : '';
  $report['Request']['G2Path'] = isset($_GET['g2_path']) ? $_GET['g2_path'] : '';
  $report['Request']['Referer'] = referer_uri();
  
  //Drupal
  $report['Drupal']['Version'] = VERSION;
  
  // Module
  $modules = drupal_system_listing('\.module$', 'modules', 'name', 0);
  system_get_files_database($modules, 'module');
  foreach ($modules as $name => $module) {
    if (preg_match('/^gallery$|^gallery_/', $name)) {
      $info = drupal_parse_info_file(dirname($module->filename) .'/'. $module->name .'.info');
      $report['Module']['Version'][$info['name']] = array(
        'Version' => isset($info['version']) ? $info['version'] : 'unknown',
        'Schema Version' => $module->schema_version,
        'Location' => dirname($module->filename),
        'Status' => $module->status
      );
      // extract detailed version info from cvsid
      $files = file_scan_directory(dirname($module->filename), '.*\.(inc|module)$', array('.', '..', 'CVS'), 0, FALSE);
      foreach ($files as $file) {
        if (preg_match('/\x24Id: '. $file->basename .',v ([^\\$]+) Exp \x24/i', file_get_contents($file->filename), $cvsid)) {
          $report['Module']['Version'][$info['name']]['Files'][$file->basename] = 'Rev. '. $cvsid[1];
        }
      }
    }
  }
  // Fetch module-related variables
  $result = db_query('SELECT * FROM {variable} WHERE name LIKE \'gallery_%\'');
  while ($var = db_fetch_object($result)) {
    $report['Module']['Variables'][$var->name] = unserialize($var->value);
  }
  $status = gallery_get_status();
  unset($status['version'], $status['gallery_valid']);
  $report['Module']['Variables']['gallery_status'] = array($status);
  
  // Gallery
  if (variable_get('gallery_valid', 0) && _gallery_init(FALSE, NULL, FALSE)) {
    $version = gallery_version();
    $report['Gallery 2']['Version'] = array(
      'Core API' => $version['core']['major'] .'.'. $version['core']['minor'],
      'Embed API' => $version['embed']['major'] .'.'. $version['embed']['minor'],
    );
    list($ret, $rewrite_api) = GalleryCoreApi::newFactoryInstance('RewriteApi');
    if (!$ret && $rewrite_api) {
      list($ret, $rewrite_params) = $rewrite_api->fetchEmbedConfig();
      $report['Gallery 2']['URL Rewrite'] = $rewrite_params;
    }
    // Get some basic information about G2 plugins
    list($ret, $plugins) = GalleryCoreApi::fetchPluginStatus('module');
    foreach ($plugins as $name => $info) {
      $report['Gallery 2']['Active Plugins'][drupal_ucfirst($name)]['Version'] = isset($info['active']) ? $info['version'] : 'inactive';
    }
    // Debug Logs
    if ($GLOBALS['gallery']->_debug == 'buffered' && !empty($GLOBALS['gallery']->_debugBuffer)) {
      $report['Gallery 2']['DebugBuffer'] = $GLOBALS['gallery']->_debugBuffer;
    }
    else if (!empty($GLOBALS['gallery']->_debugSnippet)) {
      $report['Gallery 2']['DebugSnippet'] = $GLOBALS['gallery']->_debugSnippet;
    }
  }
  else {
    $report['Gallery 2'] = 'Gallery2 not available';
  }
  
  $content = theme('gallery_report', $report);
  
  if ($cache) {
    if ($cache_content = cache_get('gallery:report_'. session_id())) {
      $content .= $cache_content->data;
    }
    cache_set('gallery:report_'. session_id(), $content, 'cache', time()+600);
  }
  if ($download) {
    _gallery_report_download($content);
  }
  
  return $content;
}

/**
 * Function _gallery_report_download().
 */
function _gallery_report_download($content) {
  header("Content-Type: application/octet-stream");
  header("Content-Disposition: attachment; filename=gallery_report.html");
  print "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
  print "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
  print " <head>\n  <title>". t('Gallery module : Report Generator') ."</title>\n </head>\n";
  print " <body>\n ". $content ."\n</body>\n";
  print "</html>";
  exit();
}

/**
 * Theme function : theme_gallery_report().
 */
function theme_gallery_report($report) {
  return '  <div><table border=1 style=\'empty-cells:hide;\'>'. _gallery_report_walk($report) ."\n".'  </table></div>';
}

/**
 * Function _gallery_report_walk().
 */
function _gallery_report_walk($element, $level = 1, $sub = FALSE) {
  $html = '';
  $element = is_object($element) ? get_object_vars($element) : $element;
  if (is_array($element)) {
    foreach ($element as $key => $value) {
      if (is_array($value) || is_object($value)) {
        $html .= "\n".'    <tr>';
        $html .= implode('<td></td>', array_fill(0, $level, ''));
        $html .= '<td>'. $key .'</td>';
        $html .= _gallery_report_walk($value, $level+1, TRUE);
      }
      else {
        $html .= $sub ? '' : "\n".'    <tr>'. implode('<td></td>', array_fill(0, $level, ''));
        $html .= '<td>'. $key .'</td><td>'. $value .'</td>'; 
        $html .= '</tr>';
        $sub = FALSE;
      }
    }
  }  
  
  return $html;
}
