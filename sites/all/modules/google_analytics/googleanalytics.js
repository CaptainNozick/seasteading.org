// $Id: googleanalytics.js,v 1.1.2.2 2008/05/03 22:18:03 hass Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function() {

    $('a').click( function() {
      var ga = Drupal.settings.googleanalytics;
      // Extract the domain from the location (the domain is in domain[2]).
      // http://docs.jquery.com/Plugins/Validation/Methods/url
      var domain = /^(https?|ftp|news|nntp|telnet|irc|ssh|sftp|webcal):\/\/(((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:)*@)?(((\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5])\.(\d|[1-9]\d|1\d\d|2[0-4]\d|25[0-5]))|((([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|\d|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.)*(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])*([a-z]|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])))\.?)(:\d*)?)(\/((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)+(\/(([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)*)*)?)?(\?((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)|[\uE000-\uF8FF]|\/|\?)*)?(\#((([a-z]|\d|-|\.|_|~|[\u00A0-\uD7FF\uF900-\uFDCF\uFDF0-\uFFEF])|(%[\da-f]{2})|[!\$&'\(\)\*\+,;=]|:|@)|\/|\?)*)?$/i.exec(window.location);
      // Expression for check external links.
      var isInternal = new RegExp("^(https?|ftp|news|nntp|telnet|irc|ssh|sftp|webcal):\/\/" + domain[2], "i");
      // Expression for special links like gotwo.module /go/* links.
      var isInternalSpecial = new RegExp("(\/go\/.*)$", "i");
      // Expression for check download links.
      var isDownload = new RegExp("\\.(" + ga.trackDownloadExtensions + ")$", "i");

      // Is the clicked URL internal?
      if (isInternal.test(this.href)) {
        // Is download tracking activated and the file extension configured for download tracking?
        if ((ga.trackDownload && isDownload.test(this.href)) || isInternalSpecial.test(this.href)) {
          // Keep the internal URL for Google Analytics website overlay intact.
          if (ga.LegacyVersion) {
            urchinTracker(this.href.replace(isInternal, ''));
          }
          else {
            pageTracker._trackPageview(this.href.replace(isInternal, ''));
          }
        }
      }
      else {
        if (ga.trackMailto && this.href.indexOf('mailto:') == 0) {
          // Mailto link clicked.
          if (ga.LegacyVersion) {
            urchinTracker('/mailto/' + this.href.substring(7));
          }
          else {
            pageTracker._trackPageview('/mailto/' + this.href.substring(7));
          }
        }
        else if (ga.trackOutgoing) {
          // External link clicked. Clean and track the URL.
          if (ga.LegacyVersion) {
            urchinTracker('/outgoing/' + this.href.replace(/^(https?|ftp|news|nntp|telnet|irc|ssh|sftp|webcal):\/\//i, '').split('/').join('--'));
          }
          else {
            pageTracker._trackPageview('/outgoing/' + this.href.replace(/^(https?|ftp|news|nntp|telnet|irc|ssh|sftp|webcal):\/\//i, '').split('/').join('--'));
          }
        }
      }
    });
  });
}
