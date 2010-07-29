<?php
// $Id: extractor.inc,v 1.1.2.13.2.2 2008/10/15 00:09:30 goba Exp $

/**
 * @file
 *   Package extractor for localization community.
 *
 *   This is not used in the l10n_community module but reused in
 *   different connectors which utilize the same functionality.
 */

/**
 * Parses contents of a specific local package file.
 *
 * @param $package_file
 *   Path to the package file to be extracted and parsed.
 * @param $release_rid
 *   Release ID.
 */
function l10n_community_parse_package($package_file, $release_rid) {
  if (!ini_get('safe_mode')) {
    // This could take a long time.
    @set_time_limit(0);
  }
  $error = $message = '';

  // l10n_community_requirements() makes sure there is a status
  // error if this is not installed.
  include_once 'Archive/Tar.php';

  // Potx module is already a dependency.
  include_once drupal_get_path('module', 'potx') .'/potx.inc';

  // Set up status messages if not in automated mode.
  potx_status('set', POTX_STATUS_MESSAGE);

  // Generate temp folder to extract the tarball. tempnam() creates a regular
  // file, thus reserving the name. But we need a directory. Because $rempname
  // is unique, we can append '_dir', and it will still be unique.
  $temp_name = tempnam(file_directory_temp(), 'l10n_community_');
  $temp_path = $temp_name .'_dir';
    
  // Nothing to do if the file is not there or the extraction folder is taken.
  if (!file_exists($package_file)) {
    $error = t('Package to parse (%file) does not exist.', array('%file' => $package_file));
  }
  elseif (is_dir($temp_path)) {
    $error = t('Temporary directory %path already exists.', array('%path' => $temp_path));
  }
  
  // Extract the local file to the temporary directory.
  else {
    $obj = new Archive_Tar($package_file);
    if (!$obj->extract($temp_path)) {
      $error = t('Error on untaring %filename file.', array('%filename' => $package_file));

      // Delete the files extracted from broken archive.
      l10n_community_rmdir_recursive($temp_path);
    }
    else {
      // Get all source files and save strings with our callback for this release.
      $files = _potx_explore_dir($temp_path);
      l10n_community_save_file($release_rid);
      $version = l10n_community_detect_major_version($package_file);
      foreach ($files as $name) {
        _potx_process_file($name, strlen($temp_path) + 1, 'l10n_community_save_string', 'l10n_community_save_file', $version);
      }
      
      // Delete the extracted files.
      l10n_community_rmdir_recursive($temp_path);
      
      // Record changes of the scanned project in the database.
      $message = t('Contents of %filename have been scanned.', array('%filename' => $package_file));

      // Parsed this releases files.
      db_query("UPDATE {l10n_community_release} SET last_parsed = %d WHERE rid = %d", time(), $release_rid);
      
      // Update error list for this release. Although the errors are related to
      // files, we are not interested in the fine details, the file names are in
      // the error messages as text. We assume no other messages are added while
      // importing, so we can safely use drupal_get_message() to grab our errors.
      db_query("DELETE FROM {l10n_community_error} WHERE rid = %d", $release_rid);
      $messages = drupal_get_messages('error');
      if (isset($messages['error']) && is_array($messages['error'])) {
        foreach ($messages['error'] as $error_message) {
          db_query("INSERT INTO {l10n_community_error} (rid, value) VALUES (%d, '%s')", $release_rid, $error_message);
        }
      }
    }
  }
  
  // Remove temporary file we used to reserve the name for the directory.
  unlink($temp_name);

  $return = array();
  if ($error) {
    // WARNING: We should not do this reuse in the 6.x port, or otherwise
    // the extractor cannot pick the watchdog() strings up.
    watchdog('l10n_community', $error, WATCHDOG_ERROR);
    $return['error'] = $error;
  }
  if ($message) {
    watchdog('l10n_community', $message);
    $return['message'] = $message;
  }
  
  return $return;
}

/**
 * CVS revision saver callback for potx. We call it with a release id
 * if $file is not given. And we ask for a file ID (to save the string
 * with), if $revision is not given.
 *
 * This is called:
 *  - before any file parsing with ($rid, NULL)
 *  - just as a new file is found by potx with ($revision, $file)
 *  - just as a new string is found by our own callback with (NULL, $file)
 *
 * @param $revision
 *   CVS revision information about $file. If not given, the recorded
 *   fid of $file will be returned.
 * @param $file
 *   File location in package. If not given, $revision is taken as a
 *   release id to use to store the file list.
 */
function l10n_community_save_file($revision = NULL, $file = NULL) {
  static $rid = 0;
  static $files = array();
  
  if (!isset($file)) {
    // We get the release number for the files.
    $rid = $revision;
  }
  elseif (!isset($revision)) {
    // We return data for a specific file.
    return $files[$file];
  }
  else {
    if ($existing_file = db_fetch_object(db_query("SELECT * FROM {l10n_community_file} WHERE rid = %d AND location = '%s'", $rid, $file))) {
      if ($existing_file->revision != $revision) {
        // Changed revision on a file.
        db_query("UPDATE {l10n_community_file} SET revision = '%s' WHERE fid = %d", $revision, $existing_file->fid);
      }
      $fid = $existing_file->fid;
    }
    else {
      // New file in this release.
      db_query("INSERT INTO {l10n_community_file} (rid, location, revision) VALUES(%d, '%s', '%s')", $rid, $file, $revision);
      $fid = db_result(db_query("SELECT fid FROM {l10n_community_file} WHERE rid = %d and location = '%s'", $rid, $file));
    }
    $files[$file] = $fid;
  }
}

/**
 * String saving callback for potx.
 *
 * @todo
 *   More elegant plural handling.
 * @todo
 *   Find a way to properly use POTX constants before potx.inc is
 *   loaded.
 *
 * @param $value
 *   String value to store.
 * @param $file
 *   Name of file the string occured in.
 * @param $line
 *   Number of line the string was found.
 * @param $string_type
 *   String type: POTX_STRING_INSTALLER, POTX_STRING_RUNTIME
 *   or POTX_STRING_BOTH.
 */
function l10n_community_save_string($value = NULL, $file = NULL, $line = 0, $string_type = 2 /*POTX_STRING_RUNTIME*/) {
  static $files = array();
  
  // Strip all slashes from string.
  $value = stripcslashes($value);
  
  if (!isset($files[$file])) {
    // Get file ID for saving, locally cache.
    $files[$file] = l10n_community_save_file(NULL, $file);
  }
  
  // If we have the file entry now, we can process adding the string.
  if (isset($files[$file])) {
    // A \0 separator in the string means we deal with a string with plural variants.
    // Unlike Drupal core, we store all in the same string, as it is easier
    // to handle later, and we don't need the individual string parts.
    if (!$sid = db_result(db_query("SELECT sid FROM {l10n_community_string} WHERE value = BINARY '%s'", $value))) {
      // String does not exist.
      db_query("INSERT INTO {l10n_community_string} (value) VALUES ('%s')", $value);
      $sid = db_result(db_query("SELECT sid FROM {l10n_community_string} WHERE value = BINARY '%s'", $value));
    }
    if (!db_result(db_query("SELECT fid FROM {l10n_community_line} WHERE fid = %d AND sid = %d AND lineno = %d AND type = %d", $files[$file], $sid, $line, $string_type))) {
      // Location does not exist with this string.
      db_query("INSERT INTO {l10n_community_line} (fid, sid, lineno, type) VALUES (%d, %d, %d, %d)", $files[$file], $sid, $line, $string_type);
    }
  }
}

/**
 * Delete the complete contents of a directory recursively.
 *
 * @param $dirname
 *  The directory name to be deleted.
 */
function l10n_community_rmdir_recursive($directory) {
  if (!is_dir($directory)) {
    return;
  }
  if (substr($directory, -1) != '/') {
    $directory .= '/';
  }
  if ($handle = opendir($directory)) {
    while ($file = readdir($handle)) {
        if ($file == '.' or $file == '..') {
          continue;
        }
        $path = $directory .'/'. $file;
        if (is_dir($path)) {
          l10n_community_rmdir_recursive($path);
        }
        else {
          unlink($path);
        }
    }
    rmdir($directory);
    closedir($handle);
  }
}
