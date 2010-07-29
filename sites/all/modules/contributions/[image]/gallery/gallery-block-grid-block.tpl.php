<?php
// $Id: gallery-block-grid-block.tpl.php,v 1.1.2.1 2008/03/08 16:27:25 kiz0987 Exp $

/**
 * @file gallery-block-grid-block.tpl.php
 * Default theme implementation to display a Gallery grid block.
 *
 * Available variables:
 * - $class: The CSS class for the block.
 * - $images: An array of image HTML organized as rows and columns.
 * - $content: The unprocessed image HTML from Gallery2.
 * - $num_cols: The number of columns.
 *
 * @see template_preprocess_gallery_block_grid_block()
 */
?>
<div class="<?php print $class; ?>">
  <div class="image-grid">
    <table>
      <tbody>
      <?php foreach ($images as $image_row): ?>
        <tr class="<?php print $image_row['zebra']; ?>">
        <?php foreach ($image_row['data'] as $image): ?>
          <td style="text-align:center;">
            <?php print $image; ?>
          </td>
          <?php endforeach; ?> 
        </tr>
      <?php endforeach; ?>
      </tbody>
    </table>
  </div>
</div>