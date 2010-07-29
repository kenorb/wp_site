<?php
// $Id: composite-layout-threecol-flexigrid.tpl.php,v 1.1.2.1 2008/11/19 15:10:07 bengtan Exp $

/**
 * @file 
 *
 * Variables:
 * - $layout: Layout definition (from $node->composite_layout)
 *
 * Layout specific variables:
 * - $left: Content for the left zone. 
 * - $right: Content for the right zone. 
 *
 * @see template_preprocess_composite_content
 */
?>
<div class="composite composite-threecol-flexigrid clear-block <?php print $composite_classes; ?>">
  <?php if ($row1_has_content): ?>
    <div class="composite-zone composite-row composite-row-1 <?php print $row1_classes; ?>">
      <?php if ($left): ?>
        <div class="composite-zone composite-column composite-column-first">
          <div class="composite-zone-inner">
            <?php print $left; ?>
          </div>
        </div>
      <?php endif; ?>

      <?php if ($middle): ?>
        <div class="composite-zone composite-column composite-column-middle">
          <div class="composite-zone-inner">
            <?php print $middle; ?>
          </div>
        </div>
      <?php endif; ?>
        
      <?php if ($right): ?>
        <div class="composite-zone composite-column composite-column-last">
          <div class="composite-zone-inner">
          <?php print $right; ?>
          </div>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php if ($row2_has_content): ?>
    <div class="composite-zone composite-row composite-row-2 <?php print $row2_classes; ?>">
      <?php if ($left2): ?>
        <div class="composite-zone composite-column composite-column-first">
          <div class="composite-zone-inner">
            <?php print $left2; ?>
          </div>
        </div>
      <?php endif; ?>

      <?php if ($middle2): ?>
        <div class="composite-zone composite-column composite-column-middle">
          <div class="composite-zone-inner">
            <?php print $middle2; ?>
          </div>
        </div>
      <?php endif; ?>
        
      <?php if ($right2): ?>
        <div class="composite-zone composite-column composite-column-last">
          <div class="composite-zone-inner">
          <?php print $right2; ?>
          </div>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php if ($row3_has_content): ?>
    <div class="composite-zone composite-row composite-row-3 <?php print $row3_classes; ?>">
      <?php if ($left3): ?>
        <div class="composite-zone composite-column composite-column-first">
          <div class="composite-zone-inner">
            <?php print $left3; ?>
          </div>
        </div>
      <?php endif; ?>

      <?php if ($middle3): ?>
        <div class="composite-zone composite-column composite-column-middle">
          <div class="composite-zone-inner">
            <?php print $middle3; ?>
          </div>
        </div>
      <?php endif; ?>
        
      <?php if ($right3): ?>
        <div class="composite-zone composite-column composite-column-last">
          <div class="composite-zone-inner">
          <?php print $right3; ?>
          </div>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>

  <?php if ($row4_has_content): ?>
    <div class="composite-zone composite-row composite-row-4 <?php print $row4_classes; ?>">
      <?php if ($left4): ?>
        <div class="composite-zone composite-column composite-column-first">
          <div class="composite-zone-inner">
            <?php print $left4; ?>
          </div>
        </div>
      <?php endif; ?>

      <?php if ($middle4): ?>
        <div class="composite-zone composite-column composite-column-middle">
          <div class="composite-zone-inner">
            <?php print $middle4; ?>
          </div>
        </div>
      <?php endif; ?>
        
      <?php if ($right4): ?>
        <div class="composite-zone composite-column composite-column-last">
          <div class="composite-zone-inner">
          <?php print $right4; ?>
          </div>
        </div>
      <?php endif; ?>
    </div>
  <?php endif; ?>
</div>
