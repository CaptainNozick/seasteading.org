<?php include('head.tpl.php') ?>

<body>
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
		$rnd_banner = rand(1,15);
		?>
		
		<div id="banner" style="background-image: url('<?php print base_path() . path_to_theme() . '/images/banners/' . $rnd_banner?>.jpg')">
		
			<div id="key-areas-backing"></div>
			
			<div id="key-areas">
				<?php if ($banner_blocks): ?>
					<?=$banner_blocks ?>
					<form action="http://groups.google.com/group/seasteading-announcements/boxsubscribe">
						<p><strong>Join our mailing list</strong><br />
							(1-2 messages per month):<br />
						<input type="text" name="email" /> <input type="image" src="<?php print base_path() . path_to_theme() ?>/images/submit_arrow.gif" /></p>
					</form>
				<?php endif; ?>
			</div>
			
		</div>
		
		<div id="content-home">
		
			<div id="navigator">
				<a href="javascript:prevItem(true)"><img src="<?php print base_path() . path_to_theme() ?>/images/go-left.png" /></a>
				<span id="current-item">1</span>/<span id="total-items"></span> 
				<a href="javascript:nextItem(true)"><img src="<?php print base_path() . path_to_theme() ?>/images/go-right.png" /></a>
			</div>
			
			<div id="center_left">
				<div id="center_left_inner">
					<?php if ($center_left): ?>
						<?=$center_left ?>
					<?php endif; ?>
				</div>
			</div>
			
			<div id="center_right">
				<div id="center_right_inner">
					<?php if ($center_right): ?>
						<?=$center_right ?>
					<?php endif; ?>
				</div>
			</div>
			
			<script type="text/javascript" src="<?=base_path() . path_to_theme() ?>/scripts/randomizer.js"></script>
						
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