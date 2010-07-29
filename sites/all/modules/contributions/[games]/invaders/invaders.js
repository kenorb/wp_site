// ***************************************************
// Invaders
//
//  Modified for Drupal by Glenn Linde Nov 2008
//  Original javascript by Tony Goacher 2002 
//  www.cesint.demon.co.uk
//  This software is supplied without warranty under the GNU Public licence agreement.
//
// ***************************************************


var invaders_scale=0.5;	
var iWidth=22*invaders_scale ;
var iHeight=16*invaders_scale ;
var iXSpace=(iWidth+10*invaders_scale) ; // for the space between space invaders
var iYSpace=(iHeight+10*invaders_scale) ;
var invaders_rows=5;
var invaders_columns=9;
//var invaders_angle=0 ;
var isExplorer=0 ;
var useSound=0 ;
var Invaders=new Array() ;
var invaderXJump=10 *invaders_scale;
var invaderYJump=20 *invaders_scale;
var invaderTimer=1 ;
var invaderExpSpeed=1 ;
var invaderAudio=0 ;
var invaderCount ;
var maxX=350*invaders_scale ;
var maxY=350*invaders_scale;
var invadersDirection=1 ;
var changeDir=0 ;
var maxInv;//=(invaders_rows*invaders_columns) ;
var shipDir=0 ;
var shipWidth=26*invaders_scale ;
var shipHeight=16*invaders_scale ;
var ship=0 ;
var shipxshift;//=10*invaders_scale ;
var invaders_your_ship_move_dist=10;
var shipXMouseTime=40*invaders_scale ;
var shipBulletSpeed=20*invaders_scale;
var shipBulletHeight=10*invaders_scale ;		// Height of the ships bullet
var shipBullets=new Array() ;
var maxShipBullets=1;
var insert ;
var invaderYOffset=60 ;
var invaders_score ;
var maxInvaderBullets=20;
var allowableInvaderBullets=3;
var invaders_level=1 ;
var invaderBullets=new Array() ;
var invaderBulletChance;
var invaderBulletHeight=new Array(12*invaders_scale,14*invaders_scale);
var invaderBulletWidth=6*invaders_scale;
var invaderBulletSpeed;//=5*invaders_scale;
var maxInvaderPics=6 ;
var invaderPics;//=new Array(invaders_image_path+"i1a.png",invaders_image_path+"i1b.png",invaders_image_path+"i2a.png",invaders_image_path+"i2b.png",invaders_image_path+"i3a.png",invaders_image_path+"i3b.png") ;
var invaderPicIndexes=new Array(2,3,4,5,4,5,0,1,0,1) ;
var bonusLifeScore=1850 ;
var invaders_landed=0 ;
var maxBarrierLines=6 ;
var linePos=new Array(new RowData(12*invaders_scale,0,6*invaders_scale),new RowData(4*invaders_scale,8*invaders_scale,10*invaders_scale),new RowData(0,16*invaders_scale,4*invaders_scale),new RowData(32*invaders_scale,16*invaders_scale,4*invaders_scale),new RowData(0,24*invaders_scale,4*invaders_scale),new RowData(32*invaders_scale,24*invaders_scale,4*invaders_scale)) ;
var barriers=new Array(0,0,0,0) ;
var barriercolor='#8f8';
var restartLevel=0 ;
var lastScore=0 ;
var delayStart=0 ;
var updateSpeed ;
var spaceshipYpos=30 ;
var bonusSpaceship=0 ;
var spaceshipSpeed=5*invaders_scale ;
var spaceshipHeight=14*invaders_scale ;   // for mystery ship
var spaceshipWidth=32*invaders_scale ;
var leftMost=maxX ;
var rightMost=0 ;
var bottomMost=0;
var invaders_gamespeed=4;
var gameSpeedTimer=0 ;
var lifeLineYpos;//=maxY+shipHeight ;
var invaders_start_message="Click to start.<br/>Mouse to move. Mouse button to shoot";
var invaders_gameover_message="Game Over";
var index ;
var theinterval;
var invaders_keyboard=true;
var invaders_leftkey=90;  // Z
var invaders_rightkey=88; // X
var invaders_firekey=67;  // C
var invaders_width;
var invaders_height;
var invaders_your_ship_width;
var invaders_your_ship_height;
var invaders_your_ship_bullet_speed=20;
var invaders_image_path;
var shipExplosion;
var invaders_lives=3;
var invaders_bestscore=-1;
var invaders_bullet_speed=4;
var invaders_background_transparency=0.2;
var invaders_background;

function InitScale()
{
  iWidth=22*invaders_scale ;
  iHeight=16*invaders_scale ;
  iXSpace=(iWidth+10*invaders_scale) ; // for the space between space invaders
  iYSpace=(iHeight+10*invaders_scale) ;
  invaderXJump=10 *invaders_scale;
  invaderYJump=20 *invaders_scale;
  maxX=invaders_width*invaders_scale ;
  maxY=invaders_height*invaders_scale;
  maxInv=(invaders_rows*invaders_columns) ;
  shipWidth=invaders_your_ship_width*invaders_scale ;
  shipHeight=invaders_your_ship_height*invaders_scale ;
  shipxshift=invaders_your_ship_move_dist*invaders_scale ;
  shipXMouseTime=40*invaders_scale ;
  shipBulletSpeed=invaders_your_ship_bullet_speed*invaders_scale;
  shipBulletHeight=10*invaders_scale ;		// Height of the ships bullet
  invaderBulletHeight=new Array(12*invaders_scale,14*invaders_scale);
  invaderBulletWidth=6*invaders_scale;
  invaderBulletSpeed=invaders_bullet_speed*invaders_scale;
  linePos=new Array(new RowData(12*invaders_scale,0,6*invaders_scale),new RowData(4*invaders_scale,8*invaders_scale,10*invaders_scale),new RowData(0,16*invaders_scale,4*invaders_scale),new RowData(32*invaders_scale,16*invaders_scale,4*invaders_scale),new RowData(0,24*invaders_scale,4*invaders_scale),new RowData(32*invaders_scale,24*invaders_scale,4*invaders_scale)) ;
  spaceshipSpeed=5*invaders_scale ;
  spaceshipHeight=14*invaders_scale ;
  spaceshipWidth=32*invaders_scale ;
  lifeLineYpos=maxY+shipHeight ;
  invaderPics=new Array(invaders_image_path+"i1a.png",invaders_image_path+"i1b.png",invaders_image_path+"i2a.png",invaders_image_path+"i2b.png",invaders_image_path+"i3a.png",invaders_image_path+"i3b.png") ;
  
  
}

isExplorer=navigator.appName.indexOf("Microsoft")!=-1;

/////////////////////////////////////////
// Reset the most's for the barrier destruction checks
/////////////////////////////////////////
function ResetMosts()
{
	leftMost=maxX; 
	rightMost=0 ;
	bottomMost=0 ;
}

///////////////////////////////////////////
// Bonus spaceship class
///////////////////////////////////////////

function Spaceship_Kill()
{
	var el=document.getElementById('spaceship') ;
	el.style.visibility="hidden" ;
	el=document.getElementById('spaceshipText') ;
	el.style.visibility="hidden" ;
	//StopPlay("bship") ;
}

function Spaceship_Move()
{
	var	el=document.getElementById('spaceship') ;
	var   elText=document.getElementById('spaceshipText') ;
	if(this.showBonus)
	{
		el.style.visibility="hidden" ;
		elText.innerHTML="<font face='Arial, Helvetica sans-serif' style='color:white'>"+this.bonus+"</font>"  ;
		elText.style.left=this.xpos+'px';
		elText.style.top=this.ypos+'px' ;
		elText.style.visibility="visible" ;
		this.showBonus-- ;
		if(!this.showBonus)
		{
			this.Kill() ;
			return 1 ;
		}
		return 0;
	}
	
	
	this.xpos+=spaceshipSpeed ;
	el.style.left=this.xpos+'px' ;
	if(this.xpos>maxX-this.width)
	{
		this.Kill() ;
		return 1 ;
	}
	return 0 ;
}

/////////////////////////////////////////////
// Check for collision with ship bullet
// Ship bullet is a single pixel line so only
// need to do a simple check
////////////////////////////////////////////
function Spaceship_CheckCollision(xpos,ypos)
{
	// If alreadyhit, don't bother
	if(this.showBonus)
		return ;
	if(xpos<this.xpos || xpos>(this.xpos+this.width))
		return 0 ;
	
	if(ypos>this.ypos+this.height)
		return 0
	this.showBonus=20 ;
	return 1 ;
}

////////////////////////////////////////////
// Create a spaceship
///////////////////////////////////////////
function Spaceship()
{
	var el=document.getElementById('spaceship') ;
	var bonus=Math.random() ;
	
	this.xpos=0 ;
	this.ypos=spaceshipYpos ;
	this.showBonus=0 ;
	this.Move=Spaceship_Move ;
	this.height=spaceshipHeight;
	this.width=spaceshipWidth ;
	this.CheckCollision=Spaceship_CheckCollision ;
	this.Kill=Spaceship_Kill ;
	if(bonus<0.25)
		this.bonus=100 ;
	else
		if(bonus<0.5)
			this.bonus=150 ;
		else
			if(bonus<0.75)
				this.bonus=200 ;
			else
				this.bonus=250 ;
	el.style.left=this.xpos +'px';
	el.style.visibility="visible" ;
}	
	


/////////////BARRIER ELEMENT CLASS
function BarrierElement_CheckCollision(xpos,ypos)
{
	if(xpos<this.xpos || xpos>this.xpos+4)
		return 0;

	if(ypos<this.ypos || ypos>this.ypos+8)
		return 0;

	return 1 ;
}

function BarrierElement_Kill()
{
	var element=document.getElementById(this.id) ;
	element.style.visibility="hidden"; 
}

function BarrierElement(xpos,ypos,id)
{
	var element ;
	element=document.getElementById(id) ;
	element.style.visibility="visible" ;
	this.xpos=xpos;
	this.ypos=ypos; ;
	this.Kill=BarrierElement_Kill ;
	this.id=id ;
	this.CheckCollision=BarrierElement_CheckCollision ;
	this.intact=1 ;
}
//////// END BARRIER ELEMENT CLASS /////////////////


///////// BARRIER CLASS ///////////////

function Barrier_CheckCollision(xpos,ypos,height)
{
	var i ;
	var element ;
	if(xpos<this.xpos || xpos>this.xpos+this.width || ypos<this.ypos || ypos>this.ypos+this.height)
		return 0 ;
	for(i=0 ; i<this.elementCount ; i++)
	{
		if(this.elements[i]!=0)
		{
			if(this.elements[i].CheckCollision(xpos,ypos))
			{
				this.elements[i].Kill() ;				
				delete this.elements[i] ;
				this.elements[i]=0 ;
				return 1;
			}
	
			if(this.elements[i].CheckCollision(xpos,ypos+height))
			{
				this.elements[i].Kill() ;				
				delete this.elements[i] ;
				this.elements[i]=0 ;
				return 1 ;
			}
		
	 	}
  	}
	return 0;
}

function Barrier_CheckInvaderCollision()
{
	var i ;
	// While we are here, see if we need to do an invader check
	if(bottomMost>=this.ypos)
	{
		for(i=0 ; i<this.elementCount ; i++)
		{
			if(this.elements[i]!=0)
			{
				// If traveling to the right
				if(invaderXJump>0)
				{
					if(this.elements[i].xpos<rightMost && this.elements[i].ypos<=bottomMost)
					{
						this.elements[i].Kill() ;				
						delete this.elements[i] ;
						this.elements[i]=0 ;
					}
				}
				else
				{
					// If invaders to the left
					if(this.elements[i].xpos>leftMost && this.elements[i].ypos<=bottomMost)
					{
						this.elements[i].Kill() ;				
						delete this.elements[i] ;
						this.elements[i]=0 ;
					}
			  	}
			}
		}
	}			
}

function Barrier(barrierXpos,barrierYpos,barrierID)
{
	
	var elementID=0 ;

	this.xpos=barrierXpos ;
	this.ypos=barrierYpos ;
	this.elements=new Array() ;
	this.width=48 ;
	this.height=32 ;
	this.CheckCollision=Barrier_CheckCollision ;
	this.CheckInvaderCollision=Barrier_CheckInvaderCollision ;
	for(i=0 ; i<maxBarrierLines ; i++)
	{
		for(j=0 ; j<linePos[i].count ; j++) 
		{
			
			var xpos=linePos[i].xpos + (j*4)+ barrierXpos;
			var ypos=barrierYpos+linePos[i].ypos; 
			var divID="Bar"+barrierID+"_"+elementID ;
			this.elements[elementID]=new BarrierElement(xpos,ypos,divID) ;
			elementID++ ;
		}
	}
	this.elementCount=elementID ;
}	



//////////// END OF BARRIER CLASS ///////////////////////

function RowData(xpos,ypos,count)
{
	this.xpos=xpos ;
	//if (isExplorer)
	//	ypos=ypos/4; 
	this.ypos=ypos ;
	this.count=count ;
}

var maxBarrierLines=6 ;
//var linePos=new Array(new RowData(12,0,6),new RowData(4,8,10),new RowData(0,16,4),new RowData(32,16,4),new RowData(0,24,4),new RowData(32,24,4)) ;

function WriteBarrierHTML()
{
	var insert ;
	var barrierXStep=maxX/5 ;
	var barrierYpos;
	var barrierXpos=barrierXStep ;
	// True if microsoft
	if(isExplorer)
	{
		barrierYpos=maxY-shipHeight*2-invaders_scale*24 ;
		//alert('x');
	}
	else
	{
		barrierYpos=maxY-shipHeight*2-invaders_scale*10 ;
		//alert('n');
	}
	for(id=0 ; id<4 ; id++)
	{
		var elementID=0 ;
		for(i=0 ; i<maxBarrierLines ; i++)
		{
			for(j=0 ; j<linePos[i].count ; j++) 																			 
			{

				var xpos=linePos[i].xpos + (j*4)+ barrierXpos;
				if (isExplorer)
					var ypos=barrierYpos+linePos[i].ypos*invaders_scale*0.8; 
				else
					var ypos=barrierYpos+linePos[i].ypos; 
				var divID="Bar"+id+"_"+elementID ;
				// individual block of the barrier
				insert="<div id='"+divID+"' style='position:absolute; visibility:hidden ;width:"+4+"px; height:"+8+"px; top:"+ypos+"px; left:"+xpos+"px; background:"+barriercolor+"'></div>" ;
				elementID++ ;
				document.write(insert) ;
				document.close() ;
			}
		}
		barrierXpos+=barrierXStep ;
	}
}


function CreateBarriers()
{
 	var i ; 
	var barrierXStep=maxX/5 ;
	var xpos=barrierXStep ;
	var barrierYpos
	// True if microsoft
	if(isExplorer)
		barrierYpos=maxY-shipHeight*2-18 ;
	else
		barrierYpos=maxY-shipHeight*2-5 ;
	for(i=0 ; i<4 ; i++)
	{
		if(barriers[i]!=0)
			delete barriers[i] ;
		barriers[i]=new Barrier(xpos,barrierYpos,i) ;
		xpos+=barrierXStep ;
  	}
}


//////////////////////////////////////////////////////
// Disable text selection
//function disableselect(e){
//return false;
//}
//function reEnable(){
//return true;
//}
//document.onmousedown=disableselect;
//document.onclick=reEnable;
//////////////////////////////////////////////////////

///////////////////////////////////////////////////////
// INVADER BULLET CLASS
/////////////////////////////////////////////////////
// Check if bullet is in a rectangle
// true if bullet is in rectangle
function InvaderBullet_InRectangle(xpos,ypos,width,height)
{
	if(this.xpos<=xpos || this.xpos>=(xpos+20))
		return false ;

	if(this.ypos>=ypos && this.ypos<=(ypos+20))
	{

		return true ;
	}

	if((this.ypos+15)>=ypos && (this.ypos+15)<=(ypos+20))
	{
		return true ;
	}
		
	return false ;
}

function Debug(text)
{
	var debug=document.getElementById('debug') ;
	debug.innerHTML=text ;
}


// Returns 1 if bullet has expired
function InvaderBullet_Move()
{  
	this.ypos+=this.speed ;

	if(this.ypos>=ship.ypos+shipHeight) 
	{
		this.Kill() ;
		return 1 ;
	}
	this.element.style.top=this.ypos +'px';
	if(this.type==0)
	{
		this.bulletAnimation++ ;
		if(this.bulletAnimation >=6)
		{
			this.bulletAnimation=0 ;
		}
		document.images[this.graphicId].src=document.iBullet1[this.bulletAnimation].src ;
	}
	else
	{
		this.bulletAnimation++ ;
		if(this.bulletAnimation >=2)
		{
			this.bulletAnimation=0 ;
		}
		document.images[this.graphicId].src=document.iBullet2[this.bulletAnimation].src ;
  	}
		

	return 0 ;
}

// Kill this ship bullet
function InvaderBullet_Kill()
{
	this.element.style.visibility="hidden" ;
}


// Kill all invader bullets, not a class method
function InvaderBullet_KillAll()
{
	var i ;
	for(i=0 ; i<allowableInvaderBullets ; i++)
	{
		if(invaderBullets[i]!=0)
		{
			invaderBullets[i].Kill() ;
			delete invaderBullets[i] ;
			invaderBullets[i]=0 ;
			
		}
 	}
}

function InvaderBullet(idDiv,idSrc,xpos,ypos)
{
	

	this.state=0 ;
	this.xpos=xpos ;
	this.ypos=ypos ;

	this.speed=invaderBulletSpeed ;

	this.idDiv=idDiv ;
	this.graphicId=idSrc;
	this.height=invaderBulletHeight ;
	this.Move=InvaderBullet_Move ;
	this.Kill=InvaderBullet_Kill ;
  	this.InRectangle=InvaderBullet_InRectangle ;

	this.element=document.getElementById(this.idDiv) ;
	this.element.style.visibility="visible";
	this.element.style.left=xpos+'px' ;
	this.element.style.top=ypos+'px' ;
	this.bulletAnimation=0 ;

	if(Math.random()<0.5)
	{
			this.type=0 ;
			document.images[idSrc].src=document.iBullet1[this.bulletAnimation].src ;
                    
			
	}
		else
	{
			this.type=1 ;
			document.images[idSrc].src=document.iBullet2[this.bulletAnimation].src ;
			this.speed+=2 ;
	}
        document.images[idSrc].height=invaderBulletHeight[this.type];
	this.element.style.height=invaderBulletHeight[this.type] ;
}

function WriteInvaderBulletHTML()
{
	var i ;
	var make ;
	for(i=0 ; i<maxInvaderBullets ; i++)
	{
		make="<div id=\'IBDiv"+i+"\' style=\'visibility:hidden; position:absolute ;width:3px; height:0px top:0px ;left:0\'>";
		make+="<img id=\'IBSrc"+i+"\' src=\'"+invaders_image_path+"b01.png\'>" ;
		make=make+"</div>" ;
		document.write(make) ;
		document.close() ;
		invaderBullets[i]=0 ;	// Clear the bullet array
	}
	
}
						   

///////////////////////////////////////////////////
// 	ENDINVADER BULLET CLASS
//////////////////////////////////////////////////


////////////////////////////////////////////////////
// Define invader class
////////////////////////////////////////////////////


function Invader_GetYpos()
{
	return this.ypos ;
}

function Invader_GetXpos()
{
	return this.xpos ;

}

function Invader_Move()
{
	var element=document.getElementById(this.id) ;
	var oldX=this.xpos ;

	oldX+=invaderXJump ;
	if(invaderXJump>0)
	{
		if((oldX >=(maxX-invaderXJump)))
		{
			changeDir=1 ;
						
	  	}
		this.xpos+=invaderXJump ;
	}else
	{
		if(oldX<=(-invaderXJump))
		{
			changeDir=1 ;
		
		}
		this.xpos+=invaderXJump ;
	}
	element.style.left=this.xpos+'px' ;
	element.style.top=this.ypos+'px' ;

	this.incarnation++ ;
	if(this.incarnation>=2)
		this.incarnation=0 ;

	if(this.incarnation==0)
		document.images[this.graphicId].src=document.imageArray[this.graphic1].src ;
	else
		document.images[this.graphicId].src=document.imageArray[this.graphic2].src;

   // Now set the mosts
	if(this.xpos<leftMost)
		leftMost=this.xpos ;
	
	if(this.xpos>rightMost)
		rightMost=this.xpos ;

   if(this.ypos+this.height>bottomMost)
		bottomMost=this.ypos+this.height ;
}	



function Invader_Kill()
{
	var element=document.getElementById(this.id) ;
	element.style.visibility="hidden" ;
	
}
// Check bullet array for collision
function Invader_CollisionDetection()
{
	var i ;
	var graphic ;
	for(i=0 ; i<maxShipBullets ; i++)
	{
		if(shipBullets[i]!=0)
		{
			if(shipBullets[i].InRectangle(this.xpos,this.ypos,this.width,this.height))
			{
				this.state=3 ;
				document.images[this.graphicId].src=document.blowImage.src ;
				shipBullets[i].Kill() ;
				delete shipBullets[i] ;
				shipBullets[i]=0 ;
				return false;
			}
	  	}
 	}
	return 0 ;
}	  	



function Invader_SetPos(xpos,ypos)
{
	var element=document.getElementById(this.id) ;
	element.style.left=xpos +'px';
	element.style.top=ypos+'px' ;
	this.xpos=xpos ;
	this.ypos=ypos ;
}

function Invader_PosDelta(xposDelta,yposDelta)
{
	var element=document.getElementById(this.id) ;
	this.xpos=this.xpos+xposDelta ;
	this.ypos=this.ypos+yposDelta ;
	element.style.left=this.xpos +'px';
	element.style.top=this.ypos+'px' ;
}
// Invader constructor
function Invader(xpos,ypos,id,stage1,stage2,graphicID,iscore)
{
	
	this.xpos=xpos ;
	this.ypos=ypos ;
	this.width=iWidth ;
	this.height=iHeight ;
	this.id=id ;
	this.graphic1=stage1 ;
	this.graphic2=stage2 ;
	this.incarnation=0 ;	  
	this.state=0 ;		// For explosion (=1)
	this.graphicId=graphicID ;
	this.Getxpos=Invader_GetXpos ;
	this.GetYpos=Invader_GetYpos ;
	this.SetPos=Invader_SetPos ;
	this.Move=Invader_Move ;
	this.PosDelta=Invader_PosDelta ;
	this.Kill=Invader_Kill ;
	this.CollisionDetection=Invader_CollisionDetection ;
	this.score=iscore ;

	var el=document.getElementById(this.id) ;
	el.style.visibility="visible";
	el.style.top=ypos +'px';
	el.style.left=xpos+'px' ;
	document.images[this.graphicId].src=document.imageArray[stage1].src;
    document.images[this.graphicId].width=iWidth;

}

////////////////////////////////////////////
// Create the invaders  NOT A CLASS METHOD
////////////////////////////////////////////
function InitialiseInvaders()
{

	var i,j ;
	
	for(i=0 ; i<invaders_rows ; i++)
	{
		for(j=0 ; j<invaders_columns ; j++)
		{
			var identifier=(j+(i*invaders_columns));
			var invaderYpos=(i*iYSpace)+invaderYOffset ;
			insert="<div id=\'Inv" + identifier + "\' style=\'position:absolute; z-index:2; visibility:hidden; width:"+iWidth+"px; height:"+iHeight+"px ;left:0"+j*iXSpace+"px ; top:"+invaderYpos+"px\'>"  ;
			insert=insert+"<img id=\'ISrc" + identifier + "\'src=\'\'></div>" ;
			document.write(insert) ;
			document.close() ;
			
			Invaders[identifier]=0 ;
		}
	
	}		 
}

///////////////////////////////////////////
// Kill all invaders: NOT A CLASS METHOD
///////////////////////////////////////////
function Invader_KillAll()
{
	var i ;
	for(i=0 ; i<maxInv ; i++)
	{
		if(Invaders[i]!=0)
		{
			Invaders[i].Kill() ;
			delete Invaders[i] ;
			Invaders[i]=0 ;
		}
	}
}

////////////////////////////////////////////
// Re-generate the invaders. NOT A CLASS METHOD
////////////////////////////////////////////
function ReGenerateInvaders()
{
	var i,j ;
	invScore=invaders_rows*10 ;
	for(i=0 ; i<invaders_rows ; i++)
	{
		for(j=0 ; j<invaders_columns ; j++)
		{
			var identifier=(j+(i*invaders_columns));
			var invaderYpos=(i*iYSpace)+invaderYOffset ;
			CreateInvader(j*iXSpace,invaderYpos,identifier,invaderPicIndexes[i*2],invaderPicIndexes[(i*2)+1],invScore) ;
		}
		invScore=invScore-10 ;
	}		 
	invaderCount=invaders_rows * invaders_columns ;
	invaderXJump=Math.abs(invaderXJump) ;
	changeDir=0 ;
	invaderTimer=Math.round(invaderCount/2) ;
	lastScore=0 ;
}
///////////END OF INVADER CLASS /////////////////////

//////////////////////////////////////////////////////
// Ship bullet class
/////////////////////////////////////////////////////

// Check if bullet is in a rectangle
// true if bullet is in rectangle
function ShipBullet_InRectangle(xpos,ypos,width,height)
{
	if(this.xpos<=xpos || this.xpos>=(xpos+20))
		return false ;

	if(this.ypos>=ypos && this.ypos<=(ypos+20))
	{

		return true ;
	}

	if((this.ypos+15)>=ypos && (this.ypos+15)<=(ypos+20))
	{
		return true ;
	}
		
	return false ;
}

// Returns 1 if bullet has expired
function ShipBullet_Move()
{  
	var element ;
	element=document.getElementById(this.idDiv) ;

	if(this.ypos>0)
	{
		this.ypos-=shipBulletSpeed ;
	}
	if(this.ypos<=0) 
	{
		this.Kill() ;
		return 1 ;
	}
	element.style.top=this.ypos+'px' ;
	return 0 ;
}

// Kill this ship bullet
function ShipBullet_Kill()
{
	var element ;
	element=document.getElementById(this.idDiv) ;
	element.style.visibility="hidden" ;
}

// Kill all ship bullets; not a class method
function ShipBullet_KillAll()
{
	var i ; 
	for(i=0 ; i<maxShipBullets ; i++)
	{
		if(shipBullets[i]!=0)
		{
			shipBullets[i].Kill() ;
			delete shipBullets[i] ;
			shipBullets[i]=0 ;
		}
	}
}


function ShipBullet(idDiv,idSrc,xpos,ypos)
{
	var element ;

	this.state=0 ;
	this.xpos=xpos ;
	this.ypos=ypos ;
	this.idDiv=idDiv ;
	this.idSrc=idSrc ;
	this.height=shipBulletHeight ;
	this.Move=ShipBullet_Move ;
	this.Kill=ShipBullet_Kill ;
  	this.InRectangle=ShipBullet_InRectangle ;
	element=document.getElementById(this.idDiv) ;
	element.style.visibility="visible";
	element.style.left=xpos +'px';
	element.style.top=ypos +'px';
	
}

function WriteShipBulletHTML()
{
	var i ;
	var make ;
	for(i=0 ; i<maxShipBullets ; i++)
	{
		make="<div id=\'SBDiv"+i+"\' style=\'visibility:hidden; position:absolute ;width:3px; height:"+shipBulletHeight+"px; background:white; top:0px;left:0px\'>" ;		
		make=make+"<img src='"+invaders_image_path+"bullet.png'></div>" ;
		document.write(make) ;
		document.close() ;
		shipBullets[i]=0 ;	// Clear the bullet array
	}
	
}
		
									   

///////////////////////////////////////////////////
// 	END SHIP BULLET CLASS
//////////////////////////////////////////////////

//////////////////////////////////////////////////////////
/////////////////////////// SHIP CLASS //////////////////
//////////////////////////////////////////////////////////





function Ship_MoveShip()
{
	var element=document.getElementById('shipDiv') ;
	var i,j ;
	if(!this.state)
	{		

		if((this.xpos+this.xDir)>=0 && (this.xpos+this.xDir)<=(maxX-shipWidth))
			this.xpos+=this.xDir ;
		element.style.left=this.xpos +'px';
		element.style.top=this.ypos+'px' ;
	}
	else
	{	
		if(this.state==100)	// All lives gone
			return ;
		// If the ship is exploding
		if(this.state<(shipExplosion.length*2))
		{
			var currentExp=this.state ;
			if(currentExp>=shipExplosion.length)
				currentExp=1 ;
			currentExp-- ;
			document.images['shipImg'].src=document.shipexp[currentExp].src ;
			this.state++ ;
		}
		else
		{
			this.state=0 ;
			
			InvaderBullet_KillAll() ;
			ShipBullet_KillAll() ;
			//StopAllAudio() ;
			if(invaders_landed)
			{
				this.lives=0 ;
			}
			if(this.lives)
			{	
				document.images['shipImg'].src=document.shipImage.src;

				this.xpos=maxX/2 ;
				this.lives-- ;
				this.ShowLives() ;
				delayStart=30 ;
			}
			else
			{
				this.state=100	;		// Say all lives gone, prevent game continuation
				return 1 ;
			}
				
	  	}
	}
	// Now shift the ship bullets
	for(i=0 ; i<maxShipBullets ; i++)
	{
		var bullet ;
		if(shipBullets[i]!=0)
		{
			if(shipBullets[i].Move()!=0)
			{
				delete shipBullets[i] ;
				shipBullets[i]=0 ;
			}
			else
			{
				for(j=0 ; j<4 ; j++)
				{
					if(barriers[j].CheckCollision(shipBullets[i].xpos,shipBullets[i].ypos,shipBulletHeight) || barriers[j].CheckCollision(shipBullets[i].xpos,shipBullets[i].ypos+(shipBulletHeight/2),shipBulletHeight/2))
					{
						shipBullets[i].Kill() ;
						delete shipBullets[i] ;
						shipBullets[i]=0 ;
						break ;
					}
				}

				if(bonusSpaceship !=0 && shipBullets[i]!=0)
				{
					if(bonusSpaceship.CheckCollision(shipBullets[i].xpos,shipBullets[i].ypos))
					{
						invaders_score.AddScore(bonusSpaceship.bonus) ;
						shipBullets[i].Kill() ;
						delete shipBullets[i] ;
						shipBullets[i]=0 ;
						//DoPlay("bshit") ;
						//StopPlay("bship") ;
					}
				}

			}
	  	}
  	}
	// Finally check for a bonus life
	if(invaders_score.GetScore()>bonusLifeScore && !this.lifeGiven)
	{
		this.lives++ ;
		this.ShowLives() ;
		this.lifeGiven=1 ;
	}
	return 0 ;
}

function Ship_Die()
{
	ship.state=1 ;
	//DoPlay("wav6") ;
}

function Ship_Kill()
{
	var element=document.getElementById('shipDiv') ;
	element.style.visibility="hidden" ;
}

function Ship_Shoot()
{
	// Find an empty slot in the array
	var i ;
	if(!this.state)
	{
		for(i=0 ; i<maxShipBullets ; i++)
		{
			if(shipBullets[i]==0)
			{
				shipBullets[i]=new ShipBullet("SBDiv"+i,"SBSrc"+i,this.xpos+(shipWidth/2),this.ypos-shipHeight/2) ;
				//DoPlay("wav4") ;
				break ;
			}
	 	}
	}
}

function Ship_SetDir(dir)
{
	this.xDir=dir ;
}

function Ship_ShowLives()
{
	var i ;
	for(i=0 ; i<invaders_lives+1 ; i++)
	{
		if(this.lives>i)
			document.images["life"+i].style.visibility="visible" ;
		else
			document.images["life"+i].style.visibility="hidden" ;
	}
}

////////////////////
// Ship constructor 
////////////////////
function Ship(startX,startY)
{
	this.xpos=startX ;
	this.ypos=startY ;
	this.xDir=0 ;
	this.Move=Ship_MoveShip ;
	this.SetDir=Ship_SetDir ;
	this.Shoot=Ship_Shoot ;
	this.Die=Ship_Die ;
	this.Kill=Ship_Kill ;
	this.ShowLives=Ship_ShowLives; 
	this.lives=invaders_lives ;
	this.lifeGiven=0 ;
	this.state=0 ;			// Alive=0, dying=1+, dead=10
	document.images['shipImg'].src=document.shipImage.src  ;
        document.images['shipImg'].width=shipWidth;
	var el=document.getElementById('shipDiv') ;
	el.style.visibility="visible" ;
}

/////////////////////////////////////////////////////
// End ship class
/////////////////////////////////////////////////////

////////////////////////////
////////// Set the score
///////////////////////////
function Score_ShowScore()
{
	var scoreHTML=document.getElementById('score') ;
	scoreHTML.innerHTML=this.text+this.score;
}


function Score_AddScore(award)
{
	this.score+=award ;
	this.ShowScore() ;
}

function Score_SetScore(newScore)
{
	this.score=0 ;
	this.ShowScore() ;
}

function Score_GetScore()
{
	return this.score ;
}


function Score(w,h,x,y,text)
{
	var HTML ;
	this.score=0 ;
	this.text=text ;
	HTML="<div style='position:absolute; top:"+y+"px; left:"+x+"px; width:"+w+"px; height:"+h+"px'><font  iface='Arial, Helvetica sans-serif' istyle='font-weight:bold;color:white' id='score'>"+this.text+"0</font></div>" ;
	document.write(HTML) ;
	document.close() ;
	this.AddScore = Score_AddScore ;
	this.SetScore = Score_SetScore ;
	this.ShowScore = Score_ShowScore ;
	this.GetScore= Score_GetScore ;
}


/////////////////////////////////////////////////////
// Create an invader with an ID
/////////////////////////////////////////////////////
function CreateInvader(xpos,ypos,identifier,stage1,stage2,iscore)
{
	Invaders[identifier]=new Invader(xpos,ypos,"Inv"+identifier,stage1,stage2,"ISrc"+identifier,iscore) ;
}


////////////////////////////////////////////////////
// Move everything
///////////////////////////////////////////////////
function MoveAll()
{
	var i,j ;
	var lastDir=0 ;
	var moved=0 ;
	var barrier ;
	
	window.setTimeout("MoveAll()",1) ;
	//ShowGameSpeed() ;

	if(ship==0)
		return ;
	// Utilise user settable speed scaler
	if(gameSpeedTimer !=0)
  	{
		gameSpeedTimer-- ;
		return ;
	}
	gameSpeedTimer=invaders_gamespeed;//gameSpeed; 

	// Delay after life lost
	if(delayStart)
	{
		delayStart-- ;
		return ;
	}

	// Use this timer to implement a small delay before  the next level starts
	if(restartLevel)
	{
		restartLevel-- ;
		if(!restartLevel)
		{
			ship.xpos=Math.round(maxX/2) ;
			allowableInvaderBullets+=2 ;
			if(allowableInvaderBullets>maxInvaderBullets)
				allowableInvaderBullets=maxInvaderBullets;
			NewLevel() ;
		}
	}

	invaderTimer--;

	if(invaderTimer==0)
	{

		/*if(invaderAudio==1)
			DoPlay("wav0") ;

		if(invaderAudio==2)
			DoPlay("wav1") ;
		if(invaderAudio==3)
			DoPlay("wav2") ;
		if( invaderAudio == 4)
			DoPlay("wav3") ;
		invaderAudio++ ;
		if(invaderAudio>4)
			invaderAudio=1 ;*/
	}

	for(i=0 ; i<maxInv ; i++)
	{
		  	
		if(Invaders[i])
		{
			
			
			if(Invaders[i]!=0 && invaderTimer==0 && !ship.state)
			{
				Invaders[i].Move() ;
				// is this where detects has reached bottom?
				//Debug(Invaders[i].ypos+" "+(maxY-iHeight));

				if(Invaders[i].ypos>(maxY-iHeight))
				{
					ship.Die() ;
					//Debug("Death");
					invaders_landed=1 ;
				}
			}
			Invaders[i].CollisionDetection() ;
			// Check for exploding invaders
			if(Invaders[i].state)
			{
				Invaders[i].state-- ;
				if(Invaders[i].state==0)
				{
					Invaders[i].Kill() ;
					invaders_score.AddScore(Invaders[i].score) ;
					delete Invaders[i] ;
					Invaders[i]=0 ;
					invaderCount-- ;
					if(invaderCount==0)
						restartLevel=50 ;
					if(invaderCount<5)
						invaderBulletChance=0.5 ;
					//DoPlay("wav5") ;
				
				}
		 	}

			// Generate and move invader bullets if invader was not killed above and ship not dying/dead
			if((Math.random()<invaderBulletChance) && !ship.state && Invaders[i])
			{
				for(j=0 ; j<allowableInvaderBullets ; j++)
				{
					if(invaderBullets[j]==0)
					{
						var xDelta=0 ;
						var rnd=Math.random() ;
						if(rnd<0.33)
							xDelta=-3;
						if(rnd>0.66)
							xDelta=3 ;

						invaderBullets[j]=new InvaderBullet("IBDiv"+j,"IBSrc"+j,Invaders[i].xpos+(iWidth/2)+xDelta,Invaders[i].ypos+iHeight) ;
						break ;
					}
			  	}
			}
		}
	}

	
	for(i=0 ; i<allowableInvaderBullets ; i++)
	{
		if(invaderBullets[i]!=0)
		{
			// If the Ypos is too high
			if(invaderBullets[i].Move()!=0)
			{
				invaderBullets[i].Kill() ;
				delete invaderBullets[i] ;
				invaderBullets[i]=0 ;
			}
			else
			{
					if(invaderBullets[i].InRectangle(ship.xpos,ship.ypos,shipWidth,shipHeight))
					{
						invaderBullets[i].Kill() ;
						delete invaderBullets[i] ;
						invaderBullets[i]=0 ;
						ship.Die() ;
					}
				
					if(invaderBullets[i]!=0)
					{
						for(barrier=0 ; barrier<4 ; barrier++)
						{
							 	if(barriers[barrier].CheckCollision(invaderBullets[i].xpos,invaderBullets[i].ypos,invaderBulletHeight))
								{
									invaderBullets[i].Kill() ;
									delete invaderBullets[i] ;
									invaderBullets[i]=0 ;
									break ;
								}
								
						}
					}
			}
		}
	}

	
	if(!invaderTimer)
	{
		invaderTimer=Math.round((invaderCount/2)/invaders_level) ;
		for(barrier=0 ; barrier<4 ; barrier++)
			if(barriers[barrier]!=0)
				barriers[barrier].CheckInvaderCollision() ;
	}


  
		
	if(changeDir!=0)
	{
		for(i=0 ; i<maxInv ; i++)
		{
			if(Invaders[i]!=0)
				Invaders[i].PosDelta(0,invaderYJump) ;
		}
		invaderXJump=-invaderXJump ;
		changeDir=0 ;
		ResetMosts() ;
	}

	if(ship!=0)
	{
		if(bonusSpaceship == 0)
		{
	
			if(Math.random()<0.002)
				bonusSpaceship=new Spaceship() ;
		}
		else
	  	{
			if(bonusSpaceship.Move())
			{
				delete bonusSpaceship ;
				bonusSpaceship=0 ;
			}

	  	}
		if(ship.Move())
		{
			Invader_KillAll() ;
			
			if(bonusSpaceship)
			{
				bonusSpaceship.Kill() ;
				delete bonusSpaceship ;
				bonusSpaceship=0 ;
			}
			
			ship.Kill() ;
		 	delete ship ;
			ship=0 ;
			var gOverEl=document.getElementById('gameOver') ;
			gOverEl.style.visibility="visible" ;
			gOverEl.innerHTML=invaders_gameover_message+"<br/>"+invaders_start_message;
			
			//document.getElementById('invaders_formmessage').innerHTML = invaders_gameover_message;

			if (document.getElementById('invaders_winform')&&
			   (!invaders_bestscore||invaders_bestscore==-1||invaders_score.GetScore()>invaders_bestscore))
			{
				document.getElementById('invaders_win_message').value=gOverEl.innerHTML;
				document.getElementById('invaders_ticker').value=invaders_score.GetScore();
				document.getElementById('invaders_winform').submit();
			}							  
			

			
	
		}

	}	

	// Update the score
	if(lastScore !=invaders_score.GetScore())
	{
		invaders_score.ShowScore() ;
		lastScore=invaders_score.GetScore()  ;
	}
}


/*function ShowGameSpeed()
{
	var el=document.getElementById('speedText') ;
	el.innerHTML="Speed="+gameSpeed ;
}*/

function Slower()
{
	var pathname = location.pathname;
   var myDomain = pathname.substring(0,pathname.lastIndexOf('/')) +'/';
   var largeExpDate = new Date ();
   largeExpDate.setTime(largeExpDate.getTime() + (30 * 24 * 3600 * 1000));
	//document.images['slowbutton'].src=document.slowDown.src ;
	invaders_gamespeed++ ;
   //SetCookie('gamespeed',gameSpeed,largeExpDate,myDomain);
	//ShowGameSpeed() ;
}

function Faster()
{
	
	var pathname = location.pathname;
   var myDomain = pathname.substring(0,pathname.lastIndexOf('/')) +'/';
   var largeExpDate = new Date ();
   largeExpDate.setTime(largeExpDate.getTime() + (30 * 24 * 3600 * 1000));
	//document.images['fastbutton'].src=document.fastDown.src ;
	if(invaders_gamespeed>0)
		invaders_gamespeed-- ;
   //SetCookie('gamespeed',gameSpeed,largeExpDate,myDomain);
	//ShowGameSpeed() ;
}


function moveTo(x)
{
	if (ship.xpos>x+1)//+shipxshift)
		//ship.SetDir(-shipxshift) ;
		ship.xpos=ship.xpos-1;//(ship.xpos-x)/shipxshift;//10;//ship.xpos-1;
	else if (ship.xpos<x-1)//-shipxshift)
		ship.xpos=ship.xpos+1;//(ship.xpos-x)/shipxshift;//10;
	else
	    clearInterval(theinterval);
		//ship.SetDir(shipxshift) ;
}

/////////////////////////////////////
// The key down handler
////////////////////////////////////
function mousemoveHandler(e)
{
 //alert('x');
  if (!isExplorer) {
    id=e.target.id;
    mX = e.layerX;//e.pageX;
    mY = e.layerY;//e.pageY;
  }
  else {
    //var scoreHTML=document.getElementById('score') ;
	//scoreHTML.innerHTML=event.srcElement.id;//parentNode.id+'nnn'+x;
	id=event.srcElement.id;
    mX = event.offsetX + document.body.scrollLeft;//window.event.clientX;
    mY = event.offsetY + document.body.scrollTop;//window.event.clientY;

  }
  // stop case where mouseover an invader and get pos relative to invader div

  //  var scoreHTML=document.getElementById('score') ;
  //	scoreHTML.innerHTML=id;
  if (id==''||id=='invaders_background_image')
  {
	clearInterval(theinterval);
	theinterval=setInterval("moveTo("+mX+");",shipXMouseTime);//25);
  }
}

function mousedownHandler(e)
{
//alert('x');
	if (ship)
		ship.Shoot() ;
}

/////////////////////////////////////
// The key down handler
////////////////////////////////////
function keyDownHandler(e)
{
	
	var code ;
	// True if navigator
	if(isExplorer)
 	{
		e=window.event ;
		code=e.keyCode ;
	}
	else
		code=e.which ;

	if((code==invaders_leftkey/*||code==90*/) && ship)
	{
		ship.SetDir(-shipxshift) ;
	
	}
	if((code==invaders_rightkey/*||code==88*/) && ship)
		ship.SetDir(shipxshift) ;

		   //space               forward slash   c
	if((code==invaders_firekey)/*32||code==191||code==67)*/&& ship)
		ship.Shoot() ;
}

/////////////////////////////////////
// The key up handler
////////////////////////////////////
function keyUpHandler(e)
{
	var code ;
	if(navigator.appName.indexOf("Microsoft")!=-1)
 	{
		e=window.event ;
		code=e.keyCode ;
	}
	else
		code=e.which ;

	if(ship && (code==90 || code==88|| code==37 || code==39))
		ship.SetDir(0) ;
	
}
 

function NewLevel()
{
	ReGenerateInvaders() ;
	CreateBarriers() ;
	invaderBulletChance=0.1 ;
	delayStart=0 ;
	if(bonusSpaceship)
  	{
		bonusSpaceship.Kill() ;
		bonusSpaceship=0 ;
  	}
	ResetMosts() ;
	invaderAudio=0 ;
	gameSpeedTimer=invaders_gamespeed;

}

/////////////////////////////////////////
// Function to resatart the game
//////////////////////////////////////////
function RestartGame()
{
	var el=document.getElementById('gameOver') ;
	el.style.visibility="hidden" ;
	
	invaders_landed=0 ;
	restartLevel=0 ;
	// Create the ship
	if(ship !=0)
		delete ship ;
	ship = new Ship(Math.round(maxX/2),maxY) ;
	invaders_score.SetScore(0) ;
	NewLevel() ;
	running=1 ;
}


function GetSpeed()
{

	if(invaders_gamespeed==null || invaders_gamespeed<0)
	{
		invaders_gamespeed=6 ;
		Faster() ;
	}
	document.getElementById('invaders_loading').style.visibility="hidden" ;
	document.getElementById('gameOver').style.visibility="visible" ;

}



// Start the game, navigator seems a lot slower than explorer
function StartTimer()
{
	if(isExplorer)
	{
		window.setTimeout("MoveAll()",10) ;
		updateSpeed=10 ;
	}
	else
	{
		window.setTimeout("MoveAll()",5) ;
		updateSpeed=5 ;
	}
}

function Init()
{
InitScale();

// Set up the keyboard handler
if (invaders_keyboard)
{
	document.onkeydown=keyDownHandler ;
	document.onkeyup=keyUpHandler ;
}

// Array of ship explosion images
 shipExplosion=new Array(invaders_image_path+"sexp0.png",invaders_image_path+"sexp1.png",invaders_image_path+"sexp2.png",invaders_image_path+"sexp3.png",invaders_image_path+"sexp0.png",invaders_image_path+"sexp1.png",invaders_image_path+"sexp2.png",invaders_image_path+"sexp3.png") ;
document.shipexp=new Array() ;
for(index=0 ; index<8 ; index++)																										 
{
	document.shipexp[index]=new Image() ;
	document.shipexp[index].src=shipExplosion[index] ;
}
//document.onmousemove=mousemoveHandler;
//document.onMouseDown=mousedownHandler;

document.shipImage = new Image ;
document.shipImage.src=invaders_image_path+"ship1.png" ;

document.blowImage=new Image ;
document.blowImage.src=invaders_image_path+"iexp.png" ;

document.imageArray=new Array() ;
for(index=0 ; index<maxInvaderPics ; index++)
{
	document.imageArray[index]=new Image() ;
	document.imageArray[index].src=invaderPics[index] ;
}

var b1Names=new Array(invaders_image_path+"b01.png",invaders_image_path+"b02.png",invaders_image_path+"b03.png",invaders_image_path+"b04.png",invaders_image_path+"b05.png",invaders_image_path+"b06.png") ;

document.iBullet1=new Array() ;
for(index=0 ; index<6 ; index++)
{
	document.iBullet1[index]=new Image()
	document.iBullet1[index].src=b1Names[index] ;
}


var b2Names=new Array(invaders_image_path+"b10.png",invaders_image_path+"b11.png") ;

document.iBullet2=new Array() ;
for(index=0 ; index<2 ; index++)
{
	document.iBullet2[index]=new Image()
	document.iBullet2[index].src=b2Names[index] ;
}

InitialiseInvaders() ;
WriteShipBulletHTML() ;
WriteInvaderBulletHTML() ;
WriteBarrierHTML() ;


//WriteSlowFastHTML() ;
//WriteControlHTML(0,maxY+shipHeight,maxX,20,"Z=Left  X=Right  ?=Fire","right") ;
var makeHTML="<div id='shipDiv' style='position:absolute;top:"+maxY+"px ; visibility:hidden; left:"+Math.round(maxX/2)+"px; width:26px; height:16px'><img id='shipImg' src='"+invaders_image_path+"ship1.png'></div>";
makeHTML+="<div style='position:absolute; left:0px; top:"+lifeLineYpos+"px; width:"+Math.round(maxX/2)+"px; height:16px'><img style='visibility:visible' id='life0' src='"+invaders_image_path+"ship1.png' width='"+shipWidth+"'>";
for (i=1; i<invaders_lives+1; i++)
{
  if (i<invaders_lives)
	vis='visible';
  else
	vis='hidden';
  makeHTML+="<img style='visibility:"+vis+"' id='life"+i+"' src='"+invaders_image_path+"ship1.png' width='"+shipWidth+"'>";
}
//makeHTML+="<img style='visibility:visible' id='life2' src='"+invaders_image_path+"ship1.png' width='"+shipWidth+"'>" ;
//makeHTML+="<img style='visibility:hidden'  id='life3' src='"+invaders_image_path+"ship1.png' width='"+shipWidth+"'>";
makeHTML+="</div>";
makeHTML+="<div align='center' id='gameOver' style='position:absolute ; z-index:2 ;visibility:hidden ; top:"+Math.round(maxY/4)+"px ; left:0px; width:"+maxX+"px ; height:100px' onMouseDown='RestartGame()'>";
makeHTML+=invaders_start_message+"</div>";
	
makeHTML+="<div id='spaceship' style='position:absolute;  height:"+spaceshipHeight+"px; width:"+spaceshipWidth+"px; top:"+spaceshipYpos+"px; left:0px;visibility:hidden'><img style='height:"+spaceshipHeight+"px; width:"+spaceshipWidth+"px;' src='"+invaders_image_path+"bship.png'></div>";
makeHTML+="<div id='spaceshipText' style='position:absolute;  height:"+spaceshipHeight+"px; width:"+spaceshipWidth+"px; top:"+spaceshipYpos+"px; left:0px; visibility:hidden'></div>";

document.write(makeHTML) ;
document.close() ;
if (invaders_background)
{
  document.getElementById('invaders_background_image').src=invaders_background;
  document.getElementById('invaders_background_image').style.width=maxX+"px";
  document.getElementById('invaders_background_image').style.height=maxY+"px";
  document.getElementById('invaders_background_image').style.opacity=invaders_background_transparency;
  document.getElementById('invaders_background_image').style.MozOpacity=invaders_background_transparency;
  document.getElementById('invaders_background_image').style.filter='alpha(opacity='+parseInt(100*invaders_background_transparency)+')';//.alpha.opacity=parseInt(100*pong_background_transparency);
}


invaders_score = new Score(0,maxX/2,maxX*0.6,16,"Score&nbsp;") ;

}

