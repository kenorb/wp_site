// $Id: shadowbox.js,v 1.1.2.3.2.2 2009/01/13 11:05:04 psynaptic Exp $
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var settings = Drupal.settings.shadowbox;
    var options = {
      animate:            settings.animation,
      animateFade:        settings.animation_fade,
      animSequence:       settings.animation_sequence,
      flvPlayer:          settings.flv_player,
      modal:              settings.modal,
      overlayColor:       '#' + settings.overlay_color,
      overlayOpacity:     settings.overlay_opacity,
      flashBgColor:       '#' + settings.flash_background,
      autoplayMovies:     settings.autoplay_movies,
      showMovieControls:  settings.show_movie_controls,
      slideshowDelay:     settings.slideshow_delay,
      resizeDuration:     settings.resize_duration,
      fadeDuration:       settings.fade_duration,
      displayNav:         settings.display_nav,
      continuous:         settings.continuous_galleries,
      displayCounter:     settings.display_counter,
      counterType:        settings.counter_type,
      counterLimit:       settings.counter_limit,
      viewportPadding:    settings.viewport_padding,
      handleOversize:     settings.handle_oversize,
      handleUnsupported:  settings.handle_unsupported,
      initialHeight:      settings.initial_height,
      initialWidth:       settings.initial_width,
      enableKeys:         settings.enable_keys,
      ext:     {
          img:        Drupal.settings.shadowbox.extensions_img,
          swf:        Drupal.settings.shadowbox.extensions_swf,
          flv:        Drupal.settings.shadowbox.extensions_flv,
          qt:         Drupal.settings.shadowbox.extensions_qt,
          wmp:        Drupal.settings.shadowbox.extensions_wmp,
          qtwmp:      Drupal.settings.shadowbox.extensions_qtwmp,
          iframe:     Drupal.settings.shadowbox.extensions_iframe
      }
    };
    Shadowbox.init(options);
  });
}
