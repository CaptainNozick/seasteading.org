<?php
// $Id: location.d5.inc,v 1.1.2.1 2008/10/21 22:59:22 bdragon Exp $

/**
 * @file
 * Drupal 5 specific routines.
 */

/**
 * Temporary function to signify that this Location.module is using the 3.x api
 * on Drupal 5.
 */
function location_newapi() {
  return TRUE;
}

/**
 * Callback for admin settings form.
 */
function location_admin_settings() {
  require_once(drupal_get_path('module', 'location') .'/location.admin.inc');
  return _location_admin_settings();
}

/**
 * Callback for map link page of admin settings form.
 */
function location_map_link_options_form() {
  require_once(drupal_get_path('module', 'location') .'/location.admin.inc');
  return _location_map_link_options_form();
}

/**
 * Callback for geocoding options page.
 */
function location_geocoding_options_page() {
  $iso = arg(4);
  $service = arg(5);
  if (!empty($iso) || !empty($service)) {
    return location_geocoding_parameters_page($iso, $service);
  }

  require_once(drupal_get_path('module', 'location') .'/location.admin.inc');
  return drupal_get_form('location_geocoding_options_form');
}

/**
 * Drupal 6 style theming helper.
 */
function _location_render($file, $variables, $module = 'location') {
  // Run the built-in template_preprocess function.
  $func = 'template_preprocess_'. $file;
  $func($variables);
  $filepath = drupal_get_path('module', 'location');
  if (file_exists(path_to_theme() ."/$file.tpl.php")) {
    $filepath = path_to_theme();
  }
  if (!empty($variables['template_files'])) {
    foreach ($variables['template_files'] as $temp) {
      if (file_exists(path_to_theme() ."/$temp.tpl.php")) {
        $filepath = path_to_theme();
        $file = $temp;
        break;
      }
      if (file_exists(drupal_get_path('module', $module) ."/$temp.tpl.php")) {
        $filepath = drupal_get_path('module', $module);
        $file = $temp;
        break;
      }
    }
  }
  extract($variables, EXTR_SKIP);  // Extract the variables to a local namespace
  ob_start();                      // Start output buffering
  include "$filepath/$file.tpl.php"; // Include the file
  $contents = ob_get_contents();   // Get the contents of the buffer
  ob_end_clean();                  // End buffering and discard
  return $contents;                // Return the contents
}

/////////////// Theme compatibility wrappers //////////////////
// Needed due to the lack of the theme registry in Drupal 5. //
///////////////////////////////////////////////////////////////

function theme_location($location = array(), $hide = array()) {
  $variables = array('location' => $location, 'hide' => $hide);
  return _location_render('location', $variables);
}

function theme_location_distance($distance = 0, $units = 'km') {
  $variables = array('distance' => $distance, 'units' => $units);
  return _location_render('location_distance', $variables);
}

function theme_locations($locations = array(), $hide = array()) {
  $variables = array('locations' => $locations, 'hide' => $hide);
  return _location_render('locations', $variables);
}
