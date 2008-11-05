<?php
// $Id: admin_menu.inc,v 1.1.4.4 2007/12/02 09:54:28 sun Exp $


/**
 * Build the administration menu tree.
 *
 * This is only called if the menu has been modified, i.e. the menu cache has
 * been cleared.
 *
 * @param int $mid_admin
 *   The menu item id to use for the administration menu.
 */
function admin_menu_build($mid_admin) {
  global $_menu;
  $_admin_menu = array();
  
  // Temporary access permission fix for root menu items until #126621 is
  // committed to Drupal core.
  foreach ($_menu['items'][$mid_admin]['children'] as $key => $parent) {
    if ($_menu['items'][$parent]['access'] != FALSE || !isset($_menu['items'][$parent]['children'])) {
      continue;
    }
    $is_accessible = FALSE;
    foreach ($_menu['items'][$parent]['children'] as $key => $child) {
      if ($_menu['items'][$child]['access'] == TRUE) {
        $is_accessible = TRUE;
      }
    }
    if ($is_accessible) {
      $_menu['items'][$parent]['access'] = TRUE;
    }
  }
  
  // Copy admin menu items into a new menu tree.
  $_admin_menu['index'] = $_menu['path index'];
  $_admin_menu[$mid_admin] = $_menu['items'][$mid_admin];
  _admin_menu_get_children($_admin_menu, $_admin_menu[$mid_admin]);
  
  // Adjust some menu items for better user experience.
  admin_menu_adjust_items($_admin_menu);
  
  // Allow other modules to integrate with admin_menu.
  foreach (module_implements('admin_menu') as $module) {
    $function = $module .'_admin_menu';
    $function($_admin_menu, TRUE);
  }
  
  return $_admin_menu;
}

/**
 * Recursively adds items to the administration menu.
 *
 * Any accessible menu items are added, including local tasks.
 *
 * @param array $_admin_menu
 *   An array containing the complete administration menu structure, passed by
 *   reference.
 * @param array $item
 *   The (starting) menu item to use for the administration menu.
 */
function _admin_menu_get_children(&$_admin_menu, &$item) {
  global $_menu;
  
  if (isset($item['children'])) {
    foreach ($item['children'] as $child) {
      // Check access permissions.
      if (!variable_get('admin_menu_show_all', 0)) {
        $item_is_accessible = !isset($_menu['items'][$child]['access']) || (isset($_menu['items'][$child]['access']) && $_menu['items'][$child]['access']);
      }
      else {
        $item_is_accessible = TRUE;
      }
      // Check menu item type.
      $item_is_visible = $_menu['items'][$child]['type'] & (MENU_VISIBLE_IN_TREE | MENU_IS_LOCAL_TASK);
      
      // Create the child item if it is accessible and visible.
      // Additional condition to hide items linking to parent:
      //   && !($_menu['items'][$child]['type'] & MENU_LINKS_TO_PARENT)
      if ($item_is_accessible && $item_is_visible) {
        $_admin_menu[$child] = $_menu['items'][$child];
        
        // Recurse to child menu items.
        if (isset($_menu['items'][$child]['children'])) {
          _admin_menu_get_children($_admin_menu, $_admin_menu[$child]);
        }
        
        // Remove this child item if it is visible in the navigation.
        unset($_menu['visible'][$child]);
      }
      else {
        // Remove child menu item from parent's children list.
        $parent_id = $_menu['items'][$child]['pid'];
        if (isset($_menu['items'][$parent_id]['children']) && is_array($_menu['items'][$parent_id]['children'])) {
          $child_key = array_search($child, $_menu['items'][$parent_id]['children']);
          if ($child_key !== FALSE) {
            unset($_admin_menu[$parent_id]['children'][$child_key]);
          }
        }
      }
    }
  }
}

/**
 * Add some hard-coded features for better user experience.
 *
 * @param array $_admin_menu
 *   An array containing the complete administration menu structure, passed by
 *   reference.
 */
function admin_menu_adjust_items(&$_admin_menu) {
  global $_menu, $user;
  
  // Add the icon containing special links.
  $mid_admin = $_admin_menu['index']['admin'];
  $mid_icon = admin_menu_add_item($_admin_menu, $mid_admin, array(
    'title' => theme('admin_menu_icon'),
    'path' => drupal_get_normal_path(variable_get('site_frontpage', 'node')),
    'weight' => -100,
    'class' => 'admin_menu-icon',
  ));
  
  // Add 'administer' item to the icon menu.
  $mid_icon_admin = admin_menu_add_item($_admin_menu, $mid_icon, array('title' => t('Administer'), 'path' => 'admin', 'weight' => 10));
  // Reset 'administer' menu item id in path index.
  $_admin_menu['index']['admin'] = $mid_admin;
  
  // Move 'By module' item if accessible, or remove it.
  $mid_bymodule = $_menu['path index']['admin/by-module'];
  if (user_access('administer site configuration')) {
    admin_menu_move_item($_admin_menu, $mid_bymodule, $mid_icon_admin);
  }
  else {
    admin_menu_remove_item($_admin_menu, 'admin/by-module');
  }
  
  // Remove 'By task' menu item.
  admin_menu_remove_item($_admin_menu, 'admin/by-task');
  
  // Add system update links.
  admin_menu_add_item($_admin_menu, $mid_icon, array('title' => t('Run cron'), 'path' => 'admin/logs/status/run-cron', 'weight' => 50));
  if ($user->uid == 1) {
    admin_menu_add_item($_admin_menu, $mid_icon, array('title' => t('Run updates'), 'path' => 'update.php', 'weight' => 50));
  }
  
  // Add links to drupal.org.
  if (user_access('display drupal links')) {
    $mid_drupal = admin_menu_add_item($_admin_menu, $mid_icon, array('title' => t('Drupal.org'), 'path' => 'http://drupal.org', 'weight' => 100));
    admin_menu_add_item($_admin_menu, $mid_drupal, array('title' => t('Drupal issue queue'), 'path' => 'http://drupal.org/project/issues/drupal'));
    
    // Add links to project issue queues.
    foreach (module_list(FALSE, FALSE, TRUE) as $module) {
      $info = _module_parse_info_file(drupal_get_path('module', $module) .'/'. $module .'.info');
      if (isset($info['project']) && $info['project'] == 'drupal') {
        continue;
      }
      $url = 'http://drupal.org/project/issues/'. $module;
      // See http://drupal.org/node/97569
      // $url .= !empty($info['version']) ? '/'. $info['version'] : '';
      admin_menu_add_item($_admin_menu, $mid_drupal, array('title' => t('@title issue queue', array('@title' => $info['name'])), 'path' => $url));
    }
  }
  
  // Add logout item.
  if ($user->uid > 0) {
    $mid_logout = $_menu['path index']['logout'];
    admin_menu_add_item($_admin_menu, $mid_admin, array(
      'title' => t('Logout @name', array('@name' => $user->name)),
      'path' => $_menu['items'][$mid_logout]['path'],
      'weight' => -100,
      'class' => 'admin_menu-action admin_menu-logout',
    ));
  }
  
  // Add 'Create <content_type>' menu items to Content management > Content.
  $mid_node_add = $_menu['path index']['node/add'];
  $mid_content = $_menu['path index']['admin/content/node'];
  admin_menu_copy_items($_admin_menu, $mid_node_add, $mid_content, t('Add !title'));
}

/**
 * Recursively copy menu items from a source parent menu item to a target item.
 *
 * @param array $_admin_menu
 *   An array containing the complete administration menu structure, passed by
 *   reference.
 * @param int $source_pid
 *   A source parent menu item id from which children shall be copied.
 * @param int $target_pid
 *   A target parent menu item id.
 * @param string $title
 *   An optional string containing the token !title, that has already been
 *   passed through t(), which will be used to dynamically replace previous
 *   menu item titles.
 * @param bool $tree
 *   Whether to rebuild the complete hierarchy from the source parent menu item
 *   or copy menu items flattened. Defaults to TRUE.
 */
function admin_menu_copy_items(&$_admin_menu, $source_pid, $target_pid, $title = NULL, $tree = TRUE) {
  global $_menu;
  
  if (isset($_menu['items'][$source_pid]['children']) && isset($_admin_menu[$target_pid])) {
    foreach ($_menu['items'][$source_pid]['children'] as $mid) {
      $item = $_menu['items'][$mid];
      if (isset($title)) {
        $item['title'] = check_plain(strtr($title, array('!title' => $_menu['items'][$mid]['title'])));
      }
      // Only add child to target if it does not already exist.
      if (!in_array($mid, $_admin_menu[$target_pid]['children'])) {
        admin_menu_add_item($_admin_menu, $target_pid, $item);
      }
      
      // Recurse into children.
      if (isset($_menu['items'][$mid]['children']) && count($_menu['items'][$mid]['children'])) {
        if ($tree) {
          admin_menu_copy_items($_admin_menu, $mid, $mid, $title);
        }
        else {
          admin_menu_copy_items($_admin_menu, $mid, $target_pid, $title, FALSE);
          unset($_admin_menu[$mid]['children']);
          // Note: Uncomment following lines to remove unnecessary parent items.
          // unset($_admin_menu[$target_pid]['children'][array_search($mid, $_admin_menu[$target_pid]['children'])]);
          // unset($_admin_menu[$mid]);
        }
      }
    }
  }
}

/**
 * Moves the child pointer of a menu item to a new parent.
 *
 * @param array $_admin_menu
 *   An array containing the complete administration menu structure, passed by
 *   reference.
 * @param int $mid
 *   The menu item id of the item to move.
 * @param int $pid
 *   The menu item id of the new parent.
 */
function admin_menu_move_item(&$_admin_menu, $mid, $pid) {
  global $_menu;
  
  if (isset($_admin_menu[$mid]) && isset($_admin_menu[$pid])) {
    // Remove current child pointer.
    admin_menu_remove_item($_admin_menu, $_admin_menu[$mid]['path']);
    
    // Insert new child pointer.
    $_admin_menu[$mid]['pid'] = $pid;
    $_admin_menu[$pid]['children'][] = $mid;
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Removes the child pointer for a menu item.
 *
 * @param array $_admin_menu
 *   An array containing the complete administration menu structure, passed by
 *   reference.
 * @param string $path
 *   The menu path to remove, e.g. 'foo/bar'.
 */
function admin_menu_remove_item(&$_admin_menu, $path) {
  global $_menu;
  
  $mid = $_menu['path index'][$path];
  if (isset($_admin_menu[$mid])) {
    $pid = $_admin_menu[$mid]['pid'];
    $child_key = array_search($mid, $_admin_menu[$pid]['children']);
    if ($child_key !== FALSE) {
      unset($_admin_menu[$pid]['children'][$child_key]);
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  return FALSE;
}

/**
 * Render an icon to display in the Administration Menu.
 *
 * @ingroup themeable
 */
function theme_admin_menu_icon() {
  return '<img class="admin_menu-icon" src="'. check_url(base_path() .'misc/favicon.ico') .'" width="16" height="16" alt="" />';
}

