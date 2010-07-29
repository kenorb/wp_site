<?php $gallery_path = 'node/'.$gallery->nid; ?>
<div class="gallery-cover-view">
<div class="cover-image"><?php print l($cover_image, $gallery_path, array('html' => true)) ;?></div>
<h4 class="title"><?php print l($gallery->title, $gallery_path)?></h4>
<div class="meta"><?php print $meta_data; ?></div>
<?php print $cover_operations;?>
</div>