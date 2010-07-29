$Id: README.txt,v 1.2 2008/12/29 01:13:17 rmiddle Exp $

README for upload_preview.module
================================

Adds image preview thumbnails to the file attachment section on node editing pages. That way, users can see what image they are dealing with instead of guessing it from the file name. To prevent ultra wide tables, a file is now spread over two lines with the first displaying the image and the filename and the second one containing the checkboxes for deletion and listing, as well as the file size information.


Development
===========

This module provides a hook_upload_preview() which has one parameter: $file. This object is a regular file object. To inspect its properties, simply print_r() it.

If your module implements this hook, you usually want to restrict the file handling to certain mime types. If you have a preview for this file, return an array (Forms API type). If no module returns anything for a certain file, a placeholder is used instead.

function hook_upload_preview($file) {
  if (strpos($file->filemime, 'image/tiff') === 0) {
    // File handling code here    
    if ($file) {
      return array(
        '#type' => 'yourmodule_tiff_image',
        [ further parameters for your theme function ]
      );
    }
  }
}
