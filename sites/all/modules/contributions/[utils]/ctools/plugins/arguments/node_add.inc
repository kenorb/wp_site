<?php
// $Id: node_add.inc,v 1.2 2008/12/17 08:29:21 merlinofchaos Exp $

/**
 * @file
 *
 * Plugin to provide an argument handler for a Node add form
 */

/**
 * Implementation of specially named hook_ctools_arguments().
 */
function ctools_node_add_ctools_arguments() {
  $args['node_add'] = array(
    'title' => t("Node add form"),
    // keyword to use for %substitution
    'keyword' => 'node_type',
    'description' => t('Creates a node add form context from a node type argument.'),
    'context' => 'ctools_node_add_context',
  );
  return $args;
}

/**
 * Discover if this argument gives us the node we crave.
 */
function ctools_node_add_context($arg = NULL, $conf = NULL, $empty = FALSE) {
  // If unset it wants a generic, unfilled context.
  if (!isset($arg)) {
    return ctools_context_create_empty('node_add_form');
  }

  if (array_filter($conf['types']) && empty($conf['types'][$arg])) {
    return NULL;
  }

  return ctools_context_create('node_add_form', $arg);
}
