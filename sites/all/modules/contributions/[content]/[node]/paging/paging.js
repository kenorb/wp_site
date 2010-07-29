// $Id: paging.js,v 1.11 2009/01/24 16:12:52 Gurpartap Exp $

Number.prototype.ordinal = function () {
  return this + (
    (this % 10 == 1 && this % 100 != 11) ? '<sup>st</sup>' :
    (this % 10 == 2 && this % 100 != 12) ? '<sup>nd</sup>' :
    (this % 10 == 3 && this % 100 != 13) ? '<sup>rd</sup>' : '<sup>th</sup>'
  );
};

function paging_handle_names(pages) {
  var output = '';

  if (pages.length > 0) {
    for (var x = 0; x < pages.length; x++) {
      var title = $('#edit-title').val();
      var name = Drupal.t('!title - Page !number', {'!title': title, '!number': (x + 1)});
      pages[x] = (pages[x] || name);
      output += '<label for="edit-paging-title[' + x + ']">' + Drupal.t("Name of !number page: ", {'!number': (x + 1).ordinal()}) + '</label>'
        + "\t" + '<input type="text" class="form-text" value="' + pages[x] + '" size="20" name="edit-paging-title[' + x + ']" maxlength="255"/>' + "\n";
    }
    $('#paging-names-wrapper').show().each(function() {
      $(this).html('<fieldset class="form-item" id="paging-page-names" style="overflow: hidden;">'
      + '<legend class="">' + Drupal.t("Page names") + '</legend><div id="paging-names-inner" class="form-item"></div></fieldset>');
    });
    $('#paging-names-wrapper #paging-names-inner').html(output);
  }
  else {
    $('#paging-names-wrapper').hide();
  }
}

function paging_return_names() {
  var names = [];
  var i = 0;
  $('#paging-page-names').find('input[@type=text]').each(function() {
    names[i] = $(this).val();
    i++;
  }).parents('fieldset').remove();

  return names;
}

function paging_insert_separator() {
  var separator = Drupal.settings.paging.separator;
  $('textarea#edit-body').each(function() {
    if (document.selection) {
      this.focus();
      document.selection.createRange().text = separator;
    }
    else if (this.selectionStart || this.selectionStart == '0') {
      var cursorPos = this.selectionEnd + separator.length;
      this.value = this.value.substring(0, this.selectionEnd) + separator + this.value.substring(this.selectionEnd);
      this.selectionStart = this.selectionEnd = cursorPos;
    }
    else {
      this.value = this.value + separator;
    }
    this.focus();
  }).trigger('click');

  return false;
}

Drupal.behaviors.paging = function(context) {
  // Handle the widget button to insert paging separator.
  $('textarea.teaser').each(function() {
    var include = $('#' + this.id.substring(0, this.id.length - 2) + 'include');
    var widget = Drupal.settings.paging.widget;

    if (widget == 1) {
    // Add paging separator string image.
      var path = Drupal.settings.basePath + Drupal.settings.paging.module_path;
      var string = Drupal.t('Insert page separator');
      var image = $('<div class="paging-button-wrapper"><img alt="" src="' + path + '/paging-separator.png" class="paging-separator-image" /></div>');
      $(include).parent().parent().before(image);
      $('img', image).attr('alt', string).attr('title', string).bind('click', paging_insert_separator);
    }
    else if (widget == 2) {
      // Add paging separator string button.
      var button = $('<div class="paging-button-wrapper"><input type="button" class="paging-separator-button" /></div>');
      $(include).parent().parent().before(button);
      $('input', button).val(Drupal.t('Insert page separator')).bind('click', paging_insert_separator);
    }
  });

  var page_names = typeof Drupal.settings.paging != 'undefined' ? Drupal.settings.paging.page_names : 0;
  if (page_names == 1) {
  // Page names interface is enabled.
    $('#edit-body-wrapper').after('<div id="paging-names-wrapper"></div>');

    $('textarea#edit-body').each(function() {
      var str = $(this).val();
      var match = str.match(/<!--pagenames:(.*?)-->/) || [];
      $(this).val(str.replace(/<!--pagenames:(.*?)-->/, ''));
      var pages = typeof match[1] != 'undefined' ? match[1].split('||') : [];
      paging_handle_names(pages);
    }).bind('click keyup blur', function() {
      var str = $(this).val();
      var separator = Drupal.settings.paging.separator;
      var count = 0;
      var current_names = paging_return_names();
      var pages = [(current_names[count] ? current_names[count] : '')];
      var pos = str.indexOf(separator);
      while (pos != -1) {
        count++;
        pages.push(current_names[count] ? current_names[count] : '');
        pos = str.indexOf(separator, pos + 1);
      }
      paging_handle_names(str.indexOf(separator) != -1 ? pages : 0);
    });

    $('#node-form').submit(function() {
      $('textarea#edit-body').each(function() {
        var names = paging_return_names();
        if (names.length > 0) {
          $(this).val('<!--pagenames:' + names.join('||') + '-->' + this.value);
        }
      });
    });
  }

  $('div.paging-pager-contents:not(.pager-processed)', context).each(function() {
    var $target = $(this);
    $target.addClass('pager-processed')
    // Process pager links.
    .find('ul.pager > li > a, table.paging-drop-down a')
    .each(function() {
      var ajaxPath = $(this).attr('href');
      $(this).click(function () {
        $link = $(this);
        $link.addClass('throbber');
        $.ajax({
          url: ajaxPath,
          type: 'GET',
          dataType: 'json',
          data: {'paging_json_request': $target.attr('id')},
          success: function(response) {
            $link.removeClass('throbber');
            if (response.content) {
              var $newContent = $(response.content);
              $target.replaceWith($newContent);
              Drupal.attachBehaviors($newContent.parent());
              var offset = $newContent.parent().offset().top;
              $('html,body').animate({scrollTop: offset}, 500);
            }
          },
          error: function() {
            $link.removeClass('throbber');
            alert(Drupal.t("An error occurred at @path.", {'@path': ajaxPath}));
          }
        });
        return false;
      });
    });
  });
};
