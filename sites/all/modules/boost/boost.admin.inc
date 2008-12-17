<?php
// $Id: boost.admin.inc,v 1.1.2.1.2.3 2007/09/21 12:00:44 arto Exp $

/**
 * @file
 * All the code for the Boost module's administrative interface.
 */

//////////////////////////////////////////////////////////////////////////////
// DRUPAL SYSTEM SETTINGS

/**
 * Performs alterations to the system settings form before it is rendered.
 *
 * Called from hook_form_alter().
 */
function boost_system_settings_form($form = array()) {
  $form['cache'] = array('#type' => 'hidden','#value' => CACHE_DISABLED);
  $form['boost'] = array(
    '#type'          => 'radios',
    '#title'         => t('Static page cache'),
    '#default_value' => variable_get('boost', CACHE_DISABLED),
    '#options'       => array(CACHE_DISABLED => t('Disabled'),
                              CACHE_ENABLED  => t('Enabled')),
    '#description'   => t('Static page caching is a mechanism which stores dynamically generated web pages as HTML files in a special cache directory located under the Drupal installation directory. By caching a web page in this manner, the web server can serve it out in the fastest possible manner, without invoking PHP or Drupal at all. While this does provide a significant performance and scalability boost, you should note that it could have negative usability side-effects unless your site is targeted at an audience consisting mostly of "anonymous" visitors.'),
  );
  $lifetime = &$form['cache_lifetime'];
  $lifetime['#description'] = t('On high-traffic sites it can become necessary to enforce a minimum cache lifetime. The minimum cache lifetime is the minimum amount of time that will go by before the cache is emptied and recreated. A larger minimum cache lifetime offers better performance, but users will not see new content for a longer period of time.');
  unset($form['cache_lifetime']);
  $form['cache_lifetime'] = &$lifetime;
  $form['boost_file_path'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Cache file path'),
    '#default_value' => BOOST_FILE_PATH,
    '#size'          => 60,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    '#description'   => t('A file system path where the cache files will be stored. This directory has to exist and be writable by Drupal. The default setting is to store the files in a directory named \'cache\' under the Drupal installation directory. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
  );
  return $form;
}

//////////////////////////////////////////////////////////////////////////////
// BOOST SETTINGS

/**
 * Declares administrative settings for the Boost module.
 *
 * Called from hook_settings().
 */
function boost_settings_form($form = array()) {
  //_boost_check_htaccess(); // TODO

  $options = array(t('Cache every page except the listed pages.'), t('Cache only the listed pages.'));
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are '%blog' for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => theme('placeholder', 'blog'), '%blog-wildcard' => theme('placeholder', 'blog/*'), '%front' => theme('placeholder', '<front>')));
  if (user_access('use PHP for block visibility')) {
    $options[] = t('Cache pages for which the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $description .= t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can severely break your Drupal site.', array('%php' => theme('placeholder', '<?php ?>')));
  }
  $form['cacheability'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Cacheability settings'),
    '#collapsible'   => FALSE,
  );
  $form['cacheability']['boost_cacheability_option'] = array(
    '#type'          => 'radios',
    '#title'         => t('Cache specific pages'),
    '#options'       => $options,
    '#default_value' => BOOST_CACHEABILITY_OPTION,
  );
  $form['cacheability']['boost_cacheability_pages'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Pages'),
    '#default_value' => BOOST_CACHEABILITY_PAGES,
    '#description'   => $description,
  );

  // TODO
  /*$form['throttle'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Throttle settings'),
    '#collapsible'   => FALSE,
  );
  $form['throttle']['boost_cron_limit'] = array(
    '#type'          => 'select',
    '#title'         => t('Pages to update per cron run'),
    '#default_value' => BOOST_CRON_LIMIT,
    '#options'       => drupal_map_assoc(array(10, 20, 50, 100, 200, 500, 1000)),
    '#description'   => t('The maximum number of static pages that will be built or rebuilt in one cron run. Set this number lower if your cron is timing out or if PHP is running out of memory.'),
  );*/

  $form['advanced'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Advanced settings'),
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
  );
  $form['advanced']['boost_file_extension'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Cache file extension'),
    '#default_value' => BOOST_FILE_EXTENSION,
    '#size'          => 10,
    '#maxlength'     => 32,
    '#required'      => TRUE,
    '#description'   => t('The file extension to append to the file name of the generated cache files. Note that this setting is of no relevance to any public URLs, and it is strongly recommended to leave this as the default \'.html\' unless you know what you are doing. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
  );
  $form['advanced']['boost_fetch_method'] = array(
    '#type'          => 'select',
    '#title'         => t('Page fetch method'),
    '#default_value' => BOOST_FETCH_METHOD,
    '#options'       => array('php' => t('PHP fopen() wrapper'), 'wget' => t('Wget shell command'), 'curl' => t('curl shell command')),
    '#description'   => t('The method used to retrieve the contents of the Drupal pages to be cached. The default should work in most cases.'),
  );
  $form['advanced']['boost_pre_process_function'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Pre-process function'),
    '#default_value' => BOOST_PRE_PROCESS_FUNCTION,
    '#maxlength'     => 255,
    '#description'   => t('The name of a PHP function used to pre-process the contents of each page before writing them out to static files. The function is called with the contents of the page passed as a string argument, and its return value is used as the data written out to the disk.'),
  );
  // TODO:
  /*$form['advanced']['boost_post_update_command'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Post-update shell command'),
    '#default_value' => BOOST_POST_UPDATE_COMMAND,
    '#maxlength'     => 255,
    '#description'   => t('If you are synchronizing the generated static cache files to an external server through some means such as SFTP or rsync, you can enter a shell command to be executed following a successful cron-triggered cache update. Note that this is an advanced setting that should normally be left blank.'),
  );*/

  return $form;
}

//////////////////////////////////////////////////////////////////////////////
