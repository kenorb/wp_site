<?php
// $Id:

/**
 * @file
 * Admin page callbacks for the markitup module.
 */

/**
 * Menu callback; Displays a list of all markItUp editor configurations and
 * which one is the default.
 *
 * @ingroup forms
 * @see markitup_admin_overview_submit()
 */
function markitup_admin_overview() {
// we want to show a list of editor configurations with edit and delete links
// (going to the callback/path for markitup_admin_editor_page and
// markitup_admin_editor_delete)
  $output = t('Configured editors');
  $output .= markitup_admin_overview_editors();
// we also want to show a form for associating an editor configuration with an
// input format using markitup_admin_overview
  $output .= t('Input format and editor associations');
  $output .= drupal_get_form('markitup_admin_overview_form');
  
  return $output;
}

/**
 * Provide an overview form of editor configurations with links to edit and
 * delete them.
 */
function markitup_admin_overview_editors() {
  
  $output = '';
  // List out our editors using markitup_editors() and provide EDIT and DELETE links 
  $editors = markitup_editors();

  // Table header,
  $header = array( t('Editor name'), t('Description'),  array('data' => t('Operations'), 'colspan' => '2'));
  
  // Table rows.
  foreach ($editors as $editor) {
    $row[] = array($editor['name'], $editor['description'], l('configure', 'admin/settings/markitup/'. $editor['meid'] .'/edit'), l('delete', 'admin/settings/markitup/delete/'. $editor['meid']));
  }
 
  // Invoke theme_table().
  $output .= theme('table', $header, $row);
  return $output;
}

/**
 * Provide a form allowing us to associate editor configurations with input
 * formats.
 */
function markitup_admin_overview_form() {

  // Retrieve an array of editor and format associations, for setting default
  // values.
  $markitup_editors_formats = markitup_editors_formats();

  // We want a dropdown box of our markitup editors using markitup_editors().
  $editors = markitup_editors();
  $editor_options[0] = '- none -';
  foreach ($editors as $k => $v) {
    $editor_options[$v['meid']] = $v['name'];
  }

  // Let's do an entry for each input format.
  $input_formats = filter_formats($index = NULL);

  $default_format = variable_get('filter_default_format', 1);

  foreach ($input_formats as $input_format) {

    $form[$input_format->format .'_format'] = array('#type' => 'hidden', '#value' => $input_format->format);
  
    // Mark the default input format.
    $input_format_name = ($default_format == $input_format->format) ? $input_format->name .' (default)' : $input_format->name;

    $form[$input_format->format .'_editor'] = array('#title' => $input_format_name, '#type' => 'select', '#options' => $editor_options, '#default_value' => $markitup_editors_formats[$input_format->format]['meid']);
  }

  $form['markitup_include'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#description' => t('List each path where markItUp should be enabled. Put each path on a new line. You can use wildcards (*). (Leave blank for every form)'),
    '#default_value' => variable_get('markitup_include', ''),
    '#rows' => 5,
    '#cols' => 60);
  $form['markitup_ids'] = array(
    '#type' => 'textarea',
    '#title' => t('IDs'),
    '#description' => t('List each textarea where you want a markItUp editor, write the ID of each textarea on a seperate line. (If empty the editor will only replace the body textarea).'),
    '#default_value' => variable_get('markitup_ids', ''),
    '#rows' => 5,
    '#cols' => 20);

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

// markitup_admin_overview_form_validate($form, &$form_state) ?

/**
 * Submit the markitup_admin_overview_form().
 */
function markitup_admin_overview_form_submit($form, &$form_state) {
  // We'll use the list of input formats when reconstructing your $form_state array and iterating through it.
  $input_formats = filter_formats($index = NULL);

  // Set variable values for paths and textarea ids.
  variable_set('markitup_include', $form_state['values']['markitup_include']);
  variable_set('markitup_ids', $form_state['values']['markitup_ids']);
  
  foreach ($input_formats as $input_format) {
    // Drop any existing records for this format before adding our updated pairing.
    db_query("DELETE FROM {markitup_editors_formats} WHERE format = %d", $form_state['values'][$input_format->format .'_format']);
    db_query("INSERT INTO {markitup_editors_formats} (meid, format) VALUES (%d, %d)", $form_state['values'][$input_format->format .'_editor'], $form_state['values'][$input_format->format .'_format']);
  }
  drupal_set_message(t("Input format and editor associations have been saved."));
}

// ? function theme_markitup_admin_overview($form)




/**
 * Menu callback; Display a markItUp editor configruation form.
 */
function markitup_editor_page($editor = NULL) {
  if (!isset($editor)) {
    drupal_set_title(t('Add markItUp Editor'));
    $editor = array('meid' => '', 'name' => '', 'miu_set' => '', 'skin' => '', 'description' => '');
  }

  return drupal_get_form('markitup_editor_form', $editor);
}


/**
 * Generate a markitup editor configuration add/edit form.
 *
 * @ingroup forms
 * @see markitup_editor_form_validate()
 * @see markitup_editor_form_submit()
 */
function markitup_editor_form(&$form_state, $editor) {
  $form['markitup_name'] = array('#type' => 'textfield', '#title' => t('Editor name'), '#default_value' => $editor['name'], '#required' => TRUE);
  $form['markitup_description'] = array('#type' => 'textfield', '#title' => t('Description'), '#default_value' => $editor['description']);
  // get our skin options
  $skins = _markitup_dirs('skins');
  $form['markitup_skins'] = array('#type' => 'select', '#title' => t('Skins'), '#options' => $skins, '#default_value' => $editor['skin']);
  // get our set options
  $sets = _markitup_dirs('sets');
  $form['markitup_sets'] = array('#type' => 'select', '#title' => t('Button Sets'), '#options' => $sets, '#default_value' => $editor['miu_set']);

  if (!empty($editor['meid'])) {
    $form['meid'] = array('#type' => 'hidden', '#value' => $editor['meid']);
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Validate the markitup editor add/edit form.
 */
//function markitup_editor_form_validate($form, &$form_state) {
//print_r($form_state);
//  if ($form_state['values']['markitup_name'] == '') {
//    form_set_error('', t('You must enter a name for this editor.'));
//  }
//}

/**
 * Submit the markitup editor add/edit form.
 */
function markitup_editor_form_submit($form, &$form_state) {
//die(print_r('<pre>'. var_export($form_state,TRUE) .'</pre>'));

  $meid = isset($form_state['values']['meid']) ? $form_state['values']['meid'] : NULL;

  if (!isset($meid)) {
    db_query("INSERT INTO {markitup_editors} (name, description, skin, miu_set) VALUES ('%s', '%s', '%s', '%s')", $form_state['values']['markitup_name'], $form_state['values']['markitup_description'], $form_state['values']['markitup_skins'], $form_state['values']['markitup_sets']);
    drupal_set_message(t("The '%markitup_name' markItUp editor was saved.", array('%markitup_name' => $form_state['values']['markitup_name'])));
  }
  else {
    db_query("UPDATE {markitup_editors} SET name = '%s', description = '%s', skin = '%s', miu_set = '%s' WHERE meid = %d", $form_state['values']['markitup_name'], $form_state['values']['markitup_description'], $form_state['values']['markitup_skins'], $form_state['values']['markitup_sets'], $form_state['values']['meid']);
    drupal_set_message(t("The '%markitup_name' markItUp editor was updated.", array('%markitup_name' => $form_state['values']['markitup_name'])));
  }

  $form_state['redirect'] = 'admin/settings/markitup';
}




/**
 * Menu callback; confirm deletion of a markItUp editor.
 *
 * @ingroup forms
 * @see markitup_editor_delete_submit()
 */
function markitup_editor_delete() {
  $meid = arg(4);
  $editor = db_fetch_object(db_query('SELECT * FROM {markitup_editors} WHERE meid = %d', $meid));

  if ($editor) {
    if ($editor->meid != variable_get('markitup_editor_default', 1)) {
      $form['meid'] = array('#type' => 'hidden', '#value' => $editor->meid);
      $form['name'] = array('#type' => 'hidden', '#value' => $editor->name);

      return confirm_form($form, t('Are you sure you want to delete the %markitup_name editor?', array('%markitup_name' => $editor->name)), 'admin/settings/filters', t('This action cannot be undone.'), t('Delete'), t('Cancel'));
    }
    else {
      drupal_set_message(t('The default editor cannot be deleted.'));
      drupal_goto('admin/settings/markitup');
    }
  }
  else {
    drupal_not_found();
  }
}

/**
 * Process markItUp editor delete form submission.
 */
function markitup_editor_delete_submit($form, &$form_state) {
  db_query("UPDATE {markitup_editors_formats} SET meid = %d WHERE meid = %d", variable_get('markitup_editor_default', 1), $form_state['values']['meid']);
  db_query("DELETE FROM {markitup_editors} WHERE meid = %d", $form_state['values']['meid']);

  drupal_set_message(t('Deleted editor %markitup_editor.', array('%markitup_editor' => $form_state['values']['name'])));

  $form_state['redirect'] = 'admin/settings/markitup';
  return;
}
