// ***************************************************
// Connect IV
//
//   Modified for Drupal by Glenn Linde
//  Original javascript by Joan Alba Maldonado  (granvino@granvino.com). from 
// http://sourceforge.net/projects/conectayas/
//
// ***************************************************

                //number de players:
                var players = 1;
                                
                //Determina el first player:
                var first_player = "user";
                //Determina al user that le toca pull:
                var player_actual = first_player;
                
                //Determina el color de cada user (0 o 1):
                var color_user = 1;
                var color_computer = (color_user == 0) ? 1 : 0; //El computer utiliza el color contrario.
                //connectiv_colors posibles:
				var connectiv_player_color='#b00';
				var connectiv_computer_color='#00b';
                var connectiv_colors = new Array(connectiv_computer_color,connectiv_player_color);
				var connectiv_player_image="/modules/connectiv/img/red.jpg";				
				var connectiv_computer_image="/modules/connectiv/img/blue.jpg";				
				var connectiv_images = new Array(connectiv_computer_image,connectiv_player_image);

				
                //number de cada player:
                var number_user = 1;
                var number_computer = (number_user == 1) ? 2 : 1;
                
                //width de las pieces (pixels):
                var connectiv_piece_width = 55;
                //height de las pieces (pixels):
                var connectiv_piece_height = 55;
                
                //Color de los hollows:
                var connectiv_hollow_color = "#333";            
				var connectiv_hollow_image= '/modules/connectiv/img/hollow.jpg';    
				var connectiv_background_image='/modules/connectiv/img/drupal.jpg';
                //Espaciado entre un hollow y otro (pixels):
                var connectiv_piece_gap = 0;//parseInt((connectiv_piece_width + connectiv_piece_height) / 10);

                //width del board (hollows para pieces):
                var connectiv_board_width = 7;
                var connectiv_board_width_maximum = 35; //maximum de hollows that puede tener el board en horizontal.
                //height del board (hollows para pieces):
                var connectiv_board_height = 6;
                var connectiv_board_height_maximum = 35; //maximum de hollows that puede tener el board en vertical.
                
				var connectiv_board_width_pixels=connectiv_board_width * (connectiv_piece_width + connectiv_piece_gap) - connectiv_piece_gap;
				var connectiv_board_height_pixels=connectiv_board_height * (connectiv_piece_height + connectiv_piece_gap) - connectiv_piece_gap;
				
				var connectiv_background_transparency=0.7;
				
				var connectiv_loss_message="You lost.";
				var connectiv_win_message="You won.";
				var connectiv_start_message="Play Connect IV";
				
                var connectiv_besttime;
				var connectiv_bestuser;
				
				var connectiv_gtime;
				var connectiv_gintervalid;
				
                //Matriz that watch el board:
                var board = new Array(connectiv_board_width * connectiv_board_height);
                
                //cards necesarias para ganar (al putse seguidas):
                var necessary_cards = 4;
                
                //Determina si las cards caen al pulllas (Conecta-4) o no (Tres en raya):
                var gravity = true;

                //Variables that calcularan la difference entre las coordenadas del mouse y las del div de options:
                var difference_position_horizontal = false;
                var difference_position_vertical = false;
                //Variable para saber si se esta arrastrando en un campo seleccionable, y asi no dejar drag:
                var campo_seleccionable = false;

                //Variable that watch la column donde la ficha se tira:
                var column_fail_initial = 0;
                //Variable that watch la column donde la ficha esta cayendo en la animacion:
                var column_fail = 0;
                //Variable that watch la last cell donde caera la ficha de la animacion:
                var cell_last = 0;
                
                //Variables that watch el intervalo para la animacion de fail:
                var ficha_cayendo = false;

                //Variable para saber si alguien ha tirado ya:
                var se_ha_tirado = false;
                
                var ie = document.all ? true : false;

                //Funcion that muestra la piece diminuta moviendose con el mouse, cuando es el turno del user:
                function drag_piece(e, drag)
                {
                    //Si no le toca pull al user o su piece (o su shade) esta oculta, se sale de la funcion:
                    if (player_actual != "user" /*|| document.getElementById("options").style.visibility == "visible"*/) { return; }
                    
                    //Si se ha parado de drag, sale de la funcion:
                    if (!drag) { return; }
                    //...pero si se ha enviado drag, se arrastra:
                    else
                    {
                        //Variable para saber si estamos en Internet Explorer o no:
                        //var ie = document.all ? true : false;
                        //Si estamos en internet explorer, se recogen las coordenadas del mouse de una forma:
                        if (ie)
                        {
                            position_x_mouse = event.clientX + document.body.scrollLeft;
                            position_y_mouse = event.clientY + document.body.scrollTop;
                        }
                        //...pero en otro navegador, se recogen de otra forma:
                        else
                        {
                            //document.captureEvents(Event.MOUSEMOVE);
                            position_x_mouse = e.pageX;
                            position_y_mouse = e.pageY;
                        } 
                        //Si las coordenadas X o Y del mouse son menores that cero, se ponen a cero:
                        if (position_x_mouse < 0) { position_x_mouse = 0; }
                        if (position_y_mouse < 0) { position_y_mouse = 0; }
                        //Se calculan las nuevas coordenadas del div de las options:
                        var position_left_piece = eval(position_x_mouse - parseInt(connectiv_piece_width / 4));
                        var position_top_piece = eval(position_y_mouse - parseInt(connectiv_piece_height / 4));
                        //Si alguna d las coordenadas fuera menos that cero, se ponen a cero:
                        if (position_left_piece < 0) { position_left_piece = 0; }
                        if (position_top_piece < 0) { position_top_piece = 0; }
                        //Se aplican las coordenadas al div de las options:                        
                        document.getElementById("piece").style.left = position_left_piece + "px";
                        document.getElementById("piece").style.top = position_top_piece + "px";
                        document.getElementById("piece_shade").style.left = parseInt(document.getElementById("piece").style.left) + 4 + "px";
                        document.getElementById("piece_shade").style.top = parseInt(document.getElementById("piece").style.top) + 4 + "px";

                        //Si la piece esta arriba de la zone de game, se oculta la piece:
                        if (parseInt(document.getElementById("piece").style.left) + parseInt(document.getElementById("piece").style.width) / 2 < parseInt(document.getElementById("zone_game").style.left) || parseInt(document.getElementById("piece").style.top) + parseInt(document.getElementById("piece").style.height) / 2 < parseInt(document.getElementById("zone_game").style.top))
                        {
                            //Se oculta la piece y su shade:
                            document.getElementById("piece").style.visibility = "hidden";
                            document.getElementById("piece_shade").style.visibility = "hidden";
                        }
                        //...si no, se muestra (la piece y su shade):
                        else { document.getElementById("piece").style.visibility = "visible"; document.getElementById("piece_shade").style.visibility = "visible"; }
                    }
                }


                //Funcion that arrastra el menu de options:
                function drag_options(e, drag)
                {
                    //Si se ha parado de drag, sale de la funcion:
                    if (!drag) { difference_position_horizontal = false; difference_position_vertical = false; return; }
                    //...pero si se ha enviado drag, se arrastra:
                    else
                    {
                        //Variable para saber si estamos en Internet Explorer o no:
                        //var ie = document.all ? true : false;
                        //Si estamos en internet explorer, se recogen las coordenadas del mouse de una forma:
                        if (ie)
                        {
                            position_x_mouse = event.clientX + document.body.scrollLeft;
                            position_y_mouse = event.clientY + document.body.scrollTop;
                        }
                        //...pero en otro navegador, se recogen de otra forma:
                        else
                        {
                            //document.captureEvents(Event.MOUSEMOVE);
                            position_x_mouse = e.pageX;
                            position_y_mouse = e.pageY;
                        } 
                        //Si las coordenadas X o Y del mouse son menores that cero, se ponen a cero:
                        if (position_x_mouse < 0) { position_x_mouse = 0; }
                        if (position_y_mouse < 0) { position_y_mouse = 0; }

                        //Si se ha enviado drag y no es un campo seleccionable, se arrastra:
                        if (drag_options && !campo_seleccionable)
                        {
                            //Si es la first time that se arrastra despues del click, se calcula la difference initial:
                            if (!difference_position_horizontal || !difference_position_vertical)
                            {
                                //Se calcula la difference that hay horizontalmente entre el mouse y el div de las options:
                                difference_position_horizontal = 0;// eval(position_x_mouse - parseInt(document.getElementById("options").style.left));
                                //Se calcula la difference that hay verticalmente entre el mouse y el div de las options:
                                difference_position_vertical = 0;//eval(position_y_mouse - parseInt(document.getElementById("options").style.top));
                            }
                            //Se calculan las nuevas coordenadas del div de las options:
                            var position_left_menu = position_x_mouse - difference_position_horizontal;
                            var position_top_menu = position_y_mouse - difference_position_vertical;
                            //Si alguna d las coordenadas fuera menos that cero, se ponen a cero:
                            if (position_left_menu < 0) { position_left_menu = 0; }
                            if (position_top_menu < 0) { position_top_menu = 0; }

                        }
                    }
                }
                
				function click_piece(piece)
				{
					if (player_actual == "user" && !ficha_cayendo) {
						put_piece(piece);
					}
				}            
				
                //Funcion that muestra el message de espera:
                function show_message(message)
                {
                    //Se pone el message enviado:
                    document.getElementById("connectiv_show_message").innerHTML = message;
                    //Si el message no esta vacio, se muestra:
                    if (message != "") {
						document.getElementById("connectiv_show_message").style.visibility = "visible";
					}
                    //...pero si no, se oculta:
                    else {
						document.getElementById("connectiv_show_message").style.visibility = "hidden";
					}
                }
                
		

                //Funcion that muestra la piece cayendose de arriba hasta that cothat con una piece abajo, solo cuando hay gravity:
                function animacion_fail(cell)
                {
                    var fila_fail = Math.ceil( (cell + 1 ) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                    column_fail_initial = (cell - ((fila_fail - 1) * connectiv_board_width)); //number de column donde se tira la piece.
                    column_fail = (cell - ((fila_fail - 1) * connectiv_board_width)); //number de column donde se encuentra la piece mientras va cayendo.
                    cell_last = calcular_fail(cell); //last cell donde caera la piece that va cayendo.
                    var hollow;
	   	    if (connectiv_hollow_image&&connectiv_hollow_image!='') 
			{  //hollow=
				//' document.getElementById(eval(column_fail-connectiv_board_width)).style.backgroundImage = "url('+connectiv_hollow_image+')"; '+
				//' document.getElementById(eval(column_fail-connectiv_board_width)).style.backgroundRepeat = "no-repeat";'
				//hollow=' document.getElementById(eval(column_fail-connectiv_board_width)).innerHTML=\'<img src="'+connectiv_hollow_image+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'" hspace="0" vspace="0" onClick="if ( !ficha_cayendo) { alert(); }"/>\';';
			
				if (ie)
					ex='onClick="click_piece(eval('+column_fail+'));"';
				else
					ex='';
				hollow=' document.getElementById(eval(column_fail-connectiv_board_width)).innerHTML=\'<img src="'+connectiv_hollow_image+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'" hspace="0" vspace="0" '+ex+'/>\';';
				//alert(hollow);
			}	
			else
			  hollow=
				' document.getElementById(eval(column_fail-connectiv_board_width)).style.background = connectiv_hollow_color; ';
		    if (connectiv_images&&connectiv_images[0]&&connectiv_images[0]!='')
		    {
		      interval=
				' document.getElementById(eval(column_fail)).innerHTML=\'<img src="'+connectiv_images[eval("color_"+player_actual)]+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'" hspace="0" vspace="0" />\';';
				//' document.getElementById(eval(column_fail)).style.backgroundImage = "url('+connectiv_images[eval("color_"+player_actual)]+')"; '+
				//' document.getElementById(eval(column_fail)).style.backgroundRepeat = "no-repeat";'

		    }
		    else
		      interval='document.getElementById(column_fail).style.background = connectiv_colors[eval("color_"+player_actual)]; ';
                    interval+=
			'if (column_fail > column_fail_initial) {'+
			hollow+
			'}'+
			'column_fail += connectiv_board_width;'+
			'if (column_fail >= cell_last) {'+
			' clearInterval(ficha_cayendo);'+
			' ficha_cayendo = false;'+
			hollow+
			' put_piece_2('+cell+');'+
			//' hollow=true;'+
			'}';


                    ficha_cayendo = setInterval(interval,25);
                }

                
                

                //Funcion that crea el board (vacio):
                function create_board()
                {

   
                    //connectiv_piece_gap = parseInt((connectiv_piece_width + connectiv_piece_height) / 10);

                    //Vacia la matriz:
                    board = new Array(connectiv_board_width * connectiv_board_height);
                    //Variables necesarias:
                    var number_column = 0; //Contador de column.
                    var number_fila = 0; //Contador de fila.
                    var hollow_left = 0; //positionador horizontal.
                    var hollow_top = 0; //positionador vertical.
                    var code_html = ""; //code del HTML that contendra los div.
                    var code_html_invisible = ""; //code del HTML that contendra los div invisibles.
					var style_width=connectiv_board_width_pixels+'px';//connectiv_board_width * (connectiv_piece_width + connectiv_piece_gap) - connectiv_piece_gap + "px";
					var style_height=connectiv_board_height_pixels+'px';//connectiv_board_height * (connectiv_piece_height + connectiv_piece_gap) - connectiv_piece_gap + "px";

                    //Se hace un bucle:
                    for (x=0; x<board.length; x++)
                    {
                        //Se borra la matriz:
                        board[x] = 0;
                        //Se calcula la position horizontal:
                        hollow_left =  number_column * (connectiv_piece_width + connectiv_piece_gap);
                        //Se calcula la position vertical:
                        hollow_top = number_fila * (connectiv_piece_height + connectiv_piece_gap);
                        //Se almacena el code:
                        code_html += '<div id="'+x+'" style="left:'+hollow_left+'px; top:'+hollow_top+'px; width:'+connectiv_piece_width+'px; height:'+connectiv_piece_height+'px;  diplay:block; position:absolute;';
						if (connectiv_hollow_image&&connectiv_hollow_image!='')
						{
							//code_html+='background-image : url(\''+connectiv_hollow_image+'\'); ';
							//code_html+='background-repeat: no-repeat;';
						}
						else
							code_html+='background:'+connectiv_hollow_color+';';
						code_html+=' color:#ffffff; font-size:10px; font-family:arial; filter:alpha(opacity='+connectiv_background_transparency*100+'); opacity:'+connectiv_background_transparency+'; -moz-opacity:'+connectiv_background_transparency+'; -khtml-opacity:'+connectiv_background_transparency+'; z-index:3;">';
						if (connectiv_hollow_image&&connectiv_hollow_image!='')
						{
							//code_html+='<img src="'+connectiv_hollow_image+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'" hspace="0" vspace="0" style="color:#ffffff; font-size:10px; font-family:arial; filter:alpha(opacity='+connectiv_background_transparency*100+'); opacity:'+connectiv_background_transparency+'; -moz-opacity:'+connectiv_background_transparency+'; -khtml-opacity:'+connectiv_background_transparency+'; z-index:3;"/>';
							if (ie)
								ex='onClick="click_piece(eval('+x+'));"';
							else
								ex='';							
							code_html+='<img src="'+connectiv_hollow_image+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'"  hspace="0" vspace="0" '+ex+'/>';
							//code_html+='<img src="'+connectiv_hollow_image+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'"  hspace="0" vspace="0" onClick="if (player_actual == \'user\' && !ficha_cayendo) { put_piece('+x+'); }"/>';
						}
						
						code_html+='</div>';
						if (!ie)
								ex='onClick="click_piece(eval('+x+'));"';
						else
								ex='';						
                        code_html_invisible += '<div id="'+x+'_invisible" style="left:'+hollow_left+'px; top:'+hollow_top+'px; width:'+connectiv_piece_width+'px; height:'+connectiv_piece_height+'px; display:block; position:absolute; background:transparent; cursor:crosshair; z-index:11;" '+ex+'></div>';
                        //Se incrementa una unidad el contador de columns:
                        number_column++;
                        //Si el contador de columns ha llegado al final, se restaura y se incrementa una fila:
                        if (number_column >= connectiv_board_width) { number_column = 0; number_fila++; }
                    }

                   // alert(code_html);
                    //Se introduce el fondo en el div:
					if (connectiv_background_image&&connectiv_background_image!='')
                      document.getElementById("fondo").innerHTML = '<img src="'+connectiv_background_image+'" width="'+parseInt(style_width)+'" height="'+parseInt(style_height)+'" hspace="0" vspace="0">';
                    //Se vuelva el code HTML en la pantalla (en el div de la zone de game):
                    document.getElementById("zone_game").innerHTML = code_html;
                    document.getElementById("zone_game_invisible").innerHTML = code_html_invisible;
					
                    //Se setean las options de la piece:

                    //Se ajusta la position vertical del div con la informacion del autor:
                   // document.getElementById("informacion").style.top = parseInt(document.getElementById("zone_game").style.top) + parseInt(document.getElementById("zone_game").style.height) + (connectiv_piece_gap * 2) + "px";
                }

                
                //Funcion that inicia el game por first time:
                function initiate_game_first_time()
                {
                    //Muestra el message de cargando:
					if (connectiv_start_message&&connectiv_start_message!='')
					{
                      show_message(connectiv_start_message);
					}
                    //Se hace visible el area de game:
                    document.getElementById("zone_game").style.visibility = "visible";

                    //Despues de unos milisegundos, inicia el game y luego deja de show el message:
                    setTimeout('initiate_game(); /*show_message("");*/', 10);
                }


                //Funcion that inicia el game:
                function initiate_game()
                {
                    //Muestra el message de cargando:
                    if (connectiv_start_message&&connectiv_start_message!='')
					{
						show_message(connectiv_start_message);//Loading...");
					}
                    connectiv_gtime=0;
					clearInterval(connectiv_gintervalid);
              
                    //Se setea como player actual el first player:
                    player_actual = first_player;
                    //if (players == 1) { player_actual = "user"; }
                    //Despues de unso milisegundos, crea el board, deja de show el message y pasa el turno al player actual:
                    setTimeout('create_board(); /*show_message("");*/ pull();', 10);

                }

				function connectiv_tickTime()
				{
					//show_message(connectiv_gtime);
					document.getElementById("connectiv_time").innerHTML='Time: '+connectiv_gtime+' secs';
					connectiv_gtime++;
				}
                

                //Funcion that elige el mejor hollow donde se podria pull la piece:
                function choose_mejor_hollow()
                {
                    //Si el player actual es el user, retorna -1:
                    if (player_actual == "user") { return -1; }
                    
                    //Variable that contendra el mejor hollow that pueda usarse:
                    var mejor_hollow = -1;

                    //El computer busca si hay algun hollow donde pueda ganar con una sola ficha mas:
                    mejor_hollow = buscar_mejor_hollow("computer", necessary_cards, false);
                    //...pero si no es posible ganar con una sola ficha, impide that el user gane:
                    if (mejor_hollow < 0) { mejor_hollow = buscar_mejor_hollow("user", necessary_cards, true); }
                    //...pero si el user no va a ganar de momento, impide that haga necessary_cards-1:
                    if (mejor_hollow < 0 && necessary_cards-1 > 1) { if (connectiv_board_width == 3 && connectiv_board_height == 3 && !gravity && necessary_cards == 3 && board[4] == 0 && parseInt(Math.random() * 2) == 1) { return 4; } mejor_hollow = buscar_mejor_hollow("user", necessary_cards-1, true); }
                    //...pero si el user no va a hacer necessary_cards-1 de momento, impide that haga necessary_cards-2:
                    if (!gravity && mejor_hollow < 0 && necessary_cards-2 > 1) { mejor_hollow = buscar_mejor_hollow("user", necessary_cards-2, true); }
                    //...pero si el user no va a hacer necessary_cards-2 de momento, busca poder juntar una ficha con otra:
                    if (mejor_hollow < 0 && necessary_cards-1 > 1) { for (cards_bucle=necessary_cards-1; cards_bucle>1; cards_bucle--) { mejor_hollow = buscar_mejor_hollow("computer", cards_bucle, true); if (mejor_hollow >= 0) { break; } } }
                    //...pero si no se puede encajar ninguna, se intenta tapar una al user:
                    if (mejor_hollow < 0 && necessary_cards-2 > 1) { for (cards_bucle=necessary_cards-2; cards_bucle>1; cards_bucle--) { mejor_hollow = buscar_mejor_hollow("user", cards_bucle, true); if (mejor_hollow >= 0) { break; } } }
                    //...si no hay ninguna ficha con la that poder juntar otra, se calcula un number aleatorio:
                    if (mejor_hollow < 0)
                    {
                        //Si esta en el tres en raya, y aun nadie ha tirado en medio lo hace:
                        if (connectiv_board_width == 3 && connectiv_board_height == 3 && !gravity && necessary_cards == 3 && board[4] == 0) { return 4; }
                        else { while (board[mejor_hollow] != 0) { mejor_hollow = parseInt(Math.random() * (board.length)); } }
                    }

                    //Se retorna el mejor hollow posible:
                    return mejor_hollow;
                }
                
                
                //Funcion that busca el mejor hollow, dependiendo de quien sea el player:
                function buscar_mejor_hollow(player, cards_buscadas, recursivear)
                {
                    //Variable that contendra el hollow buscado:
                    var hollow_buscado = hollow_buscado_horizontal = hollow_buscado_vertical = hollow_buscado_diagonal_izquierda = hollow_buscado_diagonal_derecha = -1;
                    
                    //Recoge el number del player enviado:
                    var number_player = (player == "user") ? number_user : number_computer;
                   
                    var bucle_tope = 0;
                    if (gravity) { bucle_tope = connectiv_board_width; }
                    else { bucle_tope = board.length; }
                    
                    if (bucle_tope)
                    {
                        var cell_al_caer = -1;
                        //Hacer un bucle para verify todas las columns:
                        for (cell_contador=0; cell_contador < bucle_tope; cell_contador++)
                        {
                            //Si hay gravity, calcular donde caeria la ficha al pullla por la column actual:
                            if (gravity) { cell_al_caer = calcular_fail(cell_contador); var fila = Math.ceil( (cell_al_caer + 1 ) / connectiv_board_width); var column = cell_contador; }
                            //...si no, dejarla igual that el contador:
                            else { cell_al_caer = cell_contador; var fila = Math.ceil( (cell_al_caer + 1 ) / connectiv_board_width); var column = (cell_al_caer - ((fila - 1) * connectiv_board_width)); }

                            //Si la column esta llena, se sale esta column y continua el bucle con la siguiente:
                            if (gravity && cell_al_caer < 0) { continue; }
                            //Si el hollow ya esta ocupado, continue el bucle con el siguiente:
                            if (!gravity && board[cell_al_caer] != 0) { continue; }

                            //Crea una copia del board actual pero inserta la ficha ficticia:
                            var board_ficticio = new Array(connectiv_board_width*connectiv_board_height);
                            board_ficticio = board.slice(0, board.length);
                            board_ficticio[cell_al_caer] = number_player;

                            //Comprueba si al put la ficha ficticia da la oportunidad de ganar al user, y si es asi la descarta:
                            if (recursivear)
                            {
                                //watch el contador del bucle actual:
                                var contador_backup = cell_contador;
                                //Si actualmente el user no va a hacer linea:
                                if (buscar_mejor_hollow("user", necessary_cards, false) < 0)
                                {
                                    //Restaura el contador del bucle atual:
                                    cell_contador = contador_backup;
                                    //Pone la ficha del user:
                                    board[cell_al_caer] = 2;
                                    //Si despues de put la ficha ficticia puede hacer linea el user:
                                    if (buscar_mejor_hollow("user", necessary_cards, false) >= 0)
                                    {
                                        //Quita la ficha ficticia:
                                        board[cell_al_caer] = 0;
                                        //Restaura el contador del bucle actual:
                                        cell_contador = contador_backup;
                                        //Si la cell es la last (de la column si hay gravity o del board si no la hay), retorna -1:
                                        if (gravity && cell_al_caer+1 >= connectiv_board_width || !gravity && cell_al_caer+1 >= board.length) { return -1; }
                                        //...y si no, continua el bucle:
                                        else { continue; }
                                    }
                                    //...y si no, restaura el contador del bucle actual y borra la ficha ficticia:
                                    else { cell_contador = contador_backup; board[cell_al_caer] = 0; } //Quita la ficha ficticia.
                                }
                            }

                            var first_cell_fila = (fila - 1) * connectiv_board_width; //number de la first cell that esta en la misma column that la cell elegida.
                            var first_cell_column = column - 1; //number de la first cell that esta en la misma fila that la cell elegida.
                            var first_cell_diagonal_izquierda_superior = cell_al_caer - (column - 1) - ((column - 1) * connectiv_board_width) - connectiv_board_width - 1;
                            if (first_cell_diagonal_izquierda_superior < 0) { first_cell_diagonal_izquierda_superior = Math.abs(first_cell_diagonal_izquierda_superior / connectiv_board_width); }
                            var first_cell_diagonal_derecha_superior = cell_al_caer + (connectiv_board_width - column) - ((connectiv_board_width - column) * connectiv_board_width) + connectiv_board_width - 1;
                            if (first_cell_diagonal_derecha_superior < 0) { first_cell_diagonal_derecha_superior = Math.ceil(connectiv_board_width - 1 - Math.abs(first_cell_diagonal_derecha_superior / connectiv_board_width)) - 1; }
                            var cards_seguidas = 0; //Contador de cards seguidas, para saber si ha hecho linea.


                            //Comprueba si se ha hecho linea en diagonal izquierda:
                            var x_fila = Math.ceil( (first_cell_diagonal_izquierda_superior + 1) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                            var x_column = (first_cell_diagonal_izquierda_superior - ((x_fila - 1) * connectiv_board_width)) + 1; //number de column donde se encuentra la cell elegida.
                            for (x = first_cell_diagonal_izquierda_superior; x <= connectiv_board_width*connectiv_board_height; x+=connectiv_board_width+1)
                            {
                                
                                if (board_ficticio[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                                else { cards_seguidas = 0; } //...pero si no, se pone a cero.
                                //Si ya se ha hecho linea:
                                if (cards_seguidas >= cards_buscadas)
                                {
                                    hollow_buscado_diagonal_izquierda = cell_al_caer;
                                    //Si es imposible hacer linea aunthat se tape en cell_al_caer, se salta este loop:
                                    if (fila-necessary_cards+cards_seguidas-1 < 0 || column+necessary_cards-cards_seguidas+1 > connectiv_board_width || column-necessary_cards+cards_seguidas < 0) { hollow_buscado_diagonal_izquierda = -1; cards_seguidas = 0; }
                                }
                                //Si se ha alcanzado el final de la column o el final de las filas, sale del bucle:
                                if (x_column >= connectiv_board_width || x_fila >= connectiv_board_height) { break; }
                                //Se incrementan los contadores de fila y de column:
                                x_fila++;
                                x_column++;
                            }
                            //Si se ha encontrado un buen hollow, sale del bucle:
                            if (hollow_buscado_diagonal_izquierda > 0) { break; }

                            //Comprueba si se ha hecho linea en diagonal derecha:
                            cards_seguidas = 0;
                            var x_fila = Math.ceil( (first_cell_diagonal_derecha_superior + 1 ) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                            var x_column = (first_cell_diagonal_derecha_superior - ((x_fila - 1) * connectiv_board_width)) + 1; //number de column donde se encuentra la cell elegida.
                            for (x = first_cell_diagonal_derecha_superior; x <= connectiv_board_width*connectiv_board_height; x+=connectiv_board_width-1)
                            {
                                if (board_ficticio[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                                else { cards_seguidas = 0; } //...pero si no, se pone a cero.
                                //Si ya se ha hecho linea:
                                if (cards_seguidas >= cards_buscadas)
                                {
                                    hollow_buscado_diagonal_derecha = cell_al_caer;
                                    //Si es imposible hacer linea aunthat se tape en cell_al_caer, se salta este loop:
                                    if (fila-necessary_cards+cards_seguidas-1 < 0 || column+necessary_cards-cards_seguidas+1 > connectiv_board_width || column-necessary_cards+cards_seguidas < 0) { hollow_buscado_diagonal_derecha = -1; cards_seguidas = 0; }
                                }
                                //Si se ha alcanzado el principio de la column o el final de las filas, sale del bucle:
                                if (x_column <= 1 || x_fila >= connectiv_board_height) { break; }
                                //Se incrementan los contadores de fila y de column:
                                x_fila++;
                                x_column--;
                            }
                            //Si se ha encontrado un buen hollow, sale del bucle:
                            if (hollow_buscado_diagonal_derecha > 0) { break; }
                            
                            //Comprueba si se ha hecho linea en horizontal:
                            cards_seguidas = 0;
                            for (x = first_cell_fila; x <= first_cell_fila+connectiv_board_width-1; x++)
                            {
                                if (board_ficticio[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                                else { cards_seguidas = 0; } //...pero si no, se pone a cero.
                                //Si ya se ha hecho linea:
                                if (cards_seguidas >= cards_buscadas)
                                {
                                    hollow_buscado_horizontal = cell_al_caer;
                                    //Si es imposible hacer linea aunthat se tape en cell_al_caer, se salta este loop:
                                    if (column+necessary_cards-cards_seguidas+1 > connectiv_board_width || column-necessary_cards+cards_seguidas < 0) { hollow_buscado_horizontal = -1; cards_seguidas = 0; }
                                }
                            }
                            //Si se ha encontrado un buen hollow, sale del bucle:
                            if (hollow_buscado_horizontal > 0 && gravity) { break; }

                            //Comprueba si se ha hecho linea en vertical:
                            cards_seguidas = 0;
                            if (gravity) { var principio_bucle = cell_al_caer; var final_bucle = cell_al_caer+(connectiv_board_width*cards_buscadas); }
                            else { var principio_bucle = column; var final_bucle = board.length - connectiv_board_width + column; }
                            for (x = principio_bucle; x <= final_bucle; x += connectiv_board_width)
                            {
                                if (board_ficticio[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                                else { cards_seguidas = 0; } //...pero si no, se pone a cero.
                                if (cards_seguidas >= cards_buscadas)
                                {
                                    hollow_buscado_vertical = cell_al_caer;
                                    //Si es imposible hacer linea aunthat se tape en cell_al_caer, se salta este loop:
                                    if (fila-necessary_cards+cards_seguidas-1 < 0) { hollow_buscado_vertical = -1; cards_seguidas = 0; }
                                }
                            }
                            if (hollow_buscado_vertical > 0) { break; }
                            //Si se ha encontrado algun hollow, sale del bucle:
                        }
                    }

                    //Si no se ha encontrado el criterio buscado, sale de la funcion retornando -1:
                    if (hollow_buscado_horizontal < 0 && hollow_buscado_vertical < 0 && hollow_buscado_diagonal_izquierda < 0 && hollow_buscado_diagonal_derecha < 0) { return -1; }
                    
                    //Calcula aleatoriamente that hollow usar de los buscados Y ENCONTRADOS (si usar el horizontal, vertical o diagonal izquierda o diagonal derecha):
                    var hollows = new Array(hollow_buscado_horizontal, hollow_buscado_vertical, hollow_buscado_diagonal_izquierda, hollow_buscado_diagonal_derecha);
                    while (hollow_buscado < 0) { hollow_buscado = hollows[parseInt(Math.random() * (hollows.length))]; }
                    
                    //Retorna el mejor hollow:
                    return hollow_buscado;
                }


                //Funcion that calcula donde caeria la piece al pullla por una column, habiendo gravity:
                function calcular_fail(cell)
                {
                    //Calcula en that column esta la cell enviada:
                    var fila = Math.ceil( (cell + 1 ) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                    var column = (cell - ((fila - 1) * connectiv_board_width)) + 1; //number de column donde se encuentra la cell elegida.
                    //Busca la last ficha puesta en la column:
                    for (x=column-1; x<=connectiv_board_width*connectiv_board_height-1; x+=connectiv_board_width)
                    {
                        //Si ha encontrado una piece en la column, sale del bucle:
                        if (board[x] != 0) { break; }
                    }
                    //Si la position anterior a la last ficha puesta no existe, es that la column esta llena y returna -1:
                    if (x-connectiv_board_width < 0) { return -1; }
                    //...pero si no, retorna la position anterior:
                    else { return x-connectiv_board_width; }
                }


                //Funcion that hace pull la piece al computer:
                function pull()
                {
                    //Si el player actual es el computer y no hay mas de un player:
                    if (player_actual == "computer" && players == 1)
                    {
                        //Muestra el message de procesando:
                        //show_message("Processing...");
                        
                        //Oculta la piece del player:
                        document.getElementById("piece").style.visibility = "hidden";
                        document.getElementById("piece_shade").style.visibility = "hidden";
                        
                        //Oculta las casillas invisibles:
                        document.getElementById("zone_game_invisible").style.visibility = "hidden";
                    }
                    //...pero si es el user:
                    else //if (document.getElementById("options").style.visibility != "visible")
                    {
                        //Deja de show el message de procesando:
                       // show_message("");

                        //Muestra la piece del player:
                        document.getElementById("piece").style.visibility = "visible";
                        document.getElementById("piece_shade").style.visibility = "visible";

                        //Muestra las casillas invisibles:
                        document.getElementById("zone_game_invisible").style.visibility = "visible";
                    }

                    //Despues de unos milisegundos, calcula el mejor hollow (si es el turno del user, retornara -1) y dispone para that ponga la piece el player actual:
                    setTimeout("var mejor_hollow = choose_mejor_hollow(); put_piece(mejor_hollow);", 10);
                }                


                //Funcion that pone la piece en el board (paso 1, antes de la animacion):
                function put_piece(cell)
                {
				    //Si el player actual es el user y no se ha enviado donde put la piece, sale de la funcion:
                    if (player_actual == "user" && cell < 0) { return; }
                    //Si el hollow elegido no esta libre, sale de la funcion:
                    if (board[cell] != 0) { return; }
                    
                    //Se setea conforme ya se ha tirado:
                    se_ha_tirado = true;
					if (connectiv_gtime<=0)
					{
						connectiv_gtime=1;
						connectiv_tickTime();
						connectiv_gintervalid = setInterval("connectiv_tickTime()",1000);
					}
					show_message("");
                    
                    //Calcula donde se ha elegido put la piece (tiene en cuenta la gravity, si la hay):
                    if (gravity)
                    {
                        //Se calcula donde va a caer la piece:
                        cell = calcular_fail(cell);
                        
                        //Se hace caer la piece hasta el hollow elegido:
                        animacion_fail(cell);
                    } else {
						put_piece_2(cell);
					} //Si no hay gravity, continua con el game.
                }


                //Funcion that pone la piece en el board (paso 2, despues de la animacion):
                function put_piece_2(cell)
                {
                    //Pone la piece en la matriz:
                    board[cell] = (player_actual == "user") ? number_user : number_computer;
                    
                    //Pone la piece visualmente:
					if (connectiv_images&&connectiv_images[0]&&connectiv_images[0]!='')
					{
						//document.getElementById(cell).style.backgroundImage = 'url('+connectiv_images[eval("color_"+player_actual)]+')'; 
						//document.getElementById(cell).style.backgroundRepeat = "no-repeat";
						//document.getElementById(cell).innerHTML='<img src="'+connectiv_images[eval("color_"+player_actual)]+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'" hspace="0" vspace="0" style="color:#ffffff; font-size:10px; font-family:arial; filter:alpha(opacity='+connectiv_background_transparency*100+'); opacity:'+connectiv_background_transparency+'; -moz-opacity:'+connectiv_background_transparency+'; -khtml-opacity:'+connectiv_background_transparency+'; z-index:3;"/>';
						document.getElementById(cell).innerHTML='<img src="'+connectiv_images[eval("color_"+player_actual)]+'" width="'+parseInt(connectiv_piece_width)+'" height="'+parseInt(connectiv_piece_height)+'" hspace="0" vspace="0" />';
					}
					else
	                    document.getElementById(cell).style.background = connectiv_colors[eval("color_"+player_actual)];
                    
                    //Quita el evento onclick en la cell invisible:
                    document.getElementById(cell+"_invisible").onclick = function() { };

                    //Quita el cursor en forma de cruz a la cell invisible:
                    document.getElementById(cell+"_invisible").style.cursor = "default";
                   
                    //Comprueba si alguien ha ganado:
                    partida_acabada = verify_winner(cell);
                    
                    //Si la partida se ha acabado, sale de la funcion:
                    if (partida_acabada) { return; }

                    //Pasa el turno al adversario:
                    if (players == 1) {
						player_actual = (player_actual == "user") ? "computer" : "user";
					}
                    else {
						player_actual = "user"; var color_user_backup = connectiv_colors[color_user];
						connectiv_colors[color_user] = connectiv_colors[color_computer];
						connectiv_colors[color_computer] = color_user_backup;
						document.getElementById("piece").style.background = connectiv_colors[color_user];
						number_user = (number_user == 1) ? 2 : 1;
					}

                    //Continua el game, tirando el otro:
                    pull();
                }
                

                //Funcion that comprueba si alguien ha ganado:
                function verify_winner(cell)
                {
                    //Variable that watch si hay un winner:
                    var winner = "";
                    
                    //Matriz that calcula las cards that hacen linea:
                    var board_lineas = new Array(connectiv_board_width*connectiv_board_height);
                    for (x_board_lineas = 0; x_board_lineas < board_lineas.length; x_board_lineas++) { board_lineas[x_board_lineas] = 0; }
                    
                    //Comprueba si la nueva ficha puesta en el hollow ha formado linea en vertical, horizontal o diagonal:
                    var number_player = (player_actual == "user") ? number_user : number_computer; //Recoge el number del player actual.
                    var fila = Math.ceil( (cell + 1 ) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                    var column = (cell - ((fila - 1) * connectiv_board_width)) + 1; //number de column donde se encuentra la cell elegida.
                    var first_cell_fila = (fila - 1) * connectiv_board_width; //number de la first cell that esta en la misma column that la cell elegida.
                    var first_cell_column = column - 1; //number de la first cell that esta en la misma fila that la cell elegida.
                    var first_cell_diagonal_izquierda_superior = cell - (column - 1) - ((column - 1) * connectiv_board_width);
                    if (first_cell_diagonal_izquierda_superior < 0) { first_cell_diagonal_izquierda_superior = Math.abs(first_cell_diagonal_izquierda_superior / connectiv_board_width); }
                    var first_cell_diagonal_derecha_superior = cell + (connectiv_board_width - column) - ((connectiv_board_width - column) * connectiv_board_width);
                    if (first_cell_diagonal_derecha_superior < 0) { first_cell_diagonal_derecha_superior = Math.ceil(connectiv_board_width - 1 - Math.abs(first_cell_diagonal_derecha_superior / connectiv_board_width)) - 1; }
                    var cards_seguidas = 0; //Contador de cards seguidas, para saber si ha hecho linea.
                    
                    //Comprueba si se ha hecho linea en horizontal:
                    var comienzo_linea = first_cell_fila;
                    for (x = first_cell_fila; x <= first_cell_fila+connectiv_board_width-1; x++)
                    {
                        if (board[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                        else { cards_seguidas = 0; comienzo_linea = x; } //...pero si no, se pone a cero.
                        if (cards_seguidas >= necessary_cards)
                        {
                            var comienzo_bucle = (comienzo_linea == first_cell_fila) ? comienzo_linea : comienzo_linea+1;
                            var salir_del_bucle = false;
                            for (contador_comienzo_linea = comienzo_bucle; contador_comienzo_linea <= first_cell_fila+connectiv_board_width-1; contador_comienzo_linea++) { if (board[contador_comienzo_linea] == number_player) { board_lineas[contador_comienzo_linea] = 1; salir_del_bucle = true; } else if (salir_del_bucle) { break; } }
                            winner = player_actual;
                            break;
                        } //Si ya se ha hecho linea, sale del bucle y se define al player actual como winner.
                    }

                    //Comprueba si se ha hecho linea en vertical:
                    comienzo_linea = first_cell_column;
                    cards_seguidas = 0;
                    for (x = first_cell_column; x <= connectiv_board_width*connectiv_board_height-1; x+=connectiv_board_width)
                    {
                        if (board[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                        else { cards_seguidas = 0; comienzo_linea = x; } //...pero si no, se pone a cero.
                        if (cards_seguidas >= necessary_cards)
                        {
                            var comienzo_bucle = (comienzo_linea == first_cell_column) ? comienzo_linea : comienzo_linea+connectiv_board_width;
                            salir_del_bucle = false;
                            for (contador_comienzo_linea = comienzo_bucle; contador_comienzo_linea <= connectiv_board_width*connectiv_board_height-connectiv_board_width+column-1; contador_comienzo_linea+=connectiv_board_width) { if (board[contador_comienzo_linea] == number_player) { board_lineas[contador_comienzo_linea] = 1; salir_del_bucle = true; } else if (salir_del_bucle) { break; } }
                            winner = player_actual;
                            break;
                        } //Si ya se ha hecho linea, sale del bucle y se define al player actual como winner.
                    }

                    //Comprueba si se ha hecho linea en diagonal izquierda:
                    comienzo_linea = first_cell_diagonal_izquierda_superior;
                    cards_seguidas = 0;
                    var x_fila = Math.ceil( (first_cell_diagonal_izquierda_superior + 1) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                    var x_column = (first_cell_diagonal_izquierda_superior - ((x_fila - 1) * connectiv_board_width)) + 1; //number de column donde se encuentra la cell elegida.
                    for (x = first_cell_diagonal_izquierda_superior; x <= connectiv_board_width*connectiv_board_height; x+=connectiv_board_width+1)
                    {
                        if (board[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                        else { cards_seguidas = 0; comienzo_linea = x; } //...pero si no, se pone a cero.
                        if (cards_seguidas >= necessary_cards)
                        {
                            for (j=x; j<=connectiv_board_width*connectiv_board_height; j+=connectiv_board_width+1) { if (x_column >= connectiv_board_width || x_fila >= connectiv_board_height) { break; } } //Se calcula la last cell de la diagonal.
                            var comienzo_bucle = (comienzo_linea == first_cell_diagonal_izquierda_superior) ? comienzo_linea : comienzo_linea+connectiv_board_width+1;
                            salir_del_bucle = false;
                            for (contador_comienzo_linea = comienzo_bucle; contador_comienzo_linea <= j; contador_comienzo_linea+=connectiv_board_width+1) { if (board[contador_comienzo_linea] == number_player) { board_lineas[contador_comienzo_linea] = 1; salir_del_bucle = true; } else if (salir_del_bucle) { break; } }
                            winner = player_actual;
                            break;
                        } //Si ya se ha hecho linea, sale del bucle y se define al player actual como winner.
                        //Si se ha alcanzado el final de la column o el final de las filas, sale del bucle:
                        if (x_column >= connectiv_board_width || x_fila >= connectiv_board_height) { break; }
                        //Se incrementan los contadores de fila y de column:
                        x_fila++;
                        x_column++;
                    }

                    //Comprueba si se ha hecho linea en diagonal derecha:
                    comienzo_linea = first_cell_diagonal_derecha_superior;
                    cards_seguidas = 0;
                    var x_fila = Math.ceil( (first_cell_diagonal_derecha_superior + 1) / connectiv_board_width); //number de fila donde se encuentra la cell elegida.
                    var x_column = (first_cell_diagonal_derecha_superior - ((x_fila - 1) * connectiv_board_width)) + 1; //number de column donde se encuentra la cell elegida.
                    for (x = first_cell_diagonal_derecha_superior; x <= connectiv_board_width*connectiv_board_height; x+=connectiv_board_width-1)
                    {
                        if (board[x] == number_player) { cards_seguidas++; } //Si el hollow actual tiene una ficha del player actual, se cuenta como seguida.
                        else { cards_seguidas = 0; comienzo_linea = x; } //...pero si no, se pone a cero.
                        if (cards_seguidas >= necessary_cards)
                        {
                            for (j=x; j<=connectiv_board_width*connectiv_board_height; j+=connectiv_board_width-1) { if (x_column <= 1 || x_fila >= connectiv_board_height) { break; } } //Se calcula la last cell de la diagonal.
                            var comienzo_bucle = (comienzo_linea == first_cell_diagonal_derecha_superior) ? comienzo_linea : comienzo_linea+connectiv_board_width-1;
                            salir_del_bucle = false;
                            for (contador_comienzo_linea = comienzo_bucle; contador_comienzo_linea <= j; contador_comienzo_linea+=connectiv_board_width-1) { if (board[contador_comienzo_linea] == number_player) { board_lineas[contador_comienzo_linea] = 1; salir_del_bucle = true; } else if (salir_del_bucle) { break; } }
                            winner = player_actual;
                            break;
                        } //Si ya se ha hecho linea, sale del bucle y se define al player actual como winner.
                        //Si se ha alcanzado el principio de la column o el final de las filas, sale del bucle:
                        if (x_column <= 1 || x_fila >= connectiv_board_height) { break; }
                        //Se incrementan los contadores de fila y de column:
                        x_fila++;
                        x_column--;
                    }
                    
                    
                    //Si alguien ha ganado, lo notifica, vuevle a comenzar el game y sale retornando true:
                    if (winner != "")
                    {
                        //Se resaltan las lineas:
                        for (x_resaltar = 0; x_resaltar < board_lineas.length; x_resaltar++)
                        {
                            if (board_lineas[x_resaltar] == 1) { document.getElementById(x_resaltar).style.width = parseInt((parseInt(document.getElementById(x_resaltar).style.width) / 1.1)) + "px"; document.getElementById(x_resaltar).style.height = parseInt((parseInt(document.getElementById(x_resaltar).style.height) / 1.1)) + "px"; }
                        }
                        
                        //Si gana el user, le da la enhorabuena:
                        if (winner == "user")
                        {
                            //Esconde la piece y su shade:
                            document.getElementById("piece").style.visibility = "hidden";
                            document.getElementById("piece_shade").style.visibility = "hidden";
                            //Da la enhorabuena:
                            if (players == 2) {
							  show_message("Congratulations player "+number_player); 
							  //alert("Player "+number_player+" won.");
							}
                            else {
								show_message(connectiv_win_message);
								document.getElementById('connectiv_formmessage').innerHTML = connectiv_win_message;

								if (document.getElementById('connectiv_winform')&&
									(!connectiv_besttime||connectiv_besttime==-1||connectiv_gtime<connectiv_besttime))
								{
									document.getElementById('connectiv_message').value=document.getElementById('connectiv_formmessage').innerHTML;
									document.getElementById('connectiv_ticker').value=connectiv_gtime;
									document.getElementById('connectiv_winform').submit();
								}							  
							  
 							  /*alert("Congratulations! you won.");*/
							}
                            //Deja de show el message:
                           // show_message("");
                        }
                        else {
							show_message(connectiv_loss_message); /alert("You lost."); show_message("");*/
						} //Si gana el computer, se notifica.
						clearInterval(connectiv_gintervalid);
                        if (players == 1)
			{
				first_player = (first_player == "user") ? "computer" : "user";
			} //Alterna el first player. 
                        else {
				player_actual = "user";
				var color_user_backup = connectiv_colors[color_user];
				connectiv_colors[color_user] = connectiv_colors[color_computer];
				connectiv_colors[color_computer] = color_user_backup;
				document.getElementById("piece").style.background = connectiv_colors[color_user];
				number_user = (number_user == 1) ? 2 : 1;
			}
                        //Selecciona la opcion indicada como first player en el formulario:
                        var first_player_selected = (first_player == "user") ? 0 : 1;
                        document.getElementById("first_player").options[first_player_selected].selected = true;
                        initiate_game(); //Inicia el game otra time.
                        return true; //Sale de la funcion retornando true.
                    }
                    //...pero si nadie ha ganado, retorna false:
                    else
                    {
                        //Si el talbero esta todo usado, lo notifica y vuelve a comenzar el game:
                        var hollows_libres = false;
                        for (x=0; x<board.length; x++) { if (board[x] == 0) { hollows_libres = true; } }
                        if (!hollows_libres)
                        {
                            show_message("There is no winner");
                            if (players == 1) {
				first_player = (first_player == "user") ? "computer" : "user";
			    }
                            else {
				player_actual = "user"; 
				var color_user_backup = connectiv_colors[color_user];
				connectiv_colors[color_user] = connectiv_colors[color_computer];
				connectiv_colors[color_computer] = color_user_backup;
				document.getElementById("piece").style.background = connectiv_colors[color_user];
				number_user = (number_user == 1) ? 2 : 1;
			    }
                            var first_player_selected = (first_player == "user") ? 0 : 1;
                            document.getElementById("first_player").options[first_player_selected].selected = true;
                            alert("Game has finished, there are not more free holes");
                            initiate_game();
                            return true;
                        }
                        //...pero si no, retorna false (aun no se ha acabado la partida):
                        else { return false; }
                    }
                }
               