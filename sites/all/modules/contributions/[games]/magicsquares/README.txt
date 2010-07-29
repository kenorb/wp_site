$ Id: $

------------
Description
------------

Plays the game 'Magic Squares' where the user orders tiles as quickly as possible.
The tiles can either be text numbers or a picture that can be themed to your website.
The game is javascript (original javascript script by Arun Narayanan) based and is
a block. The user who solves it the fastest has his name saved. The game win messages
are configurable in the configuration settings. Also the URL of the picture and the
size of the magic square can be set in the configuration settings.

------------
Requirements
------------

- Drupal 5

- Magic Squares module should be enabled

------------
Installation
------------

- Create a new directory "magicsquares" in your "modules" directory and place the
  entire contents of this magicsquares directory in it.

- Enable the module by navigating to Administer -> Site building -> Modules.

- Grant the proper access to user accounts under Administer -> User management
  -> Access control. The most important setting is "play magicsquares" for
  all roles.

- Enable the Magic Squares block by navigating to Administer -> Site building ->
  Blocks.

------------
Credits
------------
Written by
  - Glenn Linde <glinde AT skinhat DOT com>

Based upon javascript script by
  - Brian Gosselin   http://jsgames.sourceforge.net/ 
