<?php
// $Id: location.be.inc,v 1.6.2.1 2008/08/06 15:19:37 bdragon Exp $

// Belgium

function location_province_list_be() {
  return array(
    'VAN' => "Antwerpen",
    'VBR' => "Vlaams Brabant",
    'VLI' => "Limburg",
    'VOV' => "Oost-Vlaanderen",
    'VWV' => "West-Vlaanderen",
    'WBR' => "Brabant Wallon",
    'WHT' => "Hainaut",
    'WLG' => "Liege",
    'WLX' => "Luxembourg",
    'WNA' => "Namur",
    // While technically not a province, Brussels-Capital Region is needed here
    // because some places would be completely without a province if we did not
    // include it.
    // See http://drupal.org/node/228766 and http://drupal.org/node/291590.
    'BRU' => "Brussels",
  );
}
