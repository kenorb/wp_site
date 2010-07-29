// $Id$

// Global Killswitch
if (Drupal.jsEnabled) {
  $(document).ready(function() {  
    // Hide the other options if "Show message (sticky)" is not checked.    
    $("#admin-message-toggle")[['hide', 'show'][Number($("#edit-sticky")[0].checked)]]();
    
    $("#edit-sticky").click(function() {
      $("#admin-message-toggle")[['hide', 'show'][Number(this.checked)]]();
    });
  });
}
