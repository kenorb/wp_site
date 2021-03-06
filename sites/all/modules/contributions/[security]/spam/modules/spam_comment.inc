<?php
// $Id: spam_comment.inc,v 1.1.2.1.2.11.2.1 2008/12/25 05:42:11 jeremy Exp $

/**
 * @file
 * Include file for integration with comments.
 */

define('SPAM_COMMENT', 9);

/**
 * Drupal _comment() hook.
 */
function comment_comment(&$comment, $op) {
  switch ($op) {
    case 'update':
      spam_content_update($comment, 'comment');
      break;
    case 'insert':
      spam_content_insert($comment, 'comment');
      break;
    case 'delete':
      spam_content_delete($comment, 'comment');
      break;
    case 'form':
      if (is_array($comment['cid'])) {
        _spam_comment_cid($comment['cid']['#value']);
      }
      break;
    case 'view':
      if (spam_score_is_spam(_spam_comment_score($comment->cid))) {
        $comment->status = COMMENT_NOT_PUBLISHED;
      }
      break;
  }
}

/**
 * Cache the comment id to be sure it's available when we need it.
 */
function _spam_comment_cid($id = NULL) {
  static $cid = 0;

  if (isset($id) && is_numeric($id)) {
    $cid = $id;
  }

  return $cid;
}

/**
 * Spam module _spamapi() hook.
 */
function comment_spamapi($op, $arg1 = NULL, $arg2 = NULL, $arg3 = NULL) {
  switch ($op) {
    case 'content_module':
      // Register with the spam api as a content type module.
      return 'comment';

    case 'content_id':
      // Tell the spam module the id of a given comment.
      if (is_object($arg1)) {
        // The delete hook uses an object instead of an array.
        $arg1 = (array)$arg1;
      }
      return _spam_comment_cid($arg1['cid']);

    case 'content_types':
      // Register the "comment" content type with the spam module.
      return array(array(
        'name' => t('comments'),
        'module' => t('comment'),
        'title' => t('Comments'),
        'description' => t('Check this box to filter comments for spam.'),
        'default_value' => 1,
      ));

    case 'filter_content_type':
      return (variable_get('spam_filter_comments', 1));

    case 'filter_fields':
      // Tell spam module which fields it should scan for spam.
      $fields['main'] = array('subject', 'comment');
      if (is_object($arg1)) {
        // The delete hook uses an object instead of an array.
        $arg1 = (array)$arg1;
      }
      if (isset($arg1['author'])) {
        $fields['other'][] = 'author';
      }
      if (isset($arg1['name'])) {
        $fields['other'][] = 'name';
      }
      if (isset($arg1['mail'])) {
        $fields['other'][] = 'mail';
      }
      if (isset($arg1['homepage'])) {
        $fields['other'][] = 'homepage';
      }
      return $fields;

    case 'feedback_form':
      $form = array();
      if (is_numeric($form['cid'])) {
        $form['cid'] = array(
          '#type' => 'textfield',
          '#title' => t('Comment ID'),
          '#value' => $arg1['cid'],
          '#disabled' => TRUE,
        );
      }
      // fall through...
    case 'error_form':
      if (!is_array($form)) {
        $form = array();
      }
      $form['comment'] = array(
        '#type' => 'fieldset',
        '#title' => 'Comment',
      );
      $form['comment']['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Subject'),
        '#value' => $arg1['subject'],
        '#disabled' => TRUE,
      );
      $form['comment']['body'] = array(
        '#type' => 'textarea',
        '#title' => t('Comment'),
        '#value' => $arg1['comment'],
        '#disabled' => TRUE,
      );
      return $form;

    case 'load':
      return db_fetch_object(db_query('SELECT * FROM {comments} WHERE cid = %d', $arg1));

    case 'title':
      return db_result(db_query('SELECT subject FROM {comments} WHERE cid = %d', $arg1));

    case 'edit_link':
      return "comment/edit/$arg1";

    case 'status':
      $status = db_result(db_query('SELECT status FROM {comments} WHERE cid = %d', $arg1));
      if ($status == COMMENT_PUBLISHED) {
        return SPAM_PUBLISHED;
      }
      else {
        return SPAM_NOT_PUBLISHED;
      }



    case 'hostname':
      return db_result(db_query('SELECT hostname FROM {comments} WHERE cid = %d', $arg1));

    case 'link':
      if (is_object($arg1) && isset($arg1->cid)) {
        return spam_links('comment', $arg1->cid, $arg1);
      }
      break;

    case 'redirect':
      $nid = db_result(db_query('SELECT nid FROM {comments} WHERE cid = %d', $arg1));
      return drupal_goto("/node/$nid", NULL, "comment-$arg1");

    case 'overview_filter_join':
      return 'INNER JOIN {comments} c ON t.content_id = c.cid';

    case 'overview_filter_where':
      switch ($arg1) {
        case 'title':
          return "c.subject LIKE '%%%s%%'";
        case 'status':
          return 'c.status = %d';
      }

    case 'menu':
      // Create page for listing all spam comments in comment admin section.
      $items = array();
      $items['admin/content/comment/list/spam'] = array(
        'title' => t('Spam'),
        'callback' => 'spam_comment_admin',
        'callback arguments' => array('spam'),
        'access' => user_access('administer spam'),
        'type' => MENU_LOCAL_TASK,
      );
      return $items;

    case 'publish':
      // Only update comments that exist and need to be published.
      $nid = db_result(db_query('SELECT nid FROM {comments} WHERE cid = %d AND status = %d OR status = %d', $arg1, COMMENT_NOT_PUBLISHED, SPAM_COMMENT));
      if ($nid) {
        db_query('UPDATE {comments} SET status = %d WHERE cid = %d', COMMENT_PUBLISHED, $arg1);
        _comment_update_node_statistics($nid);
      }
      break;

    case 'unpublish':
      // Only update comments that exist and need to be unpublished.
      // When 'unpublishing' comments, we actually mark them as spam.
      $nid = db_result(db_query('SELECT nid FROM {comments} WHERE cid = %d AND status = %d', $arg1, COMMENT_PUBLISHED));
      if ($nid) {
        db_query('UPDATE {comments} SET status = %d WHERE cid = %d', SPAM_COMMENT, $arg1);
        _comment_update_node_statistics($nid);
      }
      break;
  }
}

/**
 * Form alter gets it's own function so we can reference &$form without causing
 * errors in PHP4 installations.  (If we use spamapi, we have to set a default,
 * which PHP4 doesn't support.)
 */
function comment_spamapi_form_alter(&$form, &$form_state, $form_id) {
  // Hook to scan comment before it is inserted into the database.
  if ($form_id == 'comment_form' && is_array($form)) {
    $form['admin']['status']['#options'][9] = t('Spam');
    $comment = $form['#post'];
    if (is_array($comment) && $comment['op'] == t('Post comment')) {
      $_SESSION['spam_form'] = $form;
      $cid = $form['cid']['#value'];
      // No need to scan a comment if it's being manually marked as spam.
      if ($comment['status'] == 9) {
        if (!spam_score_is_spam(_spam_comment_score($cid))) {
          // Mark previously not-spam comment as spam
          spam_mark_as_spam('comment', $cid);
        }
      }
      else {
        if (spam_score_is_spam(_spam_comment_score($cid))) {
          // Mark previously spam comment as not-spam
          spam_mark_as_not_spam('comment', $cid);
        }
        else {
          spam_scan($comment, 'comment');
        }
      }
    }
    elseif (isset($_SESSION['spam_form'])) {
      unset($_SESSION['spam_form']);
    }
  }
  // Add spam options to comment form
  elseif ($form_id == 'comment_admin_overview' || $form_id == 'spam_comment_admin_overview') {
    $parameters = $form['#parameters'];
    if (is_array($parameters)) {
      if (!in_array('new', $parameters)) {
        $form['options']['operation']['#options']['markasnotspam'] = 'Mark the selected comments as not spam';
      }
      if (!in_array('spam', $parameters)) {
        $form['options']['operation']['#options']['markasspam'] = 'Mark the selected comments as spam';
      }
      if (in_array('new', $parameters)) {
        $form['options']['operation']['#options']['teachnotspam'] = 'Teach filters selected comments are not spam.';
      }
    }
    if (is_array($form['#post']) && is_array($form['#post']['comments'])) {
      foreach ($form['#post']['comments'] as $cid) {
        $score = _spam_comment_score($cid);
        if ($form['#post']['operation'] == 'markasspam') {
          if (!spam_score_is_spam($score)) {
            spam_mark_as_spam('comment', $cid);
          }
        }
        else if ($form['#post']['operation'] == 'markasnotspam') {
          if (spam_score_is_spam($score)) {
            spam_mark_as_not_spam('comment', $cid);
          }
        }
        else if ($form['#post']['operation'] == 'teachnotspam') {
          spam_mark_as_not_spam('comment', $cid);
        }
      }
    }
  }
}

/**
 * Retreive spam score, caching in memory for repeated use.
 */
function _spam_comment_score($cid) {
  static $scores = array();

  if (!isset($scores[$cid])) {
    $scores[$cid] = db_result(db_query("SELECT score FROM {spam_tracker} WHERE content_type = 'comment' AND content_id = %d", $cid));
  }

  return $scores[$cid];
}

/**
 * Menu callback; present an administrative spam comment listing.
 */
function spam_comment_admin() {
  $edit = $_POST;

  if ($edit['operation'] == 'delete' && $edit['comments']) {
    return drupal_get_form('comment_multiple_delete_confirm');
  }
  else {
    return drupal_get_form('spam_comment_admin_overview', arg(4));
  }
}

/**
 * The 5.x version of comment_admin_overview doesn't provide hooks to easily
 * create a spam overview page.  This is nearly an exact copy of that function,
 * with minor changes for loading spam and processing it.
 */
function spam_comment_admin_overview(&$form_state, $arg) {
  // build an 'Update options' form
  $form['options'] = array(
    '#type' => 'fieldset', '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">', '#suffix' => '</div>'
  );
  $options = array(
    'markasnotspam' => t('Mark the selected comments as not spam'),
    'delete' => t('Delete the selcted comments'),
  );

  $form['options']['operation'] = array('#type' => 'select', '#options' => $options, '#default_value' => 'markasnotspam');
  $form['options']['submit'] = array('#type' => 'submit', '#value' => t('Update'));

  // load the comments that we want to display
  $form['header'] = array('#type' => 'value', '#value' => array(
    theme('table_select_header_cell'),
    array('data' => t('Subject'), 'field' => 'subject'),
    array('data' => t('Author'), 'field' => 'name'),
    array('data' => t('Time'), 'field' => 'timestamp', 'sort' => 'desc'),
    array('data' => t('Operations'))
  ));
  $result = pager_query('SELECT c.subject, c.nid, c.cid, c.comment, c.timestamp, c.status, c.name, c.homepage, u.name AS registered_name, u.uid FROM {comments} c INNER JOIN {users} u ON u.uid = c.uid WHERE c.status = %d'. tablesort_sql($form['header']['#value']), 50, 0, NULL, SPAM_COMMENT);

  // build a table listing the appropriate comments
  $destination = drupal_get_destination();
  while ($comment = db_fetch_object($result)) {
    $comments[$comment->cid] = '';
    $comment->name = $comment->uid ? $comment->registered_name : $comment->name;
    $form['subject'][$comment->cid] = array('#value' => /* TODO
   Please manually fix the parameters on the l() or url() function on the next line.
   Typically, this was not changed because of a function call inside an array call like
   array('title' => t('View user profile.')).*/
l($comment->subject, 'node/'. $comment->nid, array('title' => truncate_utf8($comment->comment, 128)), NULL, 'comment-'. $comment->cid));
    $form['username'][$comment->cid] = array('#value' => theme('username', $comment));
    $form['timestamp'][$comment->cid] = array('#value' => format_date($comment->timestamp, 'small'));
    $form['operations'][$comment->cid] = array('#value' => l(t('edit'), 'comment/edit/'. $comment->cid, array('query' => $destination)));
  }
  $form['comments'] = array('#type' => 'checkboxes', '#options' => $comments);
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  return $form;
}

/**
 * Duplicate function from comment module so we can set correct goto url.
 */
function spam_comment_admin_overview_validate($form, &$form_state) {
  $form_state['values']['comments'] = array_diff($form_state['values']['comments'], array(0));
  if (count($form_state['values']['comments']) == 0) {
    form_set_error('', t('Please select one or more comments to perform the update on.'));
    drupal_goto('admin/content/comment/list/spam');
  }
}

/**
 * Re-use function from comment module.
 */
function spam_comment_admin_overview_submit($form, &$form_state) {
  $form_state['redirect'] = comment_admin_overview_submit($form_state['values']['form_id'], $form_state['values']);
}

/**
 * Re-use function from comment module.
 */
function theme_spam_comment_admin_overview($form_state, $form) {
  return (theme_comment_admin_overview($form));
}

