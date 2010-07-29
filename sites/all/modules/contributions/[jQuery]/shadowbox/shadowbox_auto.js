// $Id: shadowbox_auto.js,v 1.1.4.2 2008/11/05 00:07:12 psynaptic Exp $
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var settings = Drupal.settings.shadowbox;
    if (settings.auto_enable_all_images == 1) {
      var images = 'a[href$=jpg][rel=""], a[href$=png][rel=""], a[href$=gif][rel=""], a[href$=jpeg][rel=""], a[href$=bmp][rel=""]';
      $(images).each(function() {
        $(this).attr('rel', 'shadowbox');
      });
    };
  });
}
