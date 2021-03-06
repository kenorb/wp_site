<?php
// $Id: draggableviews_theme.inc,v 1.6.2.4 2009/01/09 15:02:30 sevi Exp $

/**
 * @file
 * Theme functions.
 */

function template_preprocess_draggableviews_view_draggabletable(&$vars) {
  $view     = $vars['view'];

  // We need the raw data for this grouping, which is passed in as $vars['rows'].
  // However, the template also needs to use for the rendered fields.  We
  // therefore swap the raw data out to a new variable and reset $vars['rows']
  // so that it can get rebuilt.
  $result   = $vars['rows'];
  $vars['rows'] = array();

  $options  = $view->style_plugin->options;
  $handler  = $view->style_plugin;

  $fields   = &$view->field;
  $columns  = $handler->sanitize_columns($options['columns'], $fields);

  $active   = !empty($handler->active) ? $handler->active : '';
  $order    = !empty($handler->order) ? $handler->order : 'asc';

  $query    = tablesort_get_querystring();
  if ($query) {
    $query = '&'. $query;
  }

  foreach ($columns as $field => $column) {
    // render the header labels
    if ($field == $column && empty($fields[$field]->options['exclude'])) {
      $label = check_plain(!empty($fields[$field]) ? $fields[$field]->label() : '');
      if (empty($options['info'][$field]['sortable'])) {
        $vars['header'][$field] = $label;
      }
      else {
        // @todo -- make this a setting
        $initial = 'asc';

        if ($active == $field && $order == 'asc') {
          $initial = 'desc';
        }

        $image = theme('tablesort_indicator', $initial);
        $title = t('sort by @s', array('@s' => $label));
        $link_options = array(
          'html' => TRUE,
          'attributes' => array('title' => $title),
          'query' => 'order='. urlencode($field) .'&sort='. $initial . $query,
        );
        $vars['header'][$field] = l($label . $image, $_GET['q'], $link_options);
      }
    }

    // Create a second variable so we can easily find what fields we have and what the
    // CSS classes should be.
    $vars['fields'][$field] = views_css_safe($field);
    if ($active == $field) {
      $vars['fields'][$field] .= ' active';
    }

    // Render each field into its appropriate column.
    foreach ($result as $num => $row) {
      if (!empty($fields[$field]) && empty($fields[$field]->options['exclude'])) {
        $field_output = $fields[$field]->theme($row);

        // Don't bother with separators and stuff if the field does not show up.
        if (!isset($field_output) && isset($vars['rows'][$num][$column])) {
          continue;
        }

        // Place the field into the column, along with an optional separator.
        if (isset($vars['rows'][$num][$column])) {
          if (!empty($options['info'][$column]['separator'])) {
            $vars['rows'][$num][$column] .= filter_xss_admin($options['info'][$column]['separator']);
          }
        }
        else {
          $vars['rows'][$num][$column] = '';
        }

        $vars['rows'][$num][$column] .= $field_output;
      }
    }
  }

  $vars['class'] = 'views-table';
  if (!empty($options['sticky'])) {
    drupal_add_js('misc/tableheader.js');
    $vars['class'] .= " sticky-enabled";
  }


  // Draggableviews output begin

  $info = _draggableviews_info($view);

  if (!isset($info['order'])) return;

  // analyze structure (also calculates depths)
  if (!_draggableviews_analyze_structure($info)) {
    return;
  }

  // loop through all rows the view returned
  foreach ($vars['rows'] as $i => $row) {

    $nid = $result[$i]->nid;

    // build indentation (as tabledrag requires)
    $indentation = theme('indentation', $info['nodes'][$nid]['depth']);

    if (user_access('Allow Reordering')) {

      // get node object
      $node = node_load(array('nid' => $nid));

      if (isset($info['types'][$node->type])) {
        // set node type specification, if available (e.g. tabledrag-root)
        $vars['tabledrag_type'][$i] = 'tabledrag-'. $info['types'][$node->type];
      }

      // Tabledrag needs all concerned fields to be input elements.
      // The values of the input elements will be changed by tabledrag while
      // dragging the rows. As we want to use these values we need to
      // give them names.
      //
      // Concerned fields are
      //  *) the first order field
      //  *) the parent field (if hierarchy used)
      //
      // The remaining fields are not used by tabledrag. But - depending
      // on the depth - one of them will simulate the first order field.
      // (This behavior is based on the fact that tabledrag handles each depth as it was the only one.)

      // this for loop should prevent copy->paste'ing code
      for ($modus = 'hierarchy'; $modus !== FALSE ; $modus = ($modus == 'hierarchy' ? 'order' : FALSE)) {

        if (isset($info[$modus])) {
          if ($modus == 'hierarchy') {
            $value = $info['nodes'][$nid]['parent'];

            // get the field that should be rendered
            $field = &$info['hierarchy']['field'];
          }
          else {
            // get depth of current field
            $depth = $info['nodes'][$node->nid]['depth'];

            $value = $info['nodes'][$nid]['order'][$depth];

            // get the field that should be rendered
            $field = &$info['order']['fields'][0];
          }

          // get the form element
          $form_element = $field['handler']->get_form_element($value, array('field_name' => $field['field_name'] .'_'. $nid, 'class' => $field['field_name']));

          // render new form element
          $vars['rows'][$i][$field['field_name']] = drupal_render($form_element);
        }
      }

      if (isset($info['hierarchy'])) {
        // append the node id as a hidden field. This is needed
        // because tabledrag would not know what id to assign when
        // subordinating.
        $hidden_nid = array(
          '#type' => 'hidden',
          '#name' => 'hidden_nid',
          '#value' => $nid,
          '#attributes' => array('class' => 'hidden_nid'),
        );
        // append rendered hidden node id to last rendered field
        $vars['rows'][$i][$field['field_name']] .= drupal_render($hidden_nid);
      }
    }

    if (isset($info['hierarchy'])) {
      // put indentation in front of first field
      $first_field = each($vars['rows'][$i]);
      $vars['rows'][$i][$first_field['key']] = $indentation . $vars['rows'][$i][$first_field['key']];
    }

    // Let extension modules alter the output
    foreach (module_implements('draggableviews_row_alter') as $module) {
      $function = $module .'_draggableviews_row_alter';
      $function($vars['rows'][$i], $nid, $info, $view);
    }
  }

  // output data
  $vars['tabledrag_table_id'] = 'draggableview_'. $view->name;

  if (user_access('Allow Reordering')) {
    // prepare tabledrag settings for output
    $vars['tabledrag'] = array();
    if (isset($info['order'])) {
      $vars['tabledrag'][] = array(
        'action'       => 'order',
        'relationship' => 'sibling',
        'source'       => $info['order']['fields'][0]['field_name'],
        'group'        => $info['order']['fields'][0]['field_name'],
        'subgroup'     => $info['order']['fields'][0]['field_name'],
        'hidden'       => !$info['order']['visible'],
        'limit'        => 0,
      );
    }
    if (isset($info['hierarchy'])) {
      $vars['tabledrag'][] = array(
        'action'       => 'match',
        'relationship' => 'parent',
        'source'       => 'hidden_nid',
        'group'        => $info['hierarchy']['field']['field_name'],
        'subgroup'     => $info['hierarchy']['field']['field_name'],
        'hidden'       => !$info['hierarchy']['visible'],
        'limit'        => count($info['order']['fields']) - 1,
      );
      // let javascript know about fields
      drupal_add_js(array('draggableviews' => array('parent' => $info['hierarchy']['field']['field_name'])), 'setting');
    }
  }
}

function template_preprocess_draggableviews_view_draggabletable_form($vars) {
  global $user;

  // get view object
  $view_obj = $vars['form']['#parameters'][2];
  // get structured info array
  $info     = _draggableviews_info($view_obj->view, FALSE, TRUE, FALSE);

  if (user_access('Allow Reordering') && isset($info['hierarchy'])) {
    // fetch expand information from database
    $result = db_query("SELECT parent_nid, collapsed FROM {draggableviews_collapsed} WHERE uid = %d", $user->uid);
    $states = array();
    while ($state = db_fetch_object($result)) {
      $states[$state->parent_nid] = $state->collapsed;
    }

    // check if "expand" links should be shown
    if ($info['expand_links']['show']) {
      drupal_add_js(drupal_get_path('module', 'draggableviews') .'/draggableviews.js');

      if (empty($states)) {
        // let js know whether child nodes should be expanded or not
        drupal_add_js(array(
                        'draggableviews' => array(
                          'expand_default' =>  $options['tabledrag_expand']['collapsed'] ? 1 : 0,
                        ),
                      ),
                      'setting');
      }
      else {
        drupal_add_js(array(
                        'draggableviews' => array(
                          'states' => $states,
                        ),
                      ),
                      'setting');
      }

      drupal_add_css(drupal_get_path('module', 'draggableviews') .'/styles.css');
    }
  }

  //theme view
  $sets = $view_obj->render_grouping($view_obj->view->result, $options['grouping']);

  $output = '';
  foreach ($sets as $title => $records) {
    $output .= theme($view_obj->theme_functions(), $view_obj->view, $options, $records, $title);
  }

  $vars['view'] = $output;

  // render submit form
  $vars['submit_form'] = user_access('Allow Reordering') ? drupal_render($vars['form']) : '';
}
