<?php
// $Id: composite-layout-onecol.tpl.php,v 1.1 2008/10/14 01:03:56 bengtan Exp $

/**
 * @file composite-layout-one.tpl.php
 * Composite content template a single column layout
 *
 * Variables:
 * - $layout: Layout definition (from $node->composite_layout)
 *
 * Layout specific variables:
 * - $left: Content for the left zone. 
 *
 * @see template_preprocess_composite_content
 */
?>
<div class="composite composite-onecol clear-block">
  <div class="composite-zone composite-column">
    <?php print $left; ?>
  </div>
</div>