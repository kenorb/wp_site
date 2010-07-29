$Id: README.txt,v 1.1.2.1 2007/10/16 18:27:30 deviantintegral Exp $

= search_block.module =

Search Block is a module which lets administrators exclude individual nodes or
entire content types from the search system. This is especially useful for
pages used with Login Destination (http://drupal.org/project/login_destination/),
or CCK nodes primarily meant to be presented only with views and not as
individual nodes.

== Installation ==

Follow standard Drupal module installation procedures. Place the module in
/modules or within your site's modules directory, and enable it at
admin/build/modules. Currently, only MySQL is implemented by hook_install; if
you create a schema appropriate for PostgresSQL, please submit an issue for it
and I will include it.

== Using search_block ==

Once the module is enabled, there are two options. Within individual edit pages
for nodes, you can choose to block the individual node from the search index.
Using the edit page for any content type (admin/content/types), you can choose
to block or allow searches within the Workflow fieldset. Note that changing
this value *will* override any previously set individual node settings, losing
them unless you have the {search_block} table backed up.

When an existing node is added or removed from the search index, the changes
are only made when your cronjob is run. Changes are noted (rather verbosely at
the moment) in the watchdog log. If you change settings for a content type with
thousands of nodes, expect a bit of a delay.

--
Developed by Andrew Berry for Pinchin Environmental
mailto:andrewberry@sentex.net
Project Page: http://drupal.org/project/search_block