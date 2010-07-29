********************************************************************
                     D R U P A L    M O D U L E
										 an addon for uLink module
********************************************************************
Name: uAuto module
Author: Gartheeban Ganeshapillai 
        AKA garthee at drupal <garthee at gmail dot com>
Dependencies: uLink.module, search.module(optional)

********************************************************************
IMPORTANT

Users are encouraged to read the README.txt of uLink module

********************************************************************
DESCRIPTION:

The uAuto module does auto-completion of links by providing the user
with possible links for the text selected or being typed. For example,
when user enters [l|use, he will be provided with the possible links
that could be connected with the key “use”. Alternatively, he could
select the complete word by double clicking it or highlighting it,
and he will be provided with the possible candidates (however only 
mouse based selections - double click or select highlight - are 
supported currenly).

The module uses the protocol defined by uLink module and generates
the link in the format:
 [l|link]
 or
 [l|link|text]
 or 
 [l|link|text|attribute]
 
The module provides an excellent, configurable and extensive search
(can be optionally enabled) which can be extended (by implementing
the hook under circumstances like connecting two peer sites or
supporting external links).

uAuto_dirsearch module is written as a seperate module to explain
how search funtionalities can be extended. It provides the search
on local files which are not listed in the files table in the database.

Settings for the module
------------------------------------------------------------------
Available under 
Admin > Settings > ulink > General settings : uauto - settings
(admin/settings/ulink)

Allow root-user to be referred :
		Some sites may want to prevent root user account from referring to
		in such case this could be ticked off.
		
kSearch:
    kSearch is automatically enabled with uAuto as iSearch also depends on it,
    hence it cannot be optionally turned off.
		
    kSearch is triggered when user enters	[l|
		
    Limit: Number of results to be presented to user. The higher the number of 
    searches the greater will be time and it might not present the desired output.
    Reasonable amount of 5 is set default.
iSearch
		iSearch is triggered when user selects a word by highlighting it or 
		double clicking it.
		
		Extensive node search:
				By enabling this, users can benefit from Drupals extensive
				search at the expense of longer time. However, results are cached,
				therefore subsequent searches would benefit.
        
		Limit: Number of results to be presented to user. The higher the number of 
    searches the greater will be time and it might not present the desired output.
    Reasonable amount of 5 is set default.

Usage
-------------------------------------------------------------------
UI is self-explaining and much similar to autocomplete module's.

kSearch:
When user type [l	search is triggered and result is presented in an 
auto complete list. Also while searching is carried out, a throbbing
icon is displayed similar to autocomplete.

User can use mouse or keyboard (up | down | tab | esc) keys to select.
Once selected it is inserted in the tag. Remember it is '|' key triggers
the search everytime, hence you can edit with autocomplete by typing '|' 
where you want to trigger the autocomplete the particular token.

Esc, [ and ] keypress cancels the search.

All three tokens (link, text, attributes - through the macros defined under
genral settings) are supported by autocomplete. 

eg : [l|
      triggers the autocompletion for the link token following, for instance
        [l|my_user
      will complete it with [l|user/2
     [l|link|
      triggers the autocompletion  for the text token
        [l|user/2|
      will complete it with [l|user/2|my_user
     [l|link|text| 
      triggers the autocompletion  for the attributes - macro token
        [l|user/2|
      will complete it with [l|user/2|my_user|attributes
     user ends the autocompletion with ] key
     
     At any time by pressing | key at the right location autocompletion for
     a particular token can be triggered, thus it can edited automatically.

iSearch:
When user selects with double click or mouse highlight, search is triggered,
provided it is enabled in the settings.

When a choice is opted, selected text is replaced with 
[l|link|text

User can then use kSearh i.e. by pressing | to complete the attributes or 
end the tag with ]


Caching
-------------------------------------------------------------------
All results are locally (at client side) cached and used when called
repeatedly. Browswer window has to be refreshed to clear them.
In addition, user selections are sent to the server to make subsequent
searches, especially those involving extensive node searches quicker and
thus frequently used texts are quickly and correctly matched as per user's
preference. However it is currenly done sidewise not userwise.

*** Important suggestion ***
	Having this enabled initially will move popular results to the cache
	and then extensive search can be turned off. users will still get the
	results from cache. 		

External References
--------------------------------------------------------------------
[1] Implementing uLink hooks 
Developer_documentation_ulink.txt
[2] Readme for ulink module
Readme.txt

********************************************************************
SYSTEM REQUIREMENTS

Drupal 5.x 
PHP 5.0.0 or greater
uLink.module and Filter.module installed

********************************************************************

CREDITS:

 - Thanks to Google for sponsoring this project through GSOC 2007
 - Thanks to Drupal community, Daniel DeGeest and Kaustubh Srikanth for seleting
	 this module for GSOC


********************************************************************
BUGS AND SUGGESTIONS

Please report all bug reports at:
http://drupal.org/project/issues/ulink

Contact me at:
garthee at gmail dot com
050131 at ent dot mrt dot ac dot lk

Demos at:
http://www.theebgar.phpnet.us/drupal?q=projects/ulink
http://mwt.argz.com/ulink/tests



