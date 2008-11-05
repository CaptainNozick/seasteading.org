<?php phptemplate_comment_wrapper(NULL, $node->type); ?>

<?php if ($is_front) : ?>

	<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

		<?php if ($submitted): ?>
			<span class="submitted"><?=format_date($node->created)?></span>
		<?php endif; ?>
		
		<h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>

	</div>

<?php else: ?>

	<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> post">

		<?php print $picture ?>

		<?php if ($page == 0): ?>
			<h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
		<?php endif; ?>

		<?php if ($submitted): ?>
			<span class="submitted"><?php print t('!date - !username', array('!username' => theme('username', $node), '!date' => format_date($node->created))); ?></span>
		<?php endif; ?>

		<div class="content">
			<?php print $content ?>
		</div>

		<?php if ($page == 0): ?>
			<div class="clear-block clear">
				<div class="meta">
				<?php if ($taxonomy): ?>
					<div class="terms"><?php print $terms ?></div>
				<?php endif;?>
				</div>

				<?php if ($links): ?>
					<div class="links"><?php print $links; ?></div>
				<?php endif; ?>
			</div>
		<?php endif; ?>

	</div>

<?php endif ?>