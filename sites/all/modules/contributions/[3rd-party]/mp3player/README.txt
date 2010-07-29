# $Id: README.txt,v 1.1.2.1 2009/01/02 12:14:48 starnox Exp $

==============================
Info
==============================

MP3 Player brings the WordPress Audio Player to Drupal. Easily enable the MP3 Player on a CCK FileField. You can also change any of the WordPress Audio Player settings through an admin interface.

==============================
Installation
==============================

Watch a video: http://blip.tv/file/1628471

 1. Drop the mp3player folder into the modules directory (/sites/all/modules/)
 2. Download WordPress Audio Standalone Player (http://wpaudioplayer.com/download)
 3. Drop the audio-player.js and player.swf files from the 1pixelout download into the internal mp3player directory (/sites/all/modules/mp3player/mp3player/).
 4. Enable MP3 Player module (?q=/admin/build/modules) and optionally FileField MP3 Player Formatter for use with CCK
 5. You can change the player settings (?q=admin/settings/mp3player)

Required Modules:
------------------------------

 - None

==============================
Custom Code Use
==============================

Pop this code anywhere to insert an audio player.

<?php print theme('mp3player', $audio_file_url, $audio_title, $audio_artist); ?>

==============================
The Future  
==============================

If you have any questions, issues, or feature suggestions then please do leave feedback on the project page (http://drupal.org/project/mp3player)

==============================
Sponsorship 
==============================

This module is sponsored by Alpha International (http://www.alpha.org), Holy Trinity Brompton (http://www.htb.org.uk) and CoreDesigns (http://www.coredesigns.co.uk)