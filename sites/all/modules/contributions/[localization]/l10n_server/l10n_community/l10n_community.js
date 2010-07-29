// $Id: l10n_community.js,v 1.1.2.8.2.2 2008/12/30 16:30:20 goba Exp $

l10nCommunity = {};

l10nCommunity.switchPanes = function(elem, id) {
  if ($(elem).parents('.translation').find('.'+id).css('display') == 'none') {
    // execute animation once per translation
    var once = 0;
    $(elem).parents('.translation').find('.pane:not(.'+id+')').each(function() {
      if ($(this).css('display') == 'block') {
        $(this).slideUp('fast', function() {
          if (once == 0) {
            once = 1;
            $(this).parents('.translation').find('.'+id).slideDown('fast');
          }          
        });
      }
    });
  }
  $(elem).parents('.toolbox').find('span.l10n-button').removeClass('active');
  $(elem).addClass('active');  
}

/**
 * Initialize action images: toolboxes and string copy buttons.
 */
l10nCommunity.init = function() {
  // Only attempt to register events if form exists
  if ($('#l10n-community-translate-form').size() > 0) {  
    // When the copy button is clicked, copy the original string value to the
    // translation field for the given strings. Relations are maintained with
    // the string ideitifiers.
    $('span.l10n-community-copy').click(function() {
      l10nCommunity.copyString(this);
    ;})

    // Screw AJAX submit -- we'll just hard submit the form for now
    $('span.l10n-save').click(function() {
      $('#l10n-community-translate-form').submit();
    });

    // Clear sibling text fields
    $('span.l10n-clear').click(function() {
      $(this).parents('.translation').find('input.form-text, textarea').val('');
    });

  
    $('#l10n-community-translate-form .l10n-translate').click(function() {
      // switch display panes
      l10nCommunity.switchPanes(this, 'translate');
    });
  
    $('#l10n-community-translate-form .l10n-lookup').click(function() {
      // switch display panes
      var elem = this;
      if ($(this).is(".active")) {
        // Switch back to editing form if already clicked. Convenience feature,
        // so that you don't need to move your mouse to switch back.
        var parent = $(elem).parents('.translation');
        var tool = $('.l10n-translate', parent);
        l10nCommunity.switchPanes(tool, 'translate');
        return;
      }
      var sid = $(this).parents('.translation').attr('id').substring(6);
      $.get(Drupal.settings.l10n_details_callback + sid, null, function(data) {
        $('#tpane-' + sid + ' .lookup').empty().append(data);
        l10nCommunity.switchPanes(elem, 'lookup');
      });
    });

    $('#l10n-community-translate-form .l10n-suggestions').click(function() {
      // switch display panes
      var elem = this;
      if ($(this).is(".active")) {
        // Switch back to editing form if already clicked. Convenience feature,
        // so that you don't need to move your mouse to switch back.
        var parent = $(elem).parents('.translation');
        var tool = $('.l10n-translate', parent);
        l10nCommunity.switchPanes(tool, 'translate');
        return;
      }
      var sid = $(this).parents('.translation').attr('id').substring(6);
      $.get(Drupal.settings.l10n_suggestions_callback + sid, null, function(data) {
        $('#tpane-' + sid + ' .suggestions').empty().append(data);
        l10nCommunity.switchPanes(elem, 'suggestions');
        var suggestions = $('#tpane-' + sid + ' .suggestions');
        $('span.l10n-community-copy', suggestions).click(function() {
          l10nCommunity.copyString(this);
        });
        // Hide the has-suggestion marker if we don't have suggestions anymore.
        // Could happen if we declined the last suggestion and reloading.
        $('#l10n-community-editor-' + sid + ' .l10n-no-suggestions').parent().parent().find('.l10n-has-suggestion').hide();
      });
    });
  }
}

l10nCommunity.copyString = function(elem) {
  var item = $(elem).parents('li').find('div.string > div');
  var parentlist = $(item).parents('ul.l10n-community-strings');
  var original = $('.original', item).html();
  var sid = item.attr('class').substring(7);

  if (sid.indexOf('-') > 0) {
    // Copying orignal plural string or active plural translation. We should
    // get the original sid prefix and then copy the strings to the textareas.
    sid = sid.split('-');
    sid = sid[0];
    for (i = 0; i < Drupal.settings.l10n_num_plurals; i++) {
      $('#l10n-community-translation-'+ sid +'-'+ i).val($('.string-'+ sid +'-'+ i +' .original', parentlist).html());
    }
  }
  else if (original.indexOf(";  ") > 0)  {
    // Copying plural suggestions from the suggestion list. This has the special
    // ";  " delimiter (which we suppose does not exist in source strings).
    var strings = original.split(";  ");
    for (string in strings) {
      $('#l10n-community-translation-'+ sid +'-'+ string).val(strings[string]);
    }
  }
  else {
    // Otherwise simple string. Just copy over the original string.
    $('#l10n-community-translation-' + sid).val(original);
  }

  // Show the editing controls.
  $('#l10n-community-wrapper-' + sid).show();
  
  // Switch to translate pane. The pane is in the translation column, so
  // if we are in the source column, we need to switch columns.
  var parent = $(elem).parents('td.source, td.translation');
  if (parent.get(0).className == 'source') {
    parent = $(parent.get(0)).siblings();
  }
  var tool = $('.l10n-translate', parent);
  l10nCommunity.switchPanes(tool, 'translate');
}

/**
 * Suggestion approval callback.
 */
l10nCommunity.approveSuggestion = function(tid, sid, elem, token) {
  // Invoke server side callback to save the approval.
  $.ajax({
    type: "GET",
    url: Drupal.settings.l10n_approve_callback + tid + Drupal.settings.l10n_form_token_path + token,
    success: function (data) {
      if (data == 'done') {
        // Empty translate pane and inform user that the suggestion was saved.
        $('#tpane-'+ sid +' .translate').empty().append(Drupal.settings.l10n_approve_confirm);
        // Switch back to translate pane and hide suggestion icon
        var parent = $(elem).parents('.translation');
        l10nCommunity.switchPanes($('.l10n-translate', parent), 'translate');
        $('.l10n-suggestions', parent).hide();
      }
      else {
        alert(Drupal.settings.l10n_approve_error);
      };
    },
    error: function (xmlhttp) {
      // Being an internal/system error, this is not translatable.
      alert('An HTTP error '+ xmlhttp.status +' occured.\n'+ uri);
    }
  });
  // Return false for onclick handling.
  return false;
}

/**
 * Suggestion decline callback.
 */
l10nCommunity.declineSuggestion = function(tid, sid, elem, token) {
  // Invoke server side callback to save the decline action.
  $.ajax({
    type: "GET",
    url: Drupal.settings.l10n_decline_callback + tid + Drupal.settings.l10n_form_token_path + token,
    success: function (data) {
      if (data == 'done') {
        // Reload info pane.
        $('#tpane-' + sid + ' .l10n-suggestions').click();
      }
      else {
        alert(Drupal.settings.l10n_decline_error);
      };
    },
    error: function (xmlhttp) {
      // Being an internal/system error, this is not translatable.
      alert('An HTTP error '+ xmlhttp.status +' occured.\n'+ uri);
    }
  });
  // Return false for onclick handling.
  return false;
}

/**
 * Suggestion editing copy callback.
 */
l10nCommunity.copySuggestion = function(sid, translation) {
  if (translation.indexOf("\O") > 0) {
    // If we have the null byte, suggestion has plurals, so we need to
    // copy over the distinct strings to the distinct textareas.
    var strings = translation.split("\O");
    for (string in strings) {
      $('#l10n-community-translation-'+ sid +'-'+ string).val(strings[string]);
    }
  }
  else {
    // Otherwise standard string.
    $('#l10n-community-translation-'+ sid).val(translation);
  }
  // Show the editing controls.
  $('#l10n-community-wrapper-'+ sid).css('display', 'block');
  return false;
}

// Global killswitch
if (Drupal.jsEnabled) {
  $(document).ready(l10nCommunity.init);
}
