<?php
// $Id: user.inc,v 1.1 2008/03/28 16:36:45 nedjo Exp $

function user_activemenu() {
  $items = array();
  $items['#block-user-1'] = 'activemenu/menu';
  return $items;
}