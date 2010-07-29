<?php
// $Id: spam_user.inc,v 1.1.2.1.2.7.2.1 2008/12/25 05:42:11 jeremy Exp $

/**
 * @file
 * Include file for integration with the user system.
 */

/**
 *
 */
function spam_user($type, &$edit, &$user, $category = NULL) {
  switch ($type) {
    case 'update':
      $user = spam_user_profile_update($user);
      return spam_content_update($user, 'user');
    case 'insert':
      $user = spam_user_profile_update($user);
      return spam_content_insert($user, 'user');
    case 'delete':
      return spam_content_delete($user, 'user');
    case 'view':
      $links = '';
      if (user_spamapi('filter_content_type', $user)) {
        foreach (spam_links('user', $user->uid, $user) as $link) {
          if ($link['href']) {
            $links .= l($link['title'], $link['href']) .' ';
          }
          else {
            $links .= $link['title'] .' ';
          }
        }
        $items['spam_links'] = array(
          'value' => $links,
          'class' => 'spam',
        );
        $status = db_result(db_query('SELECT status FROM {users} WHERE uid = %d', $user->uid));
        $status_text = t('User status: %status', array('%status' => $status ? t('not blocked') : t('blocked')));
        $items['spam_status'] = array(
          'value' => $status_text,
          'class' => 'spam',
        );
        return array(t('Spam status') => $items);
      }
      break;
  }
}

/**
 * Cache the user id to be sure it's available when we need it.
 */
function _spam_user_uid($id) {
  static $uid = 0;

  if (isset($id) && is_numeric($id)) {
    $uid = $id;
  }

  return $uid;
}

/**
 * User module _spamapi() hook.
 */
function user_spamapi($op, $arg1 = NULL, $arg2 = NULL, $arg3 = NULL) {
  switch ($op) {
    case 'content_module':
      // Register with the spam api as a content type module.
      return 'user';

    case 'content_id':
      if (is_object($arg1)) {
        $arg1 = (array)$arg1;
      }
      return _spam_user_uid($arg1['uid']);

    case 'content_types':
      // Register the "user" content type with the spam module.
      return array(array(
        'name' => t('users'),
        'module' => t('user'),
        'title' => t('Users'),
        'description' => t('Check this box to filter users for spam.'),
        'default_value' => 0,
      ));

    case 'process_form':
      // Hook to scan user before it is inserted into the database.
      if ($arg1 == 'user_register' && is_array($arg2)) {
        $user = $arg2['#post'];
        if (is_array($user) && $user['op'] == t('Create new account')) {
          $_SESSION['spam_form'] = $arg2;
          spam_scan($user, 'user');
        }
        else {
          if (isset($_SESSION['spam_form'])) {
            unset($_SESSION['spam_form']);
          }
        }
      }
      break;

    case 'filter_content_type':
      return (variable_get('spam_filter_users', 0));

    case 'filter_fields':
      // Be sure we're always working with an array.
      if (is_object($arg1)) {
        $arg1 = (array)$arg1;
      }
      // Determine uid so we can cache fields in static.
      if (isset($arg1['uid'])) {
        $uid = $arg1['uid'];
      }
      else {
        $uid = 0;
      }
      // Only figure out filter_fields once per user per page load.
      static $fields = array();
      if (!isset($fields[$uid])) {
        $fields[$uid]['main'] = array('name', 'mail');
        $fields[$uid]['other'] = array();
        if (isset($arg1['signature'])) {
          $fields[$uid]['other'][] = 'signature';
        }
        $result = db_query('SELECT name FROM {profile_fields}');
        while ($profile = db_fetch_object($result)) {
          if (isset($arg1[$profile->name])) {
            $fields[$uid]['other'][] = $profile->name;
          }
        }
      }
      return $fields[$uid];

    case 'redirect':
      if (is_numeric($arg1)) {
        return drupal_goto("/user/$arg1");
      }
      break;

    case 'load':
      if (is_numeric($arg1)) {
        return user_load(array('uid' => $arg1));
      }
      break;

    case 'title':
      return db_result(db_query('SELECT name FROM {users} WHERE uid = %d', $arg1));

    case 'edit_link':
      return "user/$arg1/edit";

    case 'status':
      $status = db_result(db_query('SELECT status FROM {users} WHERE uid = %d', $arg1));
      if ($status == 1) {
        return SPAM_PUBLISHED;
      }
      else {
        return SPAM_NOT_PUBLISHED;
      }

    case 'overview_filter_join':
      return 'INNER JOIN {users} u ON t.content_id = u.uid';

    case 'overview_filter_where':
      switch ($arg1) {
        case 'title':
          return "u.name LIKE '%%%s%%'";
        case 'status':
          return "u.status != %d";
      }

    case 'feedback_filter_join_field':
      return 'u.uid';

    case 'publish':
      // TODO: When un/publish user, should probably also un/publish content
      if (is_numeric($arg1)) {
        module_invoke('user', 'user_operations_unblock', array($arg1));
      }
      break;

    case 'unpublish':
      // TODO: When un/publish user, should probably also un/publish content
      if (is_numeric($arg1)) {
        module_invoke('user', 'user_operations_block', array($arg1));
      }
      break;

    case 'feedback_form':
      $form = array();
      if (is_numeric($form['uid'])) {
        $form['uid'] = array(
          '#type' => 'textfield',
          '#title' => t('User ID'),
          '#value' => $arg1['uid'],
          '#disabled' => TRUE,
        );
      }
      // fall through...
    case 'error_form':
      if (!is_array($form)) {
        $form = array();
      }
      $form['name'] = array(
        '#type' => 'textfield',
        '#title' => t('Username'),
        '#value' => $arg1['name'],
        '#disabled' => TRUE,
      );
      $form['mail'] = array(
        '#type' => 'textfield',
        '#title' => t('Email address'),
        '#value' => $arg1['mail'],
        '#disabled' => TRUE,
      );
      // TODO: Show profile fields.
      return $form;
  }
}

/**
 * Be sure we're scanning the latest profile data.
 */
function spam_user_profile_update($user) {
  $profile = array();

  $result = db_query('SELECT fid, value FROM {profile_values} WHERE uid = %d', $user->uid);
  while ($value = db_fetch_object($result)) {
    $name = db_result(db_query('SELECT name FROM {profile_fields} WHERE fid = %d', $value->fid));
    $user->$name = $value->value;
  }
  return $user;
}
