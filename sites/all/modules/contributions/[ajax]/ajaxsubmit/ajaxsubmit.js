// $Id: ajaxsubmit.js,v 1.5 2008/12/04 18:02:26 nedjo Exp $

Drupal.ajaxSubmit = Drupal.ajaxSubmit || {};

/**
 * Attaches the ajaxsubmit behaviour to forms.
 */
Drupal.behaviors.ajaxSubmit = function () {
  $('form.ajaxsubmit:not(.ajaxsubmit-processed)').each(function () {
    var target = document.createElement('div');
    var form = this;
    $(target).addClass('ajaxsubmit-message');
    $(this).before(target);
    var options = {
      dataType: 'json',
      beforeSubmit: function () {
        Drupal.ajaxSubmit.beforeSubmit(target, form);
      },
      success: function (data) {
        Drupal.ajaxSubmit.success(target, data, form);
      },
     complete: function(data, status) {
       Drupal.ajaxSubmit.error(target, form);
       if (status == 'error' || status == 'parsererror') {
         alert(Drupal.t('An error occurred.'));
       }
     },
    };
    $(this)
      .addClass('ajaxsubmit-processed')
      .ajaxForm(options)
      .find('input[name=ajaxsubmit]')
      .attr('value', '1')
      .end()
      .find('input.form-submit')
      .click(function() {
        $(this).addClass('throbbing');
      });
  });
};

/**
 * Tidy up on form submission.
 */
Drupal.ajaxSubmit.beforeSubmit = function (target, form) {
  // Remove any error messages.
  $(':input', form).removeClass('error');
  $(target).html('');
};

/**
 * Handler for successful response.
 */
Drupal.ajaxSubmit.success = function (target, data, form) {
  $('form input.throbbing').removeClass('throbbing');
  $(target).html(data.message);
  if (data.errors) {
    for (var name in data.errors) {
      $('[@name=' + name + ']', form).addClass('error');
    }
  }
  // Set preview.
  if (data.preview) {
    $(target).append(data.preview);
  }

  // Invoke any callback functions.
  if (data.__callbacks) {
    $.each(data.__callbacks, function(i, callback) {
      eval(callback)(target, response);
    });
  }

  var offset = $(target).offset();
  window.scrollTo(0, offset.top - 10);

  // Redirect.
  if (data.destination) {
    window.location = Drupal.url(data.destination);
  }
};

/**
* Handler for error.
*/
Drupal.ajaxSubmit.error = function (target, data, form) {
 $('input.form-submit', form).removeClass('throbbing');
};
