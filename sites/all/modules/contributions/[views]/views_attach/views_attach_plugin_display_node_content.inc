<?php
// $Id: views_attach_plugin_display_node_content.inc,v 1.1.2.7 2009/02/22 23:05:36 crell Exp $

/**
 * The plugin that handles node-attached views.
 */
class views_attach_plugin_display_node_content extends views_plugin_display {
  function option_definition () {
    $options = parent::option_definition();

    $options['types'] = array('default' => array());
    $options['modes'] = array('default' => array('full'));
    $options['default_argument'] = array('default' => 'nid');
    $options['show_title'] = 0;

    return $options;
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $categories['node_content'] = array(
      'title' => t('Node content settings'),
    );

    $types = $this->get_option('types');
    if (empty($types)) {
      $types = array('story');
    }

    $options['types'] = array(
      'category' => 'node_content',
      'title' => t('Node types'),
      'value' => implode(', ', $types),
    );

    $used_modes = $this->get_option('modes');
    // Get the human readable names.
    $modes = views_attach_build_modes();
    foreach ($used_modes as $mode) {
      $display_modes[] = $modes[$mode];
    }
    $options['modes'] = array(
      'category' => 'node_content',
      'title' => t('Build modes'),
      'value' => implode(', ', $display_modes),
    );

    $weight = $this->get_option('weight');
    if (empty($weight)) {
      $weight = 10;
    }

    $default_argument = $this->get_option('default_argument');
    $options['default_argument'] = array(
      'category' => 'node_content',
      'title' => t('Default argument'),
      'value' => empty($default_argument) ? t('None') : $default_argument,
    );

    $options['show_title'] = array(
      'category' => 'node_content',
      'title' => t('Show title'),
      'value' => $this->get_option('show_title') ? t('Yes') : t('No'),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'types':
        $form['#title'] .= t('Node types');
        $form['types'] = array(
          '#type' => 'checkboxes',
          '#multiple' => TRUE,
          '#required' => TRUE,
          '#title' => t("Embed this display in the following node types"),
          '#options' => node_get_types('names'),
          '#default_value' => $this->get_option('types'),
        );
        break;

      case 'modes':
        $form['#title'] .= t('Build modes');
        $form['modes'] = array(
          '#type' => 'checkboxes',
          '#title' => t("Embed this display in the following build modes"),
          '#options' => views_attach_build_modes(),
          '#default_value' => $this->get_option('modes'),
         );
         break;

      case 'default_argument':
        $form['#title'] .= t('Default argument');
        $form['default_argument'] = array(
          '#type' => 'checkbox',
          '#title' => t('Provide the current node id as a default argument.'),
          '#default_value' => $this->get_option('default_argument') === 'nid',
          '#return_value' => 'nid',
        );
        break;

      case 'show_title':
        $form['#title'] .= t('Show title');
        $form['show_title'] = array(
          '#type' => 'checkbox',
          '#title' => t('Show the title of the view above the attached view.'),
          '#default_value' => $this->get_option('show_title'),
        );
        break;
    }

  }

  function options_submit($form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'types':
        $this->set_option('types', array_filter($form_state['values']['types']));
        break;
      case 'modes':
        $this->set_option('modes', array_values(array_filter($form_state['values']['modes'])));
        break;
      case 'default_argument':
        $this->set_option('default_argument', $form_state['values']['default_argument']);
        break;
      case 'show_title':
        $this->set_option('show_title', $form_state['values']['show_title']);
        break;
    }
  }

  /**
   * The display block handler returns the structure necessary for a block.
   */
  function execute() {
    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    $data = $this->view->render();
    if (!empty($this->view->result) || $this->get_option('empty') || !empty($this->view->style_plugin->definition['even empty'])) {
      return $data;
    }
  }

  /**
   * Block views do not use exposed widgets.
   */
  function uses_exposed() { return FALSE; }
}
