if (Drupal.jsEnabled) {
  $(document).ready(function() {
  	$('form .form-item input.form-text + .description').prev('input').bt({
	    trigger: ['focus', 'blur'],
	    positions: ['bottom']
	  });
	  $('form .form-item input + .description').hide();
	//$('form .form-item input + .description').hide();
  });
}