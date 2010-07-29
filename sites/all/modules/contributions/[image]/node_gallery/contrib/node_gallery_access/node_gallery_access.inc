<?php
// $Id: node_gallery_access.inc,v 1.3 2008/11/17 01:46:40 wilson98 Exp $

/**
 * @file
 * Node gallery access model file. 
 *
 */

class NgAccessGateway {
  
  static $salt = "fslkj131rlkjl";
  
  static function find($nid) {
    return db_fetch_object(db_query("SELECT * FROM {ng_access} WHERE nid = %d", $nid));
  }
  
  static function delete($nid) {
    return db_query("DELETE FROM {ng_access} WHERE nid = %d", $nid);
  }
  
  static function save($access) {
    $result = db_fetch_object(db_query("SELECT * FROM {ng_access} WHERE nid = %d", $access->nid));
    //insert
    if (empty($result)) {
      if ($access->access_type < 2) {
        return db_query("INSERT INTO {ng_access} (nid, access_type) VALUES(%d, %d)",
       $access->nid, $access->access_type);
      }
      else {
        return db_query("INSERT INTO {ng_access} (nid, access_type, password) VALUES(%d, %d, '%s')",
       $access->nid, $access->access_type, self::setPassword($access->password));
      }
    }
    //update
    else {
      if ($access->access_type < 2) {
        if ($access->access_type == $result->access_type) {
          return TRUE;
        }
        else {
          db_query("UPDATE {ng_access} SET access_type = %d, password = '' WHERE nid = %d", 
            $access->access_type, $access->nid);
        }
      }
      //password type;
      else {
        //user didn't change password;
        if (empty($access->password)) {
          return TRUE;
        }
        else {
          return db_query("UPDATE {ng_access} SET access_type = %d, password = '%s' WHERE nid = %d", 
            $access->access_type, self::setPassword($access->password), $access->nid);
        }
      }
    }
  }
  
  static function insert($access) {
    return db_query("INSERT INTO {ng_access} (nid, access_type, password) VALUES(%d, %d, '%s')",
       $access->nid, $access->access_type, self::setPassword($access->password));
  }
  
  static function update($access) {
    return db_query("UPDATE {ng_access} SET access_type = %d, password = '%s' WHERE nid = %d", 
    $access->access_type, self::setPassword($access->password), $access->nid);
  }
  
  static function setPassword($raw) {
    return sha1(self::$salt . $raw);
  }
}