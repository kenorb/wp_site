<?php
// $Id: user_relationship_mailer_defaults.inc,v 1.1.2.5 2008/10/01 09:38:11 alexk Exp $

global $user_relationship_mailer_ops;
$user_relationship_mailer_ops = array('request', 'cancel', 'approve', 'disapprove', 'remove');

/**
 * Replacements for mail messages
 * @param relationship relationship object for which emails are being sent
 */
function user_relationship_mailer_replacements(&$relationship) {
  $requester = $relationship->requester;
  $requestee = $relationship->requestee;
  //load relationship type, if necessary
  if (!$relationship->relationship_type) {
    $relationship->relationship_type = user_relationships_type_load($relationship->rtid);
  }
  $replacements = array(
    '@site'                     => variable_get('site_name', 'Drupal'),
    '@site_url'                 => $GLOBALS['base_url'],

    '@requester_name'           => $requester->name,
    '@requester_link'           => theme('user_relationships_user_link', $requester->uid),
    '@requester_uid'            => $requester->uid,
    '@requester_list_link'      => url("user/{$requester->uid}/relationships/list", array('absolute' => TRUE)),
    '@requester_pending_link'   => url("user/{$requester->uid}/relationships/requests", array('absolute' => TRUE)),

    '@requestee_name'           => $requestee->name,
    '@requestee_link'           => theme('user_relationships_user_link', $requestee->uid),
    '@requestee_uid'            => $requestee->uid,
    '@requestee_list_link'      => url("user/{$requestee->uid}/relationships/list", array('absolute' => TRUE)),
    '@requestee_pending_link'   => url("user/{$requestee->uid}/relationships/requests", array('absolute' => TRUE)),

    '@relationship_name'        => $relationship->relationship_type->name,
    '@relationship_plural_name' => $relationship->relationship_type->plural_name,
  );

  //'remove' action needs a few more variables for the template
  if ($deleted_by = $relationship->deleted_by) {
    $replacements['@deleted_by_name'] = $deleted_by->name;
    $replacements['@deleted_by_link'] = url("user/{$deleted_by->uid}", array('absolute' => TRUE));
    $replacements['@deleted_by_uid']  = $deleted_by->uid;
  }

  return $replacements;
}

/**
 * Default request relationship message
 */
function user_relationship_mailer_request_default() {
  $subject = "[@site] @requester_name wants to be your @relationship_name";
  $message = <<<MESSAGE
Dear @requestee_name,

@requester_name wants to be your @relationship_name.

Here's a link to @requester_name's profile:
  @requester_link

To approve/disapprove this, log in to @site_url and see your pending relationship requests at @requestee_pending_link.

Regards,
The @site team
This message was sent to you because you have user relationships email notifications turned on.
If you no longer wish to receive these emails, you can turn them off at @site_url/user/@requestee_uid/edit.
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}

/**
 * Default cancel request message
 */
function user_relationship_mailer_cancel_default() {
  $subject = "[@site] @requester_name has canceled their @relationship_name request";
  $message = <<<MESSAGE
Dear @requestee_name,

@requester_name has canceled their @relationship_name request.

Regards,
The @site team

This message was sent to you because you have user relationships email notifications turned on.
If you no longer wish to receive these emails, you can turn them off at @site_url/user/@requestee_uid/edit.
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}

/**
 * Default approve to relationship message
 */
function user_relationship_mailer_approve_default() {
  $subject = "[@site] You are @requestee_name's newest @relationship_name";
  $message = <<<MESSAGE
Dear @requester_name,

@requestee_name has approved your request to be their @relationship_name.

Here's a link to your relationships:
  @requester_list_link

Regards,
The @site team

This message was sent to you because you have user relationships email notifications turned on.
If you no longer wish to receive these emails, you can turn them off at @site_url/user/@requester_uid/edit.
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}

/**
 * Default disapprove to relationship message
 */
function user_relationship_mailer_disapprove_default() {
  $subject = "[@site] @requestee_name has declined your relationship request";
  $message = <<<MESSAGE
Dear @requester_name,

@requestee_name has declined your request to be their @relationship_name.

Regards,
The @site team

This message was sent to you because you have user relationships email notifications turned on.
If you no longer wish to receive these emails, you can turn them off at @site_url/user/@requester_uid/edit.
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}

/**
 * Default removed relationship message
 */
function user_relationship_mailer_remove_default() {
  $subject = "[@site] @relationship_name relationship between @requester_name and @requestee_name has been deleted";
  $message = <<<MESSAGE
Dear @target_name,

@requester_name's @relationship_name relationship to @requestee_name has been deleted by @deleted_by_name.

Regards,
The @site team

This message was sent to you because you have user relationships email notifications turned on.
If you no longer wish to receive these emails, you can turn them off at @site_url/user/@profile_uid/edit.
MESSAGE;

  return array(
    'subject' => $subject,
    'message' => $message,
  );
}
