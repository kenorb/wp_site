<?php

/**
 * @file
 *   Include file contain additional functions
 *
 * @version
 *   $Id: wsod.inc,v 1.1.2.4 2009/06/22 12:18:40 kenorb Exp $
 *
 * @developers:
 *   Rafal Wieczorek <kenorb@gmail.com>
 */

/** 
 * Validate theme hook files and callbacks
 * 
 * @param string $router_item
 * @return array or false 
 */ 
function wsod_validate_theme_hooks($verbose = FALSE, &$output) {
    $hooks = theme_get_registry(); // get list of theme hook
    $theme_test = true;
    $nl = t('<br>');
    $output .= t("Validating theme hooks...").$nl;
    foreach ($hooks as $hook_name => $hook) {
        if (isset($hook['path']) && !file_exists($hook['path'])) {
            $output .= t("ERROR: File or path doesn't exist (path: %path; theme callback: %function)", array('%path' => $hook['path'], '%function' => $hook_name)).$nl;
            $theme_test = false;
        }
        $filepath = $hook['path'] ? $hook['path'].'/'.$hook['file'] : $hook['file'];
        if (isset($hook['file']) && !file_exists($filepath)) {
            $output .= t("ERROR: File or path doesn't exist (path: %path; theme callback: %function)", array('%path' => $filepath, '%function' => $hook_name)).$nl;
            $theme_test = false;
        }
        if (isset($hook['theme path']) && !file_exists($hook['theme path'])) {
            $output .= t("ERROR: Theme path doesn't exist (path: %path; theme callback: %function)", array('%path' => $hook['theme path'], '%function' => $hook_name)).$nl;
            $theme_test = false;
        }
        if (isset($hook['access_callback']) && !function_exists($hook['access_callback'])) {
            $output .= t("ERROR: Access callback doesn't exist (callback: %function)", array('%function' => $hook['access_callback'])).$nl;
            $theme_error = false;
        }
    }
    return $theme_test;
}

/** 
 * Get menu items and simulate menu router callback
 * 
 * @param string $router_item
 * @return array or false 
 */ 
function wsod_check_page_callback($router_item, $verbose = FALSE, &$messages, &$content_output) {
    $res = FALSE;
    ob_start(); // start output buffering
    $nl = t('<br>');
    if ($router_item) {
        if ($router_item['access']) {
            if ($router_item['file']) {
                require_once($router_item['file']);
                $file = basename($router_item['file']);
                $messages .= t("Included render file: %file",array('%file' => $file)).$nl; // PRINT included file
            }
            $messages .= t("Checking %function() page callback...",array('%function' => $router_item['page_callback'])).$nl; // PRINT included file
            $res = call_user_func_array($router_item['page_callback'], $router_item['page_arguments']);
        } else {
            $messages .= t("User permission denied to execute %function() page callback...",array('%function' => $router_item['page_callback'])).$nl;
        }
    }
    $content_output = ob_get_clean(); // get output
    return $res;
}

