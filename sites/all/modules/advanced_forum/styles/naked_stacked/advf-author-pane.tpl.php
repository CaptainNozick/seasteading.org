<?php
// $Id: advf-author-pane.tpl.php,v 1.7 2008/11/18 02:30:08 michellec Exp $

/**
 * @file advf-forum-user.tpl.php
 * Theme implementation to display information about the posting user.
 *
 * Available variables (core modules):
 * - $account: The entire user object.
 * - $picture: Themed user picture.
 * - $account_name: Themed user name.
 * - $account_id: User ID number.
 *
 * - $joined: Date the user joined the site.
 * - $joined_ago: Time since the user registered in the format "TIME ago"
 *
 * - $online_icon: Online/Offline icon.
 * - $online_status: Translated text "Online" or "Offline"
 *
 * - $contact: Linked icon.
 * - $contact_link: Linked translated text "Contact user".
 *
 * - $profile - Profile object from core profile.
 *     Usage: $profile['category']['field_name']['value']
 *     Example: <?php print $profile['Personal info']['profile_name']['value']; ?>
 *
 * Available variables (contributed modules):
 * - $facebook_status: Status from the facebook status module.
 *
 * - $privatemsg: Linked icon.
 * - $privatemsg_link: Linked translated text "Send PM".
 *
 * - $user_stats_posts: Number of posts from user stats module.
 * - $user_stats_ip: IP address from user stats module.
 *
 * - $userpoints_points: Number of points from default category of the userpoints module.
 * - $userpoints_categories: Array holding each category and the points for that category.
 *
 * 5.x only at this time:
 *
 * - $buddylist: Linked icon.
 * - $buddylist_link: Linked translated text "Add to buddylist" or "Remove from buddylist".
 *
 * - $subscribe: Formatted link to subscribe to the author's forum topics.
 * - $subscribe_link: As above but just the relative path.
 *
 * - $user_title: Title from user titles module.
 *
 * - $user_badges: Badges from user badges module.

*/
?>
<div class="author-pane-inner">

  <div class="author-pane-first">

    <?php if (!empty($picture)): ?>
      <?php print $picture; ?>
    <?php endif; ?>

    <div class="author-pane-name-section">
      <div class="author-pane-line author-name"> <?php print $account_name; ?> </div>

      <?php if (!empty($user_title)): ?>
        <div class="author-pane-line author-title"> <?php print $user_title; ?> </div>
      <?php endif; ?>

      <?php if (!empty($user_badges)): ?>
        <div class="author-pane-line author-badges"> <?php print $user_badges;  ?> </div>
      <?php endif; ?>
    </div>
  </div>

  <div class="author-pane-last">

    <?php if (!empty($joined)): ?> 
      <div class="author-pane-line author-joined">
        <span class="author-pane-label"><?php print t('Joined'); ?>:</span> <?php print $joined; ?>
      </div>
    <?php endif; ?>

    <?php if (isset($user_stats_posts)): ?>
      <div class="author-pane-line author-posts">
        <span class="author-pane-label"><?php print t('Posts'); ?>:</span> <?php print $user_stats_posts; ?>
      </div>
    <?php endif; ?>

    <?php if (isset($userpoints_points)): ?>
      <div class="author-pane-line author-points">
        <span class="author-pane-label"><?php print t('!Points: ', userpoints_translation()); ?></span> <?php print $userpoints_points; ?>
      </div>
    <?php endif; ?>

    <?php if (!empty($user_stats_ip)): ?>
      <div class="author-pane-line author-ip">
        <span class="author-pane-label"><?php print t('IP'); ?>:</span> <?php print $user_stats_ip; ?>
      </div>
    <?php endif; ?>

    <div class="author-pane-icon"><?php print $online_icon; ?></div>

    <?php if (!empty($contact)): ?>
      <div class="author-pane-icon"><?php print $contact; ?></div>
    <?php endif; ?>

    <?php if (!empty($privatemsg)): ?>
      <div class="author-pane-icon"><?php print $privatemsg; ?></div>
    <?php endif; ?>

    <?php if (!empty($buddylist)): ?>
      <div class="author-pane-icon"><?php print $buddylist; ?></div>
    <?php endif; ?>

  </div>

</div>
