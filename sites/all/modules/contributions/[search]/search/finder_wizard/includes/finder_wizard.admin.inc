<?php
// $Id: finder_wizard.admin.inc,v 1.1.2.3 2008/11/08 05:37:05 danielb Exp $


/**
 * Admin finder wizard list page
 */

function finder_wizard_admin_list() {
  finder_wizard_css();
  finder_wizard_inc('crud');

  $output = '';
  $finder_wizards = finder_wizard_crud_read_all_summaries();

  if (empty($finder_wizards)) {
    $output .= t("!add finder wizard.", array('!add' => l('Add', 'admin/build/finder_wizard/add')));
  }
  else {
    foreach ((array)$finder_wizards as $finder_wizard) {
      $rows = array();
      $rows[] = array(
        array(
          'data' => "<strong>". check_plain($finder_wizard->title) ."</strong>", 
          'class' => 'finder-wizard-title',
        ), 
        array(
          'data' => l('Edit', 'admin/build/finder_wizard/edit/'. $finder_wizard->fwid)
            . t(" | ") . l('Delete', 'admin/build/finder_wizard/delete/'. $finder_wizard->fwid), 
          'class' => 'finder-wizard-ops',
          'align' => 'right',
        ), 
      );
      $rows[] = array(
        array(
          'data' => "Path: <span>". l($finder_wizard->path, $finder_wizard->path) ."</span>", 
          'class' => 'finder-wizard-summary',
        ), 
        array(
          'data' =>  "<div class='description'>". check_markup($finder_wizard->description) ."</div>", 
          'class' => 'finder-wizard-desc description',
        ), 
      );
      $output .= theme('table', array(), $rows, array('class' => 'finder-wizard-table finder-wizard-'. $finder_wizard->fwid));
    }
  }

  return $output;

}

/**
 * Admin finder wizard add/edit page
 */
function finder_wizard_admin_edit($form_state, $finder_wizard = 'add') {
  finder_wizard_css();
  finder_wizard_inc('node');  // for the 'types' options
  $types = finder_wizard_get_types();
  $field_options = finder_wizard_get_fields();
  // build the form
  $form = array();
  $form['#tree'] = TRUE;
  $form['fwid'] = array(
    '#type' => 'value',
    '#value' => $finder_wizard->fwid ? $finder_wizard->fwid : 'add',
    '#weight' => 0,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $finder_wizard->title ? $finder_wizard->title : '',
    '#size' => 60,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => 2,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $finder_wizard->description ? $finder_wizard->description : '',
    '#size' => 60,
    '#description' => t('This will appear on the Finder Wizard administrative page.'),
    '#weight' => 3,
  );
  $form['path'] = array(   //to do : add validation to ensure this is a valid path
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $finder_wizard->path ? $finder_wizard->path : '',
    '#description' => t('The finder wizard needs a path from which it can be accessed.'),
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => 4,
  );
  $form['adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['adv']['none_above'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include a "none of the above" option by default.'),
    '#weight' => 2,
    '#default_value' => $finder_wizard->none_above ? $finder_wizard->none_above : 0,
    '#description' => t('For textfields this will be executed when an invalid value is given, rather than giving an error.  You can override this per step.'),
  );
  $form['adv']['none_above_path'] = array(   //to do : add validation to ensure this is a real path
    '#type' => 'textfield',
    '#title' => t('Path to go to upon choosing "none of the above" by default'),
    '#default_value' => $finder_wizard->none_above_path ? $finder_wizard->none_above_path : 'node',
    '#description' => t('The finder wizard needs a path to send the none of the above option to.  You can override this per step.'),
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => 4,
  );
  $form['adv']['error_msg'] = array(   //to do : add validation to ensure this is a real path
    '#type' => 'textarea',
    '#title' => t('Error message to use if not using "none of the above" by default'),
    '#default_value' => $finder_wizard->error_msg ? $finder_wizard->error_msg : t('Invalid value specified.'),
    '#description' => t('The error message to display for a textfield if the input is not a valid value.  You can override this per step.'),
    '#size' => 8,
    '#required' => TRUE,
    '#weight' => 6,
  );
  $form['adv']['early_exit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Finish wizard upon finding <em>one or zero</em> nodes, even if all questions are not answered yet.'),
    '#weight' => 8,
    '#default_value' => $finder_wizard->early_exit ? $finder_wizard->early_exit : 0,
  );
  $form['adv']['results_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Always show the results page after finishing the wizard.'),
    '#description' => t('By default if there is only one result node, the user is redirected to that node instead of getting a results page.'),
    '#weight' => 10,
    '#default_value' => $finder_wizard->results_page ? $finder_wizard->results_page : 0,
  );
  $form['types'] = array(
    '#type' => 'select',
    '#title' => t('Node types'),
    '#default_value' => $finder_wizard->types ? $finder_wizard->types : array(''),
    '#options' => finder_wizard_get_types(),
    '#description' => t('Pick the node types that will be searched upon, choose &lt;none&gt; for all node types.'),
    '#multiple' => TRUE,
    '#weight' => 18,
    '#size' => 6,
  );
  if (is_numeric($finder_wizard->fwid)) {
    $form['steps'] = array(
      '#title' => t('Steps'),
      '#weight' => 20,
      '#theme' => 'finder_wizard_admin_edit_steps_table',
      '#suffix' => "<div class='add-step'>". l(t('Add a step'), 'admin/build/finder_wizard/edit/'. $finder_wizard->fwid .'/step/add') ."</div>",
    );
    if (!empty($finder_wizard->steps)) {
      foreach($finder_wizard->steps as $key => $value) {
        $step_output = "<div class='finder-wizard-step'>";
        $step_output .= "<div class='finder-wizard-question'><strong>". check_markup($value['question']) ."</strong></div>";
        $step_output .= "<div class='finder-wizard-field'><em>Answers:</em> <span>";
        $step_output .= $field_options[$value['field_handler'] .'.'. $value['field_name']] ."</span></div>";
        $step_output .= "<div class='finder-wizard-question'><em>Response:</em> ". check_markup($value['response']) ."</div>";
        $step_output .= "</div>";
        $form['steps'][$value['fwsid']]['value']['#value'] = $step_output;
        $form['steps'][$value['fwsid']]['ops']['#value'] = 
          l('Edit', 'admin/build/finder_wizard/edit/'. $finder_wizard->fwid . '/step/edit/'. $value['fwsid'])
            . t(" | ") . l('Delete', 'admin/build/finder_wizard/edit/'. $finder_wizard->fwid . '/step/delete/'. $value['fwsid']);
        $form['steps'][$value['fwsid']]['weight'] = array(
          '#type' => 'weight',
          '#delta' => count($finder_wizard->steps),
          '#default_value' => $key,
        );
      }
    }
  }
  else {
    drupal_set_message('You may configure steps for this finder wizard after saving.', 'warning');
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save finder wizard'),
    '#weight' => 22,
  );
  return $form;
}

/**
 * Submit function for admin finder wizard add/edit page
 */
function finder_wizard_admin_edit_submit($form, &$form_state) {
  finder_wizard_inc('crud');
  $values = &$form_state['values'];
  if (!is_numeric($values['fwid'])) {
    $values['fwid'] = finder_wizard_crud_create_summary(
      $values['title'], 
      $values['description'], 
      $values['path'], 
      $values['adv']['none_above'], 
      $values['adv']['none_above_path'], 
      $values['adv']['error_msg'], 
      $values['adv']['early_exit'], 
      $values['adv']['results_page']);
  }
  else {
    finder_wizard_crud_update_summary(
      $values['fwid'], 
      $values['title'], 
      $values['description'], 
      $values['path'],
      $values['adv']['none_above'], 
      $values['adv']['none_above_path'], 
      $values['adv']['error_msg'], 
      $values['adv']['early_exit'], 
      $values['adv']['results_page']);
  }
  unset($values['types']['']);
  if (!empty($values['types'])) {
    finder_wizard_crud_update_types($values['fwid'], $values['types']);
  }
  if (is_array($values['steps'])) {
    foreach ($values['steps'] as $fwsid => $value) {
      finder_wizard_crud_update_step_weight($fwsid, $value['weight']);
    }
  }
  drupal_set_message("The finder wizard was saved.");
  menu_rebuild();
  drupal_goto("admin/build/finder_wizard/edit/". $values['fwid']);
}

/**
 * Admin finder wizard delete page
 */
function finder_wizard_admin_delete($form_state, $finder_wizard) {
  finder_wizard_css();
  // build the form
  $form = array();
  $form['#tree'] = TRUE;
  $form['fwid'] = array(
    '#type' => 'value', 
    '#value' => $finder_wizard->fwid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the "!fw" finder wizard?', array('!fw' => $finder_wizard->title)),
    $_GET['destination'] ? $_GET['destination'] : 'admin/build/finder_wizard/list',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Submit function for admin finder wizard delete page
 */
function finder_wizard_admin_delete_submit($form, &$form_state) {
  finder_wizard_inc('crud');
  $fwid = &$form_state['values']['fwid'];
  finder_wizard_crud_delete_summary($fwid);
  finder_wizard_crud_delete_types($fwid);
  finder_wizard_crud_delete_all_steps($fwid);
  drupal_set_message("The finder wizard was deleted.");
  menu_rebuild();
  drupal_goto("admin/build/finder_wizard/list");
}

/**
 * Admin finder wizard step add/edit page
 * must have a Finder wizard object at this point
 */
function finder_wizard_admin_step_edit($form_state, $finder_wizard, $fwsid = 'add') {
  finder_wizard_css();
  $field_options = finder_wizard_get_fields();

  // because we are not keying by $fwsid, we have to find the correct step like this.
  if (!empty($finder_wizard->steps)) {
    foreach ($finder_wizard->steps as $key => $value) {
      if ($value['fwsid'] == $fwsid) {
        $step = $finder_wizard->steps[$key];
        break;
      }
    }
  }

  $form = array();
  $form['#tree'] = TRUE;
  $form['fwid'] = array(
    '#type' => 'value',
    '#value' => $finder_wizard->fwid,
    '#weight' => 0,
  );
  $form['fwsid'] = array(
    '#type' => 'value',
    '#value' => $fwsid,
    '#weight' => 2,
  );
  $form['question'] = array(
    '#type' => 'textarea',
    '#title' => t('Question'),
    '#default_value' => $step['question'] ? $step['question'] : '',
    '#required' => TRUE,
    '#size' => 8,
    '#description' => t("The question to ask of the user in this step."),
    '#weight' => 4,
  );
  $form['field'] = array(
    '#type' => 'select',
    '#title' => t('Answers'),
    '#default_value' => ($step['field_handler'] && $step['field_name']) ? $step['field_handler'].'.'.$step['field_name'] : '',
    '#options' => $field_options,
    '#description' => t('Which field to get the answers from.'),
    '#weight' => 6,
  );
  $form['response'] = array(
    '#type' => 'textarea',
    '#title' => t('Response'),
    '#default_value' => $step['response'] ? $step['response'] : 'You have selected !answer',
    '#required' => TRUE,
    '#size' => 8,
    '#description' => t("The message to give the user on the NEXT step.  Use <em>!answer</em> to insert the chosen value."),
    '#weight' => 8,
  );
  $form['element'] = array(
    '#type' => 'select',
    '#title' => t('Element'),
    '#default_value' => $step['element'] ? $step['element'] : '',
    '#options' => array(
      'radios' => t('Radios'),
      'textfield' => t('Textfield'), 
    ), // this sucks - make this smarter somehow
    '#description' => t('Which form element to use in this step.'),
    '#weight' => 10,
  );
  $form['adv'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#weight' => 12,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['adv']['none_above'] = array(
    '#type' => 'radios',
    '#title' => t('Include a "none of the above" option.'),
    '#weight' => 2,
    '#options' => array(
      0 => t('Use wizard default.'),
      1 => t('Include'),
      2 => t('Do not include'),
    ),
    '#default_value' => $step['none_above'] ? $step['none_above'] : 0,
    '#description' => t('For textfields this will be executed when an invalid value is given, rather than giving an error.'),
  );
  $form['adv']['none_above_path'] = array(   //to do : add validation to ensure this is a real path
    '#type' => 'textfield',
    '#title' => t('Path to go to upon choosing "none of the above"'),
    '#default_value' => $step['none_above_path'] ? $step['none_above_path'] : '',
    '#description' => t('Leave blank to use wizard default.'),
    '#size' => 30,
    '#maxlength' => 128,
    '#weight' => 4,
  );
  $form['adv']['error_msg'] = array(   //to do : add validation to ensure this is a real path
    '#type' => 'textarea',
    '#title' => t('Error message to use if not using "none of the above"'),
    '#default_value' => $step['error_msg'] ? $step['error_msg'] : '',
    '#description' => t('Leave blank to use wizard default.'),
    '#size' => 8,
    '#weight' => 6,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save finder wizard step'),
    '#weight' => 20,
  );
  return $form;
}

/**
 * Submit function for admin finder wizard add/edit page
 */
function finder_wizard_admin_step_edit_submit($form, &$form_state) {
  finder_wizard_inc('crud');
  $values = &$form_state['values'];

  // expand 'field' values into handler and name
  $field = explode('.', $values['field']);
  $values['field_handler'] = &$field[0];
  $values['field_name'] = &$field[1];

  if (!is_numeric($values['fwsid'])) {
    $values['fwsid'] = finder_wizard_crud_create_step(
      $values['fwid'], $values['question'], $values['response'], $values['field_handler'], 
      $values['field_name'], $values['adv']['none_above'], $values['adv']['none_above_path'], $values['adv']['error_msg'], $values['element']);
  }
  else {
    finder_wizard_crud_update_step(
      $values['fwsid'], $values['fwid'], $values['question'], $values['response'], 
      $values['field_handler'], $values['field_name'], $values['adv']['none_above'], $values['adv']['none_above_path'], $values['adv']['error_msg'], $values['element']);
  }
  drupal_set_message("The finder wizard step was saved.");
  drupal_goto("admin/build/finder_wizard/edit/". $values['fwid']);
}

/**
 * Admin finder wizard delete page
 */
function finder_wizard_admin_step_delete($form_state, $finder_wizard, $fwsid) {
  finder_wizard_css();
  // build the form
  $form = array();
  $form['#tree'] = TRUE;
  $form['fwsid'] = array(
    '#type' => 'value', 
    '#value' => $fwsid,
  );
  $form['fwid'] = array(
    '#type' => 'value', 
    '#value' => $finder_wizard->fwid,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the finder wizard step?'),
    $_GET['destination'] ? $_GET['destination'] : 'admin/build/finder_wizard/edit/'. $finder_wizard->fwid,
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

/**
 * Submit function for admin finder wizard delete page
 */
function finder_wizard_admin_step_delete_submit($form, &$form_state) {
  finder_wizard_inc('crud');
  $fwsid = &$form_state['values']['fwsid'];
  $fwid = &$form_state['values']['fwid'];
  finder_wizard_crud_delete_step($fwsid);
  drupal_set_message("The finder wizard step was deleted.");
  drupal_goto("admin/build/finder_wizard/edit/". $fwid);
}

// TO DO:
// validation
// checks to see if database queries were actually executed
// ensure unique weights when no javascript present