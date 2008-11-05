Menu Trails module implements an "active trail" in primary/secondary links, setting class="active" on parents to the current menu item.

It also allows nodes to be categorized by type or taxonomy as falling "under" existing menu items, triggering the above functionality.

To implement:

1) Install the module
2) Place the code-snippet below into your template.php file. You may want to excerpt the two lines which set $vars['primary_links'] and $vars['secondary_links'] and place them into a pre-existing _phptemplate_variables() funtion wherever page variables are set.
3) Visit admin/settings/menutrails to classify nodes
4) Enjoy!


CODE SNIPPET:
This is an example _phptemplate_variables() implementation. It will keep first level primary links open when secondary links are selected. 

You need to add this code (or something like it) to your template.php file for this module to do anything useful. 

function _phptemplate_variables($hook, $vars = array()) {
  switch ($hook) { // what function is active?
    case 'page': // page is where menu comes into play
      // set the primary links
      $vars['primary_links'] = menutrails_primary_links(1);
      // you may want to also override secondary_links
      $vars['secondary_links'] = menutrails_primary_links(2);
      break;
  }
}
