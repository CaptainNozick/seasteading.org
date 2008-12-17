<?php
// $Id: boost.api.inc,v 1.3.2.1.2.5 2007/09/21 12:50:08 arto Exp $

/**
 * @file
 * Implements the Boost API for static page caching.
 */

//////////////////////////////////////////////////////////////////////////////
// BOOST API

/**
 * Determines whether a given Drupal page can be cached or not.
 *
 * To avoid potentially troublesome situations, the user login page is never
 * cached, nor are any admin pages. At present, we also refuse to cache any
 * RSS feeds provided by Drupal, since they would require special handling
 * in the mod_rewrite ruleset as they shouldn't be sent out using the
 * text/html content type.
 */
function boost_is_cacheable($path) {
  $alias = drupal_get_path_alias($path);
  $path = drupal_get_normal_path($path); // normalize path

  // Never cache the basic user login/registration pages or any administration pages
  if ($path == 'user' || preg_match('!^user/(login|register|password)!', $path) || preg_match('!^admin!', $path))
    return FALSE;

  // At present, RSS feeds are not cacheable due to content type restrictions
  if ($path == 'rss.xml' || preg_match('!/feed$!', $path))
    return FALSE;

  // Don't cache comment reply pages
  if (preg_match('!^comment/reply!', $path))
    return FALSE;

  // Match the user's cacheability settings against the path
  if (BOOST_CACHEABILITY_OPTION == 2) {
    $result = drupal_eval(BOOST_CACHEABILITY_PAGES);
    return !empty($result);
  }
  $regexp = '/^('. preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/', '/(^|\|)\\\\<front\\\\>($|\|)/'), array('|', '.*', '\1'. preg_quote(variable_get('site_frontpage', 'node'), '/') .'\2'), preg_quote(BOOST_CACHEABILITY_PAGES, '/')) .')$/';
  return !(BOOST_CACHEABILITY_OPTION xor preg_match($regexp, $alias));
}

/**
 * Determines whether a given Drupal page is currently cached or not.
 */
function boost_is_cached($path) {
  $path = (empty($path) ? BOOST_FRONTPAGE : $path);
  $alias = drupal_get_path_alias($path);
  $path = drupal_get_normal_path($path); // normalize path

  // TODO: also determine if alias/symlink exists?
  return file_exists(boost_file_path($path));
}

/**
 * Deletes all static files currently in the cache.
 */
function boost_cache_clear_all() {
  clearstatcache();
  if (($cache_dir = boost_cache_directory()) && file_exists($cache_dir)) {
    return _boost_rmdir_rf($cache_dir);
  }
}

/**
 * Deletes all expired static files currently in the cache.
 */
function boost_cache_expire_all() {
  clearstatcache();
  if (($cache_dir = boost_cache_directory()) && file_exists($cache_dir)) {
    _boost_rmdir_rf($cache_dir, 'boost_file_is_expired');
  }
  return TRUE;
}

/**
 * Expires the static file cache for a given page, or multiple pages
 * matching a wildcard.
 */
function boost_cache_expire($path, $wildcard = FALSE) {
  // TODO: handle wildcard.

  $alias = drupal_get_path_alias($path);
  $path = drupal_get_normal_path($path); // normalize path

  if (($filename = boost_file_path($path)) && file_exists($filename)) {
    @unlink($filename);
  }

  if ($alias != $path && ($symlink = boost_file_path($alias)) && is_link($symlink)) {
    @unlink($symlink);
  }

  return TRUE;
}

/**
 * Returns the cached contents of the specified page, if available.
 */
function boost_cache_get($path) {
  $path = drupal_get_normal_path($path); // normalize path

  if (($filename = boost_file_path($path))) {
    if (file_exists($filename) && is_readable($filename)) {
      return file_get_contents($filename);
    }
  }

  return NULL;
}


/**
 * Replaces the cached contents of the specified page, if stale.
 */
function boost_cache_set($path, $data = '') {
  // Append the Boost footer with the relevant timestamps
  $time = time();
  $cached_at = date('Y-m-d H:i:s', $time);
  $expires_at = date('Y-m-d H:i:s', $time + variable_get('cache_lifetime', 600));
  $data = rtrim($data) . "\n" . str_replace(array('%cached_at', '%expires_at'), array($cached_at, $expires_at), BOOST_BANNER);

  // Execute the pre-process function if one has been defined
  if (function_exists(BOOST_PRE_PROCESS_FUNCTION))
    $data = call_user_func(BOOST_PRE_PROCESS_FUNCTION, $data);

  $alias = drupal_get_path_alias($path);
  $path = drupal_get_normal_path($path); // normalize path

  // Create or update the static file as needed
  if (($filename = boost_file_path($path))) {
    _boost_mkdir_p(dirname($filename));
    if (!file_exists($filename) || boost_file_is_expired($filename)) {
      if (file_put_contents($filename, $data) === FALSE) {
        watchdog('boost', t('Unable to write file: %file', array('%file' => $filename)), WATCHDOG_WARNING);
      }
    }

    // If a URL alias is defined, create that as a symlink to the actual file
    if ($alias != $path && ($symlink = boost_file_path($alias))) {
      _boost_mkdir_p(dirname($symlink));
      if (!is_link($symlink) || realpath(readlink($symlink)) != realpath($filename)) {
        if (file_exists($symlink)) {
          @unlink($symlink);
        }
        if (!_boost_symlink($filename, $symlink)) {
          watchdog('boost', t('Unable to create symlink: %link to %target', array('%link' => $symlink, '%target' => $filename)), WATCHDOG_WARNING);
        }
      }
    }
  }

  return TRUE;
}

/**
 * Returns the full directory path to the static file cache directory.
 */
function boost_cache_directory($user_id = 0, $host = NULL) {
  global $user, $base_url;
  $user_id = 0; //(!is_null($user_id) ? $user_id : BOOST_USER_ID);
  $parts = parse_url($base_url);
  $host = (!empty($host) ? $host : $parts['host']);

  // FIXME: correctly handle Drupal subdirectory installations.
  return implode('/', array(getcwd(), BOOST_FILE_PATH, $host, $user_id));
}

/**
 * Returns the static file path for a Drupal page.
 */
function boost_file_path($path) {
  if (empty($path) || $path == BOOST_FRONTPAGE) {
    $path = 'index'; // special handling for Drupal's front page
  }

  // Under no circumstances should the incoming path contain '..' or null
  // bytes; we also limit the maximum directory nesting depth of the path
  if (strpos($path, '..') !== FALSE || strpos($path, "\0") !== FALSE ||
      count(explode('/', $path)) > BOOST_MAX_PATH_DEPTH) {
    return FALSE;
  }

  // Convert any other undesirable characters in the path to underscores
  $path = preg_replace('@[^/a-z0-9_-]@i', '_', $path);

  return boost_cache_directory() . '/' . $path . BOOST_FILE_EXTENSION;
}

/**
 * Returns the age of a cached file, measured in seconds since it was last
 * updated.
 */
function boost_file_get_age($filename) {
  return time() - filemtime($filename);
}

/**
 * Determines whether a cached file has expired, i.e. whether its age exceeds
 * the maximum cache lifetime as defined by Drupal's system settings.
 */
function boost_file_is_expired($filename) {
  if (is_link($filename))
    return FALSE;
  return boost_file_get_age($filename) > variable_get('cache_lifetime', 600);
}

//////////////////////////////////////////////////////////////////////////////
