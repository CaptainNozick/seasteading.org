<?php if ($block->module == 'icalnews' && $block->delta == 0) :?>

	<?=$block->content?>

<?php else: ?>

	<?php if ( ($block->module == 'month' && $block->delta == 0) || ( $block->module == 'user' && $block->delta == 1 )) : ?><div class="sub-menu"><?php endif ?>
	<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">

	<?php if ($block->subject): ?>
	  <h2><?php print $block->subject ?></h2>
	<?php endif;?>

	  <div class="content"><?php print $block->content ?></div>
		
		<?php if ($block->module == 'search' && $block->delta == 0) : ?><div class="more-link"><?=l('Advanced Search','search/node',null,'m=e')?></div><?php endif ?>
		
	</div>
	<?php if ( ($block->module == 'month' && $block->delta == 0) || ( $block->module == 'user' && $block->delta == 1 )) : ?></div><?php endif ?>

	<?php endif ?>