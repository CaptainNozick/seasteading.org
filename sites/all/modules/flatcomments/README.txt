Description
===========
Flatcomments is a very small module that allows you to force comments to always be replies to the node, not other comments.

node (story). add new comment link
 \- comment 1
 \- comment 2

Suppose user replies via the reply link on comment 1:

Without flatcomments:

node (story). add new comment link
 \- comment 1
    \- comment 3 
 \- comment 2

With flatcomments enabled for the node type story:

node (story). add new comment link
 \- comment 1
 \- comment 2
 \- comment 3


Configuration
=============
1. Copy the contents of the archive to your sites/all/modules directory, then enable 
   the module at Admininister >> Site building >> Modules (admin/build/modules).
2. Enable flatcomments for specific content types on the content types' edit form, reachable via
   edit links on Administer >> Content >> Content types (admin/content/types).


Remaining problems
==================
You will notice that when replying to a comment you'll see the content of the 'parent' comment, but as soon as you have previewed your comment you will see the content of the node.


Author
======
Heine Deelstra (http://drupal.org/user/17943)