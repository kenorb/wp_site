<?php
// $Id: ab.admin.inc,v 1.4 2008/09/22 15:25:49 tjholowaychuk Exp $

/**
 * @file
 * ApacheBench administration and reporting pages.
 */

/* -----------------------------------------------------------------

  Administration

------------------------------------------------------------------ */

/**
 * Settings form.
 * 
 * @return array
 *
 * @ingroup forms
 */
function ab_settings() {
  $form = array();

	$form['ab_frequency'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Frequency'),
	    '#description' => t('Run ApacheBench tests every x hours. Cron must be scheduled to run at this same frequency or higher. Set to 0 to prevent automated testing.'),
	    '#default_value' => variable_get('ab_frequency', 0),
	  );
	$form['ab_requests'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Requests'),
	    '#description' => t('Total number of requests.'),
	    '#default_value' => variable_get('ab_requests', 100),
	  );
	$form['ab_concurrency'] = array(
	    '#type' => 'textfield',
	    '#title' => t('Concurrency'),
	    '#description' => t('Number of concurrenct requests.'),
	    '#default_value' => variable_get('ab_concurrency', 25),
	  );
	$form['ab_uris'] = array(
	    '#type' => 'textarea',
	    '#title' => t('Website URIs'),
	    '#description' => t('One absolute URI per line, be sure to include a trailing slash (/) for example <em>http://vision-media.ca/</em>.'),
	    '#default_value' => variable_get('ab_uris', ''),
	  );

  return system_settings_form($form);
}

/* -----------------------------------------------------------------

  Reporting

------------------------------------------------------------------ */

/**
 * Display ApacheBench reports when charting is available.
 *
 * @return string
 *  Markup.
 */
function ab_reports() {
  $output = ' ';

  if (!module_exists('open_flash_chart_api')){
	  drupal_set_message(t('In order to view these reports you must install the <a href="http://drupal.org/project/open_flash_chart_api">open_flash_chart_api</a> module.'));
  }
  else {
	  ab_require_chart();
	  $output .= ab_get_overview_report();
  }

  return $output;
}

/**
 * Create overview report.
 *
 * @return string
 *  Markup.
 */
function ab_get_overview_report() {
  $output = '';
  $rows = array();
  $header = array(t('URI'), t('Avg requests per second'), t('Total tests'), t('Avg requests'), t('Avg concurrency'), t('Avg failed'), t('Avg document length'), t('Avg time taken'));
  
  // @todo finish charts, remove table reports
  // @todo utilize tablesort api
  
  // Fetch results
  $results = db_query("
    	SELECT 
        uri,
				COUNT(*) as total_tests,
				AVG(concurrency) as avg_concurrency,
				AVG(requests) as avg_requests,
				AVG(requests_per_second) as avg_requests_per_second,
				AVG(failed_requests) as avg_failed_requests,
				AVG(time_taken_for_tests) as avg_time_taken_for_tests,
				AVG(document_length) as avg_document_length
			FROM {ab_results}
			GROUP BY uri
			ORDER BY AVG(requests_per_second) DESC
  	");
  while ($result = db_fetch_object($results)) {
    $rows[] = array(
				$result->uri, 
				floor($result->avg_requests_per_second), 
				floor($result->total_tests), 
				floor($result->avg_requests), 
				floor($result->avg_concurrency),
				floor($result->avg_failed_requests),
				format_size($result->avg_document_length),
				t('%time seconds', array('%time' => round($result->avg_time_taken_for_tests, 2))),
			);
  }
 
  // Display results
  $output .= theme('table', $header, $rows);

  return $output;
}

