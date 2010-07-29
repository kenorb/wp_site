// $Id: views.js,v 1.7 2008/08/05 21:55:59 wimleers Exp $


/**
 * @file
 * Make Hierarchical Select work in Views' exposed filters form.
 *
 * Views' exposed filters form is a GET form, but since Hierarchical Select
 * really is a combination of various form items, this will result in a very
 * ugly and unnecessarily long GET URL, which also breaks the exposed filters.
 * This piece of JavaScript is a necessity to make it work again, but it will
 * of course only work when JavaScript is enabled!
 */


if (Drupal.jsEnabled) {
  $(document).ready(function(){
    $('form#views-filters, form#views-filterblock').submit(function() {
      // Remove the Hierarchical Select form build id and the form id, to
      // prevent them from ending up in the GET URL.
      $('#edit-hs-form-build-id, #edit-views-filters, #edit-views-filterblock').remove();

      // Prepare the hierarchical select form elements that are used as
      // exposed filters for a GET submit.
      $('form#views-filters, form#views-filterblock')
      .find('.hierarchical-select-wrapper')
      .trigger('prepare-GET-submit');
    });
  });
}
