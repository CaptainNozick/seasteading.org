------------------------------------------------------------------------------
  User Badges Module for Drupal
    developed by: Jeff Robbins | jjeff | robbins (att) jjeff (d0t) com
------------------------------------------------------------------------------

Description
===========
The User Badges module allows each user to be assigned 'badges', displayed as
a series of iconic images. A common use will probably be to display the badges
along with the user's information within forums, comments, or node postings.
These badges can be assigned individually by the administrator or attached to
a role so that, for example, all users in the 'admin' role will show the
'Administrator' badge which could display as any graphic.

Any badge can optionally be associated to a URL that links the image to a
description page. If a user has more than one badge, there is also a mechanism
that allows administrators to only show the most highest-level badge.
You can also set a special badge for blocked users and even override their 
role badges with this one.

User Badges can be used as a way to establish trust (in the same way as eBay's
star graphics) or as an incentive for users. They can also be a quick way to
identify moderators, administrators, or anyone with a given role.


Limitation
==========

User Badges is not compatible with the "private" file download method.


Dependencies
============
In order to upload images via the user interface, user badges depends on:

* Upload


Configuration
=============
Once the module is activated, go to Administer >> User management >> Badges
(admin/user/user_badges). Here you'll find three tabbed sections for User Badges.

* Edit user badges - allows you to add and edit badges.
* Images - where you can upload or delete images to associate with each
  badge.
* Roles - where you can associate a badge with a given role so that all users
  with that role will display the selected badge.
* Settings - where you can set the global settings of the badges on your site

Users with the 'manage badges' access permission will be shown a 'Badges' tab
when visiting user profiles. This is the place to manually assign badges to
users.


Display badges
==============
To display user badges in your theme use :

<?php print user_badges_for_uid($uid); ?>

Note: $uid means 'a user id' here, not the literal use of $uid. Most people will use
the uid of the node ($node->uid) or comment ($comment->uid) author.


Former maintainers
==================
* Jeff Robbins (http://drupal.org/user/17190)
* Chad Phillips (http://drupal.org/user/22079)
* Heine Deelstra (http://drupal.org/user/17943)


Current maintainer
===================
Nuno Veloso (http://drupal.org/user/80656)
