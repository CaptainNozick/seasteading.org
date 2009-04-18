<?php
/**
 * @file
 *   This file provides a preprocess function for the author pane used by  
 *   Advanced Forum and Advanced Profile Kit.
 */

/**
 * Implementation of hook_preprocess_author_pane().
 */
function user_badges_preprocess_author_pane(&$variables) {
  $account_id = $variables['account']->uid;
  
  if ($account_id != 0) {
    $variables['user_badges'] = user_badges_for_uid($account_id);
  } 
}