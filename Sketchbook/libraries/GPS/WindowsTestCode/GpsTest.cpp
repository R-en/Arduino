// GpsTest.cpp  Arduino GPS driver test for Windows.  Reads NMEA
// records from stdin as if they were coming from the actual device.
// Nicole Hamilton, nicole@nicolehamilton.com, Copyright 2013.

// Compile this as cl /DDebug /Zi GpsTest.cpp ../GPS.cpp
// Run as GpsTest < GpsLog.txt

// To pause GpsTest at the start to allow attaching by a debugger,
// give it any argument, e.g., GpsTest foo < GpsLog.txt


#include <windows.h>
#include "../GPS.h"


GPS   Gps;


inline HANDLE OpenKeyboard( )
   {
   return CreateFile( "CONIN$", ( GENERIC_READ | GENERIC_WRITE ),
         ( FILE_SHARE_READ | FILE_SHARE_WRITE ), NULL,
         OPEN_EXISTING, 0, NULL );
   }


void Pause( )
   {
   HANDLE Keyboard, Stderr;
   char buffer;
   char pressEnter[] = "Press Enter to continue ...";
   DWORD bytes;

   Keyboard = OpenKeyboard( );
   Stderr = GetStdHandle( STD_ERROR_HANDLE );

   WriteFile( Stderr, ( char * )pressEnter,
      sizeof( pressEnter ) - 1, &bytes, NULL );
   ReadFile( Keyboard, ( char * )( void * )&buffer,
      sizeof( buffer ), &bytes, NULL );

   CloseHandle( Keyboard );
   CloseHandle( Stderr );
   }


void main( int argc, char **argv )
   {
   if ( argc > 1 )
      // Any argument causes it to pause to allow attaching
      // a debugger.
      Pause( );

   Gps.Initialize( );

   while ( !feof( stdin ) )
      {
      if ( Gps.PollForNewMessage( ) )
         Gps.PrintStatus( );
      }
   }
