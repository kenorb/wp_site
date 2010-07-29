<?php
// $Id: node_import.inc,v 1.1.2.22 2008/12/16 22:12:23 robrechtj Exp $

/**
 * @file
 * Public API of the Node import module.
 */

/**
 * @mainpage Public API of Node import
 * TODO
 */

/**
 * @defgroup node_import_constants Node import constants
 * @{
 * The constants are used in {node_import_tasks} and {node_import_status}
 * database tables to specify the status of either the complete task or
 * one individual row.
 */

/**
 * Status pending.
 *
 * - In {node_import_tasks} : the task is not yet finished and there are
 *    more pending rows to import.
 *
 * - In {node_import_status} : the import of the row has started, but has
 *   not yet finished. This means the row may have to be reimported. Note
 *   that we don't really know what part of it didn't finish :-(
 */
define('NODE_IMPORT_STATUS_PENDING', 0);

/**
 * Status error.
 *
 * - In {node_import_tasks} : the task has finished with some errors and
 *   the user has chosen to retry importing the rows that had errors.
 *   Possibly after editing the task. This has not yet been implemented.
 *
 * - In {node_import_status} : the row did not import and there were
 *   errors.
 */
define('NODE_IMPORT_STATUS_ERROR', 1);

/**
 * Status finished.
 *
 * - In {node_import_tasks} : the task is fully finished and file and task
 *   can be deleted.
 *
 * - In {node_import_status} : the row has been imported succesfully.
 */
define('NODE_IMPORT_STATUS_DONE', 2);

/**
 * @}
 */

foreach ((array)drupal_system_listing('.*\.inc', drupal_get_path('module', 'node_import'), 'name') as $name => $file) {
  if (module_exists($name)) {
    require_once('./'. $file->filename);
  }
}

/**
 * @defgroup node_import_hooks Node import hooks
 * @{
 */

/**
 * Returns a list of available content types.
 *
 * @param $check_access
 *   Boolean. If TRUE, only the types the user can create are
 *   returned. If FALSE, all types are returned.
 *
 * @param $reset
 *   Boolean. If TRUE, the internal cache is rebuilt.
 *
 * @return
 *   Array of types. See hook_node_import_types().
 */
function node_import_types($check_access = TRUE, $reset = FALSE) {
  static $types;
  static $allowed_types;

  if (!isset($types) || $reset) {
    $defaults = array(
      'title' => '',
      'can_create' => '',
      'create' => '',
    );

    $utypes = (array)module_invoke_all('node_import_types');
    foreach ($utypes as $type => $typeinfo) {
      $utypes[$type] = array_merge($defaults, $typeinfo);
    }
    drupal_alter('node_import_types', $utypes);

    $types = array_map('strip_tags', node_import_extract_property($utypes, 'title'));
    asort($types);
    foreach ($types as $type => $title) {
      $types[$type] = $utypes[$type];
    }

    $allowed_types = array();
    foreach ($types as $type => $typeinfo) {
      if (function_exists($function = $typeinfo['can_create']) && $function($type) === TRUE) {
        $allowed_types[$type] = $typeinfo;
      }
    }
  }

  return $check_access ? $allowed_types : $types;
}

/**
 * Returns a list of available content fields for given
 * node_import type.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $reset
 *   Boolean. If TRUE, the internal cache is rebuilt.
 *
 * @return
 *   Array of fields. See hook_node_import_fields().
 */
function node_import_fields($type, $reset = FALSE) {
  static $fields;

  if (!isset($fields[$type]) || $reset) {
    $defaults = array(
      'title' => '',
      'group' => '',

      'is_mappable' => TRUE,
      'map_required' => FALSE,

      'has_multiple' => FALSE,
      'multiple_separator' => variable_get('node_import:multiple_separator', '||'),

      'has_hierarchy' => FALSE,
      'hierarchy_separator' => variable_get('node_import:hierarchy_separator', '>>'),
      'hierarchy_reverse' => FALSE,

      'input_format' => '',
      'preprocess' => array(),
      'allowed_values' => array(),

      'default_value' => NULL,
      'allow_empty' => FALSE,
      'is_required' => FALSE,
      'is_checkboxes' => FALSE,
    );

    $fields[$type] = (array)module_invoke_all('node_import_fields', $type);

    foreach ($fields[$type] as $fieldname => $fieldinfo) {
      // Merge sane defaults.
      $fields[$type][$fieldname] = array_merge($defaults, $fieldinfo);

      // Add preprocessors for builtin input_formats.
      if (!empty($fieldinfo['allowed_values'])) {
        $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_values';
      }

      switch ($fieldinfo['input_format']) {
        case 'user': // Could be in supported/user.inc instead.
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_user';
          break;

        case 'node': // Could be in supported/node.inc instead.
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_node';
          break;

        case 'date':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_date';
          break;

        case 'email':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_email';
          break;

        case 'weight':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_weight';
          break;

        case 'boolean':
          $fields[$type][$fieldname]['preprocess'][] = 'node_import_check_boolean';
          break;
      }
    }

    drupal_alter('node_import_fields', $fields[$type], $type);
  }

  return $fields[$type];
}

/**
 * Returns a list of default (form elements).
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $defaults
 *   Array of currently filled in values.
 *
 * @param $fields
 *   Array of available fields.
 *
 * @param $map
 *   Array of how fields are mapped.
 *
 * @return
 *   FAPI array. See hook_node_import_defaults().
 */
function node_import_defaults($type, $defaults, $fields, $map) {
  if (!is_array($defaults)) {
    $defaults = array();
  }

  $form = (array)module_invoke_all('node_import_defaults', $type, $defaults, $fields, $map);

  foreach ($fields as $fieldname => $fieldinfo) {
    // Set #node_import-group if not set.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#node_import-group'])) {
      $form[$fieldname]['#node_import-group'] = $fieldinfo['group'];
    }

    // Set default_value as value.
    if (!empty($fieldinfo['default_value']) && !isset($form[$fieldname])) {
      $form[$fieldname] = array(
        '#type' => 'value',
        '#value' => $fieldinfo['default_value'],
      );
    }
  }

  drupal_alter('node_import_defaults', $form, $type, $defaults, $fields, $map);

  return $form;
}

/**
 * Returns a list of options (form elements).
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $options
 *   Array of currently filled in values.
 *
 * @param $fields
 *   Array of available fields.
 *
 * @param $map
 *   Array of how fields are mapped.
 *
 * @return
 *   FAPI array. See hook_node_import_options().
 */
function node_import_options($type, $options, $fields, $map) {
  if (!is_array($options)) {
    $options = array();
  }

  $form = (array)module_invoke_all('node_import_options', $type, $options, $fields, $map);

  foreach ((array)$fields as $fieldname => $fieldinfo) {
    // Add the group so all wizard pages look the same.
    if (isset($form[$fieldname]) && !isset($form[$fieldname]['#node_import-group'])) {
      $form[$fieldname]['#node_import-group'] = $fieldinfo['group'];
    }

    $map_count = node_import_field_map_count($fieldname, $map);

    // Add multiple_separator option for fields that can have multiple
    // values AND that is mapped to exactly one file column.
    if ($fieldinfo['has_multiple'] &&
        $map_count == 1 &&
        (!isset($form[$fieldname]) || !isset($form[$fieldname]['multiple_separator']))) {

      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname]['multiple_separator'] = array(
        '#type' => 'textfield',
        '#title' => t('Multiple values are separated by'),
        '#size' => 6,
        '#default_value' => isset($options[$fieldname]['multiple_separator']) ? $options[$fieldname]['multiple_separator'] : $fieldinfo['multiple_separator'],
      );
    }

    // Add hierarchy_separator option for fields that can have
    // hierarchical values AND (that can have multiple values OR
    // that can not have multiple values but is mapped to exactly
    // one file column).
    if ($fieldinfo['has_hierarchy'] &&
        ($fieldinfo['has_multiple'] || $map_count == 1) &&
        $map_count > 0 &&
        (!isset($form[$fieldname]) || !isset($form[$fieldname]['hierarchy_separator']))) {

      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname]['hierarchy_separator'] = array(
        '#type' => 'textfield',
        '#title' => t('Hierarchy is specified by'),
        '#size' => 6,
        '#default_value' => isset($options[$fieldname]['hierarchy_separator']) ? $options[$fieldname]['hierarchy_separator'] : $fieldinfo['hierarchy_separator'],
      );
    }

    // Add hierarchy_reverse option for fields that can have
    // hierarchical values AND that can not have multiple values
    // AND that is mapped to more than one file column.
    if ($fieldinfo['has_hierarchy'] &&
        $map_count > 1 && !$fieldinfo['has_multiple'] &&
        (!isset($form[$fieldname]) || !isset($form[$fieldname]['hierarchy_reverse']))) {

      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      if ($map_count > 1 && !$fieldinfo['has_multiple']) {
        $form[$fieldname]['hierarchy_reverse'] = array(
          '#type' => 'checkbox',
          '#title' => t('Reverse file columns for hierarchy'),
          '#default_value' => isset($options[$fieldname]['hierarchy_reverse']) ? $options[$fieldname]['hierarchy_reverse'] : $fieldinfo['hierarchy_reverse'],
        );
      }
    }

    // Add a custom date format option for fields that are dates.
    if ($fieldinfo['input_format'] == 'date' && $map_count > 0) {
      if (!isset($form[$fieldname])) {
        $form[$fieldname] = array(
          '#title' => $fieldinfo['title'],
          '#node_import-group' => $fieldinfo['group'],
        );
      }

      $form[$fieldname]['date_custom'] = array(
        '#type' => 'textfield',
        '#title' => t('Date input format'),
        '#default_value' => isset($options[$fieldname]['date_custom']) ? $options[$fieldname]['date_custom'] : variable_get('date_format_short', 'm/d/Y - H::i'),
      );
    }
  }

  drupal_alter('node_import_options', $form, $type, $options, $fields, $map);

  return $form;
}

/**
 * Create an array of values to submit to the form.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $data
 *   Array of data from the file as ($col_index => $value).
 *
 * @param $map
 *   Array of how the data maps to fields.
 *
 * @param $defaults
 *   Array of default values.
 *
 * @param $options
 *   Array of options.
 *
 * @param $fields
 *   Array of available fields.
 *
 * @param $preview
 *   Boolean. If TRUE a preview will be created. If FALSE construct the
 *   values for real.
 *
 * @return
 *   Array of values to submit to a form. See hook_node_import_values().
 */
function node_import_values($type, $data, $map, $defaults, $options, $fields, $preview) {
  $values = module_invoke_all('node_import_values', $type, $defaults, $options, $fields, $preview);

  foreach ($fields as $fieldname => $fieldinfo) {
    $map_count = node_import_field_map_count($fieldname, $map);
    $mseparator = isset($options[$fieldname]['multiple_separator']) ? $options[$fieldname]['multiple_separator'] : $fieldinfo['multiple_separator'];
    $hseparator = isset($options[$fieldname]['hierarchy_separator']) ? $options[$fieldname]['hierarchy_separator'] : $fieldinfo['hierarchy_separator'];
    $hreverse = isset($options[$fieldname]['hierarchy_reverse']) ? $options[$fieldname]['hierarchy_reverse'] : $fieldinfo['hierarchy_reverse'];

    // Merge default value for each field.
    if (isset($defaults[$fieldname])) {
      if ($fieldinfo['is_checkboxes']) {
        $values[$fieldname] = array_keys(array_filter($defaults[$fieldname]));
      }
      else {
        $values[$fieldname] = $defaults[$fieldname];
      }
    }

    // Map the data ONLY IF the data to map is not empty.
    if ($fieldinfo['has_multiple']) {
      if ($map_count > 0) {
        $fieldvalues = array();
        foreach ($map[$fieldname] as $col) {
          $value = isset($data[$col]) ? $data[$col] : '';
          if ($map_count == 1) {
            $fieldvalues = strlen($value) > 0 ? array_map('trim', explode($mseparator, $value)) : array();
            break;
          }
          $fieldvalues[] = $value;
        }

        if (!$fieldinfo['allow_empty']) {
          $fieldvalues = array_filter($fieldvalues, 'strlen');
        }

        if ($fieldinfo['has_hierarchy']) {
          foreach ($fieldvalues as $i => $value) {
            $fieldvalues[$i] = strlen($value) > 0 ? array_map('trim', explode($hseparator, $value)) : array($value);
          }
        }

        $values[$fieldname] = empty($fieldvalues) ? $values[$fieldname] : $fieldvalues;
      }
    }
    else {
      if ($map_count > 0 && $fieldinfo['has_hierarchy']) {
        $fieldvalues = array();
        foreach ($map[$fieldname] as $col) {
          $value = isset($data[$col]) ? $data[$col] : '';
          if ($map_count == 1) {
            $fieldvalues = strlen($value) > 0 ? array_map('trim', explode($hseparator, $value)) : array();
            break;
          }
          $fieldvalues[] = $value;
        }

        if (!$fieldinfo['allow_empty']) {
          $fieldvalues = array_filter($fieldvalues, 'strlen');
        }

        if ($hreverse) {
          $fieldvalues = array_reverse($fieldvalues);
        }

        $values[$fieldname] = empty($fieldvalues) ? $values[$fieldname] : $fieldvalues;
      }
      else if ($map_count == 1) {
        $value = isset($data[$map[$fieldname]]) ? $data[$map[$fieldname]] : '';
        $values[$fieldname] = strlen($value) > 0 ? $value : $values[$fieldname];
      }

      $values[$fieldname] = array($values[$fieldname]);
    }

    // Preprocess the data as long as the value is not empty and it
    // validates.
    foreach ((array)$values[$fieldname] as $i => $value) {
      foreach ($fieldinfo['preprocess'] as $function) {
        if (strlen($value) > 0) {
          $return = $function($values[$fieldname][$i], $fieldinfo, $options[$fieldname], $preview);
          if ($return === FALSE) {
            $values[$fieldname][$i] = '';
          }
          if ($return === TRUE) {
            continue 2;
          }
        }
      }
    }

    // If empty values are not allowed, filter them out.
    if (!$fieldinfo['allow_empty']) {
      $values[$fieldname] = array_filter((array)$values[$fieldname], 'strlen');
    }

    // If only a single value is allowed, get the first one.
    if (!$fieldinfo['has_multiple']) {
      $values[$fieldname] = array_shift($values[$fieldname]);
    }

    // Convert checkboxes fields to a format FAPI understands.
    if ($fieldinfo['is_checkboxes'] && !empty($values[$fieldname])) {
      // Only in PHP > 5.2: $values[$fieldname] = array_fill_keys($values[$fieldname], 1);
      $values[$fieldname] = array_combine($values[$fieldname], array_fill(0, count($values[$fieldname]), 1));
    }
  }

  drupal_alter('node_import_values', $values, $type, $defaults, $options, $fields, $preview);

  return $values;
}

/**
 * @}
 */

/**
 * Import a number of rows from all available tasks. Should only be called
 * from within hook_cron() or from a JS callback as this function may take
 * a long time.
 *
 * The function ends when $count $units have been finished. For example
 * @code
 * node_import_do_all_tasks('all');
 * node_import_do_all_tasks('rows', 10);
 * node_import_do_all_tasks('bytes', 4096);
 * node_import_do_all_tasks('ms', 1000);
 * @endcode
 *
 * @param $unit
 *   String. Either 'rows', 'bytes', 'ms' (milliseconds) or 'all'.
 *   Defaults to 'all'.
 *
 * @param $count
 *   Integer. Number of $units to do. Defaults to 0 (in which case
 *   exactly one row will be imported if $unit != 'all').
 *
 * @return
 *   Boolean. See node_import_do_task().
 */
function node_import_do_all_tasks($unit = 'all', $count = 0) {
  $can_continue = TRUE;

  $byte_count = 0;
  $row_count = 0;
  timer_start('node_import:do_all_tasks');

  foreach (node_import_list_tasks(TRUE) as $taskid => $task) {
    $bytes = $task['offset'];
    $rows = $task['row_done'] + $task['row_error'];

    $can_continue = node_import_do_task($task, $unit, $count);

    $byte_count += $task['offset'] - $bytes;
    $row_count += $task['row_done'] + $task['row_error'] - $rows;

    if ($can_continue && ($unit == 'all'
        || ($unit == 'bytes' && $byte_count < $count)
        || ($unit == 'rows' && $row_count < $count)
        || ($unit == 'ms' && timer_read('node_import:do_all_tasks') < $count))) {
      continue;
    }

    break;
  }

  timer_stop('node_import:do_all_tasks');
  return $can_continue;
}

/**
 * Import a number of rows from the specified task. Should only be called
 * from within hook_cron() or from a JS callback as this function may take
 * a long time.
 *
 * The function ends when $count $units have been finished. For example
 * @code
 * node_import_do_task($task, 'all');
 * node_import_do_task($task, 'rows', 10);
 * node_import_do_task($task, 'bytes', 4096);
 * node_import_do_task($task, 'ms', 1000);
 * @endcode
 *
 * @param $task
 *   Array. The task to continue. Note that this is passed by reference!
 *   So you can check the status of the task after running this function
 *   without having to query the database.
 *
 * @param $unit
 *   String. Either 'rows', 'bytes', 'ms' (milliseconds) or 'all'.
 *   Defaults to 'all'.
 *
 * @param $count
 *   Integer. Number of $units to do. Defaults to 0 (in which case
 *   exactly one row will be imported if $unit != 'all').
 *
 * @return
 *   Boolean. Some imports may not be able to do more then one row at a
 *   time because of all kinds of (static) caching in Drupal. For example
 *   if you import a node with taxonomy terms and select to create the
 *   non-existing terms on import, taxonomy_get_tree() will not have an
 *   up-to-date internal list of terms and so the submission of the form
 *   will fail due to "An illegal choice has been detected. Please contact
 *   the site administrator.".
 *
 *   The status of each imported row (error or not) is stored in the
 *   database. @see node_import_constants.
 */
function node_import_do_task(&$task, $unit = 'all', $count = 0) {
  $can_continue = TRUE;

  if ($task['status'] != NODE_IMPORT_STATUS_DONE && node_import_lock_acquire()) {
    global $user;
    $backup_user = $user;
    if ($task['uid'] != $user->uid) {
      session_save_session(FALSE);
      $user = user_load(array('uid' => $task['uid']));
    }

    $taskid = $task['taskid'];

    $byte_count = 0;
    $row_count = 0;
    timer_start('node_import:do_task:'. $taskid);

    $data = array();
    switch ($task['status']) {
      case NODE_IMPORT_STATUS_PENDING:
        if ($task['offset'] == 0 && $task['has_headers']) {
          list($offset, $data) = node_import_read_from_file($task['file']->filepath, $offset, $task['file_options']);
        }
        else {
          $offset = $task['offset'];
        }
        break;

      case NODE_IMPORT_STATUS_ERROR:
        $task['status'] = NODE_IMPORT_STATUS_DONE;
        $offset = $task['file']->filesize; //TODO
        break;
    }

    while ($task['status'] != NODE_IMPORT_STATUS_DONE) {
      list($new_offset, $data) = node_import_read_from_file($task['file']->filepath, $offset, $task['file_options']);

      if (is_array($data)) {
        switch ($task['status']) {
          case NODE_IMPORT_STATUS_PENDING:
            db_query("DELETE FROM {node_import_status} WHERE taskid = %d AND offset = %d", $taskid, $offset);
            db_query("INSERT INTO {node_import_status} (taskid, offset, errors) VALUES (%d, %d, '%s')", $taskid, $offset, serialize(array()));
            break;

          case NODE_IMPORT_STATUS_ERROR:
            db_query("UPDATE {node_import_status} SET errors = '%s', status = %d WHERE taskid = %d AND offset = %d", serialize(array()), NODE_IMPORT_STATUS_PENDING, $taskid, $offset);
            break;
        }

        db_query("UPDATE {node_import_tasks} SET offset = %d, changed = %d WHERE taskid = %d", $new_offset, time(), $taskid);
        $task['offset'] = $new_offset;

        list($can_continue, $errors) = node_import_create($task['type'], $data, $task['map'], $task['defaults'], $task['options'], FALSE);

        if (is_array($errors)) {
          db_query("UPDATE {node_import_status} SET status = %d, errors = '%s' WHERE taskid = %d AND offset = %d", NODE_IMPORT_STATUS_ERROR, serialize($errors), $taskid, $offset);
          db_query("UPDATE {node_import_tasks} SET row_error = row_error + 1 WHERE taskid = %d", $taskid);
          $task['row_error']++;
        }
        else {
          db_query("UPDATE {node_import_status} SET status = %d, objid = %d WHERE taskid = %d AND offset = %d", NODE_IMPORT_STATUS_DONE, $errors, $taskid, $offset);
          db_query("UPDATE {node_import_tasks} SET row_done = row_done + 1 WHERE taskid = %d", $taskid);
          $task['row_done']++;
        }

        $byte_count += $new_offset - $offset;
        $row_count++;
      }
      else {
        db_query("UPDATE {node_import_tasks} SET status = %d, offset = %d WHERE taskid = %d", NODE_IMPORT_STATUS_DONE, $task['file']->filesize, $taskid);
        $task['status'] = NODE_IMPORT_STATUS_DONE;
        $task['offset'] = $task['file']->filesize;
      }

      switch ($task['status']) {
        case NODE_IMPORT_STATUS_PENDING:
          $offset = $new_offset;
          break;

        case NODE_IMPORT_STATUS_ERROR:
          $offset = $task['file']->filesize; //TODO
          break;
      }

      if ($can_continue && ($unit == 'all'
          || ($unit == 'bytes' && $byte_count < $count)
          || ($unit == 'rows' && $row_count < $count)
          || ($unit == 'ms' && timer_read('node_import:do_task:'. $taskid) < $count))) {
        continue;
      }

      break;
    }

    // Cleanup before exit.
    $user = $backup_user;
    session_save_session(TRUE);
    timer_stop('node_import:do_task:'. $taskid);
    node_import_lock_release();
  }

  return $can_continue;
}

/**
 * JS callback to continue the specified task and returns the status
 * of it. This function will take at most one second.
 *
 * @param $task
 *   Full loaded $task.
 *
 * @return
 *   JSON.
 */
function node_import_js($task) {
  node_import_do_task($task, 'ms', 1000);
  echo drupal_json(array(
    'status' => 1,
    'message' => format_plural($task['row_done'], t('1 row imported'), t('@count rows imported')) .'<br />'.
                 format_plural($task['row_error'], t('1 row with errors'), t('@count rows with errors')),
    'percentage' => $task['status'] == NODE_IMPORT_STATUS_DONE ? 100 : round(floor(100.0 * $task['offset'] / $task['file']->filesize), 0),
  ));
  exit();
}

/**
 * Create a new object of specified $type.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $data
 *   Array of data from the file as ($col_index => $value).
 *
 * @param $map
 *   Array of how the data maps to fields.
 *
 * @param $defaults
 *   Array of default values.
 *
 * @param $options
 *   Array of options.
 *
 * @param $preview
 *   Boolean.
 *
 * @return
 *   array($can_continue, $output).
 *
 *   For an explanation of $can_continue, see node_import_do_task().
 *
 *   The $output is: if $preview is TRUE, a string with the preview of
 *   the object is returned. If $preview is FALSE, an unique identifier
 *   is returned or an array with errors if failed.
 */
function node_import_create($type, $data, $map, $defaults, $options, $preview) {
  $can_continue = TRUE;
  $output = $preview ? '' : array();

  // Reset execution time. Note that this only works when SAFE_MODE is OFF but
  // it has no side-effects if SAFE_MODE is ON. See
  // http://php.net/manual/en/function.set-time-limit.php
  set_time_limit(variable_get('node_import:set_time_limit', 60));

  $types = node_import_types();
  $fields = node_import_fields($type);

  // We need to clean out the form errors before submitting.
  form_set_error(NULL, '', TRUE);

  // Create a list of values to submit.
  $values = node_import_values($type, $data, $map, $defaults, $options, $fields, $preview);

  // Submit it for preview or creation.
  if (function_exists($function = $types[$type]['create'])) {
    list($can_continue, $output) = $function($type, $values, $preview);
  }

  // Check for errors and clear them again.
  if ($preview) {
    $output = theme('status_messages') . $output;
  }
  if (($errors = form_get_errors())) {
    if ($preview) {
      $output .= '<pre>values = '. print_r($values, TRUE) .'</pre>'; //TODO: show data instead?
    }
    else {
      $output = $errors;
    }
  }
  form_set_error(NULL, '', TRUE);
  drupal_get_messages(NULL, TRUE); // Otherwise they are still showed.

  return array($can_continue, $output);
}

/**
 * @defgroup node_import_tasks Node import tasks
 * @{
 */

/**
 * Create a new import task.
 *
 * @param $values
 *   Array of filled in values.
 *
 * @return
 *   Integer. Unique identifier for the task or FALSE if the task could
 *   not be saved to the database.
 */
function node_import_save_task($values) {
  global $user;

  if (!isset($values['uid'])) {
    $values['uid'] = $user->uid;
  }
  if (!isset($values['created'])) {
    $values['created'] = time();
  }
  if (!isset($values['changed'])) {
    $values['changed'] = time();
  }

  if (drupal_write_record('node_import_tasks', $values) === SAVED_NEW) {
    return $values['taskid'];
  }
  return FALSE;
}

/**
 * Get a list of available tasks.
 *
 * @param $all
 *   Boolean. If TRUE, all tasks are returned. If FALSE, only the tasks
 *   the current user has access to.
 *
 * @return
 *   Array of tasks.
 */
function node_import_list_tasks($all = FALSE) {
  global $user;
  $tasks = array();

  if ($all || user_access('administer imports')) {
    $result = db_query("SELECT * FROM {node_import_tasks} ORDER BY created ASC");
  }
  else {
    $result = db_query("SELECT * FROM {node_import_tasks} WHERE uid = %d ORDER BY created ASC", $user->uid);
  }

  while (($task = db_fetch_array($result))) {
    foreach (array('file_options', 'headers', 'map', 'defaults', 'options') as $key) {
      $task[$key] = isset($task[$key]) ? unserialize($task[$key]) : array();
    }

    $task['file'] = db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $task['fid']));
    $tasks[$task['taskid']] = $task;
  }

  return $tasks;
}

/**
 * Delete an import task.
 *
 * @param $taskid
 *   Unique identifier.
 *
 * @return
 *   Nothing.
 */
function node_import_delete_task($taskid) {
  db_query("DELETE FROM {node_import_tasks} WHERE taskid = %d", $taskid);
  db_query("DELETE FROM {node_import_status} WHERE taskid = %d", $taskid);
}

/**
 * @}
 */

/**
 * @defgroup node_import_preprocess Node import preprocess functions
 * @{
 */

/**
 * Check if the value is in the list of allowed values (by key or value).
 *
 * Uses: $field['allowed_values'].
 */
function node_import_check_values(&$value, $field, $options, $preview) {
  foreach ($field['allowed_values'] as $key => $title) {
    $tmp = drupal_strtolower($value);
    if ($tmp === drupal_strtolower($key) || $tmp === drupal_strtolower($title)) {
      $value = $key;
      return TRUE;
    }
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not in allowed values list).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid boolean (1, 0, true, false, yes, no, on, off).
 *
 * Uses: nothing.
 */
function node_import_check_boolean(&$value, $field, $options, $preview) {
  static $trues;
  static $falses;

  if (!isset($trues)) {
    $trues = array(
      '1',
      'on', drupal_strtolower(t('On')),
      'yes', drupal_strtolower(t('Yes')),
      'true', drupal_strtolower(t('True')),
    );
    $falses = array(
      '0',
      'off', drupal_strtolower(t('Off')),
      'no', drupal_strtolower(t('No')),
      'false', drupal_strtolower(t('False')),
    );
  }

  if (in_array($value, $trues, TRUE)) {
    $value = '1';
    return TRUE;
  }
  else if (in_array($value, $falses, TRUE)) {
    $value = '0';
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a boolean).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid weight (integer between -X and X).
 *
 * Uses: $field['max_weight'].
 */
function node_import_check_weight(&$value, $field, $options, $preview) {
  $weight = isset($field['max_weight']) ? $field['max_weight'] : 10;

  if (is_numeric($value) && intval($value) <= $weight && intval($value) >= -$weight) {
    $value = intval($value);
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a weight).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid date.
 *
 * Uses: $field['date_format'] (output format type - defaults to DATE_DATETIME).
 * Uses: $options['date_custom'] (custom input format).
 */
function node_import_check_date(&$value, $field, $options, $preview) {
  static $timezone;
  if (!isset($timezone)) {
    $timezone = date_default_timezone_name();
  }

  $date_format = isset($field['date_format']) ? $field['date_format'] : DATE_DATETIME;

  if (date_is_valid($value, DATE_DATETIME)) {
    $value = date_convert($value, DATE_DATETIME, $date_format, $timezone);
    return TRUE;
  }

  if (date_is_valid($value, DATE_ISO)) {
    $value = date_convert($value, DATE_ISO, $date_format, $timezone);
    return TRUE;
  }

  if (isset($options['date_custom']) && strlen($options['date_custom']) > 0
      && ($date = date_convert_from_custom($value, $options['date_custom']))) {
    // It is useless to check for date_is_valid() as it is a DATE_DATETIME already.
    $value = date_convert($date, DATE_DATETIME, $date_format, $timezone);
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a date).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid email address.
 *
 * Uses: nothing.
 */
function node_import_check_email(&$value, $field, $options, $preview) {
  if (!valid_email_address($value)) {
    node_import_input_error(t('Input error: %value is not a valid e-mail address.', array('%value' => $value)));
    return FALSE;
  }
  return TRUE;
}

/**
 * Check if the value is a valid user (by uid, name or email).
 *
 * Uses: nothing.
 */
function node_import_check_user(&$value, $field, $options, $preview) {
  if (($uid = db_result(db_query("SELECT uid FROM {users} WHERE uid = %d OR LOWER(name) = '%s' OR LOWER(mail) = '%s'", is_numeric($value) && intval($value) > 0 ? $value : -1, drupal_strtolower($value), drupal_strtolower($value))))) {
    $value = $uid;
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not an user).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Check if the value is a valid node reference (by nid or title).
 *
 * Uses: nothing.
 */
function node_import_check_node(&$value, $field, $options, $preview) {
  if (is_numeric($value) && intval($value) > 0) {
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a node reference).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * @}
 */

/**
 * Get a property from each element of an array.
 *
 * @param $array
 *   Array of ($key => $info).
 *
 * @param $property
 *   String.
 *
 * @return
 *   Array of ($key => $info->$property).
 */
function node_import_extract_property($array, $property = 'title') {
  $result = array();
  foreach ((array)$array as $key => $info) {
    if (is_array($info) || is_object($info)) {
      $info = (array)$info;
      $result[$key] = isset($info[$property]) ? $info[$property] : '';
    }
    else {
      $result[$key] = $info;
    }
  }
  return $result;
}

/**
 * Returns the number of columns given $fieldname is mapped
 * to.
 *
 * @param $fieldname
 *   String.
 *
 * @param $map
 *   Array of file column mapping.
 *
 * @return
 *   Integer. Number of file columns the field is mapped to.
 */
function node_import_field_map_count($fieldname, $map) {
  if (!isset($map[$fieldname])) {
    return 0;
  }
  if (!is_array($map[$fieldname])) {
    return strlen($map[$fieldname]) > 0;
  }
  $count = 0;
  foreach ($map[$fieldname] as $col) {
    if ($col !== '') {
      $count++;
    }
  }
  return $count;
}

/**
 * Set an error on a random form element.
 */
function node_import_input_error($message) {
  static $count = 0;
  form_set_error('node_import-'. $count, $message, $args);
  $count++;
}

/**
 * @defgroup node_import_files Node import file functions
 * @{

/**
 * Returns a list of available files.
 */
function node_import_list_files($reset = FALSE) {
  global $user;
  static $files;

  if (!isset($files) || $reset) {
    $files = array();

    // If FTP uploads of files is allowed, rescan the directory.
    if (variable_get('node_import:ftp:enabled', 0)) {
      $path = file_create_path(variable_get('node_import:ftp:directory', 'imports'));
      $ftp_user = user_load(array('name' => variable_get('node_import:ftp:user', '')));
      //TODO: use the $validators functionality of file_save_upload() ?
      $extensions = array_filter(explode(' ', variable_get('node_import:ftp:extensions', 'csv tsv txt')));

      if (!empty($extensions)) {
        $existing_files = array();
        $result = db_query("SELECT filepath FROM {files} WHERE filepath LIKE '%s%%'", $path);
        while (($file = db_fetch_object($result))) {
          $existing_files[$file->filepath] = TRUE;
        }
        foreach (file_scan_directory($path, '.*\.(('. implode(')|(', $extensions) .'))') as $filename => $file) {
          if (!isset($existing_files[$file->filename])) {
            $record = (object)array(
              'uid' => $ftp_user->uid,
              'filename' => $file->basename,
              'filepath' => $file->filename,
              'filemime' => 'text/plain', //TODO: how to get real MIME?
              'filesize' => filesize($file->filename),
              'status' => FILE_STATUS_PERMANENT,
              'timestamp' => time(),
            );
            drupal_write_record('files', $record);
          }
        }
      }
    }

    // Users with 'administer imports' permission can see all files.
    //TODO: we should also filter out files that are already in use by another task.
    if (user_access('administer imports')) {
      $result = db_query("SELECT * FROM {files} WHERE filepath LIKE '%s%%' ORDER BY filename, timestamp", $path);
    }
    else {
      $result = db_query("SELECT * FROM {files} WHERE filepath LIKE '%s%%' AND (uid = %d OR uid = 0) ORDER BY filename, timestamp", $path, $user->uid);
    }
    while (($file = db_fetch_object($result))) {
      $files[$file->fid] = $file;
    }
  }

  return $files;
}

/**
 * Return an autodetected mapping for given headers and content type.
 *
 * The automapping is done by checking the column titles in the file,
 * whether they match with the field name or field title.
 *
 * @param $type
 *   String. The node_import type.
 *
 * @param $headers
 *   Array of column titles in the file.
 *
 * @return
 *   Array of mapping.
 */
function node_import_automap($type, $headers) {
  if (user_access('administer imports')) {
    $result = db_query("SELECT map FROM {node_import_tasks} WHERE type = '%s' AND LOWER(headers) = '%s' ORDER BY created DESC LIMIT 1", $type, strtolower(serialize($headers)));
  }
  else {
    $result = db_query("SELECT map FROM {node_import_tasks} WHERE type = '%s' AND LOWER(headers) = '%s' AND uid = %d ORDER BY created DESC LIMIT 1", $type, strtolower(serialize($headers)), $user->uid);
  }
  if (($map = db_result($result))) {
    return unserialize($map);
  }

  $map = array();
  $headers = array_map('strtolower', $headers);

  foreach (node_import_fields($type) as $field => $info) {
    if ($info['is_mappable']) {
      $map[$field] = '';
      if (($col = array_search(strtolower($field), $headers)) !== FALSE
          || ($col = array_search(strtolower($info->title), $headers))) {
        $map[$field] = $col;
      }
    }
  }

  return $map;
}

/**
 * Return an autodetected file format and file options for given
 * file.
 *
 * @param $filepath
 *   String. Path to file.
 *
 * @return
 *   Array of file options.
 */
function node_import_autodetect($filepath) {
  //TODO: really implement this.
  $file_formats = (array)module_invoke_all('node_import_file_formats', 'file formats');
  return $file_formats['csv'] + array('file_format' => 'csv');
}

/**
 * Returns one record from the file starting at offset using
 * the supplied file options.
 *
 * @param $filepath
 *   String. Path to file.
 *
 * @param $offset
 *   Integer. Starting point of record.
 *
 * @param $file_options
 *   Array with 'record separator', 'field separator', 'text delimiter'
 *   and 'escape character'. If not set, the options default to the
 *   CSV options ("\n", ',', '"', '"').
 *
 * @return
 *   Array ($offset, $record). The $offset is the start offset of the
 *   next record. The $record is an array of fields (strings).
 *
 *   On error or when the end of the file has been reached we return
 *   FALSE.
 */
function node_import_read_from_file($filepath, $offset, $file_options) {
  // Open file and set to file offset.
  if (($fp = fopen($filepath, 'r')) === FALSE) {
    return FALSE;
  }
  if (fseek($fp, $offset)) {
    return FALSE;
  }

  // File options.
  $replaces = array('<newline>' => "\n", '<tab>' => "\t", '<none>' => '');
  $options = array(
    'record separator' => '<newline>',
    'field separator' => ',',
    'text delimiter' => '"',
    'escape character' => '"',
  );
  foreach ($options as $key => $default) {
    if (isset($file_options[$key]) && strlen($file_options[$key]) > 0) {
      $options[$key] = $file_options[$key];
    }
    else if (isset($file_options['other '. $key]) && strlen($file_options['other '. $key]) > 0) {
      $options[$key] = $file_options['other '. $key];
    }
  }
  $options = str_replace(array_keys($replaces), array_values($replaces), $options);
  $rs = $options['record separator'];
  $fs = $options['field separator'];
  $td = $options['text delimiter'];
  $ec = $options['escape character'];

  // The current record is stored in the $fields array. The $new_offset
  // contains the file position of the end of the returned record. Note
  // that if $new_offset == $offset we have reached the end of the file.
  $fields = array();
  $new_offset = $offset;
  $start = 0;

  // We read $length bytes at a time in the $buffer.
  $length = variable_get('node_import:fgets:length', 1024);
  $buffer = '';

  // A field can be enclosed in text delimiters or not. If this variable is
  // TRUE, we need to parse until we find the next unescaped text delimiter.
  // If FALSE, the field value was not enclosed.
  $enclosed = FALSE;

  // Read until the EOF or until end of record.
  while (!feof($fp) || $start < strlen($buffer)) {
    // Read some more data into the $buffer.
    $buffer .= fgets($fp, $length);

    if (!$enclosed) {
      // Find the next record separator, field separator and text delimiter.
      $pos_rs = strpos($buffer, $rs, $start);
      $pos_fs = strpos($buffer, $fs, $start);
      $pos_td = strlen($td) ? strpos($buffer, $td, $start) : FALSE;

      // Check for begin of text delimited field.
      if ($pos_td !== FALSE && ($pos_rs === FALSE || $pos_td <= $pos_rs) && ($pos_fs === FALSE || $pos_td <= $pos_fs)) {
        $enclosed = TRUE;
        $buffer = substr($buffer, 0, $pos_td) . substr($buffer, $pos_td + strlen($td));
        $new_offset += strlen($td);
        $start = $pos_td;
        continue;
      }

      // Check for end of record.
      if ($pos_rs !== FALSE && ($pos_fs === FALSE || $pos_rs <= $pos_fs)) {
        if ($pos_rs > 0) {
          $fields[] = substr($buffer, 0, $pos_rs);
          $buffer = '';
          $new_offset += $pos_rs;
          $start = 0;
        }
        else if (empty($fields)) {
          $buffer = substr($buffer, strlen($rs), strlen($buffer) - strlen($rs));
          $new_offset += strlen($rs);
          $start = 0;
          continue;
        }
        $new_offset += strlen($rs);
        break;
      }

      // Check for end of field.
      if ($pos_fs !== FALSE) {
        $fields[] = substr($buffer, 0, $pos_fs);
        $buffer = substr($buffer, $pos_fs + strlen($fs));
        $new_offset += $pos_fs + strlen($fs);
        $start = 0;
        continue;
      }
    }
    else {
      // Find the next text delimiter and escaped text delimiter.
      $pos_td = strpos($buffer, $td, $start);
      $pos_ec = strpos($buffer, $ec . $td, $start);

      // Check for end of text delimited field.
      if ($pos_td !== FALSE && ($pos_ec === FALSE || $pos_td <= ($pos_ec - strlen($td)))) {
        $enclosed = FALSE;
        $buffer = substr($buffer, 0, $pos_td) . substr($buffer, $pos_td + strlen($td));
        $new_offset += strlen($td);
        $start = $pos_td;
        continue;
      }

      // Check for escaped text delimiter.
      if ($pos_ec !== FALSE) {
        $buffer = substr($buffer, 0, $pos_ec) . substr($buffer, $pos_ec + strlen($ec));
        $new_offset += strlen($ec);
        $start = $pos_ec + strlen($td);
        continue;
      }
    }

    // Nothing found... read more data.
    $start = strlen($buffer);
  }

  // Check if we need to add the last field.
  if (feof($fp) && strlen($buffer) > 0) {
    $fields[] = $buffer;
    $new_offset += strlen($buffer);
  }

  // Cleanup and return.
  $fields = array_map('trim', $fields);
  $result = (!feof($fp) || !empty($fields)) ? array($new_offset, $fields) : FALSE;
  unset($buffer);
  fclose($fp);

  return $result;
}

/**
 * @}
 */

/**
 * @defgroup node_import_form_hacks Hacks for includes/form.inc
 * @{
 * The problem node_import has by design - where the design is that we
 * want to use the normal form validation - is that when a form is
 * submitted more then once, the validation of the form is not done
 * correctly by the core includes/form.inc
 *
 * This file tries to lift this limitation. The solution is based (or
 * rather copied) from sites/all/modules/views/includes/form.inc of
 * the views module which needs to do the same crappy stuff (and even
 * more).
 *
 * Short explanation: instead of
 * @code
 *   drupal_execute($form_id, $form_state, ...);
 * @endcode
 * use
 * @code
 *   node_import_drupal_execute($form_id, $form_state, ...);
 * @endcode
 * whenever the form you want to execute can be executed more than
 * once in the same page request.
 *
 * For the core bug, see http://drupal.org/node/260934 : Static caching:
 * cannot call drupal_validate_form on the same form more than once.
 *
 * Note that another bug for multiple form validation and submission
 * (one that could not be lifted) was fixed in Drupal 6.5, see
 * http://drupal.org/node/180063 : No way to flush form errors during
 * iterative programatic form submission.
 *
 * Many, many thanks to merlinofchaos!!
 */

/**
 * The original version of drupal_execute() calls drupal_process_form().
 * The modified version sets $form_state['must_validate'] = TRUE and
 * calls node_import_drupal_process_form() instead.
 */
function node_import_drupal_execute($form_id, &$form_state) {
  $args = func_get_args();
  $form = call_user_func_array('drupal_retrieve_form', $args);
  $form['#post'] = $form_state['values'];
  $form_state['must_validate'] = TRUE;
  drupal_prepare_form($form_id, $form, $form_state);
  node_import_drupal_process_form($form_id, $form, $form_state);
}

/**
 * The original version of drupal_process_form() calls drupal_validate_form().
 * The modified version calls node_import_drupal_validate_form() instead.
 */
function node_import_drupal_process_form($form_id, &$form, &$form_state) {
  $form_state['values'] = array();

  $form = form_builder($form_id, $form, $form_state);
  // Only process the form if it is programmed or the form_id coming
  // from the POST data is set and matches the current form_id.
  if ((!empty($form['#programmed'])) || (!empty($form['#post']) && (isset($form['#post']['form_id']) && ($form['#post']['form_id'] == $form_id)))) {
    node_import_drupal_validate_form($form_id, $form, $form_state);

    // form_clean_id() maintains a cache of element IDs it has seen,
    // so it can prevent duplicates. We want to be sure we reset that
    // cache when a form is processed, so scenerios that result in
    // the form being built behind the scenes and again for the
    // browser don't increment all the element IDs needlessly.
    form_clean_id(NULL, TRUE);

    if ((!empty($form_state['submitted'])) && !form_get_errors() && empty($form_state['rebuild'])) {
      $form_state['redirect'] = NULL;
      form_execute_handlers('submit', $form, $form_state);

      // We'll clear out the cached copies of the form and its stored data
      // here, as we've finished with them. The in-memory copies are still
      // here, though.
      if (variable_get('cache', CACHE_DISABLED) == CACHE_DISABLED && !empty($form_state['values']['form_build_id'])) {
        cache_clear_all('form_'. $form_state['values']['form_build_id'], 'cache_form');
        cache_clear_all('storage_'. $form_state['values']['form_build_id'], 'cache_form');
      }

      // If batches were set in the submit handlers, we process them now,
      // possibly ending execution. We make sure we do not react to the batch
      // that is already being processed (if a batch operation performs a
      // drupal_execute).
      if ($batch =& batch_get() && !isset($batch['current_set'])) {
        // The batch uses its own copies of $form and $form_state for
        // late execution of submit handers and post-batch redirection.
        $batch['form'] = $form;
        $batch['form_state'] = $form_state;
        $batch['progressive'] = !$form['#programmed'];
        batch_process();
        // Execution continues only for programmatic forms.
        // For 'regular' forms, we get redirected to the batch processing
        // page. Form redirection will be handled in _batch_finished(),
        // after the batch is processed.
      }

      // If no submit handlers have populated the $form_state['storage']
      // bundle, and the $form_state['rebuild'] flag has not been set,
      // we're finished and should redirect to a new destination page
      // if one has been set (and a fresh, unpopulated copy of the form
      // if one hasn't). If the form was called by drupal_execute(),
      // however, we'll skip this and let the calling function examine
      // the resulting $form_state bundle itself.
      if (!$form['#programmed'] && empty($form_state['rebuild']) && empty($form_state['storage'])) {
        drupal_redirect_form($form, $form_state['redirect']);
      }
    }
  }
}

/**
 * The original version of drupal_validate_form() keeps a static array
 * of validated forms. The modified version checks $form_state['must_validate']
 * to see if the form needs validation. If set and TRUE, validation is
 * forced even if it was already done.
 */
function node_import_drupal_validate_form($form_id, $form, &$form_state) {
  static $validated_forms = array();

  if (isset($validated_forms[$form_id]) &&
      (!isset($form_state['must_validate']) || $form_state['must_validate'] !== TRUE)) { //Changed!
    return;
  }

  // If the session token was set by drupal_prepare_form(), ensure that it
  // matches the current user's session.
  if (isset($form['#token'])) {
    if (!drupal_valid_token($form_state['values']['form_token'], $form['#token'])) {
      // Setting this error will cause the form to fail validation.
      form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    }
  }

  _form_validate($form, $form_state, $form_id);
  $validated_forms[$form_id] = TRUE;
}

/**
 * @}
 */

/**
 * @defgroup node_import_locking Node import locking functions
 * @{
 * Locking functions for node_import. This code is based on #251792
 * (Implement a locking framework for long operations). We need
 * locking to avoid both hook_cron() and node_import_view_form() to
 * process the same task at the same time (which would result in
 * rows being processed twice).
 *
 * The code below uses the database-based locking mechanism. Note
 * that is does not use the full implementation as in the patch
 * because we can have only one global lock. One lock for each task
 * does not work because a task can spawn the creation of something
 * else (such as a taxonomy term).
 *
 * We only need one _acquire() and _release() function.
 *
 * When a more general locking framework is committed to Drupal,
 * we can easily replace this.
 */

/**
 * Acquire or release our node_import lock.
 *
 * @param $release
 *   Boolean. If TRUE, release the lock. If FALSE, acquire the
 *   lock.
 *
 * @return
 *   Boolean. Whether the lock was acquired.
 */
function node_import_lock_acquire($release = FALSE) {
  static $lock_id, $locked;

  if (!isset($lock_id)) {
    $lock_id = md5(uniqid());
    $locked = FALSE;
    register_shutdown_function('node_import_lock_release');
  }

  if ($release) {
    db_query("DELETE FROM {variable} WHERE name = '%s'", 'node_import:lock');
    $locked = FALSE;
  }
  else if (!$locked) {
    if (@db_query("INSERT INTO {variable} (name, value) VALUES ('%s', '%s')", 'node_import:lock', $lock_id)) {
      $locked = TRUE;
    }
  }

  return $locked;
}

/**
 * Release our node_import lock.
 *
 * @return
 *   Nothing.
 */
function node_import_lock_release() {
  node_import_lock_acquire(TRUE);
}

/**
 * @}
 */

