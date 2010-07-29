The iPaper module user's guide
For Drupal newbies and experts alike.

*Why use iPaper*
www.scribd.com has created an online document viewing and sharing portal. The documents look great and load really quickly!
Advantages of incorporating iPapers in your site:
-supports many different formats (including images, excel files, OpenOffice etc.)
-users can't download file if you don't let them
-Document hosted on Scribd can be public (showing up on Scribd as well) or private (accessible only through your site)

*About the module*
The scribd.com iPaper application can be incorporated into your own Drupal website through this module. This can be done by attaching 
files to the iPaper node type which the module defines. You can also 
seamlessly convert all your existing Drupal attachments to iPapers 
by using Quickswitch. 

*Requirements*
To create iPaper nodes (these do not apply to Quickswitch):
-PHP 5 (http://drupal.org/node/272299)
-cURL library on your PHP or "allow_url_fopen" set to true in your php.ini (one of these is available at most web hosts by default).
Your visitors need to have Adobe Flash Player and JS enabled browsers (although plain text can be displayed with the iPaper object by granting the respective permission).

*Getting started*
After you install the module, get an API key for your website at http://www.scribd.com/platform/start. It only takes a minute. Enter the API key on the configuration page (/admin/settings/ipaper). 
You cannot use this module to upload files to scribd without an API account. After you have an account you will be able to see the files you uploaded through the Drupal site under "My documents" on scribd.com (www.scribd.com/mydocs).

*Creating iPapers*
After you enter your API key, you should be able to go ahead and create iPaper nodes. Attach a document of the types supported by Scribd and save the node (you cannot save it unless you attach at least one file). You will be taken to a progress screen while the file is sent to and processed by Scribd, and you should then see the node page with the iPaper display. Errors regarding the API requests and document conversion are recorded in the event log.
When you delete an iPaper node, the corresponding Scribd document is also deleted.
If you have more attachments, a message is displayed and only the most recent one is sent to Scribd. When you save the node with a new attachments, the new file becomes a new revision of the existing Scribd document. This allows you to update documents without having to delete the node. 
If you need to show more files as part of the same node, consider using QuickSwitch (see below) on a different node type.

*Permissions*
Remember to set the necessary permissions extended by the module. You should be aware that the module will show more than you need if you are logged on with the administrator account (user 1), because you have all the permissions.
In addition to the self-explanatory "create ipapers", "edit ipapers", "edit own ipapers", "delete ipapers", "delete own ipapers", take note of the following:
-"embed ipapers" shows users a text box with embed code for the current document in node view. This code also works if your document is private.
-"download ipapers" allows users to download a PDF version of the upload (a link is added under the iPaper). The format in which the download is offered can be set in the administration form. The link added to the node is to a Scribd dynamic URL.
-"edit ipaper parameters" is normally useful only to administrators. You might, however, have a setup in which you want to embed documents that are already uploaded, so then you can extend the "edit ipaper parameters" to the users who would enter these parameters manually.

*Quickswitch*
Quickswitch replaces URLs to documents with iPapers. Go to http://www.scribd.com/platform/documentation/quickswitch to get the code and customize the appearance of the iPapers. This feature works for documents that are not stored through iPaper nodes.
To use Quickswitch, your files need to be accessible without logon. This means that if you have private uploads, the anonymous user needs to have permission to see them.
You might find that you can implement QuickSwitch on your site without this module. Perhaps for your needs it's enough to create a block with
the code you get from Scribd, and display that code on all pages.
However, if you incorporate Quickswitch by pasting the code in the module's configuration page, it will help you integrate Quickswitch into Drupal in two ways:
-removes the attachments table and replaces it with just the links (the upload module normally creates a table for attachments table)
-does not show QS code with iPaper nodes (if you wanted to leave the regular list of uploads showing under the iPaper display, you could, without them being shown as iPapers)
Note: In order to delete an attachment/file that has been displayed through Quickswitch, you must log in to scribd.com and delete it 
manually from your account.

*More*
You can extend the iPaper content type by installing CCK (drupal.org/project/cck) and adding more fields to be displayed with your documents. The iPaper module does not extend a CCK field and does not modify other content types (http://drupal.org/node/272321). So if you want iPapers showing up with within a node type other than iPaper, you might have to use Quickswitch instead (or Slurp, or paste custom embed codes you got from Scribd.com).
If you want to disable the 'Body' text field for iPapers and to not show it on the form nor in the teaser view, set the label for the body field to blank.

*Known problems*
-When uploading a file to Scribd, the wait time is limited to the PHP execution time limit('max_execution_time') or 57 seconds, whichever comes first. If the file is not ready to display by then, an error is recorded in the event log and the user is taken to view the node. Whenever this happens, the plain text of the file will also not be available for inclusion in your site's search index.
-CCK Warning: As with all content types, if you disable the iPaper module, you will lose all CCK data! You will not lose your iPaper nodes unless you uninstall, but CCK will delete 'content_fields_ipaper' if the module is disabled.

*Help*
Developers, You can find documentation for the API at http://www.scribd.com/platform/documentation/api.
As you work to extend this module, you can find answers at http://www.scribd.com/platform/faq 
and ask questions at http://groups.google.com/group/scribd-platform-developers/topics

Post support requests at http://drupal.org/project/issues/ipaper

Project created by Rares Pamfil - www.rarespamfil.com