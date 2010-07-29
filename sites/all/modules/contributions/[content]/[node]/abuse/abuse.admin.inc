<?php
// $Id: abuse.admin.inc,v 1.1.2.4 2008/10/15 17:50:58 btmash Exp $

/**
 * @file
 * A list of all administration functions
 */

function abuse_admin_settings(&$form_state) {
  $form = array();
  $form['abuse_reasons_configuration'] = array(
    '#title' => t('Reasons'),
    '#type' => 'item',
    '#value' => t('You can configure the list of reasons at !link', array('!link' => l('Reason configuration settings', 'admin/settings/abuse/reasons')))
  );
  // Configure which content types can be flagged
  $form['contenttypes'] = array(
    '#title' => t('Enable flagging for these content types'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  foreach (node_get_types() as $type => $name) {
    $form['contenttypes'][ABUSE_CONTENT_NODE_TYPE . $type] = array(
      '#title' => $name->type,
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => variable_get(ABUSE_CONTENT_NODE_TYPE . $type, 0),
    );
  }
  $form['contenttypes'][ABUSE_CONTENT_COMMENTS] = array(
    '#title' => t('comments'),
    '#type' => 'checkbox',
    '#return_value' => 1,
    '#default_value' => variable_get(ABUSE_CONTENT_COMMENTS, 0),
  );
  $form['contenttypes'][ABUSE_CONTENT_USERS] = array(
    '#title' => t('users'),
    '#type' => 'checkbox',
    '#description' => t('Users is still a work in progress - do not bother till fully coded out'),
    '#return_value' => 1,
    '#default_value' => variable_get(ABUSE_CONTENT_USERS, 0),
  );

  // Ticketing system settings
  $form['assigned'] = array(
    '#title' => t('Ticketing settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['assigned']['abuse_assigned_moderators'] = array(
    '#title' => t('Abuse Assigned Moderators'),
    '#description' => t('Select this option if you have a pool of moderators and you wish to assign each one a certain number of tickets to work with.'),
    '#type' => 'checkbox',
    '#return_value' => TRUE,
    '#default_value' => variable_get('abuse_assigned_moderators', FALSE)
  );
  $form['assigned']['abuse_num_assigned'] = array(
    '#title' => t('Moderator queue limit'),
    '#type' => 'textfield',
    '#description' => t('This field is to set a maximum limit on the number of flagged items that will be added to the queue of a moderator'),
    '#default_value' => variable_get('abuse_num_assigned', 20),
    '#size' => 6,
    '#maxlength' => 6,
  );
  $form['assigned']['abuse_cleanup_hour'] = array(
    '#title' => t('Reset assigned ticket items (Please type hour of day)'),
    '#type' => 'textfield',
    '#default_value' => variable_get('abuse_cleanup_hour', 0),
    '#size' => 2,
    '#maxlength' => 2,
  );

  // General settings
  $form['general_settings'] = array(
    '#title' => t('Settings for all abuse content'),
    '#type' => 'fieldset',
    '#description' => t('These settings apply to all content that is allowed to be flagged into the abuse administration system'),
    '#collapsible' => FALSE,
  );

  $form['general_settings']['abuse_threshold'] = array(
    '#title' => t('Abuse threshold'),
    '#type' => 'textfield',
    '#default_value' => variable_get('abuse_threshold', 3),
    '#size' => 6,
    '#maxlength' => 6,
    '#required' => TRUE,
  );

  $form['general_settings']['abuse_warn_subject'] = array(
    '#title' => t('Warning subject'),
    '#type' => 'textfield',
    '#default_value' => variable_get('abuse_warn_subject', t('Abuse warning')),
    '#size' => 72,
    '#required' => TRUE,
  );

  $form['general_settings']['abuse_warn_body'] = array(
    '#title' => t('Warning body'),
    '#type' => 'textarea',
    '#default_value' => variable_get('abuse_warn_body', ''),
    '#cols' => 72,
    '#rows' => 10,
    '#required' => TRUE,
  );

  $form['general_settings']['abuse_warn_bcc'] = array(
    '#title' => t('Warning BCC'),
    '#type' => 'textfield',
    '#default_value' => variable_get('abuse_warn_bcc', ''),
    '#size' => 72,
  );

  $form['general_settings']['abuse_form_pre'] = array(
    '#title' => t('Abuse form intro text'),
    '#type' => 'textarea',
    '#default_value' => variable_get('abuse_form_pre', ''),
    '#cols' => 72,
    '#rows' => 10,
  );

  return system_settings_form($form);
}

function abuse_admin_reason_settings(&$form_state) {
  $form = array();
  $form['add_reason'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add new reason'),
    '#weight' => -1,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['add_reason']['short_form'] = array(
    '#type' => 'textfield',
    '#title' => t('Reason'),
    '#description' => t('Provide a short form of what the reason is'),
    '#size' => 35,
    '#maxlength' => 35,
  );
  $form['add_reason']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('A more details description of what the reason is'),
    '#rows' => 5,
    '#cols' => 50,
  );
  $form['add_reason']['email_notice'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Notice addition'),
    '#description' => t('Text that will automatically be included in the warning email.'),
    '#rows' => 5,
    '#cols' => 50,
  );
  $form['add_reason']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $num_reasons = db_result(db_query('SELECT COUNT(arid) FROM {abuse_reasons}'));
  if ($num_reasons > 0) {
    $form['reason_list'] = array(
      '#type' => 'fieldset',
      '#title' => t('Current list of reasons - check items that you wish to remove'),
      '#weight' => 5,
      '#collapsible' => FALSE,
    );
    $reasons = _abuse_reasons();
    $count = 0;
    foreach ($reasons as $reason) {
      $count++;
      $form['reason_list']['field'. $count] = array(
        '#type' => 'fieldset',
        '#title' => t($reason->reason),
      );
      $form['reason_list']['field'. $count]['arid'. $reason->arid] = array(
        '#type' => 'checkbox',
        '#title' => t('Remove from of list of reasons')
      );
      $form['reason_list']["field$count"]['edit'] = array(
        '#type' => 'item',
        '#value' => l('Edit reason', "admin/settings/abuse/reasons/edit/$reason->arid"),
      );
      $form['reason_list']['field'. $count]['reason'] = array(
        '#type' => 'item',
        '#value' => t('Description') .': '. $reason->description,
      );
      $form['reason_list']['field'. $count]['argumentation'] = array(
        '#type' => 'item',
        '#value' => t('Email content') .': '. check_markup($reason->argumentation),
      );
    }
    $form['reason_list']['remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
    );
  }
  return $form;
}

function abuse_admin_edit_reason($form_state, $arid) {
  $reason = db_fetch_object(db_query("SELECT * FROM {abuse_reasons} WHERE arid=%d", $arid));
  if (!$reason->arid) {
    drupal_not_found();
  }
  $form = array();
  $form['arid'] = array(
    '#type' => 'value',
    '#value' => $reason->arid,
  );
  $form['short_form'] = array(
    '#type' => 'textfield',
    '#title' => t('Reason'),
    '#description' => t('Provide a short form of what the reason is'),
    '#default_value' => $reason->reason,
    '#size' => 35,
    '#maxlength' => 35,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#description' => t('A more details description of what the reason is'),
    '#default_value' => $reason->description,
    '#rows' => 5,
    '#cols' => 50,
  );
  $form['email_notice'] = array(
    '#type' => 'textarea',
    '#title' => t('Email Notice addition'),
    '#description' => t('Text that should automatically be included in the warning email.'),
    '#default_value' => $reason->argumentation,
    '#rows' => 5,
    '#cols' => 50,
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save')
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel')
  );

  return $form;
}

function abuse_admin_reason_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  if ($values['op'] == t('Save')) {
    if (empty ($values['short_form'])) {
      form_set_error('short_form', t('You MUST provide a reason.'));
    }
    elseif (empty($values['description'])) {
      form_set_error('description', t('You MUST provide a description of the reason.'));
    }
    elseif (empty($values['email_notice'])) {
      form_set_error('email_notice', t('You MUST provide an email notice that may be sent to the user for this reason.'));
    }
  }
}

function abuse_admin_reason_settings_submit($form, &$form_state) {
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  if ($values['op'] == t('Save')) {
    db_query("INSERT INTO {abuse_reasons} (reason, description, argumentation) VALUES ('%s', '%s', '%s')",
      $values['short_form'], $values['description'], $values['email_notice']);
    drupal_set_message(t("Added new reason to list"));
  }
  elseif ($op == t('Remove')) {
    foreach ($values as $key => $value) {
      if (strpos($key, 'arid') === 0 && $value === 1) {
        db_query("DELETE FROM {abuse_reasons} WHERE arid=%d", str_replace('arid', '', $key));
        drupal_set_message("Successfully removed reason from list");
      }
    }
  }
}

function abuse_admin_edit_reason_validate($form, &$form_state) {
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  if ($values['op'] == t('Save')) {
    if (empty ($values['short_form'])) {
      form_set_error('short_form', t('You MUST provide a reason.'));
    }
    elseif (empty($values['description'])) {
      form_set_error('description', t('You MUST provide a description of the reason.'));
    }
    elseif (empty($values['email_notice'])) {
      form_set_error('email_notice', t('You MUST provide an email notice that may be sent to the user for this reason.'));
    }
  }
}

function abuse_admin_edit_reason_submit($form, &$form_state) {
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  if ($op == t('Save')) {
    db_query("UPDATE {abuse_reasons} SET reason='%s', description='%s', argumentation='%s' WHERE arid=%d",
      $values['short_form'], $values['description'], $values['email_notice'], $values['arid']);
    drupal_set_message(t('Reason edit '. $values['arid'] .' saved'));
  }
  $form_state['redirect'] = 'admin/settings/abuse/reasons';
}

/**
 * Implement a generic moderation point
 */
function abuse_admin_moderate($status = array(), $assigned_to_uid = FALSE) {
  global $user;
  $limit = (empty($_GET['limit'])) ? 25 : $_GET['limit'];

  $content = '';
  $query = "SELECT type, oid, status, assigned_to_uid FROM {abuse_status} WHERE (status=". implode(' OR status=', $status) .")";
  if ($assigned_to_uid) {
    $query .= " AND assigned_to_uid=$user->uid";
  }
  $query .= " ORDER BY oid ASC";
  $result = pager_query($query, $limit, 0, NULL);
  $reports = array();
  while ($object = db_fetch_object($result)) {
    $obj = _abuse_load($object);
    $reports[] = $obj;
  }
  $content = theme('abuse_page', $reports, $limit);
  return $content;
}

function abuse_admin_default_callback() {
  if (variable_get('abuse_assigned_moderators', FALSE)) {
    return abuse_admin_moderate(array(ABUSE_PENDING, ABUSE_HIDDEN), TRUE);
  }
  else {
    return abuse_admin_moderate(array(ABUSE_PENDING));
  }
}

function abuse_admin_status($type, $oid) {
  if ('user' === drupal_strtolower($type)) {
    return abuse_admin_user($oid);
  }
  $object = _abuse_load($type, $oid);
  if ($object->oid) {
    $reports[] = $object;
  }
  else {
    return drupal_not_found();
  }
  return theme('abuse_page', $reports);
}

function abuse_admin_user($uid = NULL) {
  if (empty($uid)) {
    return drupal_not_found();
  }
  $limit = (empty($_GET['limit'])) ? 25 : $_GET['limit'];
  $node_query = "SELECT a.oid, a.type, a.status, a.assigned_to_uid FROM {abuse_status} a INNER JOIN {node} n ON a.oid=n.nid WHERE a.type='node' AND n.uid = %d AND a.status = %d ORDER BY a.oid DESC";
  $comment_query = "SELECT a.oid, a.type, a.status, a.assigned_to_uid FROM {abuse_status} a INNER JOIN {comments} c ON a.oid=c.cid WHERE a.type='comment' AND c.uid = %d AND a.status = %d ORDER BY a.oid DESC";
  $query = "SELECT a.oid, a.type, a.status, a.assigned_to_uid FROM (($node_query) UNION ($comment_query)) AS a ORDER BY a.oid DESC";
  $count_query = "SELECT COUNT(*) FROM (($node_query) UNION ($comment_query)) AS counter";

  $result = pager_query($query, $limit, 0, $count_query, $uid, ABUSE_REMOVED, $uid, ABUSE_REMOVED);
  //$result = db_query("SELECT n.nid, count(a.valid) as num  FROM {node} n INNER JOIN {abuse} a ON a.oid=n.nid WHERE a.type='node' AND n.uid = %d GROUP BY n.nid HAVING num <= 2 ORDER BY num DESC", $uid);
  $reports = array();
  while ($object = db_fetch_object($result)) {
    $obj = _abuse_load($object);
    $reports[] = $obj;
  }
  $content = theme('abuse_page', $reports, $limit);
  return $content;
}

function abuse_admin_allow($type = NULL, $oid = NULL) {
  $object = _abuse_load($type, $oid);
  $status = FALSE;
  $message = t('Sorry, this content could not be allowed');
  if ($object->oid) {
    if (_abuse_allow($type, $oid)) {
      $status = TRUE;
      $message = t('Item allowed');
    }
  }
  return array('status' => $status, 'data' => $message);
}

function abuse_admin_remove($type = NULL, $oid = NULL) {
  $object = _abuse_load($type, $oid);
  $status = FALSE;
  $message = t('Sorry, this content could not be removed');
  if ($object->oid) {
    _abuse_remove($type, $oid);
    $status = TRUE;
    $message = t('Item removed: %title', array('%title' => $object->title));
  }
  return array('status' => $status, 'data' => $message);
}

function abuse_admin_assign_to_superadmin($type = NULL, $oid = NULL) {
  $object = _abuse_load($type, $oid);
  $status = FALSE;
  $message = t('Sorry, this content could not be sent to the super moderator');
  if ($object->oid) {
    _abuse_assign_superadmin($type, $oid);
    $status = TRUE;
    $message = t('Item sent to super moderator: %title', array('%title' => $object->title));
  }
  return array('status' => $status, 'data' => $message);
}

function abuse_admin_warn_user($type, $oid, $subject = NULL, $body = NULL, $op = 'allow') {
  global $user, $language;
  $object = _abuse_load($type, $oid);
  $status = FALSE;
  $message = t('Sorry, the user could not be warned and the operation could not be carried out');
  if ($object->oid) {
    $account = user_load(array('uid' => $object->uid));
    $to = check_plain($account->name) ."<$account->mail>";
    $params = array();
    $params['object'] = $object;
    $params['account'] = $account;
    $params['subject'] = (isset($subject)) ? $subject : variable_get('abuse_warn_subject', t('Abuse warning'));
    $params['body'] = (isset($body)) ? $body : variable_get('abuse_warn_body', '');
    $params['bcc'] = variable_get('abuse_warn_bcc', '');
    drupal_mail('abuse', 'warning_email', $to, $language, $params);
    db_query("INSERT INTO {abuse_warnings} (type, oid, created, uid, sent_by_uid) VALUES ('%s', %d, %d, %d, %d)",
      $type, $oid, time(), $account->uid, $user->uid);
    if ('allow' == $op) {
      _abuse_allow($object->type, $object->oid);
    }
    elseif ('remove' == $op) {
      _abuse_remove($object->type, $object->oid);
    }
    $status = TRUE;
    $message = t('Your message has been sent.');
  }
  return array('status' => $status, 'data' => $message);
}

function abuse_admin_ban_user($uid) {
  global $user;
  $account = user_load(array('uid' => $uid));
  $status = FALSE;
  $message = 'User @name could not be banned.';
  if ($user->uid > 1 && $user->uid !== $account->uid) {
    abuse_remove_account_content($account);
    sess_destroy_uid($uid);
    db_query("INSERT INTO {access} (mask, type, status) VALUES ('%s', 'mail', 0)", $account->mail);
    db_query("INSERT INTO {access} (mask, type, status) VALUES ('%s', 'user', 0)", $account->mail);
    db_query("UPDATE {users} SET status=0 WHERE uid=%d", $account->uid);
    $status = TRUE;
    $message = "The user @name has been banned.";
  }
  return array('status' => $status, 'data' => t($message, array('@name' => $account->name)));
}

function abuse_admin_unban_user($account) {
  db_query("DELETE FROM {access} WHERE mask='%s' AND type='mail' AND status=0", $account->mail);
  db_query("DELETE FROM {access} WHERE mask='%s' AND type='user' AND status=0", $account->name);
  db_query("UPDATE {users} SET status=1 WHERE uid=%d", $account->uid);
  drupal_set_message(t('The user @name has been unbanned', array('@name' => $account->name)));
  drupal_goto("user/$account->uid/edit");
}

function abuse_admin_moderate_content(&$form_state, $type = NULL, $oid = NULL, $inline = FALSE) {
  $object = _abuse_load($type, $oid);
  if (!isset($object)) {
    return drupal_not_found();
  }

  global $user;
  static $run_once;
  $form = array();
  $form['#validate'][] = 'abuse_admin_moderate_content_validate';
  $form['#submit'][] = 'abuse_admin_moderate_content_submit';

  if (!$inline) {
    $form['target'] = array(
      '#type' => 'item',
      '#value' => "<!-- Empty Section -->",
      '#prefix' => '<div id="message-wrapper" class="message status">',
      '#suffix' => '</div>' ,
    );
    $run_once = TRUE;
  }

  $form['object_type'] = array(
    '#type' => 'hidden',
    '#value' => $object->type
  );
  $form['object_oid'] = array(
    '#type' => 'hidden',
    '#value' => $object->oid,
  );
  $form['object_uid'] = array(
    '#type' => 'hidden',
    '#value' => $object->uid,
  );

  if ($inline) {
    $form['redirect'] = array(
      '#type' => 'hidden',
      '#value' => $_GET['q'],
    );
  }

  $form['allow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Allow content on site?'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'class' => 'abuse-report-allow'
    ),
  );
  $form['allow']['message'] = array(
    '#type' => 'item',
    '#value' => t('Are you sure you want to allow "@content"?', array('@content' => $object->title)),
    '#attributes' => array(
      'class' => 'confirm'
    ),
  );
  $form['allow']['allow'] = array(
    '#type' => 'submit',
    '#value' => t('allow'),
    //'#ahah' => $temp,
  );

  $form['remove'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remove content from site?'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'class' => 'abuse-report-remove'
    ),
  );
  $form['remove']['message'] = array(
    '#type' => 'item',
    '#value' => t('Are you sure you want to remove "@content"?', array('@content' => $object->title)),
    '#attributes' => array(
      'class' => 'confirm'
    ),
  );
  $form['remove']['remove'] = array(
    '#type' => 'submit',
    '#value' => t('remove'),
  );

  if (variable_get('abuse_assigned_moderators', FALSE) && !user_access(ADMINISTER_ALL_ABUSE_REPORTS)) {
    $form['assign'] = array(
      '#type' => 'submit',
      '#value' => t('assign to superadmin'),
    );
  }

  $form['warn']['allow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Warn and Allow'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'class' => 'abuse-report-warn-and-allow'
    ),
  );

  $form['warn']['remove'] = array(
    '#type' => 'fieldset',
    '#title' => t('Warn and Remove'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#attributes' => array(
      'class' => 'abuse-report-warn-and-remove'
    ),
  );
  $form['warn']['allow']['allow_subject'] = $form['warn']['remove']['remove_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Warning subject'),
    '#default_value' => variable_get('abuse_warn_subject', t('Abuse warning')),
    '#cols' => 72,
    '#rows' => 10,
    '#required' => TRUE,
  );
  $form['warn']['allow']['allow_body'] = $form['warn']['remove']['remove_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Warning body'),
    '#default_value' => t(variable_get('abuse_warn_body', '')),
    '#size' => 72,
    '#required' => TRUE,
    '#description' => t('available fields are !title, !url, !name, and !id. You can copy from the list below.'),
  );
  $tmp_content = "<div class=\"clear-block\"><dl class=\"warning-templates\">\n";
  $reasons = _abuse_reasons();
  foreach ($reasons as $key => $reason) {
    $tmp_content .= "<!-- $reason->reason -->\n";
    $tmp_content .= "<dt class=\"warning-reason-title\"><a href='#'>". check_plain($reason->reason) ."</a></dt>\n";
    $tmp_content .= "<dd class=\"warning-reason-email\">". check_plain($reason->argumentation) ."</dd>\n";
  }
  $tmp_content .= '</dl></div>';

  $form['warn']['allow']['allow_preset_messages'] = $form['warn']['remove']['remove_preset_messages'] = array(
    '#type' => 'item',
    '#value' => $tmp_content,
  );
  $form['warn']['allow']['allow_warn'] = array(
    '#type' => 'submit',
    '#value' => t('warn and allow'),
  );

  $form['warn']['remove']['remove_warn'] = array(
    '#type' => 'submit',
    '#value' => t('warn and remove'),
  );

  if ($user->uid !== $object->uid && $object->uid > 1) {
    $form['ban'] = array(
      '#type' => 'fieldset',
      '#title' => t('Ban !user?', array('!user' => check_plain($object->name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#attributes' => array(
        'class' => 'abuse-report-ban'
      ),
    );
    $form['ban']['confirmation_message'] = array(
      '#type' => 'item',
      '#value' => t('Are you sure you want to ban "@name"?', array('@name' => $object->name)),
      '#attributes' => array(
        'class' => 'confirm'
      ),
    );
    $form['ban']['ban'] = array(
      '#type' => 'submit',
      '#value' => t('ban')
    );
  }
  return $form;
}

function abuse_admin_moderate_content_validate($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  if (t('ban') === $op && ($values['object_uid'] || $values['object_uid'] <= 1)) {
    form_set_error(NULL, t('User cannot be banned'));
  }
}

function abuse_admin_moderate_content_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  if ($values['redirect']) {
    $form_state['redirect'] = $values['redirect'];
  }
  $message = _abuse_admin_moderate_content_shared($values, $op);
  drupal_set_message($message['data']);
}


function abuse_admin_moderate_content_js() {
  global $user;
  $values = $_POST;
  $op = $values['op'];
  $message = _abuse_admin_moderate_content_shared($values, $op);
  drupal_json($message);
}

function _abuse_admin_moderate_content_shared($values, $op) {
  $message = array('status' => FALSE, 'data' => t('Sorry, could not perform requested operation.'));
  switch ($op) {
    case t('allow'):
      $message = abuse_admin_allow($values['object_type'], $values['object_oid']);
      break;
    case t('remove'):
      $message = abuse_admin_remove($values['object_type'], $values['object_oid']);
      break;
    case t('warn and allow'):
      $message = abuse_admin_warn_user($values['object_type'], $values['object_oid'], $values['allow_subject'], $values['allow_body'], 'allow');
      break;
    case t('warn and remove'):
      $message = abuse_admin_warn_user($values['object_type'], $values['object_oid'], $values['remove_subject'], $values['remove_body'], 'remove');
      break;
    case t('ban'):
      $message = abuse_admin_ban_user($values['object_uid']);
      break;
    case t('assign to superadmin'):
      $message = abuse_admin_assign_to_superadmin($values['object_type'], $values['object_oid']);
      break;
  }
  return $message;
}

function abuse_admin_ban(&$form_state, $account, $inline = FALSE) {
  global $user;
  $form = array();
  $form['#validate'][] = array('abuse_admin_ban_validate');
  $form['#submit'][] = array('abuse_admin_ban_submit');

  $form['#attributes'] = array('class' => 'abuse-admin-ban');
  if ($user->uid === $account->uid || $account->uid === 1) {
    $form['notallowed'] = array(
      '#type' => 'item',
      '#value' => t('Sorry, you are not allowed to ban this user'),
    );
  }
  else {
    if ($inline) {
      $form['message'] = array(
        '#title' => t('Ban !user', array('!user' => $account->name)),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE
      );
      $form['redirect'] = array(
        '#type' => 'value',
        '#value' => $_GET['q'],
      );
      $form['ajax'] = array(
        '#type' => 'hidden',
        '#default_value' => '0',
      );
    }
    $form['message']['uid'] = array(
      '#type' => 'value',
      '#value' => $account->uid
    );
    $form['message']['confirmation_message'] = array(
      '#type' => 'item',
      '#value' => t('Are you sure you want to ban "@name"?', array('@name' => $account->name)),
    );
    $form['message']['confirm'] = array(
      '#type' => 'submit',
      '#value' => t('Yes')
    );
    if ($inline === FALSE) {
      $form['message']['cancel'] = array(
        '#type' => 'item',
        '#value' => l(t('No'), 'admin/content/abuse')
      );
    }
  }
  return $form;
}

function abuse_admin_ban_validate($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  $account = user_load(array('uid' => $values['uid']));
  if (($user->uid === 1 || $user->uid === $account->uid) && t('Yes') == $op) {
    form_set_error(NULL, t('You cannot ban the site administrator or yourself.'));
  }
}

function abuse_admin_ban_submit($form, &$form_state) {
  global $user;
  $values = $form_state['values'];
  $op = $form_state['clicked_button']['#value'];
  $account = user_load(array('uid' => $values['uid']));
  if ($values['redirect']) {
    $form_state['redirect'] = $values['redirect'];
  }
  else {
    $form_state['redirect'] = 'admin/content/abuse';
  }
  if ($user->uid > 1 && $user->uid !== $account->uid && t('Yes') == $op) {
      abuse_remove_account_content($account);

    // ban this email address and username
    db_query("INSERT INTO {access} (mask, type, status) VALUES ('%s', 'mail', 0)", $account->mail);
    db_query("INSERT INTO {access} (mask, type, status) VALUES ('%s', 'user', 0)", $account->name);

    // block this user
    db_query("UPDATE {users} SET status=0 WHERE uid=%d", $account->uid);
    db_query("DELETE FROM {sessions} WHERE uid=%d", $uid);
    drupal_set_message(t('The user @name has been banned.', array('@name' => $account->name)));
  }
}

/**
 * Remove user content
 *
 * @param user $account account of user whose content is being blocked
 */
function abuse_remove_account_content($account) {
  $result = db_query("SELECT nid FROM {node} WHERE uid=%d", $account->uid);
  while ($nid = db_fetch_object($result)) {
    _abuse_remove('node', $nid->nid);
  }

  // remove their comments:
  $result = db_query("SELECT cid FROM {comments} WHERE uid=%d", $account->uid);
  while ($cid = db_fetch_object($result)) {
    _abuse_remove('comment', $cid->cid);
  }
  return TRUE;
}


/**
 * Implement mailing functionality
 */
function abuse_mail($key, &$message, $params) {
  $account = $params['account'];
  $object = $params['object'];
  $vars = array(
    '@title' => $object->title,
    '@url' => $object->link,
    '@name' => $account->name,
    '@id' => $object->link,
  );
  $subject = strtr($params['subject'], $vars);
  $body = strtr($params['body'], $vars);
  $message['subject'] .= str_replace(array("\r", "\n"), '', $subject);
  $message['body'][] = htmlspecialchars($body, ENT_NOQUOTES);
}

function template_preprocess_abuse_page(&$variables) {
  drupal_add_js('misc/jquery.form.js');
  drupal_add_js("Drupal.base_url = '". url('') ."';", 'inline');
  drupal_add_js(drupal_get_path('module', 'abuse') .'/abuse.js');
}

function template_preprocess_abuse_report(&$variables) {
  static $counter;
  if (!isset($counter)) {
    $counter = 1;
  }
  $object = $variables['object'];
  $variables['account'] = user_load(array('uid' => $object->uid));
  $variables['offences'] = number_format(_abuse_get_offence_count($object->uid));
  $variables['warnings'] = number_format(_abuse_get_warning_count($object->uid));
  $variables['nodeType'] = ($object->type == 'comment') ? "Comment" : "Movie";

  $variables['moderate'] = drupal_get_form('abuse_admin_moderate_content'. $counter++, $object->type, $object->oid, TRUE);
}

