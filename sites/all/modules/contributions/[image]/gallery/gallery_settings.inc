<?php
// $Id: gallery_settings.inc,v 1.12.2.11 2008/08/04 21:13:06 profix898 Exp $

/**
 * gallery.module : gallery_settings.inc
 * Settings functions
 */

/**
 * Function _gallery_settings_general().
 */
function _gallery_settings_general() {
  // Short Status
  gallery_version();
  gallery_plugin_set_status(array('imageblock', 'imageframe', 'search'));
  $status = gallery_get_status();
  
  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
    '#description' => gallery_format_status($status, ''),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  
  // Theme Settings
  $form['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme and Display settings'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $themes = system_theme_data();
  ksort($themes);
  $options = array('default' => t('System default'));
  foreach ($themes as $theme) {
    $options[$theme->name] = $theme->info['name'];
  }
  $form['theme']['gallery_page_theme'] = array(
    '#type' => 'select',
    '#title' => t('Gallery page theme'),
    '#default_value' => variable_get('gallery_page_theme', 'default'),
    '#options' => $options,
    '#description' => t('Drupal theme to be used for all gallery pages (gallery/*).'),
  );
  $form['theme']['gallery_embed_theme'] = array(
    '#type' => 'select',
    '#title' => t('Gallery embedded theme'),
    '#default_value' => variable_get('gallery_embed_theme', 'default'),
    '#options' => array('default' => t('Gallery default')) + gallery_list_themes(),
    '#description' => t('Gallery2 theme to be used for embedded gallery.'),
  );
  $form['theme']['gallery_root_album'] = _gallery_settings_album_select(
    t('Default root album'),
    variable_get('gallery_root_album', 'default'),
    t('Default root album to use on gallery pages. If you have a separate album for use with nodes (Filter, G2Image, ...)
       it can be helpful to move the root album to hide that album from the browseable gallery.')
  );
  
  // Gallery Block Settings
  $form['blocks'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery block settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Use these settings to determine the number of available blocks.<br />
                         <strong>Warning: If you decrease the number of blocks, always the last
                         block (with the highest ID) will be removed.</strong>'),
  );
  $form['blocks']['gallery_block_num'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of blocks'),
    '#default_value' => variable_get('gallery_block_num', 2),
    '#size' => 10,
    '#maxlength' => 10,
    '#description' => t('Select how many blocks should be available.'),
  );
  
  // Gallery2 Sidebar Settings
  $form['sidebar'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sidebar settings'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['sidebar']['gallery_move_sidebar_to_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move Gallery2 sidebar to Drupal Gallery Navigation block'),
    '#default_value' => variable_get('gallery_move_sidebar_to_block', 1),
    '#description' => t('When selected, the Gallery2 sidebar will be moved into
                         the Drupal "Gallery Navigation" block. This typically allows
                         for a much cleaner embedded gallery and is generally
                         recommended. Note that you will need to enable the Gallery
                         Navigation block.'),
  );
  $form['sidebar']['gallery_move_admin_sidebar_to_block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move Gallery2 Admin sidebar to Drupal Gallery Navigation block'),
    '#default_value' => variable_get('gallery_move_admin_sidebar_to_block', 0),
    '#description' => t('When selected, the Gallery2 Admin sidebar will be moved into
                         the Drupal "Gallery Navigation" block. This is not normally
                         recommended since the Admin Sitebar is very long and the
                         formatting does not always fit with the rest of the block
                         (but cannot be changed in the current Gallery2 versions).'),
    '#disabled' => !variable_get('gallery_move_sidebar_to_block', 1),
  );
  
  // Gallery2 Google Sitemap Settings
  $gallery2_sitemap_status = gallery_single_plugin_status('sitemap');
  $gallery2_sitemap_status_str = theme('gallery_plugin_status_message', $gallery2_sitemap_status);
  $xmlsitemap_status = module_exists('xmlsitemap');
  $xmlsitemap_status_str = theme('gallery_module_status_message', $xmlsitemap_status);
  $xmlsitemap_available = $xmlsitemap_status && ($gallery2_sitemap_status == GALLERY_PLUGIN_ENABLED);
  
  $desc = t('Allows the Gallery2 sitemap to be merged with the Drupal one so that only one URL needs to
             be supplied to search engines. Requires the Drupal XML Sitemap module (!xmlsitemap_status) and
             the Gallery2 sitemap module (!gallery2_sitemap_status) to be installed/activated.',
    array(
      '!xmlsitemap_status' => $xmlsitemap_status_str,
      '!gallery2_sitemap_status' => $gallery2_sitemap_status_str,
    )
  );
  
  $form['sitemap'] = array(
    '#type' => 'fieldset',
    '#title' => t('XML Sitemap settings'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => $desc,
  );
  $form['sitemap']['gallery_enable_sitemap'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable merge of Gallery2 sitemap with Drupal sitemap'),
    '#default_value' => $xmlsitemap_available ? variable_get('gallery_enable_sitemap', 1) : FALSE,
    '#disabled' => !$xmlsitemap_available,
  );
  
  // Error logging / Debug Settings
  $form['error'] = array(
    '#type' => 'fieldset',
    '#title' => t('Error logging / Debug settings'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['error']['gallery_error_mode'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Error logging'),
    '#default_value' => variable_get('gallery_error_mode', array(GALLERY_ERROR_WATCHDOG)),
    '#options' => array(GALLERY_ERROR_WATCHDOG => t('Watchdog'),
                        GALLERY_ERROR_BROWSER => t('Output to the browser'),
                        GALLERY_ERROR_VERBOSE => t('Verbose error messages')),
    '#description' => t('Choose where errors are displayed and how detailed they are.'),
  );
  $form['error']['gallery_report'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable bug report assistant'),
    '#default_value' => variable_get('gallery_report', 1),
    '#description' => t('In case of errors the module can automatically assemble useful data (system info and debug
                         traces) to help you with detailed bug reports. Only available to users with \'administer
                         site configuration\' permission.')
  );
  $search_status = module_exists('search') && (gallery_single_plugin_status('search') == GALLERY_PLUGIN_ENABLED);
  $form['error']['gallery_error_redirect'] = array(
    '#type' => 'checkbox',
    '#title' => t('Redirect to Gallery search form for invalid paths'),
    '#default_value' => $search_status ? variable_get('gallery_error_redirect', 0) : 0,
    '#description' => t('Instead of showing a message that the requested Gallery URL does not exist the
                         user is redirected to the search form.'),
    '#disabled' => !$search_status,
  );
  $form['error']['debug'] = array(
    '#type' => 'fieldset',
    '#title' => t('Debug settings'),
    '#description' => '',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $debug = variable_get('gallery_debug', 0);
  $form['error']['debug']['gallery_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#default_value' => $debug,
    '#description' => t('Print out debug variables and verbose error messages. Only visible to users
                         with \'administer site configuration\' permission.')
  );
  $form['error']['debug']['gallery_g2debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable G2 debug output'),
    '#default_value' => $debug && variable_get('gallery_g2debug', 0),
    '#description' => t('Enables Gallery2\'s buffered debug output to be displayed.'),
    '#disabled' => !$debug
  );
  
  // Relevant links
  $form['links'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links to the most relevant Drupal & Gallery2 pages'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // Drupal links
  $desc  = '<ul>';
  $desc .= '<li><a href="@gallery-users">Gallery users</a> : Gallery2 user integration and synchronization.</li>';
  $desc .= '<li><a href="@gallery-report">Report Generator</a> : Collect information about your installation. Useful
            especially for bug reports.</li>';
  $desc .= '</ul>';
  $form['links']['drupal'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links to Drupal paths'),
    '#description' => t($desc, array(
      '@gallery-users' => url('admin/user/gallery'),
      '@gallery-report' => url('admin/settings/gallery/report/download'))),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  // Gallery2 links
  $desc  = '<ul>';
  $desc .= '<li><a href="@g2-modules">Plugins</a> : To install, activate or configure the required and optional plugins
            relating to gallery.module (Image Block, Image Frame, URL Rewrite, Sitemap, Search).</li>';
  $desc .= '<li><a href="@g2-themes">Themes</a> : To add/remove blocks in the sidebar.</li>';
  if (gallery_single_plugin_status('rewrite') == GALLERY_PLUGIN_ENABLED) {
    $desc .= '<li><a href="@g2-rewrite">Embedded URL Rewrite settings</a> : To define the rewrite rules.</li>';
  }
  $desc .= '<li><a href="@g2-cookie">Cookie Settings</a> : Only needed for a small number of site configurations, such
            as the use of different subdomains for Gallery2 and Drupal.</li>';
  $desc .= '</ul>';
  $g2_uri = variable_get('gallery_embed_uri', '?q=gallery');
  $form['links']['gallery'] = array(
    '#type' => 'fieldset',
    '#title' => t('Links to Gallery2 Site Admin sections'),
    '#description' => t($desc, array(
      '@g2-modules' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=core.AdminModules',
      '@g2-themes' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=core.AdminThemes&g2_mode=defaults',
      '@g2-rewrite' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=rewrite.AdminRewrite&g2_mode=setup',
      '@g2-cookie' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=core.AdminCore')),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  
  $form = system_settings_form($form);
  $form['#validate'] = array('_gallery_settings_general_validate');
  $form['#submit'] = array('_gallery_settings_general_submit', 'system_settings_form_submit');
  return $form;
}

/**
 * Function _gallery_settings_general_validate().
 */
function _gallery_settings_general_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['gallery_block_num']) || $form_state['values']['gallery_block_num'] < 1) {
    form_set_error('gallery_block_num', t('Number of image blocks must be a positiv integer greater zero.'));
  }
}

/**
 * Function _gallery_settings_general_submit().
 */
function _gallery_settings_general_submit($form, &$form_state) {
  if ($form_state['values']['gallery_enable_sitemap'] != variable_get('gallery_enable_sitemap', 0)) {
    drupal_set_message('The XML sitemap setting has been updated, but the new sitemap may not be generated
                        immediately (controlled by XML Sitemap).');
  }
}

/**
 * Function _gallery_settings_filter().
 */
function _gallery_settings_filter() {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_block.inc');
  $plugin_status =  gallery_plugin_status(array('imageblock', 'imageframe'));
  
  $desc = t('Note that changing the defaults here will change the all the gallery filter images
    on the site, not just new images. '); 
  $desc .= t('The Gallery Filter requires the Gallery2 Image Block plugin (!imageblock_status) and
              optionally the Gallery2 Image Frame plugin (!imageframe_status).',
              array(
                '!imageblock_status' => theme('gallery_plugin_status_message', $plugin_status['imageblock']),
                '!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']),
              ));
  if ($plugin_status['imageblock'] != GALLERY_PLUGIN_ENABLED) {
    $g2_uri = variable_get('gallery_embed_uri', '?q=gallery');
    $g2_plugins_page = (variable_get('gallery_valid', 0)) ? 
      t('<a href= "@g2_plugins">Gallery2 Plugins</a>',
        array('@g2_plugins' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=core.AdminModules'))
      : t('Gallery2 Site Admin -> Plugins');
    $desc .= t(' However the Image Block plugin is unavailable, so the Gallery Filter is
      not available and the settings are disabled. To use the filter please go to
      the !g2_plugins page and install/activate the Image Block plugin.',
        array('!g2_plugins' => $g2_plugins_page));
  }    

  $imageframe_desc = ($plugin_status['imageframe'] != GALLERY_PLUGIN_ENABLED) ?
    t('Requires the Gallery2 Image Frame plugin (!imageframe_status).',
      array('!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']))) : '';

  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery Filter settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => $desc,
  );
  
  $image_frames = gallery_list_image_frames();
  _gallery_block_options($type_map, $param_map);
  $type_map['specificItem'] = t('Specific item');
  
  if ($plugin_status['imageblock'] == GALLERY_PLUGIN_ENABLED) {          
    $form['filter']['gallery_filter_prefix'] = array(
      '#type' => 'textfield',
      '#title' => t('Filter prefix'),
      '#default_value' => variable_get('gallery_filter_prefix', 'G2'),
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('Prefix to use with filter. Example: \'G2\' means you use [G2: 999].
        Be careful when changing this as any pages using the previous prefix will not be changed
        and so will not be filtered correctly.'),
    );
    
    $form['filter']['gallery_filter_default_block_type'] = array(
      '#type' => 'select',
      '#title' => t('Default image type'),
      '#default_value' => variable_get('gallery_filter_default_block_type', 'viewedImage'),
      '#options' => $type_map,
      '#description' => t('Pick the default type of image you would like to use.'),
    );
    
    $form['filter']['gallery_filter_can_cache'] = array(
      '#type' => 'checkbox',
      '#title' => t('Cache gallery filter pages'),
      '#default_value' => variable_get('gallery_filter_can_cache', 1),
      '#description' => t('By default the gallery filter output is cached by Drupal to speed up page loading.
                           However, it will not cache the css class info for the frames. The best approach is the
                           make sure that the sidebar image block and the gallery filter images use the same frames.
                           If you are unable to do this you will have to deselect this option to force Drupal not to
                           cache the pages, or else your frames will not appear. If you change this option you will
                           need to go to <a href="@link">admin/settings/filters</a> and re-save the image formats
                           that use gallery filter.', array('@link' => url('admin/settings/filters'))),
    );
    
    $form['filter']['gallery_filter_n_images'] = array(
      '#type' => 'textfield',
      '#title' => t('Default number of images'),
      '#default_value' => variable_get('gallery_filter_n_images', 1),
      '#size' => 3,
      '#maxlength' => 3,
      '#description' => t('How many images you want the default block to show.  Best to keep at 1 and use the n parameter.'),
    );
    
    $form['filter']['gallery_filter_default_show'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Default image data'),
      '#default_value' => variable_get('gallery_filter_default_show', array('none')),
      '#options' => $param_map,
      '#description' => t('Choose the item metadata you would like to display by default.
                           This will change all instances where show parameter was not specified.'),
    );
    
    // Parse gallery_filter.css and extract classes
    $css = implode('', file(drupal_get_path('module', 'gallery') .'/gallery_filter.css'));
    preg_match_all('/div.giImageBlock.(\w+) {/', $css, $matches);
    if (count($matches[1])) {
      $classes = array();
      foreach ($matches[1] as $class) {
        $classes[$class] = $class;
      }
      $form['filter']['gallery_filter_default_div_class'] = array(
        '#type' => 'select',
        '#title' => t('Default class'),
        '#default_value' => variable_get('gallery_filter_default_div_class', 'nowrap'),
        '#options' => $classes,
      );
    }
    else {
      $form['filter']['gallery_filter_default_div_class'] = array(
        '#type' => 'textfield',
        '#title' => t('Default class'),
        '#default_value' => variable_get('gallery_filter_default_div_class', 'nowrap'),
        '#size' => 20,
        '#maxlength' => 20,
        '#description' => t('left, right, or nowrap. (See gallery_filter.css to add more or modify these.)'),
      );
    }
    
    $form['filter']['gallery_filter_default_maxsize'] = array(
      '#type' => 'textfield',
      '#title' => t('Default \'Max Size\' thumbnail size'),
      '#default_value' => variable_get('gallery_filter_default_maxsize', GALLERY_FILTER_MAXSIZE_DEFAULT),
      '#size' => 10,
      '#maxlength' => 10,
    );
    
    $form['filter']['gallery_filter_default_exactsize'] = array(
      '#type' => 'textfield',
      '#title' => t('Default \'Exact Size\' thumbnail size'),
      '#default_value' => variable_get('gallery_filter_default_exactsize', GALLERY_FILTER_EXACTSIZE_DEFAULT),
      '#size' => 10,
      '#maxlength' => 10,
      '#description' => t('If no size is specified when calling the filter, one of
        these sizes (and implied method) will be used. <br />\'Max Size\' gives faster
        image downloading, but the image size 
        may be smaller than the size defined. <br />\'Exact Size\' may be slower
        (as a larger image is downloaded and then scaled by the browser) but the image
        will be guaranteed to be the size defined. Only supported for G2.2+.
        Only fill in one size box (not both) to set the default method.'),
    );
    
    $form['filter']['gallery_filter_default_album_frame'] = array(
      '#type' => 'select',
      '#title' => t('Default album frame'),
      '#default_value' => variable_get('gallery_filter_default_album_frame', 'none'),
      '#options' => $image_frames,
      '#description' => $imageframe_desc,
    );
    
    $form['filter']['gallery_filter_default_item_frame'] = array(
      '#type' => 'select',
      '#title' => t('Default item frame'),
      '#default_value' => variable_get('gallery_filter_default_item_frame', 'none'),
      '#options' => $image_frames,
      '#description' => $imageframe_desc,
    );
    
    $form['filter']['gallery_filter_default_link_target'] = array(
      '#type' => 'textfield',
      '#title' => t('Default link target'),
      '#default_value' => variable_get('gallery_filter_default_link_target', ''),
      '#size' => 20,
      '#maxlength' => 20,
      '#description' => t('Enter a link target (e.g. "_blank", "_new").'),
    );
    
    $form['filter']['gallery_filter_default_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Image Link'),
      '#default_value' => variable_get('gallery_filter_default_link', ''),
      '#size' => 20,
      '#maxlength' => 20,
      '#description' => t('By default the image has a link to the item in the Gallery. This
        can be overridden here (or leave empty for the default). Use \'none\' for no link, or a URL
        to link somewhere else instead.'),
    );
  }
  
  $form['array_filter'] = array('#type' => 'value');
  $form = system_settings_form($form);
  $form['#validate'] = array('_gallery_settings_filter_validate');
  $form['#submit'] = array('_gallery_settings_filter_submit', 'system_settings_form_submit');
  return $form;
}

/**
 * Function _gallery_settings_filter_validate().
 */
function _gallery_settings_filter_validate($form, &$form_state) {
  // Only validate if a setting was returned
  if (isset($form_state['values']['gallery_filter_default_maxsize'])) {
    if ((strlen($form_state['values']['gallery_filter_default_maxsize']) > 0) &&
      (strlen($form_state['values']['gallery_filter_default_exactsize']) > 0)) {
      form_set_error('gallery_filter_default_maxsize',
        t('You must set either the Max Size or the Exact Size, not both.'));  
    } 
    elseif ((strlen($form_state['values']['gallery_filter_default_maxsize']) == 0) &&
      (strlen($form_state['values']['gallery_filter_default_exactsize']) == 0)) {
      form_set_error('gallery_filter_default_maxsize',
        t('You must set either the Max Size or the Exact Size.'));  
    }
    if ((strlen($form_state['values']['gallery_filter_default_maxsize']) > 0) &&
      (!is_numeric($form_state['values']['gallery_filter_default_maxsize']) ||
      $form_state['values']['gallery_filter_default_maxsize'] < 1)) {
      form_set_error('gallery_filter_default_maxsize',
        t('Max Size must be a number greater than zero.'));
    }
    if ((strlen($form_state['values']['gallery_filter_default_exactsize']) > 0) &&
      (!is_numeric($form_state['values']['gallery_filter_default_exactsize']) ||
      $form_state['values']['gallery_filter_default_exactsize'] < 1)) {
      form_set_error('gallery_filter_default_exactsize',
        t('Exact Size must be a number greater than zero.'));
    }
    if ((strlen($form_state['values']['gallery_filter_n_images']) > 0) &&
      (!is_numeric($form_state['values']['gallery_filter_n_images']) ||
      $form_state['values']['gallery_filter_n_images'] < 1)) {
      form_set_error('gallery_filter_n_images',
        t('Number of images must be a number greater than zero.'));
    }
  }
}

/**
 * Function _gallery_settings_filter_submit().
 */
function _gallery_settings_filter_submit($form, &$form_state) {
  // The default values have changed, so the pages containing the gallery filter need
  // to be updated (or else it would only occur on an edit), so empty the filter cache.
  
  // Find out which formats are using the gallery filter
  $result = db_query("SELECT format FROM {filters} WHERE module = 'gallery'");
  while ($format = db_fetch_object($result)) {
    cache_clear_all($format->format .':', 'cache_filter', TRUE);
  }
  
  drupal_set_message('The gallery filter cache has been cleared so that the new defaults apply.');
}

/**
 * Function _gallery_settings_search().
 */
function _gallery_settings_search() {
  require_once(drupal_get_path('module', 'gallery') .'/gallery_block.inc');
  $plugin_status =  gallery_plugin_status(array('imageblock', 'imageframe', 'search'));

  $search_desc = t('The Gallery Search requires the Gallery2 Search plugin (!search_status) and
    optionally the Gallery2 Image Block plugin (!imageblock_status) and Gallery2 Image Frame
    plugin (!imageframe_status).',
      array(
        '!search_status' => theme('gallery_plugin_status_message', $plugin_status['search']),
        '!imageblock_status' => theme('gallery_plugin_status_message', $plugin_status['imageblock']), 
        '!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']),
      )
    );
  $g2_uri = variable_get('gallery_embed_uri', '?q=gallery');
  $g2_plugins_page = (variable_get('gallery_valid', 0)) ? 
    t('<a href= "@g2_plugins">Gallery2 Plugins</a>',
      array('@g2_plugins' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=core.AdminModules'))
    : t('Gallery2 Site Admin -> Plugins');
  $thumbs_desc = '';
  if ($plugin_status['search'] != GALLERY_PLUGIN_ENABLED) {
    $search_desc .= t(' However the Search plugin is unavailable, so the search is
      not available and these settings are disabled. To use the search feature please go to
      the !g2_plugins page and install/activate the Image Block plugin.',
        array('!g2_plugins' => $g2_plugins_page));
  }   
  if ($plugin_status['imageblock'] != GALLERY_PLUGIN_ENABLED) {
    $thumbs_desc = t('To display search results with thumbnail images requires the Gallery2 Image
      Block plugin (!imageblock_status) and optionally the  and Gallery2 Image Frame 
      plugin (!imageframe_status).',
        array(
          '!imageblock_status' => theme('gallery_plugin_status_message', $plugin_status['imageblock']), 
          '!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']),
        )
      );
    $thumbs_desc .= t(' However the Image Block plugin is unavailable, so the thumbnails are
      not available and these settings are disabled. To use this feature please go to
      the !g2_plugins page and install/activate the Image Block plugin.',
        array('!g2_plugins' => $g2_plugins_page));
  }    

  $imageframe_desc = ($plugin_status['imageframe'] != GALLERY_PLUGIN_ENABLED) ?
    t('Requires the Gallery2 Image Frame plugin (!imageframe_status).',
        array('!imageframe_status' => theme('gallery_plugin_status_message', $plugin_status['imageframe']))) : '';
     
  $form['search'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => $search_desc,
  );
  
  $image_frames = gallery_list_image_frames();
  _gallery_block_options($type_map, $param_map);
  
  if ($plugin_status['search'] == GALLERY_PLUGIN_ENABLED) {
    $form['search']['gallery_search_advanced'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display advanced search options'),
      '#default_value' => variable_get('gallery_search_advanced', 1),
      '#description' => t('Adds options to the search form to select which Gallery fields to search in.'),
    );
    
    $form['search']['gallery_search_num_per_row'] = array(
      '#type' => 'select',
      '#title' => t('Number of search results per table row'),
      '#default_value' => variable_get('gallery_search_num_per_row', 3),
      '#options' => _gallery_range_array(1, 5),
      '#description' => t('Select the number of search results per row in the paged table.'),
    );
    
    $form['search']['gallery_search_rows_per_pager'] = array(
      '#type' => 'select',
      '#title' => t('Number of rows per page'),
      '#default_value' => variable_get('gallery_search_rows_per_pager', 4),
      '#options' => _gallery_range_array(1, 10),
      '#description' => t('Select the number of rows in the paged table.'),
    );
    
    $form['search']['thumbs'] = array(
      '#type' => 'fieldset',
      '#title' => t('Search thumbnail settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#description' => $thumbs_desc,
    );
    
    if ($plugin_status['imageblock'] == GALLERY_PLUGIN_ENABLED) {          
      $form['search']['thumbs']['gallery_search_show_thumbs'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display thumbnails'),
        '#default_value' => variable_get('gallery_search_show_thumbs', 1),
        '#description' => t('Display thumbnail images for the search results.'),
      );
      
      $form['search']['thumbs']['gallery_search_block_show'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Image data'),
        '#default_value' => variable_get('gallery_search_block_show', array('title' => t('Title'))),
        '#options' => $param_map,
        '#description' => t('Choose the item metadata you would like to display.'),
        );
        
      $form['search']['thumbs']['gallery_search_size_method'] = array(
        '#type' => 'select',
        '#title' => t('Image size method'),
        '#default_value' => variable_get('gallery_search_size_method', GALLERY_SEARCH_SIZE_METHOD_DEFAULT),
        '#options' => array(
          'maxsize' => t('Max Size'),
          'exactsize' => t('Exact Size'),
        ),
        '#description' => t('\'Max Size\' gives faster image downloading, but the image size 
          may be smaller than the size defined below. <br />\'Exact Size\' may be slower
          (as a larger image is downloaded and then scaled by the browser) but the image
          will be guaranteed to be the size defined below. Only supported for G2.2+.'),
      );  
    
      $form['search']['thumbs']['gallery_search_size'] = array(
        '#type' => 'textfield',
        '#title' => t('Image size'),
        '#default_value' => variable_get('gallery_search_size', GALLERY_SEARCH_SIZE_DEFAULT),
        '#size' => 10,
        '#maxlength' => 10,
        '#description' => t('Sets the size (in pixels) of the longest side of the image
          according to the method defined above.'),
      );
       
      $form['search']['thumbs']['gallery_search_album_frame'] = array(
        '#type' => 'select',
        '#title' => t('Album frame'),
        '#default_value' => variable_get('gallery_search_album_frame', 'none'),
        '#options' => $image_frames,
        '#description' => $imageframe_desc,
      );
      
      $form['search']['thumbs']['gallery_search_item_frame'] = array(
        '#type' => 'select',
        '#title' => t('Item frame'),
        '#default_value' => variable_get('gallery_search_item_frame', 'none'),
        '#options' => $image_frames,
        '#description' => $imageframe_desc,
      );
      
      $form['search']['thumbs']['gallery_search_link_target'] = array(
        '#type' => 'textfield',
        '#title' => t('Link target'),
        '#default_value' => variable_get('gallery_search_link_target', ''),
        '#size' => 20,
        '#maxlength' => 20,
        '#description' => t('Enter a link target (e.g. "_blank", "_new").'),
      );
      
      $form['search']['thumbs']['gallery_search_default_link'] = array(
        '#type' => 'textfield',
        '#title' => t('Image Link'),
        '#default_value' => variable_get('gallery_search_default_link', ''),
        '#size' => 20,
        '#maxlength' => 20,
        '#description' => t('By default the image has a link to the item in the Gallery. This
          can be overridden here (or leave empty for the default). Use \'none\' for no link, or a
          URL to link somewhere else instead.'),
      );
    }
  }
  
  $form['array_filter'] = array('#type' => 'value');
  $form['#validate'] = array('_gallery_settings_search_validate');
  return system_settings_form($form);
}

/**
 * Function _gallery_settings_search_validate().
 */
function _gallery_settings_search_validate($form, &$form_state) {
  if (isset($form_state['values']['gallery_search_size']) &&
    (!is_numeric($form_state['values']['gallery_search_size']) || $form_state['values']['gallery_search_size'] < 1)) {
    form_set_error('gallery_search_size',
      t('Image size must be a number greater than zero.'));
  }
}

/**
 * Function _gallery_settings_album_select().
 */
function _gallery_settings_album_select($title, $value, $description, $root = NULL, $depth = 1, $uid = NULL) {
  $options = array();
  $tree = gallery_album_tree($root, $depth, $uid);
  _gallery_settings_album_traverse($tree, $options);

  return array(
    '#type' => 'select',
    '#title' => $title,
    '#default_value' => $value,
    '#options' => array('default' => t('Default')) + $options,
    '#description' => $description
  );
}

/**
 * Function _gallery_settings_album_traverse().
 */
function _gallery_settings_album_traverse(&$tree, &$items, $indent = '') {
  foreach (array_keys($tree) as $id) {
    $album = gallery_item_details($id);
    $items[$id] = (empty($indent) ? '' : $indent .' ') . $album['title'];
    if (count($tree[$id])) {
      _gallery_settings_album_traverse($tree[$id], $items, $indent .'--');
    }
  }
}

/**
 * Function _gallery_range_array().
 * (create an associative range array
 *  like array($min=>$min, ..., $max=>$max))
 */
function _gallery_range_array($min, $max, $interval = 1) {
  $range_array = array();
  for ($i=$min; $i<=$max; $i+=$interval) {
    $range_array[$i] = $i;
  }

  return $range_array;
}

/**
 * Function _gallery_php_memcheck().
 * (check the amount of PHP memory)
 */
function _gallery_php_memcheck($min_memory = 24) {
  $mem_limit = trim(ini_get('memory_limit'));
  $mem_limit_bytes = parse_size($mem_limit);
  
  if (empty($mem_limit)) {
    return array(
      'status' => TRUE,
      'info' => t('There is no memory limit restricting your PHP installation.'),
    );
  }
  elseif (($mem_limit_bytes / (1024 * 1024)) < $min_memory) {
    return array(
      'status' => FALSE,
      'info' => t('Your PHP is configured to limit the memory to @mem_limit
        (memory_limit parameter in php.ini). You must raise this limit 
        to at least @min_mem_limitM for proper embedded Gallery2 operation.',
        array(
          '@mem_limit' => $mem_limit, 
          '@min_mem_limit' => $min_memory,
        )
      )
    );
  } 
  else {
    return array(
      'status' => TRUE,
      'info' => t('Your PHP is configured to limit the memory to @mem_limit
        (memory_limit parameter in php.ini). This should be fine for embedded Gallery2 operation.',
        array(
          '@mem_limit' => $mem_limit, 
        )
      )
    );
  }
}

/**
 * Implementation of hook_gallery_plugin_info().
 */
function gallery_gallery_plugin_info($type) {
  switch ($type) {
    case GALLERY_PLUGIN_WANTED:
      return array(
        'imageblock' => array(
          'title' => 'Image Block',
          'info' => t('Allows images to be included in the Drupal sidebar or in nodes.'),
          'severity' => GALLERY_SEVERITY_ERROR,
          'status' => gallery_single_plugin_status('imageblock'),
        ),
        'imageframe' => array(
          'title' => 'ImageFrame',
          'info' => t('Provides a variety of frames around the images.'),
          'severity' => GALLERY_SEVERITY_WARNING,
          'status' => gallery_single_plugin_status('imageframe'),
        ),
        'search' => array(
          'title' => 'Search',
          'info' => t('Allow the Drupal search to also search the Gallery.'),
          'severity' => GALLERY_SEVERITY_WARNING,
          'status' => gallery_single_plugin_status('search'),
        ),
        'rewrite' => array(
          'title' => 'URL Rewrite',
          'info' => t('Allow short URLs (clean URLs).'),
          'severity' => GALLERY_SEVERITY_ADVISE,
          'status' => gallery_single_plugin_status('rewrite'),
        ),
      );
    case GALLERY_PLUGIN_UNWANTED:
      return array(
        'register' => array(
          'title' => 'Registration',
          'info' => t('All user registration must take place via Drupal to ensure that the users
            are synchronized between Drupal and Gallery2. This plugin would allow a user to register
            only in Gallery2 and not in Drupal and so should not be used.'),
          'severity' => GALLERY_SEVERITY_WARNING,
          'status' => gallery_single_plugin_status('register'),
        ),
      );
    case GALLERY_PLUGIN_DRUPAL:
      return array(
        'xmlsitemap' => array(
          'title' => 'XML Sitemap',
          'status' => module_exists('xmlsitemap'),
          'severity' => GALLERY_SEVERITY_ADVISE,
          'info' => t('Enables the Drupal and Gallery2 sitemaps to be merged which allows for
                       a single sitemap to be sent to Google and other web search sites.'),
        ),
        'profile' => array(
          'title' => 'Profile',
          'status' => module_exists('profile'),
          'severity' => GALLERY_SEVERITY_ADVISE,
          'info' => t('Allows support of the \'Full Name\' field in Gallery2.'),
        ),
      );
    default:
      return array();
  }
}

/**
 * Function gallery_plugin_set_status().
 */
function gallery_plugin_set_status($plugin_names) {
  $plugins_status = gallery_plugin_status($plugin_names);
  $plugin_info = module_invoke_all('gallery_plugin_info', GALLERY_PLUGIN_WANTED);
  // Generate array containing module status
  $status = array();
  foreach ($plugins_status as $plugin => $plugin_status) {
    if ($plugin_status == GALLERY_PLUGIN_ENABLED) {
      $status[$plugin] = '';
    }
    else {
      $status[$plugin]['title'] = t('Gallery2 plugin \'@plugin\' is not available',
        array('@plugin' => isset($plugin_info[$plugin]) ? $plugin_info[$plugin]['title'] : drupal_ucfirst($plugin)));
      $status[$plugin]['severity'] = isset($plugin_info[$plugin]['severity']) ? $plugin_info[$plugin]['severity'] : GALLERY_SEVERITY_WARNING;
      $status[$plugin]['url'] = url('admin/settings/gallery/install', array('fragment' => 'gallery-install-plugin-'. $plugin));
    }
  }
  gallery_set_status($status);
}

/**
 * Function gallery_format_status().
 */
function gallery_format_status($status = array(), $title = 'Gallery message(s):') {
  $message = $title .'<ul>';
  if (count($status)) {
    foreach ($status as $item) {
      $message .= '<li>';
      if (isset($item['title'])) {
        $message .= empty($item['url']) ? t($item['title']) : '<a href="'. $item['url'] .'">'. t($item['title']) .'</a>';
        $message .= ': ';
      }
      $message .= (isset($item['info']) && !empty($item['info'])) ? t($item['info']) : '';
      $message .= isset($item['severity'])  ? (' ['. theme('gallery_severity_message', $item['severity']) .']') : '';
      $message .= '</li>';
    }
  }
  else {
    $message = t('Status not available');
  }
  $message .= '</ul>';
  
  return $message;
}

/**
 * Theme function : theme_gallery_module_status_message().
 */
function theme_gallery_module_status_message($status) {
  if ($status) {
    return '<span class="admin-enabled">'. t('enabled') .'</span>';
  } 
  else {
    return '<span class="admin-disabled">'. t('disabled') .'</span>';
  }
}

/**
 * Theme function : theme_gallery_severity_message().
 */
function theme_gallery_severity_message($severity = NULL) {
  switch ($severity) {
    case GALLERY_SEVERITY_SUCCESS:
      return '<span class=\'g2_embed_success\'>'. t('OK') .'</span>';
    case GALLERY_SEVERITY_ERROR:
      return '<span class=\'g2_embed_error\'>'. t('Error') .'</span>';
    case GALLERY_SEVERITY_WARNING:
      return '<span class=\'g2_embed_warning\'>'. t('Warning') .'</span>';
    case GALLERY_SEVERITY_ADVISE:
      return '<span class=\'g2_embed_warning\'>'. t('Advisory') .'</span>';
    case GALLERY_SEVERITY_UNKNOWN:
      return '<span class=\'g2_embed_warning\'>'. t('Unknown') .'</span>';
    default:
  }      
}

/**
 * Theme function : theme_gallery_plugin_status_message().
 */
function theme_gallery_plugin_status_message($status, $invert = FALSE) {
  $classes = array('enabled' => 'admin-enabled', 'disabled' => 'admin-disabled');
  if ($invert) {
    $classes = array_reverse($classes, TRUE);
  }

  switch ($status) {
    case GALLERY_PLUGIN_ENABLED:
      return '<span class="'. $classes['enabled'] .'">'. t('activated') .'</span>';
    case GALLERY_PLUGIN_DISABLED:
      return '<span class="'. $classes['disabled'] .'">'. t('disabled') .'</span>';
    case GALLERY_PLUGIN_NOT_ACTIVE:
      return '<span class="'. $classes['disabled'] .'">'. t('deactivated') .'</span>';
    case GALLERY_PLUGIN_NOT_INSTALLED:
      return '<span class="'. $classes['disabled'] .'">'. t('not installed') .'</span>';
    case GALLERY_PLUGIN_MISSING:
      return '<span class="'. $classes['disabled'] .'">'. t('missing') .'</span>';
    case GALLERY_PLUGIN_STATUS_UNKNOWN:
    default:
      return '<span class="'. $classes['disabled'] .'">'. t('unknown') .'</span>';
  }    
}

/**
 * Theme function : theme_gallery_severity_status_message().
 */
function theme_gallery_severity_status_message($severity, $status, $full_msg = FALSE, $invert = FALSE) {
  // In some cases (e.g. unwanted plugins) it makes sense to return the full 2 part message
  // even on a success, but in most cases a simple "OK" is sufficient.
  if ($full_msg) {
    return theme('gallery_severity_message', $severity) .' ('. theme('gallery_plugin_status_message', $status, $invert) .')';
  }    
  switch ($severity) {
    case GALLERY_SEVERITY_SUCCESS:
      return theme('gallery_severity_message', $severity);
    default:
      return theme('gallery_severity_message', $severity) .' ('. theme('gallery_plugin_status_message', $status, $invert) .')';
  }      
}
