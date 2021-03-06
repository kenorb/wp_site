<?php
// $Id: system_service.inc,v 1.1.2.2 2008/09/01 12:54:08 brmassa Exp $
/**
 * @author Services Dev Team
 * @file
 *  Link general system functionalities to services module.
 */

/**
 * Returns a specified node.
 */
function system_service_connect() {
  global $user;

  $return = new stdClass();
  $return->sessid = session_id();
  $return->user = $user;

  return $return;
}

function system_service_mail($mailkey, &$message, $params) {
  $language = $message['language'];
  $variables = user_mail_tokens($params['account'], $language);
  $message['subject'] = t($params['subject'], $variables, $language->language);
  $message['body'] = t($params['body'], $variables, $language->language);
  foreach ($params['headers'] as $header => $val) {
    $message['headers'][$header] = $val;
  }
}

/**
 * Send an email using the Services module.
 */
function system_service_mailprepare($mailkey, $to, $subject, $body, $from = NULL, $headers = array()) {
  $params = array();
  $params['subject'] = $subject;
  $params['body'] = $body;
  $params['headers'] = $headers;
  $status = drupal_mail('system_service', $mailkey, $to, user_preferred_language($to), $params, $from, TRUE);

  if (!$status) {
    return services_error(t('There was a problem sending your email.'));
  }
  return $status;
}

/**
 * Returns a specified variable.
 */
function system_service_getvariable($name, $default = NULL) {
  return variable_get($name, $default);
}

/**
 * Set a variable.
 */
function system_service_setvariable($name, $value) {
  variable_set($name, $value);
}

/**
 * Check if a module is enabled. If so, return its version.
 */
function system_service_module_exists($module) {
  if (module_exists($module)) {
    $modules = module_rebuild_cache();
    if (array_key_exists($module, $modules)) {
      return (string)$modules[$module]->info['version'];
    }
  }

  return "";
}
