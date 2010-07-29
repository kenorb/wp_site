<?php
// $Id: storminvoiceitem.admin.inc,v 1.2.4.5 2009/01/02 08:49:14 robertogerola Exp $

/**
 * @file
 */

function storminvoiceitem_invoiceitems($invoice) {
  $node_invoice = node_load($invoice->nid);
  $breadcrumb = array();
  $breadcrumb[] = l(t('Storm'), 'storm');
  $breadcrumb[] = l(t('Invoices'), 'storm/invoices');
  $breadcrumb[] = l(t('Invoice : '. $node_invoice->title), 'node/'. $node_invoice->nid);
  drupal_set_breadcrumb($breadcrumb);

  $i = new stdClass();
  $i->type = 'storminvoiceitem';

  $params = $_GET;
  $params['organization_nid'] = $invoice->organization_nid;
  $params['project_nid'] = $invoice->project_nid;
  $params['invoice_nid'] = $invoice->nid;

  $header = array(
    array(
      'data' => storm_t('Title', 'invoiceitem'),
    ),
    array(
      'data' => storm_t('Amount', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('VAT', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Total', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Weight', 'invoiceitem'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_icon_add_node($i, $params),
      'class' => 'storm_list_operations',
    ),
  );

  $where = array();
  $s  = "SELECT n.*, sit.* FROM {node} AS n INNER JOIN {storminvoiceitem} sit ON n.vid=sit.vid WHERE n.status=1 AND n.type='storminvoiceitem' ORDER BY sit.weight";
  $where[] = 'sit.invoice_nid='. $invoice->nid;

  $s = storminvoiceitem_access_sql($s, $where);
  $s = db_rewrite_sql($s);

  $tablesort = tablesort_sql($header);
  $r = db_query($s . $tablesort);

  $items = array();
  while ($item = db_fetch_object($r)) {
    $items[] = $item;
  }

  $o = drupal_get_form('storminvoiceitem_invoiceitems_form', $header, $items, $invoice);
  return $o;
}

function storminvoiceitem_invoiceitems_form($form_id, $header, $invoiceitems, $invoice) {

  $form = array();

  $form['invoiceitems']['#theme'] = 'storminvoiceitem_invoiceitems';

  $form['invoiceitems']['header'] = array (
    '#value' => $header,
  );

  foreach ($invoiceitems as $invoiceitem) {
    $n = new stdClass();
    $n->nid = $invoiceitem->nid;
    $n->uid = $invoiceitem->uid;
    $n->type = 'storminvoiceitem';

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_title_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->title,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_amount_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->amount,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_tax_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->tax,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_total_'. $invoiceitem->nid] = array (
      '#value' => $invoiceitem->total,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_weight_'. $invoiceitem->nid] = array (
      '#type' => 'weight',
      '#default_value' => $invoiceitem->weight,
    );

    $form['invoiceitems']['invoiceitems'][$invoiceitem->nid]['invoiceitem_operations_'. $invoiceitem->nid] = array (
      '#value' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#submit' => array('storminvoiceitem_invoiceitems_submit'),
    '#value' => storm_t('Save', 'invoiceitem'),
  );

  return $form;
}

function storminvoiceitem_invoiceitems_submit($form, &$form_state) {
  $invoiceitems = array();
  foreach($form_state['values'] as $key=>$value) {
    $ar = explode('_', $key);
    if($ar[0]=='invoiceitem') {
      if($ar[1]=='weight') $invoiceitems[$ar[2]]['weight'] = $value;
    }
  }

  $s = "UPDATE {storminvoiceitem} SET weight=%d WHERE nid=%d";  
  foreach($invoiceitems as $nid=>$values) {
    db_query($s, $values['weight'], $nid);
  }

  drupal_set_message(storm_t('Invoice items saved', 'invoice'));
}

