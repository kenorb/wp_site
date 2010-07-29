<?php
// $Id: job-posting-node.tpl.php,v 1.1.2.1 2008/05/04 15:14:31 gmarus Exp $

/**
 * @file job-posting-node.tpl.php
 * Default implementation for the display of job posting nodes
 * 
 * Available variables:
 * $page: boolean indicating whether the node is in 'page view'
 * $reference: string concatenated from adminstrator chosen prefix + node id
 * $location: string describing geographical location of posted job
 * $employer: string describing sponsoring organization
 * $contact: string describing contact person at sponsoring organization
 * $url: string url denoting sponsoring organization's website location
 * $description: string denoting additional details of the posted job
 * $deadline: associative array denoting job posting deadline date and time
 * $expired: boolean indicating whether a given job posting has expired
 *  
 * @see template_preprocess_job_posting_node_display()
 */
?>

<div class="job-posting-reference">
  <span class="job-posting-label"><?php print t('Reference: '); ?></span>
  <?php print $reference; ?>
</div>
    
<?php if (!empty($location)): ?>
  <div class="job-posting-location">
    <span class="job-posting-label"><?php print t('Location: '); ?></span>
    <?php print $location; ?>
  </div>
<?php endif; ?>
    
<?php if (!empty($employer)): ?>
  <div class="job-posting-employer">
    <span class="job-posting-label"><?php print t('Employer: '); ?></span>
    <?php print $employer; ?>
  </div>
<?php endif; ?>

<?php if (!empty($deadline)): ?>
  <div class="job-posting-deadline">
    <span class="job-posting-label"><?php print t('Application deadline: '); ?></span>
    <?php print $deadline; ?>
  </div>
<?php endif; ?>
    
<?php if ($page): ?>
  <?php if (!empty($contact)): ?>
    <div class="job-posting-contact">
      <span class="job-posting-label"><?php print t('Contact: '); ?></span>
      <?php print $contact; ?>
    </div>
  <?php endif; ?>
    
  <?php if (!empty($url)): ?>
    <div class="job-posting-url">
      <span class="job-posting-label"><?php print t('Url: '); ?></span>
      <?php print $url; ?>
    </div>
  <?php endif; ?>
  
  <div class="job-posting-description">
    <?php print $description; ?>
  </div>
  
  <?php if (!empty($joblink)): ?>
    <div class="job-posting-link">
      <?php print $joblink; ?>
    </div>
  <?php endif; ?>
<?php endif; ?>

