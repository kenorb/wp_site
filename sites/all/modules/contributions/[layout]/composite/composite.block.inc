<?php
// $Id: composite.block.inc,v 1.1.2.2 2008/12/06 04:44:05 bengtan Exp $

function composite_composite_block_api(&$reference, $op, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    // Derive additional fields 
    case 'load':
      list($reference['module'], $reference['delta']) = explode('-', $reference['id'], 2);
      break;

    // Generate and insert an informative human-readable string into ['info']
    case 'info':
      $blocks = module_invoke($reference['module'], 'block', 'list');
      $reference['info'] = check_plain($blocks[$reference['delta']]['info']);      
      break;

    // Return a rendering of the reference item
    case 'view':
      $block = new StdClass();
      $block->module = $reference['module'];
      $block->delta = $reference['delta'];
      $array = module_invoke($block->module, 'block', 'view', $block->delta);
      if (isset($array) && is_array($array)) {
        foreach ($array as $k => $v) {
          $block->$k = $v;
        }
      }
      if ($block->content) {
        // Fetch a custom title if one has been defined for this block for this theme
        global $theme_key;
        $result = db_query("SELECT title FROM {blocks} WHERE theme = '%s' AND module = '%s' AND delta = '%s'", $theme_key, $block->module, $block->delta);
        if ($object = db_fetch_object($result)) {
          if ($object->title) {
            $block->subject = $object->title == '<none>' ? '' : check_plain($object->title);
          }
        }        
        return theme('block', $block);        
      }
      break;

  }
}

/**
 * Generates and returns a keyed array of potential composite references
 *   so composite.module can create a meaningful local task. 
 */
function composite_composite_block_potentials($node) {
  // Build a list of blocks
  // Note: Instead of using code in blocks.module, we generate our own
  //   since we want this list to be theme-independent
  $block_list = array();
  foreach (module_implements('block') as $module) {
    $module_blocks = module_invoke($module, 'block', 'list');
    if (is_array($module_blocks)) {
      foreach ($module_blocks as $delta => $block) {
        $block_list[$module . '-' . $delta] = $block['info'];
      }
    }
  }
  asort($block_list);
  return $block_list;
}
