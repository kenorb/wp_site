<?php
// $Id: session-apc-db.inc,v 1.1 2007/04/19 06:44:53 slantview Exp $

/**
 * @file
 * User session handling functions.
 */

function sess_open($save_path, $session_name) {
  return TRUE;
}

function sess_close() {
  return TRUE;
}

function sess_read($key) {
  global $user;

  // Write and Close handlers are called after destructing objects since PHP 5.0.5
  // Thus destructors can use sessions but session handler can't use objects.
  // So we are moving session closure before destructing objects.
  register_shutdown_function('session_write_close');

  // Handle the case of first time visitors and clients that don't store cookies (eg. web crawlers).
  if (!isset($_COOKIE[session_name()])) {
    $user = drupal_anonymous_user();
    return '';
  }

  // If user session is in memcache, use it, otherwise load from db.
  $user = apc_handle_get($key, 'session');
  if(!$user) {
  	$user = db_fetch_object(db_query("SELECT u.*, s.* FROM {users} u INNER JOIN {sessions} s ON u.uid = s.uid WHERE s.sid = '%s'", $key));
  }
  
  if($user && $user->uid) {
    if(!isset($user->roles)) {
      $user = drupal_unpack($user);
      // Add roles element to $user
      $user->roles = array();
      $user->roles[DRUPAL_AUTHENTICATED_RID] = 'authenticated user';
      $result = db_query("SELECT r.rid, r.name FROM {role} r INNER JOIN {users_roles} ur ON ur.rid = r.rid WHERE ur.uid = %d", $user->uid);
      while ($role = db_fetch_object($result)) {
        $user->roles[$role->rid] = $role->name;
      }
    }
  } else  {
    $session = isset($user->session) ? $user->session : '';
    $user = drupal_anonymous_user($session);
  }
    
  return $user->session;
}

function sess_write($key, $value) {
  global $user;
  
  // If the client doesn't have a session, and one isn't being created ($value), do nothing.
  if(empty($_COOKIE[session_name()]) && empty($value)) {
    return TRUE;
  }
  
  // If we have a session in memcache, lets keep track.
  $in_memcache = apc_handle_get($key, 'session');
  if(!$in_memcache) {
    $result = db_query("SELECT sid FROM {sessions} WHERE sid = '%s'", $key);
    $in_db = db_num_rows($result);
  }
  
  if(!$in_memcache && $user->uid) { 
    // Prepare the information to be saved
    $info = new stdClass;
    $info->sid = $key;
    $info->uid = $user->uid;
    $info->cache = $user->cache;
    $info->hostname = $_SERVER["REMOTE_ADDR"];
    $info->session = $value;
    $info->timestamp = time();
    $put_in_memcache = apc_handle_set($key, $info, ini_get('session.gc_maxlifetime'), 'session');
  }

  // First time the browser sends back the cookie, save to both db and memcache in case of memcache failure
  if((!$in_db && !$in_memcache) && ($user->uid || $value || count($_COOKIE))) {
      db_query("INSERT INTO {sessions} (sid, uid, cache, hostname, session, timestamp) VALUES ('%s', %d, %d, '%s', '%s', %d)", $key, $user->uid, $user->cache, $_SERVER["REMOTE_ADDR"], $value, time());
  } else {
    // On memcache_set failure, we update the database 
    if(!$in_memcache && !$put_in_memcache) {
      db_query("UPDATE {sessions} SET uid = %d, cache = %d, hostname = '%s', session = '%s', timestamp = %d WHERE sid = '%s'", $user->uid, $user->cache, $_SERVER["REMOTE_ADDR"], $value, time(), $key);
    }
    
    //Let's only update every 5 minutes for now.
    /*
    if(variable_get('session_access_update', 0) < time()) {
      db_query("UPDATE {users} SET access = %d WHERE uid = %d", time(), $user->uid);
      variable_set('session_access_update', time() + 300);
    }
    */
  }
  
  

  return TRUE;
}

/**
 * Called when an anonymous user becomes authenticated or vice-versa.
 */
function sess_regenerate() {
  $old_session_id = session_id();

  // We code around http://bugs.php.net/bug.php?id=32802 by destroying
  // the session cookie by setting expiration in the past (a negative
  // value).  This issue only arises in PHP versions before 4.4.0,
  // regardless of the Drupal configuration.
  // TODO: remove this when we require at least PHP 4.4.0
  if (isset($_COOKIE[session_name()])) {
    setcookie(session_name(), '', time() - 42000, '/');
  }

  session_regenerate_id();
  $key = session_id();
  
  // Grab the user's information that is cached with the anonymous key
  $info = apc_handle_get($old_session_id, 'session');
  if($info) { 
    // Update it.
    $info->sid = $key;
    // Store it with the new key.
    apc_handle_set($key, $info, ini_get('session.gc_maxlifetime'), 'session');
    // Clear the old data from the cache.
    apc_handle_delete($old_session_id, 'session');
  }
  //Always keep the db synced with memcache in case of failure
  db_query("UPDATE {sessions} SET sid = '%s' WHERE sid = '%s'", $key, $old_session_id);
}

/**
 * Counts how many users have sessions. Can count either anonymous sessions, authenticated sessions, or both.
 *
 * @param int $timestamp
 *   A Unix timestamp representing a point of time in the past.
 *   The default is 0, which counts all existing sessions.
 * @param int $anonymous
 *   TRUE counts only anonymous users.
 *   FALSE counts only authenticated users.
 *   Any other value will return the count of both authenticated and anonymous users.
 * @return  int
 *   The number of users with sessions.
 */
function sess_count($timestamp = 0, $anonymous = true) {
  /*
  $query = $anonymous ? ' AND uid = 0' : ' AND uid > 0';
  return db_result(db_query('SELECT COUNT(sid) AS count FROM {sessions} WHERE timestamp >= %d'. $query, $timestamp));
  */
}

/**
 * Called by PHP session handling with the PHP session ID to end a user's session.
 *
 * @param  string $sid
 *   the session id
 */
function sess_destroy_sid($sid) {
  apc_handle_delete($sid, 'session');
  db_query("DELETE FROM {sessions} WHERE sid = '%s'", $sid);
}

/**
 * End a specific user's session
 *
 * @param  string $uid
 *   the user id
 */
function sess_destroy_uid($uid) {
  apc_handle_delete($uid,'session');
  db_query('DELETE FROM {sessions} WHERE uid = %d', $uid);
}

function sess_gc($lifetime) {
  // Be sure to adjust 'php_value session.gc_maxlifetime' to a large enough
  // value. For example, if you want user sessions to stay in your database
  // for three weeks before deleting them, you need to set gc_maxlifetime
  // to '1814400'. At that value, only after a user doesn't log in after
  // three weeks (1814400 seconds) will his/her session be removed.
  if(variable_get('session_gc_update', 0) < time()) {
    db_query("DELETE FROM {sessions} WHERE timestamp < %d", time() - $lifetime);
    variable_set('session_gc_update', time() + 300);
  }
  l
  return TRUE;
}

