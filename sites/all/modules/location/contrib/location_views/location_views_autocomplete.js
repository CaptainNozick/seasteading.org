// $Id: location_views_autocomplete.js,v 1.1.2.1 2008/10/06 17:49:08 bdragon Exp $

/**
 * Twiddle the province autocomplete whenever the user changes the country.
 */
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('select.location_views_auto_country').change(function(e) {
      // If you're using multiple views with exposed filters on one page, you
      // will have to change how this works. Use $(this).parents(...) instead;
      // see location_autocomplete.js.
      var theinput = $('form#views-filters').find('.location_views_auto_province').unbind()[0];
      // Clear province.
      theinput.value = '';
      // Unbind events.
      var theid = '#' + theinput.id + '-autocomplete';
      // Reset autocomplete status.
      var input = $(theid).attr('autocomplete', 'ON')[0];
      // Update autocomplete url for new country.
      input.value = input.value.substr(0, input.value.length - 2) + this.value;
      // Re-attach.
      Drupal.autocompleteAutoAttach();
    });
  });
}
