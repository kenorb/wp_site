<?php
// $Id: swftools.admin.inc,v 1.1.2.13.2.9 2008/10/01 19:29:56 stuartgreenfield Exp $

function swftools_admin_embed_form() {

  // Clear all caches
  drupal_flush_all_caches();

  $swf_options = _swftools_options();

  drupal_add_css(drupal_get_path('module', 'swftools') .'/swftools.admin.css', 'module', 'all', FALSE);
  //$playlist_path = file_create_path(variable_get('swftools_playlist_path', SWFTOOLS_PLAYLIST_PATH));
  $playlist_path = swftools_get_playlist_path();
  $player_directory = swftools_get_player_path() .'/';

  $form = array();

  $form['swftools_method'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Embedding methods'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $methods = swftools_methods_available(SWFTOOLS_EMBED_METHOD);

  if (count($methods)) {
    foreach ($methods AS $method => $info) {

      if (!file_exists($player_directory . $info['shared_file'])) {
        $swf_embed_methods[$method] = $info['title'] .' - <span class="error">Missing</span>';
        if ($info['download']) {
          $swf_embed_methods[$method] = $swf_embed_methods[$method] .' - '. l(t('Download here'), $info['download']);
        }
      }
      else {
        $swf_embed_methods[$method] = $info['title'];
      }
    }
  }

  $form['swftools_method'][SWFTOOLS_EMBED_METHOD] = array(
    '#type' => 'radios',
    '#title' => t('Method'),
    '#default_value' => variable_get(SWFTOOLS_EMBED_METHOD, SWFTOOLS_NOJAVASCRIPT),
    '#options' => $swf_embed_methods,
    '#description' => t('Choose the method that will be used for Flash replacement. JavaScript methods
                         require supporting JavaScript files to be downloaded and added to an appropriate
                         sub-directory of <em>%default</em>. Direct embedding is built in to SWF Tools.',
                         array('%default' => t(drupal_get_path('module', 'swftools') .'/shared'))
                       ),
  );

  // Might have another setting like above for SIFR, if it ever gets used
  $form['swftools_method']['swftools_always_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add JavaScript to all pages'),
    '#default_value' => variable_get('swftools_always_add_js', SWFTOOLS_ALWAYS_ADD_JS),
    '#description' => t('Modules such as filters result in caching of the mark up generated
                         to display Flash content. In these cases SWF Tools will not be
                         triggered to add the necessary JavaScript to the page and the Flash
                         content will not display. Enable this setting to push the necessary
                         JavaScript into the header of every page. This will ensure that cached
                         filter results are processed correctly.
                         '),
  );

/**
  // This feature is not yet implemented
  $form['swftools_method']['swftools_user_no_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to override embedding methods'),
    '#disabled' => TRUE,
    '#default_value' => variable_get('swftools_user_no_js', FALSE),
    '#description' => t('User can choose how Flash content is delivered overriding
                         the \'Method\' setting above. Adds a setting to the profile page.'),
  );
**/

  $form['swftools_method']['swftools_html_alt'] = array(
    '#type' => 'textarea',
    '#title' => t('HTML alternative'),
    '#default_value' => variable_get('swftools_html_alt', SWFTOOLS_DEFAULT_HTML_ALT),
    '#description' => t('Enter the default HTML that will appear if the Flash
                         content cannot be embedded.'),
  );

  $form['swftools_params'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Default settings for embedding'),
    '#description' => t('Please refer to !technote for more details about each setting.
                         If you are looking for the \'base\' option, this is defined
                         in the \'File Locations\' settings above.',
                         array('!technote' => l('Adobe technote 12701', 'http://kb.adobe.com/selfservice/viewContent.do?externalId=tn_12701'))),
  );

  $form['swftools_params']['swftools_params_version'] = array(
    '#type' => 'textfield',
    '#size' => '5',
    '#title' => 'Flash version',
    '#default_value' => variable_get('swftools_params_version', '7'),
  );

  $form['swftools_params']['swftools_params_play'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Play immediately</strong> (play)'),
    '#default_value' => variable_get('swftools_params_play', TRUE),
  );

  $form['swftools_params']['swftools_params_loop'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Loop movie</strong> (loop) .'),
    '#default_value' => variable_get('swftools_params_loop', TRUE),
  );

  $form['swftools_params']['swftools_params_menu'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Full menu</strong> (menu) Displays the full menu, allowing
                   the user a variety of options to enhance or control playback.'),
    '#default_value' => variable_get('swftools_params_menu', FALSE),
  );

  $form['swftools_params']['swftools_params_allowfullscreen'] = array(
    '#type' => 'checkbox',
    '#title' => t('<strong>Allow full screen mode</strong> (allowfullscreen)'),
    '#default_value' => variable_get('swftools_params_allowfullscreen', TRUE),
  );

  $form['swftools_params']['swftools_params_bgcolor'] = array(
    '#type'  => 'textfield',
    '#title' => t('Background color <span class="weight-normal">(bgcolor)</span>'),
    '#size'  => 7,
    '#maxlength' => 7,
    '#required'  => TRUE,
    '#default_value' => variable_get('swftools_params_bgcolor', '#FFFFFF'),
    '#description' => t('Hexadecimal background color in the form #RRGGBB.'),
  );

  $form['swftools_params']['swftools_params_quality'] = array(
    '#type' => 'select',
    '#title' => t('Quality') .' <span class="weight-normal">(quality)</span>',
    '#default_value' => variable_get('swftools_params_quality', 'autohigh'),
    '#options' => $swf_options['quality'],
  );

  $form['swftools_params']['swftools_params_scale'] = array(
    '#type' => 'select',
    '#title' => t('Scale') .' <span class="weight-normal">(scale)</span>',
    '#default_value' => variable_get('swftools_params_scale', 'showall'),
    '#options' => $swf_options['scale'],
  );

  $form['swftools_params']['swftools_params_wmode'] = array(
    '#type' => 'select',
    '#title' => t('Window mode') .' <span class="weight-normal">(wmode)</span>',
    '#default_value' => variable_get('swftools_params_wmode', 'opaque'),
    '#options' => $swf_options['wmode'],
  );

  $form['swftools_params']['swftools_params_align'] = array(
    '#type' => 'select',
    '#title' => t('Align') .' <span class="weight-normal">(align)</span>',
    '#default_value' => variable_get('swftools_params_align', 'l'),
    '#options' => $swf_options['align'],
  );

  $form['swftools_params']['swftools_params_salign'] = array(
    '#type' => 'select',
    '#title' => t('salign') .' <span class="weight-normal">(salign)</span>',
    '#default_value' => variable_get('swftools_params_salign', 'tl'),
    '#options' => $swf_options['salign'],
  );

  $form['swftools_params']['swftools_params_swliveconnect'] = array(
    '#type' => 'select',
    '#options' => array('true' => 'Yes', 'default' => 'No'), // 'default' setting will simple omit this value for simplicity.
    '#title' => t('Load Java') .' <span class="weight-normal">(swliveconnect)</span>',
    '#default_value' => variable_get('swftools_params_swliveconnect', 'default'),
  );

  return system_settings_form($form);

}

function swftools_admin_handling_form() {

  // Clear all caches
  drupal_flush_all_caches();

  $playlist_path = swftools_get_playlist_path();
  $player_directory = swftools_get_player_path() .'/';

  $form = array();

  $form['swftools_files'] = array(
    '#type'  => 'fieldset',
    '#title' => t('File locations'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['swftools_files']['swftools_player_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Default flash player path'),
    '#default_value' => variable_get('swftools_player_path', SWFTOOLS_PLAYER_PATH),
    '#description' => t('Leave this blank to default to the shared player file
                         inside the swftools module (%default). Otherwise your options are:<ul>
                          <li>a path inside your files directory, <em>without</em>
                              a preceding slash (eg. <strong>myplayers</strong>), or</li>
                          <li>a path from the webroot <em>with</em> a preceding slash.
                              (eg <strong>/alternate_filepath</strong>)</li></ul>',
                              array('%default' => t(drupal_get_path('module', 'swftools') .'/shared'))
                              ),
  );

  $form['swftools_files']['swftools_playlist_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Directory in files where playlist/xml files can be cached'),
    '#default_value' => variable_get('swftools_playlist_path', SWFTOOLS_PLAYLIST_PATH),
    '#description' => t('A location inside your files directory
                          without preceding or trailing slashes (eg. <strong>playlists</strong>)'),
  );

  $current_path = variable_get('swftools_media_url', $GLOBALS['base_url'] .'/'. file_create_path(''));

  // Misname the variable with '_dummy' so that it is not saved!
  $form['swftools_files']['swftools_media_url_dummy'] = array(
    '#type' => 'textfield',
    '#title' => t('Media directory url'),
    '#default_value' => $current_path,
    '#disabled' => TRUE,
    '#description' => t("This helps swftools identify the server where your media files are located.
                         You cannot change it here, as changing it has the potential to break existing
                         content. You can set this on the &#36;conf array in your settings.php file.
                         For example: <pre>
  &#36;conf = array(
    'swftools_media_url' => 'http://anothersite.com/media', // No trailing slashes!
  );</pre>               Leaving this value unset in your settings.php file will
                         persuade SWF Tools to check for the existence of your
                         media files, as they are automatically assumed to be
                         on the local server."),
  );

  $form['swftools_files']['swftools_check_media'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check if local files exist'),
    '#default_value' => variable_get('swftools_check_media', TRUE),
    '#description' => t('This option will cause SWF Tools to check that requested local
                         media files exist, and issue a warning if they do not.'),
  );

  $form['swftools']['handling'] = array(
    '#type'  => 'fieldset',
    '#title' => t('File handling'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'Choose how different file types are handled.',
  );

  $form['swftools']['handling'][SWFTOOLS_FLV_DISPLAY] =
    _swftools_admin_file_handling_option(SWFTOOLS_FLV_DISPLAY, $player_directory, "Default player for single FLV movies");

  $form['swftools']['handling'][SWFTOOLS_MP3_DISPLAY] =
    _swftools_admin_file_handling_option(SWFTOOLS_MP3_DISPLAY, $player_directory, "Default player for single MP3 files");

  $form['swftools']['handling'][SWFTOOLS_SWF_DISPLAY] =
    _swftools_admin_file_handling_option(SWFTOOLS_SWF_DISPLAY, $player_directory, "Default player for single SWF files");

  $form['swftools']['handling'][SWFTOOLS_IMAGE_DISPLAY_LIST] =
    _swftools_admin_file_handling_option(SWFTOOLS_IMAGE_DISPLAY_LIST, $player_directory, "Default player for a list of images");

  $form['swftools']['handling'][SWFTOOLS_FLV_DISPLAY_LIST] =
    _swftools_admin_file_handling_option(SWFTOOLS_FLV_DISPLAY_LIST, $player_directory, "Default player for a list of FLV movies");

  $form['swftools']['handling'][SWFTOOLS_MP3_DISPLAY_LIST] =
    _swftools_admin_file_handling_option(SWFTOOLS_MP3_DISPLAY_LIST, $player_directory, "Default player for a list of MP3 music files");

  $form['swftools']['handling'][SWFTOOLS_MEDIA_DISPLAY_LIST] =
    _swftools_admin_file_handling_option(SWFTOOLS_MEDIA_DISPLAY_LIST, $player_directory, "Default player for a list of mixed media");

  // Playlist settings
  $form['swftools']['playlist'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Playlist caching'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => 'Settings for XML files and playlists.',
  );

  $caching = variable_get('swftools_playlist_caching', 'here');

  if ($caching == 'here') {
    file_scan_directory(swftools_get_playlist_path(), '.*', array('.', '..', 'CVS'), 'file_delete', TRUE);
  }

  $form['swftools']['playlist']['swftools_playlist_caching'] = array(
    '#type' => 'select',
    '#title' => t('Clear playlist cache'),
    '#default_value' => $caching,
    '#options' => array('here' => 'Only here', 'always' => 'Every page view'),
    '#description' => 'Select \'Only here\' to clear the playlist directory just '.
                      'by visiting this page. It is useful to select \'Every '.
                      'page view\' for site or module development but not '.
                      'advisable for live sites.',
  );

  $form['swftools_private'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Private file system'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  // Allow SWF Tools to grant access to non-uploaded files
  $form['swftools_private']['swftools_grant_access_to_private_files'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow SWF Tools to grant access to supported private files'),
    '#default_value' => variable_get('swftools_grant_access_to_private_files', SWFTOOLS_GRANT_ACCESS_TO_PRIVATE_FILES),
    '#description' => t('If using a private file system then SWF Tools is unable to
                         access files that are not uploaded via a file module that
                         supports a private file system. For example, SWF Tools will
                         be unable to access files that were manually uploaded via FTP.
                         Enabling this setting will allow SWF Tools to grant access
                         to files with extensions defined by the next setting.
                         Note - if another module <em>denies</em> access then access
                         to the file will not be granted. SWF Tools itself does not
                         deny access to any file. SWF Tools always allows access
                         to files in the playlists directory.
                         '),
  );

  // Default file extensions that SWF Tools will grant access for
  $form['swftools_private']['swftools_grant_access_extensions'] = array(
    '#type' => 'textfield',
    '#title' => t('Extensions that SWF Tools will allow access to'),
    '#default_value' => variable_get('swftools_grant_access_extensions', SWFTOOLS_GRANT_ACCESS_EXTENSIONS),
    '#maxlength' => 255,
    '#description' => t('Under a private file system SWF Tools will grant access to files
                         with extensions in this list. Separate extensions with a space
                         and do not include the leading dot. Although any extension can be
                         added to this list, only <em>swf flv xml mp3 jpg jpeg</em> and
                         <em>png</em> will return the correct headers.'),
  );

  return system_settings_form($form);
}


function _swftools_admin_file_handling_option($type, $player_directory, $description) {

  // Initialise list of methods
  $list = array();

  // Obtain list of available methods
  $methods = swftools_methods_available($type);
  if (count($methods)) {
    foreach ($methods AS $method => $info) {
      if (!file_exists($player_directory . $info['shared_file'])) {
        $list[$method] = $info['title'] .' - <span class="error">Missing</span>';
        if ($info['download']) {
          $list[$method] = $list[$method] .' - '. l(t('Download here'), $info['download']);
        }
      }
      else {
        $list[$method] = $info['title'];
      }
    }
  }

  // Sort the list of methods
  asort($list);

  // None is always an option so add this at the top of the list
  $list = array('None') + $list;

  // Determine the appropriate default based on $type
  $default = swftools_get_player($type);

  // swftools_get_player() returns FALSE for nothing configured, change to 0
  if (!$default) {
    $default = 0;
  }

  return array(
    '#type' => 'radios',
    '#title' => t($description),
    '#default_value' => $default,
    '#options' => $list,
  );

}


/**
 * flashvar and param option arrays. These are used for options settings in the
 * configuration screen and also as a lookup (particularly 'bool') to output the correct value for to html.
 *
 */
function _swftools_options() {
  // cache output each request
  static $swf_options = array();
  if (!count($swf_options)) {
    $swf_options['quality'] = array('low' => 'low', 'autolow' => 'autolow', 'medium' => 'medium', 'high' => 'high', 'autohigh' => 'autohigh', 'best' => 'best', );
    $swf_options['wmode'] = array('window' => 'window',  'opaque' => 'opaque',   'transparent' => 'transparent', );
    $swf_options['scale'] = array('showall' => 'showall', 'noborder' => 'noborder', 'exactfit' => 'exactfit', );
    $swf_options['align'] = array('default' => 'centered', 'l' => 'left', 'r' => 'right', 't' => 'top', 'b' => 'bottom', );
    $swf_options['salign'] = array('l' => 'left', 'r' => 'right', 't' => 'top', 'b' => 'bottom', 'tl' => 'top left', 'tr' => 'top right', 'bl' => 'bottom left', 'br' => 'bottom right', );
    $swf_options['bool'] = array('default' => 'default', 'true' => 'true', 'false' => 'false');
  }
  return $swf_options;
}
