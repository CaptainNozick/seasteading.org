<?php
// $Id: template.php,v 1.4.2.1 2007/04/18 03:38:59 drumm Exp $

/**
 * Sets the body-tag class attribute.
 *
 * Adds 'sidebar-left', 'sidebar-right' or 'sidebars' classes as needed.
 */
function phptemplate_body_class($sidebar_left, $sidebar_right) {
	if ($sidebar_left != '' && $sidebar_right != '') {
		$class = 'sidebars';
	}
	else {
		if ($sidebar_left != '') {
			$class = 'sidebar-left';
		}
		if ($sidebar_right != '') {
			$class = 'sidebar-right';
		}
	}

	if (isset($class)) {
		print ' class="'. $class .'"';
	}
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param $breadcrumb
 *	 An array containing the breadcrumb links.
 * @return a string containing the breadcrumb output.
 */
function phptemplate_breadcrumb($breadcrumb) {
	
	if (!empty($breadcrumb)) {

		// We need a special BC for forum topics and blog
		if (arg(0) == 'node' || (arg(0) == 'comment' && arg(1) == 'reply') ) {
			
			if (arg(0) == 'node' && is_numeric(arg(1))) {
				$node = node_load(arg(1)); // let's load the current node
			} elseif (is_numeric(arg(2))) {
				$node = node_load(arg(2)); // commented node
			}
			
			if ($node->type == 'forum') { // is it a forum topic?
				$breadcrumb[] = l('Forums','forum');
				$par = array_keys($node->taxonomy); // ok, let's get the immediate parent term TID
				$par_tid = $par[0];
				$all_pars = taxonomy_get_parents_all($par_tid);
				$all_pars = array_reverse($all_pars);
				foreach($all_pars as $this_par) {
					$breadcrumb[] = l($this_par->name,drupal_get_path_alias('forum/'.$this_par->tid));
				}
				if (arg(0) == 'comment' && arg(1) == 'reply') $breadcrumb[] = l($node->title,drupal_get_path_alias('node/'.$node->nid));
			} 
			
		} elseif (arg(0) == 'blog') {
			
			$breadcrumb[] = l('Stay in Touch','stay-in-touch');
			
		} elseif (arg(0) == 'month') {
			
			$breadcrumb[] = l('Stay in Touch','stay-in-touch');
			$breadcrumb[] = l('Captain\'s Blog','stay-in-touch/blog');
			
		} elseif (arg(0) == 'taxonomy') {
			
			$this_term = taxonomy_get_term(arg(2));
			if ($this_term->vid == 2) {
				$breadcrumb[] = l('Stay in Touch','stay-in-touch');
				$breadcrumb[] = l('Captain\'s Blog Tags','stay-in-touch/blog');
			}
			
		} 
		return implode(' <span>&gt</span> ', $breadcrumb);
	}
}

/**
 * Allow themable wrapping of all comments.
 */
function phptemplate_comment_wrapper($content, $type = null, $n = null) {
	static $node_type;
	static $node;

	if (isset($type)) $node_type = $type;
	if (isset($n)) $node = $n;

	if ($node_type == 'forum' && module_exists('advanced_forum')) {
		$variables = array();
		$variables['node'] = $node;
		$variables['content'] = $content;
		advanced_forum_preprocess_comment_wrapper($variables);
		$forum_style = advanced_forum_get_current_style();
		return _phptemplate_callback('advf-comment-wrapper', $variables, array("$forum_style/advf-comment-wrapper")); 
	}

	if (!$content || $node_type == 'forum') {
		return '<div id="comments">'. $content . '</div>';
	} else {
		return '<div id="comments"><h2 class="comments">'. t('Comments') .'</h2>'. $content .'</div>';
	}
}

/**
 * Override or insert PHPTemplate variables into the templates.
 */
function _phptemplate_variables($hook, $vars) {

  switch ($hook) {
  /*
    case 'node':
      if (module_exists('user_titles')) {
        $vars['user_title'] = user_titles_get_user_title($vars['node']->uid);
        $vars['user_title_image'] = user_titles_get_user_image($vars['node']->uid);
      }
   */
    case 'comment':
      if (module_exists('user_titles')) {
        $vars['user_title'] = user_titles_get_user_title($vars['comment']->uid);
        $vars['user_title_image'] = user_titles_get_user_image($vars['comment']->uid);
      }
  }

	if (module_exists('advanced_forum')) {
		$vars = advanced_forum_addvars($hook, $vars);
		return $vars;
  } 


	if ($hook == 'page') {

		if ($secondary = menu_secondary_local_tasks()) {
			$output = '<span class="clear"></span>';
			$output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
			$vars['tabs2'] = $output;
		}

		// Hook into color.module
		if (module_exists('color')) {
			_color_page_alter($vars);
		}
		return $vars;
	}
	
	return array();
}

/**
 * Returns the rendered local tasks. The default implementation renders
 * them as tabs.
 *
 * @ingroup themeable
 */
function phptemplate_menu_local_tasks() {
	$output = '';

	if ($primary = menu_primary_local_tasks()) {
		$output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
	}

	return $output;
}


/**************************************************************************************************************************************************
 *
 *	CUSTOM TWEAKS
 *	
**************************************************************************************************************************************************/

function seastead_regions() {
	return array(
		'sidebar_left' => t('left sidebar'),
		'sidebar_right' => t('right sidebar'),
		'banner_blocks' => t('banner blocks'),
		'center_left' => t('center left at home'),
		'center_right' => t('center right at home'),
		'header' => t('header')
	);
}


/**
* Implementation of theme_menu_item().
*
* Add active class to current menu item links.
*/
function phptemplate_menu_item($mid, $children = '', $leaf = TRUE) {
	$item = menu_get_item($mid); // get current menu item
	$class = 'menu-'. str_replace(' ', '-', strtolower(str_replace('?','',$item['title'])));
	$active_class = in_array($mid, _menu_get_active_trail()) ? ' active-trail' : '';
	// decide whether to add the active class to this menu item
	if ((drupal_get_normal_path($item['path']) == $_GET['q']) // if menu item path...
	|| (drupal_is_front_page() && $item['path'] == '<front>')	// or front page...
	|| (in_array($mid, _menu_get_active_trail()))) {
		$active_class = ' active'; // set active class
	} else { // otherwise...
		$active_class = ''; // do nothing
	}
	return '<li class="'. ($leaf ? 'leaf' : ($children ? 'expanded' : 'collapsed')) . $active_class .' '.$class.'">'. menu_item_link($mid) . $children ."</li>\n";
}

/* Customizing Views' HTML code */

function phptemplate_views_view($view, $type, $nodes, $level = NULL, $args = NULL) {
	$num_nodes = count($nodes);

	if ($type == 'page') {
		drupal_set_title(filter_xss_admin(views_get_title($view, 'page')));
		views_set_breadcrumb($view);
	}

	if ($num_nodes) {
		$output .= views_get_textarea($view, $type, 'header');
	}

	if ($type != 'block' && $view->exposed_filter) {
		$output .= views_theme('views_display_filters', $view);
	}

	$plugins = _views_get_style_plugins();
	$view_type = ($type == 'block') ? $view->block_type : $view->page_type;
	if ($num_nodes || $plugins[$view_type]['even_empty']) {
		if ($level !== NULL) {
			$output .= views_theme($plugins[$view_type]['summary_theme'], $view, $type, $level, $nodes, $args);
		}
		else {
			$output .= views_theme($plugins[$view_type]['theme'], $view, $nodes, $type);
		}
		$output .= views_get_textarea($view, $type, 'footer');

		if ($type == 'block' && $view->block_more && $num_nodes >= $view->nodes_per_block) {
			$output .= theme('views_more', $view->real_url);
		}
	}
	else {
		$output .= views_get_textarea($view, $type, 'empty');
	}

	if ($view->use_pager) {
		$output .= theme('pager', '', $view->pager_limit, $view->use_pager - 1);
	}

	return $output;
}

/*
	Override Press Releases default RSS view -- because Patri wanted to have full text in this feed
*/

function phptemplate_views_rss_feed_press_releases_rss($view, $nodes, $type) {
	if ($type == 'block') {
		return;
	}
	global $base_url;

	$channel = array(
		// a check_plain isn't required on these because format_rss_channel
		// already does this.
		'title'			 => views_get_title($view, 'page'),
		'link'				=> url($view->feed_url ? $view->feed_url : $view->real_url, NULL, NULL, true),
		'description' => $view->description,
	);

	$item_length = 'fulltext';
	$namespaces = array('xmlns:dc="http://purl.org/dc/elements/1.1/"');

	// Except for the original being a while and this being a foreach, this is
	// completely cut & pasted from node.module.
	foreach ($nodes as $node) {
		// Load the specified node:
		$item = node_load($node->nid);
		$link = url("node/$node->nid", NULL, NULL, 1);

		if ($item_length != 'title') {
			$teaser = ($item_length == 'teaser') ? TRUE : FALSE;

			// Filter and prepare node teaser
			if (node_hook($item, 'view')) {
				node_invoke($item, 'view', $teaser, FALSE);
			}
			else {
				$item = node_prepare($item, $teaser);
			}

			// Allow modules to change $node->teaser before viewing.
			node_invoke_nodeapi($item, 'view', $teaser, FALSE);
		}

		// Allow modules to add additional item fields
		$extra = node_invoke_nodeapi($item, 'rss item');
		$extra = array_merge($extra, array(array('key' => 'pubDate', 'value' =>	date('r', $item->created)), array('key' => 'dc:creator', 'value' => $item->name), array('key' => 'guid', 'value' => $item->nid . ' at ' . $base_url, 'attributes' => array('isPermaLink' => 'false'))));
		foreach ($extra as $element) {
			if ($element['namespace']) {
				$namespaces = array_merge($namespaces, $element['namespace']);
			}
		}
		
		// Prepare the item description
		switch ($item_length) {
			case 'fulltext':
				$item_text = $item->body;
				break;
			case 'teaser':
				$item_text = $item->teaser;
				if ($item->readmore) {
					$item_text .= '<p>'. l(t('read more'), 'node/'. $item->nid, NULL, NULL, NULL, TRUE) .'</p>';
				}
				break;
			case 'title':
				$item_text = '';
				break;
		}

		$items .= format_rss_item($item->title, $link, $item_text, $extra);
	}

	$channel_defaults = array(
		'version'		 => '2.0',
		'title'			 => variable_get('site_name', 'drupal') .' - '. variable_get('site_slogan', ''),
		'link'				=> $base_url,
		'description' => variable_get('site_mission', ''),
		'language'		=> $GLOBALS['locale'],
	);
	$channel = array_merge($channel_defaults, $channel);

	$output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	$output .= "<rss version=\"". $channel["version"] . "\" xml:base=\"". $base_url ."\" ". implode(' ', $namespaces) .">\n";
	$output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
	$output .= "</rss>\n";

	drupal_set_header('Content-Type: text/xml; charset=utf-8');
	print $output;
	module_invoke_all('exit');
	exit; 
}


/*
	Override Recent Blog Posts default RSS view -- because Patri wanted to have full text in this feed
*/

function phptemplate_views_rss_feed_recent_blog_posts_rss($view, $nodes, $type) {
	if ($type == 'block') {
		return;
	}
	global $base_url;

	$channel = array(
		// a check_plain isn't required on these because format_rss_channel
		// already does this.
		'title'			 => views_get_title($view, 'page'),
		'link'				=> url($view->feed_url ? $view->feed_url : $view->real_url, NULL, NULL, true),
		'description' => $view->description,
	);

	$item_length = 'fulltext';
	$namespaces = array('xmlns:dc="http://purl.org/dc/elements/1.1/"');

	// Except for the original being a while and this being a foreach, this is
	// completely cut & pasted from node.module.
	foreach ($nodes as $node) {
		// Load the specified node:
		$item = node_load($node->nid);
		$link = url("node/$node->nid", NULL, NULL, 1);

		if ($item_length != 'title') {
			$teaser = ($item_length == 'teaser') ? TRUE : FALSE;

			// Filter and prepare node teaser
			if (node_hook($item, 'view')) {
				node_invoke($item, 'view', $teaser, FALSE);
			}
			else {
				$item = node_prepare($item, $teaser);
			}

			// Allow modules to change $node->teaser before viewing.
			node_invoke_nodeapi($item, 'view', $teaser, FALSE);
		}

		// Allow modules to add additional item fields
		$extra = node_invoke_nodeapi($item, 'rss item');
		$extra = array_merge($extra, array(array('key' => 'pubDate', 'value' =>	date('r', $item->created)), array('key' => 'dc:creator', 'value' => $item->name), array('key' => 'guid', 'value' => $item->nid . ' at ' . $base_url, 'attributes' => array('isPermaLink' => 'false'))));
		foreach ($extra as $element) {
			if ($element['namespace']) {
				$namespaces = array_merge($namespaces, $element['namespace']);
			}
		}
		
		// Prepare the item description
		switch ($item_length) {
			case 'fulltext':
				$item_text = $item->body;
				break;
			case 'teaser':
				$item_text = $item->teaser;
				if ($item->readmore) {
					$item_text .= '<p>'. l(t('read more'), 'node/'. $item->nid, NULL, NULL, NULL, TRUE) .'</p>';
				}
				break;
			case 'title':
				$item_text = '';
				break;
		}

		$items .= format_rss_item($item->title, $link, $item_text, $extra);
	}

	$channel_defaults = array(
		'version'		 => '2.0',
		'title'			 => variable_get('site_name', 'drupal') .' - '. variable_get('site_slogan', ''),
		'link'				=> $base_url,
		'description' => variable_get('site_mission', ''),
		'language'		=> $GLOBALS['locale'],
	);
	$channel = array_merge($channel_defaults, $channel);

	$output = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	$output .= "<rss version=\"". $channel["version"] . "\" xml:base=\"". $base_url ."\" ". implode(' ', $namespaces) .">\n";
	$output .= format_rss_channel($channel['title'], $channel['link'], $channel['description'], $items, $channel['language']);
	$output .= "</rss>\n";

	drupal_set_header('Content-Type: text/xml; charset=utf-8');
	print $output;
	module_invoke_all('exit');
	exit; 
}

/*
	Override forumTracker display
*/

function phptemplate_views_view_table_forumTracker($view, $nodes, $type) {
	$fields = _views_get_fields();
	$comments_per_page = _comment_get_display_setting('comments_per_page');

	foreach ($nodes as $node) {
		$row = array();
		foreach ($view->field as $field) {
			if ($fields[$field['id']]['visible'] !== FALSE) {
				if($field['field'] == 'comment_count') {
					$count = db_result(db_query('SELECT COUNT(c.cid) FROM {comments} c WHERE c.nid=%d', $node->nid));
					// Find the ending point. The pager URL is always 1 less than
					// the number being displayed because the first page is 0.
					$last_display_page = ceil($count / $comments_per_page);

					$last_pager_page = $last_display_page - 1;
					if ($last_pager_page < 0) $last_pager_page = 0;

					$cell['data'] = $node->node_comment_statistics_comment_count.'<br />';
					//$cell['data'] .= l('new', 'node/'.$node->nid, array(), 'page='.$last_pager_page.'#new');
					$cell['data'] .= advanced_forum_first_new_post_link($node, $count);
					$cell['class'] = "view-field ". views_css_safe('view-field-'. $field['queryname']);
					$row[] = $cell;
				} else {
					$cell['data'] = views_theme_field('views_handle_field', $field['queryname'], $fields, $field, $node, $view);
					$cell['class'] = "view-field ". views_css_safe('view-field-'. $field['queryname']);
					$row[] = $cell;
				}
			}
		}
		$rows[] = $row;
	}
	return theme('table', $view->table_header, $rows);
}


/*
	Override default forum display
*/

/*

function seasteading_forum_display($forums, $topics, $parents, $tid, $sortby, $forum_per_page) {
	global $user;
	// forum list, topics list, topic browser and 'add new topic' link

	$vocabulary = taxonomy_get_vocabulary(variable_get('forum_nav_vocabulary', ''));
	$title = $vocabulary->name;

	// Breadcrumb navigation:
	$breadcrumb = array();
	if ($tid) {
		$breadcrumb[] = array('path' => 'forum', 'title' => $title);
	}

	if ($parents) {
		$parents = array_reverse($parents);
		foreach ($parents as $p) {
			if ($p->tid == $tid) {
				$title = $p->name;
			}
			else {
				$breadcrumb[] = array('path' => 'forum/'. $p->tid, 'title' => $p->name);
			}
		}
	}

	drupal_set_title(check_plain($title));

	$breadcrumb[] = array('path' => $_GET['q']);
	menu_set_location($breadcrumb);

	if (count($forums) || count($parents)) {
		$output = '<div id="forum">';
		if (isset($parents[1])) { // we're NOT at a container page
			$output .= l('<img width="16" height="16" title="Syndicate &ldquo;'.$title.'&rdquo; forum" alt="Syndicate &ldquo;'.$title.'&rdquo; forum" src="'.base_path().'misc/feed.png" />', drupal_get_path_alias('taxonomy/term/'.arg(1).'/0/feed'), array('class' => 'rss'), NULL, NULL, FALSE, TRUE);
		} elseif (arg(0) == 'forum' && arg(1) == '' && arg(2) == '') {
			$output .= l('<img width="16" height="16" title="Syndicate latest forum topics" alt="Syndicate latest forum topics" src="'.base_path().'misc/feed.png" />', 'forum/feed', array('class' => 'rss'), NULL, NULL, FALSE, TRUE);
		}
		$output .= '<ul>';

		if (user_access('create forum topics')) {
			$output .= '<li>'. l(t('Post new forum topic.'), "node/add/forum/$tid") .'</li>';
		}
		else if ($user->uid) {
			$output .= '<li>'. t('You are not allowed to post a new forum topic.') .'</li>';
		}
		else {
			$output .= '<li>'. t('<a href="@login">Login</a> to post a new forum topic.', array('@login' => url('user/login', drupal_get_destination()))) .'</li>';
		}
		$output .= '</ul>';

		$output .= theme('forum_list', $forums, $parents, $tid);

		if ($tid && !in_array($tid, variable_get('forum_containers', array()))) {
			$output .= theme('forum_topic_list', $tid, $topics, $sortby, $forum_per_page);
			drupal_add_feed(url('taxonomy/term/'. $tid .'/0/feed'), 'RSS - '. $title);
		}
		$output .= '</div>';
	}
	else {
		drupal_set_title(t('No forums defined'));
		$output = '';
	}

	return $output;
}

*/
