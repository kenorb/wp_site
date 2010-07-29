NOTE: Important - still in development, be careful!

INSTALLATION:

1. Unzip and place the included chess pieces in your fiels directory (default is /files/chess)
2. Enable module
3. Go to administer >> site configuration >> Chess settings (/admin/settings/chess) and verify settings
   The module defaults should work as is, but display of chess pieces may need to be modified depending on your theme.
4. Enable Chess blocks (admin/build/block) - there are three to choose from
   - Chess - displays the chessboard in a block. This is required.
   - Previous moves - lists the previous moves. This is optional.
   - Edit last move - provides a button to allow user to undo last move if opposing player has not yet moved. 
5. Create a new chess match (node/add/chess)
5.1. Select players
5.2. Save
6. Play!

NOTES:

a. The form to enter a move will only display if you are a registered player, and if it is your move
b. This module is still very early in development, contact eoin@eoinbailey.com if you encounter any bugs
c. you can change the chess pieces, but the naming is important, it follows standard chess notation
d. It is up to you to determine if a move is legal
e. This is my very first Drupal module, be nice!


ROADMAP
version 0.01 - display a board
version 0.05
	- display board
	- add chess content type
	- add user reference fields to chess content type
	- enable moves
	- verify players
	- draw board based on state saved in DB
version 0.06
	- removed need for CCK module
	- rewrote the module to create custom content type 'chess'
	- added uninstaller
	
	
Future Versions
	- display captured pieces
	- replay all previous moves
	- option to show a second block, that shows the previous state (state before the current move)
	- enable queening
	- verify moves (only allow legal moves) - not sure about this, standard chess rules don't enforce a 'legal move' system, it is up to the players to verify moves.


CHESS PIECE NAMING:

the pieces must have specific names also like this: (fill this in to enable people to make their own squares)

Chess pieces have to be named as follows:

Chess_<piece code><player colour><square colour>44.png

where piece codes are as follows: 
r - rook
n - knight
b - bishop
k - king
q - queen

player colour
d - dark
l - light

square colour
d - dark
l - light

AUTHORS:

chess was developed and written by Eoin Bailey (eoin@eoinbailey.com)
chess 6.x.1.x-dev was initially ported from 5.x.0.09 by MGN (MGN@drupal.org)

This README.txt file has been slightly modified from the 5.x.0.09 version to better reflect
the 6.x port.
