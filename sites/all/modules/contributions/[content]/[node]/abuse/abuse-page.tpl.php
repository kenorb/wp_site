<?php
// $Id: abuse-page.tpl.php,v 1.1.2.3 2008/10/15 17:50:58 btmash Exp $

/**
 * @file
 * Default theme implementation to show content that requires moderation
 *
 *  * Available variables:
 * - $reports: An array of reports
 *
 * @see template_preprocess()
 * @see template_preprocess_abuse_page()
 */
?>
<div id="message-wrapper" class="message status"></div>
<ul class="abuse-report-list">
  <?php foreach ($reports as $report): ?>
  <?php print theme('abuse_report', $report); ?>
  <?php endforeach; ?>
</ul>

<?php // hide the controls for the main abuse page on the individual ticket page ?>
<?php if (!preg_match('/^admin\/abuse\/status/i', $_REQUEST['q'])): ?>
<div id="abuse-report-list-controls">
	<?php print l(t('Get More Tickets'), $_REQUEST['q']); ?>
</div>
<?php endif; ?>

<?php print theme('pager', array(), $limit, 0); ?>
