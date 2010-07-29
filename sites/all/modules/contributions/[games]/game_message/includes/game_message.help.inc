<?php
// $Id: game_message.help.inc,v 1.1 2009/01/15 03:47:18 aaron Exp $

/**
 *  Callback for the game queue help page.
 */

/**
 *  Define the help pages for hook_help().
 */
function _game_queue_help($section) {
  return t("
    <p>Game clocks may be used for various purposes, such as keeping an in-game calendar, tracking game effects and events, and limiting characters to acting at a slower pace suitable for a web-based game.</p>
    <p>You may view all active game clocks from the !game_queue page, as well as create new clocks there. Additionally, you can create new game clocks programmatically with the API provided with the Game Clock module.</p>
    <p>To do so, you would create a game clock array as follows:</p>
<pre>  \$game = array(
    'name' => \$name,           // A unique machine-name.
    'title' => \$title,         // A human-readable title.
    'type' => \$type,           // An optional string. Some modules may act only
                               // on events by clocks of a certain type.
    'status' => \$status,       // If TRUE, then the clock will begin started.
                               // If FALSE it begins paused. Defaults to FALSE.
    'turn' => \$turn,           // The current turn to begin the clock.
                               // Defaults to 0.
    'increment' => \$increment, // How many seconds before incrementing to the
                               // next turn. Defaults to 0 (never; must be
                               // manually incremented).
    'block' => \$block,         // If TRUE, then a block displaying this clock's
                               // current turn and status will become
                               // available. Defaults to FALSE.
    'init' => \$init,           // If TRUE, then the clock will be checked for
                               // incrementation on every page load, assuming
                               // the proper time has passed.
  );
  game_queue_create(\$game);
</pre>
    <p>Other available functions in the Game Clock API that may be useful:</p>
<pre>
  game_queue_state(\$clock = NULL);
  game_queue_pause(\$clock = 'default', \$status = FALSE);
  game_queue_start(\$clock = 'default');
  game_queue_increment(\$clock = 'default', \$force = FALSE);
  game_queue_reset(\$clock = 'default', \$turn = NULL);
  game_queue_create(\$state, \$report_errors = FALSE)
  game_queue_save(\$clock, \$report_errors = FALSE);
  game_queue_delete(\$clock, \$report_errors = FALSE);
</pre>
    <p>Additionally, you may create a hook_game_queue function in your module to act on game clock events, as follows, noting that \$state will be an object, rather than an array. All of these functions are called with (\$clock = 'default', \$state = NULL); where \$clock is the name of the clock, and \$state is the object representing the clocks state.</p>
<pre>
  // A clock named \$clock has been created with \$state.
  hook_game_queue_create(\$clock = 'default', \$state = NULL);

  // The clock named \$clock has just been paused.
  hook_game_queue_pause(\$clock = 'default', \$state = NULL);

  // The clock named \$clock has just been started.
  hook_game_queue_start(\$clock = 'default', \$state = NULL);

  // The clock named \$clock has just been incremented a tick.
  hook_game_queue_increment(\$clock = 'default', \$state = NULL);

  // The clock named \$clock has just been reset to 0 turns.
  hook_game_queue_reset(\$clock = 'default', \$state = NULL);

  // The clock named \$clock has just been edited and saved.
  hook_game_queue_save(\$clock = 'default', \$state = NULL);

  // The clock named \$clock has just been deleted.
  hook_game_queue_delete(\$clock = 'default', \$state = NULL);
</pre>
    <p>Please read the documentation in the game_queue.module file for more information.</p>
", array('!game_queue' => l(t('Game Clock administration'), 'admin/settings/game_queue')));
}
