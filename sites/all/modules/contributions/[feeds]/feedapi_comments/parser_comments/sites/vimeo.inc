<?php
// $Id: vimeo.inc,v 1.2 2008/10/06 11:11:19 aronnovak Exp $

/**
 * @file
 *   vimeo.com info to scraping comments from the site
 */

// Currently one path can contain one placeholder, no more (imagine, one placeholder - running value  - one more loop inside loops!
$xpath_comments = array(
  '/html/body/div[2]/div[2]/div[3]/div[@class=\'columns\']/div[2]/ul[@class=\'comments\']/li[@class=\'parent first\' and position()=%first_place]',
  '/html/body/div[2]/div[2]/div[3]/div[@class=\'columns\']/div[2]/ul[@class=\'comments\']/li[%place]',
);

// No placeholder support in data members yet
$xpath_data_members = array(
  'title' => '',
  'body' => '/html/body/li/div/div[2]',
  'author' => '/html/body/li/div/div[1]/a',
  'date' => '/html/body/li/div/div[1]',
  'original_url' => '/html/body/li/a/@name',
  'author_picture' => '/html/body/li/img/@src',
);

$placeholders = array(
//'%placeh_name' => array(first = 0, last = UNLIMITED, step = 1),
  '%first_place' => array(1, 1),
  '%place' => array(2, UNLIMITED),
);

// To decide if the given video feed is okay for this handler.
// preg_match, so perl-style please
$url_checker_regexp = '/vimeo.com\/([0-9]*)/';
