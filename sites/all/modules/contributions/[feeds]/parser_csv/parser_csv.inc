<?php
// $Id: parser_csv.inc,v 1.2 2008/12/01 23:44:50 alexb Exp $

/**
 * @file
 *   Functions that are only used when parsing.
 */

/**
 * Download file from URL. Unzip file if zipped.
 * @todo: 
 * Support large files:
 * - Download to a file and then parse line by line.
 * - Stop parsing after a set time, pick up where left off.
 */
function parser_csv_download($url) {
  
  // If data is zipped, download and store it.
  if (strpos(substr($url, strlen($url) - 4), '.zip') !== FALSE || strpos(substr($url, strlen($url) - 8), '.ziptest') !== FALSE) {
    // Download file and unzip it before reading it.
    $result = parser_csv_unzip($url);
  }
  else {
    $result = drupal_http_request($url);
  }
  return $result->data;
}

/**
 * Download unzip and load a zip file at the given URL.
 * @param $url
 *   URL that points to a valid zip file.
 * @return 
 *   return value of drupal_http_request() data element unzipped.
 */
function parser_csv_unzip($url) {
  $result = drupal_http_request($url);
  
  if ($result->error) {
    watchdog('parser_cvs', 'Couldn\'t download file from !url', array('!url' => $url), WATCHDOG_ERROR);
    drupal_set_message(t('Could not download file from !url', array('!url' => $url)), 'error');
    return FALSE;
  }

  // Write file to disk
  $tmp_file = file_directory_temp() .'/unzip_'. hash('md5', $url) .'.zip';
  $tmp_fp = fopen($tmp_file, 'w');
  fwrite($tmp_fp, $result->data);
  fclose($tmp_fp);

  // Unzip it
  $command = 'unzip -o '. $tmp_file .' -d '. file_directory_temp() .' | grep inflating';
  $output = shell_exec($command);
  $strpos = strpos(trim($output), 'inflating: ');
  if ($strpos === 0) {
    $unzipped_file = trim(substr($output, strpos($output, '/')));
  }

  // Read unzipped data 
  $unzipped_fp = fopen($unzipped_file, 'r');
  $data = fread($unzipped_fp, filesize($unzipped_file));
  fclose($unzipped_fp);
  
  // Stick it onto $result->data or throw error.
  if ($data) {
    $result->data = $data;
    return $result;
  }
  else {
    watchdog('parser_cvs', 'Could not read unzipped file !file', array('!file' => $unzipped_file), WATCHDOG_ERROR);
    drupal_set_message(t('Could not download unzipped file !file', array('!file' => $unzipped_file)), 'error');
  }
  return FALSE;
}

/**
 * Parse CSV into a FeedAPI compatible $feed object.
 * 
 * @param $csv_string 
 *   String in CSV format.
 * 
 * @return $feed
 *   FeedAPI feed object.
 */
function _parser_csv_parse($feed) {
  // Create FeedAPI feed object.
  $feed->title = t('CSV data from !url', array('!url' => $feed->url));
  $feed->description = '';
  $feed->options->link = $feed->url;
  $feed->items = array();

  // Download and parse document @ URL
  $csv_string = parser_csv_download($feed->url);
  _parser_csv_parse_csv($csv_string, $feed);
  return $feed;
}

/**
 * Parse CSV file into items.
 * 
 * Expects first line as header.
 * Expects , as delimiter.
 * 
 * @todo:
 *   Make configurable and more robust.
 */
function _parser_csv_parse_csv($csv_string, $feed) {
  $lines = explode(chr(10), $csv_string);
  // We expect first line to be a header.
  if (@count($lines)) {
    $header = array_shift($lines);
    $header = explode(',', $header);
  }

  // Get rules for building timestamp, title and description.
  // @todo: support more complex rules.
  $title_rule = $description_rule = $timestamp_rule = array();
  if (isset($feed->settings['parsers']['parser_csv']['timestamp_rule'])) {
    list($timestamp_field, $timezone_name) = explode(',',$feed->settings['parsers']['parser_csv']['timestamp_rule']);
    $timestamp_rule = array('timestamp_field' => $timestamp_field, 'timezone_name' => $timezone_name);
  }
  if (isset($feed->settings['parsers']['parser_csv']['title_rule'])) {
    $title_rule = explode(',', $feed->settings['parsers']['parser_csv']['title_rule']);
  }
  if (isset($feed->settings['parsers']['parser_csv']['description_rule'])) {
    $description_rule = explode(',', $feed->settings['parsers']['parser_csv']['description_rule']);
  }
  
  // Iterate through all lines and create items from it.
  $time = time();
  
  foreach ($lines as $line) {
    if (strlen($line)) {
      if ($fields = _parser_csv_parse_line($header, $line)) {

        // Build title and description.
        $title = array();
        foreach ($title_rule as $field_name) {
          $title[] = $fields[$field_name];
        }
        $description = array();
        foreach ($description_rule as $field_name) {
          $description[] = $fields[$field_name];
        }
        
        // Get timestamp.
        // First try to get from feed, if not available, use current time.
        $timestamp = NULL;
        if (isset($timestamp_rule['timestamp_field']) && isset($fields[$timestamp_rule['timestamp_field']])) {
          $timestamp = $fields[$timestamp_rule['timestamp_field']];
          if ($timestamp_rule['timezone_name']) {
            $old_timezone = date_default_timezone_get();
            if (!date_default_timezone_set($timestamp_rule['timezone_name'])) {
              drupal_set_message(t('Invalid timezone identifier !identifier', array('!identifier' => $timestamp_rule['timezone_name'])));
            }
            // Convert timestamp to UNIX time.
            $timestamp = strtotime($timestamp);
            // Set back to old time zone.
            date_default_timezone_get($old_timezone);
          }
          else {
            $timestamp = strtotime($timestamp);
          }
        }
        else {
          $timestamp = $time;
        }
        
        // Build feed item.
        $item = new stdClass();
        $item->title = count($title) ? implode(', ', $title) : t('No title');
        $item->description = count($description) ? implode(', ', $description) : '';
        $item->options = new stdClass();
        $item->options->timestamp = $timestamp;
        $item->options->guid = hash('md5', $line);
        $item->options->raw = $fields;

        // Attach it to feed.
        $feed->items[] = $item;
      }
      else {
        drupal_set_message(t('Could not parse line !line', array('!line' => $line)), 'error');
      }
    }
  }
}

/**
 * Parse a line from a CSV file.
 *
 * @param $header
 *   Array that is the header of the CSV file.
 * @param $line
 *   String that is the line to parse.
 * @return 
 *   Associative array where the keys are the field names as defined in the header
 *   and the values are the ones found in $line.
 */
function _parser_csv_parse_line($header, $line, $delimiter = ',') {
  $fields = explode($delimiter, $line);
  $result = array();
  foreach ($header as $i => $field_name) {
    if (isset($fields[$i])) {
      $result[$field_name] = $fields[$i];
    }
    else {
      drupal_set_message(t('No value found for !field_name', array('!field_name' => $field_name)), 'error');
      return FALSE;
    }
  }
  return $result;
}
