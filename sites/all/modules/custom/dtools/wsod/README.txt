Got WSOD (White Screen of Death)?  Don't panic.
Before you will delete the all modules, make database restore or reinstalling the whole Drupal:
Give a try.

How to install:
1. Copy module to your Drupal module directory. (sites\all\modules\ ?)
2. Enable the module. (You can't? Goto emergency run section)
3. Module will detect WSOD automatically each page is loaded and log the debug errors in your watchdog.

Emergency run:
1. Copy module to your Drupal module directory. (sites\all\modules\ ?)
2. Open in example: http://example.com/sites/all/modules/dtools/wsod/wsod_emergency.php (this is only the example, please update your path argument)
   To detect specified path for WSOD, use 'q' argument.
3. Module will run Drupal core, install the WSOD module and will detect WSOD automatically.

How to uninstall:
1. Normally - http://drupal.org/handbook/

How it's working?
    Module simulate Drupal run and checking for common problems (like invalid return of menu_execute_active_handler)
    If it will detect WSOD, it trying to refresh all caches, rebuild module list and verify your Drupal installation.

TIPS:
- To check your WSOD for specified path in emergency mode:
  just try to add ?q argument into your URL (Example: wsod_emergency.php?q=forum)

FAQ:
1.  I've got Fatal error: Call to undefined function wsod_check_wsod()?

    - Make sure that you've copied WSOD module into modules directory.
    - run in emergency mode to execute module_rebuild_cache() to refresh module paths
    - check for duplicated WSOD modules in your filesystem

2. My .../wsod_emergency.php contain dots or is empty.
   That's fine, that mean there is no any WSOD (White Screen of Death detected) on default page.
   - go again to your default page (like http://www.example.com/) and make sure that WSOD is still there
   - if your WSOD is on specified path, try to add ?q argument into your URL (Example: wsod_emergency.php?q=forum)
   If you sure that there is still WSOD, so the problem can be not related to Drupal:
   - check your php.log for fatal errors
   - test if your web server is running correctly (Example: http://example.com/UPGRADE.txt)
   - test your server on fresh/other Drupal installation, maybe there is problem with some web server configuration
   - if you made some patches to Drupal core, make backup and replace all Drupal core files again (apart of settings.php)
   - migrate your Drupal website to your local environment and test it again on your WAMP/LAMP web server
     Still have problem?
     - Install XDebug debugging library and debug it: http://xdebug.org/
     - Send me an e-mail, maybe I'll be able to help you (kenorb@gmail.com)
     
3. My .../wsod_emergency.php returns unthemed 'Page not found'.

   That's mean there is no any WSOD (White Screen of Death detected) on this page,
   so the page was themed normally (but because of emergency run, path are different and CSS files couldn't be loaded)

4. My .../wsod_emergency.php returns standard 'Page not found'.

   - check your path to wsod_emergency.php file
        sites/all/modules/contributions/dtools/wsod/wsod_emergency.php
        sites/all/modules/dtools/wsod/wsod_emergency.php
        modules/dtools/wsod/wsod_emergency.php
        modules/wsod/wsod_emergency.php
        etc.

TODO:
- Show last watchdog entries on emergency run

