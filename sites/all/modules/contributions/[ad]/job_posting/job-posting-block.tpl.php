<?php
// $Id: job-posting-block.tpl.php,v 1.1.2.1 2008/05/04 15:14:31 gmarus Exp $

/**
 * @file job-posting-block.tpl.php
 * Default implementation for the display of job posting blocks
 * 
 * @see template_preprocess_job_posting_block_display()
 */
?>

<ul>
<?php foreach ($titles as $nid => $label): ?>
  <li><?php print l($label, 'node/' . $nid); ?></li>
<?php endforeach; ?>
</ul>
<div class="more-link"><?php print $morelink; ?></div>