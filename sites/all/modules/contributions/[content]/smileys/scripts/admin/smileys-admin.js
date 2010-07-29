// $Id: smileys-admin.js,v 1.1.2.3 2009/01/05 08:50:26 Gurpartap Exp $

var smileysLoader = {
  start: function() {
    $('body').append('<div class="smileys-progress-panel">' + Drupal.t('Saving...') + '</div>');
  },
  stop: function() {
    $('div.smileys-progress-panel').html(Drupal.t('Saved...')).fadeOut("slow", function() { $(this).remove(); });
  },
  error: function() {
    alert(Drupal.t("An error occurred while requesting the server. Be sure that you have proper permission(s), and the server is responding properly."));
    $('div.smileys-progress-panel').remove();
  }
};

checkEnter = function(event) {
  var e = (event) ? event : ((window.event) ? window.event : "");
  if (e) {
    if (e.keyCode === 13 || e.which === 13) {
      $('td.category a.save').trigger('click');
      $('#smileys-admin-form').submit(function() { return false; });
    }
  }
}

Drupal.behaviors.smileysAdmin = function() {  
  $('td.category span').bind('click', function() {
    $(this).hide().parent().append('<input type="textfield" class="form-text"/><a href="#" class="save" title="Save category title">Save</a>')
      .find('input').val(this.innerHTML).focus().bind('keypress', function(event) { checkEnter(event); })
      .find('~ a').bind('click', function() {
        var basePath = Drupal.settings.basePath;
        var orig_span = $(this).parent().find('span');
        var orig_name = orig_span.text();
        var new_name = $.trim($(this).prev().val());
        if (orig_name !== new_name && new_name !== '') {
          $.ajax({
            type: 'GET',
            url: basePath + 'admin/settings/smileys/category/' + orig_name + '/' + new_name,
            beforeSend: function() {
              smileysLoader.start();
            },
            success: function(msg){
              setTimeout(function() {
                orig_span.html(new_name).show().find('~ input, ~ a').remove();
                  $('table#smileys_admin_list tr td:has(select)').each(function() {
                    $(this).find('select option').each(function() {
                      if (this.value === orig_name) {
                        this.value = new_name;
                        this.innerHTML = new_name;
                      }
                  });
                });
                smileysLoader.stop();
              }, 500);
            },
            error: function() {
              smileysLoader.error();
            }
          });
        }
        return false;
      });
  });
  $('span.smiley-selected, span.smiley-deselected, span.smiley-middle', 'table').bind('click', function() {
    var current = $(this);
    var class_attr = current.attr('class');
    var basePath = Drupal.settings.basePath;

    if (class_attr === 'smiley-selected') {
      var promote_to_box = 0;
    }
    else if (class_attr === 'smiley-deselected') {
      var promote_to_box = 1;
    }
    else if (class_attr === 'smiley-middle') {
      var promote_to_box = 2;
    }
    var classes = ['smiley-selected', 'smiley-deselected', 'smiley-middle'];
    var key = (promote_to_box + 1) > 2 ? 0 : (promote_to_box + 1);
    var new_class = classes[key];
    var id = current.attr('id');

    $.ajax({
      type: 'GET',
      url: basePath + 'admin/settings/smileys/promote/' + (promote_to_box-1) + '/' + id,
      beforeSend: function() {
        smileysLoader.start();
      },
      success: function(msg){
        current.removeAttr('class').addClass(new_class);
        setTimeout(function() {
          smileysLoader.stop();
        }, 500);
      },
      error: function() {
        smileysLoader.error();
      }
    });
    return false;
  });
};

Drupal.behaviors.smileyDrag = function(context) {
  var basePath = Drupal.settings.basePath;
  $('table#smileys_admin_list').each(function() {
    $('<button class="smileys-save-weights">Save</button>').insertAfter(this).bind('mousedown keydown', function() {
      var ids = '';
      var weights = '';
      var categories = '';
      $('form#smileys-admin-form input').each(function() {
         ids = ids ? ids + ':' + this.id : this.id;
         weights = weights ? weights + ':' + this.value : this.value;
      });
      $('form#smileys-admin-form select').each(function() {
         categories = categories ? categories + ':' + this.value : this.value;
      });
      $.ajax({
        type: 'GET',
        url: basePath + 'admin/settings/smileys/weight/' + weights + '/' + ids + '/' + categories,
        beforeSend: function() {
          smileysLoader.start();
        },
        success: function(msg){
        setTimeout(function() {
          smileysLoader.stop();
          window.location = window.location;
        }, 1000);
        },
        error: function() {
          smileysLoader.error();
        }
      });
      return false;
    });
  });
  $('#smileys_admin_list td .tabledrag-handle').each(function() {
    $('~ .nub', this).prepend($(this));
  });
};
