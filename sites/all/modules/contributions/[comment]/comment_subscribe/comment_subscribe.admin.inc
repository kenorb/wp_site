<?php
// $Id: comment_subscribe.admin.inc,v 1.1.2.1 2008/12/22 09:40:51 zyxware Exp $

/**
 * @file
 * Admin page callbacks for the comment_subscribe module.
 */

/*
 *Function for mail subscription
 */
function comment_subscribe_settings() {
  $form['comment_subscribe_settings'] = array();
  $form['comment_subscribe_settings'][
		'comment_subscribe_regged_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Let registered users save their preferences in their profile.'),
    '#return_value' => 1,
    '#default_value' => variable_get(
		'comment_subscribe_regged_checkbox', TRUE),
    '#description' => t('Letting registered users select/unselect if 
		they want to be notified. If this is disabled, this setting is propagated from their user profile, hence reducing
		and clutter and confusion.')
  );
  $form['comment_subscribe_settings'][
		'comment_subscribe_node_alert'] = array(
    '#type' => 'checkbox',
    '#title' => t('Subscribe to node instead of subscribing to comments.'),
    '#return_value' => 1,
    '#default_value' => variable_get(
		'comment_subscribe_node_alert', FALSE),
    '#description' => t('Show a subscribe to node checkbox instead of subscribe to replies. When a user subscribe for a 
		node he will recieve all notification about all the comments to that node. Subscribing to comments sends 
		notification only about replies to that particular comment. Leave unchecked to enable subscribe to replies.')
  );
  $form['comment_subscribe_settings'][
		'comment_subscribe_default_anon_mailalert'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fill checkbox for followup with YES by default for 
		anonymous users, so they will get an email 
		alert for follow up comments if they do not uncheck it then'),
    '#return_value' => 1,
    '#default_value' => variable_get(
		'comment_subscribe_default_anon_mailalert', TRUE),
    '#description' => t('This flag presets the flag for the follow-up 
		notification on the form that anonymous users will 
    see when posting a comment')
  );
  $form['comment_subscribe_settings'][
		'comment_subscribe_default_mailtext'] = array(
    '#type' => 'textarea',
    '#title' => t('Default mail text for sending out the 
		notifications.'),
    '#description' => t("You can use the following variables to be 
		replaced 
    <ul>
    <li>   !commname = the username who posted the comment
    <li>   !commtext = the text of the posted comment
    <li>   !commsubj = the subject of the posted comment
    <li>   !comment_url = the full url of the post and comment - note: 
		if you have paging enabled, this does not work 
    correct - set your max comments per page so that all fit on one 
		page or reverse order them    
    <li>   !node_title = the title of the node that was commented on    
    <li>   !node_teaser = the teaser of the node that was commented 
		on    
    <li>   !node_body = the body of the node that was commented on    
    <li>   !mission = site_mission text    
    <li>   !name = username receiving the alert   
    <li>   !site = your site    
    <li>   !uri = base_url of site   
    <li>   !uri_brief = base_url of site w/o http    
    <li>   !date = the current time    
    <li>   !login_uri  uri to login the user   
    <li>   !edit_uri = uri to edit user profile   
    <li>   !link1 the QUICKLINK to disable future follow-up mail 
		notifications for the article
    <li>   !link2 the QUICKLINK to disable future reply mail 
		notifications for the user's comment
    </ul>"),
    '#default_value' => variable_get(
		'comment_subscribe_default_mailtext', t(DEFAULT_MAILTEXT) ),
    '#return_value' => 1,
    '#cols' => 80,
    '#rows' => 15
  );
  $form['comment_subscribe_settings'][
    'comment_subscribe_default_author_mailtext'] = array(
    '#type' => 'textarea',
    '#title' => t('Default mail text for sending out the 
    notifications to authors.'),
    '#description' => t("You can use the following variables to be 
    replaced 
    <ul>
    <li>   !commname = the username who posted the comment
    <li>   !commtext = the text of the posted comment
    <li>   !commsubj = the subject of the posted comment
    <li>   !comment_url = the full url of the post and comment - note: 
    if you have paging enabled, this does not work 
    correct - set your max comments per page so that all fit on one 
    page or reverse order them    
    <li>   !node_title = the title of the node that was commented on    
    <li>   !node_teaser = the teaser of the node that was commented 
    on    
    <li>   !node_body = the body of the node that was commented on    
    <li>   !mission = site_mission text    
    <li>   !name = username receiving the alert   
    <li>   !site = your site    
    <li>   !uri = base_url of site   
    <li>   !uri_brief = base_url of site w/o http    
    <li>   !date = the current time    
    <li>   !login_uri  uri to login the user   
    <li>   !edit_uri = uri to edit user profile   
    <li>   !link1 the QUICKLINK to disable future follow-up mail  
    notifications for the article
    <li>   !link2 the QUICKLINK to disable future reply mail 
    notifications for the user's comment
    </ul>"),
    '#default_value' => variable_get(
    'comment_subscribe_default_author_mailtext', t(DEFAULT_AUTHOR_MAILTEXT) ),
    '#return_value' => 1,
    '#cols' => 80,
    '#rows' => 15
  );
  
  return system_settings_form($form);
}
