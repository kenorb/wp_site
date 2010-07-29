<?php
// $Id: corner.admin.inc,v 1.1 2008/12/08 20:58:37 kirie Exp $

/**
 * @file
 * Admin page callbacks for the corner module.
 */

/**
 * Menu callback for admin/build/corner.
 */
function corner_admin_display() {
  
  $path = drupal_get_path('module', 'corner');
  drupal_add_css($path .'/corner-admin.css', 'module', 'all', FALSE);
    
  $result = db_query("SELECT * FROM {corners} ORDER BY cid");
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $rows[] = array(
      'data' => array(
        check_plain($row->name),
        check_plain($row->image),
        $row->position,
        !$row->status ? l(t('enable'), 'admin/build/corner/'. $row->cid .'/enable') : l(t('disable'), 'admin/build/corner/'. $row->cid .'/disable'),
        l(t('clone'), 'admin/build/corner/'. $row->cid .'/clone'),
        l(t('configure'), 'admin/build/corner/'. $row->cid .'/configure'),
        l(t('delete'), 'admin/build/corner/'. $row->cid .'/delete'),
      ),
      'class' => $row->status ? 'enabled' : 'disabled',
    );
  }

  $header = array(t('Corner'), t('Image'), t('Position'), array('data' => t('Operations'), 'colspan' => '4'));

  return theme('table', $header, $rows, $attributes = array('class' => 'corner-admin-list'), NULL);
}

/**
 * Menu callback; displays the corner configuration form.
 *
 * Using the same form for three different operations ($op):
 * - 'configure': configure an existing corner (default)
 * - 'add': add a new corner
 * - 'clone': clone an existing corner
 */
function corner_admin_configure($form_state, $corner, $op = 'configure') {

  $form = array();
  $form['cid'] = array('#type' => 'value', '#value' => ($op == 'configure') ? $corner->cid : 0);

  if (!$corner->cid) {
    $corner = corner_get_default();
  }

  $images = corner_get_images();
  $image_options = array('0' => t('--- select ---'));
  foreach ($images as $filename => $data) {
    $image_options[$data->basename] = $data->basename;
  }  

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Corner info'),
    '#collapsible' => TRUE,
  );
  $form['info']['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable corner'),
    '#default_value' => $corner->status,
    '#description' => t('If this setting is enabled, the current corner is shown on the configured pages (see specific options below).'),
  );
  $form['info']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner name'),
    '#default_value' => $corner->name,
    '#description' => t('The name of the corner.'),
    '#required' => TRUE,
    '#maxlength' => 64,
  );

  $form['details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Corner details'),
    '#collapsible' => TRUE,
  );
  $form['details']['image'] = array(
    '#type' => 'select',
    '#title' => t('Corner image'),
    '#options' => $image_options,
    '#default_value' => $corner->image,
    '#description' => t('Select which corner to show. You can add additional images (PNG-24) to the %images directory inside the module.', array('%images' => EDGEBANNER_PATH_IMAGES)),
    '#required' => TRUE,
  );
  $form['details']['position'] = array(
    '#type' => 'select',
    '#title' => t('Corner position'),
    '#options' => array('0' => t('--- select ---'), 'left' => t('Left'), 'right' => t('Right')),
    '#default_value' => $corner->position,
    '#description' => t('The position of the corner.'),
    '#required' => TRUE,
  );
  $form['details']['image_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner title'),
    '#default_value' => $corner->image_title,
    '#description' => t('If you enter a title, the corner will use it in its title propery. Leave empty for none.'),
    '#maxlength' => 64,
  );
  $form['details']['image_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Corner link URI'),
    '#default_value' => $corner->image_uri,
    '#description' => t('If you enter a URI, the corner will be made clickable. Use &lt;front&gt; for the front page of the site. Leave empty to disable making the corner into a link.'),
    '#maxlength' => 255,
  );
  
  // Role-based visibility settings (mostly borrowed from 'block.admin.inc')
  $result = db_query('SELECT rid, name FROM {role} ORDER BY name');
  $role_options = array();
  while ($role = db_fetch_object($result)) {
    $role_options[$role->rid] = $role->name;
  }
  $form['roles_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Role specific visibility settings'),
    '#collapsible' => TRUE,
  );
  $form['roles_vis_settings']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show corner for specific roles'),
    '#options' => $role_options,
    '#default_value' => explode(',', $corner->roles),
    '#description' => t('Show the corner only for the selected role(s). If you select no roles, the corner will be visible to all users.'),
  );

  $form['page_vis_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page specific visibility settings'),
    '#collapsible' => TRUE,
  );
  $access = user_access('use PHP for corner visibility');

  if ($corner->visibility == 2 && !$access) {
    $form['page_vis_settings'] = array();
    $form['page_vis_settings']['visibility'] = array('#type' => 'value', '#value' => 2);
    $form['page_vis_settings']['pages'] = array('#type' => 'value', '#value' => $corner->pages);
  }
  else {
    $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
    $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

    if ($access) {
      $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
      $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
    }
    $form['page_vis_settings']['visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show corner on specific pages'),
      '#options' => $options,
      '#default_value' => $corner->visibility,
    );
    $form['page_vis_settings']['pages'] = array(
      '#type' => 'textarea',
      '#title' => t('Pages'),
      '#default_value' => $corner->pages,
      '#description' => $description,
    );
  }

  if ($op == 'configure') {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );
    $form['cancel'] = array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
      '#validate' => array('corner_admin_cancel_validate'),
      '#submit' => array('corner_admin_cancel_submit'),
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    );
  }
  else if ($op == 'add' || $op == 'clone') {
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#submit' => array('corner_admin_add_submit'),
    );
  }

  return $form;
}

function corner_admin_configure_validate($form, &$form_state) {
  if (empty($form_state['values']['image'])) {
    form_set_error('image', t('!field field is required.', array('!field' => $form['details']['image']['#title'])));
  }
  if (empty($form_state['values']['position'])) {
    form_set_error('position', t('!field field is required.', array('!field' => $form['details']['position']['#title'])));
  }
}

function corner_admin_configure_submit($form, &$form_state) {

  // Save the edited corner
  if (!form_get_errors() && $form_state['clicked_button']['#value'] == t('Save')) {
    $roles = implode(',', $form_state['values']['roles']);

    $sql = "UPDATE {corners} SET";
    $sql .= " name = '%s', image = '%s', image_title = '%s', image_uri = '%s', status = '%d', position = '%s', roles = '%s', visibility = '%d', pages = '%s'";
    $sql .= " WHERE cid = %d";
    db_query($sql, 
      $form_state['values']['name'],
      $form_state['values']['image'],
      $form_state['values']['image_title'],
      $form_state['values']['image_uri'],
      $form_state['values']['status'], 
      $form_state['values']['position'],
      $roles,
      $form_state['values']['visibility'],
      $form_state['values']['pages'],
      $form_state['values']['cid']
    );
    drupal_set_message(t('The corner %name has been saved.', array('%name' => $form_state['values']['name'])));
    $form_state['redirect'] = 'admin/build/corner';
  }
  else if ($form_state['clicked_button']['#value'] == t('Delete')) {
    $form_state['redirect'] = 'admin/build/corner/'. $form_state['values']['cid'] .'/delete';
  }

  return;
}

/**
 * Save the new corner.
 */
function corner_admin_add_submit($form, &$form_state) {

  $roles = implode(',', $form_state['values']['roles']);
  
  $sql = "INSERT INTO {corners} (name, image, image_title, image_uri, status, position, roles, visibility, pages)";
  $sql .= " VALUES ('%s', '%s', '%s', '%s', %d, '%s', '%s', %d, '%s')";

  db_query($sql, 
    $form_state['values']['name'],
    $form_state['values']['image'],
    $form_state['values']['image_title'],
    $form_state['values']['image_uri'],
    $form_state['values']['status'], 
    $form_state['values']['position'],
    $roles,
    $form_state['values']['visibility'],
    $form_state['values']['pages']
  );
  drupal_set_message(t('The corner %name has been created.', array('%name' => $form_state['values']['name'])));
  $form_state['redirect'] = 'admin/build/corner';
  return;
}

/**
 * Menu callback: disable a specific corner.
 */
function corner_admin_disable($corner) {
  db_query("UPDATE {corners} SET status = 0 WHERE cid = %d", $corner->cid);
  drupal_goto('admin/build/corner');
}

/**
 * Menu callback: enable a specific corner.
 */
function corner_admin_enable($corner) {
  db_query("UPDATE {corners} SET status = 1 WHERE cid = %d", $corner->cid);
  drupal_goto('admin/build/corner');
}

function corner_admin_cancel_validate($form, &$form_state) {
}

function corner_admin_cancel_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/corner';
  return;
}

/**
 * Menu callback; confirm deletion of a corner.
 */
function corner_admin_delete($form_state, $corner) {
  $form['corner'] = array('#type' => 'hidden', '#value' => serialize($corner));
  $params = array('%name' => $corner->name);
  return confirm_form($form, t('Are you sure you want to delete the corner %name?', $params), 'admin/build/corner', '', t('Delete'), t('Cancel'));
}

function corner_admin_delete_submit($form, &$form_state) {
  $corner = unserialize($form_state['values']['corner']);
  db_query('DELETE FROM {corners} WHERE cid = %d', $corner->cid);
  drupal_set_message(t('The corner %name has been removed.', array('%name' => $corner->name)));
  $form_state['redirect'] = 'admin/build/corner';
  return;
}
