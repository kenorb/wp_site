// ***************************************************
// Breakout
//
//  Modified for Drupal by Glenn Linde Dec 2008
//  Original javascript by Alexis "Fyrd" Deveria on November 12, 2007
//  This software is supplied without warranty under the GNU Public licence agreement.
//
// ***************************************************

/*
To do/fix:
x- Right-most column blocks don't always get hit
x- Paddle size fix on miss / next level
x - Unpause does not work
x - All bonuses should disappear on miss / next level
x- Allow bonuses to be received when ball at paddle
- More bonus blocks
x- Get ball when at bottom-left corner
x- Bonus is being accepted even when too far to the right
*/

var breakout_scale=1.4;
var bDraw=true;
var breakout_launch=true;
var paused=false;
var bkH;//= breakout_scale*8;
var bkW;//= breakout_scale*16;
var xSpd = 4;// overwritten anyway
var ySpd = 4;
var pwidthdef;// = breakout_scale*50;
var pwidth;// = pwidthdef;
//var bwidth=10;
var bIval = 40;
var baseSpd;// = 8*breakout_scale;
var breakout_msg, pd, bmover, bc, breakout_x, breakout_y, breakout_b, breakout_blocks;//, breakout_colors;
var blockCnt = 0;
var breakout_bonuses = [];
var breakout_width;//=breakout_scale*256;
var breakout_height;//=breakout_scale*158;
var breakout_image_path='img/';

var normalBlock;//=breakout_image_path+'normalblock.png';
var toughBlock;//=breakout_image_path+'toughblock.png';
var breakout_start_message='Click New Game to begin';
var breakout_gameover_message='Game Over';
var breakout_miss_message='Missed';
var breakout_background;
var breakout_background_transparency=0.2;
var breakout_lives=3;
var breakout_gamespeed=8;
var breakout_bestscore=-1;
var breakout_paddle_height=5;
var breakout_paddle_width=50;
//[20,10,20],[5,2,0],[35,-2,40]
/*var levelcols = [
	[[15,8,20],[5,2,0],[25,30,30]],
	[[2,2,20],[3,9,5],[10,5,80]],
	[[8,3,20],[0,9,50],[5,2,40]],
	[[7,8,11],[13,1,10],[7,5,80]]
];*/

var breakout_levels = [
/*
[
	[0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0]
],
*/
[
	[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],	
	[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
	[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1],
	[2,2,2,2,2,2,1,1,1,1,2,2,2,2,2,2],
	[2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2]
],[
	[2,2,2,0,0,0,2,2,2,2,0,0,0,2,2,2],
	[1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1],
	[1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1],
	[2,2,2,0,0,0,2,2,2,2,0,0,0,2,2,2],
	[1,1,1,0,0,0,1,1,1,1,0,0,0,1,1,1]
],[
	[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
	[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
	[0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2],
	[0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2],
	[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
	[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],	
	[2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0]
],[
	[0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0],
	[0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0],
	[0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0],
	[0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0],
	[0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0],
	[0,1,1,1,1,0,0,1,1,0,0,1,1,1,1,0],
	[0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0]
],[
	[0,0,2,2,2,2,0,2,0,2,2,2,2,2,2,2],
	[0,0,2,2,2,0,2,0,0,0,2,2,2,2,2,2],
	[2,2,2,2,0,2,0,2,2,0,0,2,2,2,2,2],
	[1,1,1,0,1,0,1,0,1,0,0,0,1,1,1,1],
	[1,1,0,1,0,1,0,1,0,1,0,0,0,1,1,1],
	[1,0,1,0,1,0,1,0,1,0,1,0,0,0,1,1],
	[0,1,0,1,1,1,0,1,0,1,0,1,1,0,0,0]
],[
	[0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,0],
	[0,1,1,0,0,0,0,1,0,1,1,0,0,0,0,1],
	[1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,1],
	[2,2,2,2,0,0,0,2,2,2,2,2,0,0,0,2],
	[2,2,2,2,0,0,0,2,0,2,2,0,0,0,0,2],
	[0,2,2,0,0,0,2,2,0,2,2,2,0,0,2,2],
	[0,0,0,2,2,2,2,0,0,0,0,0,2,2,2,0]
],[
	[2,2,2,0,0,2,2,0,0,0,2,2,0,0,0,0],
	[2,2,0,0,2,2,0,0,0,2,0,2,2,0,0,0],
	[2,0,0,2,2,0,0,0,2,0,0,0,2,2,0,0],
	[0,0,2,2,0,0,0,0,0,2,0,2,2,0,0,0],
	[0,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1],
	[1,1,0,0,0,0,0,1,0,1,0,1,1,1,1,0],
	[1,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0]
],[
	[1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1],
	[1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1],
	[1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1],
	[0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0],
	[1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1],
	[1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1],
	[1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1]
  ],[

	[0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0],
	[0,0,0,0,1,1,1,0,0,1,1,1,0,0,0,0],
	[0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0],
	[1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1],
	[0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0],
	[0,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0],
	[0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0]
],[
	[0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0],
	[1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0],
	[1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0],
	[1,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0],
	[1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1],
	[1,0,0,0,1,0,1,0,0,0,1,1,0,0,0,1],
	[0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,0]
]
];

var isExplorer=navigator.appName.indexOf("Microsoft")!=-1;
	
function breakout_init(){
	
    bkH = breakout_scale*8;
    bkW = breakout_scale*16;
    pwidthdef = breakout_scale*breakout_paddle_width;
    pwidth = pwidthdef;
    baseSpd = breakout_gamespeed*breakout_scale;
    breakout_width=breakout_scale*256;
    breakout_height=breakout_scale*158;
    normalBlock=breakout_image_path+'normalblock.png';
    toughBlock=breakout_image_path+'toughblock.png';

	breakout_msg = document.getElementById('breakout_msg');
	breakout_msg.style.top=50*breakout_scale+'px';
	breakout_msg.style.width=128*breakout_scale+'px';
	breakout_msg.style.left=64*breakout_scale+'px';
	breakout_msg.style.position='absolute';
	p=document.getElementById('breakout_paddle');
	//p.style.top=breakout_height-(breakout_scale*breakout_paddle_height/2);
	//p.style.width=50*breakout_scale+"px";
	p.style.height=breakout_paddle_height*breakout_scale+"px";
	p.style.top=breakout_height+"px";
	p.style.left="0px";
	//p.style.top=(breakout_height-(breakout_scale*breakout_paddle_height/2))+"px";
        //document.getElementById('nude').src=toughBlock;
	breakout_b = document.getElementById('breakout_ball');
	breakout_b.style.width=5*breakout_scale+"px";
	breakout_b.style.height=5*breakout_scale+"px";
	breakout_blocks = CopyArray(breakout_levels[0]);
	//breakout_colors = CopyArray(levelcols[0]);
	

	  if (breakout_background)
	  {
	    document.getElementById('breakout_background_image').src=breakout_background;
	    document.getElementById('breakout_background_image').style.width=breakout_width+"px";
	    document.getElementById('breakout_background_image').style.height=breakout_paddle_height*breakout_scale+breakout_height+"px";
	    document.getElementById('breakout_background_image').style.opacity=breakout_background_transparency;
	    document.getElementById('breakout_background_image').style.MozOpacity=breakout_background_transparency;
	    document.getElementById('breakout_background_image').style.filter='alpha(opacity='+parseInt(100*breakout_background_transparency)+')';//.alpha.opacity=parseInt(100*pong_background_transparency);
	  }
	
	breakout_setup();

}



function ballsetup() {
	xSpd = Math.round(baseSpd/2);
	ySpd = baseSpd-xSpd;
	breakout_bonuses = [];
	document.getElementById('breakout_bonuses').innerHTML = '';
	pwidth = pwidthdef;
	p.style.width = pwidth + 'px';
	//breakout_msg.innerHTML = breakout_start_message;//'Tap to launch ball';
	breakout_b.style.display = 'none';
	breakout_launch = true;

}

function breakout_setup() {

	ballsetup();
	bc=document.getElementById('breakout_blocks');
	//alert(breakout_width);
    bc.style.width=breakout_width+'px';
    bc.style.height=breakout_height+'px';
    bc.style.top='0px';
    bc.style.left='0px';
    if (breakout_background)
	  bc.style.position='absolute';
	else
	  bc.style.position='relative';//absolute';
	
	bc.innerHTML = '';

	var info=document.getElementById('breakout_info');
	info.style.width=breakout_width+'px';
	//info.style.height=breakout_height+'px';
	info.style.top=(breakout_height+5*breakout_scale+13)+"px";//163*breakout_scale+'px';
	
	//var pause = document.getElementById('pause');

    //var about = document.getElementById('about');

	breakout_msg.style.display = 'block';
	//pause.onclick = pauseGame;
	/*about.onclick = function() {
		abt = document.getElementById('aboutcontent');
		if(abt.style.display != 'block') {
			if(!paused) { pauseGame(); };
			abt.style.display = 'block';
		} else {
			abt.style.display = 'none';
		}
		
	}*/

	for(i=0;i<breakout_blocks.length;i++) {
		for(j=0;j<breakout_blocks[i].length;j++) {
			var block = document.createElement('img');
			block.style.top = i*bkH + 'px';
			block.style.left = j*bkW + 'px';
			block.style.width = bkW+'px';
			block.style.height = bkH+'px';
			block.style.position='absolute';
			if(breakout_blocks[i][j] == 0) {
				block.style.visibility = 'hidden';
			} else {
				blockCnt++;
				if(breakout_blocks[i][j] == 1) {
					block.src = normalBlock;
				} else if(breakout_blocks[i][j] == 2) {
					block.src = toughBlock;
				}
			}
			 			
			/*var red = (breakout_colors[0][0]*i)+(breakout_colors[0][1]*j)+(breakout_colors[0][2]);
			var green = (breakout_colors[1][0]*i)+(breakout_colors[1][1]*j)+(breakout_colors[1][2]);
			var blue = (breakout_colors[2][0]*i)+(breakout_colors[2][1]*j)+(breakout_colors[2][2]);
			block.style.backgroundColor = 'rgb(' + red + ',' + green + ',' + blue + ')';*/
			

			bc.appendChild(block);
			
		}
	}
	
	var pdSpd = 10;
	
	window.onkeypress = function(e) {

		var key = e.keyCode;
		if(key==13) {
			if(pdSpd==10) {
				pdSpd = 20;
			} else {
				pdSpd = 10;
			}
			
		} else {
			var pCtr = pwidth/2;
			var pX = p.style.left.split('px')[0]*1;
			if(key==39) {
				//move right
				pX += pdSpd;
				if(pX >= (breakout_width - pwidth)) {
					p.style.left = (breakout_width - pwidth) + 'px';
				} else {
					p.style.left = pX + 'px';
				}
				return false;
			} else if(key=='37') {
				//move left
				pX -= pdSpd;
				
				if (pX <= 0){
					p.style.left = '0px';
				} else {
					p.style.left = pX + 'px';
				}
				return false;
			} 
		}

	}


	document.onmousemove=function(e) {

		var pCtr = pwidth/2;
		if (!isExplorer)
		{
		    id=e.target.id;
			mX=e.layerX;
			mY=e.layerY;
		}
		else
		{
			id=event.srcElement.id;
			mX=event.offsetX + document.body.scrollLeft;
			mY=event.offsetY + document.body.scrollTop;
		}
		//breakout_msg.innerHTML=id;
		if (id=='breakout_blocks'||id=='breakout_background_image')
		{
			if(mX > pCtr && mX < (breakout_width - pCtr)) {
				p.style.left = mX - pCtr + 'px';
			} else if(mX <= (pwidth/2)) {
				p.style.left = '0px';
			} else if(mX >= (breakout_width - pCtr)) {
				p.style.left = (breakout_width - pwidth) + 'px';
			}
		}
		return false;
	}

	bc.onclick=function(e) {
	    breakout_start();
	}
}

function breakout_start()
{
		if(breakout_launch) {
			var pX = p.style.left.split('px')[0]*1;
			ySpd = -1*Math.abs(ySpd);
			xSpd = Math.abs(xSpd);
			breakout_x = pX + (pwidth/2);
			breakout_y = breakout_height;//158;
			breakout_b.style.top = '-50px'; //hide to prevent flicker
			breakout_b.style.display = 'block';
			bmover = setInterval(bmove,bIval);
			breakout_msg.style.display = 'none';
			breakout_launch = false;
		}
}

function bmove() {
	var iH = breakout_height;//*158;
	var iW = breakout_scale*250;
	var missed = false;
	var pX = p.style.left.split('px')[0]*1;
	
	//Move bonuses if found
	var bnbls = document.getElementById('breakout_bonuses').getElementsByTagName('div');
	
	if(bnbls.length > 0) {
		
		for(var i=0;i<bnbls.length;i++) {
			//Get ID
			var id = bnbls[i].id.split('bon')[1];
			breakout_bonuses[id].bny += 5;
		
			if(breakout_bonuses[id].bny > iH) {
				var bnx = breakout_bonuses[id].bnx;
				//see if paddle was hit
				if((bnx+bkW) >= pX && bnx <= (pX + pwidth)) {
					
					//bonus received
					switch(breakout_bonuses[id].type) {
					case 'widerpad':
						var oldwidth = pwidth;
						pwidth += 20;
						p.style.width = pwidth + 'px';
						p.style.left = (pX+(oldwidth-pwidth)/2) + 'px';
						break;
						
					case 'shorterpad':
						var oldwidth = pwidth;
						pwidth = Math.round(pwidth*0.7);
						p.style.width = pwidth + 'px';
						p.style.left = (pX+(oldwidth-pwidth)/2) + 'px';
						break;

					case '100pt':
					  	updateScore(100);
					  	break;

					case '250pt':
						updateScore(250);
					  	break;

					case '-100pt':
						updateScore(-100);
					  	break;

					}
				}
				var bn = bnbls[0];
				//remove bonus
				bnbls[i].parentNode.removeChild(bnbls[i]);
			} else {
				bnbls[i].style.top = breakout_bonuses[id].bny + 'px';
			}
		}
		
	}
	
	if(breakout_y>iH && ySpd>0) {
		//see if paddle is hit
		if(breakout_x >= pX && breakout_x <= pX + pwidth || breakout_x<=0 && pX<=0) {
			atPaddle = false;

			// 32(x-0.5)^2+0.4
			var tmpSpd = 33*Math.pow((((breakout_x-pX)/pwidth)-0.5),2)+0.6;
			
			xSpd = Math.round(Math.abs((tmpSpd*baseSpd)/10));
			ySpd = -1*Math.abs(baseSpd-xSpd);
	        if (ySpd==0)
	 	       ySpd=0.1;  // stop rare case where just bounces horizontally
			
			//see where paddle was hit
			if (breakout_x < pX + pwidth*0.5) {
				xSpd = -1*Math.abs(xSpd);
			} else {
				xSpd = Math.abs(xSpd);
			}

		} else {
			missed = true;
		}
	} else if(breakout_y<iH*0.75 && breakout_y>0) {

		var row = Math.floor(breakout_y/bkH);
		var col = Math.floor(breakout_x/bkW);
		
		if(breakout_blocks.length > row && breakout_blocks[row][col] > 0) {
			var blVal = breakout_blocks[row][col];
			var bnum = breakout_blocks[0].length * row + col;
			
			if(blVal == 1) {
				//normal block
				//Block is hit, remove from array and hide
				breakout_blocks[row][col] = 0;
				bc.getElementsByTagName('img')[bnum].style.visibility = 'hidden';
				updateScore(10);
				blockCnt--;
			} else if(blVal < 3) {
				//number block
				breakout_blocks[row][col] -= 1;
				bc.getElementsByTagName('img')[bnum].src = normalBlock;
			} else {
				//letter block
			}
			
				
			if(blockCnt<=0) {
			// all blocks cleared
				clearInterval(bmover);
				var lvl = document.getElementById('breakout_level');
				var next = lvl.innerHTML*1;
				breakout_msg.style.display = 'block';
				if(next == breakout_levels.length) {
					var sc = document.getElementById('breakout_score');
					breakout_msg.innerHTML = 'Congratulations! You win!<br>Score: ' + sc.innerHTML + '<br><a href="#" onclick="breakout_reset()">Play again<\/a>';
				} else {
					bc.innerHTML = '';
					breakout_blocks = CopyArray(breakout_levels[next]);
					//breakout_colors = CopyArray(levelcols[next]);
					lvl.innerHTML = next+1;
					breakout_msg.innerHTML = 'Level complete!<br><a href="#" onclick="breakout_setup()">Continue...<\/a>';
				}
			}
			

									
			//Find new direction for ball
			if(xSpd == 0) {
				//vertical, so just reverse direction
				ySpd *= -1;
			} else {
				var slope = ySpd/xSpd;	
				
				var bkL = bkW*col;
				var bkR = bkL+bkW;
				var bkT = bkH*row;
				var bkB = bkT+bkH
				
				var bkXside = (xSpd>0)?bkL:bkR;
				var bkYside = (ySpd>0)?bkT:bkB;
				var eqY = (bkXside)*slope + (breakout_y-slope*breakout_x);
				
				if(eqY >= bkT && eqY < bkB-1) {
				//if(eqY >= bkT && eqY <= bkB) {
					//Set correct bounce off position
					breakout_x = 2*bkXside-breakout_x;
					
					//Reverse x
					xSpd *= -1; 

				} else {
					//Set correct bounce off position
					breakout_y = 2*bkYside-breakout_y;
					
					//Reverse y
					ySpd *= -1;
				}
			} 
			
			//See if bonus should fall
			var odds = 7;
			if (Math.floor(Math.random()*odds+1) == 1) {
				//Drop bonus
				var bnbl = document.createElement('div');
				//var goodcol = '#5D5';
				//var badcol = '#D55';
				var /*bg, text,*/type;
				var img;

				//weighting				
				var rnums = [
					1,1,
					2,2,
					3,3,3,
					4,4,
					5
				];
				var rnum = rnums[Math.floor(Math.random()*rnums.length)];

				switch(rnum) {
					case 1:
						type = 'widerpad';
						//bg = goodcol;
						//text = '<>';
						img=breakout_image_path+'widerpad.png';
						break;
					case 2:
						type = 'shorterpad';
						//bg = badcol;
						//text = '><';
						img=breakout_image_path+'shorterpad.png';
						break;
					case 3:
						type = '100pt';
						//bg = goodcol;
						//text = '100';
						img=breakout_image_path+'100pt.png';
						break;
					case 4:
						type = '-100pt';
						//bg = badcol;
						//text = '100';
						img=breakout_image_path+'badcol.png';
						break;
					case 5:
						type = '250pt';
						//bg = goodcol;
						//text = '250';
						img=breakout_image_path+'250pt.png';
						break;
				}

				//bnbl.style.backgroundColor = bg;
				//bnbl.innerHTML = text;
				bnbl.setAttribute('id','bon' + (breakout_bonuses.length));
				bnbl.style.left = breakout_x + 'px';
				bnbl.style.top = breakout_y + 'px';
				bnbl.style.width = bkW + 'px';
				bnbl.style.position='absolute';

				bnbl.innerHTML="<img width='"+bkW+"' height='"+bkH+"' src='"+img+"' />";
				//image=document.createElement('img');
				//image.setAttribute('src','normalblockold.jpg');
				//bnbl.appendChild(image);
				var bn = new bonus(breakout_bonuses.length,breakout_x,breakout_y,type);
				breakout_bonuses.push(bn);

				document.getElementById('breakout_bonuses').appendChild(bnbl);
			}
			
		}
	} else if(breakout_y<=0 && ySpd<0) {
		//at the top
		ySpd*=-1;
	} 
	
	
	if((breakout_x>iW && xSpd>0) || (breakout_x<=0 && xSpd<0)) {
		//at left or right edge
		xSpd*=-1;
	} 		
	//if (ySpd==0)
	//	ySpd=0.1;  // stop rare case where just bounces horizontally
		
	/* Set movement */
	breakout_y+=ySpd;
	breakout_x+=xSpd;
	
	if(bDraw == true) {
		breakout_b.style.top = breakout_y + 'px';
		breakout_b.style.left = breakout_x + 'px';
	//	bDraw = false;
	} else {
		bDraw = true;
	}
	

	
	if(missed) {
		
		var lv = document.getElementById('breakout_lives');
		var lvcount = (lv.innerHTML*1) - 1;
		breakout_msg.style.display = 'block';
		if(lvcount >= 0 ) {
			breakout_msg.innerHTML = breakout_miss_message;//'Missed!';
			lv.innerHTML = lvcount;
			setTimeout(function() {
				missed = false;
				ballsetup();
			},2000);
		} else {
			breakout_msg.innerHTML = breakout_gameover_message;//'Game Over<br><a href="#" onclick="breakout_reset()">Play again<\/a>';
			sc= document.getElementById('breakout_score');
		    score=sc.innerHTML;
			if (document.getElementById('breakout_winform')&&
			   (!breakout_bestscore||breakout_bestscore==-1||score>breakout_bestscore))
			{
				document.getElementById('breakout_win_message').value=breakout_msg.innerHTML;
				document.getElementById('breakout_ticker').value=score;
				document.getElementById('breakout_winform').submit();
			}			
		}
		
		clearInterval(bmover);

	}

}

function breakout_reset() {
	breakout_blocks = CopyArray(breakout_levels[0]);
	//breakout_colors = CopyArray(levelcols[0]);
	bc.innerHTML = '';
	blockCnt = 0;
	document.getElementById('breakout_score').innerHTML = 0;
	document.getElementById('breakout_lives').innerHTML = breakout_lives;
	document.getElementById('breakout_level').innerHTML = 1;
	clearInterval(bmover);
	breakout_setup();
}

/*function pauseGame() {
	var ps = document.getElementById('pause');
	if(!paused) {
			
		ps.style.background = '#555';
		clearInterval(bmover);
		paused = true;
	} else {
		ps.style.background = '#222';
		bmover = setInterval(bmove,bIval);
		paused = false;
	}
}*/

function updateScore(num) {
	var sc = document.getElementById('breakout_score');
	num = (sc.innerHTML*1) + num;
	sc.innerHTML = num<0?0:num;
}

function bonus(id,bnx,bny,type)	{
	this.id=id;
	this.bnx=bnx;
	this.bny=bny;
	this.type=type;
}

function CopyArray(a)
{
  //Create a new array with the old ones length
  var n = new Array(a.length);

  //Just copy all the values from the old to the new
  for(var i = 0; i < a.length; i++)
  {
    //if the value in the old array is an object (array)
    //call the CopyArray function recursively
    if(typeof a[i] == 'object')
      n[i] = CopyArray(a[i]);
    else
      n[i] = a[i];
  }
  
  return n;
}
	
