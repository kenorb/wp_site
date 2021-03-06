// $Id: README.txt,v 1.2.2.2 2008/12/03 21:20:26 alexb Exp $

ABOUT
-------------------

Nice Map is a WMS[1] client for Drupal. Using this module you can display
content on a map generated by a WMS server like mapserver[2] or mapnik[3].

This module doesn't require the views module but is most easily used as a views
style plugin.

INSTALL
-------------------

Install this module in the usual manner; put it in the modules directory and
enable it at admin/build/modules. Once enabled goto admin/settings/nicemap, to
use a WMS server select "WMS server" from the drop down and hit submit. Then
enter the URL for the WMS server. Some public ones are listed in the "WMS
Servers" section of this document. Hit submit again. If your site was able to
connect to the WMS server you'll be presented with a list of layers the server
offers.

VIEWS AND CCK
-------------------

Simple Views and CCK configuration for showing nodes on a map. This howto 
assumes knowledge of Views and CCK configuration.

CCK

1) Create a content type 
2) Add two decimal fields to it, one for latitude, one for longitude 
  (Default precision 10 and scale 2 are fine)

Views

1) Add a new view (View type node)
2) Add a page display
3) Select Nice map as style
4) Add at title, latitude and longitude as Fields
5) Click on gear next to style: Nice map and configure "Field usage" - 
   choose which node fields Nice map should use as title, lat and lon.
6) Save the view and go to its URL

WMS SERVERS
-------------------

The following WMS servers are publicly accessible:

* http://wms.jpl.nasa.gov/wms.cgi

DEVELOPERS 
-------------------

Support from this module came from many organizations including, Managing News
and The New America Foundation.

Young Hahn (yhahn)
Tom MacWright (tmcw)
Jeff Miccolis (jmiccolis)

[1] http://en.wikipedia.org/wiki/Web_Map_Service 
[2] http://mapserver.gis.umn.edu/ 
[3] http://www.mapnik.org/