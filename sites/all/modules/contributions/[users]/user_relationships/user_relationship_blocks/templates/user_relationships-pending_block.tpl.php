<?php
// $Id: user_relationships-pending_block.tpl.php,v 1.1.2.6 2008/10/30 13:10:47 alexk Exp $
/**
 * @file
 * Template for relationships requests block
 * List all pending requests and provide links to the actions that can be taken on those requests
 */
if ($relationships) {
  $list = array();
  foreach ($relationships as $relationship) {
    $relation_name = $relationship->name;
    $relation_plural_name = $relationship->plural_name;
    if ($user->uid == $relationship->requester_id) {
      $line = '@rel_name to !username (!controls)';
      $relation_to =& $relationship->requestee;
      $key = t('Sent requests');
      $controls = theme('user_relationships_pending_request_cancel_link', $user->uid, $relationship->rid);
    }
    else {
      $line = '@rel_name from !username (!controls)';
      $relation_to =& $relationship->requester;
      $key = t('Received requests');
      $controls =
        theme('user_relationships_pending_request_approve_link', $user->uid, $relationship->rid).'|'.
        theme('user_relationships_pending_request_disapprove_link', $user->uid, $relationship->rid);
    }
    $list[$key][] = t($line, array(
      '@rel_name' => $relationship->name,
      '@rel_plural_name' => $relationship->plural_name,
      '!username' => theme('username', $relation_to),
      '!controls' => $controls
    ));
  }

  foreach ($list as $title => $users) {
    $output[] = theme('item_list', $users, $title);
  }
}

print $output ? implode('', $output) : t('No Pending Requests');

?>