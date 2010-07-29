$(function(){
	// setup each abuse report for inline/ajax moderation
	$('.abuse-report').each(function(){
		
		function onModerateContentComplete(response, target){
			$(target).slideUp('slow', function(){
				$(target).remove();
			});
		};
		
		function onModerateContentError(response, target) {
			// not implemented
		};
		
		var $report = $(this), report = this;
		var $form = $('form', $report), form = $form[0];
		var $actions = $('.actions', $report);
		
		$form.ajaxForm({
			url:	Drupal.base_url + 'admin/abuse/moderate/content/js',
			type:	'post',
			dataType:	'json',
			success:	function(response) {
				if(response.status) {
					(Drupal.theme.onModerateContentComplete || onModerateContentComplete)(response, report);
				} else {
					(Drupal.theme.onModerateContentError || onModerateContentError)(response, report);
				}
			}
		});
		
		$('.abuse-report-allow, .abuse-report-remove', this).each(function(){
			var $submit = $(':submit', this), submit = $submit[0];
			$('<a href="#" class="button"></a>')
			.text($submit.attr('value'))
			.insertBefore($form)
			.bind('click', function(event){
				form.clk = submit;
				$form.trigger('submit');
				return false;
			});
		});
		
		$('.abuse-report-warn-and-allow, .abuse-report-warn-and-remove', this).each(function(){
			var $element = $(this);
			
			$('<a href="#" class="button"></a>')
			.text($(':submit', this).attr('value'))
			.insertBefore($form)
			.bind('click', function(event){
				$(this).toggleClass('button-active');
				$('legend a', $element).trigger('click');
				return false;
			});
			
			$('.warning-templates', $element)
			.find('dt')
				.each(function(){
//					var subject = $(this).next('dd.subject').text();
					var body = $(this).next('dd').text();
					$('a', this).bind('click', function(){
//						$(':input[@id^="edit-allow-subject"]', $element).val(subject);
						$(':input[@id^="edit-allow-body"], :input[@id^="edit-remove-body"]', $element).val(body);
						return false;
					});
				})
			.end()
			.find('dd')
				.remove()
			.end();
			
		});
		
		$('.abuse-report-ban', this).each(function(){
			var message = $('.form-item', this).text();
			var $submit = $(':submit', this), submit = $submit[0];
			$('<a href="#" class="button"></a>')
			.text($submit.attr('value'))
			.insertBefore($form)
			.bind('click', function(event){
				if(confirm(message)) {
					form.clk = submit;
					$form.trigger('submit');
				}
				return false;
			});
		});
		
	});
});