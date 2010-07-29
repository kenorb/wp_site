<?php
// $Id: user_relationships_api.author-pane.inc,v 1.1.2.4 2009/01/05 04:38:06 michellec Exp $

/**
 * @file
 *   Provides a preprocess function on behalf of the user relationships module.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_relationships_api_preprocess_author_pane(&$variables) {
  global $user;
  $account = $variables['account'];
  $image_path = $variables['image_path'];

  // Anonymous users and users viewing their own account won't get a link.
  if (!$user->uid || !$account->uid || $user->uid == $account->uid) {
    return;
  }

  // Get a list of relation ship types
  $rtypes = user_relationships_types_load();

  // If there is only one type, we can show a simple add/remove icon
  if (count($rtypes) == 1) {
    $rtype = array_shift($rtypes);
    $relationships = user_relationships_load(array('between' => array($user->uid, $account->uid), 'rtid' => $rtype->rtid));

    if (count($relationships)) {
      // Existing relationship; need remove icon/link. (Despite the foreach,
      // there should only be one.)
      foreach ($relationships as $rtid => $relationship) {
        $link_label = t('Remove from @rel_plural_name', array('@rel_plural_name' => $relationship->plural_name));
        $css_class = str_replace(' ', '-', $relationship->name);

        // Linked icon
        $variables['user_relationships_api'] =
          l(theme('image', "$image_path/buddy-remove.png", $link_label, $link_label, NULL, FALSE),
            "user/{$user->uid}/relationships/{$relationship->rid}/remove",
            array(
                'query' => drupal_get_destination(),
                'html' => TRUE,
                'attributes'  => array('title' => $link_label, 'class' => $css_class),
            ), NULL, NULL, FALSE, TRUE);

        // Just link
        $variables['user_relationships_api_link'] =
          l(t('Remove @name from @rel_plural_name', array('@name' => $account->name, '@rel_plural_name' => $relationship->plural_name)),
          "user/{$user->uid}/relationships/{$relationship->rid}/remove",
          array(
                'query' => drupal_get_destination(),
                'html' => TRUE,
                'attributes'  => array('title' => $link_label, 'class' => $css_class),
          ));
      }
    }
    else {
      // No existing relationship; need an add icon/link.
      $relationships_count = user_relationships_load(array('between' => array($user->uid, $account->uid)), array('count' => TRUE));
      $link_label = t('Add @rel_name', array('@rel_name' => $rtype->name));
      $css_class = str_replace(' ', '-', $rtype->name);

      // Linked icon
      $variables['user_relationships_api'] =
        l(theme('image', "$image_path/buddy-add.png", $link_label, $link_label, NULL, FALSE),
          "relationship/{$account->uid}/request/{$rtype->rtid}",
          array(
              'query' => drupal_get_destination(),
              'html' => TRUE,
              'attributes'  => array('title' => $link_label, 'class' => $css_class),
          ), NULL, NULL, FALSE, TRUE);

      // Just link
      $variables['user_relationships_api_link'] =
        l(t('Add @name as a @rel_name', array('@name' => $account->name, '@rel_name' => $rtype->name)),
          "relationship/{$account->uid}/request/{$rtype->rtid}",
          array(
            'query' => drupal_get_destination(),
            'html' => TRUE,
            'attributes'  => array(
              'title' => t('Become @name\'s @rel_name', array('@name' => $account->name, '@rel_name' => $rtype->name)),
              'class' => $css_class
            ),
          )
        );
    }
  }
  else {
    //relationship request link(s)
    $relationships_count = user_relationships_load(array('between' => array($user->uid, $account->uid)), array('count' => TRUE));
    if (!$relationships_count || (variable_get('user_relationships_ui_allow_multiple', TRUE) && ($relationships_count < count($rtypes)))) {
      //if configured, create direct links
      if (variable_get('user_relationships_show_direct_links', 1)) {
        //draw a single link, or one for each relationship type
        $existing_relationships = user_relationships_load(array('between' => array($user->uid, $account->uid)), array('sort' => 'rtid'));
        $all_relationships = user_relationships_types_load();
        foreach ($all_relationships as $rtid => $relationship) {
          if ($existing_relationships[$rtid]) {
            continue;
          }
          $css_class = str_replace(' ', '-', $relationship->name);
          $list[] = l(
            t('+%rel_name', array('%name' => $account->name, '%rel_name' => $relationship->name, '%rel_plural_name' => $relationship->plural_name)),
            "relationship/{$account->uid}/request/{$relationship->rtid}",
            array(
              'query' => drupal_get_destination(),
              'html' => TRUE,
              'attributes'  => array(
                'title' => t('Become %name\'s %rel_name', array('%name' => $account->name, '%rel_name' => $relationship->name, '%rel_plural_name' => $relationship->plural_name)),
                'class' => "ur-request-link-$css_class"
              ),
            )
          );
        }
      }
      //just one generic link pointing to a page with dropdown
      else {
        $list[] = theme('user_relationships_request_relationship_link', $account);
      }
    }
    $variables['user_relationships_api_link'] = theme('item_list', $list, NULL, 'ul', array('class' => 'ur-request-short-links'));
    $variables['user_relationships_api'] = $variables['user_relationships_api_link'];
  }
}