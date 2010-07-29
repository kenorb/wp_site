<?php
// $Id: rotor-view-rotor.tpl.php,v 1.1.2.2 2008/12/17 08:35:25 mrfelton Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php if ($rows): ?>
  <?php _rotor_doheader(); ?>
  <?php
    $settings = array('RotorBanner' => array());
    $settings['RotorBanner'][$id]['id'] = $name;
    $settings['RotorBanner'][$id]['effect'] = $options['effect'];
    $settings['RotorBanner'][$id]['time'] = $options['seconds'];
    $settings['RotorBanner'][$id]['speed'] = $options['speed'] == 0? 1 : $options['speed'] * 1000;
    $settings['RotorBanner'][$id]['pause'] = $options['pause'];
    drupal_add_js( $settings, 'setting' );
  ?>
  <div class='rotor' style="width:<?php print $options['width']; ?>px;">
    <div class='rotor-items' style="width:<?php print $options['width']; ?>px; height:<?php print $options['height']; ?>px;">
        <?php foreach ($rows as $id => $row): ?>
        <div class="rotor-content">
          <div class="rotor-content-detail">
            <?php print $row; ?>
          </div>
        </div>
      <?php endforeach; ?>
    </div>
  </div>
<?php endif; ?>