/* $Id$ */

if (!Drupal.ConditionalFields) {
  Drupal.ConditionalFields = {};
}

Drupal.ConditionalFields.switchField = function(id, values, onPageReady) {
  /* For each controlling field: find the controlled fields */
  $.each(Drupal.settings.ConditionalFields.controlling_fields, function(controllingField, controlledFields) {
    if (controllingField == id) {
      /* Find the settings of the controlled field */
      $.each(controlledFields, function(i, fieldSettings) {
        Drupal.ConditionalFields.doAnimation(fieldSettings, 'hide', onPageReady);
        /* Find the trigger values of the controlled field (for this controlling field) */
        $.each(fieldSettings.trigger_values, function(ii, val) {
          if (jQuery.inArray(val, values) != -1) {
            Drupal.ConditionalFields.doAnimation(fieldSettings, 'show', onPageReady); 
            /* Stop searching in this field */
            return false;
          }
        });
        /* To do: Feature: Multiple controlling fields on the same field, are
           not supported for now. Test: other controlling fields types and widgets. */
      });
    }
  });
}

Drupal.ConditionalFields.doAnimation = function(fieldSettings, showOrHide, onPageReady) {
  /* Multiple fields are enclosed in a wrapper */
  if ($(fieldSettings.field_id).parents('#' + fieldSettings.field_id.substring(13) + '-add-more-wrapper').length == 1) {
    var toSwitch = $('#' + fieldSettings.field_id.substring(13) + '-add-more-wrapper');
  } else {
    var toSwitch = $(fieldSettings.field_id);
  }

  if (Drupal.settings.ConditionalFields.ui_settings == 'disable') {
    var disabled = '';
    if (showOrHide == 'hide') {
      disabled = '';
    }
    toSwitch.find('textarea, input, select').attr('disabled', disabled);
  }
  /* Avoid flickering */
  else if (onPageReady == true) {
    /* Setting css instead of simply hiding to avoid interference from collapse.js */
    showOrHide == 'show' ? toSwitch.show() : toSwitch.css('display', 'none');
  }
  else {
    switch (Drupal.settings.ConditionalFields.ui_settings.animation) {
      case 0:
        showOrHide == 'show' ? toSwitch.show() : toSwitch.hide();
      case 1:
        showOrHide == 'show' ? toSwitch.slideDown(Drupal.settings.ConditionalFields.ui_settings.anim_speed) :
                               toSwitch.slideUp(Drupal.settings.ConditionalFields.ui_settings.anim_speed);
      case 2:
        showOrHide == 'show' ? toSwitch.fadeIn(Drupal.settings.ConditionalFields.ui_settings.anim_speed) :
                               toSwitch.fadeOut(Drupal.settings.ConditionalFields.ui_settings.anim_speed);
    }
  }
}

Drupal.ConditionalFields.findValues = function(field) {
  var values = [];
  field.find('option:selected, input:checked').each( function() {
    if ($(this)[0].selected || $(this)[0].checked) {
      values[values.length] = this.value;
    }
  });
  return values;
}       

Drupal.ConditionalFields.fieldChange = function() {
  var values = Drupal.ConditionalFields.findValues($(this));
  var id = '#' + $(this).attr('id');
  Drupal.ConditionalFields.switchField(id, values, false);
}

Drupal.behaviors.ConditionalFields = function (context) {
  $('.node-form').find('.controlling-field:not(.conditional-field-processed)', context).addClass('conditional-field-processed').each(function () {
    /* Set default state */
    Drupal.ConditionalFields.switchField('#' + $(this).attr('id'), Drupal.ConditionalFields.findValues($(this)), true);
    /* Add events. Apparently, Explorer doesn't catch the change event? */
    $.browser.msie == true ? $(this).click(Drupal.ConditionalFields.fieldChange) : $(this).change(Drupal.ConditionalFields.fieldChange);
  });
};
