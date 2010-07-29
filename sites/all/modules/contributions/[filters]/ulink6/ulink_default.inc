<?php
/**
 * Implementation of hook_ulink_node()
 */
function ulink_ulink_node($link, $node=NULL){
  return _ulink_ulink_xxx('node', $link, $node);
}
/**
 * Implementation of hook_ulink_node_settings()
 */
function ulink_ulink_node_settings() {
  return _ulink_ulink_settings_builder('node');
}
/**
 * Implementation of hook_ulink_node_info()
 */
function ulink_ulink_node_info() {
  return t('Three modes of rendering are possible.<br />
            1. Hardcoded rendering for novice users. <br />
            2. In the macro mode TOKENS can be used. <br />
            3. PHPcode for advanced users. <br />
            Use configurations to individually configure them to match the requirement.');
}
/**
 * Implementation of hook_ulink_intern().
 */
function ulink_ulink_intern($link, $object=NULL){
  return _ulink_ulink_xxx('intern', $link, $object);
}
/**
 * Implementation of hook_ulink_intern_settings().
 */
function ulink_ulink_intern_settings() {
  return _ulink_ulink_settings_builder('intern');
}
/**
 * Implementation of hook_ulink_intern_info().
 */
function ulink_ulink_intern_info() {
    return t('Three modes of rendering are possible.<br />
            1. Hardcoded rendering for novice users. <br />
            2. In the macro mode TOKENS can be used. <br />
            3. PHPcode for advanced users. <br />
            Use configurations to individually configure them to match the requirement.');
}
/**
 * Implementation of hook_ulink_extern()
 */
function ulink_ulink_extern($link, $object=NULL){
  return _ulink_ulink_xxx('extern', $link, $object);
}
/**
 * Implementation of hook_ulink_settings()
 */
function ulink_ulink_extern_settings() {
  return _ulink_ulink_settings_builder('extern');
}
/**
 * Implementation of hook_ulink_info()
 */
function ulink_ulink_extern_info() {
  return t(' Three modes of rendering are possible. Hardcoded rendering for novice users and PHPcode for advanced users. In the macro mode TOKENS can be used. Use configurations to individually configure them to match the requirement.');
}
/**
 * Implementation of hook_ulink_user()
 */
function ulink_ulink_user($link, $user = NULL){
  return _ulink_ulink_xxx('user', $link, $user);
}
/**
 * Implementation of hook_ulink_user_settings()
 */
function ulink_ulink_user_settings() {
   return _ulink_ulink_settings_builder('user');
}
/**
 * Implementation of hook_ulink_user_info()
 */
function ulink_ulink_user_info() {
  return t(' Three modes of rendering are possible. Hardcoded rendering for novice users and PHPcode for advanced users. In the macro mode TOKENS can be used. Use configurations to individually configure them to match the requirement.');
}
/**
 * Helper function builds the settings page for ulink_settings for all types on behalf of ulink default implementation
 */
function _ulink_ulink_settings_builder($type) {  
	if ($type != 'extern') {
    $options[1] = t('Default (Simple)').l(' : configure','admin/settings/ulink/default/'.$type.'/1', array('query' => drupal_get_destination()));
  }
  else {
    $options[1] = t('Default (Simple)');
  }  
	
  if (module_exists('token')) {
    $options[2] = t('Macro substitution (Tokens) ').l(' : configure','admin/settings/ulink/default/'.$type.'/2', array('query' => drupal_get_destination()));
  }
  else {
    drupal_set_message('If token module is installed, ulink provides support for token substitution. You can download it <a href="http://drupal.org/project/token" >here </a>','error','error');
  }
  if (module_exists('php')) {
      $options[3] = t('PHP code '.l(' : configure','admin/settings/ulink/default/'.$type.'/3',array('query' => drupal_get_destination())));
  }
  else {
    drupal_set_message('If PHPfilter module is installed, ulink provides support for PHP scripting.', 'error', 'error');
  }
	
  $form['ulink_'.$type.'_settings_option'] = array(
    '#type' => 'radios',
    '#title' => t('Implementation desired'),
    '#options' => $options,
    '#default_value' => variable_get('ulink_'.$type.'_settings_option', 1),
  ); 
  return $form;
}
/**
 * Helper function provides the implemenation for ulink_xxx for all types on behalf of ulink default implementation
 */
function _ulink_ulink_xxx($type, $link, $object = NULL){
  $path = $link['path'];
  $text = $link['text'];
  $attributes = $link['attributes'];
  switch ($type) {
    case 'node':
	  $str = '$object->type';
	  $str1 = '[title]';
	  break;
	case 'user':
	  $str1 = '[user]';
      $str = '$object->name';
	  break;
	case 'intern':
	  $str1 = '[files-filename]';
	  $str = '$object->filename';
	  break;
  case 'extern':
  default:
	  $str1 = 'external to [site-name]';
	  $str = 'external file';
  }
  
  switch (variable_get('ulink_'.$type.'_settings_option', 1)) {
    case 1:	
    default:
			$text = $text ? $text : _ulink_get_defaulttag($type, variable_get('ulink_'.$type.'_1_option', 2), $object, $path);
	    return l($text, $path, array('attributes' => $attributes));			
    case 2:
		  if (module_exists('token')) {
				$tokentext = token_replace(variable_get('ulink_'.$type.'_2_text', $str1), 'ulink', $link);
				$tokentext = token_replace($tokentext, $type, $object);
			  $link['text'] = $link['text'] ? $link['text'] : $tokentext;

				if (!$object ) {
          return token_replace_multiple(variable_get('ulink_'.$type.'_2_brokenlink', '<a href="[ulink-path]" [ulink-attributes-string]>[ulink-text] ('.$type.' not available)</a>'), array('global' => NULL, 'ulink' => $link));
				}
				else {
          return token_replace_multiple(variable_get('ulink_'.$type.'_2_tag', '<a href="[ulink-path]" [ulink-attributes-string]>[ulink-text]</a>'), array('global' => NULL, 'ulink' => $link, $type => $object));
				}
      }
      else {
  	    return $text;
      }	
		case 3:
      if (module_exists('php')) {
        return eval(variable_get('ulink_'.$type.'_3_code', 'return l($text." ( ".'.$str.'." )", $path, array("attributes" => $link["attributes"]));'));
      }
      else {
        return $text;
      }
  }
}
/**
* helper function gives the default tag when the user text is empty for the type given in $type
*/
function _ulink_get_defaulttag($type, $option, $object, $path) {
  if (!$object || !$option)
    return NULL;
  else if ($option == 1)
    return $path;
	else if ($option == 2) {
	  switch ($type) {
	    case 'node':
	      return $object->title;		
			case 'intern':
        return $object->filename;		
			case 'user':
        return $object->name;		
	  }
	}
}
/**
* helper function populates the settings form on behalf of all types under default implementation
*/
function _ulink_default_settings(&$form_state, $type, $id) {
  switch ($id) {
    case '1':
      switch ($type) {
      case 'extern':
      default:
        return NULL;
      case 'node':
        $options = array ( 0 => t('None'), 1 => t('Path'), 2 => t('Title'));
        break;
      case 'user':
        $options = array ( 0 => t('None'), 1 => t('User path'), 2  => t('User name'));
        break;
      case 'intern':
        $options = array ( 0 => t('None'), 1 => t('File path'), 2 => t('File name'));
        break;
      }

      $form['settings'] = array(
        '#type' => 'fieldset',
        '#title' => t('settings'),
        '#collapsible' => FALSE, 
      );
      $form['settings']['ulink_'.$type.'_1_option'] =  array(
        '#type' => 'radios',
        '#title' => t('Use this when text is empty'),
        '#options' => $options,
        '#default_value' => variable_get('ulink_'.$type.'_1_option', 2),
      );
      break;
    case '2':
      switch ($type) {
        case 'node':
      	  $str1 = '[title]';
      	  break;
      	case 'user':
      	  $str1 = '[user]';
      	  break;
      	case 'intern':
      	  $str1 = '[files-filename]';
      	  break;
        case 'extern':
        default:
      	  $str1 = 'external to [site-name]';
      }
  	
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Macro mode settings'),
      '#collapsible' => FALSE, 
    );
    if (!module_exists('token')) {
      drupal_set_message('If token module is installed, ulink provides support for token substitution. You can download it <a href="http://ftp.osuosl.org/pub/drupal/files/projects/token-5.x-1.7.tar.gz" >here </a>','error','error');
  		return system_settings_form($form);
    }
  	
    $form['settings']['ulink_'.$type.'_2_text'] =  array(
      '#type' => 'textfield',
      '#title' => t('Use this when text is empty'),
      '#default_value' => variable_get('ulink_'.$type.'_2_text', $str1),	
    );
    $form['settings']['ulink_'.$type.'_2_tag'] = array(
      '#type' => 'textarea',
      '#title' => t('Token tag to be rendered'),
      '#rows' => 1,
      '#default_value' => variable_get('ulink_'.$type.'_2_tag', '<a href="[ulink-path]" [ulink-attributes-string]>[ulink-text]</a>'),	
      '#description' => t('Refer to the token help for assistance'),
    );
  	
    if ($type != 'extern') {
      $form['settings']['ulink_'.$type.'_2_brokenlink'] = array(
        '#type' => 'textarea',
        '#title' => t('Token tag to be rendered if the object is not available'),
        '#rows' => 1,
        '#default_value' => variable_get('ulink_'.$type.'_2_brokenlink', '<a href="[ulink-path]" [ulink-attributes-string]>[ulink-text] ('.$type.' not available)</a>'),
        '#description' => t('Refer to the token help for assistance'),	
      );
    }
    $form['settings']['token_help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Token help'),
      '#collapsible' => TRUE, 
      '#collapsed' => TRUE, 
    );
    $form['settings']['token_help']['a']['div-tag'] = array('#type' => 'markup', '#value' => theme('token_help',$type ));
    $form['settings']['token_help']['b']['div-tag'] = array('#type' => 'markup', '#value' => theme('token_help','ulink'));
    break;
  case '3':
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('PHP mode settings'),
      '#collapsible' => FALSE, 
    );
    switch ($type) {
      case 'node':
        $str = '$object->type';
        break;
    	case 'user':
          $str = '$object->name';
    	  break;
    	case 'intern':
    	  $str = '$object->filename';
    	  break;
      case 'extern':
    	default:
    	   $str = 'external file';
    }
    $form['settings']['ulink_'.$type.'_3_code'] = array(
      '#type' => 'textarea',
      '#title' => t('PHP code to be rendered'),
    	'#rows' => 1,
    	'#default_value' => variable_get('ulink_'.$type.'_3_code', 'return l($text." ( ".'.$str.'." )", $path, array("attributes" => $link["attributes"]));'),	
    	'#description' => t('Refer to the variables help for assistance'),
    );
    $form['settings']['Variables help'] = array(
      '#type' => 'fieldset',
      '#title' => t('PHP help'),
      '#collapsible' => TRUE, 
      '#collapsed' => TRUE, 
    );
    $form['settings']['Variables help']['div-tag'] = array(
      '#type' => 'markup',
      '#value' => '<table class="description"><thead><tr><th>Variable</th><th>Description</th></tr></thead><tbody>
                     <tr class="odd"><td class="region" colspan="2">variables that can be directly used</td> </tr>
                       <tr class="even"><td>$path</td><td>URL token</td> </tr>
                       <tr class="odd"><td>$text</td><td>TEXT token</td> </tr>
                       <tr class="even"><td>$object</td><td>Target object (not available in extern mode)</td> </tr>
                       <tr class="odd"><td>$attributes</td><td>attributes array</td> </tr>
                     <tr class="odd"><td class="region" colspan="2">Variables available in the array $link</td> </tr>
                       <tr class="even"><td>path</td><td>the targets url</td> </tr>
                       <tr class="odd"><td>spath</td><td>the targets url as entered by the user</td> </tr>
                       <tr class="even"><td>text</td><td>text to be referenced</td> </tr>
                       <tr class="odd"><td>nid</td><td>Node ID (only in node mode)</td> </tr>
                       <tr class="odd"><td>uid</td><td>User ID (Only in user mode)</td> </tr>
                       <tr class="odd"><td>fid</td><td>File ID (Only in intern mode, -1 if reference not available in the database)</td> </tr>
                       <tr class="even"><td>mime</td><td>Extension of the link, null if no extension</td> </tr>
                       <tr class="odd"><td>file or node or user</td><td> object (not available in extern mode)</td> </tr>
                       <tr class="even"><td>attributes</td><td>attributes array</td> </tr>
                       <tr class="even"><td>attr</td><td>attributes string as enetered by the user</td> </tr>
                       <tr class="odd"><td>type</td><td>possible values: node, user, files<br>in extern mode: http:/ , ftp:/</td> </tr>
                   </tbody></table>'
      );
    break;
  }
  return  system_settings_form($form);
}
