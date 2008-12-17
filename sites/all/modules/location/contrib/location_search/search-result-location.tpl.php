<?php
// $Id: search-result-location.tpl.php,v 1.1.2.1 2008/10/09 22:01:55 bdragon Exp $

/**
 * @file search-result-location.tpl.php
 * Theme a location search result.
 *
 * Available variables:
 * - $links: A themed set of links to all the objects that reference this location.
 * - $location: A themed location.
 */
?>
<dt class="title">
  <?php print $links; ?>
</dt>
<dd>
  <?php print $location; ?>
</dd>
