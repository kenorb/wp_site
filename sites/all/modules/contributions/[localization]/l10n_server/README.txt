$Id: README.txt,v 1.1.2.9 2007/12/27 16:05:04 goba Exp $

Localization server module suite
--------------------------------------------------------------------------------
Project page:  http://drupal.org/project/l10n_server
Support queue: http://drupal.org/project/issues/l10n_server

ABOUT
--------------------------------------------------------------------------------

The localization server project (formerly known as lt_server) provides a 
community localization editor, which allows people from around the world to 
collaborate on translating Drupal projects to different languages. It is
inspired by Launchpad Rosetta (https://launchpad.net/rosetta) but is highly
tailored to Drupal needs.

The module suite solves the Drupal project translation problem with a web
based interface. The various Drupal projects release source code on a daily
basis. The goal is to help translators to keep up with this pace by sharing
already translated strings and distributing translations effectively.

The localization server module suite consists of a few possible components:

 - l10n_community: Required. A translation community interface which provides
   the database backend to store projects and releases, but does not fill these
   with actual data itself. Uses a role based permission model.
   
 - l10n_groups: Optional. An "Organic Groups" module binder, which provides
   permission handling based on language groups (in addition to the default
   role based model used by l10n_community). 
   
 - A connector module: One required, only use one at a time. Connectors serve
   the purpose of filling in the actual list of projects, releases and strings
   used in the released packages. Different connectors allow this suite to be
   used in different use cases.
   
     - l10n_localpacks: Works based on a list of files uploaded to a local
       file system directory. The projects and releases are identified based
       on placement and naming of the package files. 
     
     - l10n_drupalorg: To be used on drupal.org. Maintains a relation with
       the drupal.org project and release listing, downloads tarballs,
       collects translatables automatically.

INSTALLATION
--------------------------------------------------------------------------------

- Your PHP installation should have the PEAR Tar package installed (which
  requires zzlib support). This is required for Tar extraction (in the
  l10n_localpacks and l10n_drupalorg modules) and Tar generation (in the
  l10n_community module).

- With l10n_drupalorg module, files are simply copied from the drupal.org
  server, so allow_url_fopen needs to be enabled.

- Locale (built into Drupal) is required. Organic Groups
  (http://drupal.org/project/og) is required by l10n_groups.

- This package makes use of Unicode fonts to create text-based icons. If
  your icons do not display properly, test your browser for compatibility
  with the Unicode 'Dingbats' block at the URL
  http://www.fileformat.info/info/unicode/block/dingbats/utf8test.htm.
  Note: It may be necessary to install a font that supports characters in the
  Unicode 'Dingbats' block. A list of these fonts can be found at the URL
  http://www.fileformat.info/info/unicode/block/dingbats/fontsupport.htm.

1. Enable l10n_community and *only one* of the connector modules at
   Administer > Site configuration > Modules. Optionally enable l10n_groups.

2. Configure the connector at Administer > Localization Server.

HOW DOES IT WORK
--------------------------------------------------------------------------------

The connector module's duty is to maintain a list of projects and releases, as
well as fill up the database with translatable strings based on release source
codes. This module consumes a huge amount of resources. Downloading packages,
unpacking their contents and running the string extraction process takes time,
CPU cycles and hard disk space. Although only temporary copies of the packages
are kept, some hard disk space and a decent amount of memory is required. This
is why connectors are preconfigured to scan only one project at a time. Big
projects like E-Commerce or Drupal itself take considerable time to parse.

The localization community module provides the actual interface. Users with
proper permissions can suggest new translations for strings, maintainers can
even decide on the official translation based on the different suggestions. To
translate a project, go to Translations, choose a language and optionally
choose a project. There you can translate all strings.

DEVELOPERS
--------------------------------------------------------------------------------

This module suite is in heavy development now. Better cooperation with existing
modules and more interoperability functions are planned. The goal is to have
this module used as the official translation interface for Drupal modules.
We take pride by coding to Drupal, PHP E_ALL and E_STRICT coding standards,
as well as XHTML Strict and CSS 2 compliance.

CONTRIBUTORS
--------------------------------------------------------------------------------
Bruno Massa  http://drupal.org/user/67164 (original author)
Gábor Hojtsy http://drupal.org/user/4166 (current maintainer)

This module was originally sponsored by Titan Atlas (http://www.titanatlas.com),
a Brazilian computer company, and then by Google Summer of Code 2007. The
localization server is currently a free time project.
