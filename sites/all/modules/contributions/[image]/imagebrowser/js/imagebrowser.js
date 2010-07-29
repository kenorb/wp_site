// $Id: imagebrowser.js,v 1.1.2.7 2008/12/03 22:18:51 starnox Exp $

/**
 * @file imagebrowser.js
 *
 * All the javascript to make Image Browser work
 */

/**
 * FCKEditor insert function
 */
function SelectFile( fileUrl, width, height, alt )
{
  if(window.opener) {
    //url, width, height, alt
    window.opener.SetUrl( fileUrl, null, null, alt );
  }
  window.close();
}

/**
 * Prepare links in the browser for insert overlay
 */
function ib_prepareLinks() {
  //Add on click functions for images
  $("#browse .browse .view-content ul li a").click(function() {
    $("#fade").show();
    $("#fade").fadeTo('fast', 0.7);
    $("#insert").fadeIn('fast');
    $("#insert").html('<p class="loading"></p>');
    var href = $(this).attr("href");
    var rand = Math.random();
    $("#insert").load(href, function() {
      //Cancel button
      $(".close").click(function() {
        $("#fade").fadeOut('slow');
        $("#insert").fadeOut('slow');
        return false;
      });
      
      //Edit button
      $(".edit").click(function(){
        $("#edit").fadeIn('fast');
        var href = $(this).attr("href");
        $("#edit").html('<p class="loading"></p>');
        var href = $(this).attr("href");
        $("#edit").load(href, function() {
          var src = $("#edit > iframe").attr("src");
          $("#edit > iframe").attr("src", src);
          //Cancel button
          $(".close").click(function() {
            $(".view-filters > form").trigger('submit');
            $("#insert").fadeOut();
            $("#fade").fadeOut();
            $("#edit").fadeOut();
            return false;
          });
        });
        return false;
      });
      
      //Delete button
      $(".delete").click(function(){
        var node = $(this).attr("href");
        $("#insert > .details > .options a").fadeOut('slow');
        $("#insert > .details > table").slideUp('slow', function() {
          $("#insert > .details").append('<div class="confirm"><p>Are you sure you want to delete this image?</p><a href="#" class="button delete_confirm">Yes, Delete</a><a href="#" class="button cancel">No, Cancel</a></div>');
          $(".confirm").fadeIn('fast');
          
          $(".cancel").click(function() {
            $(".confirm").fadeOut('fast', function() {
              $("#insert > .details > .confirm").remove();
              $("#insert > .details > .options a").fadeIn('slow');
              $("#insert > .details > table").slideDown('slow');
            });
          });
          
          $(".delete_confirm").click(function() {
            $("#insert > .details").append('<div class="delete"></div>');
            $(".delete").load(node, function() {
              $(".view-filters > form").trigger('submit');
              $("#insert").fadeOut();
              $("#fade").fadeOut();
              ib_get_messages();
            });
          });
        });
        return false;
      });
    });
    return false;
  });

  //Add on click to apply filters button
  $("#edit-submit").click(function(){
    $("#fade").fadeOut("slow");
    $(".view-filters").fadeOut("slow");
  });
  
  //Format filter window
  ib_format_filters();
  
  //Button to close all windows
  $(".footer .apply").click(function(){
    $("#fade").fadeOut('slow');
    $("#messages").fadeOut('slow');
    $(".view-filters").fadeOut('slow');
  });
  $(".footer .close").click(function(){
    $("#fade").fadeOut('slow');
    $("#messages").fadeOut('slow');
    $(".view-filters").fadeOut('slow');
    return false;
  });
}

/**
 * Get Drupal messages
 */
function ib_get_messages() {
  var pre_num = $(".open-messages .num").text();
  var num = 0;
  $("#messages > .log").prepend('<li class="hidden"></li>');
  $("#messages > .log > li:first-child").load(ibpath, function() {
    if ( $("#messages > .log > li:first-child").children().length > 0) {
      $("#messages > .log > li:first-child").removeClass('hidden');
    }
    
    num = $("#messages > .log").children().not(".hidden").length;
    
    //Flash button
    if (parseInt(pre_num) < parseInt(num)) {
      $(".open-messages").animate({opacity: "0"}, 250, function() {
        $(".open-messages .num").text(num);
        $(".open-messages").animate({opacity: "1"}, 250);
      });
    }
  });
}

/**
 * Since the filters window is made by Views we can pretty it up via JS
 */
function ib_format_filters() {
  $(".views-exposed-widgets > div:last-child").appendTo("#views-exposed-form-ib-browser-default").addClass("footer");
  $("#views-exposed-form-ib-browser-default > div:first-child").addClass("wrapper");
  $("#views-exposed-form-ib-browser-default").prepend("<h2>Filters</h2>");
  $("#views-exposed-form-ib-browser-default > .footer")
  .append('<a href="#" class="button apply">Apply</a>').click(function(){
    $(".view-filters > form").trigger('submit');
  })
  .append('<a href="#" class="button close">Close</a>');
}

/**
 * Make things happen on page load
 */
$(document).ready(function(){
  ib_prepareLinks();

  //Button to open the Filter window
  $(".open-filters").click(function(){
    $("#fade").show();
    $("#fade").fadeTo('fast', 0.7);
    $(".view-filters").fadeIn('fast');
  });

  //Button to open the Messages window
  $(".open-messages").click(function(){
    ib_get_messages();
    $("#fade").show();
    $("#fade").fadeTo('fast', 0.7);
    $("#messages").fadeIn('fast');
  });
  
  //Delay display until JS has loaded
  $("#fade").fadeOut('slow', function() {
    $("#fade").css('background', '#000000');
  });
});