// $Id: README.txt,v 1.7 2008/07/06 01:29:46 jmiccolis Exp $

DESCRIPTION
-----------
Timeline is a DHTML-based AJAXy widget for visualizing temporal information.
It works in a similar way as Google Maps but for time-based events.

This Drupal module is based on a JavaScript widget developed by the MIT
SIMILE project. Several demo timelines demonstrating the full range of 
capabilities for this widget are available on the SIMILE project site at:
  http://code.google.com/p/simile-widgets/

The Views module is required to provide the list of nodes to render:
  http://drupal.org/project/views

This module provides two new view types for the Views module:
- Timeline Horizontal
- Timeline Vertical

In addition, two default views are provided to get you started quickly:
- timeline_nodes: plots any nodes by their creation dates.
- timeline_events: plots event nodes by their event start and end dates
  (requires the Event module).

OPERATION
---------
The module supports plotting nodes on a timeline view using any date/time
field. It uses the views fields configuration to determine which field to
use for plotting the node as an event on the timeline. If fields have been
specified for the view, they are used as follows:
- 1st: If present, used as the start time and end time for the timeline event. 
  Event nodes will use the start and end date of the event. 
  CCK date fields will use the from and to date of the field.
  If not present, the node's creation time is used instead.
- 2nd: If present, used as the title and clickable link for the timeline
  event. If not present, the node's default title is used instead.
- 3rd: If present, used as the description for the timeline event. If not
  present, the node's teaser is used (if available). NOTE: at present CCK
  nodes don't have a teaser by default, so you'll want to define the
  description field explicitly for them.
- Any other view fields are ignored by this module.

Timeline events with only a start date are displayed as points on the
timeline view, whilst events with both a start and date are displayed as
continuous colored blocks (not dissimilar to Gantt chart entries).

REQUIREMENTS
------------
Please refer to the accompanying INSTALL.txt file.

INSTALLATION
------------
Please refer to the accompanying INSTALL.txt file.

USAGE
-----
After installing the module, do the following to create a new timeline view:

1. Make sure the `views' and `views_ui' modules are installed and enabled.
2. Go to administer >> views >> add.
3. Fill out the parameters for your custom view:
   Under Page >> View Type (for page views) and/or
   under Block >> View Type (for block views),
     select the option "Timeline Horizontal".
4. Under Fields, add 1-4 fields as described in the Operation section above.
5. Set any other views settings, such as the maximum number of nodes to
   show, filters to select only certain nodes or dates, etc. You'll likely
   want to disable the view's pager option in order to get all events to
   show up on the timeline.
6. Save your view and navigate to it to test out the timeline. In case you
   don't see the timeline at all, that most likely indicates the view
   selection criteria didn't result in any nodes to be displayed.

CONFIGURATION
-------------
By default, the timeline widget is referenced and loaded from:
  sites/all/modules/timeline/api/timeline-api.js

You can override this location from the module's configuration. You will
need to download the widget's source code to the same host you are
running your Drupal instance on. Provided you have the Subversion command
line tools installed, the following is the easiest way to download the
widget:

  # svn export \
      http://simile.mit.edu/repository/timeline/trunk/src/webapp/api/ \
      modules/timeline/api

Alternatively you can download the widget code from:

  # http://www.miccolis.net/random/timeline-1-2.tar.gz

After downloading, you may need to set the Widget URL option in the module's
configuration. For example if your module doesn't live in the default location
of sites/all/modules.

KNOWN ISSUES
------------
- The timeline widget insists on claiming that dates are GMT, regardless of
  the actual specified timezone. This is an upstream issue in the widget
  itself; we work around it the best we can by claiming dates are GMT while
  they're in fact explicitly adjusted to the current user's timezone.
- If the view's selection criteria results in no nodes, the timeline widget
  isn't shown at all. We should show an empty timeline instead.
- View settings are only used for building the set of nodes/events to
  display on the timelines; the timeline user interface doesn't rely on view
  settings in any other way. Thus, e.g. exposed filter settings won't work
  with timeline views (#108886).

FUTURE WORK
-----------
- There is currently no way to override timeline display settings (such as
  width or height) on a view-specific basis. This is an important feature
  that needs to be added. Currently it is planned to implement this using
  view arguments, and in addition possibly provide a more user-friendly
  screen (administer >> timelines) for managing timelines without having to
  fiddle with a multitude of low-level Views module settings.
- The timeline widget has rudimentary support for locales. Setting a locale
  other than 'en', and automatically tying locale selection into Drupal's
  localization features, should be investigated.
- While we already support JSON and XML data feeds, the timeline widget now
  also has a SPARQL data feed loader which could provide interesting
  additional mashup capabilities.

TROUBLESHOOTING
---------------
To post bug reports or feature requests please use the following issue
trackers for the Drupal module and the MIT SIMILE widget, respectively:
- http://drupal.org/project/issues/timeline
- http://code.google.com/p/simile-widgets/issues/list
