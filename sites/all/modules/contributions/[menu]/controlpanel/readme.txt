
********************************************************************
                     D R U P A L    M O D U L E
********************************************************************
Name: Control Panel Module
Author: der <dreed10 -at- gmail -dot- com>
Drupal: 4.7, 5.x and 6.x
********************************************************************
DESCRIPTION:

This module adds a graphical control panel page. It allows the user 
to specify the menu path to use as the source for the Control Panel.
It also allows the user to specify if they want the Control Panel 
module to recursively build sub panels for the main Control Panel. 

The Control Panel can also be generated through a Drupal block. 
Multiple blocks can be created each with their own menu path source.

For developers:  You can use the build_controlpanel function of this 
module to generate a controlpanel anywhere in your module or template
code.  The function call looks like:

controlpanel_build_controlpanel('menu name:menu id', TRUE, NULL) 

The first parameter is the menu name followed by the menu id separated 
by a colon.

********************************************************************
INSTALLATION:

1. Create folder 'modules/controlpanel'

2. Copy all the modules files, keeping directory structure, to the 
folder 'modules/controlpanel'

3. Enable the controlpanel module in 'admin/build/modules'

4. [OPTIONAL] Configure module in admin/settings/controlpanel

5. If using Control Panel blocks configure the block(s) in 
admin/build/block


********************************************************************
