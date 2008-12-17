/* $Id: marker.js,v 1.1.2.4 2008/08/21 00:40:46 bdragon Exp $ */

/**
 * Common marker routines.
 */

Drupal.gmap.addHandler('gmap', function(elem) {
  var obj = this;

  obj.bind('init', function() {
    if (obj.vars.behavior.autozoom) {
      obj.bounds = new GLatLngBounds();
    }
  });

  obj.bind('addmarker',function(marker) {
    var m = Drupal.gmap.factory.marker(new GLatLng(marker.latitude,marker.longitude), marker.opts);
    marker.marker = m;
    GEvent.addListener(m,'click',function() {
      obj.change('clickmarker',-1,marker);
    });
    if (obj.vars.behavior.extramarkerevents) {
      GEvent.addListener(m,'mouseover',function() {
        obj.change('mouseovermarker',-1,marker);
      });
      GEvent.addListener(m,'mouseout',function() {
        obj.change('mouseoutmarker',-1,marker);
      });
      GEvent.addListener(m,'dblclick',function() {
        obj.change('dblclickmarker',-1,marker);
      });
    }
    /**
     * Perform a synthetic marker click on this marker on load.
     */
    if (marker.autoclick || (marker.options && marker.options.autoclick)) {
      obj.deferChange('clickmarker',-1,marker);
    }
    if (obj.vars.behavior.autozoom) {
      obj.bounds.extend(marker.marker.getPoint());
    }
  });

  // Default marker actions.
  obj.bind('clickmarker',function(marker) {
    // Local/stored content
    if (marker.text) {
      marker.marker.openInfoWindowHtml(marker.text);
    }
    // AJAX content
    if (marker.rmt) {
      var uri = marker.rmt;
      // If there was a callback, prefix that.
      // (If there wasn't, marker.rmt was the FULL path.)
      if (obj.vars.rmtcallback) {
        uri = obj.vars.rmtcallback + '/' + marker.rmt;
      }
      // @Bevan: I think it makes more sense to do it in this order.
      // @Bevan: I don't like your choice of variable btw, seems to me like
      // @Bevan: it belongs in the map object, or at *least* somewhere in
      // @Bevan: the gmap settings proper...
      //if (!marker.text && Drupal.settings.loadingImage) {
      //  marker.marker.openInfoWindowHtml(Drupal.settings.loadingImage);
      //}
      $.get(uri, {}, function(data) {
        marker.marker.openInfoWindowHtml(data);
      });
    }
    // Tabbed content
    else if (marker.tabs) {
      var infoWinTabs = [];
      for (var m in marker.tabs) {
        infoWinTabs.push(new GInfoWindowTab(m,marker.tabs[m]));
      }
      marker.marker.openInfoWindowTabsHtml(infoWinTabs);
    }
    // No content -- marker is a link
    else if (marker.link) {
        open(marker.link,'_self');
    }
  });

  obj.bind('markersready', function() {
    // If we are autozooming, set the map center at this time.
    if (obj.vars.behavior.autozoom) {
      if (!obj.bounds.isEmpty()) {
        obj.map.setCenter(obj.bounds.getCenter(), Math.min(obj.map.getBoundsZoomLevel(obj.bounds), obj.vars.maxzoom));
      }
    }
  });

  obj.bind('clearmarkers', function() {
    // Reset bounds if autozooming
    // @@@ Perhaps we should have a bounds for both markers and shapes?
    if (obj.vars.behavior.autozoom) {
      obj.bounds = new GLatLngBounds();
    }
  });

  // @@@ TODO: Some sort of bounds handling for deletemarker? We'd have to walk the whole thing to figure out the new bounds...
});
