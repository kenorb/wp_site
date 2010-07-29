$Id: README.txt,v 1.31 2008/12/03 00:16:39 wimleers Exp $

Description
-----------
This module defines the "hierarchical_select" form element, which is a greatly
enhanced way for letting the user select items in a hierarchy.

Hierarchical Select has the ability to save the entire lineage of a selection
or only the "deepest" selection. You can configure it to force the user to
make a selection as deep as possible in the tree, or allow the user to select
an item anywhere in the tree. Levels can be labeled, you can configure limit
the number of items that can be selected, configure a title for the dropbox,
choose a site-wide animation delay, and so on. You can even create new items
and levels through Hierarchical Select!


Integrates with
---------------
* Book (Drupal core) – NOT YET PORTED TO DRUPAL 6
* Forum (Drupal core)
* Menu (Drupal core)
* Taxonomy (Drupal core)
* Content Taxonomy (http://drupal.org/project/content_taxonomy) – NOT YET PORTED TO DRUPAL 6
* Taxonomy Subscriptions (http://drupal.org/project/subscriptions) – NOT YET PORTED TO DRUPAL 6
* Views – NOT YET PORTED TO DRUPAL 6


Installation
------------
1) Place this module directory in your "modules" folder (this will usually be
"sites/all/modules/"). Don't install your module in Drupal core's "modules"
folder, since that will cause problems and is bad practice in general. If
"sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) If you want to use it for one or more of your vocabularies, go to
admin/content/taxonomy and click the "edit" link for a vocabulary. Now scroll
down and you'll find a whole range of Hierarchical Select settings. All
settings are explained there as well.


Troubleshooting
---------------
If you ever have problems, make sure to go through these steps:

1) Go to admin/logs/status (i.e. the Status Report). Ensure that the status
   of the following modules is ok:
   - Hierarchical Select
   - Hierarchical Select Taxonomy Views (if installed)
   - Hierarchical Select Content Taxonomy Views (if installed)

2) Ensure that the page isn't being served from your browser's cache. Use
   CTRL+R in Windows/Linux browsers, CMD+R in Mac OS X browsers to enforce the
   browser to reload everything, preventing it from using its cache.

In case of problems, don't forget to try a hard refresh in your browser!


Limitations
-----------
- Creating new items in the hierarchy in a multiple parents hierarchy (more
  scientifically: a directed acyclic graph) is *not* supported.
- Not the entire scalability problem can be solved by installing this set of
  modules; read the maximum scalability section for details.
- The child indicators only work in Firefox. This *cannot* be supported in
  Safari or IE. See http://drupal.org/node/180691#comment-1044691.
- The special [save-lineage-termpath] token only works with content_taxonomy
  fields as long as you have the "Save option" set to either "Tag" or "Both".


Maximum scalability
-------------------
While the hs_taxonomy, hs_book, hs_menu and hs_subscriptions modules override
existing form items, those form items are *still* being generated. This can
cause scalability issues.
If you want to fix this, you *will* have to modify the original modules (so
that includes Drupal core modules). Simply move the changes from the
hook_form_alter() implementations into the corresponding form definitions.


Addressing Views exposed filters display issues
-----------------------------------------------
When using Hierarchical Select to alter an exposed Views filter (i.e. an 
exposed taxonomy filter), you may run into display issues due to the Views
method theme_views_filters(), which renders filters as columns within a table.
This results in behavior where the select boxes within the Hierarchical Select
widget may split onto multiple lines or be too cramped together. The following
theme override sample, written for the Zen theme, renders the filters as rows 
within a table, leaving much more room for the Hierarchical Select widget. To
use, follow the instructions for overriding theme functions described at 
http://drupal.org/node/55126.

<?php
function zen_views_filters($form) {
  $view = $form['view']['#value'];
  $rows = array();
  $form['submit']['#value'] = t('Search');
  if (isset($view->exposed_filter)) {
    foreach ($view->exposed_filter as $count => $expose) {
      $rows[] = array(
        array('data' => $expose['label'], 'header' => TRUE),
        drupal_render($form["op$count"]) . drupal_render($form["filter$count"]),
      );
    }
  }
  $rows[] = array(
    array('data' => '', 'header' => TRUE),
    drupal_render($form['submit'])
  );
  if (count($rows) > 1) {
    $output = drupal_render($form['q']) . theme('table', array(), $rows) . drupal_render($form);
  }
  else {
    $output = drupal_render($form);
  }
  return $output;
}
?>


Sponsors
--------
* Initial development:
   Paul Ektov of http://autobin.ru.
* Abstraction, to let other modules than taxonomy hook in:
   Etienne Leers of http://creditcalc.biz.
* Support for saving the term lineage:
   Paul Ektov of http://autobin.ru.
* Multiple select support:
   Marmaladesoul, http://marmaladesoul.com.
* Taxonomy Subscriptions support:
   Mr Bidster Inc.
* Ability to create new items/levels:
   The Worx Company, http://www.worxco.com.


Author
------
Wim Leers

* website: http://wimleers.com/
* contact: http://wimleers.com/contact

The author can be contacted for paid development on this module. This can vary
from new features to Hierarchical Select itself, to new implementations (i.e.
support for new kinds of hierarchies).
