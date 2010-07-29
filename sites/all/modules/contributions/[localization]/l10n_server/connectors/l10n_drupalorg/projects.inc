<?php
// $Id: projects.inc,v 1.1.2.6.2.2 2008/10/15 15:04:09 goba Exp $

/**
 * @file
 *   Project data updates for l10n_drupalorg.
 */

/**
 * Central update server, from where project updates come from.
 */
define('L10N_DRUPALORG_SYNC_URL', 'http://updates.drupal.org/release-history');

/**
 * Syncronize local project list with all available projects on the update server.
 *
 * @todo
 *   There is no service interface for this yet on the server, so for now this
 *   is just a stub, implement stuff later.
 */
function l10n_drupalorg_sync_projects() {
  // TEST DATA!
  $projects = array(
    'drupal' => (object) array(
      'uri' => 'drupal',
      'title' => 'Drupal',
      'link' => 'http://drupal.org/project/drupal',
    ),
    'ecommerce' => (object) array(
      'uri' => 'ecommerce',
      'title' => 'E-Commerce',
      'link' => 'http://drupal.org/project/ecommerce',
    ),
    'og' => (object) array(
      'uri' => 'og',
      'title' => 'Organic groups',
      'link' => 'http://drupal.org/project/og',
    ),
  );
  // Record all projects in the database.
  foreach ($projects as $uri => $project) {
    if ($existing_project = db_fetch_object(db_query("SELECT * FROM {l10n_community_project} WHERE uri = '%s'", $uri))) {
      if ($existing_project->connector_module == 'l10n_drupalorg') {
        if ($existing_project->status == 0) {
          // Skip this project if it is disabled.
          continue;
        }
        if ($project->title != $existing_project->title) {
          // Update if title is different (uri should not change, other data is
          // internal). Also set the parsed timestamp to the past, so we will
          // reparse the releases later.
          db_query("UPDATE {l10n_community_project} SET title = '%s', last_parsed = 0, home_link = '%s' WHERE uri = '%s' AND connector_module = 'l10n_drupalorg'", $project->title, $uri, $project->link);
        }
      }
      else {
        // Log error on existing project with another connector and skip the rest of this function.
        $t_args = array('%uri' => $uri, '%other_connector' => $existing_project->connector_module, '%this_connector' => 'l10n_drupalorg');
        watchdog('l10n_drupalorg', 'An existing project under the URI %uri is already handled by the %other_connector module. Not possible to add it with %this_connector.', $t_args);
        drupal_set_message(t('An existing project under the URI %uri is already handled by the %other_connector module. Not possible to add it with %this_connector.', $t_args), 'error');
        continue;
      }
    }
    else {
      // New project, not recorded before.
      db_query(
        "INSERT INTO {l10n_community_project} (uri, title, last_parsed, home_link, connector_module, status) VALUES ('%s', '%s', %d, '%s', '%s', %d)",
        $uri, $project->title, 0, $project->link, 'l10n_drupalorg', 1
      );
    }
  }
}

/**
 * Syncronize release data for a given project with all core versions.
 *
 * @param $project
 *   Project object.
 */
function l10n_drupalorg_sync_releases($project) {
 
  // Request the update XML file with releases for this project.
  $xml = drupal_http_request(L10N_DRUPALORG_SYNC_URL .'/'. $project->uri .'/all');
  if (!$xml->data) {
    // No data, keep our lists.
    continue;
  }
  
  $parser = new l10n_drupalorg_project_xml_parser;
  $releases = $parser->parse(array($xml->data));
  
  if (isset($releases[$project->uri]['releases'])) {
    // We have releases to work with.
    foreach ($releases[$project->uri]['releases'] as $release) {
      if ($release['status'] != 'published' || !l10n_community_is_supported_version($release['download_link']) ||
          (isset($release['version_extra']) && $release['version_extra'] == 'dev')) {
        // Skip unpublished and unsupported releases, as well as development snapshots.
        continue;
      }
      if ($existing_release = db_fetch_object(db_query("SELECT * FROM {l10n_community_release} WHERE pid = %d AND title = '%s'", $project->pid, $release['version']))) {
        if ($release['date'] != $existing_release->file_date) {
          // New file with possibly new date for the same release (dev snapshot
          // probably), so update data, set parsed date to past.
          db_query("UPDATE {l10n_community_release} SET download_link = '%s', file_date = %d, last_parsed = 0 WHERE rid = %d", $release['download_link'], $release['date'], $existing_release->rid);
        }
      }
      else {
        // No such release recorded yet.
        db_query("INSERT INTO {l10n_community_release} (pid, title, download_link, file_date, last_parsed) VALUES (%d, '%s', '%s', %d, 0)", $project->pid, $release['version'], $release['download_link'], $release['date']);
      }
    }
  }
  
  // Parsed this projects releases.
  db_query("UPDATE {l10n_community_project} SET last_parsed = %d WHERE pid = %d", time(), $project->pid);
}

/**
 * Downloads files for a particular release and parses contents.
 *
 * @param $release
 *   Release object.
 */
function l10n_drupalorg_sync_files($release) {
  // This could take a long time.
  @set_time_limit(0);
  
  $filename = basename($release->download_link);
  $file = file_directory_temp() .'/'. $filename;
  $return = array();
  
  // Nothing to do if already downloaded.
  if (file_exists($file)) {
    $error = t('File %file already exists.', array('%file' => $file));
  }
  
  // Download the tar.gz file from drupal.org and save it in the local filesystem.
  elseif (!(($contents = drupal_http_request($release->download_link)) && file_put_contents($file, $contents->data))) {
    $error = t('Unable to download and save %download_link file.', array('%download_link' => $release->download_link));
  }

  // Extract and parse the local file and remove the tarball.
  else {
    $return = l10n_community_parse_package($file, $release->rid);
    unlink($file);
  }

  if ($error) {
    // WARNING: We should not do this reuse in the 6.x port, or otherwise
    // the extractor cannot pick the watchdog() strings up.
    watchdog('l10n_drupalorg', $error, WATCHDOG_ERROR);
    $return['error'] = $error;
  }

  return $return;
}

/**
 * XML Parser object to read Drupal's release history info files.
 * This uses PHP4's lame XML parsing, but it works.
 *
 * Copied from update.module being developed for Drupal 6.
 */
class l10n_drupalorg_project_xml_parser {
  var $projects = array();
  var $current_project;
  var $current_release;
  var $current_term;
  var $current_tag;
  var $current_object;

  /**
   * Parse an array of XML data files.
   */
  function parse($data) {
    foreach ($data as $datum) {
      $parser = xml_parser_create();
      xml_set_object($parser, $this);
      xml_set_element_handler($parser, 'start', 'end');
      xml_set_character_data_handler($parser, 'data');
      xml_parse($parser, $datum);
      xml_parser_free($parser);
    }
    return $this->projects;
  }

  function start($parser, $name, $attr) {
    $this->current_tag = $name;
    switch ($name) {
      case 'PROJECT':
        unset($this->current_object);
        $this->current_project = array();
        $this->current_object = &$this->current_project;
        break;
      case 'RELEASE':
        unset($this->current_object);
        $this->current_release = array();
        $this->current_object = &$this->current_release;
        break;
      case 'TERM':
        unset($this->current_object);
        $this->current_term = array();
        $this->current_object = &$this->current_term;
        break;
    }
  }

  function end($parser, $name) {
    switch ($name) {
      case 'PROJECT':
        unset($this->current_object);
        $this->projects[$this->current_project['short_name']] = $this->current_project;
        $this->current_project = array();
        break;
      case 'RELEASE':
        unset($this->current_object);
        $this->current_project['releases'][$this->current_release['version']] = $this->current_release;
        break;
      case 'RELEASES':
        $this->current_object = &$this->current_project;
        break;
      case 'TERM':
        unset($this->current_object);
        $term_name = $this->current_term['name'];
        if (!isset($this->current_release['terms'])) {
          $this->current_release['terms'] = array();
        }
        if (!isset($this->current_release['terms'][$term_name])) {
          $this->current_release['terms'][$term_name] = array();
        }
        $this->current_release['terms'][$term_name][] = $this->current_term['value'];
        break;
      case 'TERMS':
        $this->current_object = &$this->current_release;
        break;
      default:
        $this->current_object[strtolower($this->current_tag)] = trim($this->current_object[strtolower($this->current_tag)]);
        $this->current_tag = '';
    }
  }

  function data($parser, $data) {
    if ($this->current_tag && !in_array($this->current_tag, array('PROJECT', 'RELEASE', 'RELEASES', 'TERM', 'TERMS'))) {
      $tag = strtolower($this->current_tag);
      if (isset($this->current_object[$tag])) {
        $this->current_object[$tag] .= $data;
      }
      else {
        $this->current_object[$tag] = $data;
      }
    }
  }
}
