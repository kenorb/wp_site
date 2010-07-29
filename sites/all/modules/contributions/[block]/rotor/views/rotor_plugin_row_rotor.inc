<?php
// $Id: rotor_plugin_row_rotor.inc,v 1.1.2.1 2008/12/20 17:53:55 mrfelton Exp $

/**
 * @file
 * GMap style plugin.
 */

/**
 * Style plugin to render a map.
 *
 * @ingroup views_style_plugins
 */
class rotor_plugin_row_rotor extends views_plugin_row {
  /**
   * Set default options
   */
  function option_definition() {
    return array();
  }

  function render($row) {
    return theme($this->theme_functions(), $this->view, $this->options, $row);
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    return array();
  }
}
