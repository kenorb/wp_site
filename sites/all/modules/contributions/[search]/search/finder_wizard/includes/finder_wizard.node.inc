<?php
// $Id: finder_wizard.node.inc,v 1.1.2.3 2008/11/08 05:37:06 danielb Exp $


/**
 * Get an array of content types for use in select forms
 */

function finder_wizard_get_types() {
  $types = node_get_types($op = 'types', $node = NULL, $reset = FALSE);
  $type_array = array();
  $type_array[''] = t('<none>');  // this is the 'select none' option in the form
  foreach ((array)$types as $type) {
    $type_array[$type->type] = $type->name;
  }
  if (count($type_array) == 1) {
    return FALSE;
  }
  return $type_array;
}