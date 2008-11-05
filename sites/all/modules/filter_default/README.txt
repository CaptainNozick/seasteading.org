Filter Default module
$Id: README.txt,v 1.2 2007/09/12 20:54:11 bjaspan Exp $

PREREQUISITES

- Drupal 4.7 or 5.

INSTALLATION

Install and activate the Filter Default like every other Drupal
module.

DESCRIPTION

The Filter Default module allows you to assign a default input format
for new nodes and comments for each role on your site.  It adds a
'Defaults' tab to the Administer > Site configuration > Input formats
page on which you can manage the assignments.

Whenever a new node or comment is being created, each text input box
with multiple allowed input formats is initially configured based on
the settings for the user's roles.  The user's lowest-weighted role
(as defined on the settings page) that the user posseses is used.  For
a user that does not have any role indicated in the table, the default
input format will be the one selected on the Input formats > List
page.

For example, suppose you have roles 'author' and 'admin' on your site
in addition to the standard 'anonymous user' and 'authenticated user'
and that your input formats defaults table looks like this:

Weight	Role		Default input format
1	admin		PHP code
2	author		Full HTML
3
4

Four rows are listed because you have four total roles and can assign
a default input format to each one, but only two roles have
assignments.  For users with the 'admin' role, the default input
format selected for any formatable text field will be 'PHP code'.  For
users with the 'author' role but not the 'admin' role, the default will
be 'Full HTML'.  For users with neither role, the default will be
the site-wide default input format.

AUTHOR

Barry Jaspan
http://jaspan.com
firstname at lastname dot com

SPONSOR

Event Publishing LLC
www.event-solutions.com
