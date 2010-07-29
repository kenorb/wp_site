// $Id: rotor.js,v 1.1.2.4.2.2 2009/01/06 09:37:29 mrfelton Exp $ 

(function($) {

Drupal.RotorBanner = {};

Drupal.RotorBanner.initialize = function() {
  //if(typeof Drupal.settings.RotorBanner.enabled != 'undefined' && Drupal.settings.RotorBanner.enabled) {
    Drupal.RotorBanner.animate();
  //}
  
};

Drupal.RotorBanner.animate = function() {
	// redefine Cycle's updateActivePagerLink function 
	$.fn.cycle.updateActivePagerLink = function(pager, currSlideIndex){
		$(pager).find('.rotor-tab').removeClass('selected')
		.filter('.rotor-tab:eq(' + currSlideIndex + ')').addClass('selected');
	};
	
  for (rotor_item in Drupal.settings.RotorBanner) {
    var settings = Drupal.settings.RotorBanner[rotor_item];
  	$('.view-id-'+ settings.id +' .rotor-items').cycle({
  		timeout: settings.time * 1000,
  		speed: settings.speed,
  		fx: settings.effect,
  		pause: settings.pause,
  		pager: '.view-id-'+ settings.id +' .rotor-tabs',
  		pagerAnchorBuilder: function(idx, slide){
  			return '.view-id-'+ settings.id + ' .rotor-tabs .rotor-tab:eq(' + idx + ')'; 
  		}
  	}); 
  }
};

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    Drupal.RotorBanner.initialize();
  });
}

})(jQuery);
