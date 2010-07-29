<?php
// $Id: flickr.inc,v 1.1 2008/10/29 18:05:41 aronnovak Exp $

// start site-specific data
$post_vars = ''; // if the comments are available after a POST request

// Currently one path can contain one placeholder, no more (imagine, one placeholder - running value  - one more loop inside loops!
$xpath_comments = array(
  '//div[@id=\'DiscussPhoto\']/table/tr[%place]',
);

// No placeholder support in data members yet
$xpath_data_members = array(
  'title' => '',
  'body' => '//td[@class=\'Said\' and position()=2]/p/text()[1]',
  'author' => '/html/body/tr/td[2]/h4/a',
  'author_picture' => '/html/body/tr/td[1]/a/img/@src',
  'date' => '//p/small',
  'original_url' => '',
);
$placeholders = array(
/*
  '%placeh_name' => array(first = 0, last = UNLIMITED, step = 1),
*/
  '%place' => array(1, UNLIMITED),
);

// To decide if the given video feed is okay for this handler.
// preg_match, so perl-style please
$url_checker_regexp = '/flickr.com\/photos\//';