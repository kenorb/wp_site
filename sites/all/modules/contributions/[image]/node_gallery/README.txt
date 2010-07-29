; $Id: README.txt,v 1.3 2008/11/17 01:46:40 wilson98 Exp $

DESCRIPTION
--------------------------
Node gallery enable users to create more flexible and powerful gallery. It's fully integrated with drupal's core node system. Users choose what kind of content type they want to be the "Gallery" type, and what kind of content type they want to be the "Image" type. When such a relationship is established, users can batch uploading "Images" into "Gallery", editing them within "Gallery", or just deleted them.
Because "Image" is a node type, "Gallery" is a node type too, so you can naturely use Drupal's taxonomy system, comment system, Voting system, cck module, views module and other powerful modules to enhance your gallery.

FEATURES
---------------
- Node as image, node as gallery.
- Use drupal's core batch system to do batch operations such as uploading, editing etc.
- Naturely integrated with taxonomy system, comment system, voting system, cck module and others. You can do all you can do with nodes to your gallery and image.
- Per user albums.
- And many more other features. 

REQUIREMENTS
---------------
- Upload module.
- Imagecache module.

CONTRIBUTION
---------------
- Node_gallery_access module. Control the access type (public, private, password) of your gallery. 

INSTALLATION
---------------
- Enable the Node gallery modules.
- Go to "Site building" -> "Node Gallery" -> "Add", follow the forms to set up a gallery-image relationship.
- Create a "gallery", you'll find the gallery operation bar allowing you batch uploading "images" and editing.
- Enable the Node gallery access module to control your gallery's access.

CREDITS
----------------------------
Authored and maintained by Wilson Wu <martic98 AT gmail DOT com>