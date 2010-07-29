;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Formatted Number CCK module for Drupal
;; $Id: README.txt,v 1.1.2.2 2008/11/15 13:56:19 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

The Formatted Number CCK module defines numeric types where thousands separator
and decimal point are inherited from the Format Number API module.

Numeric types supported: signed or unsigned integers (tiny, small, medium, int),
decimal and float.


REQUIREMENTS
============

- CCK (http://drupal.org/project/cck)
- Format Number API (http://drupal.org/project/format_number)


INSTALLATION
============

- Please, make sure all required modules are installed first.

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.

- Create or edit content types and start adding Money fields. :)
