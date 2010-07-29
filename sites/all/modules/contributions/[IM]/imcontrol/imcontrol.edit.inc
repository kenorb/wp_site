<?php
// $Id$
/**
 * @file
 * Editing page callback file for the imcontrol module.
 */

/**
 * Form builder; Edit messenger settings.
 *
 * @ingroup forms
 * @see imcontrol_edit_form_validate()
 * @see imcontrol_edit_form_submit()
 * @param $account
 * uid of user to edit.
 * @return
 * Form.
 */
function imcontrol_edit_form($context, $account) {
  $userid = $account->uid;

  // Perform checks first before showing form
  if ($userid == 0) {
    $form['exist'] = array(
      '#value' => t('User does not exist.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
    return $form;
  }
  $result = db_query(
    'SELECT * FROM {imcontrol} WHERE uid = %d',
    $userid
  );
  $row = db_fetch_array($result);
  unset($result); // Destroy variable

  if (empty($row['pid']) && empty($row['extra'])) {
    $form['exist'] = array(
      '#value' => t('No record found.'),
      '#prefix' => '<p>',
      '#suffix' => '</p>',
    );
    if ($GLOBALS['user']->uid == $userid) {
      $form['prompt'] = array(
        '#value' => t('You may setup your messenger <a href="@setup">here</a>.', array('@setup' => url('user/'. $userid .'/im'))),
        '#prefix' => '<p>',
        '#suffix' => '</p>',
      );
    }
    return $form;
  }
  // End of check

  $messengers = explode(",", $row['extra']);
  $settings = explode(",", $row['attrib']);
  $default = imcontrol_default_colors();

  // This is a value only -- equivalent to a hidden field, except
  // that it is never rendered into the HTML.
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $userid,
  );

  // Create a fieldset for basic settings:
  $form['account'] = array(
    '#type' => 'fieldset',
    '#title' => t('IM Accounts'),
    '#tree' => TRUE, // Required to get all values from fieldset
  );
  $form['account']['pid'] = array(
    '#type' => 'textfield',
    '#title' => t('Windows Live Messenger Unique ID'),
    '#description' => t('Do not change unless you intend to use a different account.'),
    '#default_value' => substr($row['pid'], 0, -24),
    '#size' => 16,
    '#maxlength' => 20,
    '#field_suffix' => l('@apps.messenger.live.com', 'http://settings.messenger.live.com/applications/websignup.aspx?returnurl='. url('imcontrol', array('absolute' => TRUE)) .'&privacyurl='. url('privacy', array('absolute' => TRUE))),
  );
  // Only set cookie if current user is the account owner and has permission to embed
  if (user_access('embed messenger') && $GLOBALS['user']->uid == $userid) {
    // Store session in a cookie to solve Firefox problem, expires in 7 minutes
    setcookie("imcontrol_live", session_id(), time()+420, "/", '.'. substr($GLOBALS['base_root'], 7), FALSE, TRUE);
  }

  $form['account']['others']['yahoo'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo! ID'),
    '#description' => t('Do not change unless you intend to use a different account.'),
    '#default_value' => $messengers[0],
    '#size' => 16,
    '#maxlength' => 20,
    '#field_suffix' => '@yahoo.com.*',
  );
  $form['account']['others']['qq'] = array(
    '#type' => 'textfield',
    '#title' => t('QQ/TM number'),
    '#description' => t('Do not change unless you intend to use a different account.'),
    '#default_value' => $messengers[1],
    '#size' => 16,
    '#maxlength' => 20,
  );
  $form['account']['others']['skype'] = array(
    '#type' => 'textfield',
    '#title' => t('Skype name'),
    '#description' => t('Do not change unless you intend to use a different account.'),
    '#default_value' => $messengers[2],
    '#size' => 16,
    '#maxlength' => 20,
  );
  /** Add messenger here **/

  // Create a fieldset for basic settings:
  $form['set'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['set']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Display'),
    '#default_value' => $row['status'],
    '#options' => array(t('Button format'), t('Default format'), t('Delete all')),
  );
  $form['set']['predefined'] = array(
    '#type' => 'select',
    '#title' => t('Windows Live Messenger theme'),
    '#default_value' => !empty($settings[12]) ? $settings[12] : 'default',
    '#options' => array(
      'default' => t('Default'),
      'blue' => t('Blue'),
      'green' => t('Green'),
      'orange' => t('Orange'),
      'pink' => t('Pink'),
      'purple' => t('Purple'),
      'black' => t('Black'),
      'custom' => t('Custom'),
    ),
  );

  // Create a fieldset for custom settings:
  $form['edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Custom options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE, // Required to get all values from fieldset
  );
  // This is a value only -- equivalent to a hidden field, except
  // that it is never rendered into the HTML.
  $form['edit']['theme'] = array(
    '#type' => 'value',
    '#value' => !empty($settings[0]) ? $settings[0] : 'true',
  );
  $form['edit']['forecolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color'),
    '#default_value' => !empty($settings[1]) ? $settings[1] : $default[0],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['backcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => !empty($settings[2]) ? $settings[2] : $default[1],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['linkcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Hyperlink'),
    '#default_value' => !empty($settings[3]) ? $settings[3] : $default[2],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['bordercolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Border color'),
    '#default_value' => !empty($settings[4]) ? $settings[4] : $default[3],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['buttonforecolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color for active buttons'),
    '#default_value' => !empty($settings[5]) ? $settings[5] : $default[4],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['buttonbackcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color for active buttons'),
    '#default_value' => !empty($settings[6]) ? $settings[6] : $default[5],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['buttonbordercolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Border color for buttons'),
    '#default_value' => !empty($settings[7]) ? $settings[7] : $default[6],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['buttondisabledcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color for a disabled button'),
    '#default_value' => !empty($settings[8]) ? $settings[8] : $default[7],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['headerforecolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Text color for the header area'),
    '#default_value' => !empty($settings[9]) ? $settings[9] : $default[8],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['headerbackcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color for the header area'),
    '#default_value' => !empty($settings[10]) ? $settings[10] : $default[9],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );
  $form['edit']['chatlabelcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Display name color'),
    '#default_value' => !empty($settings[11]) ? $settings[11] : $default[16],
    '#size' => 8,
    '#maxlength' => 6,
    '#field_prefix' => '#',
  );

  // Finally, a submit button:
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  unset($row, $messengers, $settings, $default); // Destroy variable
  return $form;
}

function imcontrol_edit_form_validate($form, &$form_state) {
  if ($form_state['values']['status'] == 2) return; // Skip all checks if user opts to delete record

  if ($form_state['values']['predefined'] == 'custom') {
    // Validate all the color values
    foreach ($form_state['values']['edit'] as $key => $value) {
      if ($key != 'theme') {
        if (preg_match('/[^A-Fa-f0-9]/', $value)) {
          form_set_error('edit]['. $key, t('Color value is invalid!'));
        }
        else if (drupal_strlen($value) != 6 && drupal_strlen($value) != 3) {
          form_set_error('edit]['. $key, t('Color value length incorrect!'));
        }
      }
    }
    unset($key, $value); // Destroy variables
  }
  else {
    // Boolean of whether themes are used
    $status = 'true';
    if ($form_state['values']['predefined'] == 'default') {
      $status = 'false';
    }
    // Set the selected color theme
    _imcontrol_theme($form, $form_state, imcontrol_default_colors($form_state['values']['predefined']), $status);
    unset($status); // Destroy variable
  }

  // Check if there are invalid characters in the ID or if the length is too short (may need to change according to Microsoft)
  if (!empty($form_state['values']['account']['pid']) && (preg_match('/[^A-Za-z0-9]/', $form_state['values']['account']['pid']) || drupal_strlen($form_state['values']['account']['pid']) < 10)) {
    form_set_error('pid', t('Messenger ID is invalid!'));
  }
  // Check Yahoo ID
  if (!empty($form_state['values']['account']['others']['yahoo']) && preg_match('/[^A-Za-z0-9]/', $form_state['values']['account']['others']['yahoo'])) {
    form_set_error('account][others][yahoo', t('Yahoo! ID is invalid!'));
  }
  // Check QQ/TM number
  if (!empty($form_state['values']['account']['others']['qq']) && !is_numeric($form_state['values']['account']['others']['qq'])) {
    form_set_error('account][others][qq', t('QQ/TM number is invalid!'));
  }
  // Check Skype name
  if (!empty($form_state['values']['account']['others']['skype']) && preg_match('/[^A-Za-z0-9_]/', $form_state['values']['account']['others']['skype'])) {
    form_set_error('account][others][skype', t('Skype name is invalid!'));
  }
  /** Add messenger here **/
}

/**
 * Form submission handler, which functions like a hook.
 * Note that the params $form and &$form_state are new in D6. They
 * replace $form_id and $form_values.
 */
function imcontrol_edit_form_submit($form, &$form_state) {
  if ($form_state['values']['status'] == 2) {
    db_query('DELETE FROM {imcontrol} WHERE uid = %d', $form_state['values']['uid']);
    drupal_set_message(t('IM Control record deleted'), 'status');
  }
  else {
    // Insert empty strings into pid and extra fields as required
    if (!empty($form_state['values']['account']['pid'])) {
      $form_state['values']['account']['pid'] = $form_state['values']['account']['pid'] .'@apps.messenger.live.com';
    }
    $messengers = '';
    if (!_imcontrol_none($form_state['values']['account']['others'])) {
      $messengers = implode(",", $form_state['values']['account']['others']);
    }
    db_query("UPDATE {imcontrol} "
        ."SET pid = '%s', attrib = '%s', extra = '%s', status = %d "
        ."WHERE uid = %d",
      $form_state['values']['account']['pid'],
      implode(",", $form_state['values']['edit']) .",". $form_state['values']['predefined'],
      $messengers,
      $form_state['values']['status'],
      $form_state['values']['uid']
    );

    // Redirect to the view page
    $form_state['redirect'] = 'user/'. $form_state['values']['uid'] .'/im';
    drupal_set_message(t('IM Control settings saved'), 'status');
  }
}

/**
 * Form values handler, changes the submitted values according to theme selected
 */
function _imcontrol_theme($form, &$form_state, $colors, $default = 'true') {
  // Alter form values to selected color theme
  $index = 0;
  foreach ($form_state['values']['edit'] as $key => $value) {
    if ($key == 'theme') {
      form_set_value($form['edit'][$key], $default, $form_state);
    }
    else {
      form_set_value($form['edit'][$key], $colors[$index], $form_state);
      if ($index < 9) {
        ++$index;
      }
      else {
        $index = 16;
      }
    }
  }
  unset($index, $key, $value); // Destroy variables
}
