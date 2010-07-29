// $Id: refresh.js,v 1.1.2.6 2008/12/03 22:18:51 starnox Exp $

/**
 * @file refresh.js
 *
 * Javascript needed to refresh view after image upload
 */

$(document).ready(function(){
  //Display post upload messages
  ib_get_messages();
  //Trigger exposed filters submit to refresh view
  $(".view-filters > form").trigger('submit');
});