<?php
// $Id: user_relationships-block.tpl.php,v 1.1.2.5 2008/10/30 12:49:33 alexk Exp $
/**
 * @file Main relationships listing block
 * List the relationships between the viewed user and the current user
 */
if ($relationships) {
  $the_other_uid = $settings->block_type == UR_BLOCK_MY ? $user->uid : $account->uid;
  $showing_all_types = $settings->rtid == UR_BLOCK_ALL_TYPES;
  $rows = array();
  foreach ($relationships as $rtid => $relationship) {
    if ($the_other_uid == $relationship->requester_id) {
      $rtype_heading = $relationship->is_oneway ? "@rel_name of" : "@rel_plural_name";
      $relatee = $relationship->requestee;
    }
    else {
      $rtype_heading = $relationship->is_oneway ? "@rel_plural_name" : "@rel_plural_name";
      $relatee = $relationship->requester;
    }

    $title = t($rtype_heading, array('@rel_plural_name' => $relationship->plural_name, '@rel_name' => $relationship->name, '@extra' => $extra));

    $username = theme('username', $relatee);
    if ($showing_all_types) {
      $rows[$title]['children'][] = $username;
    }
    else {
      $rows[$title][] = $username;
    }
  }

  foreach ($rows as $title => $users) {
    $output[] = theme('item_list', ($rtid == UR_BLOCK_ALL_TYPES ? array($users) : $users), $showing_all_types ? $title : NULL);
  }

  print implode('', $output);
}
/* removing printing out empty placeholder so the block is hidden when no data
// No relationships so figure out how we present that
else {
  if ($settings->rtid == UR_BLOCK_ALL_TYPES) {
    $rtype_name = 'relationships';
  }
  else {
    $rtype      = user_relationships_type_load($settings->rtid);
    $rtype_name = $rtype->plural_name;
  }

  if ($account->uid == $user->uid) {
    print t('You have no @rels', array('@rels' => $rtype_name));
  }
  else {
    print t('!name has no @rels', array('!name' => theme('username', $account), '@rels' => $rtype_name));
  }
}
*/
?>