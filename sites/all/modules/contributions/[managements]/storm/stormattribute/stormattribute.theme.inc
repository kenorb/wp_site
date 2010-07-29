<?php

function theme_stormattribute_list($header, $attributes) {
  $i = new stdClass();
  $i->type = 'stormattribute';

  $rows = array();
  foreach ($attributes as $attribute) {
    $rows[] = array(
      $attribute->domain,
      $attribute->akey,
      $attribute->avalue,
      array(
        'data' => storm_icon_edit('storm/attributes/edit/'. $attribute->aid, $i, $_GET) .'&nbsp;'.  storm_icon_delete('storm/attributes/delete/'. $attribute->aid, $i, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}
