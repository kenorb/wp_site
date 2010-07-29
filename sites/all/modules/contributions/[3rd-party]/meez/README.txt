                         ****Meez Module****
// $Id: README.txt,v 1.3 2008/07/07 21:59:26 greggles Exp $

**Description:
This module provides integration with the Meez avatar/profile site.  Meez is a 
system for creating a centralized avatar that you can use on multiple sites.  
Visit http://www.meez.com/ for more details.

This module uses a profile field to hold the Meez username for the user.  By 
default the module uses profile_meez for the name of the profile field but
you can change that on the settings page.

The module does not actually load the Meez avatar files onto your server.  
Instead it stores the URL of the files on the Meez server.  This way any updates 
to the avatar happen as soon as they are published on Meez.  

By default the module loads the URLs for all of the available avatars.  You can
configure it to only load a subset of the avatars for example if you only need a 
headshot and not the full body nor animation Meez.

**Install and Usage:
Pre-installation:
1. Enable profile module
2. Create a single line text field profile field to hold the Meez username.  The
default name is profile_meez but you can call it whatever you want.

Basic case:
1. Enable the module on the admin/modules page
2. Optionally Visit admin/settings/meez and set the profile field and the list
of avatars that should be imported.
3. You MUST alter your theme to include the meez avatar.

A simple example of theme integration is to add this to your template.php:
function THEMENAME_user_meez($account,$version) {

  if ($account->avatars[$version]->src) {
    $picture = $account->avatars[$version]->src;
  }
  else if (variable_get('user_picture_default', '')) {
    $picture = variable_get('user_picture_default', '');
  }

  if (isset($picture)) {
    $alt = t("@user's profile", array('@user' => $account->name ? $account->name : variable_get('anonymous', t('Anonymous'))));
    $picture = theme('image', $picture, $alt, $alt, '', FALSE);

    if (!empty($account->uid) && user_access('access user profiles')) {
      $picture = l($picture, "user/$account->uid", array('title' => t("View @user's profile.", array('@user' => $account->name))), NULL, NULL, FALSE, TRUE);
    }

    return '<div class="picture">'. $picture .'</div>';
  }
}

Then in your theme file, such as user_profile.tpl.php you need to call the 
function like 

<?php
  theme('user_meez', $account, 'body_sm')
?>

See http://drupal.org/node/35728 for more details on customizing the user profile.

**Future Plans
Possible enhancements include:
1. Syncing with more of the profile fields from Meez in addition to avatar.
2. Using a storage location other than the user->data field.

**Credits:
This module was developed by Greg Knaddison (greggles) of pingVision, LLC (www.pingv.com)
sponsored by, and drawing from code elements initially developed by, DoSomething.org. 

Send thanks to: 	http://drupal.org/user/36762/contact
Send problems/questions to: 	http://drupal.org/project/issues/meez
