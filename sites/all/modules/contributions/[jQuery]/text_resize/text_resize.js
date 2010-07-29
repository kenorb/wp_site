// $Id $

if (Drupal.jsEnabled) {
  $(document).ready(function(){
    // Config variables
    var largest_size_allowed = text_resize_maximum; // Based on admin form variable
    var smallest_size_allowed = text_resize_minimum; // Based on admin form variable
		var line_height_allow = text_resize_line_height_allow; // Based on admin form variable
		var line_height_min = text_resize_line_height_min; // Based on admin form variable
		var line_height_max = text_resize_line_height_max; // Based on admin form variable
    // Which div or page element are we resizing?
    if ($('DIV.left-corner').length > 0) {
      var element_to_resize = $('DIV.left-corner'); // Main body div for Garland
    } else if ($('#content-inner').length > 0) {
      var element_to_resize = $('#content-inner'); // Main body div for Zen-based themes
    } else if ($('#squeeze > #content').length > 0) {
      var element_to_resize = $('#squeeze > #content'); // Main body div for Zen Classic
    } else if ($('#'+text_resize_scope).length > 0) {
      var element_to_resize = $('#'+text_resize_scope); // ID specified by admin
    } else if ($('.'+text_resize_scope).length > 0) {
      var element_to_resize = $('.'+text_resize_scope); // CLASS specified by admin
    } else {
      var element_to_resize = $(text_resize_scope); // It's just a tag specified by admin
    }
    // Set the initial font size if necessary
    if ($.cookie('text_resize') != null) {
      element_to_resize.css('font-size', parseFloat($.cookie('text_resize')) + 'px');
      //alert( "Should be size: " + $.cookie('text_resize'));
    } else {
      //alert('Couldn\'t find text_resize cookie.');
    }
    if (line_height_allow) {
      //alert('line height adjustment allowed! The current line-height is '+parseFloat(element_to_resize.css('line-height'), 10));
			// Set the initial line height if necessary
      if ($.cookie('text_resize_line_height') != null) {
        element_to_resize.css('line-height', parseFloat($.cookie('text_resize_line_height')) + 'px');
      }
		}
    // Changer links will change the text size when clicked
    $('a.changer').click(function(){
      // Set the current font size of the specified section as a variable
      var currentFontSize = parseFloat(element_to_resize.css('font-size'), 10);
			//alert('currentFontSize = '+currentFontSize);
			// Set the current line-height
			var current_line_height = parseFloat(element_to_resize.css('line-height'), 10);
			//alert('current_line_height = '+current_line_height);
      // javascript lets us choose which link was clicked, by ID
      if (this.id == 'text_resize_increase'){
        var new_font_size = currentFontSize * 1.2;
        if (line_height_allow) { var new_line_height = current_line_height * 1.2; }
        // Allow resizing as long as font size doesn't go above largest_size_allowed.
        if (new_font_size <= largest_size_allowed) {
          $.cookie('text_resize', new_font_size, { path: '/' });
          if (line_height_allow) { $.cookie('text_resize_line_height', new_line_height, { path: '/' }); }
          var allow_change = true;
        } else {
					$.cookie('text_resize', largest_size_allowed, { path: '/' });
          if (line_height_allow) { $.cookie('text_resize_line_height', line_height_max, { path: '/' }); }
          var reset_size_max = true;
				}
      } else if (this.id == 'text_resize_decrease'){
        var new_font_size = currentFontSize * 0.8;
        if (line_height_allow) { var new_line_height = current_line_height * 0.8; }
        if (new_font_size >= smallest_size_allowed) {
          // Allow resizing as long as font size doesn't go below smallest_size_allowed.
          $.cookie('text_resize', new_font_size, { path: '/' });
          if (line_height_allow) { $.cookie('text_resize_line_height', new_line_height, { path: '/' }); }
          var allow_change = true;
        } else {
          // If it goes below smallest_size_allowed, just leave it at smallest_size_allowed.
          $.cookie('text_resize', smallest_size_allowed, { path: '/' });
					if (line_height_allow) { $.cookie('text_resize_line_height', line_height_min, { path: '/' }); }
          var reset_size_min = true;
        }
      }
      // jQuery lets us set the font Size value of the main text div
      if (allow_change == true) {
        element_to_resize.css('font-size', new_font_size + 'px'); // Add 'px' onto the end, otherwise ems are used as units by default
				if (line_height_allow) { element_to_resize.css('line-height', new_line_height + 'px'); }
        return false;
      } else if (reset_size_min == true) {
        element_to_resize.css('font-size', smallest_size_allowed + 'px');
				if (line_height_allow) { element_to_resize.css('line-height', line_height_min + 'px'); }
        return false;
      } else if (reset_size_max == true) {
        element_to_resize.css('font-size', largest_size_allowed + 'px');
				if (line_height_allow) { element_to_resize.css('line-height', line_height_max + 'px'); }
        return false;
      }
    });
  });
}