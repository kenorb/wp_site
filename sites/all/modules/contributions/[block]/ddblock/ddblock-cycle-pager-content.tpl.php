<?php
// $Id: ddblock-cycle-pager-content.tpl.php,v 1.1 2008/12/23 17:01:07 ppblaauw Exp $ 

/**
 * @file
 * Default theme implementation to display a dynamic display blocks from a dynamic display block instance.
 *
 * Available variables:
 * - $delta: Block number of the block.
 * - $pager: Add a pager to the dynamic display block.
 * - $pager_height: Height of the container of the pager.
 * - $pager_width: Width of the container of the pager.
 * - $pager_position: position of the slider (top | bottom) 
  * - $content: themed content
 *
 * notes: don't change the ID names, they are used by the jQuery script.
 */
$number_of_items = 6; 
$number_of_items_per_row=3;
?>

<?php //combo pager. ?>
 
<?php if ($pager == 'custom-pager'): ?>
 <?php if ($pager_position == 'bottom' || $pager_position == 'both'): ?>
   <div class="spacer-horizontal"><b></b></div>
 <?php endif; ?>
 <div id="ddblock-pager-<?php print $delta ?>" class="<?php print $pager ?>" class="clear-block border">
  <div  class="<?php print $pager ?>-inner" class="clear-block border">
   <?php if ($pager_items): ?>
    <?php $item_counter=0; ?>
    <?php foreach ($pager_items as $pager_item): ?>
     <div class="<?php print $pager ?>-item <?php print $pager ?>-item-<?php print $item_counter ?>">
<div class="corners"> 
      <div class="<?php print $pager ?>-item-inner"> 
       <a href="#" title="click to navigate to topic"><img src="<?php print base_path() . $pager_item['image']; ?>"  alt="testing"/><?php print $pager_item['text']; ?> </a>
      </div>
 <div class="tl"></div>
 <div class="tr"></div>
 <div class="bl"></div>
 <div class="br"></div>
</div>
     </div>
     <?php $item_counter++; if ($item_counter == $number_of_items_per_row):?>
      <div class="spacer-horizontal"><b></b></div>
     <?php else: ?>
      <div class="spacer-vertical"></div>
     <?php endif; ?>
    <?php endforeach; ?>
   <?php endif; ?>
  </div> <!-- pager-inner-->
 </div>  <!-- pager-->
 <?php if ($pager_position == 'top' || $pager_position == 'both'): ?>
   <div class="spacer-horizontal"><b></b></div>
 <?php endif; ?>
<?php endif; ?> 
