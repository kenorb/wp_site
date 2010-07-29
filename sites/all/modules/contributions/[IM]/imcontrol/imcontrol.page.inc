<?php
// $Id$
/**
 * @file
 * Show messenger page callback file for the imcontrol module.
 */

/**
 * Menu callback; generate page with embedded messenger.
 * http://msdn.microsoft.com/en-us/library/bb936685.aspx
 */
function imcontrol_page($account) {
  $result = db_query(
    'SELECT * FROM {imcontrol} WHERE uid = %d',
    $account->uid
  );
  $item = db_fetch_array($result);

  // Only show if user has completed the initial setup
  if (!empty($item['pid']) || !empty($item['extra'])) {

    // Add css file for formatting the iframe
    drupal_add_css(drupal_get_path('module', 'imcontrol') .'/imcontrol.css');
    $out = '<div id="imcontrol">';

    // Show MSN presence
    if (!empty($item['pid'])) {
      $out .= theme('imcontrol_display', 'msn', $item['pid'], $item['attrib'], $item['status']);
    }
    // Show additional messengers
    if (!empty($item['extra'])) {
      $messengers = explode(",", $item['extra']);
      $show = FALSE;

      // Display style for embedded messenger option
      if ($item['status'] && !empty($item['pid'])) {
        $out .= '<div id="imcontrol_extra">'. t('I am also available on:');
        $show = TRUE;
      }
      else {
        $out .= '<div id="imcontrol_extras">';
      }
      /** Add messenger here **/
      $out .= theme('imcontrol_display', 'skype', $messengers[2])
            . theme('imcontrol_display', 'yahoo', $messengers[0])
            . theme('imcontrol_display', 'qq', $messengers[1], NULL, $show)
            .'</div>';
      unset($messengers, $show); // Destroy variable
    }
    $out .= '<div id="imcontrol_clear"></div></div>'; // Style for clearing the floats
  }
  else {
    // Show setup prompt for user who has no record in the database
    if ((user_access('embed messenger') && $GLOBALS['user']->uid == $account->uid) && (empty($item['pid']) && empty($item['extra']))) {
      $return_url = url('imcontrol', array('absolute' => TRUE));
      $privacy_url = url('privacy', array('absolute' => TRUE));

      $out = '<p>'. t('Let others send you instant messages from this page! Setup your account at <a href="@setup">Windows Live</a> or enter your unique ID below:', array('@setup' => url('http://settings.messenger.live.com/applications/websignup.aspx') .'?returnurl='. $return_url .'&privacyurl='. $privacy_url)) .'</p>';
      $out .= drupal_get_form('imcontrol_add_form'); // Show setup form

      // Store session in a cookie to solve Firefox problem, expires in 10 minutes
      setcookie("imcontrol_live", session_id(), time()+600, "/", '.'. substr($GLOBALS['base_root'], 7), FALSE, TRUE);

      unset($return_url, $privacy_url); // Destroy variables
    }
    else {
      $out = '<p>'. t('User has no instant messenger account.') .'</p>';
    }
  }

  unset($result, $item); // Destroy variables
  return $out;
}

/**
 * Form builder; Submit presence ID.
 *
 * @ingroup forms
 * @see imcontrol_add_form_validate()
 * @see imcontrol_add_form_submit()
 * @return
 * Form.
 */
function imcontrol_add_form($context) {
  $form['pid'] = array(
    '#type' => 'textfield',
    '#size' => 16,
    '#maxlength' => 20,
    '#field_suffix' => '@apps.messenger.live.com',
  );

  // Create a fieldset for the additional messengers:
  $form['messenger'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional messengers'),
    '#tree' => TRUE, // Required to get all values from fieldset
  );
  $form['messenger']['yahoo'] = array(
    '#type' => 'textfield',
    '#title' => 'Yahoo! ID',
    '#size' => 16,
    '#maxlength' => 20,
    '#field_suffix' => '@yahoo.com.*',
  );
  $form['messenger']['qq'] = array(
    '#type' => 'textfield',
    '#title' => 'QQ/TM number',
    '#size' => 16,
    '#maxlength' => 20,
  );
  $form['messenger']['skype'] = array(
    '#type' => 'textfield',
    '#title' => 'Skype name',
    '#size' => 16,
    '#maxlength' => 20,
  );
  /** Add messenger here **/

  // Finally, a submit button:
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function imcontrol_add_form_validate($form, &$form_state) {
  // Check if empty form submitted
  if (empty($form_state['values']['pid']) && _imcontrol_none($form_state['values']['messenger'])) {
    form_set_error('messenger', 'Please enter at least one messenger.');
    // Return form and do not continue with validation
    return;
  }

  // Check if there are invalid characters in the messenger ID or if the length is too short (may need to change according to Microsoft)
  if (!empty($form_state['values']['pid']) && (preg_match('/[^A-Za-z0-9]/', $form_state['values']['pid']) || drupal_strlen($form_state['values']['pid']) < 10)) {
    form_set_error('pid', t('Messenger ID is invalid!'));
  }
  // Check Yahoo ID has no invalid characters
  if (!empty($form_state['values']['messenger']['yahoo']) && preg_match('/[^A-Za-z0-9]/', $form_state['values']['messenger']['yahoo'])) {
    form_set_error('messenger][yahoo', t('Yahoo! ID is invalid!'));
  }
  // Check QQ/TM number
  if (!empty($form_state['values']['messenger']['qq']) && !is_numeric($form_state['values']['messenger']['qq'])) {
    form_set_error('messenger][qq', t('QQ/TM number is invalid!'));
  }
  // Check Skype name has no invalid characters
  if (!empty($form_state['values']['messenger']['skype']) && preg_match('/[^A-Za-z0-9_]/', $form_state['values']['messenger']['skype'])) {
    form_set_error('messenger][skype', t('Skype name is invalid!'));
  }
  /** Add messenger here **/
}

/**
 * Submit function for intial setup.
 */
function imcontrol_add_form_submit($form, &$form_state) {
  // Insert empty strings into pid and extra fields as required
  if (!empty($form_state['values']['pid'])) {
    $form_state['values']['pid'] = $form_state['values']['pid'] .'@apps.messenger.live.com';
  }
  $messengers = '';
  if (!_imcontrol_none($form_state['values']['messenger'])) {
    $messengers = implode(",", $form_state['values']['messenger']);
  }
  db_query(
    'INSERT INTO {imcontrol} (uid, pid, extra) '
      ."VALUES (%d, '%s', '%s') "
      ."ON DUPLICATE KEY UPDATE pid = '%s', extra = '%s'",
    $GLOBALS['user']->uid,
    $form_state['values']['pid'],
    $messengers,
    $form_state['values']['pid'],
    $messengers
  );

  drupal_set_message(t('Setup complete.'), 'status');
  unset($messengers); // Destroy variable
}
