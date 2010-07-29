<?php
// $Id: finder_wizard.theme.inc,v 1.1.2.3 2008/11/08 05:37:06 danielb Exp $

function theme_finder_wizard_admin_edit_steps_table($form) {
  drupal_add_css(drupal_get_path('module', 'finder_wizard') .'/finder_wizard.css', 'module', 'all', FALSE);
  $children = element_children($form);
  if (!empty($children)) {
    $css_id = 'finder-wizard-admin-edit-steps-table';
    $css_class = 'finder-wizard-admin-edit-steps-table-order';
    drupal_add_tabledrag($css_id, 'order', 'sibling', $css_class);
    $rows = array();
    $headers = array('Step', 'Weight', 'Operations');
    foreach ((array)$children as $key) {
      $value = &$form[$key];
      $value['weight']['#attributes']['class'] = $css_class;
      $rows[] = array(
        'data' => array(
          drupal_render($value['value']), 
          drupal_render($value['weight']),
          drupal_render($value['ops']),
        ),
        'class' => 'draggable',
      );
    }
    $output = theme('table', $headers, $rows, array('id' => $css_id));
  }
  else {
    $output = t("There are no items to display");
  }
  return theme('item', array('#title' => 'Steps', '#value' => $output));
}


function theme_finder_wizard_form_page($finder_wizard, $form, $step) {
  drupal_add_css(drupal_get_path('module', 'finder_wizard') .'/finder_wizard.css', 'module', 'all', FALSE);
  drupal_set_title(check_plain($finder_wizard->title));
  $output = "<div class='finder-wizard finder-wizard-". $finder_wizard->fwid ."'>";
  $output .= "<div class='finder-wizard-step finder-wizard-step-". $step ."'>";
  $output .= "<div class='finder-wizard-header'>";
  $responses = finder_wizard_form_responses();
  if ($responses) {
    $output .= $responses;
  }
  if ($finder_wizard->steps[$step]['question']) {
    $output .= "<div class='finder-wizard-question finder-wizard-question-". $finder_wizard->steps[$step]['fwsid'] ."'>";
    $output .= check_markup($finder_wizard->steps[$step]['question']);
    $output .= "</div>";
  }
  $output .= "</div>";
  $output .= "<div class='finder-wizard-form'>";
  if ($form) {
    $output .= $form;
  }
  $output .= "</div>";
  $output .= "</div>";
  $output .= "</div>";
  return $output;
}


function theme_finder_wizard_form_button($element) {
  // Make sure not to overwrite classes.
  if (isset($element['#attributes']['class'])) {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'] .' '. $element['#attributes']['class'];
  }
  else {
    $element['#attributes']['class'] = 'form-'. $element['#button_type'];
  }
  return '<input type="submit" '. (empty($element['#name']) ? '' : 'name="'. $element['#name'] .'" ') .'id="'. $element['#id'] .'" value="'. $element['#value'] .'" '. drupal_attributes($element['#attributes']) ." />\n";
}


function theme_finder_wizard_form_description($description) {
  $output = "";
  $output .= "<div class='description'>";
  $output .= $description;
  $output .= "</div>";
  return $output;
}

function theme_finder_wizard_form_responses($responses) {
  $output = "";
  $output .= "<div class='finder-wizard-responses finder-wizard-responses-from-step-". ($step-1) ."'>";
  foreach ($responses as $response_step_id => $response) {
    $output .= "<div class='finder-wizard-response finder-wizard-response-". ($response_step_id) ."'>";
    $output .= check_markup($response);
    $output .= "</div>";
  }
  $output .= "</div>";
  return $output;
}

function theme_finder_wizard_form_results_page($results, $form_state) {

  $limit = 10;
  $quantity = count($results);
  $nids = array_keys((array)$results);
  $finder_wizard = &$form_state['storage']['finder_wizard'];

  if (empty($nids)) {
    $output .= t('There are no results to display.');
  }
  else {
    foreach ($nids as $nid) {
      $result = node_load($nid);
      $output .= node_view($result, TRUE);
    }
  }

  if (is_numeric($limit) && $limit > 0) {
    $output .= theme('pager', null, $limit, 0, array(), $quantity);
  }

  return $output;

}