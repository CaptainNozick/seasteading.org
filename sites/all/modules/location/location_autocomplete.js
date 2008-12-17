// $Id: location_autocomplete.js,v 1.1 2008/06/06 17:14:27 bdragon Exp $

/**
 * Twiddle the province autocomplete whenever the user changes the country.
 */
if (Drupal.jsEnabled) {
  $(document).ready(function() {
    $('select.location_auto_country').change(function(e) {
      var theinput = $(this).parents('fieldset').eq(0).find('.location_auto_province').unbind()[0];
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
