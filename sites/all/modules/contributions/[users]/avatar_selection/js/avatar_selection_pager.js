/* $Id: avatar_selection_pager.js,v 1.1.2.2 2008/06/30 23:43:04 snpower Exp $ */

/**
 * Fetches the new page and puts it inline.
 *
 * @param form_id   - The id of the form whose action should be updated.
 * @param id   - The element id whose contents will get replaced.
 * @param url - The URL for the new page.
 * @param page  - The page number to request.
 * @param js_file  - The javascript file to run upon completion.
 */
function fetchPage(form_id, id, url, page, js_file) {
  $.get(url, {page: page}, function(data, status) {
    var selects = $(data).find(id);
    $(id).html(selects);
    var pager = $(data).find(".avatar_selection_pager_nav");
    $(".avatar_selection_pager_nav").html(pager);
    $.getScript(js_file);
    var action = url + "?page="+page;
    $(form_id).attr("action", action);
  });
  return false;
}
