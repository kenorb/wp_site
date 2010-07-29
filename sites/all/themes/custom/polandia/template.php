<?php
function theme076_menu_local_task($link, $active = FALSE) {
  return '<li '. ($active ? 'class="active" ' : '') .'><span><span>'. $link ."</span></span></li>\n";
}

/**
* Override node form
*/
function theme076_node_form($form) {
    global $user;
    // Remove 'Log message' text area
//drupal_set_message(print_r($user,true));
    $form['log']['#access'] = FALSE;
    return drupal_render($form);
}

/*
 * Now open your template.php and put this code in _phptemplate_variables(), where $op = 'page'.
 *
$current_panel = panels_page_get_current();
  if (!isset($current_panel->pid)) {
    // Load our default panel, call it by name.
    $panel = panels_page_view_page('DEFAULT', false);
    // Insert the actual content of the page using string replace.
    $panel = str_replace('%CONTENT%', $vars['content'], $panel);
    // Replace the page content with our altered panel.
    $vars['content'] = $panel;
  }
http://drupal.org/node/232644

*/

/*
function phptemplate_menu_item($mid, $children = '', $leaf = TRUE) {
  $link = menu_item_link($mid);
  $css_id = strtolower(str_replace(' ', '_', strip_tags($link)));
  return '<li id="' . $css_id . '" class="' . ($leaf ? 'leaf' : ($children ? 'expanded' : 'collapsed')) .'">'. $link . $children ."</li>\n";
}
 */
