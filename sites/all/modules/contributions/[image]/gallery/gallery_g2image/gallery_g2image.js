// $Id: gallery_g2image.js,v 1.1.2.1 2008/06/10 12:56:35 profix898 Exp $

function g2image_open(field) {
  // Set some properties
  var form = $('#'+field).parents('form').attr('id');
  var url = Drupal.settings.gallery.g2image_uri+'/g2image.php?g2ic_form='+form+'&g2ic_field='+field+'&g2ic_tinymce=0';
	var name = 'g2image';
	var w = 600;
	var h = 600;
	var valLeft = (screen.width) ? (screen.width-w)/2 : 0;
	var valTop = (screen.height) ? (screen.height-h)/2 : 0;
	var features = 'width='+w+',height='+h+',left='+valLeft+',top='+valTop+',resizable=1,scrollbars=1';

	// Open the G2Image window
	window.open(url, name, features);
}

// BUEditor
function g2bueditor_open() {
  if (editor.active) {
    g2image_open(editor.active.textArea.id);
  }
}
