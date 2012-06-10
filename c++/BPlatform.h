/**
 * Define platform specific code here.
 */

#ifndef __BPLATFORM_H__
#define __BPLATFORM_H__

// #ifndef WIN32
// #define WIN32
// #endif
// #ifndef LINUX
// #define LINUX
// #endif

// define platform specific types
#ifdef WIN32
	#include <windows.h>
#else
	#define DWORD unsigned long
	#define BOOL char
	#define true 1
	#define false 0
	#define LPCTSTR char *
#endif

// define the library export flag
#ifdef WIN32
#define export extern "C" __declspec( dllexport )
#else
#define export extern "C"
#endif

// define standard ANSI functions
#ifndef WIN32
int abs( int var );
DWORD GetTickCount( );
void Sleep( int );
#define stricmp strcasecmp
#define strcmpi strcasecmp
#endif

#ifdef LINUX
char *strupr( const char * );
#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif
#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif
#endif

// define socket stuff
#ifndef WIN32
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket( s ) shutdown( s, SHUT_RDWR )
#else
#define ThreadId DWORD
#endif

#endif
