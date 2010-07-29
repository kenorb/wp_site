<?php
// $Id: composite-layout-twocol-bricks.tpl.php,v 1.1 2008/10/14 01:03:56 bengtan Exp $

/**
 * @file 
 *
 * Variables:
 * - $content: Original node content (from $node->body)
 * - $layout: Layout definition (from $node->composite_layout)
 *
 * Layout specific variables:
 * - $left: Content for the left zone. 
 *
 * @see template_preprocess_composite_content
 */
?>
<div class="composite composite-twocol clear-block">
  <div class="composite-zone composite-row composite-row-top">
    <div class="composite-zone-inner">
      <?php print $top; ?>
    </div>
  </div>

  <div class="composite-center-wrapper">
    <div class="composite-zone composite-column composite-column-first">
      <div class="composite-zone-inner">
        <?php print $left; ?>
      </div>
    </div>

    <div class="composite-zone composite-column composite-column-last">
      <div class="composite-zone-inner">
      <?php print $right; ?>
      </div>
    </div>
  </div>

  <div class="composite-zone composite-row composite-row-middle">
    <div class="composite-zone-inner">
      <?php print $middle_row; ?>
    </div>
  </div>

  <div class="composite-center-wrapper">
    <div class="composite-zone composite-column composite-column-first">
      <div class="composite-zone-inner">
        <?php print $left2; ?>
      </div>
    </div>

    <div class="composite-zone composite-column composite-column-last">
      <div class="composite-zone-inner">
      <?php print $right2; ?>
      </div>
    </div>
  </div>

  <div class="composite-zone composite-row composite-row-bottom">
    <div class="composite-zone-inner">
      <?php print $bottom; ?>
    </div>
  </div>

</div>
