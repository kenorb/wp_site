// $Id: galleria.js,v 1.1.2.2 2008/12/16 11:45:07 marktheunissen Exp $

var options = {
  onImage : function(image, caption, thumb) {
    // let's add some image effects for demonstration purposes
    // fade in the image & caption
    if(!($.browser.mozilla && navigator.appVersion.indexOf("Win")!=-1) ) { // FF/Win fades large images terribly slow
      image.css('display','none').fadeIn(500);
    }
    caption.css('display','none').fadeIn(500);

    // fetch the thumbnail container
    var _li = thumb.parents('li');

    // fade out inactive thumbnail
    _li.siblings().children('img.selected').fadeTo(500,0.3);

    // fade in active thumbnail
    thumb.fadeTo('fast',1).addClass('selected');

    // add a title for the clickable image
    image.attr('title','Next image >>');
  },

  onThumb : function(thumb) {
    // thumbnail effects goes here
    // fetch the thumbnail container
    var _li = thumb.parents('li');

    // if thumbnail is active, fade all the way.
    var _fadeTo = _li.is('.active') ? '1' : '0.3';

    // fade in the thumbnail when finnished loading
    thumb.css({display:'none',opacity:_fadeTo}).fadeIn(1500);

    // hover effects
    thumb.hover(
      function() { thumb.fadeTo('fast',1); },
      function() { _li.not('.active').children('img').fadeTo('fast',0.3); } // don't fade out if the parent is active
    )
  },

  history : false
};

// run Galleria on main document load. Used for viewing a plain Galleria node
jQuery(function($) {
  $('ul.gallery').galleria(options);
});

