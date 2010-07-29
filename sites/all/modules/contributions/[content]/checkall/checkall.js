// $Id: checkall.js,v 1.2.2.5 2008/11/19 14:16:38 markuspetrux Exp $

Drupal.behaviors.checkAll = function(context) {
  $('.form-checkall:not(.checkall-processed)', context).each(function() { Drupal.checkAll(this); });
};

Drupal.checkAll = function(checkBoxes) {
  $(checkBoxes).addClass('checkall-processed')
    .prepend('<div class="checkall-action"></div>').children('.checkall-action')
    .prepend($('<a href="#">'+ Drupal.t('Uncheck all') +'</a>').click(function() { setCheckboxes(false); return false; }))
    .prepend($('<span> / </span>'))
    .prepend($('<a href="#">'+ Drupal.t('Check all') +'</a>').click(function() { setCheckboxes(true); return false; }));

  function setCheckboxes(state) {
    $('input:checkbox', checkBoxes).each(function() { this.checked = state; });
  }
};
