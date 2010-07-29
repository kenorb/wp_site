<?php
// $Id: ddblock-cycle-block-content-array.tpl.php,v 1.1 2008/12/23 17:00:55 ppblaauw Exp $

/**
 * @file
 * Default theme implementation to display a dynamic display blocks from a content type.
 *
 * Available variables:
 * - $script: jQuery script for the dynamic display block module.
 * - $origin: From which module comes the block.
 * - $delta: Block number of the block.
 * - $pager: Add a pager to the dynamic display block.
 * - $pager_height: Height of the container of the pager.
 * - $pager_width: Width of the container of the pager.
 * - $content_array: Array of content.
 *
 * notes: don't change the ID names, they are used by the jQuery script.
 */

// get module path to dynamic display block module.
//$module_path = drupal_get_path('module', 'ddblock');

//add jcycle plugin.
//drupal_add_js($module_path .'/jquery.cycle.all.pack.js', 'module');
//drupal_add_js($module_path .'/jquery.easing.1.1.1.js', 'module');

// add Cascading style sheet.
//drupal_add_css($module_path .'/ddblock.css', 'module', 'all', FALSE);

// add dynamic display block javascript.
//drupal_add_js($script, 'inline');

?>
<?php //pager. ?>
<?php if (($pager == 'number-pager') || ($pager == 'image-pager') || {$pager == 'custom-pager')): ?>
 <div id="ddblock-<?php print $origin ."-". $pager ."-". $delta?>-nav" class="ddblock-<?php print $pager?>-nav ddblock-pager-nav" style="height: <?php print $pager_height?>px; width: <?php print $pager_width?>px; clear-block">
  <?php if ($pager == 'image-pager'): ?>
   <?php foreach ($content_array as $image_file): ?>
    <li><a href="#"><img src="<?php print base_path() . $image_file; ?>" alt="" width="50" height="50" /></a></li>
   <?php endforeach; ?>
  <?php endif; ?>
 </div>
<?php endif; ?>
<?php if ($pager == 'prev-next'): ?>  
 <div class="nav">
  <a id="prev2" href="#">Prev</a>
  <a id="next2" href="#">Next</a>
 </div>
<?php endif; ?>  

<?php //block content. ?>
<div id="ddblock<?php print "-". $origin ."-". $delta?>" class="ddblock-contents clear-block" style="visibility:hidden">
  <div class="ddblock-content clear-block">
  <?php foreach ($content_array as $content): ?>
      <?php print($content); ?>
  <?php endforeach; ?>
  </div>

</div>
