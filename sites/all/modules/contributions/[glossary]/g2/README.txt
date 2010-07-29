=============================================================================
Glossary2 (G2)
-----------------------------------------------------------------------------
 @copyright (C) 2005-2007 Frederic G. MARAND
 @license Licensed under the CeCILL, version 2
 @version $Id: README.txt,v 1.7.2.2 2007/08/25 13:12:27 fgm Exp $
=============================================================================

0. Table of contents
--------------------

  0. Table of contents
  1. Introduction
  2. Prerequisites
  3. Version warnings
  4. Installing / upgrading / uninstalling
  5. Notice

1. Introduction
---------------

G2 is a glossary management module written for Drupal.

It is not intended as a direct replacement for glossary.module
which has been already available with drupal for quite some time now,
but as an alternative for sites needing a glossary for a large number of entries,
or a different feature set.

Unlike glossary.module, it uses nodes for its definitions, and does not
automatically link terms in other nodes to their entries in G2, but relies on
<dfn> markup in definitions, and can link to terms in multi-byte character sets
and terms containing special characters like slashes or ampersands without
specific markup.

Its development to this date has been entirely sponsored by OSInet.fr

A word of warning: due to G2 targeting high-volume glossaries and including
its own APIs and features, the code is heavier than glossary.module: the base
version source for G2 is more than twice the volume of the base version
source for glossary.module. It may be too heavy for sites on very limited
hosting plans or unable to use clean URLs. On the other hand, it handles
glossaries with thousands of nodes without slowing or increasing its memory
requirements significantly.

Project page on Drupal.org:
        http://drupal.org/project/g2
Documentation wiki (contributions welcome)
        http://wiki.audean.com/g2/start
Sample implementation (over 5000 terms, localized to french)
        http://www.riff.org/glossaire

2. Prerequisites
----------------

  * Drupal 6.0 pre-beta
  * MySQL 4.1.x, configured for UTF-8 encoding (collating: utf8_general_ci)
  * PHP 5.1

3. VERSION WARNINGS
-------------------

As of 2007-08-19:
- the DRUPAL-6--1 branch is NOT STABLE yet
- sites not configured with clean URLs are no longer taken into account
- the module is only maintained/evolved for the DRUPAL-4-7 and
  DRUPAL-6--1 branches.
- A 5.0 version will not be considered unless someone writes the
  port or sponsors it (shouldn't cost much).

4. Installing / upgrading / uninstalling
----------------------------------------

Installing and upgrading within the DRUPAL-6--1 branch is taken care of thanks
to the new mechanisms provided by Drupal.

WARNING: Should you want to uninstall the module, take care to first remove
all G2 nodes before removing the module: this includes your glossary
definitions, and the unpublished page used for the glossary home page skeleton.
Unless you do this, you will have inconsistent nodes in your system, because
Drupal will be missing the module to load G2 entries. If you do not modify
any of these nodes, reinstalling the module will restore consistency and
enable a clean noed deletion and uninstall later on.

Upgrading from the 4.7 branch to the 6.1 branch is NOT INCLUDED, so it needs
some manual help. Contact the maintainer if the need arises.

5. NOTICE
---------

The statistics displayed on the "entries starting by initial ..." page
at URL <drupal>/g2/initial/<some initial segment> mention :

"Displaying 0 entries starting by ... from a total number of ... entries.

It must be understood that this "total number" is actually the total number
a user without administrative permissions can see, that is, published entries.
The "published" epithete is not used because site visitors are not expected
to be aware of the publishing process.


