<?php
// $Id$
/**
 * @file
 * Generate array of color values for the imcontrol module.
 */

/**
 * Edit the default color values as required by your site
 */
function _imcontrol_default_colors($select) {
  switch ($select) {
    case 'blue':
      $color = array('333333', 'E8F1F8', '333333', 'AFD3EB', '333333', 'EEF7FE', 'AFD3EB', 'EEF7FE', '0066A7', '8EBBD8', '333333', 'FFFFFF', '333333', 'FFFFFF', 'F6F6F6', '760502', '6E6C6C');
      break;
    case 'green':
      $color = array('333333', 'DCF2E5', '333333', '8ED4AB', '2C0034', 'CFE9D9', '8ED4AB', 'CFE9D9', '006629', '92D6AE', '006629', 'FFFFFF', '333333', 'F4FBF7', 'F6F6F6', '760502', '6E6C6C');
      break;
    case 'pink':
      $color = array('444444', 'FFD5D5', '444444', 'ED7B7B', 'AA3636', 'FAD6D6', 'AA3636', 'FAD6D6', '444444', 'F9A3A3', 'E45A5A', 'FFFFFF', '444444', 'FEF6F6', 'F6F6F6', '760502', '6E6C6C');
      break;
    case 'orange':
      $color = array('333333', 'FDC098', '333333', 'FB8233', '333333', 'FFC9A5', 'FB8233', 'FFC9A5', '333333', 'FC9E60', '333333', 'FFFFFF', '333333', 'FFFFFF', 'F6F6F6', '760502', '6E6C6C');
      break;
    case 'purple':
      $color = array('333333', 'F1EFF4', '333333', 'AFA9B4', '333333', 'DED6DE', 'AFA9B4', 'DED6DE', '513663', 'AEA1B9', '333333', 'FFFFFF', '333333', 'FFFFFF', 'F6F6F6', '760502', '6E6C6C');
      break;
    case 'black':
      $color = array('676769', 'DBDBDB', '444444', '8D8D8D', '99CC33', '676769', '99CC33', 'F1F1F1', '729527', 'B2B2B2', '676769', 'BBBBBB', '99CC33', 'EAEAEA', 'B2B2B2', '760502', '6E6C6C');
      break;
    default:
      $color = array('', '', '', '', '', '', '', '', '', '', '333333', 'FFFFFF', '333333', 'FFFFFF', 'F6F6F6', '760502', ''); // Create your own default
  }
  return $color;
}