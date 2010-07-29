<?php
  // don't waste time on anonymous users
  global $user;
  if ($user->uid < 1) { 
    exit;
  }


  switch ($_POST['func']) {
    case 'storeScore':
        //watchdog('arcade','triggered storeScore');
        $response = pnFlashGames_storeScore();
        print $response;
        break;
    case 'saveGame':
        $response = pnFlashGames_saveGame();
        print $response;
        break;
    case 'loadGame':
        $response = pnFlashGames_loadGame();
        print $response;
        break;
    case 'loadGameScores':
        $response = pnFlashGames_loadGameScores();
        print $response;
        break;
  }
  

function pnFlashGames_storeScore(){
    //watchdog('arcade','called storeScore');
	$success = "&opSuccess=true&endvar=1";
        $failure = "&opSuccess=false&endvar=1";
        
	global $user;
	$uid = $user->uid;
        if ($uid < 1) {
            return $failure;
        }
        $gid = $_POST['gid'];        
	$score = $_POST['score'];
	
	// Get the game's information
	$game = db_fetch_object(db_query("SELECT * FROM {arcade_games} WHERE nid = %d", $gid));
	
    if($game->game_type == '3' || $game->game_type == '4'){
        //This game uses a time based scoring method
        if(strstr($score, ":") !== false){
            // a formated time string was passed... convert it to seconds

            $timestamp = strtotime($score);
            $formatedTime = strftime("%H:%M:%S", $timestamp);
            $hours = substr($formatedTime, 0, 2);
            $minutes = substr($formatedTime, 3, 2);
            $seconds = substr($formatedTime, 6, 2);
            $numSeconds = (($hours * 60) * 60) + ($minutes * 60) + $seconds;

            $score = $numSeconds;
        }else{
            // a straight up integer value was passed, store it straight up as a number of seconds.
        }
    }

    // First check to see if this user has stored a high score for this game yet.
    // Each user is allowed to store one score per game, so we check first to
    // make sure this is not below a previous score
    $checksql = "SELECT score FROM {arcade_data}
            	  WHERE  uid=%d
            	    AND  nid=%d";
    $check = db_fetch_object(db_query($checksql, $uid, $gid));
    
    if(!$check) {
        //No rows found; this user can store a new score for this game
        $sql = "INSERT INTO {arcade_data}
                SET score='%s',
                    uid=%d,
                    nid=%d,
                    date=NOW()";
        
        /*
        At this point you may want to implement a pruning method that would
        limit the number of scores per game.  For example, if there are 500 scores
        for a particular game, but you are only displaying the top 50 then why
        keep the other 450?  It only slows down queries.
        */
					
    } else {
        $oldscore = $check->score;
        switch($game->game_type) {
            case '1':
	    case '3':
            default:
            //watchdog('arcade',"old: $oldscore - new: $score");
                if($oldscore < $score) {
                    //Row found but the new score is higher, so update the old one
                    $sql = "UPDATE {arcade_data}
                              SET    score='%s', date=NOW()
                              WHERE  uid=%d
                              AND    nid=%d";
                    $output = $success;
                } else {
                    //Row found but the new score is lower, so do nothing
                    $sql = "";
                    $output = $failure;
                }
                break;
            case '2':   //both 2 and 4 are lowest score wins type games, so store if the oldscore was higher
            case '4':   //at this point, even though 4 is time based, score has been converted to seconds, so this will still work fine
                if($oldscore > $score) {
                    //Row found but the new score is higher, so update the old one
                    $sql = "UPDATE {arcade_data}
                            SET    score='%s', date=NOW()
                            WHERE  uid=%d
                            AND    nid=%d";
                    $output = $success;
                } else {
                    //Row found but the new score is lower, so do nothing
                    $sql = "";
                    $output = $failure;
                }
        }
    }
    if($sql !== "") {
		db_query($sql, $score, $uid, $gid);
    }
	
    // Return response
    return $output;
}

function pnFlashGames_saveGame(){
//TODO - SQL security escaping
	$gid = $_POST['gid'];
	$gameData = $_POST['gameData'];
        
        global $user;
	$uid = $user->uid;
	
    // First check to see if this user has stored game data for this game yet.
    // Each user is allowed to store one game data, so we check first to
    // make sure this is not below a previous game data
    $checksql = "SELECT COUNT(1) as rowcount FROM {arcade_data}
            	 WHERE  uid=%d
            	 AND    nid=%d";
    $check = db_query($checksql, $uid, $gid);
	$count = db_fetch_array($check);
	$count = $count["rowcount"];

    if($count < 1){
        //No rows found, this user has not stored a high score for this game yet
        $result = db_query("INSERT INTO {arcade_data}
                SET nid=%d,
                    uid=%d,
                    gamedata='%s',
                    date=NOW()", $gid, $uid, $gameData);
    }else{
        //old gameData found so replace it with the new one.
        $result = db_query("UPDATE {arcade_data}
                SET    gamedata='%s',
                       date=NOW()
                WHERE  uid=%d
                AND    nid=%d", $gameData, $uid, $gid);
    }
    
    // Check for an error with the database code, and if so set an appropriate
    // error message and return
    if ($result) {
      return "&opSuccess=true&endvar=1";
    } else {
      return "&opSuccess=false&error=Error saving game data to database&endvar=1";
    }

}

function pnFlashGames_loadGame(){
	$gid = $_POST['gid'];
	global $user;
	$uid = $user->uid;
	
	
    $result = db_result(db_query("SELECT gamedata
            FROM {arcade_data}
            WHERE nid=%d
            AND uid=%d", $gid, $uid));
    
    
    // Check for an error with the database code, and if so set an appropriate
    // error message and return
    if ($result) {
      //Flash will unencode the data automatically, this way the data is sent back exactly as it came...
      $gamedata = urlencode($result);    
      return "&opSuccess=true&gameData={$gamedata}&endvar=1";
    } else {
      return "&opSuccess=false&error=Error getting game data from database&endvar=1";
    }
}

function pnFlashGames_loadGameScores(){
	$gid = $_POST['gid'];
	
         // Get the game's information
        $game = db_fetch_object(db_query("SELECT * FROM {arcade_games} WHERE nid = %d", $gid));
        
	if($game->game_type == 2 || $game->game_type == 4){
		$orderby = "ASC";
	}else{
		$orderby = "DESC";
	}
	
    $sql = "SELECT uid, score, date
            FROM {arcade_data}
            WHERE nid=%d
	      AND score IS NOT NULL
            ORDER BY CAST(score AS SIGNED INTEGER) $orderby";

    $result = db_query($sql, $gid);
	
	$output = "<scorelist>\n";
	//$dateInfo = getdate();
	$rank = 1;
	while($highscore = db_fetch_array($result)) {
		//Found some highscores
                $name = db_result(db_query("SELECT name FROM {users} WHERE uid=%d",$highscore['uid']));
		$output .= "<score rank='".$rank++."' score='{$highscore[score]}' player='{$name}' date='{$highscore[date]}' />\n";
                //print_r($highscore);
		}
	
	$output .= "</scorelist>";
	if ($_POST['raw']==1) {
            $output = "<xml>\n". $output ."</xml>\n";   
        } else {
            $output = "&opSuccess=true&gameScores=". urlencode($output) ."&endvar=1"; //Flash needs the xml urlencoded
        }
	return $output;
}
