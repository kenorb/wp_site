Drupal.timeline = {
  timerID: null,

  getTheme: function() {
    var theme = Timeline.ClassicTheme.create();
    theme.event.label.width = 250; // px
    theme.event.bubble.width = 250;
    theme.event.bubble.height = 200;
    return theme;
  },

  createWidget: function(id, orientation, initialDate, timeZone, bands, summary, url, feedType) {
    var theme = Drupal.timeline.getTheme();
    var bandInfos = [];
    var Sources = [];

    var width = 100;
    if (bands.length === 1 && summary !== '') {
      width = width - 25;
    }
    else{
      width = 100 / bands.length;
    }

    $.each(bands, function(i, v) {
      var eventSource = new Timeline.DefaultEventSource();
      Sources.push(eventSource);
      bandInfos.push(
        Timeline.createBandInfo({
          width:          width + '%',
          intervalUnit:   Timeline.DateTime[v],
          intervalPixels: 200,
          eventSource:    eventSource,
          date:           initialDate,
          timeZone:       timeZone,
          theme:          theme
        })
      );
      
      if (bandInfos.length > 1) {
        bandInfos[bandInfos.length-1].syncWith = 0;
      }
    });

    if (bands.length === 1 && summary !== '') {
      bandInfos.push(
        Timeline.createBandInfo({
          width:          '25%',
          intervalUnit:   Timeline.DateTime[summary],
          intervalPixels: 200,
          eventSource:    Sources[0],
          date:           initialDate,
          timeZone:       timeZone,
          theme:          theme,
          showEventText:  false,
          trackHeight:    0.5,
          trackGap:       0.2
        })
      );

      bandInfos[1].syncWith = 0;
      bandInfos[1].highlight = true;
      bandInfos[1].eventPainter.setLayout(bandInfos[0].eventPainter.getLayout());
    }

    var timeline = Timeline.create(document.getElementById(id), bandInfos, orientation);

    if (feedType == 'json') {
      timeline.loadJSON(url, function(json, url) {
        $.each(json.events, function(i, v) {
          Sources[i].loadJSON({dateTimeFormat: json.dateTimeFormat, events: v}, url);
        });
      });
    }
    else {
      timeline.loadXML(url, function(xml, url) { eventSource.loadXML(xml, url); });
    }

    return timeline;
  },

  // Are we using this function? -jm
  centerTimeline: function(timeline, date) {
    timeline.getBand(0).setCenterVisibleDate(Timeline.DateTime.parseGregorianDateTime(date));
  }
};

Drupal.timeline.controls = {
  setup: function(div, timeline, filterLabel, highlightLabel, clearButton) {
    var bandIndices = [0, 1];
    var theme = Drupal.timeline.getTheme();

    var table = document.createElement('table');
    var tr = table.insertRow(0);
    var td = tr.insertCell(0);
    td.innerHTML = filterLabel;

    td = tr.insertCell(1);
    td.innerHTML = highlightLabel;

    var handler = function(elmt, evt, target) {
      if (Drupal.timeline.TimerID !== null) {
        window.clearTimeout(Drupal.timeline.TimerID);
      }
      timelineTimerID = window.setTimeout(function() {
        Drupal.timeline.controls.filter(timeline, bandIndices, table);
      }, 300);
    };

    tr = table.insertRow(1);
    tr.style.verticalAlign = 'top';

    td = tr.insertCell(0);

    var input = document.createElement('input');
    input.type = 'text';
    Timeline.DOM.registerEvent(input, 'keypress', handler);
    td.appendChild(input);

    for (var i = 0; i < theme.event.highlightColors.length; i++) {
      td = tr.insertCell(i + 1);

      input = document.createElement('input');
      input.type = 'text';
      Timeline.DOM.registerEvent(input, 'keypress', handler);
      td.appendChild(input);

      var divColor = document.createElement('div');
      divColor.style.height = '0.5em';
      divColor.style.background = theme.event.highlightColors[i];
      td.appendChild(divColor);
    }

    td = tr.insertCell(tr.cells.length);
    var button = document.createElement('button');
    button.innerHTML = clearButton;
    Timeline.DOM.registerEvent(button, 'click', function() {
      Drupal.timeline.controls.clear(timeline, bandIndices, table);
    });
    td.appendChild(button);

    document.getElementById(div).appendChild(table);
  },

  cleanString: function(s) {
    return s.replace(/^\s+/, '').replace(/\s+$/, '');
  },

  filter: function(timeline, bandIndices, table) {
    Drupal.timeline.TimerID = null;

    var tr = table.rows[1];
    var text = Drupal.timeline.controls.cleanString(tr.cells[0].firstChild.value);

    var filterMatcher = null;
    if (text.length > 0) {
      var regex = new RegExp(text, 'i');
      filterMatcher = function(evt) {
        return regex.test(evt.getText()) || regex.test(evt.getDescription());
      };
    }

    var regexes = [];
    var hasHighlights = false;
    for (var x = 1; x < tr.cells.length - 1; x++) {
      var input = tr.cells[x].firstChild;
      var text2 = Drupal.timeline.controls.cleanString(input.value);
      if (text2.length > 0) {
        hasHighlights = true;
        regexes.push(new RegExp(text2, 'i'));
      }
      else {
        regexes.push(null);
      }
    }

    var highlightMatcher = hasHighlights ? function(evt) {
      var text = evt.getText();
      var description = evt.getDescription();
      for (var x = 0; x < regexes.length; x++) {
        var regex = regexes[x];
        if (regex !== null && (regex.test(text) || regex.test(description))) {
          return x;
        }
      }
      return -1;
    } : null;

    for (var i = 0; i < bandIndices.length; i++) {
      var bandIndex = bandIndices[i];
      timeline.getBand(bandIndex).getEventPainter().setFilterMatcher(filterMatcher);
      timeline.getBand(bandIndex).getEventPainter().setHighlightMatcher(highlightMatcher);
    }
    timeline.paint();
  },

  clear: function(timeline, bandIndices, table) {
    var tr = table.rows[1];
    for (var x = 0; x < tr.cells.length - 1; x++) {
      tr.cells[x].firstChild.value = '';
    }
    for (var i = 0; i < bandIndices.length; i++) {
      var bandIndex = bandIndices[i];
      timeline.getBand(bandIndex).getEventPainter().setFilterMatcher(null);
      timeline.getBand(bandIndex).getEventPainter().setHighlightMatcher(null);
    }
    timeline.paint();
  }
};
