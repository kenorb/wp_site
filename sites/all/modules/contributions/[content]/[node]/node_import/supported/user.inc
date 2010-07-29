<?php
// $Id: user.inc,v 1.1.2.6 2008/12/11 22:15:07 robrechtj Exp $

/**
 * @file
 * Support file for the core user module.
 */

/**
 * Implementation of hook_node_import_types().
 */
function user_node_import_types() {
  return array(
    'user' => array(
      'title' => t('Users'),
      'can_create' => 'node_import_can_create_user',
      'create' => 'node_import_create_user',
    )
  );
}

/**
 * Check if the user can create users.
 *
 * @param $type
 *   String. Node_import $type.
 *
 * @return
 *   TRUE if the user can create users. FALSE otherwise.
 */
function node_import_can_create_user($type) {
  if ($type === 'user') {
    return user_access('administer users');
  }
  return FALSE;
}

/**
 * Create a new user.
 */
function node_import_create_user($type, $values, $preview) {
  $can_continue = TRUE;
  $output = $preview ? '' : FALSE;

  if ($preview) {
    $form = array(); //Not needed - TODO: we should use $form['#validate']
    $form_state = array(
      'values' => $values,
    );
    user_register_validate($form, $form_state);

    $output = '<div class="preview">';
    $output .= '<h3>'. check_plain($values['name']) .'</h3>';
    $output .= '<p>'. check_plain($values['mail']) .'</p>';
    $output .= '</div>';
  }
  else {
    $form_id = 'user_register';

    $values['op'] = t('Save');
    $form_state = array(
      'values' => $values,
    );

    node_import_drupal_execute($form_id, $form_state);
    $output = $form_state['user']->uid;
  }

  return array($can_continue, $output);
}

/**
 * Implementation of hook_node_import_fields().
 */
function user_node_import_fields($type) {
  $fields = array();

  if ($type === 'user') {
    $fields['name'] = array(
      'title' => t('Username'),
      'map_required' => TRUE,
    );
    $fields['mail'] = array(
      'title' => t('E-mail address'),
      'input_format' => 'mail',
      'map_required' => TRUE,
    );
    $fields['pass'] = array(
      'title' => t('Password'),
    );
    $fields['status'] = array(
      'title' => t('Status'),
      'allowed_values' => array('1' => t('Active'), '0' => t('Blocked')),
    );
    $fields['notify'] = array(
      'title' => t('Notify user of new account'),
      'is_mappable' => FALSE,
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_options().
 */
function user_node_import_options($type, $options, $fields, $map) {
  $form = array();

  if ($type === 'user') {
    $options = array(
      'never' => t('Threat records without password as error'),
      'always' => t('Generate password for each record (even if set in the file)'),
      'empty' => t('Generate password for records with empty password'),
    );
    $default = isset($map['pass']) && strlen($map['pass']) > 0 ? 'empty' : 'never';

    $form['generate_pass'] = array(
      '#type' => 'radios',
      '#title' => t('Password'),
      '#options' => $options,
      '#default_value' => isset($options['generate_pass']) ? $options['generate_pass'] : $default,
    );
  }

  return $form;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function user_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if ($type == 'user') {
    $form['status'] = array(
      '#type' => 'radios',
      '#title' => t('Status'),
      '#options' => array('0' => t('Blocked'), '1' => t('Active')),
      '#default_value' => isset($defaults['status']) ? $defaults['status'] : '1',
    );

    $form['notify'] = array(
      '#type' => 'checkbox',
      '#title' => t('Notify user of new account'),
      '#default_value' => isset($defaults['notify']) ? $defaults['notify'] : '0',
    );
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function user_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if ($type === 'user') {
    if ($options['generate_pass'] === 'always' || ($options['generate_pass'] === 'empty' && strlen($values['pass']) == 0)) {
      $values['pass'] = user_password();
    }
  }
}

