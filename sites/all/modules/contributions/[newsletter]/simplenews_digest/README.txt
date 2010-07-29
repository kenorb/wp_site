Module description
==================
This module works with the simplenews module to allow you to automatically generate digest emails to subscribed users. The emails contain a simple listing of links, grouped by node type, to nodes posted on the site since the last time a digest newsletter was sent out. T
he following can be configured:

1) How frequently the digest newsletter gets sent (days/weeks/months)
2) Which newsletter should act as the digest newsletter
3) Which node types and taxonomy terms to send  
4) How node types should be labeled in the newsletter
5) What time of day the newsletter should be sent out

Example use
===========
Let's say you want to send out a newsletter every two weeks on Friday at midnight. Go to admin -> content management -> newsletters -> settings -> digest newsletter settings.

1) Select the newsletter you wish to use for the digest newsletter
2) Set the time of day you wish the newsletter to go out to 00:00.  
3) For the frequency settings, select "Week", "2", and under the "Weekly Settings", select "Friday"
4) Under "Digest content", select which node types and which categories to place into the digest.
5) If you wish, you can change how the node types appear in the email in the "Rename content types" section

Installation
============
Simply drop the module in /sites/all/modules and enable. No database table is needed for this module.

TODO
====
-Make the email a little nicer to look at.
-If no terms are selected, default to all terms in a vocabulary for the selected node types
