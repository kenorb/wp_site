$Id: README.txt,v 1.11.2.6 2009/01/27 23:31:11 michellec Exp $

CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Installation
 * Other configuration
 * Credits
 
INTRODUCTION
------------
Advanced Forum (http://drupal.org/project/advanced_forum) enhances Drupal's forum module to provide the look and, with the help of other modules, much of the functionality found in common forum software. Because it uses the core forum module, it uses the node and comment system built into Drupal and is completely integrated, not a bridge.

INSTALLATION
------------

1. Enable all dependencies: Author Pane ( http://drupal.org/project/author_pane ), Forum, 
   Taxonomy, Comment. (Optionally: Statistics)
   
2. Copy the entire advanced_forum project directory (not just the contents) to your normal 
   module directory (ie: sites/all/modules)
   
3. Enable the advanced forum module at ?q=admin/build/modules

4. Visit the Advanced Forum settings page at ?q=admin/settings/advanced-forum 
   "Advanced forum style directory" Enter the name of the style you are using. 
   See http://drupal.org/node/234042 for more information on this.
   "Treat all site comments like forum comments" If you would like advanced forum to take 
   over the theming of all comments, even those outside the forum, choose yes here.

OTHER CONFIGURATION
-------------------
   
1. Forum settings ( ?q=admin/content/forum/settings ) 
    * Hot topic threshold: Up to you.
    * Topics per page: Up to you.
    * Default order: "Date - newest first" so the most recent posts are at the top of the 
      topic list.
2. Select content types to use in forums ( ?q=admin/content/taxonomy ) 
3. Edit the forum vocabulary
   * Check all content types you want to use in forums.
4. Comment settings ( ?q=admin/content/node-type/forum ) [Note: do this for each content 
   type used in forums] 
   * Expand "Comment settings" fieldset.
   * Default comment setting: "Read/write"
   * Set Default display mode: Flat list - expanded. (Advforum is intended to be used 
     flat. Using it threaded should mostly work but is unsupported and may have some 
     issues.) 
   * Default display order: Date - oldest first 
   * Default comments per page: Up to you. (If you chose to have a threaded forum, 
     setting this number to the maximum will reduce issues with pagination and threading.) 
   * Comment controls: "Do not display" is recommended.
   * Anonymous commenting: Up to you.
   * Comment subject field: Up to you. If disabled, advforum will not display the Drupal 
     default subject, which is the first few words of the comment.
   * Preview comment: Up to you.
   * Location of comment submission form: Up to you. Displaying below provides a non-ajax 
     quick reply.
5. User settings ( ?q=admin/user/settings ) 
   * Signature support: Enabled
   * Picture support: Enable this for avatars in the forum.
   * Picture maximum dimensions: If you change this from the default 85x85, you will want 
     to size it in either CSS or with imagecache to avoid breaking the forum layout.
6. Statistics settings ( ?q=admin/reports/settings ) 
   * Enable access log: Enabled
   * Count content views: Enabled - Needed for topic views count.   
    
CREDITS
-------
Developer and maintainer: Michelle Cox ( http://drupal.org/user/23570 )

Advanced forum was originally based on flatforum. Though there is little or no code left
from that module, its authors deserve credit for the idea.

The Naked style was created by stephthegeek (http://drupal.org/user/47874). Previous 
theme work was done by eigentor (http://drupal.org/user/96718) and jacine 
(http://drupal.org/user/88931)

Icons provided by paris (http://drupal.org/user/14747), yoroy (http://drupal.org/user/41502), Psicomante (http://drupal.org/user/29518), and lullabot (http://lullabot.com) 
