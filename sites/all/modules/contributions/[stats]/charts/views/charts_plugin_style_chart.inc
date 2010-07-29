<?php
// $Id: charts_plugin_style_chart.inc,v 1.1 2008/11/13 01:57:11 brmassa Exp $

/**
 * @file
 * Contains the chart style plugin.
 */

/**
 * Style plugin to render view as a chart.
 *
 * @ingroup views_style_plugins
 */
class charts_plugin_style_chart extends views_plugin_style {
  // Set default options.
  function options(&$options) {
    $options['format'] = 'pie2D';
    $options['height'] = 200;
    $options['width'] = 400;
    $options['color'] = 'ffffff';
    //$options['columns'] = array();
    //$options['default'] = '';
    //$options['info'] = array();
    $options['conversion'] = 'rows';
  }

  // Generate a form for setting options.
  function options_form(&$form, &$form_state) {
    $form['format'] = array(
      '#type'    => 'select',
      '#title'   => t('Chart format'),
      '#options' => array(
        'line2D'  => t('Line 2D'),
        'hbar2D'  => t('Horizontal Bar 2D'),
        'vbar2D'  => t('Vertical Bar 2D'),
        'pie2D'   => t('Pie 2D'),
        'pie3D'   => t('Pie 3D'),
        'venn'    => t('Venn'),
        'scatter' => t('Scatter Plot')
      ),
      '#default_value' => $this->options['format'],
    );
    $form['height'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Chart height'),
      '#default_value' => $this->options['height'],
    );
    $form['width'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Chart width'),
      '#default_value' => $this->options['width'],
    );
    $form['color'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Background color'),
      '#default_value' => $this->options['color'],
      '#description'   => t('In hexadecimal format (RRGGBB). Do not use the # symbol.'),
    );
    $form['conversion'] = array(
      '#type'    => 'radios',
      '#title'   => t('Conversion type'),
      '#options' => array(
        'rows'   => t('Display numbers from every row'),
        'sum'    => t('Display sum of different values from one column'),
      ),
      '#default_value' => $this->options['conversion'],
      '#description'   => t('In the first option every row will be a new x value in the chart. In the second option every different value in one column will be a new x value in the chart.'),
    );
    $form['show_legend'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Show legend'),
      '#default_value' => 1,
      '#description'   => t('Display legend next to the chart.'),
    );
  }

  // Define and display a test chart.
  function render() {
    // Scan all Views data and insert them into a series.

    if ($this->options['conversion'] == 'rows') {
      // Get columns.
      foreach ($this->view->field as $key => $field) {
        if ($this->view->field[$key]->options['label'] == 'chart label') {
          $chart_label_column = $key;
          continue;
        }
        if (!$field->options['exclude']) {
          $data[$key]['#legend'] = ($this->options['show_legend']) ? $field->options['label'] : NULL;
        }
      }

      // Get values from rows.
      foreach ($this->view->result as $index => $row) {
        if (!isset($chart_label_column)) {
          $label = $index;
        }
        foreach ($this->view->field as $key => $field) {
          if ($chart_label_column == $key) {
            $label = theme_views_view_field($this->view, $this->view->field[$key], $row);
            continue;
          }
          if ($this->view->field[$key]->options['exclude']) {
            continue;
          }
          $field_alias = $this->view->field[$key]->field_alias;
          if (isset($this->view->result[$index]->$field_alias)) {
            // Try to get the value from the result.
            $field_value = $this->view->result[$index]->$field_alias;
          }
          else {
            // Try to get the value with theme function.
            $field_value = theme_views_view_field($this->view, $this->view->field[$key], $row);
          }
          if (is_numeric($field_value)) {
            $data[$key][$index]['#value'] = $field_value;
            $data[$key][$index]['#label'] = $label;
          }
        }
      }
    }


    if ($this->options['conversion'] == 'sum') {
      // Get fields.
      foreach ($this->view->field as $key => $field) {
        if (!$this->view->field[$key]->options['exclude']) {
          $data[$key]['#legend'] = ($this->options['show_legend']) ? $this->view->field[$key]->options['label'] : NULL;
        }
      }

      // Get values from rows.
      foreach ($this->view->result as $row) {
        foreach ($this->view->field as $key => $field) {
          if ($this->view->field[$key]->options['exclude']) {
            continue;
          }
          $field_value = theme_views_view_field($this->view, $this->view->field[$key], $row);
          if (!isset($data[$key][$field_value]['#value'])) {
            $data[$key][$field_value]['#label'] = $field_value;
            $data[$key][$field_value]['#value'] = 1;
          }
          else {
            $data[$key][$field_value]['#value']++;
          }
        }
      }

      // Convert index, because Charts module only accepts numeric index on series.
      // Don't really understund why is this restriction.
      foreach ($data as $key => $series) {
        $index = 0;
        foreach ($series as $key2 => $value) {
          if ($key2 == '#legend') {
            continue;
          }
          $data[$key][$index] = $value;
          unset($data[$key][$key2]);
          $index++;
        }
      }

    }


    // Get chart settings from options form.
    $chart = array(
      '#type'   => $this->options['format'],
      '#height' => $this->options['height'],
      '#width'  => $this->options['width'],
      '#color'  => $this->options['color'],
    );

    // Use the view title as the chart title.
    $chart['#title'] = $this->view->get_title();

    // Insert series into the chart array.
    foreach ($data as $series) {
      $chart[] = $series;
    }

    // Print the chart.
    return charts_chart($chart);
  }
}

