<?php

/* This file is part of "Broken Anchor for Node comments Module".
 *    Copyright 2009, arNuméral
 *    Author : Yoran Brault
 *    eMail  : yoran.brault@bad_arnumeral.fr (remove bad_ before sending an email)
 *    Site   : http://www.arnumeral.fr/node/2
 *
 * "Broken Anchor for Node comments Module" is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * "Broken Anchor for Node comments Module" is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with "Broken Anchor for Node comments Module"; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA, or see the FSF site: http://www.fsf.org.
 */


function broken_anchor_report() {
  $date=variable_get("broken_anchor_statistics_last_cron",null);
  $output.=t("Last check : @date", array("@date"=>$date==0?t("never"):format_date($date)))."<br>";
  foreach (module_implements('broken_anchor_info') as $module) {
    $type[$module]=module_invoke($module, 'broken_anchor_info');
    $progress=module_invoke($module, 'broken_anchor_get_progress',broken_anchor_last_check_id($module));
    $output.=t("@type checked : @percent %",
    array(
        "@type"=>$type[$module]['description'], 
        "@percent"=>$progress['total']==0?0:number_format($progress['passed']/$progress['total'],3)
    ))." (".$progress['passed']."/".$progress['total'].")<br>";
  }
  $header = array(t('checked'), t('ID'), t('url'), t('count'), t('status'), t('operation'));
  $cursor=pager_query("
    select * 
    from broken_anchor_errors b 
    where ignored is null 
    order by b.cid,checked", 
  broken_anchor_report_page_size(), 0, "
    select count(*) 
    from {broken_anchor_errors} 
    where ignored is null");


  $rows=array();
  while ($error = db_fetch_object($cursor)) {
    if ($error->match_type==0) {
      $url=parse_url($error->match);
      $match=$url['scheme']==''?$error->match:l($url['host'], $error->match,array('attributes'=>array('target'=>'_blank')));
    } else {
      $match=$error->match;
    }

    $rows[]=array(
    format_date($error->checked,"small"),
    module_invoke($error->module, 'broken_anchor_link', $error->cid),
    $match,
    $error->count,
    $error->status,
    l(t('Check'), 'admin/reports/broken_anchor/check/'. $error->module."/".$error->cid)." - ".
    l(t('Ignore'), 'admin/reports/broken_anchor/ignore/'. $error->eid)
    );
  }
  if (count($rows)==0) {
    $output.= t("No error found");
  } else  {
    $output.=theme('table', $header, $rows);
    $output.=theme('pager', NULL, variable_get('default_nodes_main', 10));
  }
  return $output;
}

function broken_anchor_update_node($module, $cid) {
  $contents=module_invoke($module, 'broken_anchor_get_contents', $cid, 1);
  if (count($contents)!=1) {
    $message=t("Unable to get content from @module with id = @id", array("@module"=>$module, "@id"=>$cid));
  } else {
    $errors=broken_anchor_check_content($module, $contents[0]);
    $message = t("Content checked again. Report is updated");
  }
  drupal_set_message($message);
  drupal_goto('admin/reports/broken_anchor');
}

function broken_anchor_ignore_error($eid) {
  db_query("update {broken_anchor_errors} set ignored=1 where eid=%d",$eid);
  $message = t('This error is now ignored.');
  drupal_set_message($message);
  drupal_goto('admin/reports/broken_anchor');
}