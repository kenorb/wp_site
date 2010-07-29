<?php
// $Id: translation_overview.admin.inc,v 1.1 2008/09/11 23:21:07 drewish Exp $

/**
 * Settings form.
 */
function translation_overview_admin_settings_form($form_state) {
  $management = variable_get('translation_overview_management', array());

  $form['translation_overview_management']['#tree'] = TRUE;

  $form['#roles'] = user_roles(TRUE, 'manage translation overview priorities');

  foreach (locale_language_list() as $lang_code => $language) {
    $form['translation_overview_management'][$lang_code]['name'] = array(
      '#value' => check_plain($language),
    );
    foreach ($form['#roles'] as $rid => $role) {
      $form['translation_overview_management'][$lang_code][$rid] = array(
        '#type' => 'checkbox',
        '#default_value' => isset($management[$lang_code][$rid]) ? $management[$lang_code][$rid] : FALSE,
      );
    }
  }

  $form = system_settings_form($form);
  $form['#theme'] = 'translation_overview_admin_settings_form';
  return $form;
}

function theme_translation_overview_admin_settings_form($form) {
  $header = array(t('Language'));
  foreach($form['#roles'] as $rid => $role) {
    $header[] = $role;
  }
  $rows = array();
  foreach (element_children($form['translation_overview_management']) as $lang_code) {
    $row = array();
    foreach (element_children($form['translation_overview_management'][$lang_code]) as $rid) {
      $row[] = drupal_render($form['translation_overview_management'][$lang_code][$rid]);
    }
    $rows[] = $row;
  }

  return theme('table', $header, $rows, array(), t('Who manages the languages')) . drupal_render($form);
}