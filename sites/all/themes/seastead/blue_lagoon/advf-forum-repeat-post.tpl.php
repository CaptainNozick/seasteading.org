<?php
// $Id: advf-forum-repeat-post.tpl.php,v 1.7 2008/12/07 06:05:46 michellec Exp $

/**
 * @file advf-forum-repeat-post.tpl.php
 * Used for the repeated node on the top of each page of a paginated forum
 * thread. By default, it contains only the "header" information for the thread
 * and the rest is empty.
 *
 * If you leave it empty, subsequent pages will start with the next comment
 * like you typically find in forum software. You could also put a specially
 * formatted teaser to remind people what post they are reading. If you like
 * having the entire node repeated, simply copy the entire contents of
 * advf-forum-post.tpl.php into this file. All the same variables are available.
 */
?>

<div class="forum-post-header clear-block">

	<?php
		
		unset($links_array['notifications_2']); // removing subscriptions to "posts by xxx user"
		unset($links_array['notifications_1']); // removing subscriptions to All Forum Topics
		
		foreach ($links_array as $k => $the_link) {
			if ($k != 'notifications_0' && $k != 'notifications_1' && $k != 'notifications_2' && $k != 'notifications_3' && $k != 'notifications_4') {
				unset($links_array[$k]); 
			} else {
				if ($links_array[$k]) $links_array[$k]['title'] = '<img src="'.base_path() . path_to_theme().'/images/icon_envelope.png" alt="Subscribe/unsubscribe (E-mail): Topic replies (this topic)" title="Subscribe/unsubscribe (E-mail): Topic replies (this topic)" />';
			}
		}
		$rss_link = array(
			'html' => 1,
			'title' => '<img width="16" height="16" title="Subscribe (RSS): Topic replies (this topic)" alt="Subscribe (RSS): Topic replies (this topic)" src="'.base_path().'misc/feed.png" />',
			'href' => 'crss/node/'.$node->nid
		);
		array_unshift($links_array, $rss_link);

	?>
	
	<?php print theme('links', $links_array, array('class' => 'links forum-links subscriptions')); ?>

  <?php print $reply_link; ?>
  
  <div class="reply-count">
    <?php print $total_posts; ?>
    
    <?php if (!empty($new_posts)): ?>
      (<?php print $new_posts; ?>)
    <?php endif; ?>
  </div>
</div>