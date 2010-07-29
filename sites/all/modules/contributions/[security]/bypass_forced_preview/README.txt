;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Bypass forced preview module for Drupal 6
;; $Id: README.txt,v 1.3.2.1 2008/11/19 20:50:18 markuspetrux Exp $
;;
;; Original author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

Drupal core has the option to force users to use the preview button, before
being able to submit content. This option is pretty effective to prevent
certain bots from spamming your site. However, once this option is enabled,
it affects all users, including trusted users and site administrators.

This module adds a new permission that may allow privileged users bypass forced
post preview, both for content and comments.


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.

- Goto Administer > Users > Permissions and assign the 'bypass forced preview'
  permission to user roles of your choice.
