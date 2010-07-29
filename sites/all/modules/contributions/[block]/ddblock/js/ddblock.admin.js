// $Id: ddblock.admin.js,v 1.2 2009/01/04 18:36:27 ppblaauw Exp $

/**
 * Show/hide advanced settings sections on the ddblock instance settings page.
 * don't change
 */
Drupal.behaviors.ddblockShowHideAdvancedOptions = function(context) {
  // Show/hide slide advanced options depending on the checkbox.
  $('#ddblock-instance-settings #edit-advanced:not(.ddblock-show-hide-advanced-options-processed)', context)
  .addClass('ddblock-show-hide-advanced-options-processed')
  .bind("change", function() {
    if (this.checked) {
      $("#ddblock-advanced-settings-wrapper").show();
      $("#ddblock-pager-position-wrapper").show();
      $("#ddblock-pager-event-wrapper").show();
      $("#edit-pager-height-wrapper").hide();
      $("#edit-pager-width-wrapper").hide();
      $("#ddblock-advanced-content-container-overflow-wrapper").hide();
      $("#ddblock-advanced-content-container-height-wrapper").hide();
      $("#ddblock-advanced-content-container-width-wrapper").hide();
      $("#ddblock-image-settings-wrapper").hide();
      $("#pager-dimensions-wrapper").hide();
      $("#edit-template-wrapper").show();
      $("#edit-custom-template-wrapper").show();
      $('#edit-pager-wrapper option[value="custom-pager"]').show();
      $('#edit-output-wrapper option[value="view_fields"]').show();
      $('#edit-output-wrapper option[value="view_content"]').hide();
    }
    else {
      $("#ddblock-advanced-settings-wrapper").hide();
      $("#ddblock-pager-position-wrapper").hide();
      $("#ddblock-pager-event-wrapper").hide();
      $("#edit-pager-height-wrapper").show();
      $("#edit-pager-width-wrapper").show();
      $("#ddblock-advanced-content-container-overflow-wrapper").show();
      $("#ddblock-advanced-content-container-height-wrapper").show();
      $("#ddblock-advanced-content-container-width-wrapper").show();
      $("#ddblock-image-settings-wrapper").show();
      $("#pager-dimensions-wrapper").show();
      $("#edit-template-wrapper").hide();
      $("#edit-custom-template-wrapper").hide();
      $('#edit-pager-wrapper option[value="custom-pager"]').hide();
      $('#edit-pager-wrapper #edit-pager').val('number-pager');
      $('#edit-output-wrapper option[value="view_fields"]').hide();
      $('#edit-output-wrapper option[value="view_content"]').show();
    }
    return false;
  }).trigger('change').trigger('change')
};
/**
 * Show/hide slide text settings sections on the ddblock instance settings page.
 * don't change
 */
Drupal.behaviors.ddblockShowHideSlideTextOptions = function(context) {
  // Show/hide slide text options depending on the checkbox.
  $('#ddblock-instance-settings #edit-slide-text:not(.ddblock-show-hide-text-options-processed)', context)
  .addClass('ddblock-show-hide-text-options-processed')
  .bind("change", function() {
    if (this.checked) {
      $("#ddblock-slide-text-settings-wrapper").show();
    }
    else {
      $("#ddblock-slide-text-settings-wrapper").hide();
    }
    return false;
  }).trigger('change').trigger('change')
};
/**
 * Show/hide pager settings sections on the ddblock instance settings page.
 * don't change
 */
Drupal.behaviors.ddblockShowHidePagerOptions = function(context) {
  // Show/hide pager options depending on the checkbox.
  $('#ddblock-instance-settings #edit-pager-toggle:not(.ddblock-show-hide-pager-options-processed)', context)
  .addClass('ddblock-show-hide-pager-options-processed')
  .bind("change", function() {
    if (this.checked) {
      $("#ddblock-pager-settings-wrapper").show();
    }
    else {
      $("#ddblock-pager-settings-wrapper").hide();
    }
    return false;
  }).trigger('change').trigger('change')
  // Show/hide pager options depending on the checkbox.
  $('#ddblock-block-settings #edit-pager-toggle:not(.ddblock-show-hide-pager-options-processed)', context)
  .addClass('ddblock-show-hide-pager-options-processed')
  .bind("change", function() {
    if (this.checked) {
      $("#ddblock-pager-settings-wrapper").show();
    }
    else {
      $("#ddblock-pager-settings-wrapper").hide();
    }
    return false;
  }).trigger('change').trigger('change')
};

/**
 * Show/hide custom template settings sections on the ddblock instance settings page.
 * don't change
 */
Drupal.behaviors.ddblockShowHideCustomTemplateOptions = function(context) {
  // Show/hide imagefolder/contenttype options depending on the select.
  $('#ddblock-instance-settings #edit-template:not(.ddblock-show-hide-custom-template-options-processed)', context)
  .addClass('ddblock-show-hide-custom-template-options-processed')
  .bind("change", function() {
    val = $('#ddblock-instance-settings #edit-template').val();
    //alert(val);
    switch (val) {
    case "custom" :
      $("#ddblock-custom-template-settings-wrapper").show();    
    case "upright10" :
      $('#edit-pager-wrapper #edit-pager').val('number-pager');
      $('#edit-pager-wrapper option[value="number-pager"]').show();
      $('#edit-pager-wrapper option[value="prev-next-pager"]').hide();
      $('#edit-pager-wrapper option[value="custom-pager"]').hide();
    break;
    case "upright20" :
      $('#edit-pager-wrapper #edit-pager').val('prev-next-pager');
      $('#edit-pager-wrapper option[value="number-pager"]').hide();
      $('#edit-pager-wrapper option[value="prev-next-pager"]').show();
      $('#edit-pager-wrapper option[value="custom-pager"]').hide();
    break;
    case "upright30" :
    case "upright40" :
    case "upright50" :
      $('#edit-pager-wrapper #edit-pager').val('custom-pager');
      $('#edit-pager-wrapper option[value="number-pager"]').hide();
      $('#edit-pager-wrapper option[value="prev-next-pager"]').hide();
      $('#edit-pager-wrapper option[value="custom-pager"]').show();
    break;
    }
    if (val.match("upright") == "upright") {
      $("#ddblock-pager-position-wrapper").hide();
      $("#ddblock-custom-template-settings-wrapper").hide();
    }
    else {
      $("#ddblock-pager-position-wrapper").show();
      $('#edit-pager-wrapper option[value="number-pager"]').show();
      $('#edit-pager-wrapper option[value="prev-next-pager"]').show();
      $('#edit-pager-wrapper option[value="custom-pager"]').show();
      $('#edit-pager-wrapper #edit-pager').val('custom-pager');
    }    
    return false;
  }).trigger('change')
};

/**
 * Show/hide imagefolder/contenttype settings sections on the ddblock settings page.
 * don't change
 */
Drupal.behaviors.ddblockShowHideContentOptions = function(context) {
  // Show/hide imagefolder/contenttype options depending on the select.
  $('#ddblock-content-settings #edit-input-type:not(.ddblock-show-hide-content-options-processed)', context)
  .addClass('ddblock-show-hide-content-options-processed')
  .bind("change", function() {
    if ($('#ddblock-content-settings #edit-input-type').val() == 'images') {
      $("#ddblock-image-folder-settings-wrapper").show();
      $("#ddblock-content-types-settings-wrapper").hide();
      $('#edit-pager-wrapper option[value="image-pager"]').show();
    }
    else {
      $("#ddblock-image-folder-settings-wrapper").hide();
      $("#ddblock-content-types-settings-wrapper").show();
      $('#edit-pager-wrapper option[value="image-pager"]').hide();
      // set pager to number-pager if not an image folder selected
      $('#edit-pager-wrapper #edit-pager').val('number-pager');
    }
    return false;
  }).trigger('change')
};
