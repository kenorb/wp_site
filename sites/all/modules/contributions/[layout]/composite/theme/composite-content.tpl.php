<?php
// $Id: composite-content.tpl.php,v 1.1 2008/10/14 01:03:56 bengtan Exp $

/**
 * @file composite-content.tpl.php
 * Default composite content template.
 *
 * This template should never actually be used. It's just in case something goes wrong. 
 *
 * Variables:
 * - $body: Original node content (from $node->body)
 * - $layout: Layout definition (from $node->composite_layout)
 *
 * For each zone defined in the layout, there will be a corresponding variable containing
 *   the content to be displayed in that zone ie. $left, $right
 *
 * @see template_preprocess_composite_content
 */
?>
<div class="composite-content">
<?php print $content; ?>
</div>