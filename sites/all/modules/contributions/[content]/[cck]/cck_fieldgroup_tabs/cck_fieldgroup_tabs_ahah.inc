<?php
// $Id: cck_fieldgroup_tabs_ahah.inc,v 1.1 2008/09/29 20:46:50 moonshine Exp $

/**
 * @file
 * Modified AHAH callbacks to handle the changes CCK Fieldgroup Tabs make to form structure
 */

/**
 * Modified version of CCK's content_add_more_js()
 */
function cck_fieldgroup_tabs_content_add_more_js($type_name_url, $field_name) {
  $type = content_types($type_name_url);
  $field = content_fields($field_name, $type['type']);

  if (($field['multiple'] != 1) || empty($_POST['form_build_id'])) {
    // Invalid request.
    drupal_json(array('data' => ''));
    exit;
  }

  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  if (!$form) {
    // Invalid form_build_id.
    drupal_json(array('data' => ''));
    exit;
  }

  // We don't simply return a new empty widget to append to existing ones, because
  // - ahah.js won't simply let us add a new row to a table
  // - attaching the 'draggable' behavior won't be easy
  // So we resort to rebuilding the whole table of widgets including the existing ones,
  // which makes us jump through a few hoops.

  // The form that we get from the cache is unbuilt. We need to build it so that
  // _value callbacks can be executed and $form_state['values'] populated.
  // We only want to affect $form_state['values'], not the $form itself
  // (built forms aren't supposed to enter the cache) nor the rest of $form_data,
  // so we use copies of $form and $form_data.
  $form_copy = $form;
  $form_state_copy = $form_state;
  $form_copy['#post'] = array();
  form_builder($_POST['form_id'], $form_copy, $form_state_copy);
  // Just grab the data we need.
  $form_state['values'] = $form_state_copy['values'];
  // Reset cached ids, so that they don't affect the actual form we output.
  form_clean_id(NULL, TRUE);

  // Sort the $form_state['values'] we just built *and* the incoming $_POST data
  // according to d-n-d reordering.
  unset($form_state['values'][$field_name][$field['field_name'] .'_add_more']);
  foreach ($_POST[$field_name] as $delta => $item) {
    $form_state['values'][$field_name][$delta]['_weight'] = $item['_weight'];
  }
  $form_state['values'][$field_name] = _content_sort_items($field, $form_state['values'][$field_name]);
  $_POST[$field_name] = _content_sort_items($field, $_POST[$field_name]);

  // Build our new form element for the whole field, asking for one more element.
  $form_state['item_count'] = array($field_name => count($_POST[$field_name]) + 1);
  $form_element = content_field_form($form, $form_state, $field);

  // Let other modules alter it.
  drupal_alter('form', $form_element, array(), 'content_add_more_js');
  
  // CCK FIELDGROUP TAB SUPPORT -- BEGIN CHANGES -- SET A
  $field_location = NULL;
  $fieldgroup_info = fieldgroup_groups($type['type']);
  
  foreach ($fieldgroup_info as $fieldgroup => $group_info) {
    if (isset($group_info['fields'][$field_name])) {
      if ($group_info['settings']['form']['style'] == "tabs") {
        if (isset($form['fieldgroup_tabs'][$fieldgroup][$field_name])) {
          // Field is under a Fieldgroup Tab
          $form['fieldgroup_tabs'][$fieldgroup][$field_name] = $form_element[$field_name];
          $field_location = "['fieldgroup_tabs']['". $fieldgroup ."']['". $field_name ."']";
        }
      }
      else {
        if (isset($form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$fieldgroup][$field_name])) {
          // Field is in a Fieldgroup inside the default "basics" Fieldgroup Tab
          $form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$fieldgroup][$field_name] = $form_element[$field_name];
          $field_location = "['fieldgroup_tabs']['fieldgroup_tabs_basic']['". $fieldgroup ."']['". $field_name ."']";
        }
        elseif (isset($form[$fieldgroup][$field_name])) {
          // Field is in a normal Fieldgroup
          $form[$fieldgroup][$field_name] = $form_element[$field_name];
          $field_location = "['". $fieldgroup ."']['". $field_name ."']";
        }
      }
    }
  }
  if ($field_location == NULL) {
    if (isset($form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$field_name])) {
      // Field is in the default "basics" Fieldgroup Tab
      $form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$field_name] = $form_element[$field_name];
      $field_location = "['fieldgroup_tabs']['fieldgroup_tabs_basic']['". $field_name ."']";
    }
    elseif (isset($form[$field_name])) {
      // Field not in any Fieldgroup or Fieldgroup tab, just in the form
      $form[$field_name] = $form_element[$field_name];
      $field_location = "['". $field_name ."']";
    }
  }
  // CCK FIELDGROUP TAB SUPPORT -- END CHANGES -- SET A

  // Save the new definition of the form.
  $form_state['values'] = array();
  form_set_cache($form_build_id, $form, $form_state);

  // Build the new form against the incoming $_POST values so that we can
  // render the new element.
  $delta = max(array_keys($_POST[$field_name])) + 1;
  $_POST[$field_name][$delta]['_weight'] = $delta;
  $form_state = array('submitted' => FALSE);
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  $form = form_builder($_POST['form_id'], $form, $form_state);

  // CCK FIELDGROUP TAB SUPPORT -- BEGIN CHANGES -- SET B
  // Render the new output.
  $field_form = eval('return $form'. $field_location .';');
  // CCK FIELDGROUP TAB SUPPORT -- END CHANGES -- SET B
  // We add a div around the new content to receive the ahah effect.
  $field_form[$delta]['#prefix'] = '<div class="ahah-new-content">'. (isset($field_form[$delta]['#prefix']) ? $field_form[$delta]['#prefix'] : '');
  $field_form[$delta]['#suffix'] = (isset($field_form[$delta]['#suffix']) ? $field_form[$delta]['#suffix'] : '') .'</div>';

  // If a newly inserted widget contains AHAH behaviors, they normally won't
  // work because AHAH doesn't know about those - it just attaches to the exact
  // form elements that were initially specified in the Drupal.settings object.
  // The new ones didn't exist then, so we need to update Drupal.settings
  // by ourselves in order to let AHAH know about those new form elements.
  $javascript = drupal_add_js(NULL, NULL);
  $output_js = isset($javascript['setting']) ? '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>' : '';

  $output = theme('status_messages') . drupal_render($field_form) . $output_js;
  drupal_json(array('status' => TRUE, 'data' => $output));
  exit;
}


/**
 * Modified version of Filefield's filefield_js()
 */
function cck_fieldgroup_tabs_filefield_js($type_name, $field_name, $delta) {
  $field = content_fields($field_name, $type_name);

  if (empty($field) || empty($_POST['form_build_id'])) {
    // Invalid request.
    print drupal_to_js(array('data' => ''));
    exit;
  }

  // Build the new form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);

  if (!$form) {
    // Invalid form_build_id.
    print drupal_to_js(array('data' => ''));
    exit;
  }

  // form_get_cache() doesn't yield the original $form_state,
  // but form_builder() does. Needed for retrieving the file array.
  $built_form = $form;
  $built_form_state = $form_state;

  $built_form += array('#post' => $_POST);
  $built_form = form_builder($_POST['form_id'], $built_form, $built_form_state);

  // Clean ids, so that the same element doesn't get a different element id
  // when rendered once more further down.
  form_clean_id(NULL, TRUE);

  // Ask CCK for the replacement form element. Going through CCK gets us
  // the benefit of nice stuff like '#required' merged in correctly.
  module_load_include('inc', 'content', 'includes/content.node_form');
  $field_element = content_field_form($form, $built_form_state, $field, $delta);
  $delta_element = $field_element[$field_name][0]; // there's only one element in there

  // Add the new element at the right place in the form.
  // CCK FIELDGROUP TAB SUPPORT -- BEGIN CHANGES -- SET A
  $field_location = NULL;
  $fieldgroup_info = fieldgroup_groups($type_name);
  
  foreach ($fieldgroup_info as $fieldgroup => $group_info) {
    if (isset($group_info['fields'][$field_name])) {
      if ($group_info['settings']['form']['style'] == "tabs") {
        if (isset($form['fieldgroup_tabs'][$fieldgroup][$field_name])) {
          // Field is under a Fieldgroup Tab
          $form['fieldgroup_tabs'][$fieldgroup][$field_name][$delta] = $delta_element;
          $field_location = "['fieldgroup_tabs']['". $fieldgroup ."']['". $field_name ."']";
        }
      }
      else {
        if (isset($form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$fieldgroup][$field_name])) {
          // Field is in a Fieldgroup inside the default "basics" Fieldgroup Tab
          $form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$fieldgroup][$field_name][$delta] = $delta_element;
          $field_location = "['fieldgroup_tabs']['fieldgroup_tabs_basic']['". $fieldgroup ."']['". $field_name ."']";
        }
        elseif (isset($form[$fieldgroup][$field_name])) {
          // Field is in a normal Fieldgroup
          $form[$fieldgroup][$field_name][$delta] = $delta_element;
          $field_location = "['". $fieldgroup ."']['". $field_name ."']";
        }
      }
    }
  }
  if ($field_location == NULL) {
    if (isset($form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$field_name])) {
      // Field is in the default "basics" Fieldgroup Tab
      $form['fieldgroup_tabs']['fieldgroup_tabs_basic'][$field_name][$delta] = $delta_element;
      $field_location = "['fieldgroup_tabs']['fieldgroup_tabs_basic']['". $field_name ."']";
    }
    elseif (isset($form[$field_name])) {
      // Field not in any Fieldgroup or Fieldgroup tab, just in the form
      $form[$field_name][$delta] = $delta_element;
      $field_location = "['". $field_name ."']";
    }
  }
  // CCK FIELDGROUP TAB SUPPORT -- END CHANGES -- SET A

  // Write the (unbuilt, updated) form back to the form cache.
  form_set_cache($form_build_id, $form, $form_state);

  // Render the form for output.
  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
  );
  drupal_alter('form', $form, array(), 'filefield_js');
  $form_state = array('submitted' => FALSE);
  $form = form_builder('filefield_js', $form, $built_form_state);
  // CCK FIELDGROUP TAB SUPPORT -- BEGIN CHANGES -- SET B
  $field_form = eval('return $form'. $field_location .';');
  // CCK FIELDGROUP TAB SUPPORT -- END CHANGES -- SET B

  // We add a div around the new content to tell AHAH to let this fade in.
  $field_form[$delta]['#prefix'] = '<div class="ahah-new-content">';
  $field_form[$delta]['#suffix'] = '</div>';
  
  $output = theme('status_messages') . drupal_render($field_form[$delta]);

  // AHAH is not being nice to us and doesn't know the "other" button (that is,
  // either "Upload" or "Delete") yet. Which in turn causes it not to attach
  // AHAH behaviours after replacing the element. So we need to tell it first.
  $javascript = drupal_add_js(NULL, NULL);
  if (isset($javascript['setting'])) {
    $output .= '<script type="text/javascript">jQuery.extend(Drupal.settings, '. drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .');</script>';
  }

  // For some reason, file uploads don't like drupal_json() with its manual
  // setting of the text/javascript HTTP header. So use this one instead.
  $GLOBALS['devel_shutdown'] = false;
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}