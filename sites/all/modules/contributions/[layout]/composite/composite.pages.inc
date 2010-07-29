<?php
// $Id: composite.pages.inc,v 1.1.2.2 2009/01/15 02:41:13 bengtan Exp $

/***********************************************************
*                       LOCAL TASK                         *
************************************************************/

function composite_general_select_page($type, $node) {
  $type_def = composite_get_types($type);
  drupal_set_title(t('@label for @title', array('@label' => $type_def['label']['plural'], '@title' => $node->title)));
  return drupal_get_form('composite_general_select_form', $type_def, $node);
}

function composite_general_select_form(&$form_state, $type, $node) {
  // If there is no potentials callback defined, then there's no point continuing. 
  $potentials_callback = $type['potentials callback'];
  composite_include_file($type);
  if (!$potentials_callback || !function_exists($potentials_callback)) {
    return;
  }

  // Save for later use
  $form['#node'] = $node;
  $form['composite_type'] = array('#type' => 'value', '#value' => $type['type']);

  // Retrieve select list of potentials
  $options = $potentials_callback($node);
  
  // Fill in our selected values
  $selected = array();
  foreach ($node->composite_references['#'. $type['type'] .'_references'] as $id => $unused) {
    // Make sure this block still exists
    if (array_key_exists($id, $options)) {
      $selected[$id] = $id;
    }
  }
  
  $form['composite_items'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Items to display in @title', array('@title' => $node->title)),
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' => t('Please select the items you would like to display as part of this node.'),
    '#default_value' => $selected,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;  
}

function composite_general_select_form_submit($form, &$form_state) {
  $node = $form['#node'];
  $type = composite_get_types($form_state['values']['composite_type']);
  
  // Construct a list of items to delete, and a list of new items to insert
  $delete_items = array();
  $insert_items = array();
  foreach ($form_state['values']['composite_items'] as $id => $flag) {
    if ($flag && !$node->composite_references['#'. $type['type'] .'_references'][$id]) {
      // This is an added item, add to in the insert list
      $insert_items[$id] = $id;
    }
    else if (!$flag && $node->composite_references['#'. $type['type'] .'_references'][$id]) {
      // This is a to-be-deleted item, add to the delete list
      $delete_items[$id] = $id;
    }
  }

  // Delete old items  
  foreach ($delete_items as $id => $flag) {
    db_query("DELETE FROM {node_composite_references} WHERE vid = %d AND id = '%s'", $node->vid, $id);
  }

  // Add new entries
  $args = array();
  $query_parts = array();
  foreach ($insert_items as $id => $flag) {
    $query_parts[] = " (%d, %d, '%s', %d, '%s', '%s', '%s')";
    $args[] = $node->nid;
    $args[] = $node->vid;
    $args[] = $type['type'];
    $args[] = 0;
    $args[] = $id;
    $args[] = '';
    $args[] = COMPOSITE_ZONE_NONE;      
  }

  if (count($query_parts)) {
    $query = "INSERT INTO {node_composite_references} (nid, vid, type, weight, id, data, zone) VALUES" . implode(', ', $query_parts);
    db_query($query, $args);
  }
  drupal_set_message(t('The selected items have been saved.'));
}

/***********************************************************
*                          ZONES                           *
************************************************************/

function composite_zones_page($node) {
  drupal_set_title(t('Zones for @title', array('@title' => $node->title)));
  if ($node->composite_layout) {
    $output .= drupal_get_form('composite_zones_form', $node);
    // $output .= theme('composite_zones_preview', node_view($node, FALSE, FALSE, FALSE));
    return $output;
  }
  else {
    return t('You need to choose a layout first.');
  }
}

function composite_zones_form(&$form_state, $node) {
  // Dispatch to other form handlers if applicable
  switch ($form_state['operation']) {
    case 'delete':
      return composite_zones_multiple_delete_confirm($form_state, $node);
    case 'save_new_set':
    case 'save_set':
    case 'delete_set':
      return composite_sets_form($form_state, $node);
  }

  drupal_add_css(drupal_get_path('module', 'block') .'/block.css', 'module', 'all');

  $form['#tree'] = TRUE;

  if (isset($form_state['node'])) {
    // Use the node override instead of the default
    $node = $form_state['node'];
  }
  // Insert preview if it exists
  if (isset($form_state['node_preview'])) {
    $form['#prefix'] = $form_state['node_preview'];
  }

  // Save for later
  $form['#node'] = $node;
  
  $layout = composite_get_layout($node->composite_layout);
  $layout['data'] = $node->composite_layout_data;
  $layout_zones = $layout['zones'] + array(COMPOSITE_ZONE_NONE => t('<none>'));
  // Need to pass this through to template function
  $form['#zones'] = $layout_zones;

  // Layout settings
  if (isset($layout['settings callback'])) {
    $function = $layout['settings callback'];
    if (function_exists($function)) {
      $layout_settings = $function('form', $layout);
      if ($layout_settings) {
        $form['layout_settings'] = array(
          '#type' => 'fieldset',
          '#title' => t('Layout settings'),
          '#description' => t('These settings affect the whole layout.'),
          '#collapsible' => TRUE,
        );
        $form['layout_settings']['nid'] = array('#type' => 'value', '#value' => $node->nid);
        $form['layout_settings']['vid'] = array('#type' => 'value', '#value' => $node->vid);
        $form['layout_settings']['data'] = $layout_settings;
      }
    }
  }
  $form['composite_layout'] = array('#type' => 'value', '#value' => $node->composite_layout);
  $form['vid'] = array('#type' => 'value', '#value' => $node->vid);

  // Composite references
  $references = _composite_references_preprocess($node->composite_references, $layout_zones, $node);
  _composite_compare('reset-zones', $layout_zones);
  usort($references, '_composite_compare');

  foreach ($references as $id => $reference) {
    $form[$id]['id'] = array('#type' => 'value', '#value' => $reference['id']);
    $form[$id]['type'] = array('#type' => 'value', '#value' => $reference['type']);
    $form[$id]['nid'] = array('#type' => 'value', '#value' => $node->nid);
    $form[$id]['vid'] = array('#type' => 'value', '#value' => $node->vid);
    $form[$id]['info'] = array('#value' => $reference['info']);
    $form[$id]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $reference['weight'],    
    );
    $form[$id]['zone'] = array(
      '#type' => 'select',
      // If the reference is in a zone that no longer exists, place it in the disabled list. 
      '#default_value' => array_key_exists($reference['zone'], $layout_zones) ? $reference['zone'] : '-1',
      '#options' => $layout_zones,
    );
    $extra_fields = composite_invoke_referenceapi($reference, 'settings', $node);
    if ($extra_fields) {
      $form[$id]['data'] = $extra_fields;
      // Kludgey
      $form[$id]['data']['#prefix'] = '<div class="container-inline">';
      $form[$id]['data']['#suffix'] = '</div>';
    }
    $form[$id]['checkbox'] = array(
      '#type' => 'checkbox',
      '#default_value' => 0,
    );
  }

  $form['update_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tasks'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $form['update_options']['operation'] = array(
    '#type' => 'select',
    '#options' => array(
      'delete' => t('Delete checked items'),
      t('Layout sets') => array(
        'save_new_set' => t('Save as new layout set'),
        'save_set' => t('Save to existing layout set'),
        'delete_set' => t('Delete a layout set'),
      ),
    ),
  );
  if (composite_sets_select()) {
    $form['update_options']['operation']['#options'][t('Populate with layout set:')] = composite_sets_select();
  }

  $form['update_options']['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('composite_zones_form_operations_submit'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save zones'),
    '#submit' => array('composite_zones_form_submit'),
  );
  $form['save_and_view'] = array(
    '#type' => 'submit',
    '#value' => t('Save zones and view'),
    '#submit' => array('composite_zones_form_submit'),
  );
  $form['buttons']['preview'] = array(
    '#type' => 'submit',
    '#value' => t('Preview'),
    '#submit' => array('composite_zones_form_preview'),
  );
  return $form;
}

function composite_zones_form_submit($form, &$form_state) {
  // Fetch references from database so we can decide whether to insert or update
  $vid = $form_state['values']['vid'];
  $existings = array();
  $result = db_query("SELECT vid, id FROM {node_composite_references} WHERE vid = %d", $vid);
  while ($object = db_fetch_object($result)) {
    $existings[$object->id] = $object;
  }

  // Save references
  foreach ($form_state['values'] as $reference) {
    if (is_array($reference) && isset($reference['id'])) {
      $data = $reference['data'] ? serialize($reference['data']) : '';
      if (isset($existings[$reference['id']])) {
        db_query("UPDATE {node_composite_references} SET weight = %d, data = '%s', zone = '%s' WHERE vid = %d AND id = '%s'", $reference['weight'], $data, $reference['zone'], $reference['vid'], $reference['id']);
        unset($existings[$reference['id']]);
      }
      else {
        db_query("INSERT INTO {node_composite_references} (nid, vid, type, weight, id, data, zone) VALUES (%d, %d, '%s', %d, '%s', '%s', '%s')", $reference['nid'], $reference['vid'], $reference['type'], $reference['weight'], $reference['id'], $data, $reference['zone']);
      }
    }
  }

  // Now delete existing references which aren't needed anymore
  foreach ($existings as $object) {
    db_query("DELETE FROM {node_composite_references} WHERE vid = %d AND id = '%s'", $object->vid, $object->id);
  }

  // Save layout settings
  if (isset($form_state['values']['layout_settings']) || isset($form_state['values']['composite_layout'])) {
    $composite_layout_data = $form_state['values']['layout_settings']['data'] ? serialize($form_state['values']['layout_settings']['data']) : '';
    db_query("UPDATE {node_composite} SET layout = '%s', data = '%s' WHERE vid = %d", $form_state['values']['composite_layout'], $composite_layout_data, $vid);
  }

  drupal_set_message(t('The selected zones have been saved.'));
  if ($form_state['values']['op'] == t('Save zones and view')) {
    $node = $form['#node'];
    $form_state['redirect'] = 'node/' . $node->nid;
  }
}

function composite_zones_form_preview($form, &$form_state) {
  $node = $form['#node'];

  // Refresh the layout settings
  $node->composite_layout_data = $form_state['values']['layout_settings']['data'];

  // Rebuild composite references with the submitted values
  $original_references = $node->composite_references;
  $node->composite_references = array();
  
  foreach ($form_state['values'] as $reference) {
    if (is_array($reference) && isset($reference['id'])) {
      // Fill in values which were not transferred with the form (and
      //   implicitly, remained unchanged). 
      $reference['type'] = $original_references[$reference['id']]['type'];
      composite_invoke_referenceapi($reference, 'load');
      
      $node->composite_references[$reference['id']] = $reference;
    }
  }

  // Save values for the next form iteration
  $form_state['node'] = $node;
  $form_state['node_preview'] = theme('composite_zones_preview', drupal_clone($node));

  drupal_set_title(t('Preview'));
  drupal_set_message('This is a preview. Changes will not be saved until the Save zones button is clicked.');

  // Setting this flag seems to propagate ['node_preview'] back to the form builder
  $form_state['rebuild'] = TRUE;
}

function composite_zones_form_operations_submit($form, &$form_state) {
  if ($form_state['values']['update_options']['operation']) {
    // Special case: A number means populate with that layout set sid
    if (is_numeric($form_state['values']['update_options']['operation'])) {
      composite_zones_form_operations_populate($form, $form_state);
    }
    else {
      $form_state['operation'] = $form_state['values']['update_options']['operation'];
      // Cycle this form
      $form_state['rebuild'] = TRUE;
    }
  }  
}

function composite_zones_form_operations_populate($form, &$form_state) {
  // Fetch the layout set
  $node = $form['#node'];
  composite_sets_populate_node($node, $form_state['values']['update_options']['operation']);
  
  // Trigger a preview. Save values for the next form iteration
  $form_state['node'] = $node;
  $form_state['node_preview'] = theme('composite_zones_preview', drupal_clone($node));
  
  drupal_set_title(t('Populating with layout set'));
  drupal_set_message(t('Populating with layout set %name.', array('%name' => $layout_set->name)));
  drupal_set_message('This is a preview. Changes will not be saved until the Save zones button is clicked.');

  // Setting this flag propagates ['node_preview'] back to the form builder
  $form_state['rebuild'] = TRUE;
}

function composite_zones_multiple_delete_confirm(&$form_state, $node) {
  $form['#node'] = $node;
  $form['references'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  foreach ($form_state['values'] as $reference) {
    if (is_array($reference) && isset($reference['id']) && $reference['checkbox']) {
      composite_invoke_referenceapi($reference, 'load');
      composite_invoke_referenceapi($reference, 'info');

      $form['references'][$reference['id']] = array(
        '#type' => 'hidden',
        '#value' => $reference['id'],
        '#prefix' => '<li>',
        '#suffix' => $reference['info'] ."</li>\n",
      );
    }
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');  
  $form['#submit'][] = 'composite_zones_multiple_delete_confirm_submit';
  return confirm_form($form,
                      t('Are you sure you want to delete these items?'),
                      'node/' . $node->nid . '/composite_zones', '',
                      t('Delete all'), t('Cancel'));
}

function composite_zones_multiple_delete_confirm_submit($form, &$form_state) {
  $node = $form['#node'];
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['references'] as $id => $reference) {
      db_query("DELETE FROM {node_composite_references} WHERE vid = %d AND id = '%s'", $node->vid, $id);
    }
    drupal_set_message(t('The items have been deleted.'));
  }
  $form_state['redirect'] = 'node/' . $node->nid . '/composite_zones';
  return;
}


// This function borrows some code from template_preprocess_block_admin_display_form
function template_preprocess_composite_zones_form(&$variables) {
  $layout_zones = $variables['form']['#zones'];
  $layout_zones[COMPOSITE_ZONE_NONE] = t('Disabled');
  $variables['layout_zones'] = $layout_zones;
  
  foreach ($layout_zones as $key => $value) {
    // Highlight regions on page to provide visual reference.
    // drupal_set_content($key, '<div class="block-region">'. $value .'</div>');
    // Initialize an empty array for the region.
    $variables['zone_listing'][$key] = array();
  }
  $variables['zone_listing'][COMPOSITE_ZONE_NONE] = array();

  foreach (element_children($variables['form']) as $i) {
    $reference = &$variables['form'][$i];

    // Only take form elements that are references.
    if (isset($reference['id'])) {
      // Fetch zone for current block.
      $zone = $reference['zone']['#default_value'];

      // Set special classes needed for table drag and drop.
      $variables['form'][$i]['zone']['#attributes']['class'] = 'block-region-select block-region-'. $zone;
      $variables['form'][$i]['weight']['#attributes']['class'] = 'block-weight block-weight-'. $zone;

      $variables['zone_listing'][$zone][$i]->row_class = isset($reference['#attributes']['class']) ? $reference['#attributes']['class'] : '';
      $variables['zone_listing'][$zone][$i]->reference_modified = isset($reference['#attributes']['class']) && strpos($reference['#attributes']['class'], 'block-modified') !== FALSE ? TRUE : FALSE;
      $variables['zone_listing'][$zone][$i]->reference_title =  drupal_render($reference['info']);
      $variables['zone_listing'][$zone][$i]->zone_select = drupal_render($reference['zone']); //. drupal_render($block['theme']);
      $variables['zone_listing'][$zone][$i]->weight_select = drupal_render($reference['weight']);
      $variables['zone_listing'][$zone][$i]->data_widget = drupal_render($reference['data']);
      $variables['zone_listing'][$zone][$i]->checkbox = drupal_render($reference['checkbox']);
      $variables['zone_listing'][$zone][$i]->printed = FALSE;
    }
  }

  $variables['form_layout_settings'] = drupal_render($variables['form']['layout_settings']);
  $variables['form_submit'] = drupal_render($variables['form']);
}

/***********************************************************
*               COMPOSITE LAYOUT SETS                      *
************************************************************/

function composite_sets_form(&$form_state, $node) {
  $title_map = array(
    'save_new_set' => t('Save new layout set'),
    'save_set' => t('Save to existing layout set'), 
    'delete_set' => t('Delete layout set'),
  );
  $submit_map = array(
    'save_new_set' => 'composite_sets_form_save_new_submit',
    'save_set' => 'composite_sets_form_save_submit', 
    'delete_set' => 'composite_sets_form_delete_submit',
  );
  $description_map = array(
    'save_new_set' => t('Please enter a name for your new layout set. Name must start with a letter, and only contain letters, numbers, underscore, hyphen, and space.'),
    'save_set' => t('Please select the layout set to save to.'), 
    'delete_set' => t('Please select the layout set to delete. Note: You cannot undo this action.'),
  );
  
  $operation = $form_state['operation'];  
  drupal_set_title($title_map[$operation]);

  // Save some data for later
  $form['#node'] = $node;
  $form['composite_layout'] = array('#type' => 'value', '#value' => $node->composite_layout);
  $form['composite_layout_data'] = array('#type' => 'value', '#value' => $node->composite_layout_data);
  
  $form['composite_references'] = array('#tree' => TRUE);
  foreach ($form_state['values'] as $reference) {
    if (is_array($reference) && isset($reference['id'])) {
      // composite_invoke_referenceapi($reference, 'load');
      // composite_invoke_referenceapi($reference, 'info');

      // Get rid of some fields we don't want
      unset($reference['nid']);
      unset($reference['vid']);
      unset($reference['checkbox']);

      $form['composite_references'][$reference['id']] = array(
        '#type' => 'value',
        '#value' => $reference,
      );
    }
  }

  if ($operation == 'save_new_set') {
    $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#required' => TRUE,
      '#description' => $description_map[$operation],
    );
  }
  else {
    $form['sid'] = array(
      '#type' => 'select',
      '#title' => t('Layout set'),
      '#required' => TRUE,
      '#options' => composite_sets_select(),
      '#description' => $description_map[$operation],
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $title_map[$operation],
  );
  $form['cancel'] = array(
    '#value' => l(t('Cancel'), 'node/' . $node->nid . '/composite_zones'),
  );
  
  $form['operation'] = array('#type' => 'hidden', '#value' => $operation);
  // $form['#validate'][] = 'composite_sets_save_new_form_validate';

  $form['#submit'][] = $submit_map[$operation]; // 'composite_sets_save_new_form_submit';
  // Theme this form like a normal form - Bypass template_preprocess_composite_zones_form()
  $form['#theme'] = 'markup';
  return $form;
}

// ToDo: Need to validate a layout set name? 
/*
function composite_sets_save_new_form_validate($form, &$form_state) {
  // _dd($form_state['values']);
  // form_set_error('blah', 'ding');
  // ToDo: validate legal chars? 

  // ToDo: Check for unique set name
}
*/

function composite_sets_form_save_new_submit($form, &$form_state) {
  $values = $form_state['values'];
  $object = new StdClass();
  $object->name = $values['name'];
  $object->data = array(
    'composite_layout' => $values['composite_layout'], 
    'composite_layout_data' => $values['composite_layout_data'],
    'composite_references' => $values['composite_references'], 
  );
  drupal_write_record('node_composite_sets', $object);
  drupal_set_message(t('The layout set %name has been saved.', array('%name' => $object->name)));
}

function composite_sets_form_save_submit($form, &$form_state) {
  $select = composite_sets_select();
  $values = $form_state['values'];
  $object = new StdClass();
  $object->sid = $values['sid'];
  $object->name = $select[$object->sid];
  $object->data = array(
    'composite_layout' => $values['composite_layout'], 
    'composite_layout_data' => $values['composite_layout_data'],
    'composite_references' => $values['composite_references'], 
  );
  drupal_write_record('node_composite_sets', $object, 'sid');
  drupal_set_message(t('The layout set %name has been saved.', array('%name' => $object->name)));
}

function composite_sets_form_delete_submit($form, &$form_state) {
  $select = composite_sets_select();
  $values = $form_state['values'];
  db_query("DELETE FROM {node_composite_sets} WHERE sid = '%d'", $values['sid']);
  drupal_set_message(t('The layout set %name has been deleted.', array('%name' => $select[$values['sid']])));
}

