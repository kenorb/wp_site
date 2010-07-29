<?php
// $Id: node_import.inc,v 1.1.2.13 2008/12/12 08:10:25 robrechtj Exp $

/**
 * @file
 * Support file for node_import module.
 */

/**
 * Implementation of hook_node_import_values().
 */
function node_import_node_import_values($type, $defaults, $options, $fields, $preview) {
  return array(
    'node_import_build_mode' => $preview ? NODE_BUILD_PREVIEW : NODE_BUILD_NORMAL,
  );
}

/**
 * Implementation of hook_node_import_file_formats().
 */
function node_import_node_import_file_formats($op) {
  // This function lists the builtin record separators, field
  // separators, text delimiters, escape characters and predefined
  // file formats.
  switch ($op) {
    case 'record separators':
      return array(
        '<newline>' => t('Newline'),
      );

    case 'field separators':
      return array(
        ':' => t('Colon (:)'),
        ',' => t('Comma (,)'),
        '|' => t('Pipe (|)'),
        ';' => t('Semicolon (;)'),
        ' ' => t('Space ( )'),
        '<tab>' => t('Tab'),
      );

    case 'text delimiters':
      return array(
        '<none>' => t('None'),
        '"' => t('Double quote (")'),
        "'" => t("Single quote (')"),
      );

    case 'escape characters':
      return array(
        '<none>' => t('None'),
        '\\' => t('Backslash (\\)'),
        '"' => t('Double quote (")'),
        "'" => t("Single quote (')"),
      );

    case 'file formats':
      return array(
        'csv' => array(
          'title' => t('Comma Separated Values'),
          'record separator' => '<newline>',
          'field separator' => ',',
          'text delimiter' => '"',
          'escape character' => '"',
        ),
        'csv-excel' => array(
          'title' => t('Comma Separated Values from MS Excel'),
          'record separator' => '<newline>',
          'field separator' => ';',
          'text delimiter' => '"',
          'escape character' => '"',
        ),
        'tsv' => array(
          'title' => t('Tab Separated Values'),
          'record separator' => '<newline>',
          'field separator' => '<tab>',
          'text delimiter' => '<none>',
          'escape character' => '<none>',
        ),
      );
  }

  return array();
}

