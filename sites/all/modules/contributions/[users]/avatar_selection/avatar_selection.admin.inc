<?php
// $Id: avatar_selection.admin.inc,v 1.1.2.7 2008/07/20 14:55:40 snpower Exp $

/**
 * @file
 * Administrative page callbacks for the avatar_selection module.
 */


/**
 * Scan the directory for new files.
 *
 * @param $name
 *   The name of the avatar(s).
 * @param $access
 *   The permission - the value determines which user roles have rights to see
 *   the avatar.
 * @param $og
 *   Organic group (optional).
 * @param $weight
 *   Avatar weight to assign.
 * @return
 *   Number of images found.
 */
function _avatar_selection_scan_images($name, $access, $og = array(), $weight = 0) {
  $avatars = array();
  $add_count = 0;
  $delete_count = 0;

  $dir = file_create_path('avatar_selection');
  $mask = '.*\.(gif|GIF|Gif|jpg|JPG|Jpg|jpeg|JPEG|Jpeg|png|PNG|Png)';
  $listings = file_scan_directory($dir, $mask, array('.', '..', 'CVS'), 0, FALSE);

  $result = db_query("SELECT avatar FROM {avatar_selection} avs");
  while ($avatar = db_fetch_object($result)) {
    $avatars[$avatar->avatar] = $avatar->avatar;
  }

  // Search for new files.  Remove matching records from avatars array.
  foreach ($listings as $listing) {
    if (in_array($listing->basename, $avatars)) {
      unset($avatars[$listing->basename]);
    }
    else {
      _avatar_selection_save_avatar_info(0, $listing->basename, (empty($name) ?  $listing->basename : $name), $access, $og, $weight);
      $add_count++;
    }
  }

  // Remove records from database where we have an avatar entry but no
  // corresponding file.
  foreach ($avatars as $avatar) {
    avatar_selection_image_delete($avatar);
    $delete_count++;
  }

  $count['add'] = $add_count;
  $count['delete'] = $delete_count;

  return $count;
}


/**
 * Select which form will be shown to the user, according to the permissions.
 *
 * @param $op
 *   Default NULL; the action the user wants to do after the function checks
 &   the permission.
 * @return
 *   Return the structure of the form.
 */
function avatar_selection_settings_page($op = NULL) {

  switch ($op) {
    case 'edit':
      $output = drupal_get_form('avatar_selection_config_form');
      break;
    case 'upload':
      $output = drupal_get_form('avatar_selection_upload_form');
      break;
    case 'list' :
      $output = drupal_get_form('avatar_selection_edit_form');
      break;
    default:
      $form[] = array(
        '#type' => 'fieldset',
        '#title' => t('Add another'),
      );
      $output = drupal_get_form('avatar_selection_config_form');
      break;
  }
  return $output;
}


/**
 * Create the form structure for configuring the avatar module settings; seen
 * in the 'Configure' tab under the Avatar Selection administration page.
 *
 * @return
 *   Return the structure of the form.
 */
function avatar_selection_config_form() {
  if (!variable_get('user_pictures', 0)) {
    drupal_set_message(t('User Pictures option is disabled.  You will need to enable this option before you can use the Avatar Selection module.  You may configure this setting on the <a href="@url">User settings</a> page.', array('@url' => url('admin/user/settings'))));
  }

  // To store how many avatars per page are displayed.
  $form['update']['avatar_per_page'] = array(
    '#type' => 'textfield',
    '#title' => t('How many avatars per page'),
    '#description' => t('The number of avatars to show per page.'),
    '#default_value' => variable_get('avatar_selection_avatar_per_page', 30),
    '#size' => 3,
  );

  $form['update']['disable_user_upload'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable users uploading pictures to profile'),
    '#description' => t('Allow users to pick their avatar from the selection but prevent them from uploading new avatars when editing their account.'),
    '#default_value' => variable_get('avatar_selection_disable_user_upload', FALSE),
    );
  $form['update']['force_set_image_reg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force users to select an avatar image on user registration.'),
    '#description' => t('This only applies on the user registration screen.'),
    '#default_value' => variable_get('avatar_selection_force_user_avatar_reg', FALSE),
    );
  $form['update']['force_set_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force users to select an avatar image when editing their account'),
    '#description' => t('This only applies when the user is editing their account details and image uploads are disabled.'),
    '#default_value' => variable_get('avatar_selection_force_user_avatar', FALSE),
    );
  $form['update']['set_random_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable random default avatar image.'),
    '#description' => t("Automatically set a random avatar image to be used when the user doesn't set one for their account."),
    '#default_value' => variable_get('avatar_selection_set_random_default', FALSE),
    );
  $form['update']['distinctive_avatars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable unique avatars.'),
    '#description' => t("Only allow users to pick an avatar that isn't already in use by another user.  If there are no available avatars left, the default avatar image will be used."),
    '#default_value' => variable_get('avatar_selection_distinctive_avatars', FALSE),
    );


  $form['update']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Create the form structure for uploading an avatar.
 *
 * @return
 *   Return the structure of the form.
 */
function avatar_selection_upload_form() {
  if (!variable_get('user_pictures', 0)) {
    drupal_set_message(t('User Pictures option is disabled.  You will need to enable this option before you can use the Avatar Selection module.  You may configure this setting on the <a href="@url">User settings</a> page.', array('@url' => url('admin/user/settings'))));
  }

  $form['#attributes']['enctype'] = 'multipart/form-data';

  $form['bulk_upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bulk Upload / Delete'),
    '#description' => t("To upload a large number of avatars, first copy the images manually to the %dir folder, using ftp for example.  To make these new avatar images available, check the 'Scan for new avatars' option. All new images will then be added to the list.  By removing files from this directory and checking the box, you can also perform a bulk delete.", array('%dir' => file_create_path('avatar_selection'))),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['bulk_upload']['scan_avatars'] = array(
    '#type' => 'checkbox',
    '#title' => t('Scan avatars'),
    '#description' => t('All new avatar images found will be added to the list of available avatars with the name, weight and permissions defined below.  Scanning for new avatars may be slow depending on the number of files.  All avatar entries which no longer have a corresponding file will be be removed.'),
    '#default_value' => 0,
  );


  $form['picture_upload'] = array('#type' => 'file',
    '#title' => t('Upload image'),
    '#size' => 48,
    '#description' => t('A new avatar image.  Maximum dimensions are %dimensions and the maximum size is %size kB.  Images must have one of the following extensions (case sensitive): png, jpg, jpeg, gif, PNG, JPG, JPEG, GIF.', array('%dimensions' => variable_get('user_picture_dimensions', '85x85'), '%size' => variable_get('user_picture_file_size', 30))) .' '.  variable_get('user_picture_guidelines', ''),
  );

  $form['avatar_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t("Image name or title which will be displayed when hovering over the image.  It's also used in conjunction with the weight setting for sorting the avatars."),
    '#size' => 48,
  );
  $form['avatar_weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#delta' => 100,
    '#description' => t('Avatars with a lower weight appear before higher weighted avatars in lists.'),
  );
  $form['permissions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Permissions'),
    '#weight' => 2,
  );
  $form['permissions']['access'] = array(
    '#type' => 'checkboxes',
    '#title' => t('User Roles'),
    '#options' => avatar_selection_handler_filter_role(),
    '#description' => t('Only the checked roles will be able to see this avatar icon; if no roles are checked, access will not be restricted.'),
  );
  if (module_exists("og")) {
    $form['permissions']['og_access'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Organic Groups'),
      '#options' => og_all_groups_options(),
      '#description' => t('Only users in the checked organic groups will be able to see this avatar icon; if no groups are checked, access will not be restricted.'),
    );
  }

  $form['upload'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
    '#weight' => 10,
  );

  return $form;
}

function avatar_selection_roles_page($op = NULL) {
  $output = "";

  // Display the form where appropriate.
  if (isset($op) && ($op == 'role' || $op == 'og')) {
    $output .= drupal_get_form('avatar_selection_edit_form');
  }

  // Display the number of avatars per role / group.
  else {
    $avs_access = array();
    $og_access = array();
    $avs_access[0] = 0;
    $og_access[0] = 0;

    // Get the list of access roles and initialise the count to 0.
    $roles = avatar_selection_handler_filter_role();
    foreach ($roles as $rid => $role_name) {
      $avs_access[$rid] = 0;
    }

    // Get the list of organic groups and initialise the count to 0.
    if (module_exists("og")) {
      $ogroups = og_all_groups_options();
      foreach ($ogroups as $ogid => $ogroup_name) {
        $og_access[$ogid] = 0;
      }
    }

    // Get the total number of avatars available on the system.
    $total_count = 0;
    $result = db_query("SELECT count(*) AS count FROM {avatar_selection} avs");
    while ($avatar = db_fetch_object($result)) {
      $total_count = $avatar->count;
    }
    $output .= '<p>'. t('There is a total of %count avatars configured.', array('%count' => $total_count)) .'</p>';


    // Get the count of avatars per role.
    $result = db_query("SELECT avsr.rid, count(*) AS count FROM {avatar_selection} avs LEFT JOIN {avatar_selection_roles} avsr ON avs.aid = avsr.aid GROUP BY avsr.rid");
    while ($avatar = db_fetch_object($result)) {
      if (empty($avatar->rid)) {
        $avs_access[0] += $avatar->count;
      }
      else {
        $avs_access[$avatar->rid] += $avatar->count;
      }
    }

    // Get the count of avatars per organic group.
    $result = db_query("SELECT avso.ogid, count(*) AS count FROM {avatar_selection} avs LEFT JOIN {avatar_selection_og} avso ON avs.aid = avso.aid GROUP BY avso.ogid");
    while ($avatar = db_fetch_object($result)) {
      if (empty($avatar->ogid)) {
        $og_access[0] += $avatar->count;
      }
      else {
        $og_access[$avatar->ogid] += $avatar->count;
      }
    }

    // Format the user roles table.
    $avs_rows = array();
    $header = array(t('User Role'), t('Number of Avatars'));
    $edit = l(t('edit'), 'admin/settings/avatar_selection/edit/role/0');
    $avs_rows[] = array(t('Available to all roles'), $avs_access[0], $edit);
    foreach ($roles as $rid => $role_name) {
      $edit = l(t('edit'), 'admin/settings/avatar_selection/edit/role/'. $rid);
      $avs_rows[] = array($role_name, $avs_access[$rid], $edit);
    }
    $output .= theme('table', $header, $avs_rows);


    // Format the organic groups table.
    if (module_exists("og")) {
      $og_rows = array();
      $header = array(t('Organic Group'), t('Number of Avatars'));
      $edit = l(t('edit'), 'admin/settings/avatar_selection/edit/og/0');
      $og_rows[] = array(t('Available to all groups'), $og_access[0], $edit);
      foreach ($ogroups as $ogid => $ogroup_name) {
        $edit = l(t('edit'), 'admin/settings/avatar_selection/edit/og/'. $ogid);
        $og_rows[] = array($ogroup_name, $og_access[$ogid], $edit);
      }
      $output .= theme('table', $header, $og_rows);
    }
  }

  return $output;
}


/**
 * Create the form structure for listing the avatars and managing them in the
 * 'Manage Avatars' tab under the Avatar Selection administration page.
 *
 * @param $form_state
 *   General variable, used to control the processing of the form.
 * @return
 *   Return the structure of the form.
 */
function avatar_selection_edit_form($form_state) {
  // We need the pager global variables.
  global $_GET;

  $form = array();

  if (!variable_get('user_pictures', 0)) {
    drupal_set_message(t('User Pictures option is disabled.  You will need to enable this option before you can use the Avatar Selection module.  You may configure this setting on the <a href="@url">User settings</a> page.', array('@url' => url('admin/user/settings'))));
  }

  drupal_add_css(drupal_get_path('module', 'avatar_selection') .'/avatar_selection.css');

  $set_type = arg(4);
  $set_id = arg(5);

  // We find out the current page number.
  $page = 0;
  if (isset($_GET['page']) && is_numeric($_GET['page'])) {
    $page = $_GET['page'];
  }

  $avatars_per_page = variable_get('avatar_selection_avatar_per_page', 30);
  $selects = _avatar_selection_image_list("", $set_type, $set_id, $page * $avatars_per_page, $avatars_per_page);
  if (!count($selects['avatars'])) {
    drupal_set_message(t('There are no avatars configured.'));
  }

  else {

    if (!isset($form_state['values'])) {
      $step = "list";
    }
    else {
      $step = "edit";
    }
    $form['step'] = array(
      '#type' => 'value',
      '#value' => $step,
    );


    if ($step == "list") {

      if ($set_type == 'role') {
        $sets = avatar_selection_handler_filter_role();
      }
      else if ($set_type == 'og') {
        $sets = og_all_groups_options();
      }
      drupal_set_title(t('Manage Avatars - %name', array('%name' => $sets[$set_id])));
      drupal_add_js(drupal_get_path('module', 'avatar_selection') .'/js/avatar_selection_pager.js', 'module', 'header');
      $js_file = drupal_get_path('module', 'avatar_selection') .'/js/avatar_selection.js';

      $form['select_avatar'] = array(
        '#type' => 'radios',
        '#title' => t('Select an avatar to edit'),
        '#options' => $selects['avatars'],
        '#required' => TRUE,
        '#attributes' => array('class' => 'user_avatar_select'),
        '#suffix' => theme('avatar_selection_pager', 'form#avatar-selection-edit-form', 'div.user_avatar_select', $selects['total'], $avatars_per_page, '/'. $js_file),
      );

      $form['search'] = array(
        '#type' => 'submit',
        '#value' => t('Edit'),
        '#submit' => array('avatar_selection_edit_list_form_submit'),
      );

      drupal_add_js($js_file, 'module', 'header');

    }
    else if ($step == "edit") {
      drupal_set_title(t('Manage Avatars'));
      $form['#redirect'] = array('admin/settings/avatar_selection/edit');
      $roles = avatar_selection_handler_filter_role();
      $aid = 0;
      $avs_access = $og_access = array();
      $weight = 0;
      $name = "";

      $result = db_query("SELECT avs.aid, avatar, name, weight, rid, ogid FROM {avatar_selection} avs LEFT JOIN {avatar_selection_roles} avsr ON avs.aid = avsr.aid LEFT JOIN {avatar_selection_og} avso ON avs.aid = avso.aid WHERE avatar = '%s' ORDER BY weight, name, avatar", $form_state['values']['select_avatar']);
      while ($avatar = db_fetch_object($result)) {
        $aid = $avatar->aid;
        $name = $avatar->name;
        $weight = $avatar->weight;
        if ($avatar->rid) {
          array_push($avs_access, $avatar->rid);
        }
        if ($avatar->ogid) {
          array_push($og_access, $avatar->ogid);
        }
      }

      $image_path = file_create_path('avatar_selection');
      $selected_avatar = $form_state['values']['select_avatar'];
      $image = theme('image', $image_path .'/'. $selected_avatar);
      $form['avatar_image'] = array('#value' => $image);
      $form['aid'] = array(
        '#type' => 'value',
        '#value' => $aid,
      );

      $form['select_avatar'] = array(
        '#type' => 'value',
        '#value' => $form_state['values']['select_avatar'],
      );

      $form['avatar_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Name'),
        '#description' => t("Image name or title which will be displayed when hovering over the image.  It's also used in conjunction with the weight setting for sorting the avatars."),
        '#size' => 48,
        '#default_value' => $name,
      );
      $form['avatar_weight'] = array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#delta' => 100,
        '#description' => t('Avatars with a lower weight appear before higher weighted avatars in lists.'),
        '#default_value' => $weight,
      );
      $form['permissions'] = array(
        '#type' => 'fieldset',
        '#title' => t('Permissions'),
        '#weight' => 1,
      );
      $form['permissions']['access'] = array(
        '#type' => 'checkboxes',
        '#title' => t('User Roles'),
        '#default_value' => $avs_access,
        '#options' => $roles,
        '#description' => t('Only the checked roles will be able to see this avatar icon; if no roles are checked, access will not be restricted.'),
      );

      if (module_exists("og")) {
        $form['permissions']['og_access'] = array(
          '#type' => 'checkboxes',
          '#title' => t('Organic Groups'),
          '#default_value' => $og_access,
          '#options' => og_all_groups_options(),
          '#description' => t('Only users in the checked organic groups will be able to see this avatar icon; if no groups are checked, access will not be restricted.'),
        );
      }

      $form['update'] = array(
        '#type' => 'submit',
        '#value' => t('Update'),
        '#weight' => 9,
        '#submit' => array('avatar_selection_edit_update_form_submit'),
      );
      $form['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete'),
        '#weight' => 10,
        '#submit' => array('avatar_selection_edit_delete_form_submit'),
      );
    }

  }

  return $form;
}


/**
 * Validate the submission.
 *
 * Ensure the number of avatars page setting is numeric.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_config_form_validate($form, &$form_state) {
  $error = FALSE;

  if ($form_state['values']['op'] == t('Update')) {
    if (!is_numeric($form_state['values']['avatar_per_page'])) {
      form_set_error('avatar_per_page', t('Must be a number.'));
      $error = TRUE;
    }
  }
}



/**
 * Validate the submission.
 *
 * Check if Delete has been chosen AND a checkbox has been selected.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_delete_form_validate($form, &$form_state) {
  if ($form_state['values']['op'] == t('Delete')) {
    if (count(array_filter($form_state['values']['images'])) == 0) {
      form_set_error('images', t('Please select images to delete.'));
    }
  }
}

/**
 * Submit the settings form in the Avatar Selection administration page.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_config_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == t('Update')) {
    // Save system variables.
    variable_set('avatar_selection_disable_user_upload', $form_state['values']['disable_user_upload']);
    variable_set('avatar_selection_force_user_avatar_reg', $form_state['values']['force_set_image_reg']);
    variable_set('avatar_selection_force_user_avatar', $form_state['values']['force_set_image']);
    variable_set('avatar_selection_avatar_per_page', $form_state['values']['avatar_per_page']);
    variable_set('avatar_selection_set_random_default', $form_state['values']['set_random_default']);
    variable_set('avatar_selection_distinctive_avatars', $form_state['values']['distinctive_avatars']);
    drupal_set_message(t('Configuration has been updated.'));
  }
}

/**
 * Submit the image upload form in the Avatar Selection administration page.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_upload_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];

  if ($op == t('Upload')) {
    // Get access settings.
    $access = array_keys(array_filter($form_state['values']['access']));
    $og = array();
    if (module_exists("og")) {
      $og = array_keys(array_filter($form_state['values']['og_access']));
    }
    $name = $form_state['values']['avatar_name'];
    $weight = $form_state['values']['avatar_weight'];

    // Scan for new files.
    if ($form_state['values']['scan_avatars'] == 1) {
      $count = _avatar_selection_scan_images($name, $access, $og, $weight);
      drupal_set_message(t('Scan complete: %added new avatars found. %deleted avatars removed.', array('%added' => $count['add'], '%deleted' => $count['delete'])));
    }

    // Save uploaded files.
    $dir = file_create_path('avatar_selection');
    $is_writable = file_check_directory($dir, 1);

    if ($is_writable) {
      // If required, validate the uploaded picture.
      $validators = array(
        'file_validate_is_image' => array(),
        'file_validate_image_resolution' => array(variable_get('user_picture_dimensions', '85x85')),
        'file_validate_size' => array(variable_get('user_picture_file_size', 30) * 1024),
      );

      if ($file = file_save_upload('picture_upload', $validators, $dir, FILE_EXISTS_RENAME)) {

        if (image_get_info($file->filepath)) {
          file_set_status($file, FILE_STATUS_PERMANENT);
          _avatar_selection_save_avatar_info(0, $file->filename, $name, $access, $og, $weight);
          drupal_set_message(t('New image saved.'));
        }
        else {
          file_delete($file->filepath);
          drupal_set_message(t('Uploaded file does not appear to be a valid image file. Please try again.'));
        }
      }
    }
    else {
      form_set_error('picture_upload', t('Directory not writable: !dir', array('!dir' => $dir)));
    }
  }
}

/**
 * Submit the image list form in the Avatar Selection - 'Manage Avatars' page.
 *
 * Function called when the Edit button is pressed.
 * Practically it sets the $form_state['rebuild'] value to true, which will
 * determine the rebuilding of the page.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_edit_list_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit the image list  form in the Avatar Selection - 'Manage Avatars' page.
 *
 * Function called when the Update button is pressed.
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_edit_update_form_submit($form, &$form_state) {
  $og = array();
  $access = array_keys(array_filter($form_state['values']['access']));
  if (module_exists("og")) {
    $og = array_keys(array_filter($form_state['values']['og_access']));
  }
  $file = $form_state['values']['select_avatar'];
  $name = $form_state['values']['avatar_name'];
  $weight = $form_state['values']['avatar_weight'];
  $aid = $form_state['values']['aid'];
  _avatar_selection_save_avatar_info($aid, $file, $name, $access, $og, $weight);
  drupal_set_message(t('Image updated.'));
}

/**
 * Submit the image list form in the Avatar Selection - 'Manage Avatars' page.
 *
 * Function called when the Delete button is pressed, and deletes the selected
 * avatar(s).
 *
 * @param $form
 *   General variable used in drupal, defining the structure & the fields of a
 *   form.
 * @param &$form_state
 *   General reference, used to control the processing of the form.
 */
function avatar_selection_edit_delete_form_submit($form, &$form_state) {
  $image = $form_state['values']['select_avatar'];
  $deleted = avatar_selection_image_delete($image);
  if ($deleted) {
    drupal_set_message(t('Image deleted.'));
  }
}

/**
 * Delete the specified avatar image.
 *
 * @param $image
 *   Path to the image to be deleted.
 */
function avatar_selection_image_delete($image) {
  $dir = file_create_path('avatar_selection');
  if (file_check_location($dir .'/'. $image, $dir)) {
    $aid = db_result(db_query("SELECT aid FROM {avatar_selection} WHERE avatar = '%s'", $image));
    if ($aid) {
      $result = db_query("DELETE FROM {avatar_selection} WHERE aid = %d", $aid);
      $result = db_query("DELETE FROM {avatar_selection_roles} WHERE aid = %d", $aid);
      $result = db_query("DELETE FROM {avatar_selection_og} WHERE aid = %d", $aid);
      file_delete($dir .'/'. $image);
      if (!image_get_info($dir .'/'. $image)) {
        return 1;
      }
    }
  }
  return 0;
}

/**
 * Create the SQL queries in order to save the avatar path and data into the
 * database, and perform them according to the SQL server type.
 *
 * @param $aid
 *   The avatar identifier.
 * @param $image
 *   The avatar image.
 * @param $name
 *   The avatar name, used as the image alternative text on the forms.
 * @param $access
 *   Array of roles - the value determines which user roles have rights to see
 *   the avatar.
 * @param $og
 *   Array of organic groups (optional).
 * @param $weight
 *   The weight of the avatar.  Lower weighted avatars appear before higher
 *   weighted avatars in the list.
 */
function _avatar_selection_save_avatar_info($aid, $image, $name, $access, $og = array(), $weight = 0) {

  // Add or update avatar_selection table.
  if ($aid) {
    $result = db_query("UPDATE {avatar_selection} SET name = '%s', weight = %d WHERE aid = %d AND avatar = '%s'", $name, $weight, $aid, $image);
    $result = db_query("DELETE FROM {avatar_selection_roles} WHERE aid = %d", $aid);
    $result = db_query("DELETE FROM {avatar_selection_og} WHERE aid = %d", $aid);
  }
  else {
    $result = db_query("INSERT INTO {avatar_selection} (avatar, name, weight) VALUES('%s', '%s', %d)", $image, $name, $weight);
    $aid = db_last_insert_id('avatar_selection', 'aid');
  }

  // Add access settings.
  if (is_array($access) && count($access)) {
    foreach ($access as $rid) {
      $result = db_query("INSERT INTO {avatar_selection_roles} (aid, rid) VALUES(%d, %d)", $aid, $rid);
    }
  }
  if (is_array($og) && count($og)) {
    foreach ($og as $ogid) {
      $result = db_query("INSERT INTO {avatar_selection_og} (aid, ogid) VALUES(%d, %d)", $aid, $ogid);
    }
  }
}

/**
 * Return a list of the existing roles.
 *
 * @return
 *   Return the role id(s).
 */
function avatar_selection_handler_filter_role() {
  $rids = array();
  $result = db_query("SELECT r.rid, r.name FROM {role} r ORDER BY r.name");
  while ($obj = db_fetch_object($result)) {
    $rids[$obj->rid] = $obj->name;
  }
  return $rids;
}

