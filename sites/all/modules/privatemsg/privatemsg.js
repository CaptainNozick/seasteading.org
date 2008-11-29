// $Id: privatemsg.js,v 1.3.2.4 2008/06/13 05:11:48 litwol Exp $

if (Drupal.jsEnabled) {
  $(document).ready(
    function() {
      // Hide the elements we want hidden with JS.
      $('.js-hide').hide();

      // Show the elements we want to expose with JS.
      $('.js-show').show();

      // Add the JS to the delete button if necessary.
      $('.pm-add-delete').click(
        function() {
          var response = confirm(Drupal.settings.privatemsg.deleteMessage);
          if (response == true) {
            $('#edit-js-bypass').val(1);
          }
          return response;
        }
      );

      // Add the JS to the folder select if necessary.
      $('.pm-add-folder-select').change(
        function() {
          // click is necessary for right $op.
          $('#edit-go-folder').each(function() { this.click(); });
        }
      );
      
      // Add the JS to the folder select if necessary.
      $('#edit-action').change(
        function() {
          // click is necessary for right $op.
          $('#edit-submit').each(function() { this.click(); });
        }
      );

      $('.pm-filter-select').change(function() {
        var table = $('#privatemsg_message_table');
        switch (this.value) {
          case '_privatemsg_delimiter':
            break;
          case '_all':
          case 'all types':
            table.find('tr td input:checkbox').attr('checked', true);
            break;
          case '_read':
            table.find('tr.pm-new td input:checkbox').attr('checked', false).end()
              .find('tr:not(.pm-new) td input:checkbox').attr('checked', true);
            break;
          case '_unread':
            table.find('tr.pm-new td input:checkbox').attr('checked', true).end()
              .find('tr:not(.pm-new) td input:checkbox').attr('checked', false);
            break;
          case '_none':
            table.find('tr td input:checkbox').attr('checked', false);
            break;
          case '_invert':
            table.find('tr td input:checkbox').each(function() {
              this.checked = !this.checked;
            });
            break;
          default:
            var cl = this.value.toLowerCase().replace(/[^\w]+/i, '_');
            table.find('tr.pm-'+ cl +' td input:checkbox').attr('checked', true).end()
              .find('tr:not(.pm-'+ cl +') td input:checkbox').attr('checked', false);
        }

        var all = true;
        table.find('td input:checkbox').each(function() {
          all = all && this.checked;
          $(this).parents('tr:first')[ this.checked ? 'addClass' : 'removeClass' ]('selected');
        });

        if (Drupal.settings.tableSelect) {
          var settings = Drupal.settings.tableSelect;
          table.find('th.select-all input:checkbox').attr('checked', all)
            .attr('title', all ? settings.selectNone : settings.selectAll);
        }

        this.value = '_privatemsg_delimiter';
      });
    }
  );
}
