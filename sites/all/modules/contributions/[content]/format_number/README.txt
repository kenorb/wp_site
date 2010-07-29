;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Format Number module for Drupal
;; $Id: README.txt,v 1.1.2.4 2008/11/13 13:25:57 markuspetrux Exp $
;;
;; Original author: markus_petrux at drupal.org (October 2008)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS
========
* OVERVIEW
* INSTALLATION
* PHP API
* JAVASCRIPT API


OVERVIEW
========

This module provides a method to configure number formats (site default and user
defined) with configurable decimal point and thousand separators.

The function <code>format_number($number, $decimals = 0)</code> can be used by
other contributed or custom modules to display numbers accordingly.

External references:
- http://www.php.net/number_format
- http://www.unicode.org/reports/tr35/tr35-11.html
- http://www.unicode.org/cldr/data/charts/by_type/number.symbol.html
- http://en.wikipedia.org/wiki/Decimal_separator


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.

- Optionally goto Administer > User management > Permissions to assign the
  "configure default number format" permission to roles of your choice.

- Goto Administer > Site configuration > Number format to configure default
  number format for the site. You may also allow users to set override these
  options from their profiles.

- When user-configurable option is enabled, a new fieldset (Number format
  settings) is available when the user profile is edited.


PHP API
=======

- Other modules may use the following functions exposed by this module:

/**
 * Format a number with (site default or user defined) thousands separator and
 * decimal point.
 *
 * @param float $number
 *   The number being formatted.
 * @param int $decimals
 *   Number of decimal digits. Use -1 for any number if decimals.
 * @return string
 *   The formatted number.
 */
function format_number($number, $decimals = 0) {}

/**
 * Parse a formatted number.
 *
 * This function implements lenient parsing when possible, and only falls
 * back to site/user defined symbols when in doubt.
 * See http://www.unicode.org/reports/tr35/tr35-11.html#Lenient_Parsing
 *
 * @todo
 *  The algorithm probably needs optimization (using regular expressions?).
 *
 * @param string $formatted_number
 *   A number formatted with localized thousands separator and decimal point.
 * @param boolean $required
 *   If input is empty string, return FALSE when number is strictly required,
 *   otherwise an empty string is returned as 0.
 * @return number
 *   A valid PHP number. FALSE when input cannot be deciphered.
 */
function parse_formatted_number($formatted_number, $required = TRUE) {}

/**
 * Get the site/user defined thousands separator and decimal point characters.
 *
 * @param string $name
 *   The name of the option to retrieve (optional). Available options:
 *   - 'thousands_sep'  A one character string (it could be empty).
 *   - 'decimal_point'  A one character string.
 * @return mixed
 *   If name is not specified, an array with all options is returned.
 *   If name does not exist, NULL is returned.
 *   If name exists, its value is returned.
 */
function format_number_get_options($name = NULL) {}

/**
 * Expose a javascript version of the Format Number API.
 */
function format_number_add_js() {}


JAVASCRIPT API
==============

/**
 * Format a number with (site default or user defined) thousands separator
 * and decimal point.
 *
 * Formatting options are expected to be at Drupal.settings.format_number.
 *
 * @param float number
 *   The number being formatted.
 * @param int decimals
 *   Number of decimal digits. Use -1 for any number of decimals.
 * @return string
 *   The formatted number.
 */
Drupal.formatNumber = function(number, decimals) {}

/**
 * Parse a number with (site default or user defined) thousands separator
 * and decimal point.
 *
 * Formatting options are expected to be at Drupal.settings.format_number.
 *
 * @param string number
 *   A number formatted with localized thousands separator and decimal point.
 * @param boolean required
 *   FALSE to return "" when input is empty string. Otherwise, result is always
 *   returned as a valid number. Default is TRUE.
 * @return number
 *   A valid number.
 */
Drupal.parseNumber = function(number, required) {}
