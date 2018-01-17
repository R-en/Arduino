#include <SoftwareSerial.h>

#define SoftrxPin 3
#define SofttxPin 4

SoftwareSerial GPS = SoftwareSerial(SoftrxPin, SofttxPin);

void setup( )
	{
	Serial.begin( 9600 );
	GPS.begin( 9600 );
	//Serial.println( "Press Enter to start" );
	//while ( Serial.read( ) == -1 )
	//	;
	}

void loop( )
	{
	char c;

	while ( ( c = GPS.read( ) ) == -1 )
		;
	Serial.write( c );
	}
	
