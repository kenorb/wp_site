<?php
// $Id: imagebrowser-thumbnail.tpl.php,v 1.1.2.4 2008/12/03 22:18:51 starnox Exp $

/**
 * @file
 * This template outputs individual thumbnails in Image Browser's browser.
 *
 * Available variables:
 *  string $thumbnail
 *  string $path
 *  string $title
 */
?>
<a href="<?php print $path; ?>" title="<?php print $title; ?>"><img src="<?php print $thumbnail; ?>" alt="<?php print $title; ?>" /></a>