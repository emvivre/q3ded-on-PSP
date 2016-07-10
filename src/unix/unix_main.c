/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
#include <psppower.h>
#include <pspwlan.h>
#include <psputility_netparam.h>
#include <pspnet_apctl.h>
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <stdio.h>
//#include <string.h>
#include <malloc.h>



#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
//#include <sys/ipc.h>
//#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
//#include <sys/mman.h>
#include <errno.h>
#include <setjmp.h>
//#include <dlfcn.h>


// FIXME TTimo should we gard this? most *nix system should comply?
//#include <termios.h>

#define printf pspDebugScreenPrintf

#include "../game/q_shared.h"
#include "../qcommon/qcommon.h"

#include "linux_local.h" // bk001204

extern jmp_buf abortframe;

/* Define the module info section */
PSP_MODULE_INFO("Quake 3 server", 0x1000, 1, 0);
//PSP_MAIN_THREAD_PARAMS(0x18,0x100000,THREAD_ATTR_USER|THREAD_ATTR_VFPU);
PSP_MAIN_THREAD_ATTR(0);

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
    sceKernelExitGame();

        return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
    int cbid;
    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();

        return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, THREAD_ATTR_USER, 0);
    if (thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}


#ifdef PSP_DYNAMIC_LINKING
#error dynamic not available

SceUID load_module(const char *path, int flags, int type)
{
        SceKernelLMOption option;
        SceUID mpid;

        /* If the type is 0, then load the module in the kernel partition, otherwise load it
           in the user partition. */
        if (type == 0) {
                mpid = 1;
        } else {
                mpid = 2;
        }

        memset(&option, 0, sizeof(option));
        option.size = sizeof(option);
        option.mpidtext = mpid;
        option.mpiddata = mpid;
        option.position = 0;
        option.access = 1;

        return sceKernelLoadModule(path, flags, type > 0 ? &option : NULL);
}
#endif


// Structure containing functions exported from refresh DLL
//refexport_t re;

unsigned  sys_frame_time;

uid_t saved_euid;
qboolean stdin_active = qtrue;

// =============================================================
// tty console variables
// =============================================================

// enable/disabled tty input mode
// NOTE TTimo this is used during startup, cannot be changed during run
//static cvar_t *ttycon = NULL;
// general flag to tell about tty console mode
//static qboolean ttycon_on = qfalse;
// when printing general stuff to stdout stderr (Sys_Printf)
//   we need to disable the tty console stuff
// this increments so we can recursively disable
//static int ttycon_hide = 0;
// some key codes that the terminal may be using
// TTimo NOTE: I'm not sure how relevant this is//
//static int tty_erase;
//static int tty_eof;

//static struct termios tty_tc;

//static field_t tty_con;

// history
// NOTE TTimo this is a bit duplicate of the graphical console history
//   but it's safer and faster to write our own here
//#define TTY_HISTORY 32
//static field_t ttyEditLines[TTY_HISTORY];
//static int hist_current = -1, hist_count = 0;

// =======================================================================
// General routines
// =======================================================================

// bk001207 
#define MEM_THRESHOLD 96*1024*1024

/*
==================
Sys_LowPhysicalMemory()
==================
*/
qboolean Sys_LowPhysicalMemory() {
  //MEMORYSTATUS stat;
  //GlobalMemoryStatus (&stat);
  //return (stat.dwTotalPhys <= MEM_THRESHOLD) ? qtrue : qfalse;
  return qfalse; // bk001207 - FIXME
}

/*
==================
Sys_FunctionCmp
==================
*/
int Sys_FunctionCmp(void *f1, void *f2) {
  return qtrue;
}

/*
==================
Sys_FunctionCheckSum
==================
*/
int Sys_FunctionCheckSum(void *f1) {
  return 0;
}

/*
==================
Sys_MonkeyShouldBeSpanked
==================
*/
int Sys_MonkeyShouldBeSpanked( void ) {
  return 0;
}

void Sys_BeginProfiling( void ) {
}

/*
=================
Sys_In_Restart_f

Restart the input subsystem
=================
*/
void Sys_In_Restart_f( void ) 
{
  IN_Shutdown();
  IN_Init();
}


// never exit without calling this, or your terminal will be left in a pretty bad state
void Sys_ConsoleInputShutdown()
{
    Com_Printf("Shutdown tty console\n");
}



void Hist_Add(field_t *field)
{

}

field_t *Hist_Prev()
{
	return NULL;
}

field_t *Hist_Next()
{
	return NULL;
}



// =============================================================
// general sys routines
// =============================================================

#if 0
// NOTE TTimo this is not used .. looks interesting though? protection against buffer overflow kind of stuff?
void Sys_Printf (char *fmt, ...)
{
  va_list   argptr;
  char    text[1024];
  unsigned char   *p;

  va_start (argptr,fmt);
  vsprintf (text,fmt,argptr);
  va_end (argptr);

  if (strlen(text) > sizeof(text))
    Sys_Error("memory overwrite in Sys_Printf");

  for (p = (unsigned char *)text; *p; p++)
  {
    *p &= 0x7f;
    if ((*p > 128 || *p < 32) && *p != 10 && *p != 13 && *p != 9)
      printf("[%02x]", *p);
    else
      printf("%c",*p);
    
  }
}
#endif

// single exit point (regular exit or in case of signal fault)
void Sys_Exit( int ex ) {
		
	Sys_ConsoleInputShutdown();
	
#ifdef NDEBUG // regular behavior

  // We can't do this 
  //  as long as GL DLL's keep installing with atexit...
  //exit(ex);
  _exit(ex);
#else
  
  void shutdown_server(); 
  shutdown_server();
  // Give me a backtrace on error exits.
  //assert( ex == 0 );
  //exit(ex);
#endif
}


void Sys_Quit (void) {
  CL_Shutdown ();
  //fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);
  Sys_Exit(0);
}

void Sys_Init(void)
{
  Cmd_AddCommand ("in_restart", Sys_In_Restart_f);

  Cvar_Set( "arch", "mips PSP" );


  Cvar_Set( "username", Sys_GetCurrentUser() );

  IN_Init();

}

void  Sys_Error( const char *error, ...)
{ 
  va_list     argptr;
  char        string[1024];

  // change stdin to non blocking
  // NOTE TTimo not sure how well that goes with tty console mode
  //fcntl (0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);

  CL_Shutdown ();

  va_start (argptr,error);
  vsprintf (string,error,argptr);
  va_end (argptr);
  printf("Sys_Error: %s\n", string);

  Sys_Exit( 1 ); // bk010104 - use single exit point.
} 

void Sys_Warn (char *warning, ...)
{ 
  va_list     argptr;
  char        string[1024];

  va_start (argptr,warning);
  vsprintf (string,warning,argptr);
  va_end (argptr);

  printf("Warning: %s", string);

}

/*
============
Sys_FileTime

returns -1 if not present
============
*/
int Sys_FileTime (char *path)
{
  struct  stat  buf;

  if (stat (path,&buf) == -1)
    return -1;

  return buf.st_mtime;
}

void floating_point_exception_handler(int whatever)
{

}

// initialize the console input (tty mode if wanted and possible)
void Sys_ConsoleInputInit()
{

}

char *Sys_ConsoleInput(void)
{
    return NULL;
}

/*****************************************************************************/


/*
=================
Sys_UnloadDll

=================
*/
void Sys_UnloadDll( void *dllHandle ) {
 // bk001206 - verbose error reporting
	//const char* err; // rb010123 - now const
#ifdef PSP_DYNAMIC_LINKING
#error dynamic not available


  if ( !dllHandle )
  {
    Com_Printf("Sys_UnloadDll(NULL)\n");
    return;
  }

  sceKernelUnloadModule(dllHandle);
#endif
}






/*
=================
Sys_LoadDll

Used to load a development dll instead of a virtual machine
TTimo:
changed the load procedure to match VFS logic, and allow developer use
#1 look down current path
#2 look in fs_homepath
#3 look in fs_basepath
=================
*/
extern char   *FS_BuildOSPath( const char *base, const char *game, const char *qpath );

void *Sys_LoadDll( const char *name, char *fqpath ,
                   int (**entryPoint)(int, ...),
                   int (*systemcalls)(int, ...) ) 
{
#ifdef PSP_DYNAMIC_LINKING
#error dynamic not available
	
	//void *libHandle;
	SceUID libHandle;

  void  (*dllEntry)( int (*syscallptr)(int, ...) );
  char  curpath[MAX_OSPATH];
  char  fname[MAX_OSPATH];
  char  *basepath;
  char  *homepath;
  char  *pwdpath;
  char  *gamedir;
  char  *fn;
  const char*  err = NULL;
	
	*fqpath = 0;

  // bk001206 - let's have some paranoia
  assert( name );

  getcwd(curpath, sizeof(curpath));
#if defined __i386__
  snprintf (fname, sizeof(fname), "%si386.so", name);
#elif defined __powerpc__   //rcg010207 - PPC support.
  snprintf (fname, sizeof(fname), "%sppc.so", name);
#elif defined __axp__
  snprintf (fname, sizeof(fname), "%saxp.so", name);
#elif defined __mips__
  snprintf (fname, sizeof(fname), "%smips.so", name);
#else
#error Unknown arch
#endif

// bk001129 - was RTLD_LAZY 
#define Q_RTLD    RTLD_NOW
#define LOAD_IN_KERNEL 0
#define LOAD_IN_USER   1

  pwdpath = Sys_Cwd();
  basepath = Cvar_VariableString( "fs_basepath" );
  homepath = Cvar_VariableString( "fs_homepath" );
  gamedir = Cvar_VariableString( "fs_game" );

  // pwdpath
  fn = FS_BuildOSPath( pwdpath, gamedir, fname );
  Com_Printf( "Sys_LoadDll(%s)... \n", fn );
  //libHandle = dlopen( fn, Q_RTLD );
  libHandle =  load_module(fn,0,LOAD_IN_USER);

  if ( !libHandle )
  {
    Com_Printf( "Sys_LoadDll(%s) failed:\n\"%s\"\n", fn, dlerror() );
    // fs_homepath
    fn = FS_BuildOSPath( homepath, gamedir, fname );
    Com_Printf( "Sys_LoadDll(%s)... \n", fn );
    libHandle = dlopen( fn, Q_RTLD );

    if ( !libHandle )
    {
      Com_Printf( "Sys_LoadDll(%s) failed:\n\"%s\"\n", fn, dlerror() );
      // fs_basepath
      fn = FS_BuildOSPath( basepath, gamedir, fname );
      Com_Printf( "Sys_LoadDll(%s)... \n", fn );
      libHandle = dlopen( fn, Q_RTLD );

      if ( !libHandle )
      {
#ifndef NDEBUG // bk001206 - in debug abort on failure
        Com_Error ( ERR_FATAL, "Sys_LoadDll(%s) failed dlopen() completely!\n", name  );
#else
        Com_Printf ( "Sys_LoadDll(%s) failed dlopen() completely!\n", name );
#endif
        return NULL;
      } else
        Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn );
    } else
      Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn );
  } else
    Com_Printf ( "Sys_LoadDll(%s): succeeded ...\n", fn ); 

  dllEntry = dlsym( libHandle, "dllEntry" ); 
  *entryPoint = dlsym( libHandle, "vmMain" );
  if ( !*entryPoint || !dllEntry )
  {
    err = dlerror();
#ifndef NDEBUG // bk001206 - in debug abort on failure
    Com_Error ( ERR_FATAL, "Sys_LoadDll(%s) failed dlsym(vmMain):\n\"%s\" !\n", name, err );
#else
    Com_Printf ( "Sys_LoadDll(%s) failed dlsym(vmMain):\n\"%s\" !\n", name, err );
#endif
    dlclose( libHandle );
    err = dlerror();
    if ( err != NULL )
      Com_Printf ( "Sys_LoadDll(%s) failed dlcose:\n\"%s\"\n", name, err );
    return NULL;
  }
  Com_Printf ( "Sys_LoadDll(%s) found **vmMain** at  %p  \n", name, *entryPoint ); // bk001212
  dllEntry( systemcalls );
  Com_Printf ( "Sys_LoadDll(%s) succeeded!\n", name );
  if ( libHandle ) Q_strncpyz ( fqpath , fn , MAX_QPATH ) ;		// added 7/20/02 by T.Ray
  return libHandle;
	
	return NULL;	
#else
	return NULL;
#endif
}


/*
========================================================================

BACKGROUND FILE STREAMING

========================================================================
*/

#if 1

void Sys_InitStreamThread( void ) {
}

void Sys_ShutdownStreamThread( void ) {
}

void Sys_BeginStreamedFile( fileHandle_t f, int readAhead ) {
}

void Sys_EndStreamedFile( fileHandle_t f ) {
}

int Sys_StreamedRead( void *buffer, int size, int count, fileHandle_t f ) {
  return FS_Read( buffer, size * count, f );
}

void Sys_StreamSeek( fileHandle_t f, int offset, int origin ) {
  FS_Seek( f, offset, origin );
}

#else

typedef struct
{
  fileHandle_t file;
  byte  *buffer;
  qboolean  eof;
  int   bufferSize;
  int   streamPosition; // next byte to be returned by Sys_StreamRead
  int   threadPosition; // next byte to be read from file
} streamState_t;

streamState_t stream;

/*
===============
Sys_StreamThread

A thread will be sitting in this loop forever
================
*/
void Sys_StreamThread( void ) 
{
  int   buffer;
  int   count;
  int   readCount;
  int   bufferPoint;
  int   r;

  // if there is any space left in the buffer, fill it up
  if ( !stream.eof )
  {
    count = stream.bufferSize - (stream.threadPosition - stream.streamPosition);
    if ( count )
    {
      bufferPoint = stream.threadPosition % stream.bufferSize;
      buffer = stream.bufferSize - bufferPoint;
      readCount = buffer < count ? buffer : count;
      r = FS_Read ( stream.buffer + bufferPoint, readCount, stream.file );
      stream.threadPosition += r;

      if ( r != readCount )
        stream.eof = qtrue;
    }
  }
}

/*
===============
Sys_InitStreamThread

================
*/
void Sys_InitStreamThread( void ) 
{
}

/*
===============
Sys_ShutdownStreamThread

================
*/
void Sys_ShutdownStreamThread( void ) 
{
}


/*
===============
Sys_BeginStreamedFile

================
*/
void Sys_BeginStreamedFile( fileHandle_t f, int readAhead ) 
{
  if ( stream.file )
  {
    Com_Error( ERR_FATAL, "Sys_BeginStreamedFile: unclosed stream");
  }

  stream.file = f;
  stream.buffer = Z_Malloc( readAhead );
  stream.bufferSize = readAhead;
  stream.streamPosition = 0;
  stream.threadPosition = 0;
  stream.eof = qfalse;
}

/*
===============
Sys_EndStreamedFile

================
*/
void Sys_EndStreamedFile( fileHandle_t f ) 
{
  if ( f != stream.file )
  {
    Com_Error( ERR_FATAL, "Sys_EndStreamedFile: wrong file");
  }

  stream.file = 0;
  Z_Free( stream.buffer );
}


/*
===============
Sys_StreamedRead

================
*/
int Sys_StreamedRead( void *buffer, int size, int count, fileHandle_t f ) 
{
  int   available;
  int   remaining;
  int   sleepCount;
  int   copy;
  int   bufferCount;
  int   bufferPoint;
  byte  *dest;

  dest = (byte *)buffer;
  remaining = size * count;

  if ( remaining <= 0 )
  {
    Com_Error( ERR_FATAL, "Streamed read with non-positive size" );
  }

  sleepCount = 0;
  while ( remaining > 0 )
  {
    available = stream.threadPosition - stream.streamPosition;
    if ( !available )
    {
      if (stream.eof)
        break;
      Sys_StreamThread();
      continue;
    }

    bufferPoint = stream.streamPosition % stream.bufferSize;
    bufferCount = stream.bufferSize - bufferPoint;

    copy = available < bufferCount ? available : bufferCount;
    if ( copy > remaining )
    {
      copy = remaining;
    }
    memcpy( dest, stream.buffer + bufferPoint, copy );
    stream.streamPosition += copy;
    dest += copy;
    remaining -= copy;
  }

  return(count * size - remaining) / size;
}

/*
===============
Sys_StreamSeek

================
*/
void Sys_StreamSeek( fileHandle_t f, int offset, int origin ) {
  // clear to that point
  FS_Seek( f, offset, origin );
  stream.streamPosition = 0;
  stream.threadPosition = 0;
  stream.eof = qfalse;
}

#endif

/*
========================================================================

EVENT LOOP

========================================================================
*/

// bk000306: upped this from 64
#define	MAX_QUED_EVENTS		256
#define	MASK_QUED_EVENTS	( MAX_QUED_EVENTS - 1 )

sysEvent_t  eventQue[MAX_QUED_EVENTS];
// bk000306: initialize
int   eventHead = 0;
int             eventTail = 0;
byte    sys_packetReceived[MAX_MSGLEN];
//byte    sys_packetReceived[16384];

/*
================
Sys_QueEvent

A time of 0 will get the current time
Ptr should either be null, or point to a block of data that can
be freed by the game later.
================
*/
void Sys_QueEvent( int time, sysEventType_t type, int value, int value2, int ptrLength, void *ptr ) {
  sysEvent_t  *ev;

  ev = &eventQue[ eventHead & MASK_QUED_EVENTS ];

  // bk000305 - was missing
  if ( eventHead - eventTail >= MAX_QUED_EVENTS )
  {
    Com_Printf("Sys_QueEvent: overflow\n");
    // we are discarding an event, but don't leak memory
    if ( ev->evPtr )
    {
      Z_Free( ev->evPtr );
    }
    eventTail++;
  }

  eventHead++;

  if ( time == 0 )
  {
    time = Sys_Milliseconds();
  }

  ev->evTime = time;
  ev->evType = type;
  ev->evValue = value;
  ev->evValue2 = value2;
  ev->evPtrLength = ptrLength;
  ev->evPtr = ptr;
}

/*
================
Sys_GetEvent

================
*/
sysEvent_t Sys_GetEvent( void ) {
  sysEvent_t  ev;
  char    *s;
  msg_t   netmsg;
  netadr_t  adr;

  // return if we have data
  if ( eventHead > eventTail )
  {
    eventTail++;
    return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
  }

  // pump the message loop
  // in vga this calls KBD_Update, under X, it calls GetEvent
  Sys_SendKeyEvents ();

  // check for console commands
  s = Sys_ConsoleInput();
  if ( s )
  {
    char  *b;
    int   len;

    len = strlen( s ) + 1;
    b = Z_Malloc( len );
    strcpy( b, s );
    Sys_QueEvent( 0, SE_CONSOLE, 0, 0, len, b );
  }

  // check for other input devices
  IN_Frame();

  // check for network packets
  MSG_Init( &netmsg, sys_packetReceived, sizeof( sys_packetReceived ) );
  if ( Sys_GetPacket ( &adr, &netmsg ) )
  {
    netadr_t    *buf;
    int       len;

    // copy out to a seperate buffer for qeueing
    len = sizeof( netadr_t ) + netmsg.cursize;
    buf = Z_Malloc( len );
    *buf = adr;
    memcpy( buf+1, netmsg.data, netmsg.cursize );
    Sys_QueEvent( 0, SE_PACKET, 0, 0, len, buf );
  }

  // return if we have data
  if ( eventHead > eventTail )
  {
    eventTail++;
    return eventQue[ ( eventTail - 1 ) & MASK_QUED_EVENTS ];
  }

  // create an empty event to return

  memset( &ev, 0, sizeof( ev ) );
  ev.evTime = Sys_Milliseconds();

  return ev;
}

/*****************************************************************************/

qboolean Sys_CheckCD( void ) {
  return qtrue;
}

void Sys_AppActivate (void)
{
}

char *Sys_GetClipboardData(void)
{
  return NULL;
}

void  Sys_Print( const char *msg )
{
  printf("%s",msg);

}


void    Sys_ConfigureFPU() { // bk001213 - divide by zero
#ifdef __linux__
#ifdef __i386
#ifndef NDEBUG

  // bk0101022 - enable FPE's in debug mode
  static int fpu_word = _FPU_DEFAULT & ~(_FPU_MASK_ZM | _FPU_MASK_IM);
  int current = 0;
  _FPU_GETCW(current);
  if ( current!=fpu_word)
  {
#if 0
    Com_Printf("FPU Control 0x%x (was 0x%x)\n", fpu_word, current );
    _FPU_SETCW( fpu_word );
    _FPU_GETCW( current );
    assert(fpu_word==current);
#endif
  }
#else // NDEBUG
  static int fpu_word = _FPU_DEFAULT;
  _FPU_SETCW( fpu_word );
#endif // NDEBUG
#endif // __i386 
#endif // __linux
}

void Sys_PrintBinVersion( const char* name ) {
  char* date = __DATE__;
  char* time = __TIME__;
  char* sep = "==============================================================";
  printf( "\n\n%s\n", sep );
#ifdef DEDICATED
  printf( "Linux Quake3 Dedicated Server [%s %s]\n", date, time );  
#else
  printf( "Linux Quake3 Full Executable  [%s %s]\n", date, time );  
#endif
  printf( " local install: %s\n", name );
  printf( "%s\n\n", sep );
}

void Sys_ParseArgs( int argc, char* argv[] ) {

  if ( argc==2 )
  {
    if ( (!strcmp( argv[1], "--version" ))
         || ( !strcmp( argv[1], "-v" )) )
    {
      Sys_PrintBinVersion( argv[0] );
      Sys_Exit(0);
    }
  }
}

void shutdown_server(){
	printf("Server shuting down\n");
	pspSdkInetTerm();
	sceKernelExitDeleteThread(0);
}


void * heap_top;
#define DEFAULT_ALIGNMENT       16
 
#ifndef ALIGNDW  //align down
#define ALIGNDW(x, align) (((x)-((align)-1))&~((align)-1))
#endif


//#include "../client/client.h"
//extern clientStatic_t cls;

int quake3 (SceSize args, void* argp)
{
  

  char  cmdline = 0 ;
  int rc;
  int state,statelast;

  void Sys_SetDefaultCDPath(const char *path);

  char* date = __DATE__;
  char* time = __TIME__;
  char* sep = " ==================================================================";  
  int num;
  cvar_t * ctemp; 

  pspSdkInetInit();
 
  printf( "\n"); 
  printf( "%s\n", sep );
  printf( "\t\tPSP Quake3 Dedicated Server [%s %s]\n", date, time );  
  printf( "%s\n", sep );
	
  Sys_SetDefaultCDPath(argp);

  #if 0  
  int   len, i;
  // merge the command line, this is kinda silly
  for (len = 1, i = 1; i < argc; i++)
    len += strlen(argv[i]) + 1;
  cmdline = (char *) malloc((size_t) len);
  *cmdline = 0;
  for (i = 1; i < argc; i++)
  {
    if (i > 1)
      strcat(cmdline, " ");
    strcat(cmdline, argv[i]);
  }
  #endif
  
  // bk000306 - clear queues
  memset( &eventQue[0], 0, MAX_QUED_EVENTS*sizeof(sysEvent_t) ); 
  memset( &sys_packetReceived[0], 0, MAX_MSGLEN*sizeof(byte) );

  Com_Init(&cmdline);

    
  if (sceWlanGetSwitchState() != 1){
	  printf("Please enable the WLAN the next time !\n");
	  longjmp(abortframe,-1);
  }

  ctemp = Cvar_Get( "net_wificonnection", "0", 0 );
  num = ctemp->integer;
  if (sceUtilityCheckNetParam(num)){
	  printf("No Network Parameter found .\n"
		 "Please create a configuration network before.\n");
	  longjmp(abortframe,-1);
  }
 
  netData buff;

  //sceNetApctlDisconnect();
  
  sceUtilityGetNetParam(num, PSP_NETPARAM_SSID ,&buff);
  rc = sceNetApctlConnect(num);
  if(rc != 0) {
	  Com_Printf("Error sceNetApctlConnect(%d)",num );
	  longjmp(abortframe,-1);
  }

  printf("Connecting no %d to %s",num,buff.asString);  
  statelast = 0;
  do {
	  rc = sceNetApctlGetState(&state);
	  if(rc != 0) {
		  Com_Printf("\nError sceNetApctlGetState()\n");
		  shutdown_server();
	  }
	  if (statelast!=state){
		  printf(".");
		  statelast=state;
	  }

	  sceKernelDelayThread (50 * 100); 
} while(state != 4);
  printf("ok\n");

  NET_Init();

  ctemp =  Cvar_Get( "MAPTOLOAD", "q3dm1", 0 ); 
  Cbuf_AddText(va("map %s",ctemp->string));
  
  while (1)
  {
    sceKernelDelayThread(50 * 100); //horrible ! 
	  Com_Frame ();  
   }
  sceKernelExitDeleteThread(0);
  //shutdown();
  return 0;    
}

int main(int argc, char** argv) {
        pspSdkLoadInetModules();
        
	//for debug
	//pspDebugInstallKprintfHandler(NULL);
	
        SceUID thid;
	pspDebugScreenInit();
	pspDebugScreenClear();
	
	SetupCallbacks();
	
        thid = sceKernelCreateThread("quake3", quake3 , 0x18, 0x60000, PSP_THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU,0);
        if(thid >= 0) {
                sceKernelStartThread(thid,  strlen(argv[0])+1, argv[0]);
        }
	
        sceKernelExitDeleteThread(0);
	return 0;
}
