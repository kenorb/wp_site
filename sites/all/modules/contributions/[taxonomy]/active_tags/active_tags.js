// $Id: active_tags.js,v 1.1.2.2 2008/09/18 07:55:34 darrenmuk Exp $

function activetags_activate(context) {
  if ($(context).length == 1) { 
    var tagarea = activetags_widget(context);
    $(context).before(tagarea); 
    Drupal.behaviors.autocomplete(document);
  }
  $('.add-tag:not(.tag-processed)').click(function() { 
    jQuery.each($(this).prev().val().split(','), function (i, v) { 
      if (jQuery.trim(v) != '') {
        activetags_add(context, v);
      }
    }); 
    activetags_update(context);  
    $(this).prev().val('');
  }).addClass('tag-processed');
  
  if ($.browser.mozilla) {
    $('.tag-entry:not(.tag-processed)').keypress(activetags_check_enter).addClass('tag-processed');
  } else {
    $('.tag-entry:not(.tag-processed)').keydown(activetags_check_enter).addClass('tag-processed');
  }
  
  jQuery.each($(context + ' input.form-text').attr('value').split(','), function (i, v) { 
    if (jQuery.trim(v) != '') {
      activetags_add(context, v);
    }
  }); 
  
  $(context).hide(); 
}

function activetags_check_enter(event) {
  if (event.keyCode == 13) {
    $('#autocomplete').each(function () {
      this.owner.hidePopup();
    })
    $(this).next().click();
    event.preventDefault();
    return false;
  }  
}

function activetags_add(context, v) {
  if (jQuery.trim(v) != '') {
    $(context).prev().children('.tag-holder').append('<div class="tag-tag"><span class="tag-text">'+ 
      jQuery.trim(v)+'</span><span class="remove-tag">x</span></div>'); 
    $('.remove-tag:not(.tag-processed)').click(function() { 
      $(this).parent().remove();
      activetags_update(context);   
    }).addClass('tag-processed'); 
  }
}

function activetags_update(context) {
  $(context).children('input.form-text').val('');
  $(context).prev().children('.tag-holder').children().children('.tag-text').each(function(i) {
    if(i==0) {
      $(context).children('input.form-text').val($(this).text());
    } else { 
      $(context).children('input.form-text').val(
        $(context).children('input.form-text').val() + ', ' + $(this).text()
      );
    }
  });
}

function activetags_widget(context) {
  var vid = context.substr(20,1);
  return '<div id="'+context+'-activetags" class="form-item">'+
    '<label for="'+context+'-edit-tags">'+ $(context + ' label').text() +'</label>'+  
    '<div class="tag-holder"></div>'+ 
    '<input type="text" class="tag-entry form-autocomplete" size="30" id="active-tag-edit0'+vid+'" />'+ 
    '<input type="button" value="add" class="add-tag">'+
    '<input class="autocomplete" type="hidden" id="active-tag-edit0'+vid+'-autocomplete" '+
    'value="'+Drupal.settings.basePath+'taxonomy/autocomplete/'+vid+'" disabled="disabled" />'+
  '</div>';
}

Drupal.behaviors.tagger = function (context) {
  jQuery.each(Drupal.settings['active_tags'],function(i, v) {
    if ($(v).length == 1) {
      activetags_activate(v); 
    } 
  });
}




