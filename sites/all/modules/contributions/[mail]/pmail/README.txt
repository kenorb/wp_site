/* $Id: README.txt,v 1.3 2007/11/18 20:06:09 smk Exp $ */

-- SUMMARY --

This module allows you to personalize any e-mails sent from your site by
enhancing the From and Reply-To email addresses with display names (eg.
"Some Name <foo@example.com>").

This is done in a fully configurable way by relying on the token module to 
provide on-the-fly substitutions, and contains a fall back on the user's login
name if the token replacement did't yield a result (for example because a user
didn't fill in the name on its profile).

For a full description visit the project page:
  http://drupal.org/project/pmail
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/pmail/issues


-- PREREQUISITES --

* Token
  http://drupal.org/project/token

* Enhanced token_user.inc from the patches directory.


-- INSTALLATION --

* Copy the module to your modules directory and enable it on the Modules
  page (admin/build/modules).

* Copy token_user.inc to the token installation directory (overwrite the
  existing file).

* Configure the replacement pattern to construct the display name from at the
  Personalized mail settings page (admin/user/settings).


-- CONTACT --

Author:
Stefan M. Kudwien (smk-ka) - dev@unleashedmind.com

