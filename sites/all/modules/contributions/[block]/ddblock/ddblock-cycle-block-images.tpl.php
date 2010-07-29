<?php

// $Id: ddblock-cycle-block-images.tpl.php,v 1.2 2008/12/23 18:14:32 ppblaauw Exp $ 

/**
 * @file
 * Default theme implementation to display image blocks.
 *
 * Available variables:
 * - $script: jQuery script for the dynamic display block module.
 * - $origin: From which module comes the block.
 * - $delta: Block number of the block.
 * - $pager: Add a pager to the dynamic display block.
 * - $pager_height: Height of the container of the pager.
 * - $pager_width: Width of the container of the pager.
 * - $image_array: Array of images.
 *
 * notes: don't change the ID names, they are used by the jQuery script
 */
?>
<?php //pager ?>
<?php if ($pager <> 'none'): ?>
  <div id="ddblock-<?php print $origin ."-". $pager ."-". $delta?>-nav" class="ddblock-<?php print $pager?>-nav ddblock-pager-nav clear-block" style="height: <?php print $pager_height?>px; width: <?php print $pager_width?>px;" >
<?php if ($pager == 'image-pager'): ?>
  <?php foreach ($content_array as $image_file): ?>
  <li><a href="#"><img src="<?php print base_path() . $image_file; ?>" alt="" width="50" height="50" /></a></li>
   <?php endforeach; ?> 
  <?php endif; ?>
  <?php if ($pager == 'prev-next'): ?>  
   <div class="nav">
    <a id="prev2" href="#">Prev</a>
    <a id="next2" href="#">Next</a>
   </div>
  <?php endif; ?>  
 </div>
<?php endif; ?>

<?php //block content ?>
<div id="ddblock<?php print "-". $origin ."-". $delta?>" class="ddblock-contents clear-block" style="visibility:hidden">
<div class="ddblock-content clear-block">
<p>
<?php foreach ($content_array as $image_file): ?>
  <img src="<?php print base_path() . $image_file; ?>" alt="" width="<?php print $image_width;?>px" height="<?php print $image_height; ?>px" />
<?php endforeach; ?>  
</p>
</div>
</div>


