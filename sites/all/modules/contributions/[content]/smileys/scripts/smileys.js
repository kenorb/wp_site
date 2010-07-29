// $Id: smileys.js,v 1.1.2.3 2009/01/05 08:50:25 Gurpartap Exp $

/* Filename: smileys.js
 * jQuery Smileys Code for Drupal smileys module.
 * License: GPL (Read LICENSE.txt for more information).
 * Copyright, authors.
*/

// Add more textarea IDs that can parse Smileys.
var textareaIDs = {
  "edit-teaser-js": "",
  "edit-body": "",
  "edit-comment": ""
};

var textareaFocussed = "edit-body";

Drupal.behaviors.smileysAutoAttach = function() {
  if (Drupal.settings.smileys.enable_dialog) {
    $('.smileys-box .smileys').after('<span id="toggleSmileysDialog" class="smiley-class">' + Drupal.t('more...') + '</span>');
    var basePath = Drupal.settings.basePath;
    $('<div id="smileysDialogContent"></div>').appendTo("body").load(basePath + "smileys/fetch", function() {
      Drupal.smileysAttach();
      var options = {
        autoOpen: false,
        autoResize: true,
        buttons: {"Close window": function() { $(this).dialog("close"); }},
        closeOnEscape: true,
        dialogClass: "smileysDialog",
        draggable: Drupal.settings.smileys.draggable || false,
        height: Drupal.settings.smileys.dialog_height === "0" ? 'auto' : parseInt(Drupal.settings.smileys.dialog_height, 10),
        minWidth: 200, minHeight: 200,
        maxWidth: 650, maxHeight: 650,
        modal: false,
        overlay: {},
        position: "center",
        resizable: Drupal.settings.smileys.resizable ? "se" : false,
        stack: true,
        width: Drupal.settings.smileys.dialog_width === "0" ? 400 : parseInt(Drupal.settings.smileys.dialog_width, 10),
        zIndex: 1000
      };
      $(this).dialog(options);
      if (options.draggable && $.fn.draggable) {
        $(".smileysDialog .ui-dialog-buttonpane").css('cursor', 'move');
      }
      else {
        $(".smileysDialog .ui-dialog-buttonpane").css('background', 'none');
      }
      if (options.resizable && $.fn.resizable) {
        $(".smileysDialog .ui-resizable-se").removeAttr("style");
      }
      $(".smileysDialog .ui-dialog-titlebar").remove();
    }).css('max-height', 650);
    $("#toggleSmileysDialog").bind("click", function() {
      if (!$("#smileysDialogContent").dialog("isOpen")) {
        $("#smileysDialogContent").dialog("open");
        $('.smileysDialog button').blur();
      }
      else {
        $("#smileysDialogContent").dialog("close");
      }
    });
  }
  $("textarea").bind("focus", function() {
    var textAreaID = $(this).attr("id");
    if (textAreaID in textareaIDs) {
      textareaFocussed = textAreaID;
    }
  });
  Drupal.smileysAttach();
};

Drupal.smileysAttach = function() {
  $("img.smiley-class:not(.smileysProcessed)").addClass("smileysProcessed").bind("mouseover", function() {
    $('.ui-dialog-buttonpane').prepend('<span>' + $(this).attr('title') + '</span>');
  }).bind("mouseout", function() {
    $('.ui-dialog-buttonpane span').remove();
  }).bind("click", function() {
    var smiley = " " + this.alt + " ";
    // edit-body and edit-comment included to insert smiley into them when none of the textarea is focussed.
    $("textarea#"+ textareaFocussed +", textarea#edit-comment").each(function() {
      if (typeof tinyMCE !== "undefined" && (tinyMCE.getInstanceById("edit-comment") ? true : false || tinyMCE.getInstanceById("edit-body") ? true : false)) {
      // tinyMCE support
        tinyMCE.execCommand("mceInsertContent", false, smiley);
      }
      // If you have FCKeditor always enabled, you may uncomment the following code
      // For advanced information on issue see: http://drupal.org/node/213679
      /*else if (typeof FCKeditorAPI !== "undefined" && fckIsRunning[fckLaunchedJsId]) {
        // FCKeditor support
        FCKeditorAPI.GetInstance(fckLaunchedJsId).InsertHtml(smiley);
      }*/
      else {
        // Plain textarea support
        if (document.selection) {
          this.focus();
          document.selection.createRange().text = smiley;
        }
        else if (this.selectionStart || this.selectionStart === 0) {
          var cursorPos = this.selectionEnd + smiley.length;
          this.value = this.value.substring(0, this.selectionStart) + smiley + this.value.substring(this.selectionEnd);
          this.selectionStart = this.selectionEnd = cursorPos;
        }
        else {
          this.value = this.value + smiley;
        }
        this.focus();
      }
    });
  });
};
