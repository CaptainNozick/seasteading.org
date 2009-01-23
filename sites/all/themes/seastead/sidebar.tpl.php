<div id="sidebar">
	
	<? if (
		arg(0) != 'user' &&
		!(arg(0) == 'node' && arg(1) == 'add') &&
		!(arg(0) == 'node' && arg(1) == 'edit') &&
		arg(0) != 'blog' &&
		arg(0) != 'month' && 
		arg(0) != 'search' && 
		$node->type != 'blog' &&
		!$at_blog_term &&
		$menu_root_title['title'] != 'Content' &&
		$menu_root_title['title'] != 'View' &&
		$menu_root_title['title'] != 'Registered Users'
	) : ?>
	
		<div class="sub-menu">
			<div id="block-menu-83" class="clear-block block block-menu">
				<h2><?=$menu_root_title['title']?></h2>
				<div class="content">
					<?=theme('menu_tree',$menu_root)?>
				</div>
			</div>
		</div>
	
	<?php endif ?>
	
	<?php if ($sidebar_left): ?>
		<?=$sidebar_left ?>
	<?php endif; ?>
	
	<?php if ( arg(0) == 'blog' || $node->type == 'blog' || arg(0) == 'month' || $at_blog_term || (arg(0) == 'stay-in-touch' && arg(1) == 'press-releases') || $node->type == 'press_release' || $commented_node->type == 'press_release') : ?>
	
		<?php
			$rss_url = '';
			if (arg(0) == 'blog' || $node->type == 'blog' || arg(0) == 'month' || $at_blog_term) $rss_url = base_path() .  'blog/feed/full';
			if ((arg(0) == 'stay-in-touch' && arg(1) == 'press-releases') || $node->type == 'press_release' || $commented_node->type == 'press_release') $rss_url = base_path() . 'stay-in-touch/press-releases/feed'
		?>
	
		<div id="subscribe" class="clear-block block">
			<div class="content">
				<p><strong>Subscribe by:</strong> <a href="<?=$rss_url?>">RSS</a><!-- &nbsp;|&nbsp;  <a href="#">E-mail</a>--> <a href="<?=$rss_url?>"><img width="16" height="16" title="Syndicate content" alt="Syndicate content" src="<?=base_path()?>misc/feed.png"/></a></p>
			</div>
		</div>
	
	<?php endif ?>
	
</div>