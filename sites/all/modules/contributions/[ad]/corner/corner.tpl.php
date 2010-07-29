<?php
// $Id: corner.tpl.php,v 1.1 2008/12/08 20:58:37 kirie Exp $

/**
 * @file
 * Default theme implementation to present a corner.
 *
 * Note: The encapsulating <div> contains an extra class,  'pngfix' - which can be used with the optional
 * pngfix module (http://drupal.org/project/pngfix) to fix png transparency in IE 5.5 and IE 6.
 * 
 * Available variables:
 * - $html: A themed corner.
 * - $corner: An corner object.
 *
 * The $corner object contains:
 * - $corner->cid: The id of the corner.
 * - $corner->name: The name of the corner.
 * - $corner->image: The name of image used as for corner..
 * - $corner->image_title: The title to be used on the corner image.
 * - $corner->image_uri: The URI used to make the corner into a link..
 * - $corner->status: The status of the corner (0 = disabled, 1 = enabled).
 * - $corner->position: The position of the corner (left or right).
 */
?>

<div id="corner-<?php echo $corner->cid; ?>" class="corner corner-<?php echo $corner->position; ?> pngfix">
  <?php echo $html; ?>
</div>