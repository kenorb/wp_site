function storm_empty_select(_select) {
  if(_select == undefined) return;
  for (var i=_select.length-1; i>=0; i--) {
    _select.remove(i);
  }
};

function storm_fill_select(_select, _items, _with_all_option, _all_text) {
  if(_select == undefined) return;

  if(_with_all_option) {
    opt=new Option();
    opt.value = 0;
    opt.text = _all_text;
    try
    {
      _select.add(opt, null); // standards compliant
    }
    catch(ex)
    {
      _select.add(opt); // IE only
    }
  }

  for (key in _items) {
    opt=new Option();
    opt.value = key;
    opt.text = _items[key];
    try
    {
      _select.add(opt, null); // standards compliant
    }
    catch(ex)
    {
      _select.add(opt); // IE only
    }
  }
};

function storm_popup(sender, name, title, width, height, content_id, position) {
  var p_name = "storm" + name + '_popup';
  var p_close = "storm" + name + "_popup_close";

  var a = $(sender);
  var top = a.offset().top;
  var left = a.offset().left;

  switch (position) {
    case 'l':
      left = left-width;
    break;
    case 'lt':
      left = left - width;
      top = top - height;   
    break;
    case 't':
      left = left - Math.floor(width / 2); 
      top = top - height; 
    break;
    case 'rt':
      top = top - height; 
    break;
    case 'r':
    break;
    case 'rb':
    break;
    case 'b':
      left = left - Math.floor(width / 2); 
    break;
    case 'lb':
      left = left-width;
    break;
  }

  $("#" + p_name).remove();
  var popup = '<div class="storm_popup" id="' + p_name + '">';
  popup += '<div class="storm_popup_title" id="' + p_close + '">' + title + '</div>';
  popup += '<div class="storm_popup_inner">';
  popup += $("#" + content_id).html();
  popup += "</div>";
  popup += "</div>";
  $("body").append(popup);
  var p = $("#" + p_name);
  p.css('position', 'absolute');
  p.css('top', top);
  p.css('left', left);
  p.css('width', width);
  p.css('height', height);
  p.show();
  $("#" + p_close).click(function(){
    $("#" + p_name).remove();
    return false;
  });
};

function storm_datext_tonull(sender, date_id) {
  if (sender.value == "-1") {
    $("#" + date_id + '-day').val("-1");
    $("#" + date_id + '-month').val("-1");
    $("#" + date_id + '-year').val("-1");
  }
};
