<?php
// $Id: stormexpense.theme.inc,v 1.1.4.3 2009/01/06 15:16:22 robertogerola Exp $

/**
 * @file
 */

function theme_stormexpense_list($header, $expenses, $totals) {
  $rows = array();
  foreach ($expenses as $expense) {
    $n = new stdClass();
    $n->nid = $expense->nid;
    $n->uid = $expense->uid;
    $n->organization_nid = $expense->organization_nid;
    $n->project_nid = $expense->project_nid;
    $n->task_nid = $expense->task_nid;
    $n->ticket_nid = $expense->ticket_nid;
    $n->type = 'stormexpense';
    $rows[] = array(
      l($expense->organization_title, 'node/'. $expense->organization_nid),
      l($expense->project_title, 'node/'. $expense->project_nid),
      l($expense->title, 'node/'. $expense->nid),
      format_date($expense->expensedate, 'custom', 'Y-m-d'),
      array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }
  $o = theme('table', $header, $rows);

  $header = array(
    array(
      'data' => storm_t('Amount', 'expense'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Tax', 'expense'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Total', 'expense'),
      'style' => 'text-align: center;',
    ),
  );

  $rows = array();
  $rows[] = array(
    array(
      'data' => sprintf('%.2f', $totals->t_amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->t_tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->t_total),
      'style' => 'text-align: right;',
    ),
  );

  $o .= theme('table', $header, $rows);

  return $o;
}

function theme_stormexpense_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['stormexpense'] = array(
    '#prefix' => '<div id="stormexpense" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'expense'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'expense'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['task'] = array(
    '#prefix' => '<div class="task">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Task', 'expense'), l($node->task_title, 'node/'. $node->task_nid)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['ticket'] = array(
    '#prefix' => '<div class="ticket">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Ticket', 'expense'), l($node->ticket_title, 'node/'. $node->ticket_nid)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['provider'] = array(
    '#prefix' => '<div class="provider">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Provider', 'expense'), $node->provider_title),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['expensedate'] = array(
    '#prefix' => '<div class="expensedate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Date'), format_date($node->expensedate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['amount'] = array(
    '#prefix' => '<div class="amount">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Amount', 'expense'), sprintf('%.2f', $node->amount)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['tax'] = array(
    '#prefix' => '<div class="tax">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Tax', 'expense'), sprintf('%.2f', $node->tax)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['total'] = array(
    '#prefix' => '<div class="total">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Total', 'expense'), sprintf('%.2f', $node->total)),
    '#weight' => $w++,
  );

  $node->content['stormexpense']['fields']['body'] = array(
    '#prefix' => '<div class="body">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'expense'), $node->content['body']['#value']),
    '#weight' => $w++,
  );
  unset($node->content['body']);

  $node->content['stormexpense']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#value' => '&nbsp;',
    '#weight' => $w++,
  );

  return $node;
}

function theme_stormexpense_list_form_report_reports() {
  $reports = array(
      'std' => storm_t('Standard', 'expense'),
      'for_organization' => storm_t('For an organization', 'expense'),
      'for_organization_w_task' => storm_t('For an org. with tasks', 'expense'),
      'for_project' => storm_t('For a project', 'expense'),
  );

  return $reports;
}

function theme_stormexpense_list_report($report, $language, $expenses) {
  switch ($report) {
    case 'std':
      $header = array(
        storm_t('Organization', 'expense', array(), $language),
        storm_t('Project', 'expense', array(), $language),
        storm_t('Title', 'expense', array(), $language),
        storm_t('Date', 'expense', array(), $language),
        storm_t('Total', 'expense', array(), $language));
      $expenses_total = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          $expense->organization_title,
          $expense->project_title,
          $expense->title,
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. storm_t('Expenses report', 'expense', array(), $language) .'</h2><br />';
    break;
    case 'for_organization':
      $organization = node_load($_SESSION['stormexpense_list_filter']['organization_nid']);
      $header = array(
        storm_t('Project', 'expense', array(), $language),
        storm_t('Title', 'expense', array(), $language),
        storm_t('Date', 'expense', array(), $language),
        storm_t('Total', 'expense', array(), $language));
      $expenses_total = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          $expense->project_title,
          $expense->title,
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. storm_t('Expenses report', 'expense', array(), $language) .'</h2><br />';
      $title .= storm_t('Organization : @organization', 'expense', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_organization_w_task':
      $organization = node_load($_SESSION['stormexpense_list_filter']['organization_nid']);
      $header = array(
        storm_t('Project', 'expense', array(), $language),
        storm_t('Task', 'expense', array(), $language),
        storm_t('Title', 'expense', array(), $language),
        storm_t('Date', 'expense', array(), $language),
        storm_t('Total', 'expense', array(), $language));
      $total_duration = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          $expense->project_title,
          $expense->task_title,
          $expense->title,
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. storm_t('Expenses report', 'expense', array(), $language) .'</h2><br />';
      $title .= storm_t('Organization : @organization', 'expense', array('@organization' => $organization->fullname), $language) .'<br />';
    break;
    case 'for_project':
      $organization = node_load($_SESSION['stormexpense_list_filter']['organization_nid']);
      $project = node_load($_SESSION['stormexpense_list_filter']['project_nid']);

      $header = array(
        storm_t('Task', 'expense', array(), $language),
        storm_t('Title', 'expense', array(), $language),
        storm_t('Date', 'expense', array(), $language),
        storm_t('Total', 'expense', array(), $language));
      $total_duration = 0;
      foreach ($expenses as $expense) {
        $rows[] = array(
          $expense->task_title,
          $expense->title,
          format_date($expense->expensedate, 'custom', 'Y-m-d'),
          array('data' => sprintf('%.2f', $expense->total), 'align' => 'right'),
        );
        $expenses_total += $expense->total;
      }
      $title  = '<h2>'. storm_t('Expenses report', 'expense', array(), $language) .'</h2><br />';
      $title .= storm_t('Organization : @organization', 'expense', array('@organization' => $organization->fullname), $language) .'<br />';
      $title .= storm_t('Project : @project', 'expense', array('@project' => $project->title), $language) .'<br />';
    break;
  }
  $footer = '<h3>'. storm_t('Expenses total : %expenses_total', 'expense' , array('%expenses_total' => format_plural($expenses_total, '1', '@count', array(), $language)), $language) .'</h3>';
  return theme('storm_list_report', $header, $rows, $title, $footer);
}
