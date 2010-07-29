$content[type]  = array (
  'name' => 'User Profile',
  'type' => 'uprofile',
  'description' => 'Holds the data for a user\'s profile.',
  'title_label' => 'Profile Title',
  'body_label' => '',
  'min_word_count' => '0',
  'help' => '',
  'node_options' =>
  array (
    'status' => true,
    'promote' => false,
    'sticky' => false,
    'revision' => false,
  ),
  'old_type' => 'uprofile',
  'orig_type' => '',
  'module' => 'node',
  'custom' => '1',
  'modified' => '1',
  'locked' => '0',
  'content_profile_use' => 1,
  'comment' => '2',
  'comment_default_mode' => '4',
  'comment_default_order' => '1',
  'comment_default_per_page' => '50',
  'comment_controls' => '3',
  'comment_anonymous' => '0',
  'comment_subject_field' => '1',
  'comment_preview' => '1',
  'comment_form_location' => '0',
  'flatcomments' => 0,
);
$content[groups]  = array (
  0 =>
  array (
    'label' => 'Basic Info',
    'group_type' => 'standard',
    'settings' =>
    array (
      'form' =>
      array (
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' =>
      array (
        'description' => '',
        'teaser' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        'full' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        4 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        2 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '1',
    'group_name' => 'group_about',
  ),
  1 =>
  array (
    'label' => 'Stats',
    'group_type' => 'standard',
    'settings' =>
    array (
      'form' =>
      array (
        'style' => 'fieldset_collapsible',
        'description' => '',
      ),
      'display' =>
      array (
        'description' => '',
        'teaser' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        'full' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        4 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        2 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '3',
    'group_name' => 'group_stats',
  ),
  2 =>
  array (
    'label' => 'Contact',
    'group_type' => 'standard',
    'settings' =>
    array (
      'form' =>
      array (
        'style' => 'fieldset_collapsible',
        'description' => '',
      ),
      'display' =>
      array (
        'description' => '',
        'teaser' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        'full' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        4 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        2 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '5',
    'group_name' => 'group_contact',
  ),
  3 =>
  array (
    'label' => 'Links',
    'group_type' => 'standard',
    'settings' =>
    array (
      'form' =>
      array (
        'style' => 'fieldset_collapsible',
        'description' => '',
      ),
      'display' =>
      array (
        'description' => '',
        'teaser' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        'full' =>
        array (
          'format' => 'f',
          'exclude' => 'f',
        ),
        4 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        2 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        3 =>
        array (
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'label' => 'above',
      ),
    ),
    'weight' => '7',
    'group_name' => 'group_links',
  ),
);
$content[fields]  = array (
  0 =>
  array (
    'label' => 'Name',
    'field_name' => 'field_name',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => '-9',
    'rows' => '1',
    'size' => '60',
    'description' => 'Your real name. ',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_name][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' => NULL,
    'group' => 'group_about',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  1 =>
  array (
    'label' => 'Location',
    'field_name' => 'field_location',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => '-7',
    'rows' => '1',
    'size' => '60',
    'description' => 'Where you live.',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_location][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_location' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_location][0][value',
        ),
      ),
    ),
    'group' => 'group_about',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  2 =>
  array (
    'label' => 'About Me',
    'field_name' => 'field_about_me',
    'type' => 'text',
    'widget_type' => 'text_textarea',
    'change' => 'Change basic information',
    'weight' => '-5',
    'rows' => '10',
    'size' => 60,
    'description' => 'General information about who you are, where you came from, and where you are going.',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        'format' => '1',
        '_error_element' => 'default_value_widget][field_about_me][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_about_me' =>
      array (
        0 =>
        array (
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_about_me][0][value',
        ),
      ),
    ),
    'group' => 'group_about',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
      'format' =>
      array (
        'type' => 'int',
        'unsigned' => true,
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  3 =>
  array (
    'label' => 'Interests',
    'field_name' => 'field_interests',
    'type' => 'text',
    'widget_type' => 'text_textarea',
    'change' => 'Change basic information',
    'weight' => '-3',
    'rows' => '5',
    'size' => 60,
    'description' => 'Describe your interests and hobbies. This section is for whole sentences, not tags.',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        'format' => '1',
        '_error_element' => 'default_value_widget][field_interests][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_interests' =>
      array (
        0 =>
        array (
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_interests][0][value',
        ),
      ),
    ),
    'group' => 'group_about',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
      'format' =>
      array (
        'type' => 'int',
        'unsigned' => true,
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  4 =>
  array (
    'label' => 'Gender',
    'field_name' => 'field_gender',
    'type' => 'text',
    'widget_type' => 'optionwidgets_select',
    'change' => 'Change basic information',
    'weight' => '-9',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_gender' =>
      array (
        'value' => '',
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'Male
Female
Undecided
',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'optionwidgets',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  5 =>
  array (
    'label' => 'Occupation',
    'field_name' => 'field_occupation',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => '-8',
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_occupation][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_occupation' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_occupation][0][value',
        ),
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '100',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'varchar',
        'length' => '100',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  6 =>
  array (
    'label' => 'Children',
    'field_name' => 'field_children',
    'type' => 'text',
    'widget_type' => 'optionwidgets_select',
    'change' => 'Change basic information',
    'weight' => '-7',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_children' =>
      array (
        'value' => '',
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'No children: not planning on having any
No children: would like some now or in the future
Have young child(ren)
Have grown children
My pets are my kids
',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'optionwidgets',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  7 =>
  array (
    'label' => 'Ethnicity',
    'field_name' => 'field_ethnicity',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => '-5',
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_ethnicity][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_ethnicity' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_ethnicity][0][value',
        ),
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  8 =>
  array (
    'label' => 'Status',
    'field_name' => 'field_status',
    'type' => 'text',
    'widget_type' => 'optionwidgets_select',
    'change' => 'Change basic information',
    'weight' => '-4',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_status' =>
      array (
        'value' => '',
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'Single: not looking
Single: looking
In a relationship
Married
Divorced
Widowed',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'optionwidgets',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  9 =>
  array (
    'label' => 'Orientation',
    'field_name' => 'field_orientation',
    'type' => 'text',
    'widget_type' => 'optionwidgets_select',
    'change' => 'Change basic information',
    'weight' => '-3',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_orientation' =>
      array (
        'value' => '',
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'Straight
Gay
Bi
Undecided',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'optionwidgets',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  10 =>
  array (
    'label' => 'Religion',
    'field_name' => 'field_religion',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => '-2',
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_religion][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_religion' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_religion][0][value',
        ),
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  11 =>
  array (
    'label' => 'Political Leaning',
    'field_name' => 'field_politcal_leaning',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => '-1',
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_politcal_leaning][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_politcal_leaning' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_politcal_leaning][0][value',
        ),
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  12 =>
  array (
    'label' => 'Zodiac Sign',
    'field_name' => 'field_zodiac_sign',
    'type' => 'text',
    'widget_type' => 'optionwidgets_select',
    'change' => 'Change basic information',
    'weight' => 0,
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_zodiac_sign' =>
      array (
        'value' => '',
      ),
    ),
    'group' => 'group_stats',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => 'Aries (Mar 21-Apr 19)
Taurus (Apr 20-May 20)
Gemini (May 21-Jun 20)
Cancer (Jun 21-Jul 22)
Leo (Jul 23-Aug 22)
Virgo (Aug 23-Sep 22)
Libra (Sep 23-Oct 22)
Scorpio (Oct 23-Nov 21)
Sagittarius (Nov 22-Dec 21)
Capricorn (Dec 22-Jan 19)
Aquarius (Jan 20-Feb 18)
Pisces (Feb 19-Mar 20)',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'optionwidgets',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  13 =>
  array (
    'label' => 'MSN',
    'field_name' => 'field_msn',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => 0,
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_msn][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_msn' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_msn][0][value',
        ),
      ),
    ),
    'group' => 'group_contact',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  14 =>
  array (
    'label' => 'Skype',
    'field_name' => 'field_skype',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => 0,
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_skype][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_skype' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_skype][0][value',
        ),
      ),
    ),
    'group' => 'group_contact',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  15 =>
  array (
    'label' => 'Yahoo',
    'field_name' => 'field_yahoo',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => 0,
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_yahoo][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_yahoo' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_yahoo][0][value',
        ),
      ),
    ),
    'group' => 'group_contact',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  16 =>
  array (
    'label' => 'ICQ',
    'field_name' => 'field_icq',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => 0,
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_icq][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_icq' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_icq][0][value',
        ),
      ),
    ),
    'group' => 'group_contact',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  17 =>
  array (
    'label' => 'Google',
    'field_name' => 'field_google',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => 0,
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_google][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_google' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_google][0][value',
        ),
      ),
    ),
    'group' => 'group_contact',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  18 =>
  array (
    'label' => 'AIM',
    'field_name' => 'field_aim',
    'type' => 'text',
    'widget_type' => 'text_textfield',
    'change' => 'Change basic information',
    'weight' => 0,
    'rows' => '1',
    'size' => '60',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'value' => '',
        '_error_element' => 'default_value_widget][field_aim][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_aim' =>
      array (
        0 =>
        array (
          'value' => '',
          '_error_element' => 'default_value_widget][field_aim][0][value',
        ),
      ),
    ),
    'group' => 'group_contact',
    'required' => 0,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'text',
    'columns' =>
    array (
      'value' =>
      array (
        'type' => 'text',
        'size' => 'big',
        'not null' => false,
        'sortable' => true,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  19 =>
  array (
    'label' => 'Homepage',
    'field_name' => 'field_homepage',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-9',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_homepage' =>
      array (
        0 =>
        array (
          'url' => '',
          'title' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-homepage',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  20 =>
  array (
    'label' => 'Blog',
    'field_name' => 'field_blog',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-8',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_blog' =>
      array (
        0 =>
        array (
          'url' => '',
          'title' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-blog',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  21 =>
  array (
    'label' => 'Twitter',
    'field_name' => 'field_twitter',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-7',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_twitter' =>
      array (
        0 =>
        array (
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'value',
    'title_value' => 'Twitter',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-twitter',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      2 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      3 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
  22 =>
  array (
    'label' => 'LinkedIn',
    'field_name' => 'field_linkedin',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-6',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_linkedin' =>
      array (
        0 =>
        array (
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'value',
    'title_value' => 'LinkedIn',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-linkedin',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      2 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
      3 =>
      array (
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
  23 =>
  array (
    'label' => 'Facebook',
    'field_name' => 'field_facebook',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-5',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_facebook' =>
      array (
        0 =>
        array (
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'value',
    'title_value' => 'Facebook',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-facebook',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  24 =>
  array (
    'label' => 'Myspace',
    'field_name' => 'field_myspace',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-4',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_myspace' =>
      array (
        0 =>
        array (
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'value',
    'title_value' => 'MySpace',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-myspace',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  25 =>
  array (
    'label' => 'Favorite #1',
    'field_name' => 'field_favorite1',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-3',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'title' => '',
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_favorite1' =>
      array (
        0 =>
        array (
          'title' => '',
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-favorite-1',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  26 =>
  array (
    'label' => 'Favorite #2',
    'field_name' => 'field_favorite2',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-2',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'title' => '',
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_favorite2' =>
      array (
        0 =>
        array (
          'title' => '',
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-favorite-2',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
  27 =>
  array (
    'label' => 'Favorite #3',
    'field_name' => 'field_favorite3',
    'type' => 'link',
    'widget_type' => 'link',
    'change' => 'Change basic information',
    'weight' => '-1',
    'description' => '',
    'default_value' =>
    array (
      0 =>
      array (
        'title' => '',
        'url' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array (
      'field_favorite3' =>
      array (
        0 =>
        array (
          'title' => '',
          'url' => '',
        ),
      ),
    ),
    'group' => 'group_links',
    'required' => 0,
    'multiple' => '0',
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'display' =>
    array (
      'url_cutoff' => '80',
    ),
    'attributes' =>
    array (
      'target' => '_blank',
      'rel' => 'nofollow',
      'class' => 'links-favorite-3',
    ),
    'op' => 'Save field settings',
    'module' => 'link',
    'widget_module' => 'link',
    'columns' =>
    array (
      'url' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'title' =>
      array (
        'type' => 'varchar',
        'length' => 255,
        'not null' => false,
        'sortable' => true,
      ),
      'attributes' =>
      array (
        'type' => 'text',
        'size' => 'medium',
        'not null' => false,
      ),
    ),
    'display_settings' =>
    array (
      'label' =>
      array (
        'format' => 'inline',
      ),
      'teaser' =>
      array (
        'format' => 'default',
      ),
      'full' =>
      array (
        'format' => 'default',
      ),
    ),
  ),
);
$content[extra]  = array (
  'title' => '-5',
  'menu' => '-2',
);
