<?php
// $Id: timeline_views_plugin.inc,v 1.1.2.1 2008/10/12 16:43:51 jmiccolis Exp $
/**
 * Extending the view_plugin_style class to provide a timeline view style.
 */
class timeline_views_plugin extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['type'] = array('default' => TIMELINE_HORIZONTAL);
    $options['display']['width'] = array('default' => 200);
    $options['display']['height'] = array('default' => 100);
    $options['display']['focus'] = array('default' => 'first');
    $options['bands']['band1_unit'] = array('default' => 'day');
    $options['bands']['band2_unit'] = array('default' => 'month');
    $options['misc']['controls'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Add settings for the particular timeline.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Timeline type'),
      '#options' => array(TIMELINE_HORIZONTAL => t('Horizontal'), TIMELINE_VERTICAL => t('Vertical')),
      '#default_value' => $this->options['type'],
    );
    $form['display'] = array('#type' => 'fieldset', '#title' => t('Display settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $form['display']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $this->options['display']['width'],
      '#size' => 25,
      '#maxlength' => 6,
      '#description' => t('The width of the timeline (in units of em, px or %), e.g. 600px or 90%. Leave blank to use default value.'),
    );
    $form['display']['height'] = array(
      '#type' => 'textfield',
      '#title' => t('Height'),
      '#default_value' => $this->options['display']['height'],
      '#size' => 25,
      '#maxlength' => 6,
      '#description' => t('The height of the timeline (in units of em, px or %), e.g. 400px. Leave blank to use default value.'),
    );
    $form['display']['focus'] = array(
      '#type' => 'select',
      '#title' => t('Initial date focus'),
      '#default_value' => $this->options['display']['focus'],
      '#options' => array('' => '', 'first' => t('First event in timeline'), 'last' => t('Last event in timeline'), 'today' => t('The current date')),
      '#description' => t('Determines which event or date the timeline view should initially be centered on after it has loaded. Leave blank to use default value.'),
    );

    // Band/interval settings
    $form['bands'] = array('#type' => 'fieldset', '#title' => t('Band/interval settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $intervals = array('second' => t('Second'), 'minute' => t('Minute'), 'hour' => t('Hour'), 'day' => t('Day'), 'week' => t('Week'), 'month' => t('Month'), 'year' => t('Year'), 'decade' => t('Decade'), 'century' => t('Century'), 'millennium' => t('Millennium'));
    $form['bands']['band1_unit'] = array(
      '#type' => 'select',
      '#title' => t('Main band interval unit'),
      '#default_value' => $this->options['bands']['band1_unit'],
      '#options' => $intervals,
      '#description' => t(''),
    );
    $form['bands']['band2_unit'] = array(
      '#type' => 'select',
      '#title' => t('Summary band interval unit'),
      '#default_value' => $this->options['bands']['band2_unit'],
      '#options' => array_merge(array('' => t('Not displayed')), $intervals),
      '#description' => t(''),
    );

    // Miscellanous settings
    $form['misc'] = array('#type' => 'fieldset', '#title' => t('Miscellaneous settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
    $form['misc']['controls'] = array(
      '#type' => 'radios',
      '#title' => t('Enable controls'),
      '#default_value' => $this->options['misc']['controls'],
      '#options' => array(FALSE => t('Disabled'), TRUE => t('Enabled')),
      '#description' => t('Whether to display filtering and highlighting controls under the timeline. Not recommended for block mode.'),
    );
  }

  /**
   * Render the timeline style.
   */
  function render() {
    $sets = $this->render_grouping($this->view->result, $this->options['grouping']);
    return theme($this->theme_functions(), $this->view, $this->options, $sets, $title);
  }
}