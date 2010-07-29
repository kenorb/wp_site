;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Checkall module for Drupal 6
;; $Id: README.txt,v 1.2.2.2 2008/11/18 22:49:24 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CONTENTS
========
* OVERVIEW
* INSTALLATION
* USAGE
* CREDITS


OVERVIEW
========

This module provides an extension to the builtin checkboxes element that allows
developers add "check all" / "uncheck all" actions to them. This is specially
useful for checkboxes that have a lot of elements.

Please note that Drupal 6 already has this feature available, but only for
checkboxes rendered in tables, such as those used in Administration -> Content
management -> Content. Checkboxes rendered elsewhere do not have this feature.

You may not need to install this module unless it is required/recommended by
another contributed module, and/or you need it for your own custom modules.


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.


USAGE
=====

To attach the checkall behavior to checkboxes elements, all you need to do
is add the '#checkall' property to them. You can do so for your own forms, or
you can implement hook_alter_form() to add this property to existing forms.

Example:

  $form['foobar'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => $default_value,
    '#checkall' => TRUE,  // <--- this is it ;-)
  );


CREDITS
=======

The first version of the module was written by rszrama [1] and it is still
available from the Drupal 5 branch of the project.

Many thanks to him for letting me use his project for the implementation
of this checkboxes addon for Drupal 6.

[1] http://drupal.org/user/49344
