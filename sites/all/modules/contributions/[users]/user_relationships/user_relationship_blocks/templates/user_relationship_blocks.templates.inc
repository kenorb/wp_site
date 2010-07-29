<?php
// $Id: user_relationship_blocks.templates.inc,v 1.1.2.6 2008/10/30 12:49:33 alexk Exp $
/**
 * @file 
 * misc block theming functions
 */

/**
 * Template pre processor for the main block view
 */
function template_preprocess_user_relationships_block(&$variables) {
  $user     =& $variables['user'];
  $account  =& $variables['account'];
  $settings =& $variables['settings'];
  $extra    =& $variables['extra'];

  $query_opts = array('include_user_info' => TRUE);

  // select the appropriate set of relationships based on admin's configuration settings
  switch ($settings->sort) {
    case 'newest': $query_opts['order'] = 'updated_at DESC'; break;
    case 'oldest': $query_opts['order'] = 'updated_at ASC'; break;
    case 'random': $query_opts['order'] = 'RAND()'; break;
  }

  $query_opts['limit'] = $settings->size;

  $key = $extra ? ($extra == 'you_to_them' ? 'requester_id' : 'requestee_id') : 'user';
  $args = array($key => $account->uid, 'approved' => TRUE);

  if ($settings->rtid != UR_BLOCK_ALL_TYPES) {
    $args['rtid'] = $settings->rtid;
  }

  $variables['relationships'] = user_relationships_load($args, $query_opts);
}


/**
 * Template pre processor for the pending relationships block
 */
function template_preprocess_user_relationships_pending_block(&$variables) {
  $account =& $variables['account'];

  $variables['relationships'] = user_relationships_load(array('user' => $account->uid, 'approved' => FALSE), array('include_user_info' => TRUE));
}


/**
 * Template pre processor for the relationship actions block
 */
function template_preprocess_user_relationships_actions_block(&$variables) {
  $user     =& $variables['user'];
  $account  =& $variables['account'];

  if ($user != $account) {
    $variables['current_relationships'] = _user_relationships_ui_between($user, $account);
  }

  $variables['actions'] = _user_relationships_ui_actions_between($user, $account);
}
