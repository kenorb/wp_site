// $Id: README.txt,v 1.1 2008/10/15 10:04:25 marktheunissen Exp $

Slider module creates a scrollable jQuery node display effect called the "coda slider", which was made famous by the site:

http://www.panic.com/coda/

Each panel of the Slider is an individual node. You can have any node you like appear inside a slider.

Usage
----------

This module depends on CCK (Content Creation Kit). A "slider" is actually a node type of your choice with a nodereference field called "field_slider_content". So before you can start using Slider, you need to install CCK module and choose a node type that will have the "field_slider_content" field. Add the field as a nodereference type, with "Multiple values" ticked so that you can have multiple panels in your slider!

In the Slider settings (admin/settings/slider), you should see a list of all node types that have a "field_slider_content" field. You can use the checkboxes to choose which node types the Slider module will operate on. Also select whether the tabs should appear above or below the panels.

Now, create a new node of the type you've selected, and set the nodereference field to reference the nodes you would like to be shown in the slider. Any nodes can be inside slider panels!

Don't forget that in the CCK settings for the node type, you have to configure the nodereference field and select which nodes you will allow it to reference.

When displaying the slider node, the nodereference data is removed from the display and instead you will see the nodes presented as a coda slider.

If you need to change the width and height or any other properties of the slider, you can do so in your theme's CSS file (overriding the styles in this module's css/slider.css).

Credits
-----------

This module was written by Mark Theunissen for Digital People Online, a London based Digital Agency that specialises in Drupal development.

It uses several jQuery plugins: scrollTo, localScroll, serialScroll.

The jQuery code in slider.js was created by Remy Sharp and is explained in detail on his blog at: http://jqueryfordesigners.com/coda-slider-effect/
