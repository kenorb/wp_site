// $Id: charts_color.js,v 1.2 2008/07/07 08:04:15 brmassa Exp $
/**
 * @author Bruno Massa http://drupal.org/user/67164
 * @file
 * Its the integration with Color module and Charts.
 * Using the Farbtastic JS, this file set the behavior of the
 */
Drupal.behaviors.charts_color = function (context) {
  /**
    * Function copied from Color module.
    *
    * Callback for Farbtastic when a new color is chosen.
    */
  callback = function (input, color, propagate, colorscheme) {
    // Set background/foreground color
    $(input).css({
      backgroundColor: color,
      'color': farb.RGBToHSL(farb.unpack(color))[2] > 0.5 ? '#000' : '#fff'
    });

    // Change input value
    if (input.value && input.value != color) {
      input.value = color;

      // Update locked values
      if (propagate) {
        var i = input.i;
        for (j = i + 1; ; ++j) {
          if (!locks[j - 1] || $(locks[j - 1]).is('.unlocked')) break;
          var matched = shift_color(color, reference[input.key], reference[inputs[j].key]);
          callback(inputs[j], matched, false);
        }
        for (j = i - 1; ; --j) {
          if (!locks[j] || $(locks[j]).is('.unlocked')) break;
          var matched = shift_color(color, reference[input.key], reference[inputs[j].key]);
          callback(inputs[j], matched, false);
        }

        // Update preview
        preview();
      }

      // Reset colorscheme selector
      if (!colorscheme) {
        resetScheme();
      }
    }
  }

  /**
    * Function copied from Color module.
    *
    * Focus the Farbtastic on a particular field.
    */
  focus = function () {
    var input = this;
    // Remove old bindings
    focused && $(focused).unbind('keyup', farb.updateValue)
        .unbind('keyup', preview).unbind('keyup', resetScheme)
        .parent().removeClass('item-selected');

    // Add new bindings
    focused = this;
    farb.linkTo(function (color) { callback(input, color, true, false); });
    farb.setColor(this.value);
    $(focused).keyup(farb.updateValue).keyup(preview).keyup(resetScheme)
      .parent().addClass('item-selected');
  }

  /**
  * Print a Chart (with generic data) preview.
  */
  preview = function () {
  }

  /**
    * Function copied from Color module.
    *
    * Reset the color scheme selector.
    */
  resetScheme = function () {
    $('#edit-scheme', form).each(function () {
      this.selectedIndex = this.options.length - 1;
    });
  }

  /**
    * Function copied from Color module
    *
    * Shift a given color, using a reference pair (ref in HSL).
    */
  shift_color = function (given, ref1, ref2) {
    // Convert to HSL
    given = farb.RGBToHSL(farb.unpack(given));

    // Hue: apply delta
    given[0] += ref2[0] - ref1[0];

    // Saturation: interpolate
    if (ref1[1] == 0 || ref2[1] == 0) {
      given[1] = ref2[1];
    }
    else {
      var d = ref1[1] / ref2[1];
      if (d > 1) {
        given[1] /= d;
      }
      else {
        given[1] = 1 - (1 - given[1]) * d;
      }
    }

    // Luminance: interpolate
    if (ref1[2] == 0 || ref2[2] == 0) {
      given[2] = ref2[2];
    }
    else {
      var d = ref1[2] / ref2[2];
      if (d > 1) {
        given[2] /= d;
      }
      else {
        given[2] = 1 - (1 - given[2]) * d;
      }
    }

    return farb.pack(farb.HSLToRGB(given));
  }

  // Declaring some variavles and settings some generic values
  var inputs = [];
  var hooks = [];
  var locks = [];
  var focused = null;

  // Determine what is the main charts color piece of
  var form = $('#charts_color', context);

  // Add Farbtastic
  var farb = $.farbtastic('#farbtastic');

  // Decode reference colors to HSL
  var reference = Drupal.settings.charts_color.reference;
  for (i in reference) {
    reference[i] = farb.RGBToHSL(farb.unpack(reference[i]));
  }

  // Set up colorscheme selector
  $('#edit-scheme', form).change(function () {
    var colors = this.options[this.selectedIndex].value;
    if (colors != '') {
      colors = colors.split(',');
      for (i in colors) {
        callback(inputs[i], colors[i], false, true);
      }
      preview();
    }
  });

  // Initialize color fields
  $('#palette input.form-text', form)
  .each(function () {
    // Extract palette field name
    this.key = this.id.substring(13);

    // Link to color picker temporarily to initialize.
    farb.linkTo(function () {}).setColor('#000').linkTo(this);

    // Add lock
    var i = inputs.length;
    if (inputs.length) {
      var lock = $('<div class="lock"></div>').toggle(
        function () {
          $(this).addClass('unlocked');
          $(hooks[i - 1]).attr('class',
            locks[i - 2] && $(locks[i - 2]).is(':not(.unlocked)') ? 'hook up' : 'hook'
          );
          $(hooks[i]).attr('class',
            locks[i] && $(locks[i]).is(':not(.unlocked)') ? 'hook down' : 'hook'
          );
        },
        function () {
          $(this).removeClass('unlocked');
          $(hooks[i - 1]).attr('class',
            locks[i - 2] && $(locks[i - 2]).is(':not(.unlocked)') ? 'hook both' : 'hook down'
          );
          $(hooks[i]).attr('class',
            locks[i] && $(locks[i]).is(':not(.unlocked)') ? 'hook both' : 'hook up'
          );
        }
      );
      $(this).after(lock);
      locks.push(lock);
    };

    // Add hook
    var hook = $('<div class="hook"></div>');
    $(this).after(hook);
    hooks.push(hook);

    $(this).parent().find('.lock').click();
    this.i = i;
    inputs.push(this);
  })
  .focus(focus);

  $('#palette label', form);

  // Focus first color
  focus.call(inputs[0]);

  // Render preview
  preview();
};
