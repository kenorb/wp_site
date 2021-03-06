<?php
// $Id: comment.rules.inc,v 1.1.2.2 2008/08/25 15:29:08 fago Exp $

/**
 * @file rules integration for the comment module
 */

/**
 * Implementation of hook_event_info()
 * @ingroup rules
 */
function comment_rules_event_info() {
  return array(
    'comment_insert' => array(
      'label' => t('After saving a new comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('created comment')),
      'redirect' => TRUE,
    ),
    'comment_update' => array(
      'label' => t('After saving an updated comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('updated comment')),
      'redirect' => TRUE,
    ),
    'comment_delete' => array(
      'label' => t('After deleting a comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('deleted comment')),
      'redirect' => TRUE,
    ),
    'comment_view' => array(
      'label' => t('Comment is being viewed'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('viewed comment')),
    ),
    'comment_publish' => array(
      'label' => t('After publishing a comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('published comment')),
      'redirect' => TRUE,
    ),
    'comment_unpublish' => array(
      'label' => t('After unpublishing a comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('unpublished comment')),
      'redirect' => TRUE,
    ),
  );
}

/**
 * Returns some arguments suitable for hook comment
 */
function rules_events_hook_comment_arguments($comment_label) {
  return array(
    'comment' => array(
      'type' => 'comment',
      'label' => $comment_label,
    ),
    'comment_author' => array(
      'type' => 'user',
      'label' => $comment_label .' '. t('author'),
      'handler' => 'rules_events_argument_comment_author',
    ),
    'node' => array(
      'type' => 'node',
      'label' => t('commented content'),
      'handler' => 'rules_events_argument_comment_node',
    ),
    'node_author' => array(
      'type' => 'user',
      'label' => t('commented content author'),
      'handler' => 'rules_events_argument_comment_node_author'),
    ) + rules_events_global_user_argument();
}

/**
 * Gets the author of the comment
 */
function rules_events_argument_comment_author($comment) {
  return user_load(array('uid' => $comment->uid));
}

/**
 * Gets the comment's node
 */
function rules_events_argument_comment_node($comment) {
  return node_load($comment->nid);
}

/**
 * Gets the comment's node's author
 */
function rules_events_argument_comment_node_author($comment) {
  return rules_events_argument_node_author(rules_events_argument_comment_node($comment));
}

/**
 * Implementation of hook_rules_data_type_info()
 */
function comment_rules_data_type_info() {
  return array(
    'comment' => array(
      'label' => t('Comment'),
      'class' => 'rules_data_type_comment',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'module' => 'Comment',
    ),
  );
}

/**
 * Defines the comment data type
 */
class rules_data_type_comment extends rules_data_type {

  function load($cid) {
    return _comment_load($cid);
  }

  function get_identifier() {
    $comment = $this->get();
    return $comment->cid;
  }
}
