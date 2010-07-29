Please add your Flash players and embedding scripts to this directory.
You can usually do this by downloading the supported flash player, unzipping
the files, and moving the whole file structure in the modules/swftools/shared
directory.

Links - Media Players:
MODULE        URL
wijering      http://jeroenwijering.com/?item=Flash_Media_Player
wijering      http://jeroenwijering.com/?item=Flash_Image_Rotator
simpleviewer  http://www.airtightinteractive.com/simpleviewer
onepixelout   http://www.1pixelout.net/code/audio-player-wordpress-plugin

Links - Flash Replacement
MODULE           URL
swfobject        http://blog.deconcept.com/swfobject
ufo              http://www.bobbyvandersluis.com/ufo
jquery.flash.js  http://jquery.lukelutman.com/plugins/flash

Default file structure for install players and scripts. You can change the
"Default flash player path" in admin/media/swf/embed to change the "shared"
directory. However, inside the "shared" directory, the paths to the player
file are not configurable.

  swftools/
    shared/
      generic/generic_flv.swf
      generic/generic_mp3.swf
      1pixelout/player.swf                  (onepixelout.module)
      flash_media_player/mediaplayer.swf    (wijering.module)
      flash_image_rotator/imagerotator.swf  (wijering.module)
      simpleviewer/viewer.swf               (simpleviewer.module)
      swfobject/swfobject.js             (swfobject.module)
      ufo/ufo.js                            (ufo.module)
      lutman/jquery.flash.js                (lutman.module)

Refer to http://drupal.org/project/swftools for documentation.


$Id: README.txt,v 1.1.2.5.2.1 2008/07/25 22:35:50 stuartgreenfield Exp $