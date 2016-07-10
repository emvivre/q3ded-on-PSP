This is a port of the quake 3 server binary (q3ded) on the Sony PSP. The source was released approximately one decade ago. This is the unmodified source of the project.

see http://lukasz.dk/mirror/forums.ps2dev.org/viewtopic62f9.html?t=9529


/*
=======================================================================
			     PSP Q3 Server Only	            
=======================================================================
*/

/*
===========================
Introduction
===========================
*/
I readed the Quake 3 server source code and I thinked it will be good to the
see a Quake 3 server on a PSP. Then, I ported it to PSP.
So, it's just the server side. No client.

I tested the Quake 3 server on a PSP firmware 1.5 : it works !
For others firmware, I don't know :p

I didn't change many thinks on the original source code, I just ported the code for the PSP.
There isn't interface yet, for configure the server you must edit the q3config.cfg
and for administrate the running server you must use rcon command.

Unfortunately 4 maps don't works : q3dm11, q3dm12, q3ctf2 and q3ctf3.
There produce a Hunk_Alloc failed because there isn't enough memory :(
So it'sn't a final version.


/*
===========================
How to run it
===========================
*/
First : copy the pak0.pk3 in the baseq3 folder, like if you install 
      Quake 3 game on a computer based on UNIX.
Second : configure a network with the XMB menu. ( if it isn't already do) 
Third : configure the q3config.cfg, the cvar (means Console Variable) net_wificonnection defined the issue
of the wifi connections you have create:
    if you enter : seta net_wificonnection "0" : it will see the first connection
    if you enter : seta net_wificonnection "1" : it will see the second connection
    and so on ...
       
Then launch the server, and enjoy it ! 

/*
===========================
Technical knowledge
===========================
*/

Quake 3 memory is divided in 3 types :
      -smallzone : small allocation to avoid fragment the main zone : for cvar and command strings 
      -zone : small strings and structures
      -Hunk : big structures

You can see the memory patitioning with meminfo command e.g.

rconPassword pass
rcon meminfo
17102624 bytes total hunk	<= size of hunk allocated
 2726297 bytes total zone	<= size of zone allocated

 8875680 low mark
 8875680 low permanent
 8519844 low tempHighwater

       0 high mark
       0 high permanent
 2394612 high tempHighwater

 8875680 total hunk in use
 2394612 unused highwater

 2549636 bytes in 75 zone blocks
               0 bytes in dynamic botlib
               0 bytes in dynamic renderer
         2549636 bytes in dynamic other
           14876 bytes in small Zone memory <= size of smallzone allocated

For more information qcommon/common.c

For modify : 
  the size of smallzone, it's defined in qcommon/common.c 1488 lign, I set this variable to 22ko
  the size of zone, redefined com_zoneBytes e.g : seta com_zoneBytes "3145728"
  the size of hunk, redefined com_hunkBytes e.g : seta com_hunkBytes "16777216"
    I have add a special cvar for hunk : com_useallrestforHunk
    if it equal to 1, the Hunk will use all rest of the memory, so the value of com_hunkBytes is useless.

To Conclude, if com_useallrestforHunk is equal to one, memory partitionning will be : 
    - Allocate 22ko for smallzone
    - Allocate com_zoneBytes if it is superior to minimum ( 1Mo)
    - Allocate all rest of memory for the Hunk

I think, I haven't find the optimal solution for the allocate of memory space.


/*
===========================
Credits
===========================
*/
emvivre : coder
Vash : graphist ( icon of EBOOT.PBP )


/*
===========================
Thanks too
===========================
*/
ps2dev.org community for the famous PSPSDK
IDsoftware for having coded and release Quake 3 source code.


And Merry Christmas !