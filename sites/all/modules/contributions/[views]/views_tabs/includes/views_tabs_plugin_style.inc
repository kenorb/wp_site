<?php
class views_tabs_plugin_style extends views_plugin_style {

  function options(&$options) {
    parent::options($options);
    if ($this->row_plugin->definition['uses fields']) {
      $options['tab_field'] = 'title';
    }
  }
 
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if ($this->uses_fields() && $this->definition['uses grouping']) {
      $options = $this->options;
      $opt = array('' => t('<None>'));
      foreach ($this->display->handler->get_handlers('field') as $field => $handler) {
        if ($label = $handler->label()) {
          $opt[$field] = $label;
        }
        else {
          $opt[$field] = $handler->ui_name();
        }
      }
      $form['tab_field'] = array(
        '#title' => t('Tab field'),
        '#type'  => 'select',
        '#options' => $opt,
        '#default_value' => $options['tab_field'],
      );
    }
  }
  
  function render() {
    $this->definition['theme'] = $this->row_plugin->definition['theme'];
    // Create empty tabset to build up
     
    $tabset = array(
      '#type' => 'tabset',
    );

    if ($this->row_plugin->definition['uses fields']) {
      // Get a list of the available fields.  
      $_fields = array_keys($this->view->field);
    
      // If title is the tab, and there is no title - assign the first field
      if (!$this->view->field['title'] 
            && $this->options['tab_field'] == 'title') {
        $this->options['tab_field'] = $_fields[0];
      }
      $tab_field = $this->view->field[$this->options['tab_field']]->field_alias;
    }
    foreach ($this->view->result as $row) {
      if ($this->row_plugin->definition['uses fields']) {
        $content = theme($this->theme_functions(), $this->view, $this->options, $row);
        $tab = $row->$tab_field;
      }
      else {
        $node = node_load($row->nid);
        $content = node_view($node, $this->row_plugin->options['teaser'], FALSE, $this->row_plugin->options['links']);
        $tab = $node->title;
      }
   
      if (!$tabset[$tab]) {
        $tabset[$tab] = array(
          '#type' => 'tabpage',
          '#content' => $content,
          '#title' => $tab,
          '#attributes' => array('class' => 'node-'. $row->nid),
        );
      }
    }
    return tabs_render($tabset); 
  }
}
