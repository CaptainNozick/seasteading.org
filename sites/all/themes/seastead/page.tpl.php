<?php include('head.tpl.php') ?>

<?php

	// Ok, let's determine whether we're looking at forum, container, topic or replying to a forum post
	$at_forum = false;
	if (arg(0) == 'comment' && arg(1) == 'reply') $commented_node = node_load(arg(2));
	if (
		arg(0) == 'forum' ||
		$node->type == 'forum' ||
		$commented_node->type == 'forum' ||
		(arg(0) == 'node' && arg(2) == 'forum')
	) {
		$at_forum = true;
	}
	
	//on the second thought, some admin tasks on content would require wider contents too
	$at_admin = false;
	if ( arg(0) == 'node' && ( arg(1) == 'add' || arg(2) == 'edit' )  ) {
		$at_admin = true;
	}

?>

<body<?php if($at_forum || $at_admin) : ?> class="forum"<?php endif ?>>
<!-- arg(0): <?=arg(0)?>, arg(1): <?=arg(1)?>, arg(2): <?=arg(2)?> 
	$node->type: <?=$node->type?>
	$commented_node->type : <?=$commented_node->type?>
-->
<div id="root">

	<div id="header" class="clr">
		<?php
			// Prepare header
			$site_fields = array();
			if ($site_name) {
				$site_fields[] = check_plain($site_name);
			}
			if ($site_slogan) {
				$site_fields[] = check_plain($site_slogan);
			}
			$site_title = implode(' ', $site_fields);
			$site_fields[0] = '<span>'. $site_fields[0] .'</span>';
			$site_html = implode(' ', $site_fields);

		?>
		<h1 id="logo"><a href="<?=check_url($base_path)?>" title="<?=$site_title?>"><span><?=$site_title?></span></a></h1>
		
		<?php include('user-login-bar.tpl.php'); ?>
		
	</div>
	
	<div id="wrapper-1">
		
		<?php print $header; ?>
		
		<?php

			if ($at_forum) {
				$menu_root = 64;
			} else {
				$menu_trail = _menu_get_active_trail();
				$menu_root = $menu_trail[1];
			}
			$menu_root_title = menu_get_item($menu_root);

			// Let's see if we're at a blog term page
			$at_blog_term = false;
			if (arg(0) == 'taxonomy') {
				$this_term = taxonomy_get_term(arg(2));
				if ($this_term->vid == 2) $at_blog_term = true;
			}
		?>
		
		<?php if( $menu_trail[1] == 62 ) : // Learn more ?>
			<div id="banner-sub" class="banner-learn-more">
				<h2><span>Learn more</span></h2>
			</div>
		<?php elseif( $menu_trail[1] == 63 || $at_blog_term || arg(0) == 'blog' || arg(0) == 'month' ) : // Stay in touch ?>
			<div id="banner-sub" class="banner-stay-in-touch">
				<h2><span>Stay in touch</span></h2>
			</div>
		<?php elseif( $menu_trail[1] == 65 || arg(0) == 'donate') : // Contribute ?>
			<div id="banner-sub" class="banner-contribute">
				<h2><span>Contribute</span></h2>
			</div>
		<?php elseif( $menu_trail[1] == 66) : // Strategic areas ?>
			<div id="banner-sub" class="banner-strategic-areas">
				<h2><span>Strategic Areas</span></h2>
			</div>
		<?php elseif( arg(0) == 'user' && ( is_numeric(arg(1)) && ( !arg(2) || arg(2) == 'edit' ) ) || arg(1) == 'password' || arg(1) == 'register' ) : // Strategic areas ?>
			
		<?php elseif ($at_forum || $menu_root_title == 'Interact') : ?>
			<div id="banner-sub" class="banner-interact">
				<h2><span>Interact</span></h2>
			</div>
		<?php endif; ?>
		
		<div id="content-1"<?php if ( arg(0) == 'blog' || $node->type == 'blog' || arg(1) == 'press-releases' || $node->type == 'press_release' ) : ?> class="post"<?php endif ?>>
			
			<?php if (!$at_forum) include('sidebar.tpl.php') ?>
			
			<div id="content-wrapper">
				<div id="content-wrapper-1">
				
					<?php if ($breadcrumb): ?>
						<div class="breadcrumb">
							<strong>You are here:</strong>
							<?php if ($node->type == 'forum') : ?>
								<?php 
									
									$bc = array();
									$bc[] = array('path' => $_GET['q']);
									menu_set_location($bc);
								?>
							<?php elseif(arg(0) == 'blog'): ?>
								<?php $title = 'Captain\'s Blog'; ?>
							<?php elseif(arg(0) == 'comment' && arg(1) == 'reply'): ?>
								<?php $title = 'Reply'; ?>
							<?php endif ?>
							<?=$breadcrumb;?>
							<?php if ($title): ?>
								<span>&gt;</span> <?=$title?>
							<?php endif ?>
						</div>
					<?php endif; ?>
				
					<?php if ($tabs): print '<div id="tabs-wrapper">'; endif; ?>
          <?php if ($title): print '<h2'. ($tabs ? ' class="with-tabs"' : '') .'>'. $title .'</h2>'; endif; ?>
          <?php if ($tabs): print $tabs .'</div>'; endif; ?>

          <?php if (isset($tabs2)): print $tabs2; endif; ?>

          <?php if ($help): print $help; endif; ?>
          <?php if ($messages): print $messages; endif; ?>
          <?php print $content ?>
				
				</div>
			</div>
			
			<?php if ($at_forum) include('sidebar.tpl.php') ?>
			
		</div>
		
	</div>
	
	<div id="footer" class="clr">
		<?php if ($mission): ?> <div id="mission"> <?=$mission?> </div> <?php endif; ?>
		<div class="design-info"><span class="copy">Copyright &copy; 2008 Seastead.org |</span> <h1 class="design"><a href="http://helldesign.net/" onclick="window.open(this.href,'',''); return false"	title="Helldesign">Design by<span> Helldesign</span></a></h1></div>
	</div>

	<?php print $closure ?>
	
</div>
</body>
</html>