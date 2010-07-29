$Id: README.txt,v 1.1 2008/11/26 21:43:51 alexb Exp $

FeedAPI Comments - Emfield integration
======================================

This module simplifies the UI on node forms with Embedded Media fields and 
enabled FeedAPI. 

Use this module if you would like to aggregate comments from the media you're
embedding. That is, use it if the input URL of the Embedded Media field is the 
same as the input URL for FeedAPI. 

When enabled, this module hides FeedAPI entry fields on node forms if an 
Embedded Media textfield exists. It uses the entry value of the Embedded Media 
textfield as entry value for FeedAPI comments. Only works for user  without 
'advanced feedapi options' permissions.