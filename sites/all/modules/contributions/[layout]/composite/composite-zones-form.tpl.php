<?php
// $Id: composite-zones-form.tpl.php,v 1.1.2.1 2008/11/19 15:10:07 bengtan Exp $

/**
 * This file is loosely derived from block-admin-display-form.tpl.php.
 * It still uses the same css classes.
 * Hence you will see the terms block and reference used interchangeably.
 *   You will also see region and zone used interchangeably. 
 */

/**
 * @file composite-layout-form.tpl.php
 * Default theme implementation to configure layout.
 *
 * Available variables:
 * - $layout_zones: An array of zones. Keyed by name with the title as value.
 * - $zone_listing: An array of blocks keyed by region and then delta.
 * - $form_submit: Form submit button and other required form fields. 
 *
 * Each $zone_listing[$zone] contains an array of items for that region.
 *
 * Each $data in $zone_listing[$zone] contains:
 * - $data->reference_title: Item title.
 * - $data->zone_select: Drop-down menu for assigning a zone.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->configure_link: Configuration link.
 * - $data->delete_link: For deleting user added items ??
 *
 * @see template_preprocess_composite_layout_form()
 */
?>
<?php print $form_layout_settings; ?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'composite') .'/composite.js');
  foreach ($layout_zones as $region => $title) {
    drupal_add_tabledrag('blocks', 'match', 'sibling', 'block-region-select', 'block-region-'. $region, NULL, FALSE);
    drupal_add_tabledrag('blocks', 'order', 'sibling', 'block-weight', 'block-weight-'. $region);
  }
?>
<table id="blocks" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Item'); ?></th>
      <th><?php print t('Zone'); ?></th>
      <th><?php print t('Weight'); ?></th>
      <th><?php print t('Additional settings'); ?></th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($layout_zones as $zone => $title): ?>
      <tr class="region region-<?php print $zone?>">
        <td colspan="5" class="region"><?php print $title; ?></td>
      </tr>
      <tr class="region-message region-<?php print $zone?>-message <?php print empty($zone_listing[$zone]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="5"><em><?php print t('No items in this zone'); ?></em></td>
      </tr>
      <?php foreach ($zone_listing[$zone] as $delta => $data): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?><?php print $data->row_class ? ' '. $data->row_class : ''; ?>">
        <td class="block"><?php print $data->reference_title; ?></td>
        <td><?php print $data->zone_select; ?></td>
        <td><?php print $data->weight_select; ?></td>
        <td><?php print $data->data_widget; ?></td>
        <td><?php print $data->checkbox; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_submit; ?>
