<?php
// $Id: fasttoggle.author-pane.inc,v 1.1.2.1 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the fasttoggle module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function fasttoggle_preprocess_author_pane(&$variables) {
  // The full user object for the author
  $account = $variables['account'];

  $variables['fasttoggle_block_author'] = theme('links', fasttoggle_link('user', $account), array('class' => 'links inline'));
}
