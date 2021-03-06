<?php
// $Id: admin.inc,v 1.1.2.37 2009/01/11 15:07:06 pasqualle Exp $

/**
 * Page callback for quicktabs admin landing page.
 */
function quicktabs_list() {
  $result = db_query('SELECT qtid, title FROM {quicktabs} ORDER BY title');
  $header = array(
    array('data' => t('Quicktab')),
    array('data' => t('Operations'), 'colspan' => 3),
  );
  $rows = array();
  while ($row = db_fetch_object($result)) {
    $tablerow = array(
      array('data' => $row->title),
      array('data' => l('Edit', 'admin/build/quicktabs/'. $row->qtid .'/edit')),
      array('data' => l('Clone', 'admin/build/quicktabs/'. $row->qtid .'/clone')),
      array('data' => l('Delete', 'admin/build/quicktabs/'. $row->qtid .'/delete')),
    );
    $rows[] = $tablerow;
  }
  if (!empty($rows)) {
    $output = theme('table', $header, $rows, array('id' => 'quicktabs'));
  }
  else {
    $output = '<p>'. t('Click on the "New QT block" tab to get started.') .'</p>';
  }
  return $output;
}

/**
 * Clone QuickTabs.
 */
function quicktabs_clone($quicktabs) {
  $quicktabs['qtid'] = 0;
  $quicktabs['title'] = '';
  return drupal_get_form('quicktabs_form', 'clone', $quicktabs);
}

/**
 * Build the quicktab creation and edit form.
 */
function quicktabs_form($form_state, $formtype, $quicktabs = array()) {
  $form = _qt_admin_main_form($form_state, $quicktabs);

  if ($formtype == 'edit') {
    $form['qtid'] = array(
      '#type' => 'hidden',
      '#value' => isset($quicktabs['qtid']) ? $quicktabs['qtid'] : 0,
    );
  }
  if (empty($quicktabs['tabs'])) {
    $quicktabs['tabs'] = array(
      0 => array(),
      1 => array(),
    );
  }

  // Add the current tabs to the form.
  foreach ($quicktabs['tabs'] as $delta => $details) {
    $details['delta'] = $delta;
    $form['qt_wrapper']['tabs'][$delta] = _quicktabs_form($details);
  }

  return $form;
}

/*
 * The main section of admin page.
 */
function _qt_admin_main_form($form_state, &$quicktabs) {
  $form['#cache'] = TRUE;

  // the contents of $quicktabs will either come from the db or from $form_state
  if (isset($form_state['quicktabs'])) {
    $quicktabs = $form_state['quicktabs'] + (array)$quicktabs;
  }

  $form['title'] = array(
  '#title' => t('Block title'),
  '#type' => 'textfield',
  '#description' => t('This will appear as the name of this block in administer >> site building >> blocks.'),
  '#default_value' => (isset($quicktabs['title']) ? $quicktabs['title'] : ''),
  '#weight' => -7
  );

  $styles = module_invoke_all('quicktabs_tabstyles');
  // The keys used for options must be valid html id-s.
  foreach ($styles as $style) {
    $style_options[$style] = $style;
  }
  ksort($style_options);
  $form['style'] = array(
    '#type' => 'select',
    '#title' => t('Style'),
    '#options' => array('nostyle' => t('No style')) + array('default' => t('Default style')) + $style_options,
    '#default_value' => isset($quicktabs['style']) ? $quicktabs['style'] : 'default',
    '#description' => t('Choose the quicktab style.'),
    '#weight' => -6,
  );

  $form['ajax'] = array(
    '#type' => 'radios',
    '#title' => t('Ajax'),
    '#options' => array(
      TRUE => t('Yes') .': '. t('Tab content will be loaded at tab click.'),
      FALSE => t('No') .': '. t('All tabs will be loaded at page load.'),
    ),
    '#default_value' => isset($quicktabs['ajax']) ? $quicktabs['ajax'] : 0,
    '#description' => t('Choose how the content of tabs should be loaded.'),
    '#weight' => -5,
  );

  // Add a wrapper for the tabs and Add Another Tab button.
  $form['qt_wrapper'] = array(
    '#tree' => FALSE,
    '#weight' => -4,
    '#prefix' => '<div class="clear-block" id="quicktabs-tabs-wrapper">',
    '#suffix' => '</div>',
  );

  $form['qt_wrapper']['tabs'] = array(
    '#tree' => TRUE,
    '#prefix' => '<div id="quicktabs-tabs">',
    '#suffix' => '</div>',
    '#theme' => 'quicktabs_admin_form_tabs',
  );

  $form['qt_wrapper']['tabs_more'] = array(
    '#type' => 'submit',
    '#prefix' => '<div id="add-more-tabs-button">',
    '#suffix' => '<label for="edit-tabs-more">'. t('Add tab') .'</label></div>',
    '#value' => t('More tabs'),
    '#attributes' => array('class' => 'add-tab', 'title' => t('Click here to add more tabs.')),
    '#weight' => 1,
    '#submit' => array('qt_more_tabs_submit'), // If no javascript action.
    '#ahah' => array(
      'path' => 'quicktabs/ahah',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['submit_form'] = array(
    '#type' => 'submit',
    '#weight' => 10,
    '#value' => t('Save'),
  );

  return $form;
}

/*
 * Build one row (one tabpage) on the QT admin form.
 */
function _quicktabs_form($details) {
  $form['#tree'] = TRUE;
  $delta = $details['delta'];

  $form['weight'] = array(
    '#type' => 'weight',
    '#default_value' => isset($details['weight']) ? $details['weight'] : $delta-10,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#size' => '10',
    '#default_value' => isset($details['title']) ? $details['title'] : '',
  );

  $tabtypes = array(
    'block' => t('Block'),
    'node' => t('Node'),
    'qtabs' => t('QTab'),
  );

  if (module_exists('views')) {
    $views = quicktabs_get_views();
    $views_keys = array_keys($views);
    $tabtypes['view'] = t('View');

    $selected_view = (isset($details['vid']) ? $details['vid'] : (isset($views_keys[0]) ? $views_keys[0] : ''));
    $form['view']['vid'] = array(
      '#type' => 'select',
      '#options' => $views,
      '#default_value' => $selected_view,
      '#title' => t('Select a view'),
      '#ahah' => array(
        'path' => 'quicktabs/ahah/',
        'wrapper' => 'quicktabs-tabs',
        'method' => 'replace',
        'event' => 'change',
      ),
    );
    $form['view']['display'] = array(
      '#type' => 'select',
      '#title' => 'display',
      '#options' => _quicktabs_get_views_displays($selected_view),
      '#default_value' => isset($details['display']) ? $details['display'] : '',
    );
    $form['view']['args'] = array(
      '#type' => 'textfield',
      '#title' => 'arguments',
      '#size' => '40',
      '#required' => FALSE,
      '#default_value' => isset($details['args']) ? $details['args'] : '',
      '#description' => t('Provide a comma separated list of arguments to pass to the view.'),
    );
    $form['view']['get_displays'] = array(
      '#type' => 'submit',
      '#value' => 'vdisp_'. $delta,
      '#submit' => array('qt_get_displays_submit'),
    );
  }

  $form['block']['bid'] = array(
    '#type' => 'select',
    '#options' => quicktabs_get_blocks(),
    '#default_value' => isset($details['bid']) ? $details['bid'] : '',
    '#title' => t('Select a block'),
  );
  $form['block']['hide_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the title of this block'),
    '#default_value' => isset($details['hide_title']) ? $details['hide_title'] : 1,
  );

  $form['node']['nid'] = array(
    '#type' => 'textfield',
    '#title' => t('Node'),
    '#description' => t('The node ID of the node.'),
    '#maxlength' => 10,
    '#size' => 20,
    '#default_value' => isset($details['nid']) ? $details['nid'] : '',
  );
  $form['node']['teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Teaser view'),
    '#default_value' => isset($details['teaser']) ? $details['teaser'] : 0,
  );
  $form['node']['hide_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the title of this node'),
    '#default_value' => isset($details['hide_title']) ? $details['hide_title'] : 1,
  );

  $form['qtabs']['qtid'] = array(
    '#type' => 'textfield',
    '#title' => t('Quicktab'),
    '#description' => t('The quicktab ID.') .' '. t('Different styles may not work when putting an ajax quicktab inside ajax quicktab.'),
    '#maxlength' => 10,
    '#size' => 20,
    '#default_value' => isset($details['qtid']) ? $details['qtid'] : '',
  );

  $form['type'] = array(
    '#type' => 'radios',
    '#options' => $tabtypes,
    '#default_value' => isset($details['type']) ? $details['type'] : 'block',
  );

  $form['remove'] = array(
    '#type' => 'submit',
    '#prefix' => '<div>',
    '#suffix' => '<label for="edit-remove">'. t('Delete') .'</label></div>',
    '#value' => 'remove_'. $delta,
    '#attributes' => array('class' => 'delete-tab', 'title' => t('Click here to delete this tab.')),
    '#submit' => array('qt_remove_tab_submit'),
    '#ahah' => array(
      'path' => 'quicktabs/ahah',
      'wrapper' => 'quicktabs-tabs',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

/**
 * Theme function for quicktabs admin page.
 * Theme the form elements for the tabs as draggable table rows.
 *
 * @ingroup themeable
 */
function theme_quicktabs_admin_form_tabs($form) {
  drupal_add_tabledrag('qt-tablist-table', 'order', 'sibling', 'qt-tabs-weight');
  $rows = array();
  $headers = array(
    t('Tab title'),
    t('Tab weight'),
    t('Tab type'),
    t('Tab content'),
    t('Operations'),
  );

  foreach (element_children($form) as $key) {
    $form[$key]['weight']['#attributes']['class'] = 'qt-tabs-weight';
    // Build the table row.
    $row = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['title']), 'class' => 'qt-tab-title'),
        array('data' => drupal_render($form[$key]['weight']), 'class' => 'qt-tab-weight'),
        array('data' => drupal_render($form[$key]['type']), 'class' => 'qt-tab-type'),
        // tab content (only 1 tab content (block, node or view) will be displayed. see: quicktabs_form.js)
        array('data' => drupal_render($form[$key]['block']), 'class' => 'qt-tab-block-content'),
        array('data' => (module_exists('views') ? drupal_render($form[$key]['view']) : ''), 'class' => 'qt-tab-view-content'),
        array('data' => drupal_render($form[$key]['node']), 'class' => 'qt-tab-node-content'),
        array('data' => drupal_render($form[$key]['qtabs']), 'class' => 'qt-tab-qtabs-content'),
        array('data' => drupal_render($form[$key]['remove']), 'class' => 'qt-tab-remove'),
      ),
      'class' => 'draggable',
    );

    // Add additional attributes to the row, such as a class for this row.
    if (isset($form[$key]['#attributes'])) {
      $row = array_merge($row, $form[$key]['#attributes']);
    }
    $rows[] = $row;
  }

  $output = theme('table', $headers, $rows, array('id' => 'qt-tablist-table'));
  $output .= drupal_render($form);

  // Add our JS file, which has some Drupal core JS overrides, and ensures ahah behaviours get re-attached
  drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs_ahah.js', 'module', 'footer');
  drupal_add_js(drupal_get_path('module', 'quicktabs') .'/js/quicktabs_form.js');
  drupal_add_css(drupal_get_path('module', 'quicktabs') .'/css/quicktabs-admin.css');
  return $output;
}

/**
 * Submit handler for the "Add Tab" button.
 */
function qt_more_tabs_submit($form, &$form_state) {
  $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state);
  // Add an empty tab at the end.
  $quicktabs['tabs'][] = array();

  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "Remove Tab" button.
 */
function qt_remove_tab_submit($form, &$form_state) {
  // Get the tab delta for the clicked button.
  $delta = $form_state['clicked_button']['#parents'][1];
  // Copy the form_state because unset() does not work on variable
  // that is passed by reference.
  $form_state_copy = $form_state;
  unset($form_state_copy['values']['tabs'][$delta]);
  $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state_copy);

  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the Views drop down.
 */
function qt_get_displays_submit($form, &$form_state) {
  $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state);

  unset($form_state['submit_handlers']);
  form_execute_handlers('submit', $form, $form_state);
  $form_state['quicktabs'] = $quicktabs;
  $form_state['rebuild'] = TRUE;
}

/**
 * ahah callback.
 */
function quicktabs_ahah() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $qt_form = $form['qt_wrapper']['tabs'];
  unset($qt_form['#prefix'], $qt_form['#suffix']); // Prevent duplicate wrappers.
  $javascript = drupal_add_js(NULL, NULL, 'header');
  drupal_json(array(
    'status'   => TRUE,
    'data'     => theme('status_messages') . drupal_render($qt_form),
    'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
  ));
}

/**
 * Validation handler for quicktabs admin page.
 */
function quicktabs_form_validate($form, &$form_state) {
  // We don't want it to validate when we're just adding or removing tabs.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    if (empty($form_state['values']['title'])) {
      form_set_error('title', t('Title is required for the quicktab block.'));
    }
    else {
      if (!isset($form_state['values']['tabs'])) {
        form_set_error('', t('At least one tab should be created.'));
      }
      foreach ($form_state['values']['tabs'] as $j => $tab) {
        if (empty($tab['title'])) {
          form_set_error('tabs]['. $j .'][title', t('Title is required for each tab.'));
        }
        elseif ($tab['type'] == 'qtabs' && $tab['qtabs']['qtid'] == $form_state['values']['qtid']) {
          form_set_error('tabs]['. $j .'][qtabs][qtid', t('You cannot put a quicktab inside itself.'));
        }
      }
    }
  }
}

/**
 * Submit handler for quicktabs admin page.
 */
function quicktabs_form_submit($form, &$form_state) {
  // We don't want it to submit when we're just adding or removing tabs.
  if ($form_state['clicked_button']['#id'] == 'edit-submit-form') {
    $quicktabs = _quicktabs_convert_form_to_quicktabs($form_state);

    if ($quicktabs['qtid'] > 0) {
      quicktabs_updateblock($quicktabs);
      drupal_set_message(t('The quicktab block has been updated.'));
    }
    else {
      quicktabs_createblock($quicktabs);
      drupal_set_message(t('The quicktab block has been created.'));
    }
    drupal_goto('admin/build/quicktabs');
  }
}

/**
 * Insert db record.
 */
function quicktabs_createblock($quicktabs) {
  $quicktabs['tabs'] = serialize($quicktabs['tabs']);
  db_query("INSERT INTO {quicktabs} (ajax, title, tabs, style) VALUES(%d, '%s', '%s', '%s')", $quicktabs['ajax'], $quicktabs['title'], $quicktabs['tabs'], $quicktabs['style']);
}

/**
 * Update db record.
 */
function quicktabs_updateblock($quicktabs) {
  $quicktabs['tabs'] = serialize($quicktabs['tabs']);
  db_query("UPDATE {quicktabs} SET ajax = %d, title = '%s', tabs = '%s', style = '%s' WHERE qtid = %d", $quicktabs['ajax'], $quicktabs['title'], $quicktabs['tabs'], $quicktabs['style'], $quicktabs['qtid']);
}

/**
 * Deletion of quicktab block.
 */
function quicktabs_block_delete(&$form_state, $quicktabs) {
  $form['qtid'] = array('#type' => 'hidden', '#value' => $quicktabs['qtid']);
  $form['qt_name'] = array('#type' => 'hidden', '#value' => $quicktabs['title']);
  return confirm_form($form, t('Are you sure you want to delete the quicktab block %name?', array('%name' => $quicktabs['title'])), 'admin/build/quicktabs', '', t('Delete'), t('Cancel'));
}

/**
 * Submit handler for quicktab block deletion.
 */
function quicktabs_block_delete_submit($form, &$form_state) {
  db_query('DELETE FROM {quicktabs} WHERE qtid = %d', $form_state['values']['qtid']);
  drupal_set_message(t('The quicktab block %name has been removed.', array('%name' => $form_state['values']['qt_name'])));
  cache_clear_all();
  $form_state['redirect'] = 'admin/build/quicktabs';
};

/**
 * Callback function for admin/settings/quicktabs. Display the settings form.
 */
function quicktabs_settings() {
  $styles = module_invoke_all('quicktabs_tabstyles');
  // The keys used for options must be valid html id-s.
  // Removing the css file path, because that can't be used.
  foreach ($styles as $style) {
    $options[$style] = $style;
  }
  ksort($options);

  $form['quicktabs_tabstyle'] = array(
    '#type' => 'radios',
    '#title' => t('Quicktab styles'),
    '#options' => array('nostyle' => t('No style')) + $options,
    '#default_value' => variable_get('quicktabs_tabstyle', 'nostyle'),
    '#description' => t('Select the default style for quicktabs.'),
    '#attributes' => array('class' => 'quicktabs-tabstyles clear-block'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler for QuickTabs settings.
 */
function quicktabs_settings_submit($form, &$form_state) {
  variable_set('quicktabs_tabstyle', $form_state['values']['quicktabs_tabstyle']);
  drupal_set_message(t('The default quicktab style has been saved.'));
}

/**
 * Theme function for quicktabs settings page.
 *
 * @ingroup themeable
 */
function theme_quicktabs_settings($form) {
  drupal_add_css(drupal_get_path('module', 'quicktabs') .'/css/quicktabs-admin.css');

  $tabs['one'] = array('title' => t('One'), 'type' => 'freetext', 'text' => t('First tab'));
  $tabs['two'] = array('title' => t('Two'), 'type' => 'freetext', 'text' => t('Second tab'));
  $tabs['three'] = array('title' => t('Three'), 'type' => 'freetext', 'text' => t('Third tab'));
  $quicktabs['tabs'] = $tabs;
  $quicktabs['ajax'] = FALSE;

  // Preview for each style.
  foreach ($form['quicktabs_tabstyle']['#options'] as $style => $style_title) {
    $quicktabs['style'] = $style;
    $quicktabs['qtid'] = drupal_strtolower($style);
    $preview = '<div class="quicktabs-preview">'. theme('quicktabs', $quicktabs) .'</div>';
    $form['quicktabs_tabstyle'][$style]['#description'] = t('%style preview', array('%style' => $style_title)) .':<br />'. $preview;
  }
  return drupal_render($form);
}

/**
 * Helper function to get all views.
 */
function quicktabs_get_views() {
  $enabled_views = array();
  $views = views_get_all_views();

  foreach ($views as $view) {
    // Skip disabled views.
    if (!empty($views[$view->name]->disabled)) {
      continue;
    }
    $enabled_views[$view->name] = $view->name;
  }
  ksort($enabled_views);
  return $enabled_views;
}

/**
 * Helper function to get all blocks.
 */
function quicktabs_get_blocks() {
  static $blocksarray;
  if (empty($blocksarray)) {
    global $theme_key;
    init_theme();
    $result = db_query("SELECT * FROM {blocks} WHERE theme = '%s'", $theme_key);
    $old_blocks = array();
    while ($old_block = db_fetch_array($result)) {
      $old_blocks[$old_block['module']][$old_block['delta']] = $old_block;
    }
    $blocks = array();
    foreach (module_list() as $module) {
      $module_blocks = module_invoke($module, 'block', 'list');
      if ($module_blocks) {
        foreach ($module_blocks as $delta => $block) {
          if (empty($old_blocks[$module][$delta])) {
            // If it's a new block, add identifiers.
            $block['module'] = $module;
            $block['delta']  = $delta;
            $block['theme']  = $theme_key;
            $blocks[] = $block;
          }
          else {
            $old_blocks[$module][$delta]['info'] = $block['info'];
            // Add this block to the list of blocks we return.
            $blocks[] = $old_blocks[$module][$delta];
          }
        }
      }
    }
    $blocksarray = array();
    foreach ($blocks as $i => $block) {
      if ($block['module'] != 'quicktabs') {
        $key = $block['module'] .'_delta_'. $block['delta'];
        $blocksarray[$key] = $block['info'];
      }
    }
  }
  return $blocksarray;
}

/**
 * Helper function to get all view displays.
 */
function _quicktabs_get_views_displays($view_name) {
  $displays = array();
  if (empty($view_name)) {
    // No view.
    return $displays;
  }

  $views = views_get_all_views();
  $view = $views[$view_name];

  if (empty($view->display)) {
    // This view is broken.
    return $displays;
  }

  foreach ($view->display as $id => $display) {
    $displays[$id] = $id .': '. $display->display_title;
  }
  return $displays;
}

/**
 * Helper function to convert the data on admin form into quicktabs presentation.
 */
function _quicktabs_convert_form_to_quicktabs($form_state) {
  $formvalues_tabs = array();
  if (isset($form_state['values']['tabs'])) {
    foreach ($form_state['values']['tabs'] as $j => $tab) {
      $formvalues_tabs[$j] = $tab[$tab['type']];
      $formvalues_tabs[$j]['title'] = $tab['title'];
      $formvalues_tabs[$j]['weight'] = $tab['weight'];
      $formvalues_tabs[$j]['type'] = $tab['type'];
      $weight[$j] = $tab['weight'];
    }
    array_multisort($weight, SORT_ASC, $formvalues_tabs);
  }
  $quicktabs = array(
    'title' => $form_state['values']['title'],
    'ajax' => $form_state['values']['ajax'],
    'style' => $form_state['values']['style'],
    'tabs' => $formvalues_tabs,
  );

  if (isset($form_state['values']['qtid'])) {
    $quicktabs['qtid'] = $form_state['values']['qtid'];
  }
  else {
    $quicktabs['qtid'] = 0;
  }
  return $quicktabs;
}
