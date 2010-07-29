// $Id: todolist.js,v 1.5 2008/07/31 05:05:49 marvil07 Exp $

$(function() {
  TodoList.add_incomplete_list();

  $.ajaxSetup({
    error: function(xml, status, e) {
      if (status == 'error' && e) {
        alert('An exception occurred in the script. Error name: ' + e.name + '. Error message: ' + e.message);
      } else if (xml.status == 0) {
        // retry
        $.ajax({
          type: this.type,
          url: this.url,
          data: this.data,
          dataType: this.dataType,
        });
      } else
        alert('Unknown AJAX Error: ' + status + ' Status:' + xml.status + ' ' + xml.statusText);
    },
  });

  $('#todolist-add-task-form').submit(function() {
    $.ajax({
      type: 'POST',
      url: getBaseUrl() + 'index.php?q=todolist/create_task',
      data: $('input, select', this).serialize() + '&sort='+($('.todolist.incomplete .task').length+1),
      dataType: 'json',
    });
    this.reset();
    return false;
  });

  // Bind event hooks to complete/uncomplete a task.
  $('.todolist .task').each(TodoList.bind_task);
});

/**
 * Todolist class.
 */
var TodoList = {
  add_incomplete_list: function() {
    if (!$('.todolist.incomplete').length) {
      $('<ul class="todolist incomplete"></ul>').insertBefore('#todolist-add-task-form');
    }

    // create one nub for each incomplete list item
    $('.todolist.incomplete .task').each(TodoList.add_nub);

    if (!$('.todolist.incomplete').isSortable) {
      // make every incomplete todolist sortable
      $('.todolist.incomplete').Sortable({
        accept: 'task',
        axis: 'vertically',
        revert:  true,
        handle: '.reorder',
        onchange: TodoList.incomplete_list_reorder,
//      containment: 'parent', // having trouble
      });
    }
  },

  incomplete_list_reorder: function(obj) {
    serial = $.SortSerialize(obj[0].id);
    $.ajax({
      type: 'POST',
      url: getBaseUrl() + 'index.php?q=todolist/reorder_task',
      data: serial.hash,
    });
  },

  /**
   * Bind event hooks to complete/uncomplete a task.
   */
  bind_task: function() {
    $('input[@type=checkbox]', this).click(function() {
      var element = $(this).parent();
      $.ajax({
        type: 'POST',
        url: getBaseUrl() + 'index.php?q=todolist/toggle_task',
        data: 'id='+this.name+'&checked='+(this.checked? '1' : '0')+'&sort='+(this.checked? -1 : $('.todolist.incomplete .task').length+1),
        dataType: 'json',
        beforeSend: function() { $(element).addClass('loading'); },
        complete: function() { $(element).removeClass('loading'); },
      });
    });
  },

  delete_task: function(id) {
    var element = $('#'+id);
    $.ajax({
      type: 'POST',
      url: getBaseUrl() + 'index.php?q=todolist/delete_task',
      data: 'id='+id,
      dataType: 'json',
      beforeSend: function() { $(element).addClass('loading'); },
      complete: function() { $(element).removeClass('loading'); },
    });
    return false;
  },

  edit_task: function(id) {
    if (id instanceof Object) {
      var element = $(this).parent();
      $.ajax({
        type: 'POST',
        url: getBaseUrl() + 'index.php?q=todolist/edit_task',
        data: $('input, textarea, select', this).serialize(),
        dataType: 'json',
        beforeSend: function() { $(element).addClass('loading'); },
        complete: function() { $(element).removeClass('loading'); },
      });
    } else {
      var element = $('#'+id);
      $.ajax({
        type: 'GET',
        url: getBaseUrl() + 'index.php?q=todolist/edit_task&id='+id,
        dataType: 'json',
        beforeSend: function() { $(element).addClass('loading'); },
        complete: function() { $(element).removeClass('loading'); },
      });
    }
    return false;
  },

  /**
   * Create one nub for each incomplete list item.
   */
  add_nub: function() {
    if (!$('.nub', this).length) {
      $(this).prepend(
        '<div class="nub" style="display:none">' +
        '  <a href="javascript:void(0)" title="Delete this task" class="delete" onclick="return TodoList.delete_task(\''+ $('input[@type=checkbox]', this).attr('name') +'\')">Delete</a>' +
        '  <a href="javascript:void(0)" title="Edit this task" class="edit" onclick="return TodoList.edit_task(\''+ $('input[@type=checkbox]', this).attr('name') +'\')">Edit</a>' +
        '  <a href="javascript:void(0)" title="Reorder this task" class="reorder">Reorder</a>' +
        '</div>'
      ).each(TodoList.bind_nub);
    }
  },

  /**
   * Bind event hooks to hide/show the nub.
   */
  bind_nub: function() {
    if (!$('.nub', this).length) {
      return $(this).each(TodoList.add_nub);
    }

    $(this).mouseover(function() {
      with ($('.nub', this)) {
        TodoList.NubTimer.exec(); // hide all other nubs
        css('display', 'block'); // must render before offsetWidth can be calculated
        css('left', (get(0).offsetWidth*-1)+'px');
        css('top', '-4px');
      }

      TodoList.NubTimer.stop();
    });
    $(this).mouseout(function() { TodoList.NubTimer.start(); });
    $('.nub', this).mouseover(function() { TodoList.NubTimer.stop(); });
    $('.nub', this).mouseout(function() { TodoList.NubTimer.start(); });
  },

  /**
   * Unbind event hooks to hide/show the nub.
   */
  unbind_nub: function() {
    $(this).unbind('mouseover');
    $(this).unbind('mouseout');
    $('.nub', this).unbind('mouseover');
    $('.nub', this).unbind('mouseout');
    TodoList.NubTimer.exec(); // hide all nubs
  },

  /**
   * Nub Timer class.
   */
  NubTimer: {
    tid: 0, // Timer ID

    /**
     * Start the Nub Timer.
     */
    start: function() {
      this.stop();
      this.tid = setTimeout(this.exec, 300); // start timer
    },

    /**
     * Stop the Nub Timer.
     */
    stop: function() {
      if (this.tid) {
        clearTimeout(this.tid); // stop timer
        this.tid = 0;
      }
    },

    /**
     * Execute the Nub Timer.
     */
    exec: function() {
      $('.todolist .nub').css('display', 'none');
      this.stop();
    }
  }
};


function getBaseUrl() {
  var url = '';
  var script = $.grep($('head > script'), function(i) {
    return $(i).attr('src').indexOf('todolist.js') > -1
  });
  url = $(script).attr('src');
  url = url.substring(0, url.indexOf('modules/'));
  if (url.indexOf('sites/') > -1)
    url = url.substring(0, url.indexOf('sites/'));
  return url;
}
