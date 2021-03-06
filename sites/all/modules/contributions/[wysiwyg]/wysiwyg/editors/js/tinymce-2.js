// $Id: tinymce-2.js,v 1.8 2009/02/01 09:20:48 sun Exp $

/**
 * Initialize editor instances.
 *
 * This function needs to be called before the page is fully loaded, as
 * calling tinyMCE.init() after the page is loaded breaks IE6.
 *
 * @param editorSettings
 *   An object containing editor settings for each input format.
 */
Drupal.wysiwyg.editor.init.tinymce = function(settings) {
  // If JS compression is enabled, TinyMCE is unable to find its own base path
  // and exec mode, hence we need to define it manually.
  // @todo Move global library settings somewhere else.
  tinyMCE.baseURL = Drupal.settings.wysiwyg.editorBasePath;
  tinyMCE.srcMode = (Drupal.settings.wysiwyg.execMode == 'src' ? '_src' : '');
  tinyMCE.gzipMode = (Drupal.settings.wysiwyg.execMode == 'gzip');

  // Initialize editor configurations.
  for (var format in settings) {
    tinyMCE.init(settings[format]);
  }
  // Load native external plugins.
  for (var plugin in Drupal.settings.wysiwyg.plugins.tinymce.native) {
    tinyMCE.loadPlugin(plugin, Drupal.settings.wysiwyg.plugins.tinymce.native[plugin]);
  }
  // Load Drupal plugins.
  for (var plugin in Drupal.settings.wysiwyg.plugins.tinymce.drupal) {
    Drupal.wysiwyg.editor.instance.tinymce.addPlugin(plugin, Drupal.settings.wysiwyg.plugins.tinymce.drupal[plugin], Drupal.settings.wysiwyg.plugins.drupal[plugin]);
  }
};

/**
 * Attach this editor to a target element.
 *
 * See Drupal.wysiwyg.editor.attach.none() for a full desciption of this hook.
 */
Drupal.wysiwyg.editor.attach.tinymce = function(context, params, settings) {
  // Configure editor settings for this input format.
  for (var setting in settings) {
    tinyMCE.settings[setting] = settings[setting];
  }
  // Attach editor.
  tinyMCE.execCommand('mceAddControl', true, params.field);
};

/**
 * Detach a single or all editors.
 *
 * See Drupal.wysiwyg.editor.detach.none() for a full desciption of this hook.
 */
Drupal.wysiwyg.editor.detach.tinymce = function(context, params) {
  if (typeof params != 'undefined') {
    tinyMCE.removeMCEControl(tinyMCE.getEditorId(params.field));
    $('#' + params.field).removeAttr('style');
  }
//  else if (tinyMCE.activeEditor) {
//    tinyMCE.triggerSave();
//    tinyMCE.activeEditor.remove();
//  }
};

Drupal.wysiwyg.editor.instance.tinymce = {
  addPlugin: function(plugin, settings, pluginSettings) {
    if (typeof Drupal.wysiwyg.plugins[plugin] != 'object') {
      return;
    }
    tinyMCE.addPlugin(plugin, {

      // Register an editor command for this plugin, invoked by the plugin's button.
      execCommand: function(editor_id, element, command, user_interface, value) {
        switch (command) {
          case plugin:
            if (typeof Drupal.wysiwyg.plugins[plugin].invoke == 'function') {
              var ed = tinyMCE.getInstanceById(editor_id);
              var data = { format: 'html', node: ed.getFocusElement(), content: ed.getFocusElement() };
              Drupal.wysiwyg.plugins[plugin].invoke(data, pluginSettings, ed.formTargetElementId);
              return true;
            }
        }
        // Pass to next handler in chain.
        return false;
      },

      // Register the plugin button.
      getControlHTML: function(control_name) {
        switch (control_name) {
          case plugin:
            return tinyMCE.getButtonHTML(control_name, settings.iconTitle, settings.icon, plugin);
        }
        return '';
      },

      // Load custom CSS for editor contents on startup.
      initInstance: function(ed) {
        if (settings.css) {
          tinyMCE.importCSS(ed.getDoc(), settings.css);
        }
      },

      cleanup: function(type, content) {
        switch (type) {
          case 'insert_to_editor':
            // Attach: Replace plain text with HTML representations.
            if (typeof Drupal.wysiwyg.plugins[plugin].attach == 'function') {
              content = Drupal.wysiwyg.plugins[plugin].attach(content, pluginSettings, tinyMCE.selectedInstance.editorId);
              content = Drupal.wysiwyg.editor.instance.tinymce.prepareContent(content);
            }
            break;

          case 'get_from_editor':
            // Detach: Replace HTML representations with plain text.
            if (typeof Drupal.wysiwyg.plugins[plugin].detach == 'function') {
              content = Drupal.wysiwyg.plugins[plugin].detach(content, pluginSettings, tinyMCE.selectedInstance.editorId);
            }
            break;
        }
        // Pass through to next handler in chain
        return content;
      },

      // isNode: Return whether the plugin button should be enabled for the
      // current selection.
      handleNodeChange: function(editor_id, node, undo_index, undo_levels, visual_aid, any_selection) {
        if (node == null) {
          return;
        }
        if (typeof Drupal.wysiwyg.plugins[plugin].isNode == 'function') {
          if (Drupal.wysiwyg.plugins[plugin].isNode(node)) {
            tinyMCE.switchClass(editor_id + '_' + plugin, 'mceButtonSelected');
            return true;
          }
        }
        tinyMCE.switchClass(editor_id + '_' + plugin, 'mceButtonNormal');
        return true;
      },

      /**
       * Return information about the plugin as a name/value array.
       */
      getInfo: function() {
        return {
          longname: settings.title
        };
      }
    });
  },

  openDialog: function(dialog, params) {
    var editor = tinyMCE.getInstanceById(this.field);
    tinyMCE.openWindow({
      file: dialog.url + '/' + this.field,
      width: dialog.width,
      height: dialog.height,
      inline: 1
    }, params);
  },

  closeDialog: function(dialog) {
    var editor = tinyMCE.getInstanceById(this.field);
    tinyMCEPopup.close();
  },

  prepareContent: function(content) {
    // Certain content elements need to have additional DOM properties applied
    // to prevent this editor from highlighting an internal button in addition
    // to the button of a Drupal plugin.
    var specialProperties = {
      img: { name: 'mce_drupal' }
    };
    $content = $('<div>' + content + '</div>'); // No .outerHTML() in jQuery :(
    jQuery.each(specialProperties, function(element, properties) {
      $content.find(element).each(function() {
        for (var property in properties) {
          if (property == 'class') {
            $(this).addClass(properties[property]);
          }
          else {
            $(this).attr(property, properties[property]);
          }
        }
      });
    });
    return $content.html();
  },

  insert: function(content) {
    content = this.prepareContent(content);
    var editor = tinyMCE.getInstanceById(this.field);
    editor.execCommand('mceInsertContent', false, content);
    editor.repaint();
  }
};

