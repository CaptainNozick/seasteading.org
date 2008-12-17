// $Id: README.txt,v 1.2.2.1.2.5 2007/09/30 19:18:10 arto Exp $

DESCRIPTION
-----------
This module provides static page caching for Drupal 5.x, enabling a
potentially very significant performance and scalability boost for
heavily-trafficked Drupal sites.

For an introduction, read the original blog post at:
  http://bendiken.net/2006/05/28/static-page-caching-for-drupal

FEATURES
--------
* Maximally fast page serving for the anonymous visitors to your Drupal
  site, reducing web server load and boosting your site's scalability.
* On-demand page caching (static file created after first page request).
* Full support for multi-site Drupal installations.

INSTALLATION
------------
Please refer to the accompanying file INSTALL.txt for installation
requirements and instructions.

HOW IT WORKS
------------
Once Boost has been installed and enabled, page requests by anonymous
visitors will be cached as static HTML pages on the server's file system.
Periodically (when the Drupal cron job runs) stale pages (i.e. files
exceeding the maximum cache lifetime setting) will be purged, allowing them
to be recreated the first time that the next anonymous visitor requests that
page again.

New rewrite rules are added to the .htaccess file supplied with Drupal,
directing the web server to try and fulfill page requests by anonymous
visitors first and foremost from the static page cache, and to only pass the
request through to Drupal if the requested page is not cacheable, hasn't yet
been cached, or the cached copy is stale.

FILE SYSTEM CACHE
-----------------
The cached files are stored (by default) in the cache/ directory under your
Drupal installation directory. The Drupal pages' URL paths are translated
into file system names in the following manner:

  http://mysite.com/
  => cache/mysite.com/0/index.html

  http://mysite.com/about
  => cache/mysite.com/0/about.html

  http://mysite.com/about/staff
  => cache/mysite.com/0/about/staff.html

  http://mysite.com/node/42
  => cache/mysite.com/0/node/42.html

You'll note that the directory path includes the Drupal site name, enabling
support for multi-site Drupal installations. The zero that follows, on the
other hand, denotes the user ID the content has been cached for -- in this
case the anonymous user (which is the default, and only, choice available
for the time being).

DISPATCH MECHANISM
------------------
For each incoming page request, the new Apache mod_rewrite directives in
.htaccess will check if a cached version of the requested page should be
served as per the following simple rules:

  1. First, we check that the HTTP request method being used is GET.
     POST requests are not cacheable, and are passed through to Drupal.

  2. Next, we make sure that the URL doesn't contain a query string (i.e.
     the part after the `?' character, such as `?q=cats+and+dogs'). A query
     string implies dynamic data, and any request that contains one will
     be passed through to Drupal. (This also allows one to easily obtain the
     current, non-cached version of a page by simply adding a bogus query
     string to a URL path -- very useful for testing purposes.)

  3. Since only anonymous visitors can benefit from the static page cache at
     present, we check that the page request doesn't include a cookie that
     is set when a user logs in to the Drupal site. If the cookie is
     present, we simply let Drupal handle the page request dynamically.

  4. Now, for the important bit: we check whether we actually have a cached
     HTML file for the request URL path available in the file system cache.
     If we do, we direct the web server to serve that file directly and to
     terminate the request immediately after; in this case, Drupal (and
     indeed PHP) is never invoked, meaning the page request will be served
     by the web server itself at full speed.

  5. If, however, we couldn't locate a cached version of the page, we just
     pass the request on to Drupal, which will serve it dynamically in the
     normal manner.

IMPORTANT NOTES
---------------
* Drupal URL aliases get written out to disk as relative symbolic links
  pointing to the file representing the internal Drupal URL path. For this
  to work correctly with Apache, ensure your .htaccess file contains the
  following line (as it will by default if you've installed the file shipped
  with Boost):
    Options +FollowSymLinks
* To check whether you got a static or dynamic version of a page, look at
  the very end of the page's HTML source. You have the static version if the
  last line looks like this:
    <!-- Page cached by Boost at 2006-11-24 15:06:31 -->
* If your Drupal URL paths contain non-ASCII characters, you may have to
  tweak your locate settings on the server in order to ensure the URL paths
  get correctly translated into directory paths on the file system.
  Non-ASCII URL paths have currently not been tested at all and feedback on
  them would be appreciated.

LIMITATIONS
-----------
* Only anonymous visitors will be served cached versions of pages; logged-in
  users will get dynamic content. This may somewhat limit the usefulness of
  this module for those community sites that require user registration and
  login for active participation.
* Only content of the type `text/html' will get cached at present. RSS feeds
  and URL paths that have some other content type (e.g. set by a third-party
  module) will be silently ignored by Boost.
* In contrast to Drupal's built-in caching, static caching will lose any
  additional HTTP headers set for an HTML page by a module. This is unlikely
  to be problem except for some very specific modules and rare use cases.
* Web server software other than Apache is not supported at the moment.
  Adding Lighttpd support would be desirable but is not a high priority for
  the author at present (see TODO.txt). (Note that while the LiteSpeed web
  server has not been specifically tested by the author, it may, in fact,
  work, since they claim to support .htaccess files and to have mod_rewrite
  compatibility. Feedback on this would be appreciated.)
* At the moment, Windows users are S.O.L. due to the use of symlinks and
  Unix-specific shell commands. The author has no personal interest in
  supporting Windows but will accept well-documented, non-detrimental
  patches to that effect (see http://drupal.org/node/174380).

BUG REPORTS
-----------
Post feature requests and bug reports to the issue tracking system at:
  http://drupal.org/node/add/project_issue/boost

CREDITS
-------
Developed and maintained by Arto Bendiken <http://bendiken.net/>
Ported to Drupal 5.x by Alexander I. Grafov <http://drupal.ru/>
Miscellaneous contributions by: Jacob Peddicord, Justin Miller, Barry
Jaspan.
