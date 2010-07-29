<?php
// $Id: gallery-block-image-block.tpl.php,v 1.1.2.1 2008/03/08 16:27:25 kiz0987 Exp $

/**
 * @file gallery-block-image-block.tpl.php
 * Default theme implementation to display a Gallery image block.
 *
 * Available variables:
 * - $class: The CSS class for the block.
 * - $content: The image HTML from Gallery2.
 *
 * @see template_preprocess_gallery_block_image_block()
 */
?>
<div class="<?php print $class; ?>">
  <?php print $content; ?>
</div>