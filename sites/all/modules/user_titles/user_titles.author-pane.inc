<?php
/**
 * @file
 *   This file provides a preprocess function for the author pane used by  
 *   Advanced Forum and Advanced Profile Kit.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_titles_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  
  if ($account_id != 0) {
    $variables['user_title'] = user_titles_get_user_title($account_id);
    $variables['user_title_image'] = user_titles_get_user_image($account_id);
  }  
}
