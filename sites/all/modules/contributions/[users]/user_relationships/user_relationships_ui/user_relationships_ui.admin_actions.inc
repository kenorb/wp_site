<?php
// $Id: user_relationships_ui.admin_actions.inc,v 1.1.2.9 2009/01/12 14:28:05 jaydub Exp $
/**
 * @file
 * User Relationships admin submit handlers
 */

/**
 * Validate the submitted relationship type
 */
function user_relationships_ui_type_edit_validate($form, &$form_state) {
  // My god this is ugly
  if (
    (isset($form_state['values']['rtid']) &&
      db_result(db_query(
        "SELECT name FROM {user_relationship_types}
         WHERE LOWER(name) = LOWER('%s')
           AND rtid != %d",
        $form_state['values']['name'], $form_state['values']['rtid'])
      )
    ) ||
    (!isset($form_state['values']['rtid']) &&
      db_result(db_query("SELECT name FROM {user_relationship_types} WHERE LOWER(name) = LOWER('%s')", $form_state['values']['name']))
    )
  ) {
    form_set_error('name', t('%name has already been used.', array('%name' => $form_state['values']['name'])));
  }

  $expire = $form_state['values']['expires_val'];
  if (!is_numeric($expire) || (int)$expire < 0) {
    form_set_error('expires_val', t('Expiration period must be an integer greater than or equal to 0.'));
  }
}

/***
 * Generic settings validation.
 */
function user_relationships_ui_setting_validation($element, $validations) {
  foreach ($validations as $validation => $info) {
    $arguments = array($element['#value']);
    if (isset($info['valid_options'])) {
      $arguments[] = $info['valid_options'];
    }

    $valid = function_exists($validation) && call_user_func_array($validation, $arguments);
    $valid = !$valid ? ($validation == 'is_positive' && is_numeric($arguments) && (int)$arguments > 0) : FALSE;
    $valid = !$valid ? ($validation == 'is_non_negative' && is_numeric($arguments) && (int)$arguments > -1) : FALSE;

    if (!$valid) {
      form_set_error($element['#name'], $info['msg']);
    }
  }
}

/**
 * Process the relationship edit page form submission.
 */
function user_relationships_ui_type_edit_submit($form, &$form_state) {
  $relationship_type = (object)$form_state['values'];
  if ($form_state['values']['action'] == 'add') {
    drupal_write_record('user_relationship_types', $relationship_type);

    $form_state['values']['rtid'] = $relationship_type->rtid;
    $relationship_type = user_relationships_type_load($relationship_type->rtid);
    _user_relationships_invoke('insert', $relationship_type, TRUE);

    drupal_set_message(t('Relationship %name has been added.', array('%name' => $relationship_type->name)));
    watchdog(
      'u_relationship',
      'User Relationships form: relationship %name added.', array('%name' => $relationship_type->name),
      WATCHDOG_NOTICE,
      l(t('view'), 'admin/user/user_relationship/types/'. $relationship_type->rtid .'/edit')
    );
  }
  else {
    drupal_write_record('user_relationship_types', $relationship_type, 'rtid');

    _user_relationships_invoke('update', $relationship_type, TRUE);

    drupal_set_message(t('Relationship %name has been updated.', array('%name' => $relationship_type->name)));
    watchdog(
      'u_relationship',
      'User Relationships form: relationship %name updated.', array('%name' => $relationship_type->name),
      WATCHDOG_NOTICE,
      l(t('view'), 'admin/user/relationships/'. $relationship_type->rtid .'/edit')
    );
  }

  user_relationships_types_load(TRUE);
  menu_rebuild();

  $form_state['redirect'] = 'admin/user/relationships';
}


/**
 * Process relationship delete form submission.
 */
function user_relationships_ui_type_delete_submit($form, &$form_state) {
  $relationship_type = user_relationships_type_load($form_state['values']['rtid']);

  db_query("DELETE FROM {user_relationship_types} WHERE rtid = %d", $relationship_type->rtid);
  db_query("DELETE FROM {user_relationships} WHERE rtid = %d", $relationship_type->rtid);

  _user_relationships_invoke('delete', $relationship_type, TRUE);

  drupal_set_message(t('Relationship %name has been deleted.', array('%name' => $relationship_type->name)));
  watchdog(
    'u_relationship',
    'User Relationships form: relationship %name deleted.', array('%name' => $relationship_type->name),
    WATCHDOG_NOTICE
  );

  $form_state['redirect'] = 'admin/user/relationships';
}


/**
 * Validate relationship request.
 */
function user_relationships_ui_request_validate($form, &$form_state) {
  $requester = user_load($form_state['values']['requester']);
  $requestee = user_load($form_state['values']['requestee']);

  if (user_access('can have relationships', $requestee)) {
    $current_relationships = user_relationships_load(array('between' => array($requester->uid, $requestee->uid)));

    if (!db_result(db_query(
      "SELECT COUNT(*) FROM {user_relationship_types}" . ($current_relationships ? " WHERE rtid NOT IN ('%s') ORDER BY name" : ''),
      implode(',', array_keys($current_relationships))
    ))) {
      drupal_set_message(user_relationships_ui_get_message('too_many_relationships'), 'error');
      drupal_goto();
    }
    else if ($current_relationships[$form_values['rtid']]) {
      $message_name = $current_relationships[$form_values['rtid']]->approved ? 'existing_relationship' : 'existing_request';
      form_set_error('rtid', user_relationships_ui_get_message($message_name, array(
        '%requester'                => $requester->name,
        '%requestee'                => $requestee->name,
        '%relationship_name'        => $$current_relationships[$form_values['rtid']]->name,
        '%relationship_plural_name' => $$current_relationships[$form_values['rtid']]->plural_name
      )));
    }
  }
  else {
    drupal_set_message(user_relationships_ui_get_message('not_accepting_requests'));
    drupal_goto();
  }
}
