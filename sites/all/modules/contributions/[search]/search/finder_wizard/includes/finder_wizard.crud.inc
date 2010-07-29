<?php
// $Id: finder_wizard.crud.inc,v 1.1.2.3.2.1 2009/01/03 01:30:05 danielb Exp $

/**
 * Get all finder wizard summaries
 * Great for creating a quick list of all finder wizards.
 * returns array of cut down versions of all finder wizard object
 * or FALSE on failure
 */
function finder_wizard_crud_read_all_summaries() {
  $result = db_query('SELECT * FROM {finder_wizard}');
  $finder_wizards = array();
  while ($fw = db_fetch_object($result)) {
    $finder_wizards[$fw->fwid] = $fw;
  }
  if (!empty($finder_wizards)) {
    return $finder_wizards;
  }
  return FALSE;
}

/**
 * Build and return full finder wizard object.
 * Great for getting all the settings for one finder wizard.
 * returns full version of finder wizard object
 * or FALSE on failure
 */
function finder_wizard_crud_read_finder_wizard($fwid) {
  $finder_wizard = finder_wizard_crud_read_summary($fwid);
  if (!$finder_wizard) {
    return FALSE;
  }
  $finder_wizard->types = finder_wizard_crud_read_types($finder_wizard->fwid);
  $finder_wizard->steps = finder_wizard_crud_read_all_steps($finder_wizard->fwid);
  return $finder_wizard;
}

/**
 * Get the settings of the specified fwid
 * returns cut down version of finder wizard object
 * or FALSE on failure
 */
function finder_wizard_crud_read_summary($fwid) {
  $column = 'fwid';
  $modifier = "%d";
  if (!is_numeric($fwid)) { // not an fwid, must be a path
    $column = 'path';
    $modifier = "'%s'";
    
  }
  $finder_wizard = db_fetch_object(db_query('SELECT * FROM {finder_wizard} WHERE '. $column .' = '. $modifier .'', $fwid));
  if (!empty($finder_wizard)) {
    return $finder_wizard;
  }
  return FALSE;
}

/**
 * Set settings of a new finder wizard
 * returns fwid of new finder wizard
 * or FALSE on failure
 */
function finder_wizard_crud_create_summary($title, $desc, $path, $none_above, $none_above_path, $error_msg, $early_exit, $results_page) {
  if (db_query("
    INSERT INTO {finder_wizard} (title, description, path, none_above, none_above_path, error_msg, early_exit, results_page) 
    VALUES ('%s', '%s', '%s', %d, '%s', '%s', %d, %d)", 
    $title, $desc, $path, $none_above, $none_above_path, $error_msg, $early_exit, $results_page)) {
    return db_last_insert_id('finder_wizard', 'fwid');
  }
  return FALSE;
}

/**
 * Set the settings of the specified fwid
 * returns FALSE on failure
 */
function finder_wizard_crud_update_summary($fwid, $title, $desc, $path, $none_above, $none_above_path, $error_msg, $early_exit, $results_page) {
  return db_query("
    UPDATE {finder_wizard} 
    SET title = '%s', 
    description = '%s', 
    path = '%s', 
    none_above = %d, 
    none_above_path = '%s', 
    error_msg = '%s',
    early_exit = %d, 
    results_page = %d 
    WHERE fwid = %d", 
    $title, $desc, $path, $none_above, $none_above_path, $error_msg, $early_exit, $results_page, $fwid);
}

/**
 * Del the settings of the specified fwid
 * returns FALSE on failure
 */
function finder_wizard_crud_delete_summary($fwid) {
  return db_query('DELETE FROM {finder_wizard} WHERE fwid = %d', $fwid);
}

/**
 * Get the node types of the specified fwid
 * returns an array of types
 * or FALSE on failure
 */
function finder_wizard_crud_read_types($fwid) {
  $result = db_query('SELECT type FROM {finder_wizard_types} WHERE fwid = %d', $fwid);
  $types = array();
  while ($type = db_fetch_array($result)) {
    $types[] = $type['type'];
  }
  if (!empty($types)) {
    return $types;
  }
  return FALSE;
}

/**
 * Del the node types of the specified fwid
 * returns FALSE on failure
 */
function finder_wizard_crud_delete_types($fwid) {
  return db_query('DELETE FROM {finder_wizard_types} WHERE fwid = %d', $fwid);
}

/**
 * Set node types of a new finder wizard
 * returns FALSE on failure
 */
function finder_wizard_crud_create_types($fwid, $types) {
  $args = array();
  $values_strings = array();
  foreach ($types as $type) {
    $values_strings[] = "(%d, '%s')";
    $args[] = $fwid;
    $args[] = $type;
  }
  $values = implode(',', $values_strings);
  return db_query("INSERT INTO {finder_wizard_types} (fwid, type) VALUES ". $values ." ", $args);
}

/**
 * Set node types of the specified fwid
 * returns FALSE on failure
 */
function finder_wizard_crud_update_types($fwid, $types) {
  finder_wizard_crud_delete_types($fwid);
  return finder_wizard_crud_create_types($fwid, $types);
}

/**
 * Get the steps for the specified fwid
 * returns an array of step arrays
 * or FALSE on failure
 */
function finder_wizard_crud_read_all_steps($fwid) {
  $result = db_query("SELECT * FROM {finder_wizard_steps} WHERE fwid = %d ORDER BY weight", $fwid);
  $steps = array();
  $fields = finder_wizard_get_fields();
  while ($step = db_fetch_array($result)) {
 
    $step['field_display'] = $fields[$step['field_handler'] .'.'. $step['field_name']];
    $steps[] = $step;
  }   
  if (!empty($steps)) {
    return $steps;
  }
  return FALSE;
}

/**
 * Get a step specified by the fwsid
 * returns a step array
 * or FALSE on failure
 */
function finder_wizard_crud_read_step($fwsid) {
  $fields = finder_wizard_get_fields();
  $step = db_fetch_array(db_query('SELECT * FROM {finder_wizard_steps} WHERE fwsid = %d', $fwsid));
  $step['field_display'] = $fields[$step['field_handler'] .'.'. $step['field_name']];
  return $step;
}

/**
 * Set step information of a new finder wizard step
 * returns fwsid of new step
 * or FALSE on failure
 */
function finder_wizard_crud_create_step($fwid, $question, $response, $field_handler, $field_name, $none_above, $none_above_path, $error_msg, $element) {
  if (db_query("INSERT INTO {finder_wizard_steps} (fwid, question, response, field_handler, none_above, none_above_path, error_msg, field_name, element) 
  VALUES (%d, '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s')", $fwid, $question, $response, $field_handler, $field_name, $none_above, $none_above_path, $error_msg, $element)) {
    return db_last_insert_id('finder_wizard_steps', 'fwsid');
  }
  return FALSE;
}

/**
 * Set step information for a step specified by the fwsid
 * returns FALSE on failure
 */
function finder_wizard_crud_update_step($fwsid, $fwid, $question, $response, $field_handler, $field_name, $none_above, $none_above_path, $error_msg, $element) {
  return db_query("UPDATE {finder_wizard_steps} SET 
    fwid = %d, question = '%s', response = '%s', field_handler = '%s', field_name = '%s', none_above = %d, none_above_path = '%s', error_msg = '%s', element = '%s' WHERE fwsid = %d", 
    $fwid, $question, $response, $field_handler, $field_name, $none_above, $none_above_path, $error_msg, $element, $fwsid);
}

/**
 * Set step information for a step specified by the fwsid
 * returns FALSE on failure
 */
function finder_wizard_crud_update_step_weight($fwsid, $weight) {
  return db_query("UPDATE {finder_wizard_steps} SET weight = %d WHERE fwsid = %d", $weight, $fwsid);
}

/**
 * Del step information for a step specified by the fwsid
 * returns FALSE on failure
 */
function finder_wizard_crud_delete_step($fwsid) {
  return db_query("DELETE FROM {finder_wizard_steps} WHERE fwsid = %d", $fwsid);
}

/**
 * Del step information for all step specified by the fwid
 * returns FALSE on failure
 */
function finder_wizard_crud_delete_all_steps($fwid) {
  return db_query("DELETE FROM {finder_wizard_steps} WHERE fwid = %d", $fwid);
}
