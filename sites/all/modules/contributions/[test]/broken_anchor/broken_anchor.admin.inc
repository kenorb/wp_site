<?php

/* This file is part of "Broken Anchor for Node comments Module".
 *    Copyright 2009, arNuméral
 *    Author : Yoran Brault
 *    eMail  : yoran.brault@bad_arnumeral.fr (remove bad_ before sending an email)
 *    Site   : http://www.arnumeral.fr/node/2
 *
 * "Broken Anchor for Node comments Module" is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * "Broken Anchor for Node comments Module" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with "Broken Anchor for Node comments Module"; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */


/**
 * configuration form.
 *
 * @return form
 */
function broken_anchor_admin_settings() {
  $form=array();
  $form['main'] = array (
    '#type' => 'fieldset',
    '#title' => t('Main settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  foreach (module_implements('broken_anchor_info') as $module) {
    $info=module_invoke($module, 'broken_anchor_info');
    $form['main']['broken_anchor_'.$module.'_job_size'] = array (
      '#type' => 'textfield', 
      '#title' => t("Job size for @type", array('@type'=>$info['description'])), 
      '#size' => 5, 
      '#default_value' => broken_anchor_job_size($module), 
      '#description' => t("Number of rows to process each time when checking @type (0 means disabled).", array('@type'=>$info['description']))
    );
  }

  $form['emails'] = array (
    '#type' => 'fieldset',
    '#title' => t('Emails settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['emails']['broken_anchor_check_emails'] = array (
      '#type' => 'checkbox', 
      '#title' => t("Check Emails"), 
      '#default_value' => broken_anchor_check_emails(), 
      '#description' => t("Also check clear emails addresses in content.")
  );

  $form['main']['broken_anchor_report_page_size'] = array (
      '#type' => 'textfield',
    '#size'=>5, 
      '#title' => t("Report page size"), 
      '#default_value' => broken_anchor_report_page_size(), 
      '#description' => t("Number of rows to display in the report.")
  );
  $form['advanced'] = array (
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  foreach (module_implements('broken_anchor_info') as $module) {
    $info=module_invoke($module, 'broken_anchor_info');
    $form['advanced']['broken_anchor_last_check_id_'.$module] = array (
      '#type' => 'textfield', 
      '#title' => t("Starting ID for @type", array("@type"=>$info['description'])), 
      '#size' => 5, 
      '#default_value' => broken_anchor_last_check_id($module), 
      '#description' => t("ID from which to start next time for @type", array("@type"=>$info['description']))
    );
  }
  $form['advanced']['broken_anchor_user_agent'] = array (
      '#type' => 'textarea', 
      '#title' => t("User Agent"), 
      '#default_value' => broken_anchor_user_agent(), 
      '#description' => t("User Agent to use when checking a link")
  );
  $form['advanced']['broken_anchor_timeout'] = array (
      '#type' => 'textfield', 
      '#title' => t("Timeout"), 
      '#size' => 5, 
      '#default_value' => broken_anchor_timeout(), 
      '#description' => t("Number of seconds to wait before declaring timeout error.")
  );
  $form['broken_anchor_debugging'] = array (
    '#type' => 'fieldset',
    '#title' => t('Debugging'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );

  $form['broken_anchor_debugging']['broken_anchor_debug_level'] = array (
        '#title' => t("Debug level"),
        '#description' => t("Set the debugging output level. Every logging will use php error_log function. On UNIX system, this will end up in apache error logs."),
        '#type' => 'select',
        '#default_value' => broken_anchor_debug_level(), 
        '#options' => array (
  0 => t('Disabled'),
  1 => t('Enabled'),
  2 => t('Verbose'),
  3 => t('Full'),
  )
  );
  return system_settings_form($form);
}


