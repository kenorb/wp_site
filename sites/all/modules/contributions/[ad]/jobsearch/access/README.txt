$Id: README.txt,v 1.2 2009/01/02 02:49:22 kbahey Exp $

Copyright 2007-2009 http://2bits.com

Description
-----------
This module allows resumes and jobs to be private, and hence
seen only be certain roles. Moreover, resumes can be viewable
only to the author, and to those in the employer/recruiter role.

Installation
------------
To install this module, upload or copy the job folder and all the files
in it to your modules directory.

Configuration
-------------
To enable this module do the following:

1. Go to Administer -> Site Building -> Modules, and enable Job access
   and Resume access modules.

2. Go to Administer -> User Configuration -> Roles.
   Create roles for the job seekers, and the recruiter/employer.

3. Go to Administer -> User Configuration -> Access Control.
   Assign view jobs to the job seeker role. Also assign view resume to
   the recruiter/employer role.

6. Visit Administer -> Content Management -> Post settings and click on
   "Rebuild Permissions".

Author
------
Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com)
