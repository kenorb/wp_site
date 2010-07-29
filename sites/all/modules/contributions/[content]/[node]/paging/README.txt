Paging
==========

Overview
----------
Break long pages into smaller ones automatically by words/characters limit, or by means of a customizable pagination tag. For example:

    First page here.
    <!--pagebreak-->
    Second page here.
    <!--pagebreak-->
    More pages here.

Installation
----------
1. Copy the paging-6.x-x.x.tar.gz contents to sites/SITENAME/modules directory (usually sites/all/modules) and enable the module at admin/build/modules path.

2. Enable paging for the content types you want to use it on, in the paging configuration at admin/settings/paging path.

3. Then enable the paging filter for the relevant input format(usually Filtered HTML), in Input Formats configuration at admin/settings/filters path.

4. Your module is now setup and ready to be used.

Frequently Asked Questions
----------
1. How can I enable the paging module to use Clean URLs?

   Drupal's pager system does not support clean URLs by default. However, http://myzonelabs.com/node/19 lists the method to overcome that.

2. How much support does paging provide for CCK node types?

   Paging only works with content types using the core "Body" field.

3. How can I customize the pager navigation?

   Either the theme_pager() function can be overridden to make minor customizations (see http://drupal.org/node/173880#function-override) or a complete new(custom) theme function to use can be specified in paging settings at admin/settings/paging.

Support
----------
* If you experience a problem with paging module, file a request or issue on the paging queue at http://drupal.org/project/issues/paging. DO NOT POST IN THE FORUMS. Posting in the issue queues is a direct line of communication with the module authors.

Credits
----------
* Original module written by Marco Scutari.
* Rewritten and considerably shortened and made more Drupal-friendly by Earl Miles.
* Yet again rewritten, extended and currently maintained by Gurpartap Singh.