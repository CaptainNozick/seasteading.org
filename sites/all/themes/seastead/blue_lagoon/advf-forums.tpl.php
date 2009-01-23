<?php
// $Id: advf-forums.tpl.php,v 1.6 2008/11/18 02:30:07 michellec Exp $

/**
 * @file forums.tpl.php
 * Default theme implementation to display a forum which may contain forum
 * containers as well as forum topics.
 *
 * Variables available:
 * - $links: An array of links that allow a user to post new forum topics.
 *   It may also contain a string telling a user they must log in in order
 *   to post. Empty if there are no topics on the page. (ie: forum overview)
 * - $links_orig: Same as $links but not emptied on forum overview page.
 * - $forums: The forums to display (as processed by forum-list.tpl.php)
 * - $topics: The topics to display (as processed by forum-topic-list.tpl.php)
 * - $forums_defined: A flag to indicate that the forums are configured.
 * - $forum_description: The forum's taxonomy term description, if any.
 *
 * @see template_preprocess_forums()
 * @see advanced_forum_preprocess_forums()
 */
?>

<?php
	global $user;
	if ($user->uid) {
		$links[] = array(
			'title' => 'New posts (all forums)',
			'href' => 'forum/newposts'
		);
	}
?>

<?php if ($forums_defined): ?>
<div id="forum">

  <?php if ($forum_description): ?>
  <div class="forum-description">
    <?php print $forum_description; ?>
  </div>
  <?php endif; ?>

	<div class="forum-top-links">
	
		<?php print theme('links', $links, array('class' => 'links forum-links')); ?></div>
	
		<?php if (isset($parents[1])) : // we're NOT at a container page -- but at a inner forum page?>
		
			<?php
				$subscribe_links = array();
				$subscribe_links[] = array(
					'html' => 1,
					'title' => '<img width="16" height="16" title="Subscribe (RSS): Initial topic posts (this forum)" alt="Subscribe (RSS): Initial topic posts (this forum)" src="'.base_path().'misc/feed.png" />',
					'href' => drupal_get_path_alias('taxonomy/term/'.arg(1).'/0/feed')
				);
				$subscribe_links[] = array(
					'html' => 1,
					'title' => '<img width="16" height="16" title="Subscribe (RSS): Topic replies (this forum)" alt="Subscribe (RSS): Topic replies (this forum)" src="'.base_path().'misc/feed.png" />',
					'href' => drupal_get_path_alias('crss/term/'.arg(1))
				);
			?>
			
		<?php elseif (arg(0) == 'forum' && arg(1) == '' && arg(2) == '') : // Main forum page ?>
			
			<?php
			
				// That's a tricky one -- gotta get any node of "forum" type to figure out what's the current state of subscription to all Forum Topics
				/*$res = db_result(db_query('SELECT n.nid FROM {node} n WHERE type = "forum" LIMIT 1'));
				$n = node_load($res);
				$forum_links = notifications_ui_link('node',$n); // getting actual links for all forum topics*/
			
				$subscribe_links = array();
				$subscribe_links[] = array(
					'html' => 1,
					'title' => '<img width="16" height="16" title="Subscribe (RSS): Initial topic posts (all forums)" alt="Subscribe (RSS): Initial topic posts (all forums)" src="'.base_path().'misc/feed.png" />',
					'href' => 'forum/feed'
				);
				$subscribe_links[] = array(
					'html' => 1,
					'title' => '<img width="16" height="16" title="Subscribe (RSS): Topic replies (all forums)" alt="Subscribe (RSS): Topic replies (all forums)" src="'.base_path().'misc/feed.png" />',
					'href' => drupal_get_path_alias('crss/vocab/1')
				);
				
				/*unset($links_array['notifications_2']); // removing subscriptions to "posts by xxx user"
				unset($links_array['notifications_0']); // removing subscriptions to This Post */
				
				/*if ($forum_links['notifications_1']) {
					$forum_links['notifications_1']['title'] = '<img src="'.base_path() . path_to_theme().'/images/icon_envelope.png" alt="'.strip_tags($forum_links['notifications_1']['title']).'" title="'.strip_tags($forum_links['notifications_1']['title']).'" />';;
					$subscribe_links[] = $forum_links['notifications_1'];
				}*/
			?>
			
		<?php endif ?>
		
		<?php print theme('links', $subscribe_links, array('class' => 'links forum-links subscriptions')); ?>
	
		<?php print $forums; ?>
		<?php print $topics; ?>
		
	</div>
<?php endif; ?>
