<?php
// $Id$
/**
 * @file
 * Theming file for the imcontrol module.
 * Include additional messenger HTML here.
 */

/**
 * Theme function for display of presence icons.
 *
 * @ingroup themeable
 * @param $messenger
 * The particular messenger.
 * @param $key
 * Unique id of the selected messenger.
 * @param $attributes
 * Attributes of embedded Windows Live messenger.
 * @param $show
 * Display status of Windows Live messenger.
 * @return
 * An HTML themed string.
 */
function theme_imcontrol_display($messenger, $key, $attributes = NULL, $show = FALSE) {
  if (empty($key)) {
    return '';
  }
  switch ($messenger) {
    case 'yahoo':
      $output = '<div id="imcontrol_yahoo"><a href="ymsgr:sendIM?'. $key .'"><img src="http://mail.opi.yahoo.com/online?u='. $key .'&amp;m=g&amp;t=2" alt="Yahoo IM" title="Yahoo IM" /></a></div>';
      break;
    case 'qq':
      $qq_format = 13;
      if ($show) {
        $qq_format = 11;
      }
      $output = '<div id="imcontrol_qq"><a href="tencent://message/?uin='. $key .'&amp;Site='. t(variable_get("site_name", "drupal")) .'&amp;Menu=yes"><img src="http://wpa.qq.com/pa?p=1:'. $key .':'. $qq_format .'" alt="'. t('QQ') .'" title="'. t('QQ') .'" /></a></div>';
      unset($qq_format); // Destroy variable
      break;
    case 'skype':
      $output = '<div id="imcontrol_skype"><script type="text/javascript" src="http://download.skype.com/share/skypebuttons/js/skypeCheck.js"></script>
	  	  	      <a href="skype:'. $key .'?call"><img src="http://mystatus.skype.com/smallclassic/'. $key .'" style="border: none;" alt="My Skype" /></a></div>';
      break;
    case 'msn':
      // Get user settings and the default colors
      $settings = explode(",", $attributes);
      if ($show) {
        /**
         * Create your own default theme in im_color.inc.php and change useTheme from 'false' to 'true'
         */
        $default = imcontrol_default_colors();

        $output = '<div id="imcontrol_msn"><iframe id="imcontrol_frame" src="'. url('http://settings.messenger.live.com/Conversation/IMMe.aspx', array('query' =>
          array('invitee' => $key,
            'mkt' => imcontrol_lang(),
            'useTheme' => !empty($settings[0]) ? $settings[0] : 'false',
            'foreColor' => !empty($settings[1]) ? $settings[1] : $default[0],
            'backColor' => !empty($settings[2]) ? $settings[2] : $default[1],
            'linkColor' => !empty($settings[3]) ? $settings[3] : $default[2],
            'borderColor' => !empty($settings[4]) ? $settings[4] : $default[3],
            'buttonForeColor' => !empty($settings[5]) ? $settings[5] : $default[4],
            'buttonBackColor' => !empty($settings[6]) ? $settings[6] : $default[5],
            'buttonBorderColor' => !empty($settings[7]) ? $settings[7] : $default[6],
            'buttonDisabledColor' => !empty($settings[8]) ? $settings[8] : $default[7],
            'headerForeColor' => !empty($settings[9]) ? $settings[9] : $default[8],
            'headerBackColor' => !empty($settings[10]) ? $settings[10] : $default[9],
            'menuForeColor' => $default[10],
            'menuBackColor' => $default[11],
            'chatForeColor' => $default[12],
            'chatBackColor' => $default[13],
            'chatDisabledColor' => $default[14],
            'chatErrorColor' => $default[15],
            'chatLabelColor' => !empty($settings[11]) ? $settings[11] : $default[16],
          ))) .'" frameborder="0"></iframe></div>';

        unset($default); // Destroy variable
      }
      else {
        $theme = 'D7E8EC'; // Default button background color
        if (!empty($settings[2])) {
          $theme = $settings[2];
        }
        $output = '<div id ="imcontrol_button">
          <script type="text/javascript" src="http://settings.messenger.live.com/controls/1.0/PresenceButton.js"></script>
          <div id="Microsoft_Live_Messenger_PresenceButton_'. substr($key, 0, -24) .'"
            msgr:width="100"
            msgr:backColor="#'. $theme .'"
            msgr:altBackColor="#FFFFFF"
            msgr:foreColor="#424542"
            msgr:conversationUrl="http://settings.messenger.live.com/Conversation/IMMe.aspx?invitee='. $key .'&amp;mkt='. imcontrol_lang() .'"></div>
          <script type="text/javascript" src="http://messenger.services.live.com/users/'. $key .'/presence?dt=&amp;mkt='. imcontrol_lang() .'&amp;cb=Microsoft_Live_Messenger_PresenceButton_onPresence"></script></div>';

        unset($theme); // Destroy variable
      }
      unset($settings); // Destroy variable
      break;
  }
  return $output;
}