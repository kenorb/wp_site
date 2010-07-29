<?php
// $Id: theme.inc,v 1.1.2.1 2008/12/20 17:53:55 mrfelton Exp $

function template_preprocess_rotor_view_rotor(&$vars) {
  $view = $vars['view'];
  $vars['name'] = $view->name;
  $vars['display_id'] = $view->current_display;
}
