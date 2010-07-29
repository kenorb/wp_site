<?php
// $Id: gallery_install.inc,v 1.3.2.10 2008/08/04 21:13:06 profix898 Exp $

require_once(drupal_get_path('module', 'gallery') .'/gallery_settings.inc');

/**
 * gallery.module : gallery_install.inc
 * Configure embedded Gallery install (directory options, etc.)
 */

define('GALLERY_INSTALL_STEP_PHPMEMORY',  1);
define('GALLERY_INSTALL_STEP_LOCATIONS',  2);
define('GALLERY_INSTALL_STEP_PLUGINS',    3);
define('GALLERY_INSTALL_STEP_URLREWRITE', 4);
define('GALLERY_INSTALL_STEP_USERSYNC',   5);

/**
 * Function _gallery_install_status_init().
 */
function _gallery_install_status_init() {
  return array(
    'phpmemory' => array(
      'title' => t('PHP Memory Limit'),
      'severity' => GALLERY_SEVERITY_ERROR,
      'status' => FALSE
    ),
    'locations' => array(
      'title' => t('Gallery2 locations'),
      'severity' => GALLERY_SEVERITY_ERROR,
      'status' => FALSE
    ),
    'plugins' => array(
      'title' => t('Drupal Modules / Gallery2 Plugins'),
      'severity' => GALLERY_SEVERITY_WARNING,
      'status' => FALSE
    ),
    'urlrewrite' => array(
      'title' => t('Clean URL / URL Rewrite'),
      'severity' => GALLERY_SEVERITY_WARNING,
      'status' => FALSE
    ),
    'usersync' => array(
      'title' => t('Initial User Synchronization'),
      'severity' => GALLERY_SEVERITY_ERROR,
      'status' => FALSE
    ),
  );
}

/**
 * Function _gallery_install_status_report().
 */
function _gallery_install_status_report($install_status) {
  // Install status table
  $error = FALSE;
  $rows = array();
  foreach ($install_status as $step => $status) {
    // Find the combination of status and severity of the plugin
    $severity = ($status['status'] <= 0) ? $status['severity'] : GALLERY_SEVERITY_SUCCESS;
    if (!$status['status'] && $severity == GALLERY_SEVERITY_ERROR) {
      $error = TRUE;
    }
    $row = array();
    $row[] = array('data' => $status['title'], 'align' => 'left', 'width' => '20%');
    $row[] = array('data' => theme('gallery_severity_message', $severity), 'align' => 'center');
    $row[] = array('data' => isset($status['info'])? $status['info'] : '', 'class' => 'description', 'id' => 'gallery-install-status-'. $step);
    $rows[] = $row;
  }
  
  if ($error) {
    $rows[] = array(array(
      'data' => t('Critical errors are present.'),
      'colspan' => '3',
      'align' => 'center',
      'class' => 'message')
    );
  }

  // Set the global status variable 'gallery_valid'
  gallery_set_status(array(
    'gallery_valid' => array(
      'title' => t('Overall Status (Installation)'),
      'severity' => $error ? GALLERY_SEVERITY_ERROR : GALLERY_SEVERITY_SUCCESS
    )
  ));
  
  return theme('table', array(t('Step'), t('Status'), t('Description')), $rows, array('class' => 'package'));
}

/**
 * Function _gallery_install_status().
 */
function _gallery_install_status($form_state) {
  $form['install'] = array(
    '#type' => 'fieldset',
    '#title' => t('Install status'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t('Correct configuration of the following items is essential for the
                         embedded gallery to function properly.')
  );
  
  $install_status = _gallery_install_status_init();

  // Step 1: Check PHP Memory
  $phpmemory_info = _gallery_php_memcheck();
  $install_status['phpmemory']['status'] = $phpmemory_info['status'];
  $install_status['phpmemory']['info'] = $phpmemory_info['info'];

  // Step 2: Gallery2 Locations
  $values['gallery_uri'] = variable_get('gallery_uri', '/gallery2/');
  $values['gallery_dir'] = variable_get('gallery_dir', './gallery2/');
  $values['gallery_embed_uri'] = variable_get('gallery_embed_uri', '?q=gallery');
  $values['gallery_base'] = variable_get('gallery_base', 'gallery');
  $autodetect = variable_get('gallery_autodetect_dir', 1);
  // Check location settings and generate status report
  list($install_status['locations']['status'], $values, $result) = _gallery_install_locations($values, $autodetect);
  $install_status['locations']['info'] = $install_status['locations']['status'] ?
                                         t('The Gallery2 location settings appear to be correct.') :
                                         gallery_format_status($result, t('Errors in your current Gallery2 location settings:'));
  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery2 location settings') .' - '. ($autodetect ? t('Auto Configuration') : t('Manual Configuration')),
    '#collapsible' => TRUE,
    '#collapsed' => $install_status['locations']['status'],
    '#description' => t('Overview of your Gallery2 location settings as specified or autodetected during install.')
  );
  $form['locations']['gallery_uri'] = array(
    '#title' => t('Gallery2 URL or URI'),
    '#type' => 'item',
    '#value' => $values['gallery_uri']
  );
  if (!empty($values['gallery_dir'])) {
    $form['locations']['gallery_dir'] = array(
      '#title' => t('Gallery2 filesystem path'),
      '#type' => 'item',
      '#value' => $values['gallery_dir']
    );
  }
  $form['locations']['gallery_embed_uri'] = array(
    '#title' => t('Embed URI'),
    '#type' => 'item',
    '#value' => $values['gallery_embed_uri']
  );
  $form['locations']['gallery_base'] = array(
    '#title' => t('Gallery base'),
    '#type' => 'item',
    '#value' => $values['gallery_base']
  );
  
  // Step 3: Drupal Modules and Gallery2 Plugins check
  $form += _gallery_install_plugins($install_status);
  
  // Step 4: Clean URL configuration
  _gallery_install_urlrewrite_check($install_status);
  if (gallery_single_plugin_status('rewrite') == GALLERY_PLUGIN_ENABLED) {
    if (_gallery_install_urlrewrite($public_path, $htaccess_path, TRUE)) {
      $form['urlrewrite'] = array(
        '#type' => 'fieldset',
        '#title' => t('Clean URLs / URL Rewrite settings'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#description' => t('Overview of your URL Rewrite settings as specified during install.')
      );
      $form['urlrewrite']['htaccess_public_path'] = array(
        '#title' => t('Public path to your .htaccess file'),
        '#type' => 'item',
        '#value' => $public_path
      );
      $form['urlrewrite']['htaccess_filesystem_path'] = array(
        '#title' => t('Filesystem path to your .htaccess file'),
        '#type' => 'item',
        '#value' => $htaccess_path
      );
    }
  }
  
  // Step 5: User Synchronization
  $install_status['usersync']['status'] = TRUE;
  $install_status['usersync']['info'] = t('Your initial user synchronization has been completed already.<br /> If you
                                           want to resync all your users or you want to import users from an existing
                                           Gallery2 setup, you can always use the <a href= "@gallery-usersync">Advanced
                                           Sync</a> options in the Gallery user administration.',
                                           array('@gallery-usersync' => url('admin/user/gallery/advanced')));
  
  $form['install']['status'] = array(
    '#value' => _gallery_install_status_report($install_status)
  );
  
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset & Restart'),
    '#submit' => array('_gallery_install_reset')
  );

  return $form;
}

/**
 * Function _gallery_install().
 */
function _gallery_install($form_state) {
  $form['install'] = array(
    '#type' => 'fieldset',
    '#title' => t('Install status'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => t(''),
  );
  
  if (!isset($form_state['storage'])) {
    $install_status = _gallery_install_status_init();
    $install_status['step'] = GALLERY_INSTALL_STEP_PHPMEMORY;
    // Check for advanced_help module
    if (!module_exists('advanced_help')) {
      drupal_set_message(t('If you install the <a href="@url">advanced help</a> module, Gallery module will provide more
                            and better help.', array('@url' => 'http://drupal.org/project/advanced_help')));
    }
    else {
      $install_help = theme('advanced_help_topic', 'gallery', 'install', t('Install Tutorial'));
      drupal_set_message(t('Dont know where to start? Try the "!install-help" page.', array('!install-help' => $install_help)));
    }
  }
  else {
    $install_status = $form_state['storage'];
  }
  
  // Step 1: Check PHP Memory
  _gallery_install_step_phpmemory($form, $form_state, $install_status);

  // Step 2 : Gallery2 Locations
  if ($install_status['step'] >= GALLERY_INSTALL_STEP_LOCATIONS) {
    _gallery_install_step_locations($form, $form_state, $install_status);
  }

  // Step 3: Drupal Modules and Gallery2 Plugins check
  if ($install_status['step'] >= GALLERY_INSTALL_STEP_PLUGINS) {
    _gallery_install_step_plugins($form, $form_state, $install_status);
  }

  // Step 4: Clean URL configuration
  if ($install_status['step'] >= GALLERY_INSTALL_STEP_URLREWRITE) {
    _gallery_install_step_urlrewrite($form, $form_state, $install_status);
  }

  // Step 5: User Synchronization
  if ($install_status['step'] >= GALLERY_INSTALL_STEP_USERSYNC) {
    _gallery_install_step_usersync($form, $form_state, $install_status);
  }

  $form['install']['status'] = array(
    '#value' => _gallery_install_status_report($install_status)
  );
  
  $form['install_status'] = array(
    '#type' => 'value',
    '#value' => $install_status
  );
  
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset & Restart'),
    '#submit' => array('_gallery_install_reset')
  );

  return $form;
}

/**
 * Function _gallery_install_reset().
 */
function _gallery_install_reset($form, &$form_state) {
  variable_del('gallery_autodetect_dir');
  variable_del('gallery_uri');
  variable_del('gallery_dir');
  variable_del('gallery_embed_uri');
  variable_del('gallery_valid');
  variable_del('gallery_rewrite_disable');
  menu_rebuild();

  unset($form_state['storage']);
  $form_state['redirect'] = 'admin/settings/gallery/install';
}

/**
 * Function _gallery_install_step_phpmemory().
 * Step 1: Check PHP Memory
 */
function _gallery_install_step_phpmemory(&$form, &$form_state, &$install_status) {
  if ($install_status['step'] == GALLERY_INSTALL_STEP_PHPMEMORY) {
    $phpmemory_info = _gallery_php_memcheck();
    $install_status['phpmemory']['status'] = $phpmemory_info['status'];
    $install_status['phpmemory']['info'] = $phpmemory_info['info'];
    if ($phpmemory_info['status']) {
      $install_status['step'] = GALLERY_INSTALL_STEP_LOCATIONS;
    }
  }
  
  $form['phpmemory'] = array(
    '#type' => 'fieldset',
    '#title' => t('Step 1: PHP Memory Test') . ($install_status['phpmemory']['status'] ? ' '. t('(OK)') : ''),
    '#description' => $install_status['phpmemory']['info'],
    '#collapsible' => ($install_status['step'] > GALLERY_INSTALL_STEP_PHPMEMORY),
    '#collapsed' => $install_status['phpmemory']['status'],
  );
  if (!$install_status['phpmemory']['status']) {
    $form['phpmemory']['#description'] .= ' '. t('Until this is fixed your Gallery2 cannot be installed.');
    $form['phpmemory']['check'] = array('#type' => 'submit', '#value' => t('Check PHP memory again'));
  }
}

/**
 * Function _gallery_install_step_locations().
 * Step 2: Gallery2 Locations
 */
function _gallery_install_step_locations(&$form, &$form_state, &$install_status) {
  $autodetect = variable_get('gallery_autodetect_dir', 1);
  $title = t('Step 2: Gallery2 location settings') .' - '. ($autodetect ? t('Auto Configuration') : t('Manual Configuration'));
  $desc = $install_status['locations']['status'] ? '' : $autodetect ?
      t('Enter the \'Gallery2 URL or URI\' and click \'Test location settings\'
         to automatically configure the settings needed for embedding Gallery2 into Drupal. Note that
         the auto-config will not work for all host configurations. If you have tried it with a value
         you know is correct and it did not work then just use the manual configuration instead.') :
      t('Enter the \'Gallery2 URL or URI\' and \'Gallery2 filesystem path\' and then click
         \'Test location settings\'. This will check and configure the settings needed for embedding
         Gallery2 into Drupal.');

  $form['locations'] = array(
    '#type' => 'fieldset',
    '#title' => $title . ($install_status['locations']['status'] ? ' '. t('(OK)') : ''),
    '#description' => isset($install_status['locations']['details']) ? ($desc .'<p>'. $install_status['locations']['details'] .'</p>') : $desc,
    '#collapsible' => ($install_status['step'] > GALLERY_INSTALL_STEP_LOCATIONS),
    '#collapsed' => $install_status['locations']['status']
  );

  if ($autodetect) {
    // Autodetect
    $gallery_uri = empty($form_state['values']['gallery_uri_auto']) ?
                   variable_get('gallery_uri', '/gallery2/') : $form_state['values']['gallery_uri_auto'];

    $gallery_dir = empty($form_state['values']['gallery_dir_auto']) ?
                   variable_get('gallery_dir', './gallery2/') : $form_state['values']['gallery_dir_auto'];

    $embed_uri = empty($form_state['values']['gallery_embed_uri_auto']) ?
                 variable_get('gallery_embed_uri', '/index.php?q=gallery') : $form_state['values']['gallery_embed_uri_auto'];

    $gallery_base = empty($form_state['values']['gallery_base']) ?
                    variable_get('gallery_base', 'gallery') : $form_state['values']['gallery_base'];

    $form['locations']['gallery_uri_auto'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery2 URL or URI'),
      '#default_value' => $gallery_uri,
      '#size' => 64,
      '#maxlength' => 128,
      '#description' => t('URL or URI of the G2 standalone location. This can be either the full URL to Gallery2
                           (e.g. http://www.example.com/gallery2) or the path from docroot to the Gallery2 directory
                           (e.g. /gallery2). If using a URI the protocol/hostname are both optional.'),
      '#disabled' => $install_status['locations']['status']
    );

    if ($install_status['locations']['status']) {
      $form['locations']['gallery_dir_auto_item'] = array(
        '#title' => t('Gallery2 filesystem path'),
        '#type' => 'item',
        '#value' => $gallery_dir
      );
    }
    $form['locations']['gallery_dir_auto'] = array(
      '#type' => 'value',
      '#value' => $gallery_dir
    );
    
    if ($install_status['locations']['status']) {
      $form['locations']['gallery_embed_uri_auto_item'] = array(
        '#title' => t('Embed URI'),
        '#type' => 'item',
        '#value' => $embed_uri
      );
    }
    $form['locations']['gallery_embed_uri_auto'] = array(
      '#type' => 'value',
      '#value' => $embed_uri
    );
    
    $form['locations']['gallery_base'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery base (optional)'),
      '#default_value' => $gallery_base,
      '#size' => 32,
      '#maxlength' => 64,
      '#description' => t('Drupal path to embedded Gallery. This option defaults to \'gallery\' and the embedded gallery
                           will be located at \'@default-base\'.', array('@default-base' => url('gallery'))),
      '#disabled' => $install_status['locations']['status']
    );
  }
  else {
    // No Autodetect (manual)
    $gallery_uri = empty($form_state['values']['gallery_uri_man']) ?
                   variable_get('gallery_uri', '/gallery2/') : $form_state['values']['gallery_uri_man'];

    $gallery_dir = empty($form_state['values']['gallery_dir_man']) ?
                   variable_get('gallery_dir', './gallery2/') : $form_state['values']['gallery_dir_man'];

    $embed_uri = empty($form_state['values']['gallery_embed_uri_man']) ?
                 variable_get('gallery_embed_uri', '/index.php?q=gallery') : $form_state['values']['gallery_embed_uri_man'];
      
    $gallery_base = empty($form_state['values']['gallery_base']) ?
                    variable_get('gallery_base', 'gallery') : $form_state['values']['gallery_base'];

    $form['locations']['gallery_uri_man'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery2 URL or URI'),
      '#default_value' => $gallery_uri,
      '#size' => 64,
      '#maxlength' => 128,
      '#description' => t('URL or URI of the G2 standalone location. This can be either the full URL to Gallery2
                           (e.g. http://www.example.com/gallery2) or the path from docroot to the Gallery2 directory
                           (e.g. /gallery2). If using a URI the protocol/hostname are both optional.'),
      '#disabled' => $install_status['locations']['status']
    );
    
    $form['locations']['gallery_dir_man'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery2 filesystem path'),
      '#default_value' => $gallery_dir,
      '#size' => 64,
      '#maxlength' => 128,
      '#description' => t('The filesystem path of your Gallery2 directory. This can be either an absolute path
                           (e.g. /home/mysite/htdocs/gallery2) or relative to the root directory of your Drupal
                           installation (e.g. ../gallery2).'),
      '#disabled' => $install_status['locations']['status']
    );
    
    $form['locations']['gallery_embed_uri_man'] = array(
      '#type' => 'textfield',
      '#title' => t('Embed URI'),
      '#default_value' => $embed_uri,
      '#size' => 64,
      '#maxlength' => 128,
      '#description' => t('URI to access G2 via Drupal. Don\'t worry if you are using clean urls in Drupal and this
                           still ends in \'index.php?q=gallery\'. This is needed to get the Gallery2 URL rewrite module
                           to work correctly and will not be visible.'),
      '#disabled' => $install_status['locations']['status']
    );
    
    $form['locations']['gallery_base'] = array(
      '#type' => 'textfield',
      '#title' => t('Gallery base (optional)'),
      '#default_value' => $gallery_base,
      '#size' => 32,
      '#maxlength' => 64,
      '#description' => t('Drupal path to embedded Gallery. This option defaults to \'gallery\' and the embedded gallery
                           will be located at \'@default-base\'.', array('@default-base' => url('gallery'))),
      '#disabled' => $install_status['locations']['status']
    );
  }
  
  if (!$install_status['locations']['status']) {
    $form['locations']['location_button'] = array(
      '#type' => 'submit',
      '#value' => t('Test location settings'),
      '#submit' => array('_gallery_install_step_locations_check')
    );
    $form['locations']['switch_config_button'] = array(
      '#type' => 'submit',
      '#value' => $autodetect ? t('Use Manual Configuration') : t('Use Auto Configuration'),
      '#submit' => array('_gallery_install_step_locations_switch')
    );
  }
}

/**
 * Function _gallery_install_step_locations_check().
 */
function _gallery_install_step_locations_check($form, &$form_state) {
  $install_status = $form_state['values']['install_status'];
  $autodetect = variable_get('gallery_autodetect_dir', 1);
  if ($autodetect) {
    $values['gallery_uri'] = $form_state['values']['gallery_uri_auto'];
    $values['gallery_base'] = empty($form_state['values']['gallery_base']) ? 'gallery' : $form_state['values']['gallery_base'];
  }
  else {
    $values['gallery_uri'] = $form_state['values']['gallery_uri_man'];
    $values['gallery_dir'] = $form_state['values']['gallery_dir_man'];
    $values['gallery_embed_uri'] = $form_state['values']['gallery_embed_uri_man'];
  }
  
  list($install_status['locations']['status'], $values, $result) = _gallery_install_locations($values, $autodetect);
  if (!$install_status['locations']['status']) {
    $install_status['locations']['details'] = gallery_format_status($result, t('Errors in your current Gallery2 location settings:'));
  }
  else {
    variable_set('gallery_uri', $values['gallery_uri']);
    variable_set('gallery_dir', $values['gallery_dir']);
    variable_set('gallery_embed_uri', $values['gallery_embed_uri']);
    variable_set('gallery_base', $values['gallery_base']);
    $install_status['locations']['details'] = '';
    $install_status['step'] = GALLERY_INSTALL_STEP_PLUGINS;
  }
  
  if ($autodetect) {
    $form_state['values']['gallery_uri_auto'] = $values['gallery_uri'];
    $form_state['values']['gallery_dir_auto'] = $values['gallery_dir'];
    $form_state['values']['gallery_embed_uri_auto'] = $values['gallery_embed_uri'];
  }
  else {
    $form_state['values']['gallery_uri_man'] = $values['gallery_uri'];
    $form_state['values']['gallery_dir_man'] = $values['gallery_dir'];
    $form_state['values']['gallery_embed_uri_man'] = $values['gallery_embed_uri'];
  }
  
  $install_status['locations']['info'] = $install_status['locations']['status'] ?
    t('The Gallery2 location settings appear to be correct.') :
    t('There is a problem with the Gallery2 location settings. Please check below and retest.
       Remember that the auto-config will not work for all hosts.');

  $form_state['storage'] = $install_status;
}

/**
 * Function _gallery_install_step_locations_switch().
 */
function _gallery_install_step_locations_switch($form, &$form_state) {
  unset($form_state['storage']);
  variable_set('gallery_autodetect_dir', !variable_get('gallery_autodetect_dir', 1));
}

/**
 * Function _gallery_install_step_plugins().
 * Step 3: Drupal Modules / Gallery2 Plugins
 */
function _gallery_install_step_plugins(&$form, &$form_state, &$install_status) {
  $title = t('Step 3: Drupal Modules / Gallery2 Plugins') . ($install_status['plugins']['status'] ? ' '. t('(OK)') : '');
  $form += _gallery_install_plugins($install_status, $title, ($install_status['step'] > GALLERY_INSTALL_STEP_PLUGINS));
  
  if ($install_status['step'] == GALLERY_INSTALL_STEP_PLUGINS) {
    if (!in_array($install_status['plugins']['status'], array(GALLERY_SEVERITY_ERROR, GALLERY_SEVERITY_WARNING))) {
      $install_status['step'] = GALLERY_INSTALL_STEP_URLREWRITE;
      $form['plugins']['#collapsed'] = TRUE;
    }
    else {
      $form['plugins']['plugins_button_desc'] = array(
        '#type' => 'item',
        '#value' => t('There are errors in your plugin configuration. Enable/Disable the
                       respective plugins to meet the requirements.')
      );
      $form['plugins']['plugins_button'] = array(
        '#type' => 'submit',
        '#value' => t('Check Plugins status again'),
        '#submit' => array('_gallery_install_step_plugins_check')
      );
    }
  }
}

/**
 * Function _gallery_install_step_plugins_check().
 */
function _gallery_install_step_plugins_check($form, &$form_state) {
  $install_status = $form_state['values']['install_status'];
  _gallery_init(TRUE, array(), FALSE);
  $form_state['storage'] = $install_status;
}

/**
 * Function _gallery_install_step_urlrewrite().
 * Step 4: Clean URL configuration
 */
function _gallery_install_step_urlrewrite(&$form, &$form_state, &$install_status) {
  $title = t('Step 4: Clean URLs / URL Rewrite settings') . ($install_status['urlrewrite']['status'] ? ' '. t('(OK)') : '');
  $form['urlrewrite'] = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => ($install_status['step'] > GALLERY_INSTALL_STEP_URLREWRITE),
    '#collapsed' => $install_status['urlrewrite']['status']
  );
  
  // User selected to skip the URLRewrite step
  if (variable_get('gallery_rewrite_disable', 0)) {
    $form['urlrewrite']['#description'] = $install_status['urlrewrite']['info'];
    $form['urlrewrite']['#description'] .= ' '. t('\'Reset & Restart\' your configuration to change these settings.');
    return;
  }
  
  $clean_urls_status = variable_get('clean_url', 0);
  $rewrite_status = gallery_single_plugin_status('rewrite');
  
  if ($clean_urls_status) {
    if ($rewrite_status < GALLERY_PLUGIN_ENABLED) {
      $form['urlrewrite']['#description'] = t('Clean URLs are enabled in Drupal, but the Gallery2 URL Rewrite plugin is unavailable (!status).',
                                            array('!status' => theme('gallery_plugin_status_message', $rewrite_status)));
      $form['urlrewrite']['check'] = array(
        '#type' => 'submit',
        '#value' => t('Check URL Rewrite status again'),
        '#submit' => array('_gallery_install_step_urlrewrite_check')
      );
    }
    else {
      // CleanURL and URLRewrite are both enabled
      $form['urlrewrite']['#description'] = t('Clean URLs are !clean_url_status in Drupal and the Gallery2 URL Rewrite plugin is
                                            !rewrite_status. It is possible to automatically configure the URL Rewrite plugin.
                                            The configuration assumes that the rules should be placed in your Drupal .htaccess
                                            file (it will add them to the beginning) which works in most applications. If you
                                            need a special configuration, enter the desired values manually.',
                                            array(
                                              '!clean_url_status' => theme('gallery_module_status_message', $clean_urls_status),
                                              '!rewrite_status' => theme('gallery_plugin_status_message', $rewrite_status)
                                            )
      );
      
      _gallery_install_urlrewrite($public_path, $htaccess_path, TRUE);
      $form['urlrewrite']['htaccess_public_path'] = array(
        '#type' => 'textfield',
        '#title' => t('Public path to your .htaccess file'),
        '#size' => 64,
        '#maxlength' => 255,
        '#default_value' => $public_path,
        '#description' => t('This is the location of your Drupal .htaccess file relative to your webserver document root.'),
        '#disabled' => $install_status['urlrewrite']['status'],
      );
      $form['urlrewrite']['htaccess_filesystem_path'] = array(
        '#type' => 'textfield',
        '#title' => t('Filesystem path to your .htaccess file'),
        '#size' => 100,
        '#maxlength' => 255,
        '#default_value' => $htaccess_path,
        '#description' => t('This is the absolute directory path of your Drupal .htaccess file.'),
        '#disabled' => $install_status['urlrewrite']['status'],
      );
      
      $form['urlrewrite']['config'] = array(
        '#type' => 'submit',
        '#value' => t('Configure URL Rewrite plugin'),
        '#submit' => array('_gallery_install_step_urlrewrite_configure')
      );
    }
  }
  else {
    $form['urlrewrite']['check'] = array(
      '#type' => 'submit',
      '#value' => t('Check URL Rewrite status again'),
      '#submit' => array('_gallery_install_step_urlrewrite_check')
    );
  }
  
  $form['urlrewrite']['skip'] = array(
    '#type' => 'submit',
    '#value' => t('Skip URL Rewrite Config'),
    '#submit' => array('_gallery_install_step_urlrewrite_skip')
  );
}

/**
 * Function _gallery_install_step_urlrewrite_check().
 */
function _gallery_install_step_urlrewrite_check($form, &$form_state) {
  $install_status = $form_state['values']['install_status'];
  _gallery_init(TRUE, array(), FALSE);
  $form_state['storage'] = $install_status;
}

/**
 * Function _gallery_install_step_urlrewrite_skip().
 */
function _gallery_install_step_urlrewrite_skip($form, &$form_state) {
  $install_status = $form_state['values']['install_status'];
  
  variable_set('gallery_rewrite_disable', 1);
  
  _gallery_init(TRUE, array(), FALSE);
  _gallery_install_urlrewrite_check($install_status);
  
  $install_status['step'] = GALLERY_INSTALL_STEP_USERSYNC;
  $form_state['storage'] = $install_status;
}

/**
 * Function _gallery_install_step_urlrewrite_configure().
 */
function _gallery_install_step_urlrewrite_configure($form, &$form_state) {
  $install_status = $form_state['values']['install_status'];
  
  _gallery_init(TRUE, array(), FALSE);
  if (_gallery_install_urlrewrite($form_state['values']['htaccess_public_path'], $form_state['values']['htaccess_filesystem_path'])) {
    _gallery_install_urlrewrite_check($install_status);
  }
  else {
    $install_status['urlrewrite']['status'] = FALSE;
  }
  
  if ($install_status['urlrewrite']['status']) {
    $install_status['step'] = GALLERY_INSTALL_STEP_USERSYNC;
  }
  $form_state['storage'] = $install_status;
}

/**
 * Function _gallery_install_step_usersync().
 * Step 5: User Synchronization
 */
function _gallery_install_step_usersync(&$form, &$form_state, &$install_status) {
  $title = t('Step 5: Initial User Synchronization') . ($install_status['usersync']['status'] ? ' '. t('(OK)') : '');
  $form['usersync'] = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => ($install_status['step'] > GALLERY_INSTALL_STEP_USERSYNC),
    '#collapsed' => $install_status['usersync']['status'],
  );

  if ($install_status['usersync']['status']) {
    $form['usersync']['#description'] = t('Your initial user synchronization has been completed already.<br />
                                           If you want to resync all your users or you want to import users from an existing
                                           Gallery2 setup, you can always use the <a href= "@gallery-usersync">Advanced Sync
                                           </a> options in the Gallery user administration.',
                                           array('@gallery-usersync' => url('admin/user/gallery/advanced')));
  }
  else {
    $form['usersync']['#description'] = t('User synchronization is essential for the embedded Gallery to work correctly.
                                           Drupal users/groups are usually synced with their Gallery counterparts when a
                                           user/group is modified in Drupal. Missing user mappings will cause errors in your
                                           embedded Gallery.<br /><br />
                                           It is recommended that you sync your users now. However there can be reasons to
                                           skip this step, e.g. if you have a working Gallery with many users and a fresh
                                           Drupal installation. Note that the current user and the Drupal superuser (uid=1)
                                           will always be sync.');

    $form['usersync']['sync'] = array(
      '#type' => 'submit',
      '#value' => t('Sync users/groups'),
      '#submit' => array('_gallery_install_step_usersync_sync')
    );
    $form['usersync']['skip'] = array(
      '#type' => 'submit',
      '#value' => t('Skip sync'),
      '#submit' => array('_gallery_install_step_usersync_skip')
    );
  }
}

/**
 * Function _gallery_install_step_usersync_skip().
 */
function _gallery_install_step_usersync_skip($form, &$form_state) {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_user.inc');
  global $user;
  // Always sync (at least) the current user to avoid
  // errors (unknown user) during GalleryEmbed::init()
  gallery_user_modify($user, 'update', TRUE, array());
  // Make sure the Drupal superuser (uid=1) always gets admin access
  if ($user->uid != 1) {
    gallery_user_modify(user_load(array('uid' => 1)), 'update', FALSE, array());
  }
  
  // Set the global status variable 'gallery_valid'
  gallery_set_status(array(
    'gallery_valid' => array(
      'title' => t('Overall Status (Installation)'),
      'severity' => GALLERY_SEVERITY_SUCCESS,
    ),
  ));
  variable_set('gallery_valid', TRUE);
  
  // Clear cache and rebuild menu
  cache_clear_all('gallery', 'cache', TRUE);
  menu_rebuild();
  
  $form_state['redirect'] = 'admin/settings/gallery';
  $form_state['storage'] = array();
}

/**
 * Function _gallery_install_step_usersync_sync().
 */
function _gallery_install_step_usersync_sync($form, &$form_state) {
  _gallery_install_step_usersync_skip($form, $form_state);
  
  // Trigger bulk user/group sync
  require_once(drupal_get_path('module', 'gallery') .'/gallery_user_admin.inc');
  $batch = array(
    'title' => t('Initial User Synchronization'),
    'operations' => array(array('_gallery_user_advanced_sync', array())),
    'file' => drupal_get_path('module', 'gallery') .'/gallery_user_admin.inc',
    'finished' => '_gallery_user_advanced_finished'
  );
  batch_set($batch);
}

/**
 * Function _gallery_install_locations().
 */
function _gallery_install_locations($orig, $autodetect = TRUE) {
  global $language;
  include_once(drupal_get_path('module', 'gallery') .'/G2EmbedDiscoveryUtilities.class');
  
  // Result status array
  $result = array(
    'gallery_uri' => array(
          'title' => t('URI of Gallery2'),
          'severity' => GALLERY_SEVERITY_ERROR,
    ),
    'gallery_dir' => array(
          'title' => t('Location of Gallery2'),
          'severity' => GALLERY_SEVERITY_ERROR,
    ),
    'init' => array(
          'title' => t('Gallery Init'),
          'severity' => GALLERY_SEVERITY_ERROR,
    ),
  );
  
  $vars = array();
  $vars['locations_valid'] = TRUE;
  
  // Extract gallery_base_from embed_uri if required
  $vars['gallery_base'] = isset($orig['gallery_base']) ? $orig['gallery_base'] : strtolower($orig['gallery_embed_uri']);
  if (($pos = strrpos($vars['gallery_base'], '=')) !== FALSE) {
    $vars['gallery_base'] = trim(substr($vars['gallery_base'], $pos + 1), '/');
  }
  // Construct embed_uri value
  $embed_uri = isset($orig['gallery_embed_uri']) ? $orig['gallery_embed_uri'] : '';
  if ($autodetect || empty($embed_uri)) {
    $options = array();
    // prevent the language prefix from being added to $embed_uri
    if (function_exists('language_url_rewrite')) {
      $options['language'] = (object)array('language' => $language->language, 'prefix' => '', 'domain' => '');
    }
    $embed_uri = url($vars['gallery_base'], $options);
    // Strip the end /gallery if present and replace with index.php?q=gallery to avoid any
    // url rewrite issues. See http://gallery.menalto.com/node/46181
    // Note the use of index.php in all cases. Not needed for Apache, but for IIS
    $length = strlen('/'. $vars['gallery_base']);
    if (substr($embed_uri, - $length) == '/'. $vars['gallery_base']) {
      $embed_uri = substr($embed_uri, 0, - $length + 1) .'index.php?q='. $vars['gallery_base'];
    }
  }
  
  // Normalise the Embed Application URI
  $embed_uri = G2EmbedDiscoveryUtilities::normalizeEmbedUri($embed_uri); 
  $vars['gallery_embed_uri'] = $embed_uri;
  
  // Normalise the G2 URI
  $g2_uri = G2EmbedDiscoveryUtilities::normalizeG2Uri($orig['gallery_uri']);
  $vars['gallery_uri'] = $g2_uri;
  
    // Find the Gallery Directory and Embed URI
  if ($autodetect) {
    // Auto-Detect the G2 embed path 
    list($success, $embed_php_path, $error_string) = G2EmbedDiscoveryUtilities::getG2EmbedPathByG2Uri($g2_uri);
    if ($success) {
      // G2 Embed Path is found OK
      $vars['gallery_dir'] = rtrim($embed_php_path, 'embed.php');
    }
    else {
      // Something is wrong with the G2 URI given as the G2 embed path is not found
      // Try getG2EmbedPathByG2UriEmbedUriAndLocation instead
      $drupal_path = realpath(".") .'/';
      list($success, $embed_php_path, $error_string) =
        G2EmbedDiscoveryUtilities::getG2EmbedPathByG2UriEmbedUriAndLocation($g2_uri, $embed_uri, $drupal_path);
      if ($success) {
        // G2 Embed Path is found OK
        $vars['gallery_dir'] = rtrim($embed_php_path, 'embed.php');
      }
      else {                     
        $result['gallery_uri']['info'] = $error_string;
        $result['gallery_uri']['status'] = FALSE;
        $vars['gallery_dir'] = '';
        $vars['locations_valid'] = FALSE;
      } 
    }
    // Check for config.php in the embed_path (not existing => invalid path)
    //  e.g. this is the path to the G2 codebase, but we have a multisite configuration
    if (!empty($vars['gallery_dir'])) {
      list($ok, $error_string) = G2EmbedDiscoveryUtilities::isFileReadable($vars['gallery_dir'] .'config.php');
      if (!$ok) {
        $result['gallery_dir']['status'] = FALSE;
        $result['gallery_dir']['info'] = $error_string;
        $vars['gallery_dir'] = '';
        $vars['locations_valid'] = FALSE;
      }
    }
  } 
  else {
    // Do not autodetect the variables, but check the manually entered ones.
    $embed_php_path = $orig['gallery_dir'];
    $embed_php_path .= (substr($embed_php_path, -1) != '/') ? '/' : '';
    // Check path can be read, adding embed.php if needed
    list($ok, $error_string) = G2EmbedDiscoveryUtilities::isFileReadable($embed_php_path .'embed.php');
    $vars['gallery_dir'] = $embed_php_path;
    if (!$ok) {
      $result['gallery_dir']['status'] = FALSE;
      $result['gallery_dir']['info'] = $error_string;
      $vars['locations_valid'] = FALSE;
    }
    else {
      // Check for config.php in the embed_path (not existing => invalid path)
      //  e.g. this is the path to the G2 codebase, but we have a multisite configuration
      list($ok, $error_string) = G2EmbedDiscoveryUtilities::isFileReadable($embed_php_path .'config.php');
      if (!$ok) {
        $result['gallery_dir']['status'] = FALSE;
        $result['gallery_dir']['info'] = $error_string;
        $vars['locations_valid'] = FALSE;
      }
    }
  }
  
  // If the filepaths seem correct then check Gallery2 works and has the correct module configs
  if ($vars['locations_valid']) {
    // Gallery install is thought valid, so init it.
    if (!_gallery_init(TRUE, $vars, FALSE)) {
      // Gallery install was supposed to be valid, but it still failed. Something is wrong.
      $result['gallery_init']['status'] = FALSE;
      $result['gallery_init']['info'] = t('Everything seemed OK, but the Gallery could still not be initialised.
                                           Perhaps a manually entered value is incorrect.');
      $vars['locations_valid'] = FALSE;
    }
    else {
      // Is Gallery2 installed inside the Drupal root directory?
      _gallery_install_in_drupal_folder($vars['gallery_dir']);
      // Update version info
      gallery_version();       
      GalleryEmbed::done();
    }
  }
  
  // Only return items where status has been set
  foreach ($result as $key => $value) {
    if (!isset($value['status'])) {
      unset($result[$key]);
    }
  }
  
  return array($vars['locations_valid'], $vars, $result);
}

/**
 * Function _gallery_install_in_drupal_folder().
 */
function _gallery_install_in_drupal_folder($gallery_dir) {
  global $base_url;
  static $warn = TRUE;

  // Dont proceed if G2 is not initialized
  if (!isset($GLOBALS['gallery'])) {
    return;
  }

  if (!empty($gallery_dir)) {
    // Reset 'gallery_outside' state
    variable_del('gallery_outside');
    // Get location of Drupal, Gallery2, etc.
    $docroot = strtolower(str_replace('/', '\\' , $_SERVER['DOCUMENT_ROOT']));
    $base_path = strtolower(str_replace('/', '\\' , base_path()));
    $g2_base = strtolower($GLOBALS['gallery']->getConfig('galleryBaseUrl'));
    if ((strpos(strtolower($gallery_dir), $base_path) === FALSE && strpos(strtolower($gallery_dir), $docroot) !== FALSE)
        || (!empty($g2_base) && strpos($g2_base, $base_url) === FALSE) || strpos(strtolower($gallery_dir), '../') !== FALSE) {
      // G2 is installed outside the Drupal root directory
      variable_set('gallery_outside', TRUE);
      if ($warn) {
        // Avoid duplicate warning
        $warn = FALSE;
        drupal_set_message(t('Your location settings are valid, but your Gallery2 is installed outside the Drupal
                              root directory. It is highly advisable to move G2 into the Drupal folder or to create
                              a symbolic link in the Drupal directory pointing to your G2 installation (see
                              instructions on <a href="@codex">codex.gallery2.org</a>).',
                              array('@codex' => url('http://codex.gallery2.org/Integration:Drupal:Installation'))), 'error');
      }
    }
  }
}

/**
 * Function _gallery_install_plugins().
 */
function _gallery_install_plugins(&$install_status, $title = NULL, $collapse = FALSE) {
  $form['plugins'] = array(
    '#type' => 'fieldset',
    '#title' => isset($title) ? $title : t('Drupal Modules / Gallery2 Plugins'),
    '#collapsible' => TRUE,
    '#collapsed' => $collapse,
  );

  $header = array(t('Name'), t('Status'), t('Description'));
  $overall_plugin_severity = GALLERY_SEVERITY_UNKNOWN - 1;
  $overall_plugin_status = GALLERY_PLUGIN_ENABLED;
  
  // Wanted (required) G2 plugins
  $rows = array();
  $wanted_g2_plugins = module_invoke_all('gallery_plugin_info', GALLERY_PLUGIN_WANTED);
  foreach ($wanted_g2_plugins as $key => $module) {
    // Find the combination of status and severity of the plugin
    $severity = ($module['status'] != GALLERY_PLUGIN_ENABLED) ? $module['severity'] : GALLERY_SEVERITY_SUCCESS;
    $row = array();
    $row[] = array('data' => $module['title'], 'align' => 'left', 'width' => '15%');
    $row[] = array('data' => theme('gallery_severity_status_message', $severity, $module['status']), 'align' => 'center');
    $row[] = array('data' => $module['info'], 'class' => 'description', 'id' => 'gallery-install-plugin-'. $key);
    $rows[] = $row;
    if ($module['status'] != GALLERY_PLUGIN_ENABLED && $severity > $overall_plugin_severity) {
      $overall_plugin_severity = $severity;
      $overall_plugin_status = $module['status'];
    }
  }
  $form['plugins']['wanted_g2_plugins'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recommended Gallery2 plugins'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#value' => theme('table', $header, $rows, array('class' => 'package')),
    '#description' => t('These Gallery2 plugins support much of the functionality that the Drupal Gallery module provides.
                         None of them are strictly required, but you will almost certainly want to activate at least some
                         of them (particularly the Image Block plugin).'),
  );

  // Unwanted (interfering) G2 plugins
  $rows = array();
  $unwanted_g2_plugins = module_invoke_all('gallery_plugin_info', GALLERY_PLUGIN_UNWANTED);
  foreach ($unwanted_g2_plugins as $key => $module) {
    // Find the combination of status and severity of the plugin
    $severity = ($module['status'] == GALLERY_PLUGIN_ENABLED) ? $module['severity'] : GALLERY_SEVERITY_SUCCESS;
    $row = array();
    $row[] = array('data' => $module['title'], 'align' => 'left', 'width' => '15%');
    $row[] = array('data' => theme('gallery_severity_status_message', $severity, $module['status'], TRUE, TRUE), 'align' => 'center');
    $row[] = array('data' => $module['info'], 'class' => 'description', 'id' => 'gallery-install-plugin-'. $key);
    $rows[] = $row;
    if ($module['status'] == GALLERY_PLUGIN_ENABLED && $severity > $overall_plugin_severity) {
      $overall_plugin_severity = $severity;
      $overall_plugin_status = $module['status'];
    }
  }
  $form['plugins']['unwanted_g2_plugins'] = array(
    '#type' => 'fieldset',
    '#title' =>  t('Gallery2 plugins which should not be installed'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#value' => theme('table', $header, $rows, array('class' => 'package')),
    '#description' => t('These Gallery2 plugins should not be used when Gallery2 is
                         embedded into Drupal as they may cause problems.'),
  );

  // Wanted (optional) Drupal modules
  $rows = array();
  $wanted_modules = module_invoke_all('gallery_plugin_info', GALLERY_PLUGIN_DRUPAL);
  foreach ($wanted_modules as $key => $module) {
    // Find the combination of status and severity of the plugin
    $severity = $module['status'] ? GALLERY_SEVERITY_SUCCESS : $module['severity'];
    $row = array();
    $row[] = array('data' => $module['title'], 'align' => 'left', 'width' => '15%');
    $row[] = array('data' => theme('gallery_severity_status_message', $severity, $module['status']), 'align' => 'center');
    $row[] = array('data' => $module['info'], 'class' => 'description', 'id' => 'gallery-install-plugin-'. $key);
    $rows[] = $row;
    if ($module['status'] != GALLERY_PLUGIN_ENABLED && $severity > $overall_plugin_severity) {
      $overall_plugin_severity = $severity;
      $overall_plugin_status = $module['status'];
    }
  }
  $form['plugins']['wanted_modules'] = array(
    '#type' => 'fieldset',
    '#title' =>  t('Optional Drupal modules'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#value' => theme('table', $header, $rows, array('class' => 'package')),
    '#description' => t('These Drupal modules can optionally be used to enhance the functionality of the Gallery module.'),
  );
  
  // Check if no issue was found.
  if ($overall_plugin_severity < GALLERY_SEVERITY_UNKNOWN) {
    $overall_plugin_severity = GALLERY_SEVERITY_SUCCESS;
  }
  
  $install_status['plugins']['status'] = $overall_plugin_status;
  $install_status['plugins']['severity'] = $overall_plugin_severity;
  $install_status['plugins']['info'] = ($overall_plugin_status <= 0) ?
    t('You may have some loss in functionality in your embedded Gallery2 (see below for details). You should check
       the details and if the functionality is not important for your site you can just ignore this.') :
    t('The status of all Drupal modules and Gallery2 plugins is optimal for your embedded Gallery2.');
  
  // Print out an extra warning if location errors are present
  // (and collapse fieldset to draw attention to the overview table instead)
  if (!$install_status['locations']['status']) {
    $form['plugins']['#collapsed'] = TRUE;
    $install_status['plugins']['info'] = t('Due to errors in your location settings status information is
                                            not available for your G2 plugins.');
  }
  
  return $form;
}

/**
 * Function _gallery_install_urlrewrite_check().
 */
function _gallery_install_urlrewrite_check(&$install_status) {
  // User selected to skip the URLRewrite step
  if (variable_get('gallery_rewrite_disable', 0)) {
    $install_status['urlrewrite']['status'] = TRUE;
    $install_status['urlrewrite']['info'] = t('URL Rewrite has been disabled manually.');
    return;
  }
  
  // Skip URLRewrite check if location errors are present
  if (!$install_status['locations']['status']) {
    $install_status['urlrewrite']['status'] = FALSE;
    $install_status['urlrewrite']['info'] = t('Due to errors in your location settings the URL Rewrite
                                               status could not be obtained.');
    return;
  }
  
  if (variable_get('clean_url', 0)) {
    if (gallery_single_plugin_status('rewrite') == GALLERY_PLUGIN_ENABLED) {
      _gallery_install_urlrewrite($public_path, $htaccess_path, TRUE);
      // Check for writable .htaccess file
      if (file_exists($htaccess_path .'.htaccess')) {
        if (is_writable($htaccess_path .'.htaccess')) {
          $install_status['urlrewrite']['status'] = TRUE;
          $install_status['urlrewrite']['info'] = t('There is a writable .htaccess file in your defined Drupal directory
                                                    (@dir).', array('@dir' => $htaccess_path));
        }
        else {
          $install_status['urlrewrite']['status'] = FALSE;
          $install_status['urlrewrite']['info'] = t('The .htaccess file in your defined Drupal directory (@dir) is not
                                                    writable. Please CHMOD it to 644 (or 666 if 644 does not work).',
                                                    array('@dir' => $htaccess_path));
        }
      }
      else {
        $install_status['urlrewrite']['status'] = FALSE;
        $install_status['urlrewrite']['info'] = t('There is no .htaccess file in your defined Drupal directory (@dir) or
                                                  the directory does not exist.', array('@dir' => $htaccess_path));
      }
    }
    else {
      $install_status['urlrewrite']['status'] = FALSE;
      $install_status['urlrewrite']['info'] = t('Clean URLs are enabled in Drupal, but the Gallery2 URL Rewrite plugin
                                                is unavailable.');
    }
  }
  else {
    $install_status['urlrewrite']['status'] = TRUE;
    $install_status['urlrewrite']['info'] = t('Clean URLs are disabled in Drupal and the URL Rewrite plugin will not
                                              be configured.');
  }
  
  // Check syntax of the 'Show Item' rule (must start with 'gallery/')
  list ($ret, $rules) = GalleryCoreApi::getPluginParameter('module', 'rewrite', 'activeRules');
  if (!$ret) {
    $rules = unserialize($rules);
    $gallery_base = variable_get('gallery_base', 'gallery');
    if (isset($rules['rewrite'][0]['pattern']) && substr($rules['rewrite'][0]['pattern'], 0, 7) != $gallery_base) {
      $install_status['urlrewrite']['info'] .= ' <strong>'. t('Your \'Show Item\' rule should start with \'@gallery-base/\'.
                                                               Make sure to change the URL Rewrite rule(s) accordingly.',
                                                               array('@gallery-base' => $gallery_base)) .'</strong>';
    }
  }
}

/**
 * Function _gallery_install_urlrewrite().
 */
function _gallery_install_urlrewrite(&$public_path, &$htaccess_path, $load_config = FALSE) {
  list($ret, $rewrite_api) = GalleryCoreApi::newFactoryInstance('RewriteApi');
  if ($ret || !isset($rewrite_api)) {
    gallery_error(t('Error trying to create URL Rewrite plugin instance.'), $ret);
    if (!$ret) {
      drupal_set_message(t('URL Rewrite plugin could not be instantiated.
                            Please check its configuration in standalone Gallery2.'), 'error');
    }
    return FALSE;
  }
  
  list($ret, $params) = $rewrite_api->fetchEmbedConfig();
  if ($ret) {
    gallery_error(t('Error trying to fetch Embedded URL Rewrite configuration.'), $ret);
    return FALSE;
  }
    
  // Load the configuration from G2 (or derive from Drupal path)
  if ($load_config) {
    if (empty($params['embeddedHtaccess'])) {
      $http = 'http'. (isset($_SERVER['HTTPS']) ? (($_SERVER['HTTPS'] == 'on') ? 's' : '') : '');
      $public_path = str_replace($http .'://'. $_SERVER['HTTP_HOST'], '', base_path());
      $htaccess_path = realpath(".") .'/';
    }
    else {
      $public_path = $params['embeddedLocation'];
      $htaccess_path = $params['embeddedHtaccess'];
    }
  }
        
  // Check for trailing slash
  $public_path .= (substr($public_path, -1) != '/') ? '/' : '';
  $htaccess_path .= (substr($htaccess_path, -1) != '/') ? '/' : '';
  
  if (!$load_config) {
    if (!file_exists($htaccess_path .'.htaccess')) {
      // File .htaccess does not exist
      if (is_writable($htaccess_path .'.htaccess')) {
        // Directory exists and is writable => try to create .htaccess
        $f = fopen($htaccess_path .'.htaccess', 'w');
        fclose($f);
      }
      else {
        return FALSE;
      }
    }
    // Save the G2 rewrite values
    $params['embeddedLocation'] = $public_path;
    $params['embeddedHtaccess'] = $htaccess_path;
    list($ret, $code, $errstr) = $rewrite_api->saveEmbedConfig($params);
    if ($ret) {
      gallery_error(t('Error trying to save Embedded URL Rewrite configuration.'), $ret);
      return FALSE;
    }
    if ($code) {
      $errstr =  $code .' - '. (is_array($errstr) ? $errstr[1] : $errstr);
      drupal_set_message(t('The Gallery2 URL Rewrite plugin returned the following error:') .'<br /><pre>'. $errstr .'</pre>', 'error');
      return FALSE;
    }
  }

  return TRUE;
}
