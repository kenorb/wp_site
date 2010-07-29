<?php
// $Id: admin_links.admin.inc,v 1.1.2.5 2008/11/28 23:00:38 davereid Exp $

/**
 * @file
 * Admin page callbacks for the admin_links module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function admin_links_settings_form() {
  $form['admin_links_edit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add edit link to node lists.'),
    '#default_value' => admin_links_var('admin_links_edit'),
  );
  $form['admin_links_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add delete link to node lists.'),
    '#default_value' => admin_links_var('admin_links_delete'),
  );
  $form['admin_links_universaledit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add support for the <a href="@ueb">Universal Edit Button</a> on editable content.', array('@ueb' => 'http://universaleditbutton.org/')),
    '#default_value' => admin_links_var('admin_links_universaledit'),
  );

  return system_settings_form($form);
}
