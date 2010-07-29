<?php
// $Id: page.tpl.php,v 1.25 2008/01/24 09:42:53 goba Exp $
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>
  <title><?php print $head_title ?></title>
  <meta http-equiv="Content-Style-Type" content="text/css" />
  <?php print $head ?>
<link type="text/css" rel="stylesheet" media="all" href="/sites/all/themes/custom/polandia/style.css" />
  <?php print $styles ?>
  <?php print $scripts ?>
  
  
</head>
  
<body>
	<table width="100%">
        <tr>
            <!--<td width="50%" class="bg-left"></td>-->
            <td width="100%">
            	<table id="main">
                    <tr>
                        <td width="100%" class="header">
                        	<div class="top-left">
                            	<div class="top-right">
                                	<table width="100%">
                                        <tr>
                                            <td width="100%" style="height:213px;">
                                                <?php if ($header) : ?>
                                                    <div id="header"><?php print($header) ?></div>
                                                <?php endif;?>
                                            	<?php if ($logo) : ?>
                                                    <a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print($logo) ?>" alt="<?php print t('Home') ?>" border="0" class="logo" /></a><br />
                                                <?php endif; ?>
                                                
                                                <?php if ($site_name) : ?>
                                                    <h1 class='site-name'><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
                                                <?php endif; ?>
                                                
                                                <?php if ($site_slogan) : ?>
                                                    <div class="slogan"><?php print($site_slogan) ?></div>
                                                <?php endif;?>
                                            </td>
                                         </tr>
                                         <tr>
                                            <td width="100%" style="height:65px;">
                                                <table width="100%">
                                                    <tr>
                                                        <td width="">
                                                        	<?php if (isset($primary_links)) : ?>
                                                                <div class="pr-menu">
                                                                    <?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
                                                                </div>
                                                            <?php endif; ?>
                                                        </td>
                                                        <td width="280">
                                                        	<div class="search-box">
																<?php print $search_box; ?>
                                                            </div>
                                                        </td>
                                                    </tr>
                                                </table>
                                             </td>
                                         </tr>
                                         <tr>
                                            <td width="100%" style="height:20px;">
                                            	<div class="bg-bread">
                                                	<div class="bread-left">
                                                    	<div class="bread-right">
															<?php if ($breadcrumb != ""): ?>
	                                                            <?php print $breadcrumb ?>
                                                            <?php endif; ?>
	                                                    </div>
                                                    </div>
                                                </div>
                                            </td>
                                         </tr>
                                    </table>
                                </div>
                            </div>
                        </td>
                    </tr>
                    <tr>
                        <td width="100%">
                            <table width="100%" >
                                <tr>
                                	<td width="207">
                                        <div class="left2">
                                            <?php if ($left != ""): ?>
                                                <table width="100%">
                                                  <tr>
                                                    <td width="100%">
                                                        <?php print $left ?>
                                                    </td>
                                                  </tr>
                                                </table>
                                            <?php endif; ?>
                                        </div>
                                    </td>
                                    <td width="">
                                        <?php if ($is_front != ""): ?>
                                            <div id="custom"><?php print $custom ?></div>
                                        <?php endif; ?>
                                        <div class="cent">
                                            <?php if ($mission != ""): ?>
                                                <div id="mission"><?php print $mission ?></div>
                                            <?php endif; ?>
                                            <?php if ($content_banner != ""): ?>
                                                <div id="content_banner"><?php print $content_banner ?></div>
                                            <?php endif; ?>
                                                        
                                            <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>
                                            <?php if ($title): print '
                                                <h2'. ($tabs ? ' class="with-tabs title"' : '') .'>'. $title .'</h2>
                                            '; endif; ?>
                                            <?php if ($tabs): print '<ul class="tabs primary">'. $tabs .'</ul></div>'; endif; ?>
                                            <?php if ($tabs2): print '<ul class="tabs secondary">'. $tabs2 .'</ul>'; endif; ?>
                                                             
<?php
global $user;
if (strpos($messages,'node.nid')!==FALSE && $user->uid > 1) {
    $messages = '';
}
?>
                                            <?php if ($show_messages && $messages != ""): ?>
                                                <?php print $messages ?>
                                            <?php endif; ?>
                                        
                                            <?php if ($help != ""): ?>
                                                <div id="help"><?php print $help ?></div>
                                            <?php endif; ?>
                                        
                                            <?php if ($content_header != ""): ?>
                                                <div id="content_header"><?php print $content_header ?></div>
                                            <?php endif; ?>
                                              <!-- start main content -->
                                            <?php print $content; ?>
                                        </div>
                                    </td>
                                    <td width="208">
                                        <div class="right2">
                                            <?php if ($right != ""): ?>
                                                <table width="100%">
                                                  <tr>
                                                    <td width="100%">
                                                        <?php print $right ?>
                                                    </td>
                                                  </tr>
                                                </table>
                                            <?php endif; ?>
                                        </div>
                                    </td>
                                </tr>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td width="100%" id="footer">
                        	<div class="foot-left">
                            	<div class="foot-right">
                                	<?php if ($footer_message || $footer) : ?>
                                        <span><?php print $footer_message;?></span>
                                    <?php endif; ?>
                                </div>
                            </div>
                        </td>
                    </tr>
                </table>
            </td>
            <!--<td width="50%" class="bg-right"></td>-->
        </tr>
    </table>
<?php print $closure;?>
</body>
</html> 
