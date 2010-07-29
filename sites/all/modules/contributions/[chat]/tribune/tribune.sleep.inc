<?php
// vim:filetype=php expandtab tabstop=2 softtabstop=2 shiftwidth=2 autoindent smartindent
// $Id: tribune.sleep.inc,v 1.3.2.2 2009/06/19 22:02:51 seeschloss Exp $

// des trucs du slipounet de LiNuCe qui marchent bien comme ça
function str_ctrl_clean($s) {
  $length = strlen($s);
  $byte   = 0 ;

  for ($i=0 ; $i < $length ; $i++) {
    $byte = ord($s[$i]);
    if ($byte < 32 or $byte == 127) {
      $s[$i] = ' ';
    }
  }
  return trim($s);
}

function str_clean($s, $length) {
  $clean = "";
  if (function_exists("mb_detect_encoding")) {
    // let us assume that mb_substr exists too then
    if (strcasecmp(mb_detect_encoding($s, 'UTF-8', TRUE), 'UTF-8') == 0) {
      $clean = mb_substr(str_ctrl_clean($s), 0, $length, 'UTF-8');
    }
  } else if (function_exists("iconv_substr")) {
    // there is no iconv_detect_encoding, so let us assume that the string is UTF-8-encoded
    $clean = iconv_substr(str_ctrl_clean($s), 0, $length, 'UTF-8');
  } else {
    // better than nothing?
    //$clean = substr(str_ctrl_clean($s), 0, $length);
    // actually, we'll just pass it as it is, rather than risking splitting a character in two
    $clean = $s;
  }

  return $clean;
}

function tribune_sleep($message, $length = -1) {
  $message = str_clean($message, $length);

  $message = preg_replace_callback(':<(m|s|u|b|i|tt)>(.*?)</\1>:', 'tribune_sleep_replace', $message);

  $replacement = array(
        "&" =>   "&amp;",
        "<" =>    "&lt;",
        ">" =>    "&gt;",
        "'" =>  "&#039;",
        '"' =>  "&quot;",
    chr(26) =>       "<",
    chr(27) =>       ">",
    chr(28) =>       "'",
    chr(29) =>       '"',
  );

  $message = str_replace(array_keys($replacement), array_values($replacement), $message);

  return $message;
}

function tribune_sleep_replace($matches) {
  $text = preg_replace_callback(':<(m|s|u|b|i|tt)>(.*?)</\1>:', 'tribune_sleep_replace', $matches[2]);

  switch ($matches[1]) {
    case 'm':
      $start = "====> ". chr(26) ."strong". chr(27) ."Moment ";
      $stop  = chr(26) ."/strong". chr(27) ." <====";
      break;
    case 'b':
      $start = chr(26) . "strong" . chr(27);
      $stop = chr(26) . "/strong" . chr(27);
      break;
    case 'i':
      $start = chr(26) . "em" . chr(27);
      $stop = chr(26) . "/em" . chr(27);
      break;
    case 'u':
      $start = chr(26) . "span style=" . chr(28) . "text-decoration: underline;" .chr(28) . chr(27);
      $stop = chr(26) . "/span" . chr(27);
      break;
    case 's':
      $start = chr(26) . "span style=" . chr(28) . "text-decoration: line-through;" .chr(28) . chr(27);
      $stop = chr(26) . "/span" . chr(27);
      break;
    case 'tt':
      $start = chr(26) . "code" . chr(27);
      $stop = chr(26) . "/code" . chr(27);
      break;

  }

  return $start . $text . $stop;
}

