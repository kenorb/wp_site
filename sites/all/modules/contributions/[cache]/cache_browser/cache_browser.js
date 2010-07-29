// $Id: cache_browser.js,v 1.1.2.1 2008/10/26 02:44:11 markuspetrux Exp $

if (Drupal.jsEnabled) {
  $(document).ready(function () {
    var cache_browser_icon_plus = new Image();
    cache_browser_icon_plus.src = cache_browser.icon_plus;
    var cache_browser_icon_minus = new Image();
    cache_browser_icon_minus.src = cache_browser.icon_minus;
    $(".cb-dump-hotspot-expand").bind("click", function() {
      var id = $(this).attr("id").replace(/cb-dump-hotspot-expand-/, "");
      $(".cb-dump-block-"+ id).css("display", "inline");
      $(".cb-dump-hotspot-"+ id).attr("src", cache_browser_icon_minus.src);
      this.blur();
      return false;
    });
    $(".cb-dump-hotspot-collapse").bind("click", function() {
      var id = $(this).attr("id").replace(/cb-dump-hotspot-collapse-/, "");
      $(".cb-dump-block-"+ id).css("display", "none");
      $(".cb-dump-hotspot-"+ id).attr("src", cache_browser_icon_plus.src);
      this.blur();
      return false;
    });
    $("pre.cb-dump img").bind("click", function() {
      var id = $(this).attr("id").replace(/cb-dump-hotspot-/, "");
      if ($("#cb-dump-block-"+ id).css("display") != "none") {
        $("#cb-dump-block-"+ id).hide("fast");
        $("#cb-dump-hotspot-"+ id).attr("src", cache_browser_icon_plus.src);
      }
      else {
        $("#cb-dump-block-"+ id).show("fast", function() { $(this).css("display", "inline"); });
        $("#cb-dump-hotspot-"+ id).attr("src", cache_browser_icon_minus.src);
      }
    });
  });
}
