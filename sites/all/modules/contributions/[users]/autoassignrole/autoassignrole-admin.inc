<?php
// $Id: autoassignrole-admin.inc,v 1.1.2.1 2008/09/03 23:12:30 cyberswat Exp $

/**
 * @file
 *
 * The autoassignrole-admin.inc file
 *
 * Contains the admin form and associated functionality for the autoassignrole
 * module
 */

function autoassignrole_admin_form() {
  $form['autoassignrole_settings_auto'] = array(
    '#type' => 'fieldset',
    '#title' => t('Automatic Role Assignment'),
    '#description' => t('Automatically assigned roles will be attached to
      accounts created through the administrative interface as well as normal
      new user registration.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['autoassignrole_settings_auto']['auto_active'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic role assignment'),
    '#default_value' => _autoassignrole_settings('auto_active'),
    '#description' => t('Automatic role assignment occurs when the user first
      logins to the account.  This happens without the users knowledge.  Set to
      Enabled to allow this functionality or Disabled to not allow.'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
  );
  $roles = user_roles(TRUE);
  // The disabled checkbox subelement for the 'authenticated user' role
  // must be generated separately and added to the checkboxes element,
  // because of a limitation in D6 FormAPI not supporting a single disabled
  // checkbox within a set of checkboxes.
  // TODO: This should be solved more elegantly. See issue #119038.
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  if ($roles) {
    $form['autoassignrole_settings_auto']['auto_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#default_value' => _autoassignrole_settings('auto_roles'),
      '#description' => t('Check the specific Roles the user will automatically
        be assigned to when created by an administrator or through the new user
        registration process.  The Authenticated User role is automatically
        assigned by Drupal core and can not be edited.'),
      '#options' => $roles,
      DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
    );
  }
  $form['autoassignrole_settings_user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User Role Assignment'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['autoassignrole_settings_user']['user_active'] = array(
    '#type' => 'radios',
    '#title' => t('User Role Assignment'),
    '#default_value' => _autoassignrole_settings('user_active'),
    '#description' => t('The end user will be allowed to select the following
      roles when they log in.'),
    '#options' => array(1 => t('Enabled'), 0 => t('Disabled')),
  );
  $roles = user_roles(TRUE);
  // The disabled checkbox subelement for the 'authenticated user' role
  // must be generated separately and added to the checkboxes element,
  // because of a limitation in D6 FormAPI not supporting a single disabled
  // checkbox within a set of checkboxes.
  // TODO: This should be solved more elegantly. See issue #119038.
  $checkbox_authenticated = array(
    '#type' => 'checkbox',
    '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
    '#default_value' => TRUE,
    '#disabled' => TRUE,
  );
  unset($roles[DRUPAL_AUTHENTICATED_RID]);
  if ($roles) {
    $form['autoassignrole_settings_user']['user_roles'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Roles'),
      '#default_value' => _autoassignrole_settings('user_roles'),
      '#options' => $roles,
      DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
    );
  }
  $form['autoassignrole_settings_user']['user_multiple'] = array(
    '#type' => 'radios',
    '#title' => t('User Role Selection'),
    '#default_value' => _autoassignrole_settings('user_multiple'),
    '#description' => t('Should the end user be allowed to choose a single role
      or can they choose multiple roles?'),
    '#options' => array(0 => t('Single Role'), 1 => t('Multiple Roles')),
  );
  $form['autoassignrole_settings_user']['user_selection'] = array(
    '#type' => 'radios',
    '#title' => t('Selection Method'),
    '#default_value' => _autoassignrole_settings('user_selection'),
    '#description' => t('The type of form elements the end user will be presented with.'),
    '#options' => array(0 => t('Radio Buttons'), 1 => t('Selection Box'), 2 => t('Check Boxes')),
  );
  $form['autoassignrole_settings_user']['user_required'] = array(
    '#type' => 'radios',
    '#title' => t('Required'),
    '#default_value' => _autoassignrole_settings('user_required'),
    '#description' => t('Should the end user be required to choose a role?'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
  );
  $form['autoassignrole_settings_user']['user_sort'] = array(
    '#type' => 'radios',
    '#title' => t('Sorting'),
    '#default_value' => _autoassignrole_settings('user_sort'),
    '#description' => t('Default sort order of roles the user will see.'),
    '#options' => array(
      'SORT_ASC' => t('Ascending'),
      'SORT_DESC' => t('Descending')
    ),
  );
  $form['autoassignrole_settings_user']['user_fieldset_title'] = array(
    '#type' => 'textfield',
    '#title' => t('User Role Fieldset Title'),
    '#description' => t('The title of the fieldset that contains role
      options.'),
    '#default_value' => _autoassignrole_settings('user_fieldset_title'),
    '#size' => 60,
    '#maxlength' => 128
  );
  $form['autoassignrole_settings_user']['user_title'] = array(
    '#type' => 'textfield',
    '#title' => t('User Role Title'),
    '#description' => t('The title of the field that contains the role options
      the end user sees during registration.'),
    '#default_value' => _autoassignrole_settings('user_title'),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => FALSE,
  );
  $form['autoassignrole_settings_user']['user_description'] = array(
    '#type' => 'textarea',
    '#title' => t('User Role Description'),
    '#description' => t('The description displayed to the end user when they are
      selecting thier role during registration.'),
    '#default_value' => _autoassignrole_settings('user_description'),
    '#required' => FALSE,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}
function autoassignrole_admin_form_validate($form_id, $form_values) {
  if($form_values['values']['user_multiple'] == 0 &&
      $form_values['values']['user_selection'] == 2) {
    form_set_error('user_selection', t('If a user can only select a single role you need to set the selection method as Radio Buttons or Selection Box.'));
  }
  if($form_values['values']['user_multiple'] == 1 &&
      $form_values['values']['user_selection'] == 0) {
    form_set_error('user_selection', t('If a user can select multiple roles they can not use Radio Buttons as the selection method.'));
  }
}
function autoassignrole_admin_form_submit($form_id, $form_values) {
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['auto_active'], 'auto_active');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    serialize($form_values['values']['auto_roles']), 'auto_roles');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_active'], 'user_active');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    serialize($form_values['values']['user_roles']), 'user_roles');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_multiple'], 'user_multiple');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_title'], 'user_title');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_fieldset_title'], 'user_fieldset_title');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_required'], 'user_required');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_sort'], 'user_sort');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_description'], 'user_description');
  db_query("UPDATE {autoassignrole} set value = '%s' where arid = '%s'",
    $form_values['values']['user_selection'], 'user_selection');
}