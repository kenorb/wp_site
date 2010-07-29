Module: Comment-Subscribe
Author: Zyxware <www.zyxware.com/contact-us>

Description
===========
Comment follow up e-mail notification for anonymous as well as registered users.
If you write a comment to some post ,the follow up comments for your comment will be informed to you through your email provided in the comment.Also if you don't want email notification you can unsubscribe it using the link provided in your email.There are two way for unsubscription.One is to unsubscribe email notification for the whole node.Other option is to unsubscribe email notification for a particular comment.

Usage
=====
Click the checkbox below the comment textarea.
Also one can unsubscribe the same by clicking the link provided in the email.

Installation and configuration:
------------------------------
Simply extract the download package in your modules directory and 
then enable the module in 'admin/build/modules/'.

Further Enhancement
-------------------
Send notification mails in background 
For big sites and even nodes with 50+ comment the in-sync mail sending can lead to 
a massive delay... so these mails will be put into a notification queue and sent a bit later via cron.

New Features
-------------
Comment notification for authors.
Authors can subscribe to the comments for their nodes. He can choose which node he want to receive notification from.
