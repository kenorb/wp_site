<?php
// $Id: galleria.tpl.php,v 1.1.2.1 2008/12/12 14:40:28 marktheunissen Exp $

/**
 * @file
 * Default output for a galleria node.
 */
?>
<div id="galleria-content">
  <?php print $gallery; ?>
  <div class="galleria-nav">
    <?php print $next_prev_links; ?>
  </div>
</div>
