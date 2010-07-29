<?php
// $Id: user_relationships_ui.templates.inc,v 1.1.2.6 2008/10/28 11:28:44 alexk Exp $
/**
 * @file
 * User Relationships theming functions
 */

/**
 * Pre processor for user_relationships page
 */
function template_preprocess_user_relationships(&$variables) {
  global $user;

  // readability
  $account  = $variables['account'];
  $rtid     = $variables['rtid'];

  $args = array('user' => $account->uid, 'approved' => TRUE);
  if (isset($rtid) && is_numeric($rtid)) {
    $relationship_type = user_relationships_type_load($rtid);
    $args['rtid'] = $rtid;
  }

  $query = _user_relationships_generate_query($args, array('include_user_info' => TRUE));

  // To Page or not to Page
  if ($variables['relationships_per_page'] = variable_get('user_relationships_relationships_per_page', 16)) {
    $results = pager_query($query['query'], $variables['relationships_per_page'], 0, $query['count'], $query['arguments']);
  }
  else {
    $results = db_query($query['query'], $query['arguments']);
  }

  $variables['relationships'] = array();
  while ($relationship = db_fetch_object($results)) {
    user_relationships_api_translate_user_info($relationship);
    $variables['relationships'][$relationship->rid] = $relationship;
  }

  if ($account->uid == $user->uid) {
    $msg = $relationship_type ? t("My %relationships") : t("All my relationships");
  }
  else {
    $msg = $relationship_type ? t("%username's %relationships") : t("All %username's relationships");
  }

  $variables['title'] = t($msg, array(
    '%username'       => $account->name,
    '%relationships'  => $relationship_type->plural_name ? $relationship_type->plural_name : $relationship_type->name
  ));
}

/**
 * Pre processor page for user_relationships_pending_requests page
 */
function template_preprocess_user_relationships_pending_requests(&$variables) {
  global $user;

  // readability
  $account  =& $variables['account'];

  $msg = $account->uid == $user->uid ? t('My pending relationships') : t("%username's pending relationships");
  $variables['title'] = t($msg, array('%username' => $account->name));

  $variables['relationships_per_page'] = variable_get('user_relationships_relationships_per_page', 16);

  $variables['sections'] = array(
    'requester_id' => 'sent_requests',
    'requestee_id' => 'received_requests'
  );
  $pager_id = 0;
  foreach ($variables['sections'] as $column => $section) {
    $query = _user_relationships_generate_query(array($column => $account->uid, 'approved' => FALSE), array('include_user_info' => TRUE));

    if ($relationships_per_page = variable_get('user_relationships_relationships_per_page', 16)) {
      $result = pager_query($query['query'], $variables['relationships_per_page'], $pager_id++, $query['count'], $query['arguments']);
    }
    else {
      $result = db_query($query['query'], $query['arguments']);
    }

    while ($relationship = db_fetch_object($result)) {
      user_relationships_api_translate_user_info($relationship);
      $variables[$section][$relationship->rid] = $relationship;
    }
  }
}
