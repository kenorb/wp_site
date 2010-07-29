<?php
// $Id: window_insert.tpl.php,v 1.1.2.2 2008/12/03 22:18:51 starnox Exp $

/**
 * @file
 * Template for Image Browser's insert window.
 */
?>
<h2>Insert</h2>
<div class="presets">
  <?php print $presets; ?>
  <div class="spacer"></div>
</div>
<div class="details">
  <img src="<?php print $image_url; ?>" alt="Thumbnail" />
  <table>
    <tbody>
      <tr>
        <th><?php print t('Title'); ?></th>
        <td><?php print $title; ?></td>
      </tr>
      <tr>
        <th><?php print t('Dimensions'); ?></th>
        <td><?php print $image_width; ?> x <?php print $image_height; ?> px</td>
      </tr>
      <tr>
        <th><?php print t('Mime type'); ?></th>
        <td><?php print $image_mime_type; ?></td>
      </tr>
      <tr>
        <th><?php print t('Owner'); ?></th>
        <td><?php print $username; ?></td>
      </tr>
    </tbody>
  </table>
  <div class="options">
    <a href="<?php print $edit_link; ?>" class="button edit"><?php print t('Edit'); ?></a>
    <a href="<?php print $delete_link; ?>" class="button delete"><?php print t('Delete'); ?></a>
  </div>
</div>
<div class="footer">
  <a href="#" class="button close"><?php print t('Close'); ?></a>
</div>