// $Id: README.txt,v 1.1.2.3 2008/07/09 14:52:43 gmarus Exp $

README.txt
==========
This module adds a new node content type ('job posting')
intended to facilitate the advertisement of available employment 
opportunities at a particular hosted site or on behalf of client
organizations (i.e. in the case of recruiting companies). 

It is lightweight, with no additional module dependencies beyond Drupal 
core, and includes both a display block listing the most recently added 
job postings as well as a paginated job postings list page. An optional
'deadline' field controls how long and whether a particular posting is displayed 
via the block and listing page. It is displayed on job postings as a
'countdown' timer (i.e. n weeks n days n hours n minutes) rather than
a fixed date/time. Posting creators can also choose to make a particular
node 'permanent' rather than associated with a particular deadline. 

This module also includes basic job application processing via a link
on each node page. Users can click on this link and are taken
to an application form that allows them to provide both an introductory
covering letter as well as upload and attach a resume file from their own 
local machine. Applications are sent via email to the address provided when 
the job posting is created/edited. Applicants are sent a short confirmation 
message acknowledging their submission.

**** KNOWN ISSUES ****
Due to changes between PHP 4 and PHP 5 the job application processing feature 
may NOT function properly using PHP 4. It is strongly recommended that you use 
only PHP 5 or greater to fully utilize this module. 
See http://drupal.org/node/278778 for a discussion regarding this issue.


PACKAGE CONTENTS
================
README.txt                - this file
LICENSE.txt               - terms of use
CHANGELOG.txt             - list of code changes associated with each release
job_posting.info          - configuration file
job_posting.install       - creates/removes module database tables
job_posting.module        - main module file
job-posting-block.tpl.php - default theme template for job posting blocks
job-posting-node.tpl.php  - default theme template for job posting nodes
job-posting.css           - default theme stylesheet


REQUIREMENTS
============
This module requires Drupal 6.x or higher and will NOT work without 
modification under previous versions.


INSTALLATION
============
1. Extract the package into its own directory under the site module 
   directory (usually 'sites/all/modules').

2. Visit the module configuration page at administer >> site building >> modules
   to enable this module.

3. Visit the module's administrative settings page at 
   administer >> site configuration >> job posting settings to set 
   system-wide preferences.

4. Visit user management >> permissions and set module access controls as
   desired. Currently only authenticated users are allowed to apply for jobs
   through the system in order to reduce the potential for email-related abuse.
   Posters require additional privileges as indicated on the 'permissions'
   page.

5. If desired, enable the built-in job post listing page menu item by 
   visiting administer >> site building >> menus >> navigation and/or the 
   display block by visiting administer >> site building >> blocks.
   
6. Ensure that the system's default timezone has been set properly at
   administer >> site configuration >> date and time. This is used to
   calculate system local time. Failure to set this to the correct timezone
   may lead to (possibly) undesirable results like postings being
   expired at the wrong time/day.

7. Modify the included template files and/or css stylesheet to get the 
   presentation you want for block and node output. 

Included is an installation file (job_posting.install) which adds two
new database tables: one for the job posting nodes themselves and another
auxiliary table containing a list of countries used to populate the
pulldown select form control when job posting nodes are created/edited.


TO UNINSTALL/REMOVE
===================
To uninstall, first disable the module on the modules page before
clicking the 'uninstall' tab so that the tables that were added
at installation time are removed from the system (provided this
is what you want).


AUTHOR/MAINTAINER
=================
Greg Maruszeczka <greg@websage.ca>.