if (typeof(Drupal) == "undefined" || !Drupal.nicemap) {
  Drupal.nicemap = {};
}

Drupal.nicemap.iehover = function() {
  if ($('div.nicemap-map').size() > 0) {
    // Provide consistent hovering
    $('div.nicemap-map a.geopoint').hover(function(){
        $(this).find('span').show();
    }, function() {
        $(this).find('span').hide();
    });
  }
}

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    if ($.browser.msie) {
      Drupal.nicemap.iehover();
    }
    if ($('div.nicemap-map div.geoitem').size() > 0) {
    }

  });
};

