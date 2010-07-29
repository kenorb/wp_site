<?php
// $Id: gallery_g2image_settings.inc,v 1.1.2.4 2009/01/25 14:21:32 profix898 Exp $

require_once(drupal_get_path('module', 'gallery') .'/gallery_settings.inc');

/**
 * gallery_g2image.module : gallery_g2image_settings.inc
 * Settings functions
 */

/**
 * Function _gallery_g2image_settings().
 */
function _gallery_g2image_settings() {
  $plugin_status =  gallery_plugin_status(array('imageblock'));
  $plugin_status['g2image'] = _gallery_g2image_path() ? GALLERY_PLUGIN_ENABLED : GALLERY_PLUGIN_MISSING;

  $desc = t('G2Image requires the Gallery2 Image Block plugin (!imageblock_status) and
             the G2Image application (!g2image_status) to be installed.',
              array(
                '!imageblock_status' => theme('gallery_plugin_status_message', $plugin_status['imageblock']),
                '!g2image_status' => theme('gallery_plugin_status_message', $plugin_status['g2image'])
              )
           );
  if ($plugin_status['imageblock'] != GALLERY_PLUGIN_ENABLED) {
    $g2_uri = variable_get('gallery_embed_uri', '?q=gallery');
    $g2_plugins_page = (variable_get('gallery_valid', 0)) ?
      t('<a href= "@g2_plugins">Gallery2 Plugins</a>',
        array('@g2_plugins' => $g2_uri .'&g2_view=core.SiteAdmin&g2_subView=core.AdminModules'))
      : t('Gallery2 Site Admin -> Plugins');
    $desc .= t('<br />However, the Image Block plugin is unavailable. To use G2Image please go to
                the !g2_plugins page and install/activate the Image Block plugin.', array(
                 '!g2_plugins' => $g2_plugins_page));
  }
  if ($plugin_status['g2image'] != GALLERY_PLUGIN_ENABLED) {
    $desc .= t('<br />However, the G2Image application is unavailable. Please <a href="@g2image_url">download
                G2Image</a> and install it as outlined in README.TXT.', array(
                 '@g2image_url' => 'http://g2image.steffensenfamily.com/index.php?title=Download'));
  }

  $form['g2image'] = array(
    '#type' => 'fieldset',
    '#title' => t('Gallery Image Assist (G2Image) settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#description' => $desc,
  );

  if ($plugin_status['imageblock'] == GALLERY_PLUGIN_ENABLED && $plugin_status['g2image'] == GALLERY_PLUGIN_ENABLED) {
    $form['g2image']['gallery_g2image_mode'] = array(
      '#type' => 'select',
      '#title' => t('Mode'),
      '#default_value' => variable_get('gallery_g2image_mode', 'disabled'),
      '#options' => array(
        'disabled' => t('Disabled'),
        'standalone' => t('Standalone'),
        'tinymce' => t('TinyMCE'),
      ),
      '#description' => t('Determines the mode of operation. For anything other than \'Disabled\' the G2Image
                           application has to be installed. See the README.txt instructions. In \'Standalone\'
                           mode a button will be visible under textfields to launch the g2image window. In
                           \'TinyMCE\' mode the g2image button can be used in the TinyMCE toolbar. Note that the
                           TinyMCE version will NOT work wih Safari - use the standalone version instead.'),
    );

    $visibility = variable_get('gallery_g2image_visibility', 1);
    $form['g2image']['gallery_g2image_visibility'] = array(
      '#type' => 'radios',
      '#title' => t('Show G2Image link on specific pages (Standalone mode)'),
      '#default_value' => $visibility,
      '#options' => array(
        t('Show on every page except the listed pages.'),
        t('Show on only the listed pages.')
      )
    );
    if ($visibility) {
      $form['g2image']['gallery_g2image_visibility_pages'] = array(
        '#type' => 'textarea',
        '#title' => t('Pages (Standalone mode only)'),
        '#default_value' => variable_get('gallery_g2image_visibility_pages', "node/add/*\ncomment/*"),
        '#description' => t('Enter one page per line as Drupal paths. The \'*\' character is a wildcard. Example paths are
                             \'%blog\' for the blog page and %blog-wildcard for every personal blog. %front is the front page.',
                             array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'))
      );
    }
    
    $form['g2image']['gallery_g2image_sortby'] = array(
      '#type' => 'select',
      '#title' => t('Default Sort Order'),
      '#default_value' => variable_get('gallery_g2image_sortby', 'title_asc'),
      '#options' => array(
        'title_asc' => t('Gallery2 Title (A-z)'),
        'title_desc' => t('Gallery2 Title (z-A)'),
        'name_asc' => t('Filename (A-z)'),
        'name_desc' => t('Filename (z-A)'),
        'mtime_desc' => t('Last Modification (newest first)'),
        'mtime_asc' => t('Last Modification (oldest first)'),
      ),
      '#description' => t('Determines the default sorting order.'),
    );

    $form['g2image']['gallery_g2image_images_per_page'] = array(
      '#type' => 'select',
      '#title' => t('Default Number of images per page'),
      '#default_value' => variable_get('gallery_g2image_images_per_page', 20),
      '#options' => _gallery_range_array(10, 60, 10),
      '#description' => t('Choose the default number of images per page.'),
    );

    $form['g2image']['gallery_g2image_display_filenames'] = array(
      '#type' => 'select',
      '#title' => t('Default Display Options'),
      '#default_value' => variable_get('gallery_g2image_display_filenames', 'thumbnails'),
      '#options' => array(
        'thumbnails' => t('Thumbnails only'),
        'filenames' => t('Thumbnails with title and filename'),
      ),
      '#description' => t('Choose the default display option for images.'),
    );

    $form['g2image']['gallery_g2image_default_action'] = array(
      '#type' => 'select',
      '#title' => t('Default Action'),
      '#default_value' => variable_get('gallery_g2image_default_action', 'drupal_g2_filter'),
      '#options' => array(
        'drupal_g2_filter' => t('Gallery Filter ID'),
        'thumbnail_image' => t('Thumbnail with link to image'),
        'thumbnail_album' => t('Thumbnail with link to parent album'),
        'thumbnail_custom_url' => t('Thumbnail with link to custom URL'),
        'thumbnail_only' => t('Thumbnail only'),
        'link_image' => t('Text link to image'),
        'link_parent' => t('Text link to parent album'),
      ),
      '#description' => t('Choose the default method of inserting the image info.'),
    );

    $form['g2image']['gallery_g2image_custom_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Default custom url'),
      '#default_value' => variable_get('gallery_g2image_custom_url', 'http://'),
      '#size' => 50,
      '#maxlength' => 50,
      '#description' => t('The default custom url. Used with \'Thumbnail with link to custom URL\' action setting'),
    );

    $css_options =  array(
      'none' => t('None'),
      'g2image_normal' => t('Normal'),
      'g2image_float_left' => t('Float Left'),
      'g2image_float_right' => t('Float Right'),
      'g2image_centered' => t('Centered'),
    );

    // Merge in custom class options
    $gallery_g2image_custom_class = array_filter(variable_get('gallery_g2image_custom_class', array()));
    $css_options = array_merge($css_options, drupal_map_assoc($gallery_g2image_custom_class));

    $form['g2image']['gallery_g2image_default_alignment'] = array(
      '#type' => 'select',
      '#title' => t('Default Alignment'),
      '#default_value' => variable_get('gallery_g2image_default_alignment', 'none'),
      '#options' => $css_options,
      '#description' => t('Choose the default alignment of images.'),
    );

    $form['g2image']['gallery_g2image_custom_class'] = array('#tree' => TRUE);
    for ($i=1; $i<=4; $i++) {
      $form['g2image']['gallery_g2image_custom_class'][$i] = array(
        '#type' => 'textfield',
        '#title' => t('Custom Class @index', array('@index' => $i)),
        '#default_value' => isset($gallery_g2image_custom_class[$i]) ? $gallery_g2image_custom_class[$i] : '',
        '#size' => 20,
        '#maxlength' => 50,
      );
    }
    $form['g2image']['gallery_g2image_custom_class'][$i-1]['#description'] = t('Additional css classes to be available
                                                                                for selection (must be defined in .css file).');

    $form['g2image']['gallery_g2image_class_mode'] = array(
      '#type' => 'select',
      '#title' => t('Alignment Class Insertion Point'),
      '#default_value' => variable_get('gallery_g2image_class_mode', 'img'),
      '#options' => array(
        'img' => t('<img class=...>'),
        'div' => t('<div class=...><img ...>'),
      ),
      '#description' => t('Determines where the alignment class will be inserted. If you choose \'&lt;div
                           class=...&gt;&lt;img ...&gt;\', you will have to manually delete any &lt;div&gt; tags
                           manually after deleting images from the TinyMCE window.'),
    );
  }

  $form['#validate'] = array('_gallery_g2image_settings_validate');
  return system_settings_form($form);
}

/**
 * Function _gallery_g2image_settings_validate().
 * (saves the G2Image config file)
 */
function _gallery_g2image_settings_validate($form, &$form_state) {
  switch ($form_state['values']['gallery_g2image_mode']) {
    case 'tinymce':
      $mode = t('TinyMCE');
      if (module_exists('tinymce')) {
        $path = drupal_get_path('module', 'tinymce');
      }
      else if (module_exists('wysiwyg')) {
        $path = drupal_get_path('module', 'wysiwyg');
      }
      $path .= '/tinymce/jscripts/tiny_mce/plugins/g2image';
      break;
    case 'standalone':
      $mode = t('Standalone');
      $path = drupal_get_path('module', 'gallery_g2image');
      $path .= '/g2image';
      break;
    default:
      return;
  }

  // Note that file_check_directory uses &$path and will strip the trailing '/'
  $filename = $path .'/config.php';
  if (is_writable($filename) == FALSE && !file_check_directory($path)) {
    form_set_error('', t('G2Image does not seem to be installed for @mode mode in the required directory (@dir), or the
                          directory is not writable. Please see the INSTALL.txt for instructions.',
                            array('@mode' => $mode, '@dir' => '<drupal_path>/'. $path)));
    return;
  }

  $cr = "\n";
  $g2ic_gallery2_path = str_replace('\\', '/', variable_get('gallery_dir', './gallery2/'));
  $g2ic_use_full_path = (substr($g2ic_gallery2_path, 0, 1) == '/' || substr($g2ic_gallery2_path, $_SERVER['DOCUMENT_ROOT']) !== FALSE) ? 'TRUE' : 'FALSE';
  $form_state['values']['gallery_g2image_custom_url'] = check_url($form_state['values']['gallery_g2image_custom_url']);
  $g2ic_display_filenames =  ($form_state['values']['gallery_g2image_display_filenames'] == 'filenames') ? 'TRUE' : 'FALSE';
  $g2ic_custom_class = array();
  foreach ($form_state['values']['gallery_g2image_custom_class'] as $key => $value) {
    $g2ic_custom_class[$key] = ($value) ? $value : 'not_used';
  }
  
  $content = '<?php'. $cr;
  $content .= '/*'. $cr;
  $content .= '    Gallery 2 Image Chooser'. $cr;
  $content .= '    Version 3.0 - updated 16 SEP 2007'. $cr;
  $content .= '    Documentation: http://g2image.steffensenfamily.com/'. $cr;
  $content .= $cr;
  $content .= '    Author: Kirk Steffensen with inspiration, code snippets,'. $cr;
  $content .= '        and assistance as listed in CREDITS.HTML'. $cr;
  $content .= $cr;
  $content .= '    Released under the GPL version 2.'. $cr;
  $content .= '    A copy of the license is in the root folder of this plugin.'. $cr;
  $content .= $cr;
  $content .= '    See README.HTML for installation info.'. $cr;
  $content .= '    See CHANGELOG.HTML for a history of changes.'. $cr;
  $content .= '*/'. $cr;
  $content .= $cr;
  $content .= '//  Automatically generated by gallery.module (admin/settings/gallery/g2image).'. $cr;
  $content .= '//  DO NOT EDIT'. $cr;
  $content .= $cr;

  $content .= '$g2ic_embedded_mode = TRUE;'. $cr;
  $content .= '$g2ic_gallery2_path = \''. $g2ic_gallery2_path .'\';'. $cr;
  $content .= '$g2ic_use_full_path = '. $g2ic_use_full_path .';'. $cr;
  $content .= '$g2ic_gallery2_uri = \''.  variable_get('gallery_uri', '/gallery2/') .'\';'. $cr;
  $content .= '$g2ic_embed_uri = \''. variable_get('gallery_embed_uri', '?q=gallery') .'\';'. $cr;

  $content .= '$g2ic_drupal_g2_filter = TRUE;'. $cr;
  $content .= '$g2ic_drupal_g2_filter_prefix = \''. variable_get('gallery_filter_prefix', 'G2') .'\';'. $cr;

  $content .= '$g2ic_language = \'en\';'. $cr;
  $content .= '$g2ic_images_per_page = '. $form_state['values']['gallery_g2image_images_per_page'] .';'. $cr;
  $content .= '$g2ic_display_filenames = '. $g2ic_display_filenames .';'. $cr;

  $content .= '$g2ic_default_alignment = \''. $form_state['values']['gallery_g2image_default_alignment'] .'\';'. $cr;
  $content .= '$g2ic_custom_class_1 = \''. $g2ic_custom_class[1] .'\';'. $cr;
  $content .= '$g2ic_custom_class_2 = \''. $g2ic_custom_class[2] .'\';'. $cr;
  $content .= '$g2ic_custom_class_3 = \''. $g2ic_custom_class[3] .'\';'. $cr;
  $content .= '$g2ic_custom_class_4 = \''. $g2ic_custom_class[4] .'\';'. $cr;
  $content .= '$g2ic_class_mode = \''. $form_state['values']['gallery_g2image_class_mode'] .'\';'. $cr;

  $content .= '$g2ic_default_action = \''. $form_state['values']['gallery_g2image_default_action'] .'\';'. $cr;
  $content .= '$g2ic_sortby = \''. $form_state['values']['gallery_g2image_sortby'] .'\';'. $cr;
  $content .= '$g2ic_custom_url = \''. $form_state['values']['gallery_g2image_custom_url'] .'\';'. $cr;
  $content .= $cr;
  $content .= '?>'. $cr;

  $filename = $path .'/config.php';
  if (is_writable($filename) == FALSE) {
    form_set_error('', t('Could not write to G2Image config file. Please check the permissions for @filename.',
                            array('@filename' => $filename)));
    return;
  }
  $handle = fopen($filename, "w");
  if (fwrite($handle, $content) === FALSE) {
    form_set_error('', t('Could not write to G2Image config file. Please check the permissions for @filename.',
                            array('@filename' => $filename)));
    return;
  }
  else {
    drupal_set_message(t('G2Image configuration file successfully created.'));
  }
  fclose($handle);
}

/**
 * Function _gallery_g2image_path().
 */
function _gallery_g2image_path($path = NULL) {
  if (!isset($path)) {
    // Standalone G2Image folder
    $path = drupal_get_path('module', 'gallery_g2image') .'/g2image';
    if (!file_exists($path .'/config.php') && !file_check_directory($path)) {
      $path = NULL;
      // TinyMCE folder for G2Image plugin
      if (module_exists('tinymce')) {
        $path = drupal_get_path('module', 'tinymce');
        $path .= '/tinymce/jscripts/tiny_mce/plugins/g2image';
        if (!file_exists($path .'/config.php') && !file_check_directory($path)) {
          $path = NULL;
        }
      }
      else if (module_exists('wysiwyg')) {
      // WYSIWYG folder for G2Image plugin
        $path = drupal_get_path('module', 'wysiwyg');
        $path .= '/tinymce/jscripts/tiny_mce/plugins/g2image';
        if (!file_exists($path .'/config.php') && !file_check_directory($path)) {
          $path = NULL;
        }
      }
    }
  }

  // Store location of G2Image
  variable_set('gallery_g2image_path', isset($path) ? $path : FALSE);

  return $path;
}
