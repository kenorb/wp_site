<?php

$gallery_config = array( 'name' => 'test', 'gallery_type' => 'page', 'image_type' => 'story', 'data' => 'a:7:{s:14:"display_fields";a:2:{s:5:"title";s:5:"title";s:10:"body_field";s:10:"body_field";}s:17:"gallery_directory";s:23:"galleries/%gallery_name";s:13:"default_cover";s:15:"photo_album.png";s:10:"image_size";a:3:{s:5:"cover";s:15:"album_thumbnail";s:9:"thumbnail";s:15:"album_thumbnail";s:7:"preview";s:15:"album_thumbnail";}s:13:"view_original";s:1:"0";s:15:"content_display";s:5:"image";s:13:"image_comment";s:1:"2";}', 'display_fields' => array ( 'title' => 'title', 'body_field' => 'body_field', ), 'gallery_directory' => 'galleries/%gallery_name', 'default_cover' => 'photo_album.png', 'image_size' => array ( 'cover' => 'album_thumbnail', 'thumbnail' => 'album_thumbnail', 'preview' => 'album_thumbnail', ), 'view_original' => '0', 'content_display' => 'image', 'image_comment' => '2', );
$gallery1 = array( 'type' => 'page', 'language' => '', 'uid' => '1', 'status' => '1', 'created' => '1224168226', 'changed' => '1224168226', 'comment' => '0', 'promote' => '0', 'moderate' => '0', 'sticky' => '0', 'tnid' => '0', 'translate' => '0', 'vid' => '1', 'revision_uid' => '1', 'title' => 'Test Album', 'body' => '

This is a test album.
', 'log' => '', 'revision_timestamp' => '1224168226', 'format' => '1', 'name' => 'admin', 'picture' => '', 'data' => 'a:0:{}', 'last_comment_timestamp' => '1224168226', 'last_comment_name' => NULL, 'comment_count' => '0', 'taxonomy' => array ( ), 'files' => array ( ), 'build_mode' => 0, 'readmore' => false, 'content' => array ( 'body' => array ( '#weight' => 0, '#value' => '

This is a test album.
', '#title' => NULL, '#description' => NULL, '#printed' => true, ), '#title' => NULL, '#description' => NULL, '#children' => '

This is a test album.
', '#printed' => true, ), 'links' => array ( ), );

$image1 = array('type' => 'story', 'language' => '', 'uid' => '1', 'status' => '1', 'created' => '1224226255', 'changed' => '1224226255', 'comment' => '2', 'promote' => '0', 'moderate' => '0', 'sticky' => '0', 'tnid' => '0', 'translate' => '0', 'vid' => '2', 'revision_uid' => '1', 'title' => '1.jpg', 'body' => '

1.jpg
', 'teaser' => '1.jpg', 'log' => '', 'revision_timestamp' => '1224215772', 'format' => '0', 'name' => 'admin', 'picture' => '', 'data' => 'a:0:{}', 'gid' => '1', 'fid' => '14', 'weight' => '0', 'is_cover' => '1', 'filename' => '1.jpg', 'filepath' => 'sites/default/files/galleries/Test_Album/1_6.jpg', 'filemime' => 'image/jpeg', 'filesize' => '42608', 'timestamp' => '1224215727', 'last_comment_timestamp' => '1224215772', 'last_comment_name' => NULL, 'comment_count' => '0', 'taxonomy' => array ( ), 'files' => array ( ), 'build_mode' => 0, 'readmore' => false, 'content' => array ( 'body' => array ( '#value' => '

1.jpg
', '#weight' => 0, ), ), );