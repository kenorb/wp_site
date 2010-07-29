// $Id: README.txt,v 1.1 2007/03/02 21:32:38 aymerick Exp $

When activated, the taxonomy_forceall module forces the 'all' parameter to taxonomy URLs.

For example, let's assume your site have this vocabulary:

Vocab
- Term 1
- - Term 2
- - Term 3
- Term 4
- - Term 5
- - Term 6

If you go to taxonomy/term/1, then you see all nodes in Term 1 listed, but NOT nodes in Term 2 and Term 3.

If you want to see all nodes in Term 1, Term 2 and Term 3 in a single page, you have to go to taxonomy/term/1/all.

By default, the 'all' parameter is never set by drupal core. This module solves this by simply implementing a hook_init() function that catch if requested page is taxonomy/term/x and transform it into taxonomy/term/x/all.
