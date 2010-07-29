// $Id: paging.admin.js,v 1.3 2009/01/17 02:22:34 Gurpartap Exp $

/**
 * Vertical tabs node form layout (aka drawers).
 * @author Bevan Rudge drupal.geek.nz
 * @author Joakim Stai
 * @license GPL 2
 */

$(document).ready(function(){
  // Turn all marked fieldsets into vertical tabs.
  $('.paging-fieldset').each(function(i){
    $('#paging-vertical-tabs > ul')
      .append('<li>' + $(this).find('.paging-enabled').parent().parent().html() + '<a href="#' + $(this).attr('id') + '">' + $(this).find('> legend').text() + '</a></li>')
      .find('> li').click(function() {
        $(this).find('> a').click();
      });
    $(this).removeClass('collapsible collapsed').appendTo('#paging-vertical-tabs');
    $(this).find('.paging-enabled').parent().parent().remove();
  });

  // Activate vertical tabs.
  $('#paging-vertical-tabs > ul').tabs({fx: { opacity: 'toggle', duration: 150 }, cookie: { expires: 1, path: '/' }});
  

  // Find the height of the heighest panel.
  var maxHeight = $.map($('#paging-vertical-tabs'),function(el){
    return $(el).height();
  }).sort(function(a, b) {
    return b - a;
  })[0];

  // Set the height of the container to a fixed size.
  $('#paging-vertical-tabs').height(maxHeight);

  $(".paging-method input[@type=radio]").bind("change", paging_automatic_handler).each(paging_automatic_handler);
  $(".paging-pager input[@type=radio]").bind("change", paging_pager_handler).each(paging_pager_handler);
});

function paging_automatic_handler(event) {
  var type = $(this).attr('name').substr(24);
  var checked = $('input[@name=paging_automatic_method_' + type + ']:checked').val();
  $('.paging-chars-' + type + ', .paging-words-' + type).hide();
  $('input[@name=paging_names_enabled_' + type + ']').removeAttr('disabled');

  if (checked == 1) {
    $('.paging-chars-' + type).fadeIn(500);
    $('input[@name=paging_names_enabled_' + type + ']').removeAttr('checked');
    $('input[@name=paging_names_enabled_' + type + ']').attr('disabled', 'disabled');
  }
  else if (checked == 2) {
    $('.paging-words-' + type).fadeIn(500);
    $('input[@name=paging_names_enabled_' + type + ']').removeAttr('checked');
    $('input[@name=paging_names_enabled_' + type + ']').attr('disabled', 'disabled');
  }
}

function paging_pager_handler(event) {
  var type = $(this).attr('name').substr(20);
  var checked = $('input[@name=paging_pager_widget_' + type + ']:checked').val();
  $('.paging-pager-custom-' + type).parent().hide();

  if (checked == 'custom') {
    $('.paging-pager-custom-' + type).parent().fadeIn(500);
  }
}
