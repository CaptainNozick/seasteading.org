/* $Id: gmap_marker.js,v 1.1.2.28 2008/06/30 16:36:05 bdragon Exp $ */

/**
 * GMap Markers
 * GMap API version -- No manager
 */

// Replace to override marker creation
Drupal.gmap.factory.marker = function(loc,opts) {
  return new GMarker(loc,opts);
};

Drupal.gmap.addHandler('gmap', function(elem) {
  var obj = this;

  obj.bind('addmarker',function(marker) {
    obj.map.addOverlay(marker.marker);
  });

  obj.bind('delmarker',function(marker) {
    obj.map.removeOverlay(marker.marker);
  });

  obj.bind('clearmarkers',function() {
    // @@@ Maybe don't nuke ALL overlays?
    obj.map.clearOverlays();
  });
});
