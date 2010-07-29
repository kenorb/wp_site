// On initial load, add dragable to all li dragable items
$(document).ready(function () {
  // Prevent duplicate attachment of the collapsible behavior.
  $('.autotag-dragable').draggable({
    helper: 'clone', 
    zIndex: 150,
    appendTo: '#autotag-container',
  });
  $('#autotag-dnd-right').droppable({
    accept: ".autotag-dragable",
    activeClass: 'autotag-dnd-drop-active',
    hoverClass: 'autotag-dnd-drop-hover',
    drop: function(ev, ui) {
      ui.draggable.appendTo("#autotag-dnd-right>ul");
    }
  });
  $('#autotag-dnd-left').droppable({
    accept: ".autotag-dragable",
    activeClass: 'autotag-dnd-drop-active',
    hoverClass: 'autotag-dnd-drop-hover',
    drop: function(ev, ui) {
      ui.draggable.appendTo("#autotag-dnd-left>ul");
    }
  });
  $('#autotag-dnd-right>*').droppable({
    accept: ".autotag-dragable",
    activeClass: 'autotag-dnd-drop-active',
    hoverClass: 'autotag-dnd-drop-hover',
    drop: function(ev, ui) {
      ui.draggable.appendTo("#autotag-dnd-right>ul");
    }
  });
  $('#autotag-dnd-left>*').droppable({
    accept: ".autotag-dragable",
    activeClass: 'autotag-dnd-drop-active',
    hoverClass: 'autotag-dnd-drop-hover',
    drop: function(ev, ui) {
      ui.draggable.appendTo("#autotag-dnd-left>ul");
    }
  });
  $('.autotag-dragable').droppable({
    accept: ".autotag-dragable",
    drop: function(ev, ui) {
      ui.draggable.appendTo($(this).parent());
    }
  });
});
function autotag_update_form(){
  var hidden_tids = '';
  $.each($('#autotag-dnd-right>ul>*'), function(){
    hidden_tids += '|'+$(this).attr('tid');
  });
  $('#edit-hideterms').val(hidden_tids);
}