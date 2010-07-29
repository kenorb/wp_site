// $Id$

/*
 *  Bind the colorpicker event to the form element
 */
$(document).ready(function () {
  colorpicker_init();
});

function colorpicker_init() {
  // do we have multiple colorpickers?
  if ($("div.colorpicker").size() > 1) {
  
    // loop over each colorpicker type
    $("div.colorpicker").each(function() {

      // create the farbtastic colorpicker
      var farb = $.farbtastic(this);
    
      // get the id of the current matched colorpicker wrapper div
      var id = $(this).attr("id");

      // get the colorpicker_textfields associated with this colorpicker
      $("input.colorpicker_textfield").filter("." + id).each(function () {
        // set the background colors of all of the textfields appropriately
        farb.linkTo(this);
    
        // when clicked, they get linked to the farbtastic colorpicker that they are associated with
        $(this).click(function () {
          farb.linkTo(this);
        });
      });
    });
  }
  else {
    // it is possible to have 0 colorpickers so we need to make sure at least 1 exists
    if ($("div.colorpicker").size() == 1) {
      // we do this differently because we don't care about the id
      var farb = $.farbtastic("div.colorpicker");
      $("input.colorpicker_textfield").each(function () {
        // set the background colors of all of the textfields appropriately
        farb.linkTo(this);

        // update the farbtastic colorpicker when this textfield is clicked
        $(this).click(function () {
          farb.linkTo(this);
        });
      });
    }
  }
};

