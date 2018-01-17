#include <dGPS.h>

#include <dGPS.h>

// GPS.ino  Arduino GPS NMEA sample sketch
// Nicole Hamilton, nicole@nicolehamilton.com, Copyright 2013.


#include <SoftwareSerial.h>
#include <GPS.h>

GPS    Gps;

void setup( )
   {
   Serial.begin( 115200 );
   Gps.Initialize( );
   Serial.println( "Press any key to update status." );
   }

void loop( )
   {
   if ( Gps.PollForNewMessage( ) )
      {
      // Uncomment the next line to print each new message.
      // Serial.println( Gps.Message.Buffer );
      }

   if ( Serial.read( ) != -1 )
      {
      // Print the latest status every time a key is pressed.
      Gps.PrintStatus( );
      Serial.println( "Press any key to update status." );
      }
   }
