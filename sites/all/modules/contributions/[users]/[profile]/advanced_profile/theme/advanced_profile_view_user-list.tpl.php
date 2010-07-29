<?php
// $Id: advanced_profile_view_user-list.tpl.php,v 1.2.4.4 2009/01/05 05:19:43 michellec Exp $

/**
 * @file
 * Theme implementation to display individual items on the user list view.
 */

 /**
 * View: user_list
 *
 * Variables available:
 * $view -- the entire view object. Important parts of this object are
 *   user_list, .
 * $view_type -- The type of the view. Probably 'page' or 'block' but could
 *   also be 'embed' or other string passed in from a custom view creator.
 * $node -- the raw data. This is not a real node object, but will contain
 *   the nid as well as other support fields that might be necessary.
 * $count -- the current row in the view (not TOTAL but for this page) starting
 *   from 0.
 * $stripe -- 'odd' or 'even', alternating.
 * $uid -- Display the user picture of the author.
 * $uid_label -- The assigned label for $uid
 * $name -- This will display the author of the node.
 * $name_label -- The assigned label for $name
 * $field_about_me_value --
 * $field_about_me_value_label -- The assigned label for $field_about_me_value
 */

?>

<?php
$account = user_load(array('uid' => $node->users_uid));
$author_pane = theme_advanced_profile_author_pane($account);
?>

<div class="user-list-item clear-block">
  <div class="user-list-author-pane">
    <div class="author-name"><?php print $name; ?></div>
    <?php print $author_pane; ?>
  </div>

  <div class="user-list-about-me">
    <?php print $field_about_me_value?>
  </div>
</div>

