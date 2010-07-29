<?php
// $Id: lm_paypal.ipn.inc,v 1.1.2.5 2009/01/18 20:34:38 pounard Exp $

/**
 * Handle an incoming IPN.
 *
 * PayPal sends one or more IPNs here for each transaction that takes place. The
 * IPN is present as a form submission which this routine unravels and saves for
 * processing.
 */
function lm_paypal_ipn_in() {
  // file_put_contents('/tmp/ipn-post.txt', serialize($_POST));
  watchdog(LM_PAYPAL, 'lm_paypal_ipn_in called', NULL);

  // Don't bother with these fields - but don't flag them as errors
  static $ignore_fields = array(
    // 'notify_version',
    'receipt_id',
    'charset',
    'mc_gross1',
  );

  // Process the incoming form results to check for unexpected values.
  $schema = drupal_get_schema('lm_paypal_ipns');
  foreach ($_POST as $key => $value) {
    if ($value == '' || in_array($key, $ignore_fields)) {
      continue;
    }

    if (!isset($schema['fields'][$key])) {
      watchdog(LM_PAYPAL, 'IPN unknown field ignored: !key => !value', array('!key' => check_plain($key), '!value' => check_plain($value)), WATCHDOG_ERROR);
    }
  }


  $validated = _lm_paypal_validate_ipn($_POST);

  // Sanitize and remove risky fields from $_POST to make it suitable for
  // drupal_write_record. Include the entire POST data for debugging.
  $ipn = $_POST;
  $ipn['timestamp'] = time();
  $ipn['ipn'] = serialize($_POST);
  unset($ipn['processed']);

  // Add the IPN to our database (whether it validated or not).
  $wr = drupal_write_record('lm_paypal_ipns', $ipn);
  if ($wr == SAVED_NEW) {
    $last = $ipn['id'];
    watchdog(LM_PAYPAL, 'lm_paypal_ipn_in $last = '. $ipn['id'], NULL);
    $link = l(t('view'), "admin/settings/lm_paypal/id/$ipn[id]");

    if ($validated) {
      watchdog(LM_PAYPAL, 'IPN incoming %type', array('%type' => check_plain($ipn['txn_type'])), WATCHDOG_NOTICE, $link);
      lm_paypal_process_in((object)$ipn);
    }
    else {
      watchdog(LM_PAYPAL, 'IPN incoming NOT VERIFIED %type got %ret', array('%type' => check_plain($_POST['txn_type']), '%ret' => check_plain($validate_result->data)), WATCHDOG_ERROR, $link);
    }
  }
  else {
    watchdog(LM_PAYPAL, 'IPN in failed to run sql: %sql', array('%sql' => check_plain($sql)), WATCHDOG_ERROR);
    // Return an HTTP error and hopefully PayPal will resend the IPN to me
    // later on and then I can try again! Maybe PayPal is very busy
    // or there is a network problem at the moment
  }


  return 'IPN: Only PayPal will ever see this page - humans go away!';
}




/**
 * Process a newly arrived IPN message that has been verified and saved.
 *
 * @param $id
 *  The id of the saved IPN to be processed.
 */
function lm_paypal_process_in($ipn) {
  $business = lm_paypal_api_get_business();
  $link = l(t('view'), "admin/settings/lm_paypal/id/$ipn->id");

  if ($ipn->test_ipn != '' && ! variable_get('lm_paypal_obey_test_ipns', FALSE)) {
    watchdog(LM_PAYPAL, 'test_ipn received - ignoring', array(), WATCHDOG_WARNING, $link);
    return;
  }

  if (strcasecmp(trim($ipn->receiver_email), trim($business)) != 0) {
    watchdog(
      LM_PAYPAL,
      'Incoming IPN received email does not match business email (received %received, business %business)',
      array('%received' => check_plain($ipn->receiver_email), '%business' => check_plain($business)),
      WATCHDOG_ERROR,
      $link);
    return;
  }

  // Every implementor of hook_lm_paypal_ipn($ipn) is free to take whatever
  // actions they want, based on the IPN, and return a boolean describing
  // whether they handled the IPN.  No attempt is made to prevent multiple
  // modules handling the same IPN.
  $results1 = module_invoke_all('lm_paypal_ipn', $ipn);

  // Additionally, IPNs are displatched to a txn_type-specific hook.
  $txn_type = preg_replace('/[^a-z0-9_]*/', '', $ipn->txn_type);
  $results2 = module_invoke_all('lm_paypal_ipn_' . $txn_type, $ipn);

  // Determine whether _some_ module handled the IPN, and warn if not.
  $processed = FALSE;
  foreach ($results1 as $res) {
    if ($res) {
      $processed = TRUE;
      break;
    }
  }
  if (!$processed) {
    foreach ($results2 as $res) {
      if ($res) {
        $processed = TRUE;
        break;
      }
    }
  }

  if (!$processed) {
    watchdog(LM_PAYPAL, 'No processor for this IPN, ignoring: %type', array('%type' => check_plain($ipn->txn_type)), WATCHDOG_WARNING, $link);
  }

  return;
/*
  // Don't check for processed txn_id's here as txn_id's are not provided
  // for all subscr messages. Check then in the message type specific processors

  // Find a processor.
  // It's either lm_paypal_process_in_<type> (e.g.: ..._in_subscr_payment)
  // or if not then strip any trailing _XXX and try the remaining
  //  (e.g.: ..._in_subscr)
   $in = 'lm_paypal_process_in_';
  $f = $in . $ipn->txn_type;
  if (function_exists($f)) {
    $res = $f($ipn);
    module_invoke_all('lm_paypal_process_in', $ipn, $f, $res); // TODO: ->actions?
    return $res;
  }
  $u = strpos($ipn->txn_type, '_');
  if ($u > 0) {
    $f = $in . substr($ipn->txn_type, 0, $u);
    if (function_exists($f)) {
      $res = $f($ipn);
      module_invoke_all('lm_paypal_process_in', $ipn, $f, $res); // TODO: -> actions
      return $res;
    }
  }
*/
}

/**
 * Implementation of hook_lm_paypal_ipn.
 */
/*
 function lm_paypal_lm_paypal_ipn($ipn) {
  error_log("Called lm_paypal_lm_paypal_ipn($ipn->txn_id)");
  return TRUE;
}
*/

/**
 * Implementation of hook_lm_paypal_ipn_TXNTYPE.
 * Process a send_money IPN message.
 */
function lm_paypal_lm_paypal_ipn_send_money($ipn) {
  // error_log("Called lm_paypal_lm_paypal_ipn_send_money($ipn->txn_id)");
  if (lm_paypal_debug()) {
    watchdog(LM_PAYPAL, 'lm_paypal_lm_paypal_ipn_send_money (passing to web_accept)');
  }
  return lm_paypal_process_in_web_accept($ipn);
}

/**
 * Implementation of hook_lm_paypal_ipn_TXNTYPE.
 * Process a web_accept IPN message.
 */
function lm_paypal_lm_paypal_ipn_web_accept($ipn) {
  // error_log("Called lm_paypal_lm_paypal_ipn_web_accept($ipn->txn_id)");
  _lm_paypal_process_in_web_accept($ipn);
  return TRUE;
}



/**
 * Process a newly arrived web_accept IPN message
 *
 * @param $ipn
 */
function _lm_paypal_process_in_web_accept($ipn) {
  $debug = variable_get('lm_paypal_debug', LM_PAYPAL_DEBUG_DEFAULT);
  if ($debug) {
    watchdog(LM_PAYPAL, 'in_web_accept');
  }

  $link = l(t('view'), "admin/settings/lm_paypal/id/$ipn->id");

  if (lm_paypal_already_processed($ipn->txn_id)) {
    watchdog(
      LM_PAYPAL,
      'This transaction has already been processed, ignored: %id',
      array('%id' => check_plain($ipn->txn_id)),
      WATCHDOG_WARNING,
      $link);
    return;
  }

  lm_paypal_mark_processed($ipn);

  if ($ipn->payment_status == 'Pending') {
    watchdog(
      LM_PAYPAL,
      'Ignoring IPN with status: Pending. Check your PayPal account to see why it is pending. Note: pending_reason: %reason',
      array('%reason' => check_plain($ipn->pending_reason)),
      WATCHDOG_ERROR,
      $link);
    return;
  }

  $custom = lm_paypal_unpack_ipn_custom($ipn);
  $uid = $custom['uid'];
  $other = $custom['other'];

  if ($uid == '') {
    $uid = 0;
    if ($_lm_paypal_debug) {
      watchdog(
        LM_PAYPAL,
        'No uid, try to lookup payer_email',
        array(),
        WATCHDOG_WARNING,
        $link);
    }
    $users = db_query("SELECT uid FROM {users} WHERE LOWER(mail) = LOWER('%s')", $ipn->payer_email);
    $user = db_fetch_object($users);
    if (!$user) {
      watchdog(
        LM_PAYPAL,
        'IPN web_accept no uid presuming uid 0, cannot find payer_email: %email',
        array('%email' => check_plain($ipn->payer_email)),
        WATCHDOG_WARNING,
        $link);
      $uid = 0;
    }
    else {
      $uid = $user->uid;
      watchdog(
        LM_PAYPAL,
        'IPN web_accept no uid, found payer_email %email for uid %uid',
        array('%email' => check_plain($ipn->payer_email), '%uid' => $uid),
        WATCHDOG_WARNING,
        $link);
    }
  }
  else if (!is_numeric($uid) || intval($uid) != $uid || $uid < 0) {
    watchdog(
      LM_PAYPAL,
      'Invalid uid, ignoring IPN: %uid',
      array('%uid' => $uid),
      WATCHDOG_WARNING,
      $link);
    return;
  }

  // If I receive a web_accept without a uid then presume it came from anon
  if ($uid != '') {
    // Is this uid valid?
    $users = db_query("SELECT * FROM {users} WHERE uid = %d", $uid);
    $user = db_fetch_object($users);
    if (!$user) {
      watchdog(
        LM_PAYPAL,
        'IPN web_accept unknown uid, presuming uid 0: %uid',
        array('%uid' => check_plain($uid)),
        WATCHDOG_ERROR,
        $link);
      $uid = 0;
    }
  }

  // Use the item_number to select the kind of payment coming in
  $item_number = $ipn->item_number;

  // If you use the Send Money menu item on PayPal I treat this pretty the
  //  same as a donation (item_number = 0)
  if ($ipn->txn_type == 'send_money') {
    if ($_lm_paypal_debug) {
      watchdog(LM_PAYPAL, "send_money - being converted to web_accept");
    }
    $item_number = 0;
  }
  else if ($item_number == '') {
    if ($_lm_paypal_debug) {
      watchdog(LM_PAYPAL, "empty item_number - being converted to web_accept");
    }
    $item_number = 0;
  }
  else if (!is_numeric($item_number) || intval($item_number) != $item_number || $item_number < 0) {
    watchdog(
      LM_PAYPAL,
      'Invalid item_number, ignoring IPN: %item_number',
      array('%item_number' => check_plain($item_number)),
      WATCHDOG_WARNING,
      $link);
    return;
  }

  return lm_paypal_web_accept_invoke($ipn, $link, $uid, $other, $item_number);
}

function lm_paypal_web_accept_invoke($ipn, $link, $uid, $other, $item_number) {
  // error_log("lm_paypal_web_accept_invoke($ipn->txn_id)");

  // HACK!  Remove this, and figure out how the web_accept path relates to top-
  // level IPNs.
  if (module_exists('lm_paypal_donations')) {
    include_once drupal_get_path('module', 'lm_paypal_donations') . '/lm_paypal_donations.module';
    if ($item_number == 0) {
      return _lm_paypal_process_in_donate($ipn, $link, $uid, $other, $item_number);
    }
  }

/*
  // Find the correct web_accept processor
  $ranges = lm_paypal_web_accept_register();
  if ($ranges) {
    foreach ($ranges as $r) {
      $f = $r['fun'];
      $min = $r['min'];
      $max = $r['max'];
      //watchdog(LM_PAYPAL,"found $f $min $max");
      if ($min <= $item_number && $item_number <= $max) {
        return $f($ipn, $link, $uid, $other, $item_number);
      }
    }
  }
*/
  watchdog(
    LM_PAYPAL,
    'No web_accept processor registered for this item_number: %item_number',
    array('%item_number' => check_plain($item_number)),
    WATCHDOG_WARNING,
    $link);
}





/**
 * Given the full $_POST data from an incoming IPN, check with PayPal that this
 * is a real IPN.  Without this check, IPNs may be spoofed.
 *
 * @param array $post
 *   The key-value pairs from an incoming IPN (presumably straight from $_POST)
 * @return
 *   TRUE if PayPal successfully validated the request.  This will not happen if
 *   there was an error communicating with the service, the IPN did not
 *   originate from PayPal, or it was tampered with in any way.
 */
function _lm_paypal_validate_ipn($post) {
  // If lm_paypal_skip_validation is set, then don't validate the incoming IPN
  // with PayPal. Never enable this on a real website!!
  if (variable_get('lm_paypal_skip_validation', FALSE)) {
    return TRUE;
  }

  // Build the IPN validation request from the $post fields
  $req = 'cmd=_notify-validate';
  foreach ($post as $key => $value) {
    $req .= "&$key=" . urlencode(stripslashes($value));
  }

  // Validate this incoming IPN by sending it to PayPal to be checked
  $lm_paypal_host = variable_get('lm_paypal_host', LM_PAYPAL_HOST_DEFAULT);
  $validate_result = drupal_http_request(
    lm_paypal_api_get_host(TRUE),
    array('Content-Type' => 'application/x-www-form-urlencoded'),
    'POST',
    $req
  );

  return $validate_result->data == 'VERIFIED';
}
