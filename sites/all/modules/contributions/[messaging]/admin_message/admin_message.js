// $Id$

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {  
    // Close
    $("#block-admin_message-admin_message a.admin-message-close").click(function() {
      var href = $(this).attr("href");
      $.get(href);
      $(this).parent().slideUp('fast');
      return false;
    });
  });
}
