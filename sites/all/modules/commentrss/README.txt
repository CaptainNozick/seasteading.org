Drupal commentrss.module README.txt
==============================================================================

This module provides RSS feeds for comments. This is useful for allowing
readers to subscribe to comments for a blog post, article, or forum topic.
It currently provides 3 types of feeds, and an optional 4th type. Each type of
comment field may be disabled if unneeded.

  * complete site feed        /crss
  * per node feeds        eg. /crss/node/12
  * per term feeds        eg. /crss/term/13
  * per vocabulary feeds  eg. /crss/vocab/1 (requires vocabulary_list.module)

Comment feeds provide an alternative to email subscriptions, allowing users to
monitor discussions without having to provide their email address. Due to the
limited capabilities of RSS, threading is not preserved and the comments are
listed in reversed time order.


Installation
------------------------------------------------------------------------------
 
 Required:
  - Copy the module files to modules/
  - Enable the module as usual from Drupal's admin pages.
  
 Optional (if you would like to have nice vocabulary links in feeds):
  - Get vocabulary_list module and install as documented
 
Credits / Contact
------------------------------------------------------------------------------

This module was created by Gabor Hojtsy (goba[at]php.net). Moshe Weitzman
provided several fixes and improvements. Chris Cook is the current maintainer.

TODO
------------------------------------------------------------------------------

 - Either export a block with an XML/RSS button or try to hook into the
   syndication block provided by Drupal core or hook up with syndication
   module
