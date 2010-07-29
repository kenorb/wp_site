<?php
// $Id: storminvoice.theme.inc,v 1.2.4.11 2009/01/06 15:16:22 robertogerola Exp $

/**
 * @file storminvoice.theme.inc
 * Themable functions for Storm invoice component.
 */

/**
 * Returns the HTML code for the invoices list
 *
 * @param $header
 *   Array with the fields headers.
 * @param $invoices
 *   Array with the list of invoices.
 * @param $itempsperpage
 *   Number of items to show on every page.
 * @param $totals_topay
 *   Total amount yet to pay.
 * @param $totals_paid
 *   Total amount already paid.
 * @param $totals
 *   Grand total.
 * @return
 *   HTML of the invoices list.
 */
function theme_storminvoice_list($header, $invoices, $itemsperpage, $totals_topay, $totals_paid, $totals) {
  $rows = array();
  foreach ($invoices as $invoice) {
    $n = new stdClass();
    $n->nid = $invoice->nid;
    $n->uid = $invoice->uid;
    $n->organization_nid = $invoice->organization_nid;
    $n->project_nid = $invoice->project_nid;
    $n->type = 'storminvoice';

    $invoice->status = 'open';
    if ($invoice->paymentdate) {
      $invoice->status = 'paid';
    }
    else if ($invoice->duedate < time()) {
      $invoice->status = 'overdue';
    }

    $rows[] = array(
      array(
        'data' => storm_icon('invoice_status_'. $invoice->status, $invoice->status),
        'style' => 'text-align: center',
      ),
      array(
        'data' => l($invoice->number, 'node/'. $invoice->nid),
        'style' => 'text-align: right',
      ),
      l($invoice->organization_title, 'node/'. $invoice->organization_nid),
      l($invoice->project_title, 'node/'. $invoice->project_nid),
      l($invoice->title, 'node/'. $invoice->nid),
      format_date($invoice->requestdate, 'custom', 'Y-m-d'),
      array('data' => sprintf('%.2f', $invoice->total), 'align' => 'right'),
      array(
        'data' => storm_icon_edit_node($n, $_GET) .'&nbsp;'. storm_icon_delete_node($n, $_GET),
        'class' => 'storm_list_operations',
      ),
    );
  }

  $o = theme('table', $header, $rows);
  $o .= theme('pager', NULL, $itemsperpage, 0);

  $header = array(
    array(
      'data' => ' ',
    ),
    array(
      'data' => storm_t('Amount', 'invoice'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Tax', 'invoice'),
      'style' => 'text-align: center;',
    ),
    array(
      'data' => storm_t('Total', 'invoice'),
      'style' => 'text-align: center;',
    ),
  );

  $rows = array();
  $rows[] = array(
    array(
      'data' => storm_t('Total to pay', 'invoice'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_topay->total),
      'style' => 'text-align: right;',
    ),
  );
  $rows[] = array(
    array(
      'data' => storm_t('Total paid', 'invoice'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals_paid->total),
      'style' => 'text-align: right;',
    ),
  );
  $rows[] = array(
    array(
      'data' => storm_t('Total', 'invoice'),
      'style' => 'font-weight: bold;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $totals->total),
      'style' => 'text-align: right;',
    ),
  );
  $o .= theme('table', $header, $rows);

  return $o;
}

/**
 * Returns the HTML code for the invoice view page
 *
 * @param $node
 *   The node object that contains the invoice.
 * @param $teaser
 *   True if the teaser is showed.
 * @param $page
 *   True if the full page is showed.
 * @return
 *   The node object that contains the invoice.
 */
function theme_storminvoice_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  $w = 0;
  $node->content['storminvoice'] = array(
    '#prefix' => '<div id="storminvoice" class="stormcomponent">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields'] = array(
    '#prefix' => '<div id="fields">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  $node->content['storminvoice']['fields']['number'] = array(
    '#prefix' => '<div class="number">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Number', 'invoice'), $node->number),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['description'] = array(
    '#prefix' => '<div class="invoicedescription">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Description', 'invoice'), $node->title),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['organization'] = array(
    '#prefix' => '<div class="organization">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Organization', 'invoice'), l($node->organization_title, 'node/'. $node->organization_nid)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['project'] = array(
    '#prefix' => '<div class="project">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Project', 'invoice'), l($node->project_title, 'node/'. $node->project_nid)),
    '#weight' => $w++,
  );

  if($node->reference) {
    $node->content['storminvoice']['fields']['reference'] = array(
      '#prefix' => '<div class="reference">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', storm_t('Reference', 'invoice'), $node->reference),
      '#weight' => $w++,
    );
  }

  $node->content['storminvoice']['fields']['amount'] = array(
    '#prefix' => '<div class="amount">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Amount', 'invoice'), sprintf('%.2f', $node->amount)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['tax'] = array(
    '#prefix' => '<div class="tax">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Tax', 'invoice'), sprintf('%.2f', $node->tax)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['total'] = array(
    '#prefix' => '<div class="total">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Total', 'invoice'), sprintf('%.2f', $node->total)),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['requestdate'] = array(
    '#prefix' => '<div class="requestdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Request date', 'invoice'), format_date($node->requestdate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['duedate'] = array(
    '#prefix' => '<div class="duedate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Due date', 'invoice'), format_date($node->duedate, 'custom', 'Y-m-d')),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['fields']['paymentdate'] = array(
    '#prefix' => '<div class="paymentdate">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Payment date', 'invoice'), $node->paymentdate? format_date($node->paymentdate, 'custom', 'Y-m-d') : ''),
    '#weight' => $w++,
  );

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  else if ($node->duedate < time()) {
    $status = 'overdue';
  }
  $node->content['storminvoice']['fields']['status'] = array(
    '#prefix' => '<div class="invoicestatus">',
    '#suffix' => '</div>',
    '#value' => theme('storm_view_item', storm_t('Status', 'invoice'), $status),
    '#weight' => $w++,
  );

  $organization = node_load($node->organization_nid);
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  if ($myorg->orglanguage != $organization->orglanguage) {
    $language = $organization->orglanguage .','. $myorg->orglanguage;
  }
  else {
    $language = $myorg->orglanguage;
  }

  if($node->content['body']['#value']) {
    $node->content['storminvoice']['fields']['body'] = array(
      '#prefix' => '<div class="body">',
      '#suffix' => '</div>',
      '#value' => theme('storm_view_item', storm_t('Description', 'project'), $node->content['body']['#value']),
      '#weight' => $w++,
    );
    unset($node->content['body']);
  }

  $node->content['storminvoice']['links'] = array(
    '#prefix' => '<div id="links">',
    '#suffix' => '</div>',
    '#weight' => $w++,
  );

  $w = 0;
  $node->content['storminvoice']['links']['print']['html'] = array(
    '#prefix' => '<div class="print_html">',
    '#suffix' => '</div>',
    '#value' => l(t('Print HTML'), 'storm/invoice/report/'. $node->nid .'/html/'. $language),
    '#weight' => $w++,
  );

  $node->content['storminvoice']['links']['print']['pdf'] = array(
    '#prefix' => '<div class="print_pdf">',
    '#suffix' => '</div>',
    '#value' => l(t('Print PDF'), 'storm/invoice/report/'. $node->nid .'/pdf/'. $language),
    '#weight' => $w++,
  );

  return $node;
}

function theme_storminvoice_report($node, $report, $language) {
  if($report=='html') {
    print theme('storminvoice_report_html', $node, $language); 
  }
  else if ($report=='pdf') {
    theme('storminvoice_report_pdf', $node, $language); 
  }
}

function theme_storminvoice_report_pdf($node, $language) {
  $tcpdf_dir = drupal_get_path('module', 'storm') .'/libs/tcpdf';
  require_once($tcpdf_dir .'/config/lang/eng.php');
  require_once($tcpdf_dir .'/tcpdf.php');

  $languages = explode(',', $language);
  $language = $languages[0];
  $language1 = '';
  if (array_key_exists(1, $languages)) {
    $language1 = $languages[1];
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  else if ($node->duedate < time()) {
    $status = 'overdue';
  }

  $countries = stormattribute_attributes_bydomain('Country');
  $currencies = stormattribute_attributes_bydomain('Currency');
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $mycurrency = $currencies[$myorg->currency];

  $organization = node_load($node->organization_nid);
  $project = node_load($node->project_nid);

  $o = '';
  $title = storm_t('Invoice', 'invoice', array(), $language);
  $complete_title = $title .' '. $myorg->title .' : '. $organization->title .' - '. $node->number;

  $pdf = new TCPDF(PDF_PAGE_ORIENTATION, PDF_UNIT, PDF_PAGE_FORMAT, true); 

  $pdf->SetCreator(PDF_CREATOR);
  $pdf->SetAuthor("Storm");
  $pdf->SetTitle($complete_title);
  $pdf->SetSubject($title);
  $pdf->SetKeywords($title, $myorg->title, $organization->title, $node->number);

  $pdf->SetMargins(PDF_MARGIN_LEFT, PDF_MARGIN_TOP, PDF_MARGIN_RIGHT);
  $pdf->setPrintHeader(false);
  $pdf->setPrintFooter(false);

  $margins = $pdf->getMargins();
  $pageWidth = $pdf->getPageWidth() - $margins['left'] - $margins['right'];
  $pdf->SetAutoPageBreak(TRUE, PDF_MARGIN_BOTTOM);

  $pdf->AddPage();

  $pdf->setDrawColor(204, 204, 204);
  $pdf->setFillColor(220, 220, 220);

  $pdf->SetFont("times", "B", 14);
  $headerleft = variable_get('site_name', '') .'<br />'. variable_get('site_slogan', '');
  $pdf->writeHTMLCell($pageWidth *.5, 0, $pdf->getX(), $pdf->getY(), $headerleft, 0 ,0 ,0, false, 'L');

  $pdf->SetFont("times", "N", 10);
  $pdf->writeHTMLCell($pageWidth *.5, 0, $pdf->getX(), $pdf->getY(), variable_get('storm_report_header', ''), 0 ,1 ,0, false, 'R');

  $pdf->SetFont("times", "B", 14);
  $o = $title;
  if ($language1) $o .= "\n". storm_t('Invoice', 'invoice', array(), $language1);
  $pdf->MultiCell(0, 0, $o, 0 ,'C', 0, 1,$pdf->getX(), $pdf->getY() + 10);

  $y = $pdf->getY() + 10;

  $pdf->SetFont("times", "B", 10);
  $o = storm_t('Bill to', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Bill to', 'invoice', array(), $language1);
  $pdf->MultiCell($pageWidth *.4, 0, $o, 'B', 'L', 0, 1, PDF_MARGIN_LEFT, $y);
  $o =  $organization->fullname ."\n";
  $o .= $organization->address ."\n";
  $o .= $organization->city ."\n";
  $o .= $organization->provstate .' '. $organization->zip .' '. $countries[$organization->country] ."\n";;
  if ($organization->taxid) {
    $o .= storm_t('Tax ID', 'invoice', array(), $language);
    if ($language1) $o .= ' / '. storm_t('Tax ID', 'invoice', array(), $language1);
    $o .= ' : '. $organization->taxid;
  }
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($pageWidth *.4, 0, $o,0 , 'L' ,0, 1, PDF_MARGIN_LEFT);
  $destY = $pdf->getY();

  $w = ($pageWidth *.5) / 4;

  $pdf->SetFont("times", "B", 10);

  $o = storm_t('Invoice#', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Invoice#', 'invoice', array(), $language1);
  $pdf->MultiCell($w-2, 0, $o, 1, 'L', 1, 0, PDF_MARGIN_LEFT + $pageWidth *.5, $y);

  $o = storm_t('Currency', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Currency', 'invoice', array(), $language1);
  $pdf->MultiCell($w-3, 0, $o, 1, 'L', 1, 0);

  $o = storm_t('Date', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Date', 'invoice', array(), $language1);
  $pdf->MultiCell($w+5, 0, $o, 1, 'L', 1, 0);

  $o = storm_t('Reference', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Reference', 'invoice', array(), $language1);
  $pdf->MultiCell($w, 0, $o, 1, 'L', 1, 1);

  $pdf->SetFont("times", "N", 10);
  $h = $pdf->getY();
  $pdf->MultiCell($w, 0, $node->reference?$node->reference:'-' , 1, 'L', 0, 1, PDF_MARGIN_LEFT + $pageWidth*.5 + $w*3);
  $h = $pdf->getY() - $h;
  $pdf->MultiCell($w-2, $h, $node->number, 1, 'L', 0, 0, PDF_MARGIN_LEFT + $pageWidth *.5, $pdf->getY() - $h);
  $pdf->MultiCell($w-3, $h, storm_t($mycurrency, 'invoice', array(), $language), 1, 'L', 0, 0);
  $pdf->MultiCell($w+5, $h, format_date($node->requestdate, 'custom', 'Y-m-d'), 1, 'L', 0, 1);

  $pdf->SetFont("times", "B", 10);

  $o = storm_t('Due total', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Due total', 'invoice', array(), $language1);
  $pdf->MultiCell($w*2 - 5, 0, $o, 1, 'L', 1, 0, PDF_MARGIN_LEFT + $pageWidth *.5);

  $o = storm_t('Due date', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Due date', 'invoice', array(), $language1);
  $pdf->MultiCell($w + 5, 0, $o, 1, 'L', 1, 0);

  $o = storm_t('Terms', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Terms', 'invoice', array(), $language1);
  $pdf->MultiCell($w, 0, $o, 1, 'L', 1, 1);

  $pdf->SetFont("times", "B", 10);
  $o = storm_t($mycurrency, 'invoice', array(), $language) .' '. sprintf('%.2f', $node->total);
  if ($organization->currency != $myorg->currency) {
    $o .= "\n". $organization->currency .' '. sprintf('%.2f', $node->totalcustomercurr);
  }
  $pdf->MultiCell($w*2 - 5, 12, $o, 1, 'C', 0, 0, PDF_MARGIN_LEFT + $pageWidth *.5);
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($w + 5, 12, format_date($node->duedate, 'custom', 'Y-m-d'), 1, 'L', 0, 0);
  $pdf->MultiCell($w, 12, t(variable_get('storminvoice_payment_terms', ''), array(), $language), 1, 'L', 0, 1);

  $y = $pdf->getY();
  if($destY > $y) $y = $destY;
  $pdf->setY($y+10);

  $pdf->setX(PDF_MARGIN_LEFT);
  $pdf->SetFont("times", "B", 10);

  $o = storm_t('Description', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Description', 'invoice', array(), $language1);
  $pdf->MultiCell($pageWidth * .4, 0, $o, 1, 'L', 1, 0);

  $o = storm_t('Amount', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Amount', 'invoice', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  $o = storm_t('Tax %', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Tax %', 'invoice', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  $o = storm_t('Tax', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Tax', 'invoice', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 0);

  $o = storm_t('Total', 'invoice', array(), $language);
  if ($language1) $o .= "\n". storm_t('Total', 'invoice', array(), $language1);
  $pdf->MultiCell($pageWidth * .15, 0, $o, 1, 'C', 1, 1);

  $pdf->SetFont("times", "N", 10);
  $items = storminvoiceitem_getitems($node->nid);
  $rows = array();
  $pdf->setFillColor(245, 245, 245);
  $c = 0;
  foreach ($items as $i) {
    if ($c==2) $c=0;
    $y = $pdf->getY();
    $h = $pdf->getY();
    $pdf->MultiCell($pageWidth * .4, 0, $i->title, 1, 'L', $c, 1);
    $h = $pdf->getY() - $h;
    $pdf->setY($y);
    $pdf->setX(PDF_MARGIN_LEFT + $pageWidth * .4);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->amount), 1, 0, 'R', $c);
    $pdf->Cell($pageWidth * .15, $h, $i->taxpercent, 1, 0, 'R', $c);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->tax), 1, 0, 'R', $c);
    $pdf->Cell($pageWidth * .15, $h, sprintf('%.2f', $i->total), 1, 1, 'R', $c);
    $c++;
  }

  if ($node->taxexempt && $language1=='it') {
    $o = storm_t('Tax exempt art. 7', 'invoice', array(), $language);
    if ($language1) $o .= "\n". storm_t('Tax exempt art. 7', 'invoice', array(), $language1);
    $pdf->MultiCell($pageWidth, 0, $o,0 , 'L' ,0, 1, $pdf->getX(), $pdf->getY() + 5);
  }

  $y = $pdf->getY() + 10;
  $pdf->setY($y);
  $pdf->SetFont("times", "B", 10);
  $pdf->Cell($pageWidth, 5, storm_t('Payment', 'invoice', array(), $language), 'B', 0, 'L');
  $pdf->SetFont("times", "N", 10);
  $pdf->MultiCell($pageWidth, 0, t(variable_get('storminvoice_payment_modes', ''), array(), $language),0 , 'L' ,0, 1, PDF_MARGIN_LEFT, $pdf->getY(), true, 0, true);

  if ($status=='paid') {
    $y = $pdf->getY() + 10;
    $pdf->setY($y);
    $pdf->SetFont("times", "B", 14);
    $pdf->Cell(0, 12, storm_t('Paid in full', 'invoice', array(), $language), 0 ,1,'C');  }

  $filename = strtolower($title . '_' . $myorg->title . '_' . $organization->title . '_' . str_replace('/', '-', $node->number)) .'.pdf';

  //Close and output PDF document
  $pdf->Output($filename, "I");

}

function theme_storminvoice_report_html($node, $language) {
  $languages = explode(',', $language);
  $language = $languages[0];
  $language1 = '';
  if (array_key_exists(1, $languages)) {
    $language1 = $languages[1];
  }

  $status = 'open';
  if ($node->paymentdate) {
    $status = 'paid';
  }
  else if ($node->duedate < time()) {
    $status = 'overdue';
  }

  $countries = stormattribute_attributes_bydomain('Country');
  $currencies = stormattribute_attributes_bydomain('Currency');
  $myorg = node_load(variable_get('storm_organization_nid', 0));
  $mycurrency = $currencies[$myorg->currency];

  $organization = node_load($node->organization_nid);
  $project = node_load($node->project_nid);

  $o = '';
  $title = storm_t('Invoice', 'invoice', array(), $language);
  if ($language1) $title .= '<br />'. storm_t('Invoice', 'invoice', array(), $language1);
  $headtitle = $title . ' '. $myorg->title . ' : ' . $organization->title . ' - ' . $node->number;

  $o .= '<div>';
  $o .= '<div id="storminvoice_billto"><table>';
  $o .= '<tr><td class="storminvoice_billto_title">';
  $o .= storm_t('Bill To :', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Bill to :', 'invoice', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= '<strong>'. $organization->fullname .'</strong>';
  $o .= '<br />';
  $o .= $organization->address;
  $o .= '<br />';
  $o .= $organization->city;
  $o .= '<br />';
  $o .= $organization->provstate .' '. $organization->zip .' '. $countries[$organization->country];
  $o .= '<br />';
  if ($organization->taxid) {
    $o .= storm_t('Tax ID', 'invoice', array(), $language);
    if ($language1) $o .= ' / '. storm_t('Tax ID', 'invoice', array(), $language1);
    $o .= ' : '. $organization->taxid;
  }
  $o .= '</td></tr>';
  $o .= '</table></div>';

  $o .= '<div id="storminvoice_details"><table>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Invoice#', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Invoice#', 'invoice', array(), $language1);
  $o .='</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Currency', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Currency', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Date', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Date', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Reference', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Reference', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td>'. $node->number .'</td>';
  $o .= '<td>'. storm_t($mycurrency, 'invoice', array(), $language) .'</td>';
  $o .= '<td>'. format_date($node->requestdate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. $node->reference .'</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td class="storminvoice_details_title" colspan="2">';
  $o .= storm_t('Due total', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Due total', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Due date', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Due date', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '<td class="storminvoice_details_title">';
  $o .= storm_t('Terms', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Terms', 'invoice', array(), $language1);
  $o .= '</td>';
  $o .= '</tr>';
  $o .= '<tr>';
  $o .= '<td style="text-align: center; font-weight: bold;" colspan="2">';
  $o .= storm_t($mycurrency, 'invoice', array(), $language) .' '. sprintf('%.2f', $node->total);
  if ($organization->currency != $myorg->currency) {
    $o .= '<br />';
    $o .= '('. $organization->currency .' '. sprintf('%.2f', $node->totalcustomercurr) .')';
  }
  $o .= '</td>';
  $o .= '<td>'. format_date($node->duedate, 'custom', 'Y-m-d') .'</td>';
  $o .= '<td>'. t(variable_get('storminvoice_payment_terms', ''), array(), $language) .'</td>';
  $o .= '</tr>';
  $o .= '</table></div>';
  $o .= '</div>';

  $description = storm_t('Description', 'invoice', array(), $language);
  if ($language1) $description .= '<br />'. storm_t('Description', 'invoice', array(), $language1);

  $amount = storm_t('Amount', 'invoice', array(), $language);
  if ($language1) $amount .= '<br />'. storm_t('Amount', 'invoice', array(), $language1);

  $taxpercent = storm_t('Tax %', 'invoice', array(), $language);
  if ($language1) $taxpercent .= '<br />'. storm_t('Tax %', 'invoice', array(), $language1);

  $tax = storm_t('Tax', 'invoice', array(), $language);
  if ($language1) $tax .= '<br />'. storm_t('Tax', 'invoice', array(), $language1);

  $total = storm_t('Total', 'invoice', array(), $language);
  if ($language1) $total .= '<br />'. storm_t('Total', 'invoice', array(), $language1);

  $header = array(
    array(
      'data' => $description,
    ),
    array(
      'data' => $amount,
    ),
    array(
      'data' => $taxpercent,
    ),
    array(
      'data' => $tax,
    ),
    array(
      'data' => $total,
    ),
  );
  $items = storminvoiceitem_getitems($node->nid);
  $rows = array();
  foreach ($items as $i) {
    $rows[] = array(
      array(
        'data' => $i->title,
      ),
      array(
        'data' => sprintf('%.2f', $i->amount),
        'style' => 'text-align: right;',
      ),
      array(
        'data' => $i->taxpercent,
        'style' => 'text-align: right;',
      ),
      array(
        'data' => sprintf('%.2f', $i->tax),
        'style' => 'text-align: right;',
      ),
      array(
        'data' => sprintf('%.2f', $i->total),
        'style' => 'text-align: right;',
      ),
    );
  }

  $total = storm_t('Total', 'invoice', array(), $language);
  if ($language1) $total .= ' / '. storm_t('Total', 'invoice', array(), $language1);

  $rows[] = array(
    array(
      'data' => $total,
      'style' => 'font-weight: bold; text-align: center; background-color: #e9e9e9;',
    ),
    array(
      'data' => sprintf('%.2f', $node->amount),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => '-',
      'style' => 'text-align: center;',
    ),
    array(
      'data' => sprintf('%.2f', $node->tax),
      'style' => 'text-align: right;',
    ),
    array(
      'data' => sprintf('%.2f', $node->total),
      'style' => 'text-align: right;',
    ),
  );

  $o .= '<div id="storminvoice_items">'. theme('table', $header, $rows) .'</div>';

  if ($node->taxexempt && $language1=='it') {
    $o .= '<div id="storminvoice_tax_exempt">';
    $o .= storm_t('Tax exempt art. 7', 'invoice', array(), $language);
    if ($language1) $o .= '<br />'. storm_t('Tax exempt art. 7', 'invoice', array(), $language1);
    $o .= '</div>';
  }

  $o .= '<div id="storminvoice_payment_modes"><table>';
  $o .= '<tr><td class="storminvoice_payment_title">';
  $o .= storm_t('Payment', 'invoice', array(), $language);
  if ($language1) $o .= '<br />'. storm_t('Payment', 'invoice', array(), $language1);
  $o .= '</td></tr>';
  $o .= '<tr><td>';
  $o .= t(variable_get('storminvoice_payment_modes', ''), array(), $language);
  $o .= '</td></tr>';
  $o .= '</table></div>';

  if ($status=='paid') {
    $o .= '<div id="paid">';
    $o .= '<h2>'. storm_t('Paid in full', 'invoice', array(), $language) .'</h2>';
    if ($language1) $o .= '<h2>'. storm_t('Paid in full', 'invoice', array(), $language1) .'</h2>';
    $o .= '</div>';
  }

  return theme('storm_report', $header, $o, $title, $footer, $headtitle);
}
