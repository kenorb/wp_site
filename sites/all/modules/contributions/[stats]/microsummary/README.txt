                         ****Microsummary Module****
// $Id: README.txt,v 1.3 2007/01/17 04:08:43 greggles Exp $

**Description:
This module provides a simple "microsummary" of a Drupal site.

See The Mozilla Wiki about microsummaries for details on possible uses.
http://wiki.mozilla.org/Microsummaries

Possible uses include providing a count of new or total users, a count of total 
nodes on a site, a count of certain kinds of content added to the site, the 
name of the most recent user: anything that changes on a semi regular basis.

By default this module currently displays three summaries:
1) a count of new and updated nodes for the user
2) a listing of registered users and active users
3) The most recent post title and the post time - useful for a blog site

The three default microsummaries can be disabled if they are not appropriate
for your site (or if you want to provide a better version.

Users can also specify a node which contains content to be used to create the 
microsummary.  The intended use of this feature is to have a PHP Snippet in the
node and then This completely opens up the functionality of the module.  The 
module respects the input format of the node so your site is protected from
accidentally running PHP that you did not intend to run.  


**Install and Usage:
Basic case:
1. Enable the module on the admin/modules page
2. Visit admin/settings/microsummary and set a prefix or suffix for your summary
3. Using Firefox2 (or any other enabled browser) select the "Add Bookmark" 
dialog and you should see a dropdown option for the "Title" of the bookmark to 
be a Microsummary or "Live Title"

Configuring additional Microsummaries:
1. Create a new node that contains your microsummary - if you are using PHP code
to create the summary, be sure that you have selected the PHP Input format. Make 
a note of node number for this summary.  You can use any kind of node that has 
a body, though I suggest the page or story node type.
2. In the microsummary settings page enter the node id of the node that you just
created into the "Microsummary nodes" text area and save the settings.
3. You should now be able to view this custom microsummary in your browser's 
"Add bookmark" dialog as in the steps described above.

**Future Plans
There are no more real future plans aside from "bugfixing" because the module 
now has the concept of user configurable microsummaries.  If you have any ideas
for how to improve the module, see the last line of this file for the location
to report new feature requests.

**Credits:
This module developed by Greg Knaddison - Drupal user greggles.

Send thanks to: 	greg AT knaddison DOT com

Send problems/questions to: 	http://drupal.org/project/issues/microsummary

**Performance note (squeezing blood from a stone):
The microsummary module includes special links to the microsummaries in each
page load.  By default this happens in the hook_footer.  However, the content of
these links does not change very much between each pageload so there could be
some benefit to caching this output.  For better performance it is possible to 
provide these links as a block.  In Drupal4.7 and 5.x the Block Cache module can
cache the output of these blocks to reduce the load on your site.  This is a 
very minor consideration, but if you use the block cache module then it might be
a good option for you to use this setting.  To enable it, you must disable the 
"Default linking method" method on the Microsummary settings page AND enable the 
Microsummary Block