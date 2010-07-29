// $Id: jquery.uauto.js,v 1.2 2008/03/08 10:26:36 garthee Exp $
/**
 * Module - an addon to uLink
 *
 * @file
 * Provides the client  side handling and UI - auto completition of links
 */
Drupal.behaviors.uauto = function (context) {
  var info = [];
  $('#edit-uauto-path:not(.uatuo-processed)', context).each(function () {
    info['path'] = this.value;
    $(this).addClass('uauto-processed');
  });
  $('#edit-uauto-isearch:not(.uatuo-processed)', context).each(function () {
    info['i'] = this.value;
    $(this).addClass('uauto-processed');
  });
  $('#edit-body:not(.uauto-processed)', context).each(function () {
    new Drupal.jsUA(this, info);
    $(this).addClass('uauto-processed');
  });  
};
/**
 * An UAUTO object
 */
Drupal.jsUA = function (input,info) {
  
  var ua = this;
  
  this.input = input;
  this.uri = info['path'];
  this.mode = 0;
  this.delay = 500;
  this.cache = {};
  this.cursorPos = 0;
  this.tokenString = "";
  this.lastTitle = "";
  
  $(this.input)
    .keydown(function (event) { return ua.onkeydown(this, event); })
    .keypress(function (event) { ua.keypress(this, event); })
    .blur(function () { ua.cancel(); ua.hidePopup(); });
  if (info['i']) {
    $(this.input).mouseup(function (event) { ua.mouseup(this, event); });
  }

};
/**
 * Handler for the "keydown" event
 */
Drupal.jsUA.prototype.onkeydown = function (input, e) {
  if (!e) {
    e = window.event;
  }

  switch (e.keyCode) {
    case 40: // down arrow
      this.selectDown();
      return false;
    case 38: // up arrow
      this.selectUp();
      return false;
    case 27: // esc
      this.mode = 0;
      this.input.blur();      
      return false;
    case 9:   //tab
    case 13:  //enter
      if (this.selected) {
        this.cursorPos--;
        this.select(this.selected);
        return false; 
      }            
    default: // all other keys
      return true;
      
  }
};
/**
 * Cancel the request
 */
Drupal.jsUA.prototype.cancel = function() {
  if (this) this.setStatus('cancel');
  if (this.timer) clearTimeout(this.timer);
};
/**
 * Handler for the "keypress" event
 */
Drupal.jsUA.prototype.keypress = function (input, e) {
  if (!e) {
    e = window.event;
  }
  
  var keyCode =   e.charCode ? e.charCode : e.keyCode ? e.keyCode : e.which ? e.which : void 0;
  this.cursorPos = this.get_cursorpos(this.input);
  switch (keyCode) {
    case 16: // shift
    case 17: // ctrl
    case 18: // alt
    case 20: // caps lock
    case 33: // page up
    case 34: // page down
    case 35: // end
    case 36: // home
    case 37: // left arrow
    case 38: // up arrow
    case 39: // right arrow
    case 40: // down arrow
      return true;
    case 93:
    case 91:
      this.hidePopup();
      this.mode = 0;
      return true;
    case 124:
      this.mode = this.checkStart(this.cursorPos);
      this.tokenString = "";
      if (this.mode == 2 || this.mode == 3) {    
        this.populatePopup(); 
      }
      if (this.mode == 4) {  
        alert("Invalide tag detected","uAuto");
        this.mode = 0;
        return false;
      }
      return true;   
    default:
      if (this.mode == 1 || this.mode == 3) {
        this.tokenString = this.getToken(this.input.value.substring(0, this.cursorPos), "|") + String.fromCharCode(keyCode);
        if (this.tokenString.length > 1)
          this.populatePopup();
        break;
      }
      break;      
  }
  return true;
}; 
/**
 * Handler for the "Mouseup" event
 */
Drupal.jsUA.prototype.mouseup = function (input, e) {
	var text = this.selectedText(false, '');
  if (text.length < 3) 
    return;    
  
  var token = this.checkStart(this.get_cursorpos(this.input));
  if (token > 0 && token < 4) {
    alert("Slection is detected within a uLink tag. Use ksearch instead i.e type '|' before the token to be edited to trigger it.");
  }
  else {
    this.mode = 4;
    this.tokenString = text;
    this.populatePopup(e.layerY); 
  }  
}; 
/**
 * Positions the suggestions popup and starts a search
 */		
Drupal.jsUA.prototype.populatePopup = function (y, yy) {
  // Show popup   
  if (this.popup) {
    $(this.popup).remove();
  }
  this.selected = false;
  this.popup = document.createElement('div');
  $(this.popup).addClass('uauto-background');
  this.popup.id = 'uauto';
 
  this.popup.owner = this; 
  if (y) {
    y = y + 15;
    posit = 'absolute';
  }
  else {
    y = 0;
    posit = 'static';
 
  }
  $(this.popup).css({
    marginTop:  y +'px',
    width: (this.input.offsetWidth - 4) +'px',
    position: posit,
    display: 'none'
  });
  $(this.input).before(this.popup); 
  this.search();
};
/**
 * Unhighlights a suggestion
 */
Drupal.jsUA.prototype.unhighlight = function (node) {
  $(node).removeClass('selected');
  this.selected = false;
};
/**
 * Sets the status and displays throbbing indicator
 */
Drupal.jsUA.prototype.setStatus = function (status) {
  switch (status) {
    case 'begin':
      $(this.input).addClass('throbbing');
      break;
    case 'cancel':
    case 'error':
    case 'found':
      $(this.input).removeClass('throbbing');
      break;
  }
};
/**
 * Performs a cached and delayed search
 */
Drupal.jsUA.prototype.search = function () {
  var db = this;
  var searchString;
  if (db.mode == 3 && !db.tokenString)
    searchString = "macro";
  else
    searchString = this.tokenString;
    
  // See if this key has been searched for before
  if (this.cache[searchString]) {
    return this.found(this.cache[searchString]);
  }

  // Initiate delayed search
  if (db.timer) {
    clearTimeout(db.timer);
  }
  
  if (db.mode == 1 || db.mode == 3 || db.mode == 4) { 
    db.timer = setTimeout(function() {
      db.setStatus('begin');
      // Ajax GET request for autocompletion      
      $.ajax({
        type: "GET",
        url: db.uri +'/' + db.mode + '/' + (Drupal.encodeURIComponent(searchString)).replace('/','@$!'),
        dataType: 'json',
        success: function (matches) {
          if (typeof matches['status'] == 'undefined' || matches['status'] != 0) {
            db.cache[searchString] = matches;
            db.found(matches);
            db.setStatus('found');
          }
        },
        error: function (xmlhttp) {
          alert(Drupal.ahahError(xmlhttp, db.uri));
        }
      });
    }, db.delay);
  }
  else if (this.mode == 2) {
    var matches = new Array  
    matches[this.lastTitle] = this.lastTitle;
    db.found(matches);
  }
};
/**
 * Fills the suggestion popup with any matches received
 */
Drupal.jsUA.prototype.found = function (matches) {
  // If no value in the textfield, do not show the popup.
  var ua = this;
  if (!this.input.value.length) {
    return false;
  }

  // Prepare matches  
  var ul = document.createElement('ul');
  if (this.mode == 3) {//macro array is defined in reverse order
    for (key in matches) {
      var li = document.createElement('li');
      $(li)
        .addClass('uauto')
        .html('<div>' + key + '</div>')
        .click(function () { ua.select(this); })
        .mouseover(function () { ua.highlight(this); })
        .mouseout(function () { ua.unhighlight(this); });
      li.key = key;
      li.title = matches[key]; 
      $(ul).append(li);
    }
  }
  else {
    for (key in matches) {
      var li = document.createElement('li');
      $(li)
        .addClass('uauto')
        .html('<div>' + matches[key] + '</div>')
        .click(function () { ua.select(this); })
        .mouseover(function () { ua.highlight(this); })
        .mouseout(function () { ua.unhighlight(this); });
      li.key = matches[key];
      li.title = key;
      $(ul).append(li);
    }
  }
  // Show popup with matches, if any
  if (this.popup) {
    if (ul.childNodes.length > 0) {
      $(this.popup).empty().append(ul).show();
    }
    else {
      $(this.popup).css({visibility: 'hidden'});
      this.hidePopup();
    }
  }
};
/**
 * Highlights a suggestion
 */
Drupal.jsUA.prototype.highlight = function (node) {
  if (this.selected) {
    $(this.selected).removeClass('selected');
  }
  $(node).addClass('selected');
  this.selected = node;
};
/**
 * Highlights the next suggestion
 */
Drupal.jsUA.prototype.selectDown = function () {
  if (this.selected && this.selected.nextSibling) {
    this.highlight(this.selected.nextSibling);
  }
  else {
    var lis = $('li', this.popup);
    if (lis.size() > 0) {
      this.highlight(lis.get(0));
    }
  }
};
/**
 * Highlights the previous suggestion
 */
Drupal.jsUA.prototype.selectUp = function () {
  if (this.selected && this.selected.previousSibling) {
    this.highlight(this.selected.previousSibling);
  }
};
/**
 * Hides the autocomplete suggestions
 */
Drupal.jsUA.prototype.hidePopup = function (keycode) {  // Select item if the right key or mousebutton was pressed
  // Hide popup
  var popup = this.popup;
  if (popup) {
    this.popup = null;
    $(popup).fadeOut('fast', function() { $(popup).remove(); });
  }
  this.selected = false;
  if (this.mode != 4) this.mode = 0;
};
/**
 * Puts the currently highlighted suggestion into the autocomplete field
 */
Drupal.jsUA.prototype.select = function (node) {
  if (this.mode == 4) {    
    this.selectedText(true, "[l|" + node.title + "|" + this.tokenString);
  }
  else {
    if (this.mode == 1)
      this.lastTitle  = node.key;
    this.insertText(this.cursorPos, this.tokenString.length, node.title);
  }  
  //save the selected results in  the teble	
  if (this.cache[this.tokenString] && (this.mode == 1 || this.mode == 4)) {
    var temp = this.tokenString+"#$#"+node.title+"#$#"+node.key;
    $.ajax({
      type: "GET",
      url: this.uri + '/5/' + (Drupal.encodeURIComponent(temp)).replace('/','@$!'),
      error: function (xmlhttp) {
        alert(Drupal.ahahError(xmlhttp, db.uri));
      }
    });
  }		
};
/**
* check whether to start ksearch. i.e. proper tag is found
* @returns 
* 0 - not found
* 1,2,3 - token type
* 4 - improper tag - inform user
*/
Drupal.jsUA.prototype.checkStart = function(pos) {
  var keyword = "[l";
  var temp = this.input.value.substring(this.input.value.lastIndexOf(keyword, pos), pos);
  if (temp.indexOf(']') == -1) {
    var tokens = temp.split('|', 4);
    if(tokens[0] == keyword) {
      return tokens.length;
    }
    else {
      return 0;
    }
  }
  return 0;
}
//maybe this will be needed in several places in future :-)
Drupal.jsUA.prototype.getToken = function (text, delim) {
  return text.slice(text.lastIndexOf(delim)+1, text.length);
}  
/**
* insert text at the cursor position
*/
Drupal.jsUA.prototype.insertText = function (cursorPos, vid_len, text) {

  if ($.browser.safari || $.browser.opera || $.browser.mozilla) {
    var oldStringFirst = this.input.value.substring(0, cursorPos-vid_len+1);
    var oldStringLast =  this.input.value.substring(cursorPos+1);   
    this.input.value =   oldStringFirst + text + oldStringLast;
    var len = this.input.value.length -  oldStringLast.length;
    this.input.focus();
    this.input.setSelectionRange(len, len);
  }
  else if ($.browser.msie){
    this.input.focus();
    var caretPos = document.selection.createRange();
    caretPos.moveStart("character",-1*(vid_len));
    caretPos.select();
    caretPos.text = text;
    caretPos.select();       
  }  
}
/**
 * cursorpos currently depends on the browser, getting the cursor position
 */
Drupal.jsUA.prototype.get_cursorpos = function () {
 // this.input.focus(); 
  if ($.browser.safari || $.browser.opera || $.browser.mozilla) {  
    if (typeof this.input.setSelectionRange != 'undefined') {
      return this.input.selectionStart;      
    }     
  }
 else if($.browser.msie) {   //IE doesn't have a direct method to get the cursor position
    if ( this.input.isTextEdit ) { 
      this.input.caretPos = document.selection.createRange(); 
    }
    this.input.caretPos.text = '#@#@#'; // a unique string
    var pos = this.input.value.indexOf('#@#@#', 0);
    this.input.caretPos.select();
    this.input.caretPos.moveStart("character",-5);
    this.input.caretPos.select();
    this.input.caretPos.text = '';    
    return pos;
  }
  else {
    return length(this.input.value);    
  } 
}
/**
 * get the user selected text on mouse click and replaces the text with FOUND
 */
// 
Drupal.jsUA.prototype.selectedText = function  (set, text) { 
  this.input.focus();	
	if (set) {
	  if (typeof this.input.setSelectionRange != 'undefined') {
			this.input.value = this.input.value.substring(0, this.input.selectionStart) + text +  this.input.value.substring(this.input.selectionEnd);
			this.input.focus();
			this.input.setSelectionRange(selectionStart + text.length, selectionStart + text.length);
		}
		else {
		  userSelection.text = text;
			userSelection.select();    
		}
	}
	else {
	  if (typeof this.input.setSelectionRange != 'undefined') {
		  selectionStart = this.input.selectionStart;
			selectionEnd = this.input.selectionEnd;
			return this.input.value.substring(this.input.selectionStart, this.input.selectionEnd);      
		} 
		else if (document.selection) { // should come last; Opera!
			var selText = userSelection = document.selection.createRange();
			if (userSelection.text)
				selText = userSelection.text;
			return selText;
		}	  
	}	
}