<?php

function ad_image_views_tables() {
  return array(
    'ad_image' => array(
      'name' => 'ad_image',
      'join' => array(
        'left' => array(
          'table' => 'ads',
          'field' => 'aid',
        ),
        'right' => array(
          'field' => 'aid',
        ),
      ),
      'fields' => array(
        'url' => array(
          'name' => 'Image Ad: Click-through URL',
          'handler' => 'ad_handler_field_url',
          'option' => array(
            '#type' => 'select',
            '#options' => array(
              'link' => t('As link'),
              'nolink' => t('Not as link'),
            ),
          ),
        ),
        'aid' => array(
          'name' => 'Image Ad: Image',
          'handler' => 'ad_image_handler_field_image',
          'option' => array(
            '#type' => 'select',
            '#options' => array(
              'linkimage' => t('Linked Image'),
              'image' => t('Unlinked Image'),
              'path' => t('Image path'),
            ),
          ),
        ),
      ),
    ),
  );
}

function ad_image_handler_field_image($fieldinfo, $fielddata, $value, $data) {
  if ($fielddata['options'] == 'linkimage') {
    $ad = db_fetch_object(db_query("SELECT a.redirect, f.filepath FROM {ads} a JOIN {ad_image} i JOIN {files} f ON a.aid = i.aid AND i.aid = f.nid AND i.fid = f.fid WHERE i.aid = %d AND a.adtype = 'image'", $value));
    if ($ad) {
      ad_statistics_increment($value, 'view');
      return "<a href=\"$ad->redirect\"><img src=\"$ad->filepath\" /></a>";
    }
  }
  else if ($fielddata['options'] == 'image') {
    $ad = db_fetch_object(db_query("SELECT f.filepath FROM {ad_image} i JOIN {files} f ON i.aid = f.nid AND i.fid = f.fid WHERE i.aid = %d AND a.adtype = 'image'", $value));
    if ($ad) {
      ad_statistics_increment($value, 'view');
      return "<img src=\"$ad->filepath\" />";
    }
  }
  else {
    $ad = db_fetch_object(db_query("SELECT f.filepath FROM {ad_image} i JOIN {files} f ON i.aid = f.nid AND i.fid = f.fid AND i.aid = %d", $value));
    return $ad->filepath;
  }
}
