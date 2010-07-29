/*<!----------------------------------------------------------------\
|                                                                 |
|  Fair License                                                   |
|                                                                 |
|  JS Games :: Arrange                                            |
|  Copyright (C) 2002-2004 Arun Narayanan                         |
|  Modified for Drupal by Glenn Linde                             |
|                                                                 |
|  For latest release information and downloads visit:            |
|  http://jsgames.sourceforge.net/                                |
|                                                                 |
|  Usage of the works is permitted provided that this             |
|  instrument is retained with the works, so that any entity      |
|  that uses the works is notified of this instrument.            |
|                                                                 |
|  DISCLAIMER: THE WORKS ARE WITHOUT WARRANTY.                    |
|                                                                 |
|  [2004, Fair License: rhid.com/fair]                            |
|  Fair License is GNU compatible                                 |
|                                                                 |
\----------------------------------------------------------------->
*/

var gsize, ghrow, ghcol, gtime, gmoves, gintervalid=-1, gshuffling;
var imagename; 
var imagewidth, imageheight;
var messagestart;
var messagewin;
var magicsquares_besttime;
var magicsquares_shuffleimageinitially;
var tryloadimage;

function toggleHelp()
{
  if (butHelp.value == "Hide Help")
  {
    help.style.display = "none";
    butHelp.value = "Show Help";
  }
  else
  {
    help.style.display = "";
    butHelp.value = "Hide Help";
  }  
}

//random number between low and hi
function r(low,hi)
{
  return Math.floor((hi-low)*Math.random()+low); 
}

//random number between 1 and hi
function r1(hi)
{
  return Math.floor((hi-1)*Math.random()+1); 
}

//random number between 0 and hi
function r0(hi)
{
  return Math.floor((hi)*Math.random()); 
}

function startGame()
{
  shuffle();
  gtime = 0;
  gmoves = 0;
  clearInterval(gintervalid);
  tickTime();

  document.getElementById('magicsquares_formmessage').innerHTML=messagestart;
  document.getElementById('magicsquares_startstop').value='Reset';
  gintervalid = setInterval("tickTime()",1000);
}

function stopGame()
{
  if (gintervalid==-1) return;
  clearInterval(gintervalid);
  //document.getElementById('magicsquares_fldStatus').innerHTML = "";
  gintervalid=-1;
}

function tickTime()
{
  showStatus();
  gtime++;
}

function checkWin()
{
  var i, j, s, v;

  if (gintervalid==-1) return; //game not started!
  
  if (!isHole(gsize-1,gsize-1)) return;

  for (i=0;i<gsize;i++)
    for (j=0;j<gsize;j++)
    {
      if (!(i==gsize-1 && j==gsize-1)) //ignore last block (ideally a hole)
      {
          /* Deal with image or no image tile number */
          num=getCell(i,j).innerHTML;
          if (num.length>4)
            num=getCell(i,j).firstChild.getAttribute('v');

        if (num!=(i*gsize+j+1).toString()) return;

      }
    }
  stopGame();
  if (!messagewin)
  {
    s = "<table cellpadding=4>";
    s += "<tr><td align=center class=capt3>!! CONGRATS !!</td></tr>";
    s += "<tr class=capt4><td align=center>You have done it in " + gtime + " secs ";
    s += "with " + gmoves + " moves!</td></tr>";
    s += "<tr><td align=center class=capt4>Your speed is " + Math.round(1000*gmoves/gtime)/1000 + " moves/sec</td></tr>";
    s += "</table>";
  }
  else
    s=messagewin;

  document.getElementById('magicsquares_formmessage').innerHTML = s;
  document.getElementById('magicsquares_startstop').value='Start';

  if (document.getElementById('magicsquares_winform')&&
     (!magicsquares_besttime||magicsquares_besttime==-1||gtime<magicsquares_besttime))
  {
    document.getElementById('magicsquares_message').value=document.getElementById('magicsquares_formmessage').innerHTML;
    document.getElementById('magicsquares_ticker').value=gtime;
    document.getElementById('magicsquares_winform').submit();
  }
//  shuffle();
}

function showStatus()
{
  document.getElementById('magicsquares_fldStatus').innerHTML = "Time:&nbsp;" + gtime + " secs&nbsp;&nbsp;&nbsp;Moves:&nbsp;" + gmoves
}

function showTable()
{
  var i, j, s;

  stopGame();
 // s = "<table border='3'  bgcolor='#666655'><tr><td class=bigcell>";
  s =/*s +*/ "<table style='width:0px'>"; 

  for (i=0; i<gsize; i++)
  {
    s = s + "<tr>";    
    for (j=0; j<gsize; j++)
    {

      // This if is for firefox
      if (imagename&&imagename!='')
      {
        if (j==0&&i==0)
          s = s + "<td id='a_" + i + "_" + j + "' onclick='move(this)' class='magicsquares_cell'><div v='"+(i*gsize+j+1)+"' style='position:relative;  background-image:url("+imagename+"); width:"+(imagewidth / gsize)+"px; height:"+imageheight / gsize+"px ;'  ></div></td>";
      else
          s = s + "<td id='a_" + i + "_" + j + "' onclick='move(this)' class='magicsquares_cell'><div v='"+(i*gsize+j+1)+"' style='background-position: " + -(j*(imagewidth / gsize)) + "px " +  -(i*(imageheight / gsize))+ "px;    background-image:url("+imagename+"); width:"+(imagewidth / gsize)+"px; height:"+imageheight / gsize+"px;'  ></div></td>";
    
     }
     else
        s = s + "<td id='a_" + i + "_" + j + "' onclick='move(this)'  class='magicsquares_cell' width='40'>"+(i*gsize+j+1)+"</td>";
    }
    s = s + "</tr>";        
  }
  s = s + "</table>";

//  s = s + "</td></tr></table>";

  return s;
}

function getCell(row, col)
{
  //return document.getElementById("a_1_1");
  return document.getElementById("a_" + row + "_" + col);
 //return eval("a_" + row + "_" + col);

}

function setValue(row,col,val)
{
  var v = getCell(row, col);
  v.innerHTML = val;
  
  v.className = "magicsquares_cell";
}

function getValue(row,col)
{

  var v = getCell(row, col);
  return v.innerHTML;

}

function setHole(row,col)
{ 
  var v = getCell(row, col);
  v.innerHTML = "";
  
  v.className = "hole";
  ghrow = row;
  ghcol = col;
}

function getRow(obj)
{
  var a = obj.id.split("_");
  return a[1];
}

function getCol(obj)
{
  var a = obj.id.split("_");
  return a[2];
}

function isHole(row, col)
{
  return (row==ghrow && col==ghcol) ? true : false;
}

function getHoleInRow(row)
{
  var i;
  
  return (row==ghrow) ? ghcol : -1;
}

function getHoleInCol(col)
{
  var i;

  return (col==ghcol) ? ghrow : -1;
}

function shiftHoleRow(src,dest,row)
{
  var i;

  //conversion to integer needed in some cases!
  src = parseInt(src);
  dest = parseInt(dest);

  if (src < dest)
  {
    for (i=src;i<dest;i++)
    {
      setValue(row,i,getValue(row,i+1));
      setHole(row,i+1);
    }
  }
  if (dest < src)
  {
    for (i=src;i>dest;i--)
    {
      setValue(row,i,getValue(row,i-1));
      setHole(row,i-1);
    }
  }
}

function shiftHoleCol(src,dest,col)
{
  var i;
  
  //conversion to integer needed in some cases!
  src = parseInt(src);
  dest = parseInt(dest);
    
  if (src < dest)
  {
    for (i=src;i<dest;i++)
    {
      setValue(i,col,getValue(i+1,col));
      setHole(i+1,col);
    }
  }
  if (dest < src)
  {
    for (i=src;i>dest;i--)
    {
      setValue(i,col,getValue(i-1,col));
      setHole(i-1,col);
    }
  }
}

function move(obj)
{
  var r, c, hr, hc;

  if (gintervalid==-1 && !gshuffling) 
  {
    startGame();
    //alert('Please press the "Start Game" button to start.');
    return;
  }
  r = getRow(obj);
  c = getCol(obj);
  if (isHole(r,c)) return;
  
  hc = getHoleInRow(r);
  if (hc != -1)
  {
    shiftHoleRow(hc,c,r);
    gmoves++;
    checkWin();
    return;
  }
  
  hr = getHoleInCol(c);

  if (hr != -1)
  {
    shiftHoleCol(hr,r,c);
    gmoves++;
    checkWin();
    return;
  }
}

function shuffle()
{
  var t,i,j,s,frac;

  gshuffling =  true;
  frac = 100.0/(gsize*(gsize+10));
  s = "% ";
  for (i=0;i<gsize;i++)
  {
    s += "|";
    for (j=0;j<gsize+10;j++)
    {  
      window.status = "Loading " + Math.round((i*(gsize+10) + j)*frac) + s  
      if (j%2==0)
      {
        t = r0(gsize);
        while (t == ghrow) t = r0(gsize); //skip holes
        // getCell(t,ghcol).click();
        move(getCell(t,ghcol));
      } 
      else
      {
        t = r0(gsize);
        while (t == ghcol) t = r0(gsize); //skip holes
        move(getCell(ghrow,t));
        //getCell(ghrow,t).click();
       
      }
    }
  }
  window.status = "";
  gshuffling = false;
}
var x;

function loadimage()
{
      x=new Image(); 
      x.src=imagename;
      imagewidth=x.width;
      imageheight=x.height;
	  if (x.width==0&&tryloadimage<5)
	  {
	    setTimeout('loadimage()',1000);
		tryloadimage++;
	  }
	  else
	  {
        document.getElementById('magicsquares_board').innerHTML = showTable(gsize);
        //document.write(showTable(gsize));
        setHole(gsize-1,gsize-1);
        if (magicsquares_shuffleimageinitially)
          shuffle();
	  }
}

function loadBoard(size)
{
  tryloadimage=0;
  gsize = size;
  if (imagename&&imagename!='')
  {
     loadimage();
   // x.onload = loadimage();
    //loadimage(); //function() { };
	//setTimeout('loadimage()',4000);
  }
  else
  {
      document.getElementById('magicsquares_board').innerHTML = showTable(gsize);
      setHole(gsize-1,gsize-1);
      if (magicsquares_shuffleimageinitially)
        shuffle();	  
  }


  


  //shuffle();
}

