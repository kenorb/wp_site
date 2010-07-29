<?php
// $Id: taxonomy_breadcrumb.inc,v 1.1 2009/01/12 02:19:26 mgn Exp $

/**
 * @file
 * helper functions for taxonomy_breadcrumb
 */

/**
 * This function overrides the core taxonomy_term_page.  First, call the core
 * taxonomy_term_page.  Then, alter the breadcrumb trail.  This module's
 * hook_menu and a module weight greater than taxonomy's ensure this
 * function gets called for the taxonomy/term path (the module weight is
 * in the system table and is set in taxonomy_breadcrumb.install).
 */
function _taxonomy_breadcrumb_term_page($str_tids = '', $depth = 0, $op = 'page') {
  // Include the .inc file with all helper functions
  require_once drupal_get_path('module', 'taxonomy') .'/taxonomy.pages.inc';

  // Call the core taxonomy_term_page function
  $output = taxonomy_term_page($str_tids, $depth, $op);

  // Use first term to generate breadcrumb trail
  $terms = taxonomy_terms_parse_string($str_tids);
  $breadcrumb = _taxonomy_breadcrumb_generate_breadcrumb($terms['tids'][0], TRUE);
  drupal_set_breadcrumb($breadcrumb);
  return $output;
}

/**
 * Return lightest term for given node ($nid).
 * Similar to taxonomy_node_get_terms, but only return the lightest term in the
 * lightest vocab for the node.
 */
function _taxonomy_breadcrumb_node_get_lightest_term($nid) {
  // We only want the first row of the result--this is the lightest term of the
  // lightest vocab.  This query should be the same as the query found in
  // taxonomy_node_get_terms.
  $result = db_query(db_rewrite_sql('SELECT t.* FROM {term_node} r INNER JOIN {term_data} t ON r.tid = t.tid INNER JOIN {vocabulary} v ON t.vid = v.vid WHERE r.nid = %d ORDER BY v.weight, t.weight, t.name', 't', 'tid'), $nid);
  $term = db_fetch_object($result);  // extract first row of query
  return $term;
}

/**
 * Return the administrator defined vocabulary path for a given vocabulary
 * ($vid).  If a path doesn't exist, NULL is returned.
 */
function _taxonomy_breadcrumb_get_vocabulary_path($vid) {
  $result = db_query("SELECT path FROM {taxonomy_breadcrumb_vocabulary} WHERE vid = %d", $vid);
  $path = NULL;
  if ($row = db_fetch_array($result)) {
    $path = $row['path'];
  }
  return $path;
}

/**
 * Return the administrator defined term path for a given term ($tid).
 * If a path doesn't exist, NULL is returned.
 */
function _taxonomy_breadcrumb_get_term_path($tid) {
  $result = db_query("SELECT path FROM {taxonomy_breadcrumb_term} WHERE tid = %d", $tid);
  $path = NULL;
  if ($row = db_fetch_array($result)) {
    $path = $row['path'];
  }
  return $path;
}

/**
 * If the current drupal path (q=) is /node/nid, generate the breadcrumb trail
 * based on nid.
 */
function _taxonomy_breadcrumb_generate_breadcrumb($tid, $is_term_page = FALSE) {

  $term = taxonomy_get_term($tid);

  // HOME breadcrumb generation
  $home_text = variable_get('taxonomy_breadcrumb_home', '');
  if ($home_text != '') {
    $breadcrumb[] = l(t($home_text), NULL);
  }
  // VOCABULARY breadcrumb generation
  $vocabulary_path = _taxonomy_breadcrumb_get_vocabulary_path($term->vid);
  if ($vocabulary_path != NULL) {
    $vocabulary = taxonomy_vocabulary_load($term->vid);
    $breadcrumb[] = l($vocabulary->name, $vocabulary_path);
  }

  // TERM breadcrumb generation
  $parent_terms = array_reverse(taxonomy_get_parents_all($tid));
  foreach ($parent_terms as $parent_term) {
    $term_path = _taxonomy_breadcrumb_get_term_path($parent_term->tid);
    if ($term_path == NULL) {
      $term_path = taxonomy_term_path(taxonomy_get_term($parent_term->tid));
    }
    // Do not create links to own self if we are on a taxonomy/term page.
    if ($is_term_page && $parent_term->tid == $tid) {
      $breadcrumb[] = $parent_term->name;
    }
    else {
      $breadcrumb[] = l($parent_term->name, $term_path);
    }
  }

  // Remove current TERM from end of breadcrumb trail
  if (!variable_get('taxonomy_breadcrumb_show_current_term', TRUE) && !is_null($breadcrumb)) {
      array_pop($breadcrumb);
  }
  return $breadcrumb;

}
