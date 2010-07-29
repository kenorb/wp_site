<?php

/* Helper library to create multipart/form requests

Normally we have multipart_encode($params){
  $boundary = 'A0sFSD'; //or whatever random value you want
  $output = "Content-Type: multipart/form-data; boundary=$boundary\r\n";
...

But the header has been moved outside the procedure in order to allow it to be posted directly to drupal_http_request
 */

function multipart_encode($boundary, $params){
  $output = "";
  foreach ($params as $key => $value){
    $output .= "--$boundary\r\n";
    if ($key == 'file'){
      $output .= multipart_enc_file($value);
    } else $output .= multipart_enc_text ($key, $value);
  }
  $output .="--$boundary--";
  return $output;
}

function multipart_enc_text($name, $value){
  return "Content-Disposition: form-data; name=\"$name\"\r\n\r\n$value\r\n";
}

function multipart_enc_file($path){
  if (substr($path, 0, 1) == "@") $path = substr($path, 1);
  $filename = basename($path);
  $mimetype = "application/octet-stream";
  $data = "Content-Disposition: form-data; name=\"file\"; filename=\"$filename\"\r\n";
  $data .= "Content-Transfer-Encoding: binary\r\n";
  $data .= "Content-Type: $mimetype\r\n\r\n";
  $data .= file_get_contents($path) . "\r\n";
  return $data;
}