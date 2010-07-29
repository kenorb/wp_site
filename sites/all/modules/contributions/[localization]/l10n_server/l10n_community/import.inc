<?php
// $Id: import.inc,v 1.1.2.5.2.3 2008/10/26 14:54:57 goba Exp $

/**
 * @file
 *   Gettext import for localization community.
 */

/**
 * User interface for the translation import screen.
 */
function l10n_community_import_page($langcode) {
  $languages = l10n_community_get_languages();
  if (($perm = l10n_community_get_permission($langcode)) == L10N_PERM_ALL) {
    drupal_set_title(t('Import translations to @language', array('@language' => $languages[$langcode]->name)));
  }
  else {
    drupal_set_title(t('Import suggestions to @language', array('@language' => $languages[$langcode]->name)));
  }
  // Add missing breadcrumb.
  drupal_set_breadcrumb(
    array(
      l(t('Home'), NULL),
      l(t('Translate projects'), 'translate'))    
  );
  return drupal_get_form('l10n_community_import_form', $langcode, $perm);
}

/**
 * Translation import form.
 */
function l10n_community_import_form($form_state, $langcode, $perm) {
  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['langcode'] = array(
    '#type' => 'value',
    '#value' => $langcode,
  );
  
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('Translation file'),
    '#size' => 50,
    '#description' => t('A Gettext Portable Object (.po) file to upload.'),
  );
  $form['is_suggestion'] = array(
    '#title' => t('Store as suggestions'),
    '#type' => 'checkbox',
    '#description' => t('Check if the translations you import need discussion. If checked, all translations in the imported file will be saved as suggestions.'),
  );
  // Restrict to suggestions if no permission to submit translations directly.
  if ($perm != L10N_PERM_ALL) {
    $form['is_suggestion']['#type'] = 'value';
    $form['is_suggestion']['#value'] = 1;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import')
  );
  return $form;
}

/**
 * Process the locale import form submission.
 */
function l10n_community_import_form_submit($form, &$form_state) {
  // Save file in the local file system.
  if ($file = file_save_upload('file')) {

    // Increase time limit for PO parsing if possible.
    if (!ini_get('safe_mode')) {
      @set_time_limit(240);
    }

    // Do the actual parsing on the local file.
    if (l10n_community_import($file, $form_state['values']['langcode'], $form_state['values']['is_suggestion'])) {
      // Get status report on what was done in the process.
      list($inserted, $updated, $unchanged, $suggested) = _l10n_community_import_one_string();
      drupal_set_message(t('The translation was successfully imported.'));
      l10n_community_update_message($inserted, $updated, $unchanged, $suggested, 0);
    }
  }
  else {
    drupal_set_message(t('File to import not found.'), 'error');
  }
}
 
/**
 * Parses a Gettext Portable Object file and saves strings.
 *
 * Modified version of _locale_import_po() from locale.inc:
 *   - does not support in-memory import ($op parameter)
 *   - calls _l10n_community_import_one_string() to save string
 *   - support storage of strings as suggestions
 *   - algorithm untouched
 *
 * @param $file
 *   Drupal file object corresponding to the PO file to import.
 * @param $langcode
 *   Language code.
 * @param $is_suggestion
 *   Strings uploaded are to be saved as suggestions (TRUE or FALSE).
 */
function l10n_community_import($file, $langcode, $is_suggestion) {
  include_once 'includes/locale.inc';
  
  $fd = fopen($file->filepath, "rb"); // File will get closed by PHP on return
  if (!$fd) {
    _locale_import_message('The translation import failed, because the file %filename could not be read.', $file);
    return FALSE;
  }

  $context = "COMMENT"; // Parser context: COMMENT, MSGID, MSGID_PLURAL, MSGSTR and MSGSTR_ARR
  $current = array();   // Current entry being read
  $plural = 0;          // Current plural form
  $lineno = 0;          // Current line

  while (!feof($fd)) {
    $line = fgets($fd, 10*1024); // A line should not be this long
    $lineno++;
    $line = trim(strtr($line, array("\\\n" => "")));

    if (!strncmp("#", $line, 1)) { // A comment
      if ($context == "COMMENT") { // Already in comment context: add
        $current["#"][] = substr($line, 1);
      }
      elseif (($context == "MSGSTR") || ($context == "MSGSTR_ARR")) { // End current entry, start a new one
        _l10n_community_import_one_string($current, $langcode, $is_suggestion);
        $current = array();
        $current["#"][] = substr($line, 1);
        $context = "COMMENT";
      }
      else { // Parse error
        _locale_import_message('The translation file %filename contains an error: "msgstr" was expected but not found on line %line.', $file, $lineno);
        return FALSE;
      }
    }
    elseif (!strncmp("msgid_plural", $line, 12)) {
      if ($context != "MSGID") { // Must be plural form for current entry
        _locale_import_message('The translation file %filename contains an error: "msgid_plural" was expected but not found on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 12));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgid"] = $current["msgid"] ."\0". $quoted;
      $context = "MSGID_PLURAL";
    }
    elseif (!strncmp("msgid", $line, 5)) {
      if ($context == "MSGSTR") {   // End current entry, start a new one
        _l10n_community_import_one_string($current, $langcode, $is_suggestion);
        $current = array();
      }
      elseif ($context == "MSGID") { // Already in this context? Parse error
        _locale_import_message('The translation file %filename contains an error: "msgid" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 5));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file,  $lineno);
        return FALSE;
      }
      $current["msgid"] = $quoted;
      $context = "MSGID";
    }
    elseif (!strncmp("msgstr[", $line, 7)) {
      if (($context != "MSGID") && ($context != "MSGID_PLURAL") && ($context != "MSGSTR_ARR")) { // Must come after msgid, msgid_plural, or msgstr[]
        _locale_import_message('The translation file %filename contains an error: "msgstr[]" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      if (strpos($line, "]") === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $frombracket = strstr($line, "[");
      $plural = substr($frombracket, 1, strpos($frombracket, "]") - 1);
      $line = trim(strstr($line, " "));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgstr"][$plural] = $quoted;
      $context = "MSGSTR_ARR";
    }
    elseif (!strncmp("msgstr", $line, 6)) {
      if ($context != "MSGID") {   // Should come just after a msgid block
        _locale_import_message('The translation file %filename contains an error: "msgstr" is unexpected on line %line.', $file, $lineno);
        return FALSE;
      }
      $line = trim(substr($line, 6));
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      $current["msgstr"] = $quoted;
      $context = "MSGSTR";
    }
    elseif ($line != "") {
      $quoted = _locale_import_parse_quoted($line);
      if ($quoted === FALSE) {
        _locale_import_message('The translation file %filename contains a syntax error on line %line.', $file, $lineno);
        return FALSE;
      }
      if (($context == "MSGID") || ($context == "MSGID_PLURAL")) {
        $current["msgid"] .= $quoted;
      }
      elseif ($context == "MSGSTR") {
        $current["msgstr"] .= $quoted;
      }
      elseif ($context == "MSGSTR_ARR") {
        $current["msgstr"][$plural] .= $quoted;
      }
      else {
        _locale_import_message('The translation file %filename contains an error: there is an unexpected string on line %line.', $file, $lineno);
        return FALSE;
      }
    }
  }

  // End of PO file, flush last entry
  if (($context == "MSGSTR") || ($context == "MSGSTR_ARR")) {
    _l10n_community_import_one_string($current, $langcode, $is_suggestion);
  }
  elseif ($context != "COMMENT") {
    _locale_import_message('The translation file %filename ended unexpectedly at line %line.', $file, $lineno);
    return FALSE;
  }
  
  return TRUE;
}

/**
 * Imports a string into the database.
 *
 * @param $value
 *   Details of the string stored.
 * @param $langcode
 *   Language to store the string in.
 * @param $is_suggestion
 *   TRUE if the string to store is a suggestion, FALSE otherwise.
 */
function _l10n_community_import_one_string($value = NULL, $langcode = NULL, $is_suggestion = FALSE) {
  global $user;
  
  static $inserted = 0;
  static $updated = 0;
  static $unchanged = 0;
  static $suggested = 0;

  if ($value == NULL) {
    // Result stats queried.
    return array($inserted, $updated, $unchanged, $suggested);
  }
  elseif (!empty($value['msgid']) && !empty($value['msgstr'])) {
    // We only save non-empty translations/suggestions.
    
    // If the comment array for this value contains the ', fuzzy' flag, then
    // mark this as a suggestion import in all cases. 
    $is_suggestion = ($is_suggestion ? TRUE : in_array(', fuzzy', $value['#'])); 
    
    // We use BINARY matching here to avoid case insensitively matching
    // strings like 'operations' and 'Operations'.
    if ($sid = db_result(db_query("SELECT sid FROM {l10n_community_string} WHERE BINARY value = '%s'", $value['msgid']))) {
      // We have this source string (otherwise we don't save anything).
      $translation = db_fetch_object(db_query("SELECT translation FROM {l10n_community_translation} WHERE sid = %d AND language = '%s' AND is_suggestion = 0 AND is_active = 1", $sid, $langcode));
      
      // Merge plural versions into one for saving values.
      $value['msgstr'] = is_array($value['msgstr']) ? join("\0", $value['msgstr']) : $value['msgstr'];

      if ($translation && (empty($translation->translation) || ($translation->translation != $value['msgstr']))) {
        // If we have an empty translation placeholder (ie. no translation yet,
        // but unresolved suggestions are there), or if we have some translation
        // already, which is different from what we save now, redirect the data
        // to be a suggestion instead. This allows conflict issues to be resolved.
        $is_suggestion = TRUE;
      }
      
      if ($is_suggestion || !$translation) {
        l10n_community_target_save($sid, $value['msgstr'], $langcode, $user->uid, $is_suggestion, $inserted, $updated, $unchanged, $suggested);
      }
      else {
        // We certainly did not update this one.
        $unchanged++;
      }
    }
  }
}
