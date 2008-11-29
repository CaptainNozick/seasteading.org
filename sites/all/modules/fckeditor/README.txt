$Id: README.txt,v 1.15.2.2.2.11 2008/10/08 12:04:46 wwalc Exp $

CONTENTS OF THIS FILE
---------------------

 * Overview
 * Required components
 * More information and licence
 * Requirements
 * Configuration
 * Installation troubleshooting
 * Plugins: Teaser break and Pagebreak
 * Uploading images and files
 * How to enable the built-in file browser
 * Modules: Image Assist
 * Modules: Link to content (EXPERIMENTAL)
 * Link to content + Path Filter (EXPERIMENTAL)
 * Upgrading instructions
 * Help & Contribution
 * Credits 
 
Overview
--------
This module allows Drupal to replace textarea fields with the
FCKeditor.
This HTML text editor brings many of the powerful functions of known
desktop editors like Word to the web. It's relatively lightweight and
doesn't require any kind of installation on the client computer.

Required components
-------------------
To use FCKeditor in Drupal, you will need to download the FCKeditor
http://www.fckeditor.net/

More information and licence
----------------------------
FCKeditor - The text editor for internet
Copyright (C) 2003-2006 Frederico Caldeira Knabben

Licensed under the terms of the GNU Lesser General Public License:
    http://www.opensource.org/licenses/lgpl-license.php

For further information visit:
    http://www.fckeditor.net/
    http://drupal.fckeditor.net

Requirements
------------
  - Drupal 5.x
  - PHP 4.3.0 or greater
  - FCKeditor 2.3.x or greater (http://www.fckeditor.net/)

Configuration
-------------------
   1. Enable the module as usual from Drupal's admin pages.
   2. Grant permissions for use of FCKeditor in Administer > User Management > Access Control
   3. Under Administer > Settings > FCKeditor, create the fckeditor profiles. 
      In each profile you can choose which textareas will be replaced by FCKeditor, 
      select default toolbar and configure some more advanced settings
   4. For the Rich Text Editing to work you also need to configure your filters for the users that may access Rich Text Editing. 
      Either grant those users Full HTML access or use the following:
      <a> <p> <span> <div> <h1> <h2> <h3> <h4> <h5> <h6> <img> <map> <area> <hr> 
      <br> <br /> <ul> <ol> <li> <dl> <dt> <dd> <table> <tr> <td> <em> <b> <u> <i> <strong> 
      <font> <del> <ins> <sub> <sup> <quote> <blockquote> <pre> <address> <code> 
      <cite> <embed> <object> <param> <strike> <caption>.
   5. To have a better control over line breaks, you may disable Line break converter in the chosen filter (recommended).
   6. Modify the fckeditor.config.js file to custom your needs (optional).
      You may copy the needed configuration lines from the default FCKeditor configuration settings (modules/fckeditor/fckeditor/fckconfig.js), 
      the lines in fckeditor.config.js will override most settings.
      WARNING: clear browser's cache after you modify any of the javascript files.
      If you don't do this, you may notice that browser is ignoring all your changes.

Installation troubleshooting
----------------------------
If your FCKeditor does not show you must check if all files are extracted correctly. 
The directory /drupal5/modules/fckeditor/fckeditor/ should have the following files:
fckeditor.js, fckconfig.js, fckstyles.xml, fcktemplates.xml and a directory named editor.
The correct directory structure is as follows:

    modules
       fckeditor
          fckeditor.module
          fckeditor
             _samples
             editor
             COPY_HERE.txt
             fckconfig.js
             ...

If you're still having problems, scroll down to the "Help & Contribution" section.
             
Plugins: Teaser break and Pagebreak
-----------------------------------
By default, FCKeditor module comes with two plugins that can handle teaser break (<!--break-->) and pagebreak (<!--pagebreak-->). 
You can enable any (or even both) of them.

   1. Open /drupal5/modules/fckeditor/fckeditor.config.js and uncomment these three lines:

            FCKConfig.PluginsPath = '../../plugins/' ;
            FCKConfig.Plugins.Add( 'drupalbreak' ) ;
            FCKConfig.Plugins.Add( 'drupalpagebreak' ) ;
            

   2. The second step is to add buttons to the toolbar (in the same file). 
      The button names are: DrupalBreak, DrupalPageBreak. 
      For example if you have a toolbar with an array of buttons defined as follows:

      ['Image','Flash','Table','Rule','SpecialChar']

      simply add those two buttons at the end of array:

      ['Image','Flash','Table','Rule','SpecialChar', 'DrupalBreak', 'DrupalPageBreak']

      (remember about single quotes).

Uploading images and files
--------------------------

There are three ways of uploading files: By using the built-in file browser, by using a module like IMCE or using the core upload module.

How to enable the built-in file browser
---------------------------------------
The editor gives the end user the flexibility to create a custom file browser that can be integrated on it. 
The included file browser allows users to view the content of a specific directory on 
the server and add new content to that directory (create folders and upload files).

   1. To enable file browsing you need to edit the connector configuration file in your fckeditor module directory, the file should be in:

          /drupal5/modules/fckeditor/fckeditor/editor/filemanager/connectors/php/config.php
          (FCKeditor 2.5+)

          or

          /drupal5/modules/fckeditor/fckeditor/editor/filemanager/browser/default/connectors/php/config.php
          and
          /drupal5/modules/fckeditor/fckeditor/editor/filemanager/upload/php/config.php
          (FCKeditor 2.3.x - 2.4.x)

      In this file(s) you will need to enable the file browser by adding one line that includes file with the special authentication function for Drupal (filemanager.config.php). Add this code:

          require_once "../../../../../filemanager.config.php";
          (FCKeditor 2.5+)

      or

          require_once "/absolute/path/to/drupal5/modules/fckeditor/filemanager.config.php"
          (FCKeditor 2.3.x - 2.4.x)

      straight below this line:

          $Config['UserFilesAbsolutePath'] = '' ;

      The config.php file also holds some other important settings, please take a look at it and adjust it to your needs (optional).
      
   2. As of Drupal 5.2, additional step is required: locate file named settings.php inside 
      your drupal directory (usually sites/default/settings.php) and set $cookie_domain variable to the appropiate domain (remember to uncomment that line). If you not do this, FCKeditor will claim that file browser is disabled
   3. Enabling file uploads is a security risk. That's why you have to grant a separate permission to enable the file browser to certain groups.
   4. Lastly, adjust the File browser settings for each profile.

Modules: Image Assist
---------------------
Image Assist can be integrated with FCKeditor. 
To do this, simply copy the modules/fckeditor/img_assist_fckeditor.js file to modules/img_assist/img_assist_fckeditor.js.

Modules: Link to content (EXPERIMENTAL)
---------------------------------------
Link to content module can be integrated with FCKeditor.
ATTENTION: this module is not yet compatible with FCKeditor :(

The unofficial version of Link to content module compatible with FCKeditor can be downloaded here:
 * 5.x-1.7	(2007-Oct-04)
     http://drupal.fckeditor.net/download/linktocontent-fckeditor-5.x-1.7.tar.gz 
 * 5.x-2.x-dev	(2008-Mar-06)
     http://drupal.fckeditor.net/download/linktocontent-fckeditor-5.x-2.x-dev.tar.gz

Alternatively, you may apply required changes in the linktocontent code by yourself.
To do this, download the linktocontent module from http://drupal.org/project/linktocontent.
The minimal list of required changes in this module is available here:
  http://drupal.fckeditor.net/linktocontent.patch

Installation:
Follow the instruction from INSTALL.txt attached to the linktocontent module.
Then do the following steps to add Linktocontent button to the FCKeditor toolbar:

By default, FCKeditor module comes with two plugins that allows you to use linktocontent and linktonode features.
You can enable any (or even both) of them.

   1. Open /drupal/modules/fckeditor/fckeditor.config.js and uncomment these three lines:

            FCKConfig.PluginsPath = '../../plugins/' ;
            FCKConfig.Plugins.Add( 'linktonode', 'en,pl' ) ;
            FCKConfig.Plugins.Add( 'linktomenu', 'en,pl' ) ;

   2. The second step is to add buttons to the toolbar (in the same file).
      The button names are: LinkToNode, LinkToMenu. 
      For example if you have a toolbar with an array of buttons defined as follows:

      ['Link','Unlink','Anchor']

      simply add those two buttons at the end of array (or somewhere in the middle):

      ['Link','Unlink','LinkToNode','LinkToMenu','Anchor']

      (remember about single quotes).

Link to content + Path Filter (EXPERIMENTAL)
--------------------------------------------
Linktonode plugin (read above) is also compatible with Path Filter module.
You can create links to your sites using special syntax: internal:node/23.
Download and install Path Filter module: http://drupal.org/project/pathfilter
Edit your FCKeditor profile:
    1. Go to "Advanced options" section
    2. In "Path Filter & Link To Content integration" choose either
       "Link using internal: links" or "Allow user to select between paths and internal links"

Upgrading instructions
----------------------
This instruction assumes that you are upgrading FCKeditor module [M] and FCKeditor (the editor)[E] at the same time. 
Instructions specific for module upgrades are tagged with [M], steps that must be taken when upgrading FCKeditor (the editor) are marked with [E].

   1. [M] Download the latest version of FCKeditor module from http://drupal.org/project/fckeditor (it is advised to read release notes before going further).
   2. [E] Download the latest version of FCKeditor from http://www.fckeditor.net/download (it is advised to read "what's new" before going further: http://www.fckeditor.net/whatsnew).
   3. [M] Back up your database.
   4. [EM] Place the site in "Off-line" mode, to let the database updates run without interruption and avoid displaying errors to end users of the site.
   5. [E] If you have used the FCKeditor built-in file browser, make a backup of sites/all/modules/fckeditor/fckeditor/editor/filemanager/connectors/php/config.php
   6. [E] If you have configured spellchecker, make a backup of sites/all/modules/fckeditor/fckeditor/editor/dialog/fck_spellerpages/spellerpages/server-scripts/spellchecker.php
   7. [E] If you have made any changes inside of sites/all/modules/fckeditor/fckeditor.config.js (or sites/all/modules/fckeditor/fckeditor/fckconfig.js), write down your changes and add them again after uploading new files (e.g. own toolbar definitions, re-enable a plugin etc.). Try to not make any changes to fckconfig.js and add everything to fckeditor.config.js.
   8. Delete old files:
      [EM]* Simply remove modules/fckeditor directory if upgrading both, the editor and the module. 
      [M] If you are upgrading module only, remember to leave the modules/fckeditor/fckeditor directory. 
      [E] When upgrading the editor, remove contents of modules/fckeditor/fckeditor directory only.
      WARNING: if you don't remove old files and just rename fckeditor directory instead e.g. to fckeditor_old, Drupal may use module from the fckeditor_old directory.
   9. [M] Upload FCKeditor module (extracted files and folders) to sites/all/modules directory.
   10. [E] Upload FCKeditor (extracted files and folders from the fckeditor directory) to sites/modules/fckeditor/fckeditor (i.e. where COPY HERE.txt file exists)
   11. [E] Replace the new config.php (see step 5) file with the old one (or RECOMMENDED way: perform again step with adding require_once '../../../../../filemanager.config.php'; to config.php)
   12. [E] Replace the new spellchecker.php with the old one (see step 6) (or RECOMMENDED way: configure new spellchecker.php following the settings from the old file).
   13. [E] Apply your modifications to default configuration in fckeditor.config.js file (see step 7).
   14. [M] If you're using Image Assist module, copy the new img_assist_fckeditor.js to modules/img_assist folder.
   15. [M] Run update.php.
   16. [EM] Put the site back online.

Help & Contribution
-------------------
If you are looking for more information, have any troubles in configuration or if 
you found an issue, please visit the official project page:
  http://drupal.org/project/fckeditor

Having problems? Take a look at list of common problems when installing FCKeditor:
  http://drupal.fckeditor.net/troubleshooting

How to tune up FCKeditor to your theme:
  http://drupal.fckeditor.net/tricks
  
We would like to encourage you to join our team if you can help in any way.
If you can translate FCKeditor module, please use fckeditor.pot file as a template
(located in "po" directory) and send us the translated file so that we could attach it.
Any help is appreciated.
     
Credits
-------
 - FCKeditor for Drupal Core functionality originally written by:
     Frederico Caldeira Knabben
     Jorge Tite (LatPro Inc.)

 - FCKeditor for Drupal 5.x originally written by:
     Ontwerpwerk (www.ontwerpwerk.nl)
 
 - FCKeditor for Drupal 5.x is currently maintained by FCKeditor team.
     http://www.fckeditor.net/

 - FCKeditor - The text editor for internet
     Copyright (C) 2003-2006 Frederico Caldeira Knabben
     http://www.fckeditor.net/
