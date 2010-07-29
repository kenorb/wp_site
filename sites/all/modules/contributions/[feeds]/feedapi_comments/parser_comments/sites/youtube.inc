<?php
// $Id: youtube.inc,v 1.1 2008/10/27 19:23:00 aronnovak Exp $

/**
 * @file
 *   youtube.com info to scraping comments from the site
 */

// Currently one path can contain one placeholder, no more (imagine, one placeholder - running value  - one more loop inside loops!
$xpath_comments = array(
  '/html/body/div[1]/div[4]/div[6]/div/div[1]/div[3]/div[3]/div[3]/div[%place]',
  '/html/body/div[1]/div[4]/div[3]/div[@class=\'watch-tab-contents\']/div[1]/div[3]/div[3]/div[3]/div[%place]',
  '/html/body/div[1]/div[4]/div[4]/div[@class=\'watch-tab-contents\']/div[1]/div[3]/div[3]/div[3]/div[%place]',
  '/html/body/div[1]/div[4]/div[5]/div[@class=\'watch-tab-contents\']/div[1]/div[3]/div[3]/div[3]/div[%place]',
);

// No placeholder support in data members yet
$xpath_data_members = array(
  'body' => '/html/body/div/div[2]/div[1]/div',
  'author' => '/html/body//div[1]/div[1]/a[1]',
  'date' => '/html/body//div[1]/div[1]/span[@class=\'watch-comment-time\']',
);

$placeholders = array(
//'%placeh_name' => array(first = 0, last = UNLIMITED, step = 1),
  '%place' => array(1, UNLIMITED),
);

// To decide if the given video feed is okay for this handler.
// preg_match, so perl-style please
$url_checker_regexp = '/youtube.com/';
