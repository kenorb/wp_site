/* $Id: avatar_selection.js,v 1.1.2.1.4.13 2008/06/30 20:06:00 snpower Exp $ */
function radio_button_handler() {
  // handle radio buttons
  $('div.user_avatar_select input.form-radio').hide();
  $('div.user_avatar_select img').hover(
    function(){
      $(this).addClass("avatar_hover");
    },
    function(){
      $(this).removeClass("avatar_hover");
    }
  );
}

function image_click_handler() {
  $('div.user_avatar_select img').bind("click", function(){
    $("div.user_avatar_select img.avatar_select").each(function(){
      $(this).removeClass("avatar_select");
      $(this).parent().children("input").attr("checked", "");
    });
    $(this).addClass("avatar_select");
    $(this).parent().children("input").attr("checked", "checked");
  });
}

if (Drupal.jsEnabled) {
  $(document).ready(function () {

    // handle radio buttons
    radio_button_handler();

    // handle image selection
    image_click_handler();
  });
}


