// $Id: README.txt,v 1.2.2.3 2008/12/16 12:24:36 marktheunissen Exp $

Galleria
---------------------

This module allows users to create image galleries using the Galleria jQuery plugin, found on the site:

http://devkick.com/lab/galleria/

It is an administrator's module, and the target audience is site builders who want to provide a beautiful jQuery image gallery for their clients.

How it works
---------------------

Galleria relies on the core upload.module. You attach your images to a node, and when you display it, the module replaces the attachment list with a Galleria.

1.  Choose the node type that you want converted into Gallerias. All nodes of this type will be converted. You'd usually make a type called "galleria".
2.  On that node's "edit content type" page, you'll have an option "Enable Galleria" under the "Workflow settings".
3.  On the same page, enable file uploads (Attachments).
4.  Create a new Galleria node, and attach the images using the upload files function.
5.  View the node, and voila, you have a Galleria!

Lightbox support
---------------------

You can have your Galleria open inside a lightbox. Start by downloading and installing the Lightbox2 module from http://drupal.org/project/lightbox2. Then use the following Galleria theme function:

theme('galleria_lightbox_link', $nid, $width, $height, $text);

This will give you an anchor link to the Galleria with node id "$nid". Clicking the link opens a lightbox containing the Galleria. This is done by requesting an iFrame from the module.

Captions
---------------------

To add a caption to each image, install the CCK module from http://drupal.org/project/cck, and then add a text field to your Galleria node type, with the properties:

Label: Captions
Name: field_captions
Type: Text
Form element: Text field
Number of values: Unlimited

Then you can add captions to each image. They will be chosen in order, i.e. the first caption will apply to the first image in the list.

Customisation
---------------------

There is a preprocess hook and template files for all your customisation needs. You can style the galleria by overriding the CSS.

Similar modules
---------------------

BornFree (http://drupal.org/project/bornfree) also implements the Galleria
plugin. The reasons I chose to go my own way:

- BornFree uses blocks to display images / gallerias.
- BornFree relies on image and image_attach to get these images from nodes.
- Galleria uses the core Upload module to attach a bunch of images to a node.
- Galleria uses Drupal theme layer properly (i.e. preprocess function and
  a template file). BornFree does not.
