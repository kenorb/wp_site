<?php
// $Id: abuse-report.tpl.php,v 1.1.2.4 2008/10/15 17:50:58 btmash Exp $

/**
 * @file
 * Default theme implementation to moderate one piece of content.
 *
 *  * Available variables:
 * - $object: The content that requires moderation
 * - $account: Account of user that created content
 * - $offences: Total offences by account
 * - $warnings: Total email warnings sent to account
 * - $moderate: Moderation form
 *
 * @see template_preprocess()
 * @see template_preprocess_abuse_report()
 */
?>
<li class="abuse-report corners">
  <div class="summary">
    <h2><?php print l($object->title, $object->path['URL'], array('html' => TRUE, 'query' => $object->path['QUERY'], 'fragment' => $object->path['BREADCRUMB'])); ?></h2>
    <dl>
      <dt><?php print t('Type:'); ?></dt>
      <dd><?php print $object->type === 'comment' ? t('Comment') : t('Movie'); ?></dd>
      <dt><?php print t('Status:'); ?></dt>
      <dd><?php print isset($object->abuse_status_string) ? $object->abuse_status_string : 'N/A'; ?></dd>
      <?php if (variable_get('abuse_assigned_moderators', FALSE)): ?>
        <dt><?php print t('Assigned To:'); ?></dt>
        <dd><?php print $object->abuse_assigned_to_name; ?></dd>
      <?php endif; ?>
    </dl>
    <?php if (!empty($object->description)): ?>
      <p><?php print $object->description; ?></p>
    <?php endif; ?>
    <h4><?php print t('By: !username', array('!username' => theme('username', $account))); ?></h4>
    <dl>
      <dt><?php print t('Email'); ?>:</dt>
      <dd><?php print $account->mail; ?></dd>
      <dt><?php print t('Age'); ?>:</dt>
      <dd><?php print theme('age', $account->uid); ?></dd>
      <dt><?php print t('Offences'); ?>:</dt>
      <dd><?php print $offences; ?></dd>
      <dt><?php print t('Warnings'); ?>:</dt>
      <dd><?php print $warnings; ?></dd>
    </dl>
    <ul class="links">
      <li><?php print l(t('Edit Account'), 'user/'. $account->uid .'/edit'); ?></li>
      <li><?php print l(t('View Account History'), 'admin/abuse/status/user/'. $account->uid); ?></li>
    </ul>
  </div>
  <div class="actions">
    <?php print $moderate; ?>
  </div>
  <div class="details clear-block">
    <div class="column column-first">
      <?php print $object->content; ?>&nbsp;
    </div>
    <div class="column column-last">

      <dl class="history">
        <dt><?php print t('History'); ?></dt>
        <?php if (count($object->history)): ?>
          <?php foreach ($object->history as $log): ?>
          <dd>
            <strong><?php print $log->flagger; ?>:</strong>
            <?php print t('Changed status to %status', array('%status' => $log->readable_status)); ?>
          </dd>
          <?php endforeach; ?>
        <?php else: ?>
          <dd><?php print t('None');?></dd>
        <?php endif; ?>
      </dl>

      <dl class="warnings">
        <dt><?php print t('Warnings'); ?></dt>
        <?php if (count($object->warnings)): ?>
          <?php foreach ($object->warnings as $warning): ?>
          <dd>
            <strong><?php print $warning->name; ?>:</strong>
            <?php print t('sent warning on %date', array('%date' => $warning->date)); ?>
          </dd>
          <?php endforeach; ?>
        <?php else: ?>
          <dd><?php print t('None');?></dd>
        <?php endif; ?>
      </dl>

      <dl class="flags">
        <dt><?php print t('Flags'); ?></dt>
        <?php if (count($object->reports)): ?>
          <?php foreach ($object->reports as $report): ?>
          <dd>
              <strong><?php print strcasecmp($report->name, t('Watchlist')) == 0 ? t('Watchlist') : theme("username", $report); ?></strong> &mdash;
              <?php print format_date($report->created); ?>:<br />
              <?php print check_plain(urldecode($report->body)); ?>
            </dd>
          <?php endforeach; ?>
        <?php else: ?>
          <dd><?php print t('None');?></dd>
        <?php endif; ?>
      </dl>

    </div>
  </div>
</li>