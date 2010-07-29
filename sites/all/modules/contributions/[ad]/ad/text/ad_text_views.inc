<?php

function ad_text_views_tables() {
  return array(
    'ad_text' => array(
      'name' => 'ad_text',
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
          'name' => 'Text Ad: Click-through URL',
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
          'name' => 'Text Ad: Advertisement',
          'handler' => 'ad_text_handler_field_text',
          'option' => array(''),
        ),
      ),
    ),
  );
}

function ad_text_handler_field_text($fieldinfo, $fielddata, $value, $data) {
  $ad = db_fetch_object(db_query("SELECT a.redirect, t.adheader, t.adbody FROM {ads} a JOIN {ad_text} t ON a.aid = t.aid WHERE t.aid = %d AND a.adtype = 'text'", $value));
  if ($ad) {
    ad_statistics_increment($value, 'view');
    return "<a href=\"$ad->redirect\">$ad->adheader</a><br />$ad->adbody";
  }
}
