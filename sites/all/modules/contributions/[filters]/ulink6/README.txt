********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: uLink module
Author: Gartheeban Ganeshapillai 
        AKA garthee at drupal <garthee at gmail dot com>
Dependencies: filter.module, comment.module(optional),
              php.module(optional), tokens.module(optional)
Required by: uAuto.module, ulink_gallery.module, ulink_image.module, etc

********************************************************************
IMPORTANT

Users are encouraged to read at least Default implementation
section {7}  of this guide before using the module.

********************************************************************
DESCRIPTION:

The uLink module provides universal filtering {1} which allows you to 
input tags like [l|link_to_node|text_to_appear|attributes] and replace
them with appropriate html code {2}, user defined text or code, or
third party rendered text {3} or code to provide links, display images,
fill with text or all.

This module defines the protocol and giving the framework to generate
the outputs, which can be configured or extended through the settings page {4}
and by implementing the hooks [1]. Hence, More than one module may be giving
the rendering, in case, they can optionally be enabled, cascaded or
optionally added {5}.

The module comes with basic, but powerful default implementation {6}
and two other supporting modules ulink_gallery  - which provides
image gallery like feature and ulink_image - which provides
rendering for images. They also give good demonstration on how uLink
could be extended to match different filtering needs.

********************************************************************
DETAILED DESCRIPTION:

{1} universal filtering 
--------------------------------------------------------------------
The main objective of the module is to provide universal filtering 
which is independent of the objectives of the user i.e. expected output 
text or code, limitations and expectations of the site administrator 
and target (of the link) type. However filtering is dependent on the 
tag ie  [l|link_to_node|text_to_appear|attributes] though.

Through the powerful and complex settings {4} available under
administrative settings rendering can be configured individually for
different target types {7}. Alternatively, specific rendering can be
given by implementing hooks. Please refer to the developer documentation
for details.

{2} HTML code
--------------------------------------------------------------------
[l|link_to_node|text_to_appear|attributes] will be replaced by module
implementation’s output. This eliminates having to type html tags and
allows uniform way of entering links and determines the output target
orientedly at realtime.

{3} Third party rendered code
--------------------------------------------------------------------
uLink module provides the skeleton for other modules to implement and
extend the functions. Other modules can implement the hooks [1] and
provide different outputs. ulink_gallery and ulink_image are
two perfect examples.

{4} settings page 
--------------------------------------------------------------------
Available under 
Admin > Settings > ulink > 
(admin/settings/ulink/files)

Settings page allows user to choose the modules implementing
hooks ie enable them and modify the order of cascading the output {5}
individually for all the types {7} supported.  Through these setting pages,
other modules implementing the modules will display module specific
settings. For example ulink_gallery provides settings related to its
implementation in this page. 

{5} Multiple implementations
--------------------------------------------------------------------
There may be more than one module implementing, a particular type.
For example, with the default distribution users have ulink,
ulink_gallery and ulink_imaeg providing rendering for intern type.
In the settings page there are three options (Disabled, Cascade the
module's rendering to the output, Cascade only if the previous output 
is empty) and weight.

  + Disabled: Don’t use this modules rendering
  + Cascade the module's rendering to the output: Cascade the outputs
    in the order given by the weight. For example if both ulink and
    ulink_gallery are selected, with this option where ulink is with
    weight = -2 and ulink_gallery is with	weight = 3, output will
    be of ulink’s output followed by ulink_gallery’s output.
  + Cascade only if the previous output is empty: This output will
    be used only when	all other modules with higher priority as
    determined by the weight options return	null. For example,
    if ulink (weight = -2) is selected with option2 and ulink_gallery (weight = 3)
    is selected with option3 ulink_gallery’s output	will be selected
    only if ulink’s output is null.
		
As explained above, multiple renderings can be enabled, cascaded and optionally added.

{6} Default implementation
--------------------------------------------------------------------
Although this module is meant to provide only the framework for universal
filtering, it provides a powerful default implementation as follows.

It gives three different types of rendering for all four modes available
namely NODE, USER, INTERN, EXTERN

1.	Hardcoded – user can’t modify the html output, however he can
    choose what should come for the text when user fails to enter
    anything for text	(eg [l|node/2]) through configure – option. 
2.	Macro substitution (Tokens) – When token module is available
    uLink is making use of it, by allowing users to use tokens for
    the following. Here user can use tokens to insert additional 
    information like node-title, user-name. Detailed help on available
    token is available in settings page.
    a.	Use this when text is empty: the text part to be added when
        user fails to enter anything for text. 
		b.	Token tag to be rendered: 
        The output; together with tokens and html code user have 
        the full control over the output.
		c.	Token tag to be rendered if the object is not available:
        The output when the target is local (node, user, intern)
        and it does not exist ie broken link;
        together with tokens and html code user have full control
        over the output.	
3.	PHP code – user has the fullest control over the output. Detailed
    help on available php variables  is available in settings page.
		User returns the output using return tag. 
		Eg: return l($text." ( ".$object->subject." )", $path, $link["attributes"]);

In addition, users can use macros to avoid typing similar attributes repeatedly. 
Macros can be added under general settings

Admin > Settings > ulink > General settings
(admin/settings/ulink)

Here macros are entered as a list of comma delimted pairs in the following format
  "name_of_macroA" => "attribute1=value,attribute2=value",
  "name_of_macroB" => "attribute3=value,attribute4=value",...
	
For example,
"imagelist"=>"class=imagelist", "photos"=>"width=200,height=100"

Then in the tag it can be called easily by specifying the macro name in the following format
[l-name_of_macro1|link_to_node|text_to_appear|additional_attributes]

For example,
[l-photos|files/image.png|my wedding photo]
is equivalent to
[l|files/image.png|my wedding photo|width=200,height=100]

{7} Types supported
--------------------------------------------------------------------
The following target types are supported:
node, user, intern (local files whether listed in files table or not),
extern (referring to outside assets or interally referred assets - images or files but
cannot be located on disk). 

External References
--------------------------------------------------------------------
[1] Implementing uLink hooks 
Developer_documentation_ulink.txt


********************************************************************
SYSTEM REQUIREMENTS

Drupal 5.x 
PHP 5.0.0 or greater
Filter.module installed

********************************************************************
USAGE:

[l|link]  or
[l|link|text]  or
[l|link|text|attributes]  or
[l|link||attributes]  or
[l-macro|link|text]  or
[l-macro|link]  or


Link can be specified in three ways
1. External links - by using complete absolute url
   eg: http://www.drupal.org/project/ulink
2. Internal drupal supported links or files - by using drupal
   recognizable url
	 eg: node/12, node/3#comment-1, user/2, files/drupal.png
3. Interal non drupal links - by starting the /
   eg: /files/drupal.png, /sites/default/temp.txt
	 
If internal drupal link is specified and, link is not found
under default implementation tags specified broken links will
be rendered.

Attributes are additional parameters to modify the rendering,
for example, image type can be given class=image or width=120.
eg: [l|/files/myphoto.jpg|my photo|width=200,height=300]
It can also contain information to rendering modules
+ forcelink=true, is used to link the image instead of displaying it.
+ filter=images, is used to set filter in ulink_gallery
+ filter=image_extension_in_regular_expressions, is used to add parse extensions
in ulink_gallery


**refer to the modules' README.txt for module specific attributes.


*********
EXAMPLES:

when ulink_image is enabled for image type and for other types ulink's
default implementation is used.

[l|files/CCNA.rtf|text]
[l|files/CCNA.rtf|text|title=title]
[l|http://localhost/drupal/files/The_10_90_Rule.pdf|external file|title=hello]

[l|node/6]
[l|node/6|text|title=title]
[l|node/60|text]
[l|story/node|text]

[l|node/4#comment-2|text]
[l|node/4#comment-2]

[l|files/collage.jpg]
[l|files/02.jpg|text]
[l|files/02.jpg|text|title=title]
[l|files/02.jpg|text|width=400]


********************************************************************
CREDITS:

 - Basic ideas were obtained from inline.module, token.module, reptag.module and image.module
 - Thanks to Google for sponsoring this project through GSOC 2007
 - Thanks to Drupal community, Daniel DeGeest and Kaustubh Srikanth for seleting
		this module for GSOC
 - Thanks to  Daniel DeGeest AKA dldege at drupal, for providing an excellent support,
	 suggestion, ideas and mentoring the project. He was there for the whole 
	 duration of the project for the assistance. Also initially
	 ulink_imagecache(yet to be ported to D6) and ulink_gallery were written by him.

********************************************************************
BUGS AND SUGGESTIONS

Please report all bug reports at:
http://drupal.org/project/issues/ulink

Contact me at:
garthee at theebgar dot net

Demos at:
sorry currently not available for this version
test at 
http://www.project.theebgar.net/ulink/demo
on version 5.x 2.0

http://mwt.argz.com/ulink/tests



