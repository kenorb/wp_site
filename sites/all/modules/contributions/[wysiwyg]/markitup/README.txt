// $Id:

NOTICE: The markItUp! JavaScript files are no longer included with this module, as per Drupal.org policy. You must download them and install them yourself. Download markItUp! at:
http://www.markitup.jaysalvat.com/downloads/

After expanding the markItUp! archive, copy the contents of its markitup directory into the markitup directory in the markitup module directory. (Does that make sense?)

ABOUT markItUP! Content Editor
------------------------------
The markItUp module enables the markItUp universal content editor jQuery plugin
for use in Drupal's textareas.

It provides users with helpful buttons and keyboard shortcuts to apply and insert
tags into textareas.  Additionally, users can turn on a live preview while
editing content.  This behaves much the same as a split view code editor does in
a WYSIWYG editor, with the exception that you can't edit in the preview area.

This editor is based on markItUp! 1.1 (GPL MIT licensed)
Web site: http://www.markitup.jaysalvat.com/home/
Documentation: http://www.markitup.jaysalvat.com/documentation/

INSTALLING ADDITIONAL SETS AND SKINS
------------------------------------
A "skin" is a custom interface style and a "set" is a collection of tags which
can be inserted into or applied to text. The jQuery plugin resides in the
"markitup" folder inside the markitup module folder. It is within there that you
can add new sets and skins in the corresponding folders.

IMPORTANT: Because Drupal wants stylesheets to have unique names, when you
install a set or skin in the markItUp! skins or sets folders you should make a
copy of the stylesheet and name it accordingly:
- for sets: directoryname-set-style.css
  (where directoryname is the name of the set's directory)
- for skins: directoryname-skin-style.css
  (where directoryname is the name of the set's directory)
It's a good idea to leave the original .css file intact for reference and in
case you need to make adjustments to the renamed .css file to integrate it into
your site.

IMPORTANT: When installing new sets, to avoid namespace collisions when invoking
sets, you must open each set's set.js file and rename the top level variable -
most likely "mySettings" - to "my_directoryname" (no quotes).

Once you've added a new set or plugin, you can tell the markitup module to use
it by visiting /admin/settings/markitup

Download skins and sets: http://www.markitup.jaysalvat.com/downloads/

DEVELOPER NOTES
---------------
- custom skin for Drupal
- custom set for Drupal (to match filtered html)
- implement button to insert <!--break-->, perhaps
- since switching to markItUp 1.1, live preview isn't working... need to fix
