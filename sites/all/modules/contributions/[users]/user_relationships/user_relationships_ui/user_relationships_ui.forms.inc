<?php
// $Id: user_relationships_ui.forms.inc,v 1.1.2.8 2009/01/14 13:18:14 alexk Exp $

/**
 * @file
 * User Relationships forms
 */
module_load_include('inc', 'user_relationships_ui', 'user_relationships_ui.actions');

/**
 * Request new user to user relationship
 */
function user_relationships_ui_request_form($requester, $requestee, $form_values = NULL) {
  $current_relationships = user_relationships_load(array('between' => array($requester, $requestee)), array('sort' => 'rtid'));

  $results = db_query(
    "SELECT * FROM {user_relationship_types}" . ($current_relationships ? " WHERE rtid NOT IN ('%s') ORDER BY name" : ''),
    implode(',', array_keys($current_relationships))
  );
  $relationships = array();
  while ($result = db_fetch_object($results)) {
    $relationships[$result->rtid] = $result->name;
  }
  //try to find out desired relationship type
  if ($form_values['rtid']) {//given through form
    $default_relationship = $form_values['rtid'];
  }
  else if (is_numeric(arg(3)) && user_relationships_type_load(arg(3))) {//given through URL arguments, e.g. relationship/{uid}/request/{rtid}
    $default_relationship = arg(3);
  }
  else if (count($relationships) == 1) {//preselect the only one.
    $rtids = array_keys($relationships);
    $default_relationship = $rtids[0];
  }
  if (count($relationships)) {
    //hide control if the relationship is chosen
    if ($default_relationship && variable_get('user_relationships_show_direct_links', 1)) {
      $form['rtid'] = array(
        '#type' => 'hidden',
        '#default_value' => $default_relationship,
      );
    }
    else {
      $form['rtid'] = array(
        '#title'          => t('My relationship is'),
        '#type'           => 'radios',
        '#options'        => $relationships,
        '#default_value'  => $default_relationship,
      );
    }
  }
  $form['requester'] = array(
    '#type'   => 'value',
    '#value'  => $requester
  );
  $form['requestee'] = array(
    '#type'   => 'value',
    '#value'  => $requestee
  );

  return $form;
}

/**
 * Ajax handler function that is utilized to display the form
 * in a popup rather than having the full page reload for the users
 */
function user_relationships_ui_request_ajax($requestee) {
  $form = drupal_get_form('user_relationships_ui_request', $requestee);
  if (isset($_GET['ajax'])) {
    die($form);
  }
  return $form;
}

/**
 * Request a new relationship with another user
 */
function user_relationships_ui_request(&$form_state, $requestee) {
  global $user;
  $requester = $user;

  $script = '<script type="text/javascript">setTimeout(\'Drupal.user_relationships_ui.hidePopup()\', 10000);</script>';
  if (empty($requestee->name)) {
    $form['error'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#value' => user_relationships_ui_get_message('non_existant_user') . $script
    );
    return $form;
  }
  else if (!user_access('can have relationships', $requestee)) {
    $form['error'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#value' => user_relationships_ui_get_message('not_accepting_requests') . $script
    );
    return $form;
  }
  else if ($user->uid == $requestee->uid) {
    $form['error'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#value' => user_relationships_ui_get_message('self_request') . $script
    );
    return $form;
  }
  // This is the default which means we will display the form
  $form = user_relationships_ui_request_form($requester->uid, $requestee->uid, $form_state);
  if (!$form['rtid']) {
    $data['error'] = array(
      '#type' => 'markup',
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#value' => user_relationships_ui_get_message('too_many_relationships') . $script
    );
    return $data;
  }
  else {
    if ($form['rtid']['#default_value']) {
      $relationship = user_relationships_type_load($form['rtid']['#default_value']);
    }
    $form = confirm_form(
      $form,
      t('Request relationship'),
      "user/$requestee->uid",
      $relationship ?
      t('Are you sure you wish to send a new %rel_name request to %name?', array('%name' => $requestee->name, '%rel_name' => $relationship->name, '%rel_plural_name' => $relationship->plural_name)) :
      t('How do you relate to %name?', array('%name' => $requestee->name)),
      t('Send'), t('Cancel'),
      'user_relationships_request_confirm'
    );
    return $form;
  }
}

/**
 * Ajax handler function that is utilized to display the form
 * in a popup rather than having the full page reload for the users
 */
function user_relationships_ui_pending_requested_ajax($action, $account, $relationship) {
  $form = drupal_get_form('user_relationships_ui_pending_requested', $action, $account, $relationship);
  if (isset($_GET['ajax'])) {
    die($form);
  }
  return $form;
}

/**
 * Approve, Disapprove, or Cancel confirmation form
 */
function user_relationships_ui_pending_requested(&$form_state, $action, $account, $relationship) {
  $viewed_id = $account->uid;

  $form['rid'] = array(
    '#type'   => 'value',
    '#value'  => $relationship->rid
  );
  $form['action'] = array(
    '#type'   => 'value',
    '#value'  => $action
  );
  $form['viewed_id'] = array(
    '#type'   => 'value',
    '#value'  => $viewed_id
  );

  //pick the correct question message: approve and disapprove are requests to current user, cancel is a request from current user
  switch ($action) {
    case 'approve':
      $confirmation_message = 'Are you sure you want to approve the %relationship_name relationship request from !name?';
      $action_message = 'Approve relationship';
      break;
    case 'disapprove':
      $confirmation_message = 'Are you sure you want to disapprove the %relationship_name relationship request from !name?';
      $action_message = 'Disapprove relationship';
      break;
    default:
      $confirmation_message = 'Are you sure you want to cancel your %relationship_name relationship request to !name?';
      $action_message = 'Cancel relationship';
  }

  $output = confirm_form(
    $form,
    t($action_message),
    ($viewed_id == $user->id ? 'relationships/requests' : "user/{$viewed_id}/relationships/requests"),

    t($confirmation_message, array(
      '@action'             => $action,
      '%relationship_name'  => $relationship->name,
      '!name'               => theme('username', user_load(array('uid' => ($viewed_id == $relationship->requester_id ? $relationship->requestee_id : $relationship->requester_id))))
    )),
    t('Yes'), t('No'),
    'user_relationships_approve_confirm'
  );

  return $output;
}

/**
 * Ajax handler function that is utilized to display the form
 * in a popup rather than having the full page reload for the users
 */
function user_relationships_ui_remove_ajax($account, $relationship) {
  $form = drupal_get_form('user_relationships_ui_remove', $account, $relationship);
  if (isset($_GET['ajax'])) {
    die($form);
  }
  return $form;
}

/**
 * Confirm relationship removal.
 */
function user_relationships_ui_remove(&$form_state, $account, $relationship) {
  global $user;

  $viewed_id = $account->uid;

  $form['rid'] = array(
    '#type'   => 'value',
    '#value'  => $relationship->rid
  );
  $form['viewed_id'] = array(
    '#type'   => 'value',
    '#value'  => $viewed_id
  );
  $output = confirm_form(
    $form,
    t('Remove relationship'),
    ($viewed_id == $user->id ? 'relationships' : "user/{$viewed_id}/relationships/"),

    t("Are you sure you wish to delete the %relationship_name relationship with !name?", array(
      '%relationship_name'  => $relationship->name,
      '!name'               => theme('username', user_load(array('uid' => ($viewed_id == $relationship->requester_id ? $relationship->requestee_id : $relationship->requester_id))))
    )),
    t('Yes'), t('No'),
    'user_relationships_remove_confirm'
  );

  return $output;
}
