<?php
// $Id: domain_views_handler_filter_domain_access_current_all.inc,v 1.1 2008/11/06 16:32:07 agentken Exp $

/**
 * @file
 * Interface between domain_views.module and views.module.
 */

/**
 * Filter out nodes published to all domains or current domain
 */
class domain_views_handler_filter_domain_access_current_all extends views_handler_filter_boolean_operator  {
  function query() {
    $table = $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "(($table.realm = 'domain_id' AND $table.$this->real_field ". (empty($this->value) ? '<>' : '=') ." ***CURRENT_DOMAIN***) OR ($table.realm = 'domain_site' AND $table.$this->real_field = 0))");
  }
}