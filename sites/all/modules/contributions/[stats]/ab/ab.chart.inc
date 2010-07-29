<?php
// $Id: ab.chart.inc,v 1.1 2008/09/12 17:38:31 tjholowaychuk Exp $

/**
 * @file
 * ApacheBench open flash chart implementation.
 */

/**
 * ABChart
 *
 * This implementation of open_flash_chart_api simply provides
 * unification between all ApacheBench charts, offering common colors,
 * rendering, markup, settings etc.
 */
class ABChart extends open_flash_chart_api {
   
  /**
   * Chart ID.
   * 
   * @var string
   */
  public $id;
  
  /**
   * Primary color.
   * 
   * @var string
   */
  public $color_primary = 'adda01';
  
  /**
   * Secondary color.
   * 
   * @var string
   */
  public $color_secondary = '484848';   
  
  /**
   * Tertiary color.
   * 
   * @var string
   */
  public $color_tertiary = 'd2e8f5';
  
  /**
   * Default opacity.
   * 
   * @var int
   */
  public $opacity = 75;
  
  /**
   * Size class available to style sheets.
   * 
   * @var int
   */
  public $size_class = 'large';
   
  /**
   * Constructor.
   * 
   * @param string $id
   *   Unique identifier for this chart.
   * 
   * @param string $title
   *   (optional) Title for this chart.
   * 
   * @param string $style
   *   (optional) Style of chart.
   * 
   * @param string $size
   *   (optional) Size of chart which can be styled
   *   with a style sheet.
   */
  public function __construct($id, $title = '', $style = 'normal', $size = 'large') {
    $this->id = $id;
    $this->size_class = $size;
    parent::open_flash_chart_api();
    
    // Set title
    if (!empty($title)){
      $this->set_title($title);
    }
    
    // Invoke style method
    $style_method = "_style_{$style}";
    if (method_exists($this, $style_method)){
      call_user_method($style_method, $this);  
    }
  }
  
  /* -----------------------------------------------------------------

    Public methods 

  ------------------------------------------------------------------ */
  
  /**
   * Get chart markup.
   * 
   * @return string
   */
  public function render() {
    return '<div id="ab-chart-' . $this->id . '" class="ab-chart size-' . $this->size_class . '">' . parent::render() . '</div>';
  }
  
  /* -----------------------------------------------------------------

    Styles 

  ------------------------------------------------------------------ */

  /**
   * Normal style.
   * 
   * @since 0.1
   */
  private function _style_normal() {
    $this->title_style = '{ font-size: 14; color: #' . $this->color_secondary . '; }';
    $this->set_width(750);
    $this->set_bg_colour('ffffff');
    $this->set_x_axis_colour('ffffff', 'ffffff');
    $this->set_y_axis_colour($this->color_tertiary, $this->color_tertiary); 
  }
}