// $Id: README.txt,v 1.6.2.4 2008/06/10 12:57:40 profix898 Exp $

Overview
---------
This module embeds Gallery2 (http://gallery.menalto.com) inside your Drupal
installation to support photos, videos and other content. There is support for
2 types of image blocks and a navigation menu, inserting of images into nodes
via an input filter, TinyMCE support, syncing of users between the two
applications, ...

Note that you will need to download and install Gallery2
(http://gallery.menalto.com/downloads) prior to installing this module.

Requirements
------------
* Gallery 2.3
* Drupal 6.x

Installation
------------

1. Install Gallery 2 and Drupal (see their respective installation documents).
Gallery2 should be *inside* your Drupal installation so that it's accessible
by the same website. If you installed Gallery somewhere else, you can create
a symlink to it from inside your Drupal site or you can move Gallery2.

Warning - Do not name your G2 directory 'gallery', call it something else
(e.g. 'gallery2'). Calling it 'gallery' will cause a conflict with the Drupal
gallery.module as Drupal assumes that http://www.example.com/gallery refers
to the Drupal module.

2. Ensure that 'Image Block' and 'Image Frame' plugins are installed and
activated in Gallery2. Check them in the non-embedded Gallery.

3. Log out of Gallery 2 (non-embedded) - people have reported errors relating
to sessions if this is not done.

4. Copy gallery.module to your Drupal modules/ directory.

5. Enable the gallery module in Administer ->  Site building -> Modules in
your Drupal installation (admin/build/modules).

6. Go to Administer ->  Site configuration -> Gallery (admin/settings/gallery)
and configure the path to your Gallery2 installation using the install wizard
provided there.

7. Enable the 'Gallery Block' in Administer -> Site building -> Blocks
(admin/build/block).

- Optional -
8. The Gallery user administration is now available in Administer -> User
management -> Gallery users (admin/user/gallery). You should at least check
the 'User settings' (admin/user/gallery/settings).

9. G2Image (Gallery G2Image module):
If you install G2Image (http://g2image.steffensenfamily.com) as either a
standalone or TinyMCE plugin you will have an excellent image chooser for
Drupal/Gallery2.

Support
-------
Please post bug reports and feature requests in the Issues tracker
(http://drupal.org/project/issues/gallery), support requests should generally
go to the Gallery 2 Integrations Forums (http://gallery.menalto.com/forum/78)
first.

Authors
-------
James Walker <walkah@walkah.net>
Kieran Parsons <kiz_0987@yahoo.com>
Thilo Wawrzik <drupal@profix898.de>
