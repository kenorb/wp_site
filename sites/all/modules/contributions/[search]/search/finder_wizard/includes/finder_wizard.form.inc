<?php
// $Id: finder_wizard.form.inc,v 1.1.2.3 2008/11/08 05:37:05 danielb Exp $

/**
 * The wizard page
 */
function finder_wizard_form_page($fwid) {
  $finder_wizard = finder_wizard_load($fwid);
  $form = drupal_get_form('finder_wizard_form', $finder_wizard);
  $step = finder_wizard_form_step();
  return theme('finder_wizard_form_page', $finder_wizard, $form, $step);

}

/**
 * The form on the wizard page.
 */
function finder_wizard_form($form_state, $finder_wizard) {
  finder_wizard_css();
  $form = array();
  $step = 0;
  if (isset($form_state['storage']['step'])) {
    $step = $form_state['storage']['step'];
  }
  else {
    $form_state['storage']['step'] = $step;
  }
  if (!isset($form_state['storage']['finder_wizard'])) {
    $form_state['storage']['finder_wizard'] = $finder_wizard;
  }
  finder_wizard_form_step($step);
  $form[$step] = finder_wizard_get_element($form_state);
  $form['buttons'] = array(
    '#prefix' => "<div class='finder-wizard-buttons container-inline'>",
    '#suffix' => "</div>",
    '#weight' => 100,
  );
  $form['buttons']['back'] = array(
    '#type' => 'submit',
    '#value' => t('« Back'),
    '#theme' => 'finder_wizard_form_button',
    '#attributes' => array(
      'class' => 'finder-wizard-back',
    ),
  );
  $form['buttons']['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next »'),
    '#theme' => 'finder_wizard_form_button',
    '#attributes' => array(
      'class' => 'finder-wizard-next',
    ),
  );
  if ($step <= 0) {
    $form['buttons']['back']['#disabled'] = TRUE;
    $form['buttons']['back']['#attributes']['class'] .= ' finder-wizard-back-disabled';
  }
  if (($step >= (count($finder_wizard->steps))) ||
    ($finder_wizard->early_exit && count($form_state['storage']['results']) <= 1 && $step != 0)) {
    finder_wizard_form_results($form_state, $form, $finder_wizard);
  }
  return $form;
}

/**
 * Validate function for the form on the wizard page.
 */
function finder_wizard_form_validate($form, &$form_state) {
  $step = &$form_state['storage']['step'];
  $finder_wizard = &$form_state['storage']['finder_wizard'];
  if ($form_state['clicked_button']['#id'] == 'edit-next') {
    if (!$form_state['values'][$step] && $form_state['values'][$step] !== '0') {
      form_set_error($step, t("Please complete the form before continuing to the next step."));
      return false;
    }
  }
  if (isset($form_state['values'][$step])) {
    finder_wizard_alter_answer($form_state);
  }
}

/**
 * Submit function for the form on the wizard page.
 */
function finder_wizard_form_submit($form, &$form_state) {
  $step = &$form_state['storage']['step'];

  $finder_wizard = &$form_state['storage']['finder_wizard'];
  $form_state['storage']['values'][$step] = $form_state['values'];
  $step_settings = finder_wizard_get_step_settings($form_state);
  $element = &$step_settings['element'];
  $answer = &$form_state['storage']['answers'][$step][$form_state['values'][$step]];
  finder_wizard_form_responses($step_settings['response'], $step_settings['fwsid'], $answer);
  if ($form_state['clicked_button']['#id'] == 'edit-back') {
    $form_state['storage']['step']--;
  }
  elseif ($form_state['clicked_button']['#id'] == 'edit-next') {
    if ($form_state['values'][$step] === '0' && $step_settings['none_above'] == 1) {
      drupal_goto($step_settings['none_above_path']);
    }
    $form_state['storage']['step']++;
  }
  //to do: is this following bit required?
  if ($step >= (count($finder_wizard->steps))) {
    // form finished.
    $form_state['storage']['results'] = finder_wizard_get_results($form_state);
  }
}

/**
 * Set and get the current step
 */
function finder_wizard_form_step($step = NULL) {
  static $current_step;
  if (!is_null($step)) {
    $current_step = $step;
  }
  return $current_step;
}

/**
 * Set and get the step responses
 */
function finder_wizard_form_responses($response = NULL, $response_step_id = NULL, $answer = NULL, $reset = TRUE) {
  if (!is_null($response) && !is_null($response_step_id) && !is_null($answer)) {
    if (!isset($_SESSION['finder_wizard_responses'])) {
      $_SESSION['finder_wizard_responses'] = array();
    }
    if (!in_array($response, $_SESSION['finder_wizard_responses'])) {
      $_SESSION['finder_wizard_responses'][$response_step_id] = str_replace("!answer", $answer, $response);
    }
  }
  if (isset($_SESSION['finder_wizard_responses'])) {
    $output = theme('finder_wizard_form_responses', $_SESSION['finder_wizard_responses']);
    if ($reset && is_null($response) && is_null($response_step_id) && is_null($answer)) {
      unset($_SESSION['finder_wizard_responses']);
    }
    return $output;
  }
  return NULL;
}

/**
 * Do wizard results
 * make changes to $form, or go to node page.
 */
function finder_wizard_form_results(&$form_state, &$form, $finder_wizard) {
  $results = &$form_state['storage']['results'];
  if (count($results) == 1 && !$finder_wizard->results_page) {
    drupal_goto('node/'. current(array_keys($results)));
  }
  else {
    $form['results'] = array(
      '#type' => 'markup',
      '#prefix' => "<div class='finder-wizard-results'>",
      '#suffix' => "</div>",
      '#weight' => 0,
      '#value' => theme('finder_wizard_form_results_page', $results, $form_state),
    );
    unset($form[$form_state['storage']['step']]);
    $form['buttons']['next']['#disabled'] = TRUE;
    $form['buttons']['next']['#attributes']['class'] .= ' finder-wizard-next-disabled';
  }
}
