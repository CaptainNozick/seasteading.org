This module facilitates the upgrade JQuery in Drupal 5. JQuery 1.0.1 is
included with Drupal 5, however it is not very well supported in the JQuery
community. In order to use most current and advanced JQuery functionality
you will want to build off a newer version of JQuery. This module includes
John Resig's compat-1.0.js plugin that provides backwards compatiblity for
newer versions of JQuery to work with the JS code in Drupal 5.

The main issue with this module is that you will need to *replace* the
jquery.js file in core with the jquery.js file included in the jquery_update
directory. JQuery may exhibit some odd behaviors if you fail to do this
and the JQuery Update module will warn you about this problem.

INSTALLATION:

1) Place this module directory in your modules folder (this will usually be
"sites/all/modules/").

2) Enable the module.

3) Copy the jquery.js file in this directory and use it to replace the one at
misc/jquery.js. Note: You will receive a warning message in the Drupal
administration area if you if you do not perform this step correctly.

You will need to repeat step #3 if/when you update your Drupal installation.

