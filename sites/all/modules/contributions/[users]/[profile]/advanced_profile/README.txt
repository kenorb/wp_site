$Id: README.txt,v 1.10.4.7 2009/01/05 05:19:39 michellec Exp $

!!!THESE DIRECTIONS ARE FOR THE D5 VERSION. THEY STILL NEED TO BE REWRITTEN FOR D6.!!!

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installation

INTRODUCTION
------------
Advanced Profile Kit (http://drupal.org/project/advanced_profile) provides building
blocks for putting together fancy user profile pages like those commonly found on social
networking sites.

INSTALLATION
------------
1. Install <a href="http://drupal.org/project/panels">Panels 2</a>,
   <a href="http://drupal.org/project/views">Views</a>,
   <a href="http://drupal.org/project/cck">CCK</a>,
   <a href="http://drupal.org/project/bio">bio</a>, and
   <a href="http://drupal.org/project/link">Link</a>.

2. Copy the entire advanced_profile module directory into your normal directory for
   modules, usually sites/all/modules

3. Copy theme/advanced_profile_author-pane.tpl.php and theme/advanced_profile_view_user-list.tpl.php to your theme.

4. Enable the advanced profile module in ?q=admin/build/modules

5. Navigate to ?q=admin/user/settings and enable picture support if you want users to
   have avatars on their profiles.

6. Advanced Profile can create the profile node type for you. It comes with fields
   typically found on user profiles. You can further edit this type to add or remove
   fields. If you prefer to use your own node type, skip this step. If you just want to
   add the fields, you can use the content copy module directly and import the contents
   of uprofile.inc. If you skip this step, you will need to delete the uncreated groups
   from the panels page.

    a) Navigate to ?q=admin/settings/advanced-profile
    b) Profile type auto creation: Check this box and save configuration. This option
       will be disabled once the node type exists and you will need to delete your node
       type if you want to re-create it. If you get errors with this step, it is due to
       issues with CCK's content copy module. Try importing uprofile.inc directly at
       ?q=admin/content/types/import
    c) Navigate to ?q=admin/content/types/uprofile/display and change the labels to
       inline as needed. This is not carried over via content copy.

7. Configure Advanced Profile Kit
    a) Navigate to ?q=admin/settings/advanced-profile
    b) If you want to use your own images, fill in the path to your image directory.
    c) The granularity on the profile vists list defaults to 2. Set it to 1 to take up
       less room or to 3 for more precision.

8. Configure Bio:

    a) Navigate to ?q=admin/user/bio
    b) Content type for user biographies: Choose the type you are using to hold the
       profile data. (User Profile if you had Advanced Profile create it)
    c) Display bio link: Up to you
    d) Use bio for user profiles: Check this box
    e) Takeover profile: DON'T check this box (panels handles it)
    f) Show fields on registration form: Up to you. If you don't put any fields on the
       reg form, the user's profile node will not be created until they go to the edit link.

9. Set access control ( ?q=admin/user/access ): This will vary from site to site, but you
   will want to look at these items in particular:

    a) access user profiles
    b) create uprofile content
    c) edit uprofile content
    d) edit own uprofile content

   Also note: The included User Listing view provides a list of all profile nodes. By
   default, it is visible to all roles at "?q=user-list". If you have profile information
   restricted on your site, you will want to enable access control on this view. Navigate
   to ?q=admin/build/views/add/user_list and check the roles under "access".
