/* $Id: README.txt,v 1.12.2.1 2007/12/02 05:28:00 sun Exp $ */

-- SUMMARY --

Drupal Administration Menu displays the whole menu tree below /admin including
all invisible local tasks in a drop-down menu. So administrators need less time
to access pages which are only visible after one or two clicks normally.

Admin menu also provides hook_admin_menu() that allows other modules to add or
alter menu items.

For a full description visit the project page:
  http://drupal.org/project/admin_menu
Bug reports, feature suggestions and latest developments:
  http://drupal.org/project/admin_menu/issues


-- REQUIREMENTS --

None.


-- INSTALLATION --

* Copy admin_menu module to your modules directory and enable it on the admin
  modules page.


-- CONFIGURATION --

* Go to Administer -> User management -> Access control and assign permissions
  for Drupal Administration Menu:

  - access administration menu: Displays Drupal Administration Menu.

  - display drupal links: Displays additional links to Drupal.org and issue
    queues of all enabled contrib modules in the Drupal Administration Menu icon.


-- CUSTOMIZATION --

* You have two options to override the admin menu icon:
  
  1) Disable it via CSS in your theme:

     body #admin_menu-icon { display: none; }

  2) Alter the image by overriding the theme function:

     Copy the whole function theme_admin_menu_icon() into your template.php,
     rename it to f.e. phptemplate_admin_menu_icon() and customize the output
     according to your needs.


-- TROUBLESHOOTING --

* If admin menu is not displayed, check the following steps:

  - Is the 'access administration menu' permission enabled?

  - Does your theme output $closure? (See FAQ below for more info)

* If your theme uses absolute or fixed positioned elements, and the default
  margin-top for <BODY> is not sufficient, you need to override admin menu's
  stylesheet in your theme.

* If admin menu is rendered behind a flash movie object, you need to add the
  following property to your flash object(s):
<code>
<param name="wmode" value="transparent" />
</code>
  See http://drupal.org/node/195386 for further information.


-- FAQ --

Q: After upgrading to 5.x-2.x, admin_menu disappeared. Why?

A: This should not happen. If it did, visit
   http://<yoursitename>/admin/build/menu to re-generate your menu cache.

Q: After upgrading, admin_menu disappeared. Why?

A: Prior to release 5.x-1.2, Drupal Administration Menu was output in a block.
   Since 5.x-1.2, it is output via hook_footer(). Some custom themes may not
   (yet) output $closure, so admin_menu could no longer be displayed. If you
   decided to move the 'administer' tree into a new menu and disabled that menu
   block, a site could become (temporarily) unmaintainable. Either way, you
   should fix your theme by adding the following code in front of the closing
   HTML (</html>) tag:
<code>
   <?php echo $closure; ?>
</code>

Q: After upgrading, the menu item 'administer' is no longer removed. Why?

A: Prior to release 5.x-1.2, Drupal Administration Menu was output via
   hook_block(), which allowed to alter the global menu array. Since 5.x-1.2, it
   is output via hook_footer() and thus no longer able to alter the menu. As
   long as there will be no built-in solution in an upcoming release, you may
   perform the following steps as a workaround:
   - Create a new menu.
   - Edit the menu item 'administer' and select the new menu as parent.

Q: I enabled "Aggregate and compress CSS files", but I found admin_menu.css is
   still there, is it normal?

A: Yes, this is the intended behavior. Since admin_menu is only visible for
   logged-on administrative users, it would not make sense to load its
   stylesheet for all, including anonymous users.


-- CONTACT --

Current maintainers:
* Daniel F. Kudwien (sun) - dev@unleashedmind.com
* Stefan M. Kudwien (smk-ka) - dev@unleashedmind.com

This project has been sponsored by:
* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

