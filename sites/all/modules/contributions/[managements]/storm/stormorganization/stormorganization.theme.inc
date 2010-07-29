<?php
// $Id: stormorganization.theme.inc,v 1.2.4.7 2009/01/06 15:16:22 robertogerola Exp $

/**
 * @file
 */

function theme_stormorganization_list($header, $organizations) {
  drupal_add_css(drupal_get_path('module', 'storm') .'/storm.css', 'module');

  $rows = array();
  $countries = stormattribute_attributes_bydomain('Country');
  foreach ($organizations as $key => $organization) {
    $n = new stdClass();
    $n->nid = $organization->nid;
    $n->uid = $organization->uid;
    $n->type = 'stormorganization';

    $rows[] = array(
      l($organization->title, 'node/'. $organization->nid),
      $countries[$organization->country],
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o  = theme('table', $header, $rows);
  return $o;
}

function theme_stormorganization_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);
  unset($node->content['body']);

  $w = 0;
  $node->content['stormorganization'] = array(
    '#prefix' => '<div id="stormorganization" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  $node->content['stormorganization']['fields'] ['fullname'] = array(
    '#prefix' => '<div class="fullname">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Fullname', 'organization'), $node->fullname),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['country'] = array(
    '#prefix' => '<div class="country">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Country', 'organization'), $node->country),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['provstate'] = array(
    '#prefix' => '<div class="provstate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Province / State', 'organization'), $node->provstate),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['city'] = array(
    '#prefix' => '<div class="city">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('City', 'organization'), $node->city),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['zip'] = array(
    '#prefix' => '<div class="zip">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Zip', 'organization'), $node->zip),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['address'] = array(
    '#prefix' => '<div class="address">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Address', 'organization'), $node->address),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['www'] = array(
    '#prefix' => '<div class="www">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('WWW', 'organization'), l($node->www, 'http://'. $node->www, array('absolute' => TRUE))),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['email'] = array(
    '#prefix' => '<div class="email">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Email', 'organization'), l($node->email, 'mailto:'. $node->email, array('absolute' => TRUE))),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['currency'] = array(
    '#prefix' => '<div class="currency">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Currency', 'organization'), $node->currency),
    '#weight' => $w++,
  );

  $languages = language_list('language', TRUE);
  $languages_options = array();
  foreach ($languages as $language_code => $language) {
    $languages_options[$language_code] = $language->name;
  }

  $node->content['stormorganization']['fields'] ['language'] = array(
    '#prefix' => '<div class="language">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Language', 'organization'), $languages_options[$node->orglanguage]),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['taxid'] = array(
    '#prefix' => '<div class="taxid">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Tax ID', 'organization'), $node->taxid),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['fields'] ['note'] = array(
    '#prefix' => '<div class="note">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Note', 'organization'), $node->body),
    '#weight' => $w++,
  );

  $node->content['stormorganization']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;

  if (module_exists('stormperson')) {
    $node->content['stormorganization']['links']['people'] = array(
      '#prefix' => '<div class="people">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('People', 'organization'), 'storm/people/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormproject')) {
    $node->content['stormorganization']['links']['projects'] = array(
      '#prefix' => '<div class="projects">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Projects', 'organization'), 'storm/projects/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormticket')) {
    $node->content['stormorganization']['links']['tickets'] = array(
      '#prefix' => '<div class="tickets">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Tickets', 'organization'), 'storm/tickets/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('storminvoice')) {
    $node->content['stormorganization']['links']['invoices'] = array(
      '#prefix' => '<div class="invoices">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Invoices', 'organization'), 'storm/invoices/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  if (module_exists('stormnote')) {
    $node->content['stormorganization']['links']['notes'] = array(
      '#prefix' => '<div class="notes">',
      '#suffix' => '</div>',
      '#value' => l(storm_t('Notes', 'organization'), 'storm/notes/', array('query' => array('organization_nid' => $node->nid))),
      '#weight' => $w++,
    );
  }

  return $node;
}
