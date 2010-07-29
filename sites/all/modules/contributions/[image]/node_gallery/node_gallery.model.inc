<?php
// $Id: node_gallery.model.inc,v 1.3 2008/11/17 01:46:40 wilson98 Exp $

/**
 * @file
 * Node gallery module. 
 *
 */

class Base {
  function Base($o) {
    foreach ((array)$o as $k => $v) {
      $this->$k = $v;
    }
  }
}

class GalleryConfigGateway {
  static function getBy($type, $op = 'type') {
    static $types;
    
    if (empty($types[$type])) {
      if ($op == 'id' && is_numeric($type)) {
        $type = db_result(db_query("SELECT type FROM {node} WHERE nid = %d", $type));
      }
      $result = db_query("SELECT * FROM {ng_gallery_config} gt WHERE gt.gallery_type = '%s'", $type);
      $t = drupal_unpack(db_fetch_object($result));
      if (!empty($t)) {
        $types[$type] = new GalleryConfig($t);
      }
    }
    return $types[$type];
  }
  
  static function getTypes($op = 'gallery', $type = NULL) {
    static $types;
    if (empty($types)) {
      $result = db_query("SELECT name, gallery_type, image_type FROM {ng_gallery_config}");
      while ($o = db_fetch_object($result)) {
        $types['image'][$o->image_type] = $o;
        $types['gallery'][$o->gallery_type] = $o;
      }      
    }
    return empty($type) ? $types[$op] : $types[$op][$type];
  }
  
  static function isType($op, $type) {
    $types = self::getTypes($op);
    if (in_array($type, array_keys($types))) {
      return TRUE;
    }
    return FALSE;
  }
}

class GalleryConfig extends Base {
  
  function save() {
    if (empty($this->data)) {
      foreach ((array)$this as $k => $v) {
        if ($this[$k] != 'name' ||$this[$k] != 'gallery_type' || $this[$k] != 'image_type') {
          $this->data[$k] = $v;
        }
      }
    }
    $this->data = serialize($this->data);
    
    if (!empty($this->gallery_type) && GalleryConfigGateway::getTypes('gallery', $this->gallery_type)) {
      drupal_write_record('ng_gallery_config', $this, 'gallery_type');
    }
    else {
      drupal_write_record('ng_gallery_config', $this);
    }
  }
  
  function delete() {
    return db_query("DELETE FROM {ng_gallery_config} WHERE gallery_type = '%s'", $this->gallery_type);
  }
}

class GalleryGateway {
  
  static function findBy($uid = NULL, $type = NULL) {
    $gallery_types = empty($type) ? array_keys(GalleryConfigGateway::getTypes()) : $type;
    $args = $gallery_types;
    $sql = "SELECT n.nid, n.title, n.type, n.created, n.uid FROM {node} n 
    WHERE n.type IN (". db_placeholders($gallery_types, 'varchar') .") AND n.status = 1";
    if ($uid) {
      $sql .= " AND n.uid = %d";
      $args[] = $uid;
    }
    $result = db_query($sql, $args);
    while ($r = db_fetch_array($result)) {
      $items[$r['nid']] = new Gallery($r);
    }
    
    return $items;
  }
  
  static function findCovers($gids) {
    $gids = is_numeric($gids) ? array($gids) : (array) $gids;
    $result = db_query("SELECT gid, nid FROM {ng_images} WHERE gid IN (". db_placeholders($gids) .") AND is_cover = 1", $gids);
    while ($r = db_fetch_array($result)) {
      $covers[$r['gid']] = $r['nid'];
    }
    //if no cover was set, get the first image;
    foreach ($gids as $gid) {
      if (empty($covers[$gid])) {
        $gids2[] = $gid;
      }
    }
    if (!empty($gids2)) {
      $result = db_query("SELECT gid, nid FROM {ng_images} WHERE gid IN (". db_placeholders($gids2) .") GROUP BY gid", $gids2);
      while ($r = db_fetch_array($result)) {
        $covers[$r['gid']] = $r['nid'];
      }
    }
    return $covers;
  }
  
  static function countImages($gids) {
    $gids1 = is_numeric($gids) ? array($gids) : (array) $gids;
    $result = db_query("SELECT COUNT(nid) AS count, gid FROM {ng_images} WHERE gid IN (". db_placeholders($gids1) .")
     GROUP BY gid", $gids1);
    while ($r = db_fetch_array($result)) {
      $items[$r['gid']] = empty($r['count']) ? 0 : $r['count'];
    }
    return is_numeric($gids) ? $items[$gids] : $items;
  }
  
  static function delete($gids) {
    $gids = is_numeric($gids) ? array($gids) : (array) $gids;
    return db_query("DELETE FROM {ng_images} WHERE gid IN (". db_placeholders($gids) .")", $gids);
  }
  
  static function getImages($gid) {
    $result = db_query("SELECT nid FROM {ng_images} WHERE gid = %d", $gid);
    while ($r = db_fetch_array($result)) {
      $nids[] = $r['nid'];
    }
    if (!empty($nids)) {
      return ImageGateway::findDetails($nids);
    }
  }
}

class Gallery extends Base {
  function delete() {
    return db_query("DELETE FROM {ng_images} WHERE gid = %d", $this->nid);
  }
  
  function getConfig() {
    if (empty($this->config)) {
      $this->config = GalleryConfigGateway::getBy($this->type);
    }
    
    return $this->config;
  }
  
  function getImages() {
    if (empty($this->images)) {
      $result = db_query("SELECT nid FROM {ng_images} WHERE gid = %d", $this->nid);
      while ($r = db_fetch_array($result)) {
        $nids[] = $r['nid'];
      }
      if (!empty($nids)) {
        $this->images = ImageGateway::findDetails($nids);
        $this->image_count = count($this->images);
      }
    }
    return $this->images;
  }
  
  function getContent($teaser = 0) {
    $node = db_fetch_object(db_query("SELECT teaser, body, format FROM {node_revisions} WHERE nid = %d", $this->nid));
    $node->body = isset($node->body) ? str_replace('<!--break-->', '', $node->body) : '';
    $node = node_prepare($node, $teaser);
    return $node->content['body']['#value'];
  }
  
  function getTitle() {
    if (empty($this->title)) {
      $this->title = db_result(db_query("SELECT title FROM {node} WHERE nid = %d", $this->nid));
    }
    return $this->title;
  }
  
  function getImageNavigator($nid) {
    $result = db_query("SELECT nid FROM {ng_images} WHERE gid = %d ORDER BY weight, nid", $this->nid);
    while ($r = db_fetch_array($result)) {
      $items[] = $r['nid'];
    }
    $navigator['total'] = count($items);
    $navigator['parent'] = $this->nid;
    for ($i = 0; $i < $navigator['total']; $i++) {
      if ($items[$i] == $nid) {
        $navigator['current'] = $i + 1;
        $navigator['prev_nid'] = ($i == 0) ? $items[$navigator['total'] - 1] : $items[$i - 1];
        $navigator['next_nid'] = ($i == ($navigator['total'] - 1)) ? $items[0] : $items[$i + 1];
      }
    }
    return $navigator;
  }
}

class ImageGateway {
  static function findDetails($nids) {
    $nids = is_numeric($nids) ? array($nids) : (array) $nids;

    $sql = "SELECT n.nid, n.vid, n.title, n.type, n.created, nr.body, i.*, f.* FROM {node} n 
      INNER JOIN {node_revisions} nr ON n.vid = nr.vid INNER JOIN {ng_images} i 
      ON n.nid = i.nid INNER JOIN {files} f ON i.fid = f.fid WHERE n.nid IN (". db_placeholders($nids) .") AND n.status = 1
       ORDER BY i.weight, i.nid";
    $result = db_query($sql, $nids);

    while ($o = db_fetch_object($result)) {
      $items[$o->nid] = new Image($o);
    }
    
    if (module_exists('content')) {
      foreach ($items as &$item) {
        $content_type = content_types($item->type);
        if (!empty($content_type['fields'])) {
          content_load($item);
        }
      }
    }
    
    return $items;
  }
  
  static function getFile($nids) {
    $nids2 = is_numeric($nids) ? array($nids) : (array) $nids;
    $result = db_query("SELECT i.*, f.* FROM {ng_images} i INNER JOIN {files} f ON i.fid = f.fid WHERE i.nid 
    IN (". db_placeholders($nids2) .")", $nids2);
    while ($r = db_fetch_array($result)) {
      $items[$r['nid']] = new Image($r);
    }
    return is_numeric($nids) ? $items[$nids] : $items;
  }
  
  static function delete($image) {
    db_query("DELETE FROM {ng_images} WHERE nid = %d", $image->nid);
    db_query("DELETE FROM {files} WHERE fid = %d", $image->fid);
    file_delete($image->filepath);
  }
}

class Image extends Base {
  function delete() {
    node_delete($this->nid);
    db_query("DELETE FROM {ng_images} WHERE nid = %d", $this->nid);
    db_query("DELETE FROM {files} WHERE fid = %d", $this->fid);
    file_delete($this->filepath);
  }
  
  function save($form) {
    module_load_include('inc', 'node', 'node.pages');
    $form_state['values'] = (array)$this;
    node_form_submit($form, $form_state);
    //insert node;
    if (empty($this->nid)) {
      $this->nid = $form_state['nid'];
    }
    file_set_status($this, FILE_STATUS_PERMANENT);
    if ($this->gid && $this->nid) {
      $this->addToGallery();
    }
  }
  
  function addToGallery() {
    //update
    if (db_result(db_query("SELECT nid FROM {ng_images} WHERE gid = %d AND nid = %d", $this->gid, $this->nid))) {
      return drupal_write_record('ng_images', $this, array('gid', 'nid'));
    }
    //insert
    else {
      $has_images = db_result(db_query("SELECT nid FROM {ng_images} WHERE gid = %d", $this->gid));
      //the first upload image is set to default cover;
      if (!$has_images) {
        $this->is_cover = 1;
      }
      else {
        //only one image can be set to cover;
        if ($this->is_cover) {
          db_query("UPDATE {ng_images} SET is_cover = %d WHERE is_cover = 1 AND gid = %d", $this->gid);
        }
      }
      return drupal_write_record('ng_images', $this);
    }
  }
}