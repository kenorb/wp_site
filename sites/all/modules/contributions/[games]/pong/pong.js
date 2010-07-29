// ***************************************************
// Pong
//
//   Modified for Drupal by Glenn Linde July 2008
//  Original javascript by Joan Alba Maldonado  (granvino@granvino.com). from 
// http://sourceforge.net/projects/punkpong/
//
// ***************************************************

            //Variable que guardara el primer evento de keydo en ejecutarse por primera vez (pude ser onkeydown u onkeypress):
            var primer_evento;

			var pong_inverted=true;
			
			//var factor=1;//0.5;
            //Variable que guarda la dimension del area de game:
            var pong_area_game_x = 100;//factor*400; //Pixels de la horizontal. 700 original
            var pong_area_game_y = 200;//factor*200; //Pixels de la vertical.  400 original
            
			var area_game_x;//factor*400; //Pixels de la horizontal. 700 original
            var area_game_y;//factor*200; //Pixels de la vertical.  400 original
            
            //Variable que guarda el alto de las paddles:
            var pong_paddles_width;// = 30;//factor*40;
            //Variable que guarda el ancho de las paddles:
            var pong_paddles_height;// = 4;//factor*5;

			var pong_paddles_user_color="#f00";
			var pong_paddles_user_image="paddlered.png";
			
			var pong_paddles_computer_color="#00f";
			var pong_paddles_computer_image="paddleblue.png";
			
		
            //Variable que guarda el alto de la ball:
            var pong_ball_height;// = 6;//factor*5;
            //Variable que guarda el ancho de la ball:
            var pong_ball_width;//= 6;//factor*5;

			var pong_ball_color="#000";
			
			var pong_ball_img="ball.png";
			
            //Variable para saber si se ha de extract la ball:
            var se_ha_de_extract = true;

            //Variable que guarda el last winner (para saber quien saca):
            var last_winner = "user";

            //Varialbe que define si la ball esta en movement o quita:
            var ball_moving = false;

            //Variable que guardara el setInterval que llamara a la funcion de move_ball:
            var ball_movement = setInterval("", 10000);
            
            //Variable que guardara el setInterval que hara move al computer:
            var computer_movement = setInterval("", 10000);

            //Variable que contiene el numero de pixels que la ball se desplaza:
            var displacement_x = 4; //displacement horizontal.
            var displacement_y = 1; //displacement vertical.
            
            //Variable que contiene los milisegundos entre movement y movement, para la ball:

			var pong_speed_ball_initial_pixels=20;//40*factor;  //pixels per second 1/35
            var speed_ball_initial;// = parseInt(1000/pong_speed_ball_initial_pixels);// ;//35; //La speed al empezar.
            var speed_ball; //= speed_ball_initial;//speed_ball_initial; //Aqui se ira decrementando (mas rapido) cuando se requiera.

			var pong_countdown=0;//3;
			var scoreboardoffset=10;
			var pong_start_message="Click to start.";
			var pong_computer_winsmessage="Computer beats you.";
			var pong_computer_winningmessage="Computer is beating you.";
			var pong_user_winsmessage="You beat the computer";
			var pong_user_winningmessage="You are beating the computer";
			var pong_tie_message="Currently tied.";
			var pong_gameover_message="Game over";
			var pong_countdown_message="Countdown";
			
			var pong_background="img/drupal.jpg";
			var pong_background_transparency=0.2;
			
            var pong_bestscore;
			var pong_bestuser;
			
			//var hold=0;
			
			//var obj_paddle_user;
			
            //Variable que contendra un numero chance para saber sobre que extremo de la paddle ha de rematar el computer:
            var vertical_chance = 0;

            //Variable que guarda la pong_points del user:
            var pong_points = 0;
            
            //Variables que guardan el numero de goals de cada player:
            var goals_computer = 0; //goals del computer.
            var goals_user = 0; //goals del user.

            //Variable que define los reflejos del computer (se incrementa segun level):
            var reflejos_initiales = 0; //Reflejos initiales.
            var reflejos_computer = reflejos_initiales; //Reflejos que iran aumentando conforme el level.

            //Variable que guarda el numero de level:
            var level = 1; //level initial.
            
            //Numero de lives initial:
            var pong_lives_initial = 5;
            var lives = pong_lives_initial;
            
            //Varialbe que impide el movement de la paddle del user (para cuando se inicia el game):
            var prevent_movement = true;

            //Variable para saber con que se controlara el game:
            var keyboard_control = "mouse";


			function pong_initiate_game()
			{
				// Trick so that paddle images are loaded and can get there width's
				if (pong_paddles_user_image&&pong_paddles_user_image!='')
					document.getElementById('pong_paddle_user').innerHTML='<img id="pong_paddle_user_img" src="'+pong_paddles_user_image+'"/>';
				if (pong_paddles_computer_image&&pong_paddles_computer_image!='')
					document.getElementById('pong_paddle_computer').innerHTML='<img id="pong_paddle_computer_img" src="'+pong_paddles_computer_image+'"/>';
				if (pong_ball_img&&pong_ball_img!="")
					document.getElementById('pong_ball').innerHTML='<img id="ballimg" src="'+pong_ball_img+'"/>';
					
				setTimeout("pong_initiate_game_loaded();",1000);
			}
			
            //Funcion que inicia el game:
            function pong_initiate_game_loaded()
             {
                
                //Se borran los intervalos:
				
                clearInterval(ball_movement); //Se para la ball.
                clearInterval(computer_movement); //Se para la paddle del computer.
                
                //La ball ya no se mueve:
                ball_moving = false;
                
                //Se debe extract:
                se_ha_de_extract = true;
                
                //El last winner (el primero en tirar) es el user:
                last_winner = "user";
                
                //Setear speed a initial, displacements a initial, lives a tope, pong_points a cero, etc...
                lives = pong_lives_initial;
                
                //Se setea la pong_points a cero:
                pong_points = 0;
                
                //Se setean los goals de ambos a cero:
                goals_computer = 0;
                goals_user = 0;
                
                //Se setea el level al primero (initial):
                level = 1;
                
				
                //Se setea la speed de la ball a la initial:
                speed_ball_initial = parseInt(1000/pong_speed_ball_initial_pixels);
                speed_ball = speed_ball_initial;

				//alert(speed_ball);
                //Se setean los reflejos del computer a los initiales:
                reflejos_computer = reflejos_initiales;
                
                //La variable para saber sobre que lado golpear la paddle del computer a la ball se setea a cero:
                vertical_chance = 0;
                
                //Se setea el displacement initial de la ball:
                displacement_x = 2;
                
                displacement_y = 1;
                
                //Se actualizan los scoreboards:
                update_scoreboards();

				if (pong_inverted)
				{
					area_game_x=pong_area_game_y;
					area_game_y=pong_area_game_x;
		
				}
				else
				{
				  area_game_x=pong_area_game_x;
				  area_game_y=pong_area_game_y;
				}
				
				//obj_paddle_user=document.getElementById("pong_paddle_user");
				if (pong_paddles_user_image&&pong_paddles_user_image!='')
				{
					//document.getElementById('pong_paddle_user').innerHTML='<img id="pong_paddle_user_img" src="'+pong_paddles_user_image+'"/>';
					document.getElementById('pong_paddle_user_img').style.display='block';

					if (!pong_paddles_width)
					{
					   pong_paddles_width=parseInt(document.getElementById('pong_paddle_user_img').width);
					   //alert("width"+pong_paddles_width);
					}
					else
					   document.getElementById('pong_paddle_user_img').width=pong_paddles_width;

					if (!pong_paddles_height)
				    {
					   pong_paddles_height=parseInt(document.getElementById('pong_paddle_user_img').height);
					   //alert("height"+pong_paddles_height);
					}
					else
					   document.getElementById('pong_paddle_user_img').height=pong_paddles_height;
				}
				else
				{
				  document.getElementById('pong_paddle_user').style.color=pong_paddles_user_color;
				  document.getElementById('pong_paddle_user').style.background=pong_paddles_user_color;
				}

				if (!pong_paddles_width) 
				  pong_paddles_width=30;
				if (!pong_paddles_height) 
				  pong_paddles_height=10;
				  
				if (pong_inverted)
				{
				
  				  document.getElementById('pong_area_game').style.height=area_game_x+'px';
				  document.getElementById('pong_area_game').style.width=area_game_y+'px';
				  document.getElementById('pong_paddle_user').style.height=pong_paddles_height+'px';
				  document.getElementById('pong_paddle_user').style.width=pong_paddles_width+'px';

				  
				}
				else
				{
  				  document.getElementById('pong_area_game').style.width=area_game_x+'px';
				  document.getElementById('pong_area_game').style.height=area_game_y+'px';
				  document.getElementById('pong_paddle_user').style.width=pong_paddles_height+'px';
				  document.getElementById('pong_paddle_user').style.height=pong_paddles_width+'px';
				  if (document.getElementById('pong_paddle_user_img'))
				  {
					document.getElementById('pong_paddle_user_img').height=pong_paddles_width;
					document.getElementById('pong_paddle_user_img').width=pong_paddles_height;
				  }
				}
				document.getElementById('pong_paddle_user').style.left='0px';
				//if (pong_inverted)
				//  document.getElementById('pong_paddle_user').style.top=area_game_x-pong_paddles_height+'px';
				//else 
				//  document.getElementById('pong_paddle_user').style.left=area_game_x-pong_paddles_height+'px';				
				if (pong_paddles_computer_image&&pong_paddles_computer_image!='')
				{
					//document.getElementById('pong_paddle_computer').innerHTML='<img id="pong_paddle_computer_img" src="'+pong_paddles_computer_image+'"/>';
					document.getElementById('pong_paddle_computer_img').style.display='block';
					if (!pong_paddles_height)
					   pong_paddles_height=parseInt(document.getElementById('pong_paddle_computer_img').height);
					else
					   document.getElementById('pong_paddle_computer_img').height=pong_paddles_height;
					   
					if (!pong_paddles_width)
					   pong_paddles_width=parseInt(document.getElementById('pong_paddle_computer_img').width);
					else
					   document.getElementById('pong_paddle_computer_img').width=pong_paddles_width;
				}
				else
				{
				  document.getElementById('pong_paddle_computer').style.color=pong_paddles_user_color;
				  document.getElementById('pong_paddle_computer').style.background=pong_paddles_computer_color;
				}
				
				if (pong_inverted)
				{
				  document.getElementById('pong_paddle_computer').style.height=pong_paddles_height+'px';
				  document.getElementById('pong_paddle_computer').style.width=pong_paddles_width+'px';
				}
				else
				{
				  document.getElementById('pong_paddle_computer').style.width=pong_paddles_height+'px';
				  document.getElementById('pong_paddle_computer').style.height=pong_paddles_width+'px';
				  if (document.getElementById('pong_paddle_computer_img'))
				  {
					document.getElementById('pong_paddle_computer_img').height=pong_paddles_width;
					document.getElementById('pong_paddle_computer_img').width=pong_paddles_height;
				  }
				  
				}
				document.getElementById('pong_paddle_computer').style.left='0px';
				
				if (pong_inverted)
				  document.getElementById('pong_paddle_computer').style.top=area_game_x-pong_paddles_height+'px';
				else
    		  	  document.getElementById('pong_paddle_computer').style.left=area_game_x-pong_paddles_height+'px';

				if (pong_ball_img&&pong_ball_img!="")
				{
					//document.getElementById('pong_ball').innerHTML='<img id="ballimg" src="'+pong_ball_img+'"/>';
					document.getElementById('ballimg').style.display='block';

					if (!pong_ball_width)
					   pong_ball_width=document.getElementById('ballimg').width;
					else
				   	  document.getElementById('ballimg').width=pong_ball_width;
				
					if (!pong_ball_height)
					   pong_ball_height=document.getElementById('ballimg').height;
					else
				   	  document.getElementById('ballimg').height=pong_ball_height;
				}
				else
				{
				    if (!pong_ball_width)
					  pong_ball_width=10;
				    if (!pong_ball_height)
					  pong_ball_height=10;
					  
					document.getElementById('pong_ball').style.width=pong_ball_width+'px';
					document.getElementById('pong_ball').style.height=pong_ball_height+'px';
					document.getElementById('pong_ball').style.color=pong_ball_color;
					document.getElementById('pong_ball').style.background=pong_ball_color;
				}
				//alert(document.getElementById('ballimg').width);
				//document.getElementById("pong_ball").style.backgroundImage = "url(blue.jpg)";
				//document.getElementById("pong_ball").style.backgroundRepeat = "no-repeat";
				

				if (pong_inverted)
				{
		  	      document.getElementById('pong_scoreboard_computer').style.left='0px';///*area_game_y-*/scoreboardoffset+'px';
		  	      document.getElementById('pong_scoreboard_computer').style.top=(area_game_x-parseInt(document.getElementById('pong_scoreboard_computer').style.height)-scoreboardoffset)+'px';
				}
		 	    else
		  	      document.getElementById('pong_scoreboard_computer').style.left=area_game_x-scoreboardoffset+'px';
				//document.getElementById('pong_scoreboard_computer').style.top=area_game_y-scoreboardoffset+'px';
				
				if (pong_inverted)
				  document.getElementById('pong_scoreboard_user').style.top=scoreboardoffset+'px';

				document.getElementById('pong_scoreboard_user').style.left='0px';//scoreboardoffset+'px';
				
				
				
				if (pong_inverted)
				{
  				  document.getElementById('pong_state').style.width=area_game_y+'px';
				  document.getElementById('pong_message').style.left=((area_game_y/2)-(100/2))+'px';
				  document.getElementById('pong_message').style.top=((area_game_x/2)-(40/2))+'px';
				  document.getElementById('pong_background_image').style.height=area_game_x+"px";
				  document.getElementById('pong_background_image').style.width=area_game_y+"px";
				}
				else
				{
  				  document.getElementById('pong_state').style.width=area_game_x+'px';
				  document.getElementById('pong_message').style.left=((area_game_x/2)-(100/2))+'px';
				  document.getElementById('pong_message').style.top=((area_game_y/2)-(40/2))+'px';
				  document.getElementById('pong_background_image').style.width=area_game_x+"px";
				  document.getElementById('pong_background_image').style.height=area_game_y+"px";
				}
				document.getElementById('pong_background_image').src=pong_background;
				document.getElementById('pong_background_image').style.opacity=pong_background_transparency;
				document.getElementById('pong_background_image').style.MozOpacity=pong_background_transparency;
				document.getElementById('pong_background_image').style.filter='alpha(opacity='+parseInt(100*pong_background_transparency)+')';//.alpha.opacity=parseInt(100*pong_background_transparency);
				
				
				//document.getElementById('div_desenfocar').style.visibility='visible'; 
				//formulario_control.desenfocar.focus();
				//document.getElementById('div_desenfocar').style.visibility='hidden'
                //Se positiona la ball en la paddle:
                position_ball(0);
                
                //Se hace visisble el cartel de anuncios e inicia la cuentra atras:
				if (pong_countdown>0)
				{
                  setTimeout("document.getElementById('pong_message').style.visibility = 'visible'; ", 1000);
				  for (i=0; i<pong_countdown; i++)
				  {
				   //alert("document.getElementById('pong_message').innerHTML = 'pong_countdown<br>Wait...<br>'"+(i+1)+";");
                     setTimeout("document.getElementById('pong_message').innerHTML = '"+pong_countdown_message+"<br/>"+(i+1)+"';", (i+1)*1000);
		   	      }
               // setTimeout("document.getElementById('pong_message').innerHTML = 'pong_countdown<br>Wait...<br>2';", 2000);
               // setTimeout("document.getElementById('pong_message').innerHTML = 'pong_countdown<br>Wait...<br>1';", 3000);
                
                //Despues de la cuenta atras, se anuncia que comienza el game y se desbloquea la paddle del user para que se pueda mover:
                  setTimeout("document.getElementById('pong_message').innerHTML = '"+pong_start_message+"'; prevent_movement = false;", (pong_countdown+1)*1000);
                  setTimeout("document.getElementById('pong_message').style.visibility = 'hidden';", (pong_countdown+4)*1000); //Se quita el cartel a los 3 segundos del "Comienza el game".
				}
				else
				{
				  document.getElementById('pong_message').style.visibility = 'visible';
				  document.getElementById('pong_message').innerHTML = pong_start_message;
				  prevent_movement = false;
				  //setTimeout("document.getElementById('pong_message').style.visibility = 'hidden';", 3000);
			    }
                
             }

            //Funcion que comienza el movement de la ball:
            function extract_ball()
             {
                //Calcular si se ha de extract:
                //if (se_ha_de_extract) { sadfsd }
                if (!ball_moving && se_ha_de_extract)
                 {
                    //Se setea la variable para saber que ya se ha sacado:
                    se_ha_de_extract = false;
                        
                    //Se setea la variable para saber que la ball ha comenzado a move:
                    ball_moving = true;

                    //Se destruye el movement anterior, si aun existiera:
                    clearInterval(ball_movement);

                    //Si al que le toca extract es el user:
                    if (last_winner == "user")
                     {
                       //Se define el displacement (positivo):
					   //if (pong_inverted)
					   //{
                        // displacement_x = -4;
                       //  displacement_y = -1;
					   //}
					   //else
					   //{
                         displacement_x = 4;
                         displacement_y = 1;
					   //}

                       //Numero chance para calcular en un lado de la paddle o en otro:
					   // factor chance to the size of the board
                       vertical_chance = parseInt(Math.random() * ((area_game_y/400)*40));
                     }
                    else if (last_winner == "computer")
                     {
                       //Se define el displacement (positivo):
					   //if (pong_inverted)
					   //{
                       //  displacement_x = 4;
                       //  displacement_y = 1;
						//}
						//else
					   //{
                         displacement_x = -4;
                         displacement_y = -1;
						//}
                     }

                   //Se comienza a mover la ball 2x2 pixels, cada X (speed_ball) milisegundos:
                  //  document.getElementById("pong_message").innerHTML = speed_ball;//"End<br>of<br>game";
				   
                   ball_movement = setInterval("move_ball();", speed_ball);

                 }
             }

            //Funcion que positiona la ball en la paddle de quien corresponde:
            function position_ball(position_y)
             {
                 //Si la ball no esta moving y se debe extract:
                 if (!ball_moving && se_ha_de_extract)
                  {

                    //Se positiona la ball verticalmente, segun el parametro enviado:
					// Seems to be case where starting again
					if (pong_inverted)
					{	
						//document.getElementById("pong_ball").style.top = area_game_x- (pong_paddles_width /2)- (pong_ball_height/2) /*+ 36 */+ "px";
						document.getElementById("pong_ball").style.left = position_y + (pong_paddles_width /2) - (pong_ball_height/2) /*+ 36 */+ "px";
					}
					else
						document.getElementById("pong_ball").style.top = position_y + (pong_paddles_width /2) - (pong_ball_height/2) /*+ 36 */+ "px";
                   
                    //Si el last winner es el user (el que saca), positionamos la ball en su paddle:
                    if (last_winner == "user") {
						if (pong_inverted)
					      document.getElementById("pong_ball").style.top = /*30 +*/ pong_paddles_height + "px";
					     // document.getElementById("pong_ball").style.top = /*area_game_x-*/ (pong_paddles_width /2)- (pong_ball_height/2) /*+ 36 */+ "px";//*30 +*/ pong_paddles_height + "px";
						else
					      document.getElementById("pong_ball").style.left = /*30 +*/ pong_paddles_height + "px";
					}
                    //...y si el winner es el computer, se pone la ball en su paddle para que saque el:
                    else if (last_winner == "computer") {
						if (pong_inverted)
					     document.getElementById("pong_ball").style.top = area_game_x /*- 30*/ - (pong_ball_height/2) - (pong_paddles_height/2) + "px";
					     //document.getElementById("pong_ball").style.top =  (pong_ball_height/2)  + (pong_paddles_height/2) + "px";
					   else
					     document.getElementById("pong_ball").style.left = area_game_x /*- 30*/ - (pong_ball_height/2) - (pong_paddles_width/2) + "px";
					}
                  }

                //Se actualizan los scoreboards:
                update_scoreboards();

                //Si se marcan 3 goals (o mas, por si acaso), se pasa de level:
                if (goals_user >= 3) { pasar_level(); }
                
                //Si se pierden todas las lives, se alerta del GameOver y se reinicia el game:
                if (lives < 0)
                 {
                    //Se setea conforme la ball ya no se mueve:
                    ball_moving = false;
                    //Se setea conforme ya no se ha de extract:
                    se_ha_de_extract = false;
                    //Se para la ball:
                    clearInterval(ball_movement);
                    //Se detiene la paddle del computer:
                    clearInterval(computer_movement);

                    //Se positiona la paddle del user arriba del todo:
					if (pong_inverted)
						document.getElementById("pong_paddle_user").style.left = "0px";
					else
						document.getElementById("pong_paddle_user").style.top = "0px";

                    //Se anuncia que se ha acabado el game:
                    document.getElementById("pong_message").innerHTML = pong_gameover_message;//"End<br>of<br>game";
                    document.getElementById("pong_message").style.visibility = "visible";
					
					document.getElementById('pong_formmessage').innerHTML = pong_gameover_message;

					if (document.getElementById('pong_winform')&&pong_points!=0&&
						(!pong_bestscore||pong_bestscore==-1||pong_points>pong_bestscore))
					{
							document.getElementById('pong_message').value=document.getElementById('pong_formmessage').innerHTML;
							document.getElementById('pong_ticker').value=pong_points;
							document.getElementById('pong_winform').submit();
					}							  
					
                   // setTimeout("document.getElementById('pong_message').style.visibility = 'hidden';", 3000);

                    //Se alerta del fin de game:
                    //alert("Game Over");

                    prevent_movement = true;
                 
                    //En 3 segundos (3000 milisegundos) comienza el nuevo game:
                    //setTimeout("initiate_game();", 3000);
                 }
             }

            //Funcion que captura las keys toucheds y realiza la accion correspondiente (llama a move_paddle):
            function key_touched(e, evento_actual)
             {
                //Si el primer evento esta vacio, se le introduce como valor el evento actual (el que ha llamado a esta funcion):
                if (primer_evento == "") { primer_evento = evento_actual; }
                //Si el primer evento no es igual al evento actual (el que ha llamado a esta funcion), se vacia el primer evento (para que a la proxima llamada entre en la funcion) y se sale de la funcion:
                if (primer_evento != evento_actual) { primer_evento = ""; return; }

                //Si el control seleccionado no es el keydo, sale de la funcion:
                if (keyboard_control != "keydo") { return; }

                //Capturamos la tacla touched, segun navegador:
                if (e.keyCode) { var unicode = e.keyCode; }
                //else if (event.keyCode) { var unicode = event.keyCode; }
                else if (window.Event && e.which) { var unicode = e.which; }
                else { var unicode = 40; } //Si no existe, por defecto se utiliza la flecha hacia abajo.

                //Si el movement de la paddle del user esta impedido, se sale de la funcion:
                if (prevent_movement) { return false; }

                //Definimos las variables de position (Y) del personaje:
                var position_y = parseInt(document.getElementById("pong_paddle_user").style.top);

                //Si se pulsa la flecha hacia arriba, se restan 40 pixels verticales:
                if (unicode == 38) { position_y -= 40; }
                //Si se pulsa la flecha hacia abajo, se suman 40 pixels verticales:
                else if (unicode == 40) { position_y += 40; }

                //Codigos de keys de disparo: 17 (ctrl), 16 (shift), 32 (space), 13 (return), 45 (insert), 96 (0), 190 (.).
                //Si el codigo es una de las keys de disparo:
                else if (unicode == 39 || unicode == 17 || unicode == 16 || unicode == 32 || unicode == 13 || unicode == 45 || unicode == 96 || unicode == 190)
                 {
                    //Si la ball no esta moving, se ha de extract y el que ha de extract es el user:
                    if (!ball_moving && se_ha_de_extract && last_winner == "user")
                     {
                        //Se saca la ball:
                        extract_ball();
                     }
                 }

                //Se mueve la paddle, a las nuevas coordenadas (si existen):
                move_paddle(position_y, "user");
  
                //Sale de la funcion, retornando true:
                return true;
             }
             
            
            //Funcion que mueve la paddle con el mouse:
            function move_mouse(e)
             {
                //Si el control seleccionado no es el mouse, sale de la funcion:
                if (keyboard_control != "mouse") { return; }
                
                //Si el movement de la paddle del user esta impedido, se sale de la funcion:
                if (prevent_movement) { return false; }
                
                //Variable para saber si estamos en Internet Explorer o no:
                var ie = document.all ? true : false;
                //Si estamos en internet explorer, se recogen las coordenadas del mouse de una forma:
				if (ie)
					id=event.srcElement.id;
				else
					id=e.target.id;
				if (id!='ballimg'&&id!='pong_paddle_user'&&id!='pong_message'&&
					    id!='pong_paddle_user_img'&&id!='pong_ball')
				{
				
					if (ie)
					{

						if (pong_inverted)
							position_y_mouse = event.offsetX + document.body.scrollLeft;
						else
							position_y_mouse = event.offsetY + document.body.scrollTop;
						//position_y_mouse = event.clientX + document.body.scrollLeft;
					}
					//...pero en otro navegador, se recogen de otra forma:
					else
					{
					//if (e.target.id!='ballimg'&&e.target.id!='pong_paddle_user'&&e.target.id!='pong_message'&&
					//    e.target.id!='pong_paddle_user_img'&&e.target.id!='pong_ball')
					//{
						document.captureEvents(Event.MOUSEMOVE);
						if (pong_inverted)
							position_y_mouse = e.layerX;//-document.body.scrollLeft;
						else
							position_y_mouse = e.layerY;//-document.body.scrollTop;
					}
					//alert(e.target.id);
  			        //alert(e.layerX+" "+document.getElementById("pong_area_game").offsetLeft+" "+position_y_mouse); 
					
                 } 
                //Si las coordenadas X o Y del mouse son menores que cero, se ponen a cero:
                if (position_y_mouse < 0) {
				  position_y_mouse = 0;
				  
				}
                if (position_y_mouse >= area_game_y) {
				   position_y_mouse = area_game_y - parseInt(pong_paddles_width/2) /*+ parseInt(pong_ball_height*2)*/;
				}

                //Definimos las variables de position (Y) del personaje:
				var position_y;
				if (pong_inverted)
					position_y = parseInt(document.getElementById("pong_paddle_user").style.left);
				else
					position_y = parseInt(document.getElementById("pong_paddle_user").style.top);

                //Se setea la variable de la position (Y) de la paddle igual que la del mouse:
                position_y = position_y_mouse - parseInt(pong_paddles_width/2) ;//- (pong_ball_height*2);

                //Se mueve la paddle, a las nuevas coordenadas (si existen):
                move_paddle(position_y, "user");
             }
                          
             
            //Funcion que saca la ball al do click con el mouse:
            function do_click()
             {
                //Si el control seleccionado no es el mouse, sale de la funcion:
                if (keyboard_control != "mouse") { return; }
                
                //Si el movement de la paddle del user esta impedido, se sale de la funcion:
                if (prevent_movement) { return false; }
                
                //Si la ball no esta moving, se ha de extract y el que ha de extract es el user:
                if (!ball_moving && se_ha_de_extract && last_winner == "user")
                 {
                     //Se saca la ball:
					 document.getElementById('pong_message').style.visibility = 'hidden';
                     extract_ball();
                 }
             }
            
             
             
            //Funcion que mueve al enemy:
            function move_enemy(ball_y)
             {
                //Si ball_moving = false, salir de la funcion:
                if (!ball_moving) { return false; }
                
                //Variable que guarda la nueva position vertical del enemy:
                var enemy_y;
				if (pong_inverted)
					enemy_y = parseInt(document.getElementById("pong_paddle_computer").style.left);
				else
					enemy_y = parseInt(document.getElementById("pong_paddle_computer").style.top);
               // document.getElementById("pong_message").innerHTML=document.getElementById("pong_paddle_computer").style.left;
				hold_enemy_y=enemy_y;
				
                //Variable aleatoria para definir si el enemy va a move o no:
                var move_enemy = parseInt(Math.random() * 10); //Va del 0 al 9, y si es mayor que 8 entonces no se mueve.
                if (move_enemy > reflejos_computer) { return false; } //Si es mayor a 8, sale de la funcion (el enemy no se mueve).
                
                //Si la ball esta mas arriba que la paddle del enemy, el enemy sube:
                if (ball_y < enemy_y)
                 {
                    if (enemy_y - 10 >= 0 && enemy_y - 10 <= ball_y)
                     {
                        enemy_y -= 10;
                     }
                    else if (enemy_y - 4 >= 0 && enemy_y - 4 <= ball_y)
                     {
                        enemy_y -= 4;
                     }
                    else if (enemy_y - 2 >= 0 && enemy_y - 2 <= ball_y)
                     {
                        enemy_y -= 2;
                     }
                    else if (enemy_y - 1 >= 0 && enemy_y - 1 <= ball_y)
                     {
                        enemy_y -= 1;
                     }
                    else if (enemy_y - 10 >= 0)
                     {
                        enemy_y -= 10;
                     }
                    else if (enemy_y - 4 >= 0)
                     {
                        enemy_y -= 4;
                     }
                    else if (enemy_y - 2 >= 0)
                     {
                        enemy_y -= 2;
                     }
                    else if (enemy_y - 1 >= 0)
                     {
                        enemy_y -= 1;
                     }

                    if (enemy_y - vertical_chance >= 0 && enemy_y - vertical_chance <= ball_y)
                     {
                        enemy_y -= vertical_chance;
                     }

                 }
                //...pero si la ball esta mas abajo que la paddle del enemy, el enemy baja:
                else if (ball_y > enemy_y + pong_paddles_width)
                 {
                    if (enemy_y + 10 <= area_game_y && enemy_y + 10 <= ball_y)
                     {
                        enemy_y += 10;
                     }
                    if (enemy_y + 4 <= area_game_y && enemy_y + 4 <= ball_y)
                     {
                        enemy_y += 4;
                     }
                    else if (enemy_y + 2 <= area_game_y && enemy_y + 2 <= ball_y)
                     {
                        enemy_y += 2;
                     }
                    else if (enemy_y + 1 <= area_game_y && enemy_y + 1 <= ball_y)
                     {
                        enemy_y += 1;
                     }
                    else if (enemy_y + 10 <= area_game_y)
                     {
                        enemy_y += 10;
                     }
                    else if (enemy_y + 4 <= area_game_y)
                     {
                        enemy_y += 4;
                     }
                    else if (enemy_y + 2 <= area_game_y)
                     {
                        enemy_y += 2;
                     }
                    else if (enemy_y + 1 <= area_game_y)
                     {
                        enemy_y += 1;
                     }


                    if (enemy_y + vertical_chance <= area_game_y && enemy_y + vertical_chance <= ball_y)
                     {
                        enemy_y += vertical_chance;
                     }
                 }
				 
                if (hold_enemy_y!=enemy_y)
				{
					// factor it to the size of the board so doesnt jerk on small board
					diff=parseInt((area_game_y/200)*(hold_enemy_y-enemy_y));
					if (diff!=0)
					{
					   //alert(diff+"  "+(hold_enemy_y-enemy_y));
					   enemy_y = hold_enemy_y-diff;
					}
                }
                //Mueve al enemy con la nueva configuracion:
                move_paddle(enemy_y, "computer");
             }
            
            //Funcion que mueve la paddle segun las ordenadas (vertical) enviadas por key_touched:
            function move_paddle(position_y, quien_mueve)
             {
                //Se define la variable para saber si se ha movido la paddle o no:
                var se_ha_movido_paddle = false;
                
				//if (quien_mueve == "computer")
				//	document.getElementById("pong_message").innerHTML="nn"+position_y;
				
                //Si la position enviada esta fuera de la zona de game, sale de la funcion sin mover la paddle y retornando false:
                if (position_y < 0 || position_y > area_game_y - pong_paddles_width) { 
				  return false;
				}
                //Si el que mueve es el user, mueve su paddle a la nueva position:
                else if (quien_mueve == "user") {
				   //hold++;
				   //document.getElementById("pong_message").innerHTML=hold;
				   
				   
					if (pong_inverted)
					  document.getElementById("pong_paddle_user").style.left = position_y + "px";
				    else
					  document.getElementById("pong_paddle_user").style.top = position_y + "px";
					se_ha_movido_paddle = true;
				}
                //...y si el que mueve es el computer, mueve su paddle a la nueva position:
                else if (quien_mueve == "computer") {
					if (pong_inverted)
						document.getElementById("pong_paddle_computer").style.left = position_y + "px";
					else
						document.getElementById("pong_paddle_computer").style.top = position_y + "px";
					se_ha_movido_paddle = true;
				}

                //Si la ball no se esta moviendo, y se ha de extract y se ha movido la paddle, mover tambien la ball:
                if (!ball_moving && se_ha_de_extract && se_ha_movido_paddle && quien_mueve == last_winner) {
				    position_ball(position_y);
				}
              
                //Sale de la funcion, retornando true:
                return true;
             }

            //Funcion que mueve la ball:
            function move_ball ()
             {
                //Si la ball esta parada, salir de la funcion:
                if (!ball_moving) {
				  return;
				}
                
                //if (document.getElementById("pong_ball").style.top < 0 || document.getElementById("pong_ball").style.left > area_game_y) { return; }
                
                //Mueve la ball:
				if (pong_inverted)
				{
					document.getElementById("pong_ball").style.top = parseInt(document.getElementById("pong_ball").style.top) + displacement_x + "px"; //Horizontalmente.
					//Si la nueva position vertical va a ser menor que cero, dejarla arriba del todo (sin pasar el borde):
					if (parseInt(document.getElementById("pong_ball").style.left) + displacement_y < 0)
					{	
						document.getElementById("pong_ball").style.left = "0px"; //Se pone arriba del todo, sin pasar el borde.
					}
					//...y si la nueva position vertical va a ser mayor que el alto del area de game, dejarla abajo del todo (sin pasar el borde):
					else if (parseInt(document.getElementById("pong_ball").style.left) + displacement_y + pong_ball_width > area_game_y)
					{
						document.getElementById("pong_ball").style.left = area_game_y - pong_ball_width + "px"; //Se pone abajo del todo, sin pasar el borde.
					}
					//...y si no, la nueva position vertical es correcta y se aplica tal cual:
					else {
						document.getElementById("pong_ball").style.left = parseInt(document.getElementById("pong_ball").style.left) + displacement_y + "px";
					} //Verticalmente. 
                
					//Calcular colision:
					pong_calcular_colision(parseInt(document.getElementById("pong_ball").style.top), parseInt(document.getElementById("pong_ball").style.left));
                
					//Mover al enemy:
					move_enemy(parseInt(document.getElementById("pong_ball").style.left));
				}
                else
				{
					document.getElementById("pong_ball").style.left = parseInt(document.getElementById("pong_ball").style.left) + displacement_x + "px"; //Horizontalmente.
					
					//Si la nueva position vertical va a ser menor que cero, dejarla arriba del todo (sin pasar el borde):
					if (parseInt(document.getElementById("pong_ball").style.top) + displacement_y < 0)
					{	
						document.getElementById("pong_ball").style.top = "0px"; //Se pone arriba del todo, sin pasar el borde.
					}
					//...y si la nueva position vertical va a ser mayor que el alto del area de game, dejarla abajo del todo (sin pasar el borde):
					else if (parseInt(document.getElementById("pong_ball").style.top) + displacement_y + pong_ball_width > area_game_y)
					{
						document.getElementById("pong_ball").style.top = area_game_y - pong_ball_width + "px"; //Se pone abajo del todo, sin pasar el borde.
					}
					//...y si no, la nueva position vertical es correcta y se aplica tal cual:
					else {
						document.getElementById("pong_ball").style.top = parseInt(document.getElementById("pong_ball").style.top) + displacement_y + "px";
					} //Verticalmente. 
                
					//Calcular colision:
					pong_calcular_colision(parseInt(document.getElementById("pong_ball").style.left), parseInt(document.getElementById("pong_ball").style.top));
                
					//Mover al enemy:
					move_enemy(parseInt(document.getElementById("pong_ball").style.top));
                }
             }
            
            //Funcion que calcula la colision entre paddle y ball, y entre ball y pared:
            function pong_calcular_colision (ball_x, ball_y)
             {
                //Calcular si ha colisionado con someone pared (de arriba o abajo):
				// Appears to be where ball hits sides of game
                if (ball_y <= 0 || ball_y >= area_game_y - pong_ball_height)
                 {
                    //Cancela el movement de la ball:
                    clearInterval(ball_movement);
                    //Setear los nuevos valores de displacement de la ball:
                    displacement_y *= -1; //Se invierte el movement vertical.
                    //Mueve la ball con los nuevos valores:
                    ball_movement = setInterval("move_ball();", speed_ball);
                    //Sale de la funcion:
                    return;
                 }

                //Variables que recogen las coordenadas de las paddles:
                var pong_paddle_user_top;
				if (pong_inverted)
					pong_paddle_user_top=parseInt(document.getElementById("pong_paddle_user").style.left);
				else
					pong_paddle_user_top=parseInt(document.getElementById("pong_paddle_user").style.top);
                var pong_paddle_user_left = 0;//parseInt(document.getElementById("pong_paddle_user").style.left);
                var pong_paddle_computer_top;
				if (pong_inverted)
					pong_paddle_computer_top= parseInt(document.getElementById("pong_paddle_computer").style.left);
				else
					pong_paddle_computer_top= parseInt(document.getElementById("pong_paddle_computer").style.top);
                var pong_paddle_computer_left = area_game_x;//parseInt(document.getElementById("pong_paddle_computer").style.left);
                
                //Variable para calcular la colision con el lateral de la paddle con la ball:
                var ball_against_lateral = false;
                
                //Variable para saber si invertir o no el movement de y:
                var invertir_y = false;
                
                //Calcular si la ball ha colisionado con el lateral superior de la paddle del user:
				// Case where hits users paddle
                if (ball_x >= pong_paddle_user_left && ball_x + pong_ball_width <= pong_paddle_user_left + pong_paddles_height && ball_y + pong_ball_height >= pong_paddle_user_top && ball_y <= pong_paddle_user_top)
                {
                    ball_against_lateral = true; //Ha colisionado la ball against el lateral superior de la paddle del user.
                    //Si la ball iba hacia abajo, invertir movement:
                    if (displacement_y > 0) {
					   invertir_y = true;
					}
                 }
                //Calcular si la ball ha colisionado con el lateral inferior de la paddle del user:
                else if (ball_x >= pong_paddle_user_left && ball_x + pong_ball_width <= pong_paddle_user_left + pong_paddles_height && ball_y <= pong_paddle_user_top + pong_paddles_width && ball_y + pong_ball_height >= pong_paddle_user_top + pong_paddles_width)
                 {
                    ball_against_lateral = true; //Ha colisionado la ball against el lateral inferior de la paddle del user.
                    //Si la ball iba hacia arriba, invertir movement:
                    if (displacement_y < 0) {
					   invertir_y = true;
					}
                 }
                //Calcular si la ball ha colisionado con el lateral superior de la paddle del computer:
                else if (ball_x >= pong_paddle_computer_left && ball_x + pong_ball_width <= pong_paddle_computer_left + pong_paddles_height && ball_y + pong_ball_height >= pong_paddle_computer_top && ball_y <= pong_paddle_computer_top)
                 {
                    ball_against_lateral = true; //Ha colisionado la ball against el lateral superior de la paddle del computer.
                    //Si la ball iba hacia abajo, invertir movement:
                    if (displacement_y > 0) {
					   invertir_y = true;
					}
                 }
                //Calcular si la ball ha colisionado con el lateral inferior de la paddle del computer:
                else if (ball_x >= pong_paddle_computer_left && ball_x + pong_ball_width <= pong_paddle_computer_left + pong_paddles_height && ball_y <= pong_paddle_computer_top + pong_paddles_width && ball_y + pong_ball_height >= pong_paddle_computer_top + pong_paddles_width)
                 {
                    ball_against_lateral = true; //Ha colisionado la ball against el lateral inferior de la paddle del computer.
                    //Si la ball iba hacia arriba, invertir movement:
                    if (displacement_y < 0) {
					   invertir_y = true;
					}
                 }

                //Si la ball ha colisionado con el lateral de someone paddle:
                if (ball_against_lateral)
                 {
                    //Se vuelve a setear la variable como estaba, por si acaso:
                    ball_against_lateral = false;
                    //Cancela el movement de la ball:
                    clearInterval(ball_movement);

                    //Setear los nuevos valores de displacement de la ball:
                    if (displacement_x > 0) {
						displacement_x = 8;
					}
                    else {
						displacement_x = -8;
					}
                    if (displacement_y > 0) {
						displacement_y = 8;
					} 
                    else { displacement_y = -8; }

                    //Si esta seteada a true la variable de invertir las y, se aplica:
                    if (invertir_y) {
					  displacement_y *= -1;
					}

                    //Mueve la ball con los nuevos valores:
                    ball_movement = setInterval("move_ball();", speed_ball);
                    //Sale de la funcion:
                    return;
                 }

                //Variable para calcular la colision frontal o con la esquina de someone de las paddles con la ball:
                var ball_against_frontal = false;

                //Variable para saber de quien es la paddle con la que la ball ha colisionado:
                var player_rematador = "";

                //Calcular si la ball ha colisionado con el frontal de la paddle del user:
                if (ball_y + pong_ball_height >= pong_paddle_user_top && ball_y <= pong_paddle_user_top + pong_paddles_width && ball_x <= pong_paddle_user_left + pong_paddles_height && ball_x + pong_ball_width >= pong_paddle_user_left)
                 {
                    ball_against_frontal = true; //Ha colisionado la ball against el frontal de la paddle del user.
                    player_rematador = "user";
                 }
                //Calcular si la ball ha colisionado con el frontal de la paddle del computer:
                else if (ball_y + pong_ball_height >= pong_paddle_computer_top && ball_y <= pong_paddle_computer_top + pong_paddles_width && ball_x + pong_ball_width >= pong_paddle_computer_left && ball_x <= pong_paddle_computer_left)
                 {
                    ball_against_frontal = true; //Ha colisionado la ball against el frontal de la paddle del computer.
                    player_rematador = "computer";
                 }
                
                //Si la ball ha colisionado con el frontal de someone paddle:
                if (ball_against_frontal)
                 {
                    //Se vuelve a setear la variable como estaba, por si acaso:
                    ball_against_frontal = false;
                    //Cancela el movement de la ball:
                    clearInterval(ball_movement);
                    
                    //Setear los nuevos valores de displacement vertical de la ball, segun en que extremo haya colisionado:
                    if (player_rematador == "user")
                     {
                         //Se dan 10 pong_points al user:
                         pong_points += 10;
                         
                         //Se actualizan los scoreboards:
                         update_scoreboards();
                         
                         if (ball_y >= pong_paddle_user_top && ball_y <= pong_paddle_user_top + 20)
                          {
                              if (displacement_y > 0) { displacement_y = 6; }
                              else { displacement_y = -6; }
                          }
                         else if (ball_y > pong_paddle_user_top + 20 && ball_y <= pong_paddle_user_top + 30)
                          {
                              if (displacement_y > 0) { displacement_y = 4; }
                              else { displacement_y = -4; }
                          }
                         else if (ball_y > pong_paddle_user_top + 30 && ball_y <= pong_paddle_user_top + 50)
                          {
                              if (displacement_y > 0) { displacement_y = 2; }
                              else { displacement_y = -2; }
                          }
                         else if (ball_y > pong_paddle_user_top + 50 && ball_y <= pong_paddle_user_top + 60)
                          {
                              if (displacement_y > 0) { displacement_y = 4; }
                              else { displacement_y = -4; }
                          }
                         else if (ball_y > pong_paddle_user_top + 60 && ball_y <= pong_paddle_user_top + 80)
                          {
                              if (displacement_y > 0) { displacement_y = 4; }
                              else { displacement_y = -4; }
                          }
                        else
                         {
                              if (displacement_y > 0) { displacement_y = 10; }
                              else { displacement_y = -10; }
                         }
                        
                        //Numero chance para calcular en un lado de la paddle o en otro:
					   // factor chance to the size of the board
                       vertical_chance = parseInt(Math.random() * ((area_game_y/400)*40));
                       // vertical_chance = parseInt(Math.random() * 40);

                     }
                    else if (player_rematador == "computer")
                     {
                         if (ball_y >= pong_paddle_computer_top && ball_y <= pong_paddle_computer_top + 20)
                          {
                              if (displacement_y > 0) { displacement_y = 6; }
                              else { displacement_y = -6; }
                          }
                         else if (ball_y > pong_paddle_computer_top + 20 && ball_y <= pong_paddle_computer_top + 30)
                          {
                              if (displacement_y > 0) { displacement_y = 4; }
                              else { displacement_y = -4; }
                          }
                         else if (ball_y > pong_paddle_computer_top + 30 && ball_y <= pong_paddle_computer_top + 50)
                          {
                              if (displacement_y > 0) { displacement_y = 2; }
                              else { displacement_y = -2; }
                          }
                         else if (ball_y > pong_paddle_computer_top + 50 && ball_y <= pong_paddle_computer_top + 60)
                          {
                              if (displacement_y > 0) { displacement_y = 4; }
                              else { displacement_y = -4; }
                          }
                         else if (ball_y > pong_paddle_computer_top + 60 && ball_y <= pong_paddle_computer_top + 80)
                          {
                              if (displacement_y > 0) { displacement_y = 6; }
                              else { displacement_y = -6; }
                          }
                        else
                         {
                              if (displacement_y > 0) { displacement_y = 10; }
                              else { displacement_y = -10; }
                         }
                     }

                    //Se incrementan dos pixels el movement horizontal de la ball:
                    if (displacement_x > 0 && displacement_x < 12) {
					   displacement_x += 2;
					}
                    else if (displacement_y < 0 && displacement_x > -12) {
					   displacement_x -= 2;
					}
                    
                    //Se invierte el movement de la ball:
                    displacement_x *= -1; //Se invierte el movement horizontal.
                    //displacement_y *= -1; //Se invierte el movement vertical.
                    
                    //Mueve la ball con los nuevos valores:
                    ball_movement = setInterval("move_ball();", speed_ball);
                    //Sale de la funcion:
                    return;
                 }
                
                //Variable para saber si se ha marcado algun gol:
                var se_ha_marcado = false;
                
                //Si ha marcado un gol el computer al user:
                if (ball_x < /*30 -*/ pong_ball_width/2)
                 {
				     //alert(ball_x);
                    last_winner = "computer"; //Se setea como last winner al computer.
                    clearInterval(ball_movement); //Se detiene la ball.
                    goals_computer++; //Se incrementan los goals marcados por el computer.
                    se_ha_de_extract = true; //Se setea la variable para saber que se ha de extract de nuevo.
                    lives--; //El user pierde una vida.

                    //Se anuncia que el computer ha marcado un gol:
                    if (lives >= 0 && ball_moving)
                     {
                        document.getElementById("pong_message").innerHTML = 	pong_computer_winsmessage;
						//"Computer<br>has<br>marked";
                        document.getElementById("pong_message").style.visibility = "visible";

                        //Se setea para saber que se ha marcado un gol:
                        se_ha_marcado = true;
						
                     }

                    ball_moving = false; //Se indica que la ball ya no se esta moviendo.
                    position_ball(pong_paddle_computer_top); //Se positiona la ball en la paddle del computer.
                    setTimeout("extract_ball();", 2000); //El computer saca la ball despues de 2 segundos (2000 milisegundos).
                 }
                //..y si ha marcado un gol el user al computer:
                else if (ball_x > area_game_x  /*- 30*/)
                 {
                    last_winner = "user"; //Se setea como last winner al user.
                    clearInterval(ball_movement); //Se detiene la ball.
                    goals_user++; //Se incrementan los goals marcados por el user.
                    pong_points += 100; //Se dan 100 pong_points.
                    se_ha_de_extract = true; //Se setea la variable para saber que se ha de extract de nuevo.

                    //Se anuncia que el user ha marcado un gol, siempre que no se haya de pasar level:
                    if (goals_user < 3 && ball_moving)
                     {
                        document.getElementById("pong_message").innerHTML = 	pong_user_winsmessage;
							//"You<br>has<br>marked";
                        document.getElementById("pong_message").style.visibility = "visible";

                        //Se setea para saber que se ha marcado un gol:
                        se_ha_marcado = true;
                     }

                    ball_moving = false; //Se indica que la ball ya no se esta moviendo.
                    position_ball(pong_paddle_user_top); //Se positiona la ball en la paddle del user.
                 }

                if (se_ha_marcado && lives >= 0)
                 {
                    //Se muestra el scoreboard:
                    if (goals_user > goals_computer) {
					   setTimeout("document.getElementById('pong_message').innerHTML = goals_user + ' - ' + goals_computer + '<br/>"+pong_user_winningmessage+"';", 3000);
					}
                    else if (goals_user < goals_computer) {
					   setTimeout("document.getElementById('pong_message').innerHTML = goals_user + ' - ' + goals_computer + '<br/>"+pong_computer_winningmessage+"';", 3000);
					}
                    else if (goals_user == goals_computer) {
					  setTimeout("document.getElementById('pong_message').innerHTML = goals_user + ' - ' + goals_computer + '<br>"+pong_tie_message+"';", 3000);
			        }
                    setTimeout("document.getElementById('pong_message').style.visibility = 'hidden';", 6000);
                 }

                return true;

             }

            
            //Funcion que actualiza los scoreboards:
            function update_scoreboards()
             {
                //Actualiza la barra de state:
				str="Lives: "+lives+" <br/> Level: "+level+" <br/> Score: "+pong_points;
                if (lives >= 0) {
				  document.getElementById("pong_state").innerHTML = str;
				}
                else {
				  document.getElementById("pong_message").style.visibility = "visible";
				  document.getElementById('pong_message').innerHTML=pong_gameover_message;
				  //document.getElementById("pong_state").innerHTML = str;//"Level: "+level+" | Score: "+pong_points;
				}
                
                //update scoreboards:
                document.getElementById("pong_scoreboard_user").innerHTML = goals_user;
                document.getElementById("pong_scoreboard_computer").innerHTML = goals_computer;
             }


            //Funcion que pasa de level:
            function pasar_level()
             {
               
                //Se setea el scoreboard de goals a cero en ambos equipos:
                goals_user = 0;
                goals_computer = 0;

                //Se incrementa un level:
                level++;
                
                //Se incrementan los reflejos del computer, siempre que no haya llegado a su tope (9):
                if (reflejos_computer < 9) { reflejos_computer++; }
                
                //Se dan 500 pong_points al user:
                pong_points += 500;
                
                //Se incrementa la speed, siempre que esta sea mayor a 10:
                if (speed_ball > 1) { speed_ball -= 1; } //Decrementamos para aumentar speed.
                 
                //Se actualizan los scoreboards:
                update_scoreboards();

                //Se anuncia que se ha pasado de level:
                document.getElementById("pong_message").innerHTML = "Welcome<br>to<br>level "+level;
                document.getElementById("pong_message").style.visibility = "visible";
                setTimeout("document.getElementById('pong_message').style.visibility = 'hidden';", 3000);
             }
