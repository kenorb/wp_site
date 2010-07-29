<?php
// $Id: signature.tpl.php,v 1.1.2.2 2008/05/23 02:36:15 liammcdermott Exp $

/**
 * @file
 * Default theme implementation to format a user's signature.
 *
 * Variables available:
 * - $signature: A user's signature.
 */
?>
<?php if (!trim($signature) == ''): ?>
<div class="signature"><?php print $signature; ?></div>
<?php endif; ?>
