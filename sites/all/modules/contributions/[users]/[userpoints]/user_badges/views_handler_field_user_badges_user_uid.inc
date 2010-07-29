<?php
// $Id: views_handler_field_user_badges_user_uid.inc,v 1.1 2008/11/20 17:12:09 heine Exp $
/**
 * A handler to provide proper displays for user badges.
 *
 * Currently displays the badges returned from the user search.
 * @TODO: Merge this query into the view query itself.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_user_badges_user_uid extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    return user_badges_for_uid($value);
  }
}
