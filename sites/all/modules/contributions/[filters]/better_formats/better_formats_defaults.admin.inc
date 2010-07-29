<?php
// $Id: better_formats_defaults.admin.inc,v 1.3 2008/12/15 22:02:08 dragonwize Exp $

/**
 * @file
 * Contains FAPI and theme functions for the format defaults form.
 */
 
 
/**
 * Builds the form for the filters admin.
 * 
 * @return array 
 *  FAPI array
 * 
 * @see better_formats_defaults_admin_form_validate()
 * @see better_formats_defaults_admin_form_submit()
 */
function better_formats_defaults_admin_form() {
  $form = array(
    '#tree' => TRUE,
  );
    
  $nform = better_formats_get_role_default_fields('node');
  $cform = better_formats_get_role_default_fields('comment');
  $form  = array_merge($form, $nform, $cform);
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save defaults'),
  );
  
  return $form;
}

/**
 * Validates better_formats_admin_filter_form.
 * 
 * @see better_formats_defaults_admin_form()
 * @see better_formats_defaults_admin_form_submit()
 */
function better_formats_defaults_admin_form_validate($form, &$form_state) {
  $formats = filter_formats();
  foreach ($formats as $fid => $format) {
    $roles[$fid] = explode(',', $format->roles);
  }
  
  foreach ($form_state['values'] as $key => $values) {
    if (strpos($key, 'node-') === 0 || strpos($key, 'comment-') === 0) {
      list($type, $rid) = explode('-', $key);
      $fid = $values['format'];
      if ($fid != 0 && ! in_array($rid, $roles[$fid])) {
        form_set_error($key, t('Role does not have access to selected format.'));
      }
    }
  }
}

/**
 * Updates database from better_formats_admin_filter_form.
 * 
 * @see better_formats_defaults_admin_form()
 * @see better_formats_defaults_admin_form_validate()
 */
function better_formats_defaults_admin_form_submit($form, &$form_state) {
  // update DB
  $sql = "UPDATE {better_formats_defaults} 
          SET format=%d, weight=%d 
          WHERE rid=%d AND type='%s'";
  
  foreach ($form_state['values'] as $key => $values) {
    if (strpos($key, 'node-') === 0 || strpos($key, 'comment-') === 0) {
      list($type, $rid) = explode('-', $key);
      db_query($sql, $values['format'], $values['weight'], $rid, $type);
    }
  }
    
  drupal_set_message(t('Defaults have been saved.'));
}

/**
 * Process variables for better-defaults-admin-form.tpl.php.
 *
 * @param array $variables
 *  The $variables array contains the following arguments:
 *    - $form
 */
function template_preprocess_better_formats_defaults_admin_form(&$variables) {
  foreach (element_children($variables['form']) as $key) {
    $form_row =& $variables['form'][$key];
    
    $type = strpos($key, 'node-') === 0 ? 'node' : 'comment';
    
    if (isset($form_row['role'])) {
      // Set special classes needed for table drag and drop.
      $form_row['weight']['#attributes']['class'] = 'better-formats-role-' . $type . '-weight';

      $row = new stdClass();
      $row->role = drupal_render($form_row['role']);
      $row->format_select = drupal_render($form_row['format']);
      $row->weight_select = drupal_render($form_row['weight']);
      
      $variables[$type . '_default_rows'][$key] = $row;
    }
  }
  
  $variables['form_submit'] = drupal_render($variables['form']);
}
