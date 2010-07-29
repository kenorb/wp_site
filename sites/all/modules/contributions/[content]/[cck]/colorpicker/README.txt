; $Id$

This module does nothing by itself but add a new form element type called a 'colorpicker'.  You can see an example useage in the colorpicker_example.module included in the package with this module, or use something similar to below.  

It also defines another element type called a 'colorpicker_textfield' which is associated to the 'colorpicker' by adding a property of '#colorpicker' and the '#id' of the colorpicker in question.  For an example of this, see the colorpicker_example.module.  Note that a default value (in hex) is necessary for this element type to work properly.

Also, if only one colorpicker type is defined on a page, all colorpicker_textfield's are associated with that colorpicker, regardless of the '#colorpicker' property.

$form['my_colorpicker'] = array(
  '#type' => 'colorpicker',
  '#title' => t('Color picker'),
  '#description' => t('Click in this textfield to start picking your color'),
);

$form['my_colorpicker_textfield'] = array(
  '#type' => 'colorpicker_textfield',
  '#title' => t('Color picker textfield'),
  '#description' => t('This is a textfield associated with the first Farbtastic color picker'),
  '#default_value' => variable_get('colorpicker_example_textfield', '#000000'),
  '#colorpicker' => 'my_colorpicker',
);