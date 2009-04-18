When installing this module, it won't appear to do anything at first. This is
because you need to modify your theme in order to place the output where
you like.

The functions you need to call to get the data are:
user_titles_get_user_title($user); and
user_titles_get_user_image($user).
You can pass in a user id or a user object.
You may call these functions either in your
template.php or in your node.tpl.php and/or your comment.tpl.php.
It is recommended that you place this in your template.php.

In template.php, in the function _phptemplate_variables, place this
piece of code under case 'node': and a similar one under case 'comment' (see
below):

      if (module_exists('user_titles')) {
        $vars['user_title'] = user_titles_get_user_title($vars['node']->uid);
        $vars['user_title_image'] = user_titles_get_user_image($vars['node']->uid);
      }

Note that $vars may be named something else, such as $variables in your
implementation. Note the variable in the function definition.

If you do not have a _phptemplate_variables() in your template.php
file, you may create one:

function _phptemplate_variables($hook, $vars = array()) {
  switch ($hook) {
    case 'node':
      if (module_exists('user_titles')) {
        $vars['user_title'] = user_titles_get_user_title($vars['node']->uid);
        $vars['user_title_image'] = user_titles_get_user_image($vars['node']->uid);
      }
    case 'comment':
      if (module_exists('user_titles')) {
        $vars['user_title'] = user_titles_get_user_title($vars['comment']->uid);
        $vars['user_title_image'] = user_titles_get_user_image($vars['comment']->uid);
      }
  }
  return $vars;
}

In your node.tpl.php and/or your comment.tpl.php, choose where you would like
the user title to appear, and place this code:

  <?php if ($user_title): ?>
    <div class="user-title">
      <?php
        print $user_title;
        print $user_title_image ? $user_title_image : '';
      ?>
    </div>
  <?php endif; ?>

There are other places you may wish a user title to appear, such as the user
profile page. Please see the drupal.org theming handbook for more information
on theming those; the additions you make will be materially the same as 
these.

Also, if you just want the path to the image file to use in an img tag or something you can call:
user_titles_get_user_image_path($user)
which will return the path to the image of the title for that user.

