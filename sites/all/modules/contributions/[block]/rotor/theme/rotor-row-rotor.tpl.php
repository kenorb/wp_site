<?php
// $Id: rotor-row-rotor.tpl.php,v 1.1.2.1 2008/12/20 17:53:55 mrfelton Exp $
/**
 * @file views-view-unformatted.tpl.php
 * Default simple view template to display a list of rows.
 *
 * @ingroup views_templates
 */
?>
<?php $node = node_load($row->nid); ?>
<?php print theme('rotor_item', $node); ?>
