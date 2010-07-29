<?php
// $Id$

/**
 * Menu callback for the settings form.
 */
function database_tweaks_settings_form() {
    global $db_type;
    switch ($db_type) {
        case 'mysql':
        case 'mysqli':
            // get actual variables from database
            $default_sql_big_selects = database_tweaks_get_mysql_sql_big_selects();
            $default_max_join_size = database_tweaks_get_mysql_max_join_size();
            $default_max_allowed_packet = database_tweaks_get_mysql_max_allowed_packet($max_allowed_packet);

            // get actual variables from configuration
            $sql_big_selects = variable_get('mysql_sql_big_selects', $default_sql_big_selects);
            $max_join_size = variable_get('mysql_max_join_size', $default_max_join_size);
            $max_allowed_packet = variable_get('max_allowed_packet', $default_max_allowed_packet);
            if ((bool)$default_sql_big_selects !== (bool)$sql_big_selects) {
                drupal_set_message(t("Actual SQL_BIG_SELECTS is differ than was set, probably this variable is disabled for changes."),'error');
            }
            if ((int)$default_max_join_size !== (int)$max_join_size) {
                drupal_set_message(t("Actual MAX_JOIN_SIZE is differ than was set, probably this variable is disabled for changes."),'error');
            }
            if ((int)$default_max_allowed_packet !== (int)$max_allowed_packet) {
                drupal_set_message(t("Actual MAX_ALLOWED_PACKET is differ than was set, probably this variable is disabled for changes."),'error');
            }

            $sql_big_selects_description = t('This is useful when an inadvisable WHERE statement has been issued.').
                t('<br>').t('If not active, MySQL aborts SELECT statements that are likely to take a very long time to execute (that is, statements for which the optimizer estimates that the number of examined rows exceeds the value of max_join_size). This is useful when an inadvisable WHERE statement has been issued. As default value for a new connection is enabled, which allows all SELECT statements.');
            $max_join_size_description = t('Set it if your users tend to perform joins that lack a WHERE clause, that take a long time, or that return millions of rows.').
                t('<br>').t('Do not allow SELECT statements that probably need to examine more than max_join_size rows (for single-table statements) or row combinations (for multiple-table statements) or that are likely to do more than max_join_size disk seeks. By setting this value, you can catch SELECT statements where keys are not used properly and that would probably take a long time. Set it if your users tend to perform joins that lack a WHERE clause, that take a long time, or that return millions of rows. Setting this variable to a value other than DEFAULT resets the value of sql_big_selects to 0. If you set the sql_big_selects value again, the max_join_size variable is ignored. If a query result is in the query cache, no result size check is performed, because the result has previously been computed and it does not burden the server to send it to the client.');
            $max_allowed_packet_description = t('The maximum size of one packet. Should be minimum %size. This value by default is small, to catch large (possibly incorrect) packets. You must increase this value if you are using large BLOB columns or long strings. It should be as big as the largest BLOB you want to use. The protocol limit for max_allowed_packet is 1GB.', array('%size' => 1024));

            $form['mysql'] = array(
                '#type' => 'fieldset',
                '#title' => t('MySQL settings'),
                '#collapsible' => TRUE,
            ); 
            $form['mysql']['mysql_sql_big_selects'] = array(
                '#type' => 'checkbox',
                '#title' => t('Enable SQL_BIG_SELECTS value'),
                '#description' => $sql_big_selects_description,
                '#default_value' => $default_sql_big_selects,
            ); 
            $form['mysql']['mysql_max_join_size'] = array(
                '#type' => 'textfield',
                '#title' => t('Default MAX_JOIN_SIZE value'),
                '#value' => $default_max_join_size,
                '#default_value' => $default_max_join_size,
                '#size' => 20,
                '#maxlength' => 20,
                '#description' => $max_join_size_description,
                '#disabled' => $default_sql_big_selects,
                '#field_suffix' => t('joins'),
            ); 
            $form['mysql']['max_allowed_packet'] = array(
                '#type' => 'textfield',
                '#title' => t('Default MAX_ALLOWED_PACKET value'),
                '#default_value' => $default_max_allowed_packet,
                '#size' => 20,
                '#maxlength' => 20,
                '#description' => $max_allowed_packet_description,
                '#field_suffix' => t('kb'),
            ); 
        case 'pgsql':
    }
    $form['#validate'] = array('database_tweaks_settings_form_validate');

    return system_settings_form($form); 
}
 
/**
 * Form API callback to validate the upload settings form.
 */
function database_tweaks_settings_form_validate($form, &$form_state) {
    global $db_type;
    switch ($db_type) {
        case 'mysql':
        case 'mysqli':
            // get necessary data
            $default_sql_big_selects = database_tweaks_get_mysql_sql_big_selects();
            $sql_big_selects = variable_get('mysql_sql_big_selects', $default_sql_big_selects);

            // validate MAX_JOIN_SIZE
            $max_join_size = $form_state['values']['mysql_max_join_size'];
            if (!$default_sql_big_selects && empty($max_join_size)) {
                $default_max_join_size = database_tweaks_get_mysql_max_join_size();
                if (!is_numeric($max_join_size) || ($max_join_size <= 0)) {
                    form_set_error('mysql_max_join_size', t('The MAX_JOIN_SIZE limit must be a number and greater than zero (current size is: %size).', array('%size' => $default_max_join_size)));
                } else {
                    unset($form_state['values']['mysql_max_join_size']);
                }
            }

            // validate MAX_ALLOWED_PACKET
            $max_allowed_packet = $form_state['values']['max_allowed_packet'];
            $default_max_allowed_packet = database_tweaks_get_mysql_max_allowed_packet();
            if (!is_numeric($max_allowed_packet) || ($max_allowed_packet < 1024)) {
                form_set_error('mysql_max_join_size', t('The MAX_ALLOWED_PACKET limit must be a number and greater than 2048 (current size is: %size).', array('%size' => $default_max_allowed_packet)));
            }
        case 'pgsql': ;
    }
} 

