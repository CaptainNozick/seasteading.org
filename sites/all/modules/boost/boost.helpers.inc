<?php
// $Id: boost.helpers.inc,v 1.3.2.3.2.3 2007/09/21 12:00:44 arto Exp $

/**
 * @file
 * Various helper functions for the Boost module, to make life a bit easier.
 */

//////////////////////////////////////////////////////////////////////////////

/**
 * Recursive version of mkdir(), compatible with PHP4.
 */
function _boost_mkdir_p($pathname, $mode = 0775, $recursive = TRUE) {
  if (is_dir($pathname)) return TRUE;
  if ($recursive && !_boost_mkdir_p(dirname($pathname), $mode)) return FALSE;
  if ($result = @mkdir($pathname, $mode))
    @chmod($pathname, $mode);
  return $result;
}

/**
 * Recursive version of rmdir(); use with extreme caution.
 *
 * @param $dirname
 *   the top-level directory that will be recursively removed
 * @param $callback
 *   optional predicate function for determining if a file should be removed
 */
function _boost_rmdir_rf($dirname, $callback = NULL) {
  $empty = TRUE; // Start with an optimistic mindset

  foreach (glob($dirname . '/*', GLOB_NOSORT) as $file) {
    if (is_dir($file)) {
      if (!_boost_rmdir_rf($file, $callback))
        $empty = FALSE;
    }
    else if (is_file($file)) {
      if (function_exists($callback)) {
        if (!$callback($file)) {
          $empty = FALSE;
          continue;
        }
      }
      @unlink($file);
    }
    else {
      $empty = FALSE; // it's probably a symbolic link
    }
  }

  // The reason for this elaborate safeguard is that Drupal will log even
  // warnings that should have been suppressed with the @ sign. Otherwise,
  // we'd just rely on the FALSE return value from rmdir().
  return ($empty && @rmdir($dirname));
}

/**
 * Creates a symbolic link using a computed relative path where possible.
 */
function _boost_symlink($target, $link) {
  if (!file_exists($target) || !file_exists(dirname($link)))
    return FALSE;

  $target = explode('/', $target);
  $link = explode('/', $link);

  // Only bother creating a relative link if the paths are in the same
  // top-level directory; otherwise just symlink to the absolute path.
  if ($target[1] == $link[1]) {
    // Remove the common path prefix
    $cwd = array();
    while (count($target) > 0 && count($link) > 0 && reset($target) == reset($link)) {
      $cwd[] = array_shift($target);
      array_shift($link);
    }
    // Compute the required relative path
    if (count($link) > 1)
      $target = array_merge(array_fill(0, count($link) - 1, '..'), $target);
    $link = array_merge($cwd, $link);
  }

  return symlink(implode('/', $target), implode('/', $link));
}

//////////////////////////////////////////////////////////////////////////////
// PHP4 COMPATIBILITY

if (!function_exists('file_put_contents')) {
  function file_put_contents($filename, $data) {
    if ($fp = fopen($filename, 'wb')) {
      fwrite($fp, $data);
      fclose($fp);
      return filesize($filename);
    }
    return FALSE;
  }
}

//////////////////////////////////////////////////////////////////////////////
