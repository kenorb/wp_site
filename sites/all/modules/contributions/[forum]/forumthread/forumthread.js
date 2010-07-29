/* $Id: forumthread.js,v 1.1.4.2 2008/07/29 05:27:22 brauerranch Exp $ */

$(document).ready(function(){ $('.forumthreadtitle').siblings().hide();  $('.forumthreadtitle').click(function() { $(this).siblings().toggle('slow'); return false; }); });
