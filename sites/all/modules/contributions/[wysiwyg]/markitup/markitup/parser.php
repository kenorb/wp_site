<?php

// include Texy!
require_once dirname(__FILE__).'/texy/texy.php';

$texy = new Texy();
$html = $texy->process(stripslashes($_POST["data"]));  // that's all folks!

echo $html;

?>