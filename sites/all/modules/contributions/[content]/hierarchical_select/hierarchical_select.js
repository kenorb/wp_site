// $Id: hierarchical_select.js,v 1.85 2008/12/01 01:23:41 wimleers Exp $

(function($) {

Drupal.HierarchicalSelect = {};

Drupal.HierarchicalSelect.state = [];

Drupal.HierarchicalSelect.context = function() {
  return $("form .hierarchical-select-wrapper");
};

Drupal.HierarchicalSelect.initialize = function() {
  if (this.cache != null) {
    this.cache.initialize();
  }

  for (var hsid in Drupal.settings.HierarchicalSelect.settings) {
    Drupal.settings.HierarchicalSelect.settings[hsid]['updatesEnabled'] = true;
    Drupal.HierarchicalSelect.state[hsid] = {};

    this.transform(hsid);
    if (Drupal.settings.HierarchicalSelect.settings[hsid].resizable) {
      this.resizable(hsid);
    }

    this.attachBindings(hsid);
    if (this.cache != null && this.cache.status()) {
      this.cache.load(hsid);
    }

    Drupal.HierarchicalSelect.log(hsid);
  }
};

Drupal.HierarchicalSelect.log = function(hsid) {
  if (Drupal.settings.HierarchicalSelect.initialLog == undefined || Drupal.settings.HierarchicalSelect.initialLog[hsid] == undefined) {
    return;
  }

  // Make sure we print the log of the last build for this Hierarchical Select.
  if (Drupal.HierarchicalSelect.state[hsid].lastBuildNumber == undefined) {
    Drupal.HierarchicalSelect.state[hsid].log = [];
    Drupal.HierarchicalSelect.state[hsid].log[0] = Drupal.settings.HierarchicalSelect.initialLog[hsid];
    Drupal.HierarchicalSelect.state[hsid].lastBuildNumber = -1;
  }
  var lastBuildNumber = ++Drupal.HierarchicalSelect.state[hsid].lastBuildNumber;

  // Print all log messages for the last build.
  console.log("HIERARCHICAL SELECT " + hsid);
  for (var i = 0; i < Drupal.HierarchicalSelect.state[hsid].log[lastBuildNumber].length; i++) {
    console.log(Drupal.HierarchicalSelect.state[hsid].log[lastBuildNumber][i]);
  }
  console.log(' ');
};

Drupal.HierarchicalSelect.transform = function(hsid) {
  var removeString = $('#hierarchical-select-'+ hsid +'-wrapper .dropbox .dropbox-remove:first', Drupal.HierarchicalSelect.context).text();

  $('#hierarchical-select-'+ hsid +'-wrapper', Drupal.HierarchicalSelect.context)
  // Remove the .nojs div.
  .find('.nojs').remove().end()
  // Find all .dropbox-remove cells in the dropbox table.
  .find('.dropbox .dropbox-remove')
  // Hide the children of these table cells. We're not removing them because
  // we want to continue to use the "Remove" checkboxes.
  .find('*').hide().end()
  // Put a "Remove" link there instead.
  .append('<a href="">'+ removeString +'</a>');  
};

Drupal.HierarchicalSelect.resizable = function(hsid) {
  var $selectsWrapper = $('#hierarchical-select-' + hsid + '-wrapper .hierarchical-select .selects', Drupal.HierarchicalSelect.context);

  // No select wrapper present: the user is creating a new item.
  if ($selectsWrapper.length == 0) {
    return;
  }

  // Append the drag handle ("grippie").
  $selectsWrapper.append($('<div class="grippie"></div>'));

  // jQuery object that contains all selects in the hierarchical select, to
  // speed up DOM manipulation during dragging.
  var $selects = $selectsWrapper.find('select');

  var defaultHeight = Drupal.HierarchicalSelect.state[hsid].defaultHeight = $selects.slice(0, 1).height();
  var defaultSize = Drupal.HierarchicalSelect.state[hsid].defaultSize = $selects.slice(0, 1).attr('size');
  defaultSize = (defaultSize == 0) ? 1 : defaultSize;
  var margin = Drupal.HierarchicalSelect.state[hsid].margin = parseInt($selects.slice(0, 1).css('margin-bottom').replace(/^(\d+)px$/, "$1"));

  // Bind the drag event.
  $('.grippie', $selectsWrapper)
  .mousedown(startDrag)
  .dblclick(function() {
    if (Drupal.HierarchicalSelect.state[hsid].resizedHeight == undefined) {
      Drupal.HierarchicalSelect.state[hsid].resizedHeight = defaultHeight;
    }
    var resizedHeight = Drupal.HierarchicalSelect.state[hsid].resizedHeight = (Drupal.HierarchicalSelect.state[hsid].resizedHeight > defaultHeight + 2) ? defaultHeight : 4.6 / defaultSize * defaultHeight;
    Drupal.HierarchicalSelect.resize($selects, defaultHeight, resizedHeight, defaultSize, margin);
  });

  function startDrag(e) {
    staticOffset = $selects.slice(0, 1).height() - e.pageY;
    $selects.css('opacity', 0.25);
    $(document).mousemove(performDrag).mouseup(endDrag);
    return false;
  }

  function performDrag(e) {
    var resizedHeight = staticOffset + e.pageY;
    Drupal.HierarchicalSelect.resize($selects, defaultHeight, resizedHeight, defaultSize, margin);
    return false;
  }

  function endDrag(e) {
    var height = $selects.slice(0, 1).height();

    $(document).unbind("mousemove", performDrag).unbind("mouseup", endDrag);
    $selects.css('opacity', 1);
    if (height != Drupal.HierarchicalSelect.state[hsid].resizedHeight) {
      Drupal.HierarchicalSelect.state[hsid].resizedHeight = (height > defaultHeight) ? height : defaultHeight;
    }
  }
};

Drupal.HierarchicalSelect.resize = function($selects, defaultHeight, resizedHeight, defaultSize, margin) {
  if (resizedHeight == undefined) {
    resizedHeight = defaultHeight;
  }

  $selects
  .attr('size', (resizedHeight > defaultHeight) ? 2 : defaultSize)
  .height(Math.max(defaultHeight + margin, resizedHeight)); // Without the margin component, the height() method would allow the select to be sized to low: defaultHeight - margin.
};

Drupal.HierarchicalSelect.disableForm = function(hsid) {
  // Disable *all* submit buttons in this form, as well as all input-related
  // elements of the current hierarchical select.
  $('form:has(#hierarchical-select-' + hsid +'-wrapper) input[type=submit]')
  .add('#hierarchical-select-' + hsid +'-wrapper .hierarchical-select .selects select')
  .add('#hierarchical-select-' + hsid +'-wrapper .hierarchical-select input')
  .attr('disabled', true);

  // Add the 'waiting' class. Default style: make everything transparent.
  $('#hierarchical-select-' + hsid +'-wrapper').addClass('waiting');

  // Indicate that the user has to wait.
  $('body').css('cursor', 'wait');
};

Drupal.HierarchicalSelect.enableForm = function(hsid) {
  // This method undoes everything the disableForm() method did.

  $e = $('form:has(#hierarchical-select-' + hsid +'-wrapper) input[type=submit]')
  .add('#hierarchical-select-' + hsid +'-wrapper .hierarchical-select input');

  // Don't enable the selects again if they've been disabled because the
  // dropbox limit was exceeded.
  if ($('#hierarchical-select-' + hsid +'-wrapper hierarchical-select-dropbox-limit-warning').length == 0) {
    $e.add('#hierarchical-select-' + hsid +'-wrapper .hierarchical-select .selects select');
  }

  $e.attr('disabled', false);

  $('#hierarchical-select-' + hsid +'-wrapper').removeClass('waiting');

  $('body').css('cursor', 'auto');
};

Drupal.HierarchicalSelect.prepareGETSubmit = function(hsid) {
  // Remove the name attributes of all form elements that end up in GET,
  // except for the "flat select" form element.
  $('#hierarchical-select-'+ hsid +'-wrapper', Drupal.HierarchicalSelect.context)
  .find('input, select')
  .not('.flat-select')
  .removeAttr('name');

  // Update the name attribute of the "flat select" form element
  var $flatSelect = $('#hierarchical-select-'+ hsid +'-wrapper .flat-select', Drupal.HierarchicalSelect.context);
  var newName = $flatSelect.attr('name').replace(/^([a-zA-Z0-9_\-]*)(?:\[flat_select\]){1}(\[\])?$/, "$1$2");
  $flatSelect.attr('name', newName);

  Drupal.HierarchicalSelect.triggerEvents(hsid, 'prepared-GET-submit', {});
};

Drupal.HierarchicalSelect.attachBindings = function(hsid) {
  var addOpString = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select input', Drupal.HierarchicalSelect.context).val();
  var createNewItemOpString = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .create-new-item-create', Drupal.HierarchicalSelect.context).val();
  var cancelNewItemOpString = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .create-new-item-cancel', Drupal.HierarchicalSelect.context).val();

  var data = {};
  data.hsid = hsid;

  $('#hierarchical-select-'+ hsid +'-wrapper', this.context)
  // "disable-updates" event
  .unbind('disable-updates').bind('disable-updates', data, function(e) {
    Drupal.settings.HierarchicalSelect.settings[e.data.hsid]['updatesEnabled'] = false;
  })
  
  // "enforce-update" event
  .unbind('enforce-update').bind('enforce-update', data, function(e, extraPost) {
     Drupal.HierarchicalSelect.update(e.data.hsid, 'enforced-update', { extraPost: extraPost });
  })

  // "prepare-GET-submit" event
  .unbind('prepare-GET-submit').bind('prepare-GET-submit', data, function(e) {
    Drupal.HierarchicalSelect.prepareGETSubmit(e.data.hsid);
  })

  // "update-hierarchical-select" event
  .find('.hierarchical-select .selects select').unbind().change(function(_hsid) {
    return function() {
      if (Drupal.settings.HierarchicalSelect.settings[hsid]['updatesEnabled']) {
        Drupal.HierarchicalSelect.update(_hsid, 'update-hierarchical-select', { select_id : $(this).attr('id') });
      }
    };
  }(hsid)).end()

  // "create-new-item" event
  .find('.hierarchical-select .create-new-item .create-new-item-create').unbind().click(function(_hsid) {
    return function() {
      Drupal.HierarchicalSelect.update(_hsid, 'create-new-item', { opString : createNewItemOpString });
      return false; // Prevent the browser from POSTing the page.
    };
  }(hsid)).end()

  // "cancel-new-item" event"
  .find('.hierarchical-select .create-new-item .create-new-item-cancel').unbind().click(function(_hsid) {
    return function() {
      Drupal.HierarchicalSelect.update(_hsid, 'cancel-new-item', { opString : cancelNewItemOpString });
      return false; // Prevent the browser from POSTing the page (in case of the "Cancel" button).
    };
  }(hsid)).end()

  // "add-to-dropbox" event
  .find('.hierarchical-select .add-to-dropbox').unbind().click(function(_hsid) {
    return function() {
      Drupal.HierarchicalSelect.update(_hsid, 'add-to-dropbox', { opString : addOpString });
      return false; // Prevent the browser from POSTing the page.
    };
  }(hsid)).end()

  // "remove-from-dropbox" event
  // (anchors in the .dropbox-remove cells in the .dropbox table)
  .find('.dropbox .dropbox-remove a').unbind().click(function(_hsid) {
    return function() {
      var isDisabled = $('#hierarchical-select-'+ hsid +'-wrapper', Drupal.HierarchicalSelect.context).attr('disabled');

      // If the hierarchical select is disabled, then ignore this click.
      if (isDisabled) {
        return false;
      }

      // Check the (hidden, because JS is enabled) checkbox that marks this
      // dropbox entry for removal. 
      $(this).parent().find('input[type=checkbox]').attr('checked', true);
      Drupal.HierarchicalSelect.update(_hsid, 'remove-from-dropbox', {});
      return false; // Prevent the browser from POSTing the page.
    };
  }(hsid));
};

Drupal.HierarchicalSelect.preUpdateAnimations = function(hsid, updateType, lastUnchanged, callback) {
  switch (updateType) {
    case 'update-hierarchical-select':
      // Drop out the selects of the levels deeper than the select of the
      // level that just changed.
      var animationDelay = Drupal.settings.HierarchicalSelect.settings[hsid]['animationDelay'];
      var $animatedSelects = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select', Drupal.HierarchicalSelect.context).slice(lastUnchanged);
      if ($animatedSelects.size() > 0) {
        $animatedSelects.hide();
        for (var i = 0; i < $animatedSelects.size(); i++) {
          if (i < $animatedSelects.size() - 1) {
            $animatedSelects.slice(i, i + 1).hide("drop", { direction: "left" }, animationDelay);
          }
          else {
            $animatedSelects.slice(i, i + 1).hide("drop", { direction: "left" }, animationDelay, callback);
          }
        }
      }
      else if (callback) {
        callback();
      }
      break;
    default:
      if (callback) {
        callback();
      }  
      break;
  }
};

Drupal.HierarchicalSelect.postUpdateAnimations = function(hsid, updateType, lastUnchanged, callback) {
  if (Drupal.settings.HierarchicalSelect.settings[hsid].resizable) {
    // Restore the resize.  
    Drupal.HierarchicalSelect.resize(
      $('#hierarchical-select-' + hsid + '-wrapper .hierarchical-select .selects select', Drupal.HierarchicalSelect.context),
      Drupal.HierarchicalSelect.state[hsid].defaultHeight,
      Drupal.HierarchicalSelect.state[hsid].resizedHeight,
      Drupal.HierarchicalSelect.state[hsid].defaultSize,
      Drupal.HierarchicalSelect.state[hsid].margin
    );
  }

  switch (updateType) {
    case 'update-hierarchical-select':
      var $createNewItemInput = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .create-new-item-input', Drupal.HierarchicalSelect.context);
      
      if ($createNewItemInput.size() == 0) {
        // Give focus to the level below the one that has changed, if it
        // exists.
        if (!$.browser.mozilla) { // Don't give focus in Firefox: the user would have to click twice before he can make a selection.
          $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select', Drupal.HierarchicalSelect.context)
          .slice(lastUnchanged, lastUnchanged + 1)
          .focus();
        }
      }
      else {
        // Give focus to the input field of the "create new item/level"
        // section, if it exists, and also select the existing text.
        $createNewItemInput.focus();
        $createNewItemInput[0].select();
      }
      // Hide the loaded selects after the one that was just changed, then
      // drop them in.
      var animationDelay = Drupal.settings.HierarchicalSelect.settings[hsid]['animationDelay'];
      var $animatedSelects = $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select', Drupal.HierarchicalSelect.context).slice(lastUnchanged);
      if ($animatedSelects.size() > 0) {
        $animatedSelects.hide();
        for (var i = 0; i < $animatedSelects.size(); i++) {
          if (i < $animatedSelects.size() - 1) {
            $animatedSelects.slice(i, i + 1).show("drop", { direction: "left" }, animationDelay);
          }
          else {
            $animatedSelects.slice(i, i + 1).show("drop", { direction: "left" }, animationDelay, callback);
          }
        }
      }
      else if (callback) {
        callback();
      }
      break;

    case 'create-new-item':
      // Make sure that other Hierarchical Selects that represent the same
      // hierarchy are also updated, to make sure that they have the newly
      // created item!
      var cacheId = Drupal.settings.HierarchicalSelect.settings[hsid].cacheId;
      for (var otherHsid in Drupal.settings.HierarchicalSelect.settings) {
        if (Drupal.settings.HierarchicalSelect.settings[otherHsid].cacheId == cacheId) {
          $('#hierarchical-select-'+ otherHsid +'-wrapper')
          .trigger('enforce-update');
        }
      }
      // TRICKY: NO BREAK HERE!

    case 'cancel-new-item':
      // After an item/level has been created/cancelled, reset focus to the
      // beginning of the hierarchical select.
      $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select', Drupal.HierarchicalSelect.context)
      .slice(0, 1)
      .focus();

      if (callback) {
        callback();
      }
      break;

    default:
      if (callback) {
        callback();
      }
      break;
  }
};

Drupal.HierarchicalSelect.triggerEvents = function(hsid, updateType, settings) {
  $('#hierarchical-select-'+ hsid +'-wrapper', Drupal.HierarchicalSelect.context)
  .trigger(updateType, [ hsid, settings ]);
};

Drupal.HierarchicalSelect.update = function(hsid, updateType, settings) {
  var post = $('form:has(#hierarchical-select-' + hsid +'-wrapper)', Drupal.HierarchicalSelect.context).formToArray();

  // Pass the hierarchical_select id via POST.
  post.push({ name : 'hsid', value : hsid });
  
  // If a cache system is installed, let the server know if it's running
  // properly. If it is running properly, the server will send back additional
  // information to maintain a lazily-loaded cache.
  if (Drupal.HierarchicalSelect.cache != null) {
    post.push({ name : 'client_supports_caching', value : Drupal.HierarchicalSelect.cache.status() });
  }

  // updateType is one of:
  // - 'none' (default)
  // - 'update-hierarchical-select'
  // - 'enforced-update'
  // - 'create-new-item'
  // - 'remove-from-dropbox'
  switch (updateType) {
    case 'update-hierarchical-select':
      var value = $('#'+ settings.select_id).val();
      var lastUnchanged = parseInt(settings.select_id.replace(/^.*-hierarchical-select-selects-(\d+)$/, "$1")) + 1;
      var optionClass = $('#'+ settings.select_id).find('option[@value='+ value +']').attr('class');

      // Don't do anything (also no callback to the server!) when the selected
      // item is:
      // - the '<none>' option, or
      // - a level label, or
      // - an option of class 'has-no-children', and
      //   (the renderFlatSelect setting is disabled or the dropbox is enabled)
      //   and
      //   (the createNewLevels setting is disabled).
      if (value == 'none'
          || value.match(/^label_\d+$/)
          || (optionClass == 'has-no-children'
             &&
             (
               (Drupal.settings.HierarchicalSelect.settings[hsid]['renderFlatSelect'] == false
                || $('#hierarchical-select-'+ hsid +'-wrapper .dropbox').length > 0
               )
               &&
               Drupal.settings.HierarchicalSelect.settings[hsid]['createNewLevels'] == false
             )
           )
         )
      {
        Drupal.HierarchicalSelect.preUpdateAnimations(hsid, updateType, lastUnchanged, function() {
          // Remove the sublevels.
          $('#hierarchical-select-'+ hsid +'-wrapper .hierarchical-select .selects select', Drupal.HierarchicalSelect.context)
          .slice(lastUnchanged)
          .remove();

          // The selection of this hierarchical select has changed!
          Drupal.HierarchicalSelect.triggerEvents(hsid, 'change-hierarchical-select', settings);
        });
        return;
      }
      break;
    
    case 'enforced-update':
      post = post.concat(settings.extraPost);
      break;

    case 'create-new-item':
    case 'cancel-new-item':
    case 'add-to-dropbox':
      post.push({ name : 'op', value : settings.opString });
      break;
  }

  // Construct the object that contains the options for a callback to the
  // server. If a client-side cache is found however, it's possible that this
  // won't be used.
  var ajaxOptions = {
    url:        Drupal.settings.HierarchicalSelect.url,
    type:       'POST',
    dataType:   'json',
    data:       post,
    beforeSend: function() {
      Drupal.HierarchicalSelect.triggerEvents(hsid, 'before-' + updateType, settings);
      Drupal.HierarchicalSelect.disableForm(hsid); 
    },
    success:    function(response) {
      // Replace the old HTML with the (relevant part of) retrieved HTML.
      $('#hierarchical-select-'+ hsid +'-wrapper', Drupal.HierarchicalSelect.context)
      .html($('.hierarchical-select-wrapper > *', $(response.output)));

      // Transform the hierarchical select and/or dropbox to the JS variant,
      // make it resizable again and re-enable the disabled form items.
      Drupal.HierarchicalSelect.transform(hsid);
      if (Drupal.settings.HierarchicalSelect.settings[hsid].resizable) {
        Drupal.HierarchicalSelect.resizable(hsid);
      }
      Drupal.HierarchicalSelect.enableForm(hsid);

      Drupal.HierarchicalSelect.postUpdateAnimations(hsid, updateType, lastUnchanged, function() {
        // Reattach the bindings.
        Drupal.HierarchicalSelect.attachBindings(hsid);

        // Update the client-side cache when:
        // - information for in the cache is provided in the response, and
        // - the cache system is available, and
        // - the cache system is running.
        if (response.cache != null && Drupal.HierarchicalSelect.cache != null && Drupal.HierarchicalSelect.cache.status()) {
          Drupal.HierarchicalSelect.cache.sync(hsid, response.cache);
        }

        if (response.log != undefined) {
          Drupal.HierarchicalSelect.state[hsid].log.push(response.log);
          Drupal.HierarchicalSelect.log(hsid);
        }

        Drupal.HierarchicalSelect.triggerEvents(hsid, updateType, settings);

        if (updateType == 'update-hierarchical-select') {
          // The selection of this hierarchical select has changed!
          Drupal.HierarchicalSelect.triggerEvents(hsid, 'change-hierarchical-select', settings);
        }
      });
    }
  };

  // Use the client-side cache to update the hierarchical select when:
  // - the hierarchical select is being updated (i.e. no add/remove), and
  // - the renderFlatSelect setting is disabled, and
  // - the createNewItems setting is disabled, and
  // - the cache system is available, and
  // - the cache system is running.
  // Otherwise, perform a normal dynamic form submit.
  if (updateType == 'update-hierarchical-select'
      && Drupal.settings.HierarchicalSelect.settings[hsid]['renderFlatSelect'] == false
      && Drupal.settings.HierarchicalSelect.settings[hsid]['createNewItems'] == false
      && Drupal.HierarchicalSelect.cache != null
      && Drupal.HierarchicalSelect.cache.status())
  {
    Drupal.HierarchicalSelect.cache.updateHierarchicalSelect(hsid, value, settings, lastUnchanged, ajaxOptions);
  }
  else {
    Drupal.HierarchicalSelect.preUpdateAnimations(hsid, updateType, lastUnchanged, function() {
      $.ajax(ajaxOptions); 
    });
  }
};

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    // If you set Drupal.settings.HierarchicalSelect.pretendNoJS to *anything*,
    // and as such, Hierarchical Select won't initialize its Javascript! It
    // will seem as if your browser had Javascript disabled.
    if (undefined != Drupal.settings.HierarchicalSelect.pretendNoJS) {
      return false;
    }
    
    Drupal.HierarchicalSelect.initialize();
  });
}

})(jQuery);