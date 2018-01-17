// GPS.cpp	Arduino GPS NMEA message driver
// Nicole Hamilton, nicole@nicolehamilton.com, Copyright 2013.


// This driver parses NMEA messages from the GPS module and collects
// status in a GPS class.
//
//	The following routines are defined here:
//
// public members:
//
//														// Simple constructor takes pin
//														// assignments for the GPS device
//														// that are passed through to the
//														// SoftwareSerial driver.
//		GPS( uint8_t rxPin = SoftRxPin, uint8_t txPin = SoftTxPin );
//
//		bool	PollForNewMessage( );			// Returns true if a valid new
//														// message was received and
//														// processed.  Only one message
//														// is processed to ensure that
//														// the caller never misses any.
//
//		void	Initialize( ),						// Zero out all status fields and
//														// open the GpsSerial SoftwareSerial
//														// port at 9600 baud.
//				PrintStatus( );					// Dump basic GPS status to Serial
//														// out.
//
//
//	private:
//
//		bool	ParseGPGGA( ),						// Parse a $GPGGA message.
//				ParseGPGSA( ),						// Parse a $GPGSA message.
//				ParseGPGSV( ),						// Parse a $GPGSV message.
//				ParseGPRMC( ),						// Parse a $GPRMC message.
//				ParseGPVTG( ),						// Parse a $GPVTG message.
//				Parse( ),							// Parse the supported NMEA messages.
//				IsGoodChecksum( );				// Test that the message appears to
//														// be properly formatted with a good
//														// checksum.
//
//	Other helpful routines:
//
//		int	IntegerPortion( double d ),	// sprintf on Arduino does not
//														// support %f items, so these are
//														// workaround routines to extract
//														// the integer portion and the
//														// number of thousandths as
														// integers.
//				Thousandths( double d );


// References for NMEA messages:
//
// 1. Monte Variakojis, "NMEA Parser Design", November 1, 2002,
//		http://www.visualgps.net/whitepapers/nmeaparser/nmeaparserdesign.html
//
// 2. Ger Rietman, "How to calculate the NMEA checksum", September 25, 2008,
//		http://rietman.wordpress.com/2008/09/25/how-to-calculate-the-nmea-checksum/
//
// 3. "GPS - NMEA sentence information", http://aprs.gids.nl/nmea/


#include	"GPS.h"									// Double quotes on the include mean
														// look first in the same directory
														// as this .cpp file.


//
// Routines for checking that a NMEA sentence is valid and for
// parsing common NMEA tokens.
//


char	HexChars[]	=	"0123456789ABCDEF";

bool GPS::IsGoodChecksum( )
	{
	// Basic message format is $GP....*xx

	// The checksum is calculated on all the chars starting AFTER the DollarSign
	// and BEFORE the Asterisk.

	if ( Message.p - Message.Buffer > MinNmeaMessageLength &&
			Message.Buffer[ 0 ] == DollarSign &&
			Message.p[ -1 ] == NewLine &&
			Message.p[ -2 ] == CarriageReturn &&
			Message.p[ -5 ] == Asterisk )
		{
		unsigned char sum;
		char *q;

		for ( sum = 0, q = Message.Buffer + 1;  q < Message.p - 5; )
			sum ^= *( unsigned char *)q++;
		if ( Message.p[ -4 ] == HexChars[ sum >> 4 ] &&
				Message.p[ -3 ] == HexChars [ sum & 0xf ] )
			return true;
		}

	return false;
	}


char *SkipToNext( char *p )
	{
	// Scan forward to the comma, then over it.
	while ( *p != Comma && *p != Asterisk && *p )
		p++;
	if ( *p == Comma )
		p++;
	return p;
	}


char *FindModeChar( char *p, char *acceptable, char *result )
	{
	char c = *p;
	if ( c != Asterisk && c )
		{
		// Not end of sentence
		if ( c != Comma )
			{
			while ( c != *acceptable && *acceptable )
				acceptable++;
			*result = *acceptable ? c : 0;
			}
		p = SkipToNext( p );
		}
	return p;
	}


char *FindFloat( char *p, double *f )
	{
	if ( *p != Asterisk && *p )
		{
		// Not end of sentence
		if ( *p != Comma )
			// Field isn't blank
			*f = atof( p );
		p = SkipToNext( p );
		}
	return p;
	}


char *FindUnsigned( char *p, unsigned *u )
	{
	if ( *p != Asterisk && *p )
		{
		// Not end of sentence
		if ( *p != Comma )
			{
			// Field isn't blank
			*u = ( unsigned )atoi( p );
			}
		p = SkipToNext( p );
		}
	return p;
	}


unsigned DecimalDigit( char c )
	{
	if ( c >= '0' && c <= '9' )
		return c - '0';
	return 0;
	}


char *FindDate( char *p, UtcDateObj *d )
	{
	if ( *p != Asterisk && *p )
		{
		// Not at end of sentence
		char *nextField = SkipToNext( p );
		if ( *p != Comma )
			{
			// Field isn't blank.  Calculate the length.

			unsigned fieldSize = nextField - p;
			if ( nextField[ -1 ] == Comma )
				fieldSize--;

			if ( fieldSize >= 6 )
				{
				// HHMMSS or HHMMSS.mmm
				d->Day = DecimalDigit( p[ 0 ] )*10 + DecimalDigit( p[ 1 ] );
				d->Month = DecimalDigit( p[ 2 ] )*10 + DecimalDigit( p[ 3 ] );
				d->Year = DecimalDigit( p[ 4 ] )*10 + DecimalDigit( p[ 5 ] );
				d->Year += ( d->Year >= TwoDigitRolloverYear ) ? 1900 : 2000;
				}
			}
		p = nextField;
		}
	return p;
	}


char *FindTime( char *p, UtcTimeObj *t )
	{
	if ( *p != Asterisk && *p )
		{
		// Not end of sentence
		char *nextField = SkipToNext( p );
		if ( *p != Comma )
			{
			// Field isn't blank.  Calculate the length.

			unsigned fieldSize = nextField - p;
			if ( nextField[ -1 ] == Comma )
				fieldSize--;

			if ( fieldSize >= 6 )
				{
				// HHMMSS or HHMMSS.mmm
				t->Hours = DecimalDigit( p[ 0 ] )*10 + DecimalDigit( p[ 1 ] );
				t->Minutes = DecimalDigit( p[ 2 ] )*10 + DecimalDigit( p[ 3 ] );
				t->Seconds = atof( p + 4 );
				}
			}
		p = nextField;
		}
	return p;
	}


char *FindLatitude( char *p, CoordinateObj *lat )
	{
	if ( *p != Asterisk && *p )
		{
		// Not end of sentence
		char *nextField = SkipToNext( p );

		if ( *p != Comma )
			{
			// Field isn't blank.  Calculate the length.

			unsigned fieldSize = nextField - p;
			if ( nextField[ -1 ] == Comma )
				fieldSize--;

			if ( fieldSize >= 6 )
				{
				// DDMM.MMM,<Location>
				double floatMinutes;
				lat->Degrees = DecimalDigit( p[ 0 ] )*10 + DecimalDigit( p[ 1 ] );
				floatMinutes = atof( p + 2 );
				lat->FloatDegrees = ( double )lat->Degrees + floatMinutes / 60.0;
				lat->Minutes = atoi( p + 2 );
				lat->Seconds = 60.0 * ( floatMinutes - ( double )lat->Minutes );
				}
			p = nextField;
			p = FindModeChar( p, ( char * )"NS", &lat->Location );
			if ( lat->Location == 'S' )
				lat->FloatDegrees = -lat->FloatDegrees;
			}
		else
			{
			// Blank field.  Skip it and the location character field that
			// follows.

			p = nextField;
			p = SkipToNext( p );
			}
		}

	return p;
	}


char *FindLongitude( char *p, CoordinateObj *lo )
	{
	if ( *p != Asterisk && *p )
		{
		// Not end of sentence
		char *nextField = SkipToNext( p );

		if ( *p != Comma )
			{
			// Field isn't blank.  Calculate the length.

			unsigned fieldSize = nextField - p;
			if ( nextField[ -1 ] == Comma )
				fieldSize--;

			if ( fieldSize >= 6 )
				{
				// DDMM.MMM,<Location>
				double floatMinutes;
				lo->Degrees = DecimalDigit( p[ 0 ] )*100 +
						DecimalDigit( p[ 1 ] )*10 +
						DecimalDigit( p[ 2 ] );
				floatMinutes = atof( p + 3 );
				lo->FloatDegrees = ( double )lo->Degrees + floatMinutes / 60.0;
				lo->Minutes = atoi( p + 3 );
				lo->Seconds = 60.0 * ( floatMinutes - ( double )lo->Minutes );
				}
			p = nextField;
			p = FindModeChar( p, ( char * )"EW", &lo->Location );
			if ( lo->Location == 'W' )
				lo->FloatDegrees = -lo->FloatDegrees;
			}
		else
			{
			// Blank field.  Skip it and the location character field that
			// follows.

			p = nextField;
			p = SkipToNext( p );
			}
		}

	return p;
	}


//
// Routines to parse the various supported NMEA sentences.
//


bool GPS::ParseGPGGA( )
	{
	// Global positioning system fix data.

	// http://aprs.gids.nl/nmea/#gga
	// http://www.hemispheregps.com/gpsreference/GPGGA.htm

	// $GPGGA,<Time>,<Latitude>,<Longitude>,<FixQuality>,<NumberOfSatellites>,
	// <HorizontalDilutionOfPrecision>,<Altitude>,<HeightOfGeoid>,
	// <TimeSinceLastDgps>,<DgpsRefStationID>*<checksum>

	char *p = Message.Buffer + 7;
	p = FindTime( p, &Time );
	p = FindLatitude( p, &Latitude );
	p = FindLongitude( p, &Longitude );
	p = FindUnsigned( p, &FixQuality );
	p = FindUnsigned( p, &NumberOfSatellitesInView );
	p = FindFloat( p, &HorizontalDilutionOfPrecision );
	p = FindFloat( p, &Altitude );
	p = FindFloat( p, &HeightOfGeoid );
	p = FindFloat( p, &SecondsSinceLastDgpsUpdate );
	p = FindUnsigned( p, &DgpsReferenceStationID );

	return true;
	}


bool GPS::ParseGPGSA( )
	{
	// Satellites used in the position computation.

	// http://aprs.gids.nl/nmea/#gsa
	// http://www.hemispheregps.com/gpsreference/GPGSA.htm

	// $GPGSA,<AcquisitionMode,<PositionMode>,{ <SatelliteID>, }
	// <PositionDilutionOfPrecision>,<HorizontalDilutionOfPrecision>,
	// <VerticalDilutionOfPrecision>*<checksum>

	char *p = Message.Buffer + 7;
	unsigned *u = SatellitesUsedInComputation;

	p = FindModeChar( p, ( char * )"AM", &AcquisitionMode );
	p = FindUnsigned( p, &PositionMode );

	for ( int i = MaxNumberOfSatellites;  i-- && *p && *p != Asterisk; )
		{
		if ( *p != Comma )
			p = FindUnsigned( p, u++ );
		else
			p++;
		}
	NumberOfSatellitesUsedInComputation = u - SatellitesUsedInComputation;

	p = FindFloat( p, &PositionDilutionOfPrecision );
	p = FindFloat( p, &HorizontalDilutionOfPrecision );
	p = FindFloat( p, &VerticalDilutionOfPrecision );

	return true;
	}


bool GPS::ParseGPGSV( )
	{
	// Satellites in view.

	// http://www.hemispheregps.com/gpsreference/GPGSV.htm

	// $GPGSV,<NumberOfMessages>,<MessageNumber>,<NumberOfSatellitesInView>,
	// { <SatelliteNumber>,<Elevation>,<Azimuth>,<SignalToNoiseRatio>, }
	// *<checksum>

	char *p = Message.Buffer + 7;
	SatelliteObj *v;

	p = FindUnsigned( p, &NumberOfGpgsvMessages );
	p = FindUnsigned( p, &LastGpgsvMessageNumber );
	p = FindUnsigned( p, &NumberOfSatellitesInView );
	
	v = SatellitesInView + 4*( LastGpgsvMessageNumber - 1 );

	for ( int i = 4;  i-- && *p && *p != Asterisk; )
		{
		if ( *p != Comma )
			{
			p = FindUnsigned( p, &v->ID );
			p = FindUnsigned( p, &v->Elevation );
			p = FindUnsigned( p, &v->Azimuth );
			p = FindUnsigned( p, &v->SignalToNoiseRatio );
			v++;
			}
		else
			p++;
		}

	return true;
	}


bool GPS::ParseGPRMC( )
	{
	// Recommended minimum specific GPS/Transit data.

	// http://aprs.gids.nl/nmea/#rmc
	// http://www.hemispheregps.com/gpsreference/GPRMC.htm

	// $GPRMC,<Time>,<Validity>,<Latitude>,<Longitude>,<SpeedInKnots>,
	// <TrueCourse>,<Date>,<MagneticVariation>,<VariationSense>,
	// <NavigationStatus>,*<checksum>

	char *p = Message.Buffer + 7;

	// Serial.println( "Entering ParseGPRMC" );

	p = FindTime( p, &Time );
	p = FindModeChar( p, ( char * )"AV", &Validity );
	p = FindLatitude( p, &Latitude );
	p = FindLongitude( p, &Longitude );
	p = FindFloat( p, &SpeedInKnots );
	p = FindFloat( p, &TrueCourse );
	p = FindDate( p, &Date );
	p = FindFloat( p, &MagneticVariation );
	p = FindModeChar( p, ( char * )"NADPRFEMS", &PositionFix );
	p = FindModeChar( p, ( char * )"SCUV", &NavigationStatus );

	// PrintStatus( );

	return true;
	}


bool GPS::ParseGPVTG( )
	{
	// True course and ground speed.

	// http://aprs.gids.nl/nmea/#vtg
	// http://www.hemispheregps.com/gpsreference/GPVTG.htm

	// $GPVTG,<TrueCourse>,<TrueCourseIndicator>,
	// <MagneticCourse>,<MagneticCourseIndicator>,
	// <SpeedInKnots>,<SpeedInKnotsIndicator>,
	// <SpeedInKmh>,<SpeedInKmhIndicator>,
	// <PositionFix>,*<checksum>

	char *p = Message.Buffer + 7;

	p = FindFloat( p, &TrueCourse );
	p = FindModeChar( p, ( char * )"T", &TrueCourseIndicator );
	p = FindFloat( p, &MagneticCourse );
	p = FindModeChar( p, ( char * )"M", &MagneticCourseIndicator );
	p = FindFloat( p, &SpeedInKnots );
	p = FindModeChar( p, ( char * )"N", &SpeedInKnotsIndicator );
	p = FindFloat( p, &SpeedInKmh );
	p = FindModeChar( p, ( char * )"K", &SpeedInKmhIndicator );
	p = FindModeChar( p, ( char * )"NADPRFEMS", &PositionFix );

	return true;
	}


bool GPS::Parse( )
	{
	// Parse the new NMEA sentence.

	/*
	if ( Message.Buffer[ 0 ] == DollarSign &&
			Message.Buffer[ 1 ] == 'G' &&
			Message.Buffer[ 2 ] == 'P' &&
			Message.Buffer[ 3 ] == 'R' &&
			Message.Buffer[ 4 ] == 'M' &&
			Message.Buffer[ 5 ] == 'C' )
		Serial.println( "Got a GPRMC sentence." );
	*/

	if ( IsGoodChecksum( ) &&
			Message.Buffer[ 1 ] == 'G' && Message.Buffer[ 2 ] == 'P' &&
			Message.Buffer[ 6 ] == Comma )
		{
		// Possible message formats are $GPGGA, $GPGSA, $GPGSV, $GPRMC, $GPVTG
		switch ( Message.Buffer[ 3 ] )
			{
			case 'G':
				switch ( Message.Buffer[ 4 ] )
					{
					case 'G':
						if ( Message.Buffer[ 5 ] == 'A' )
							return ParseGPGGA( );
						break;
					case 'S':
						if ( Message.Buffer[ 5 ] == 'A' )
							return ParseGPGSA( );
						else
							if (Message.Buffer[ 5 ] == 'V' )
								return ParseGPGSV( );
					}
				break;

			case 'R':
				if ( Message.Buffer[ 4 ] == 'M' && Message.Buffer[ 5 ] == 'C' )
					return ParseGPRMC( );
				break;

			case 'V':
				if ( Message.Buffer[ 4 ] == 'T' && Message.Buffer[ 5 ] == 'G' )
					return ParseGPVTG( );
			}
		}

	// Discard anything else as a garbled or unsupported sentence.
	*( Message.p = Message.Buffer ) = 0;

	return false;
	}


//
// Initialize (zero out) the GPS and start listening
// on the serial port.
//


void GPS::Initialize( )
	{
	// Zero out all the data fields.

	*( Message.p = Message.Buffer ) = 0;

	Time.Hours = Time.Minutes = 0;
	Time.Seconds = 0.0;
	Date.Month = Date.Day = Date.Year = 0;

	Latitude.Degrees = Longitude.Degrees = 0;
	Latitude.Minutes = Longitude.Minutes = 0.0;
	Latitude.Location = Longitude.Location = 0;

	Altitude = TrueCourse = MagneticCourse = SpeedInKnots =
		SpeedInKmh = MagneticVariation = 0.0;

	NumberOfSatellitesInView = NumberOfSatellitesUsedInComputation = 0;

	FixQuality = PositionMode = 0;

	HorizontalDilutionOfPrecision = VerticalDilutionOfPrecision =
		PositionDilutionOfPrecision = HeightOfGeoid =
		SecondsSinceLastDgpsUpdate = 0.0;

	DgpsReferenceStationID = 0;

	AcquisitionMode = Validity = PositionFix = NavigationStatus =
		TrueCourseIndicator = MagneticCourseIndicator =
		SpeedInKnotsIndicator = SpeedInKmhIndicator = 0;

	// Startup the inherited SoftwareSerial port to listen for
	// NMEA sentences.

	begin( 9600 );
	}


//
// PollForNewMessage( )
//


#define	GetGpsCharacter( )	read( )		// Read characters from the GPS using
														// the inherited SoftwareSerial
														// read( ) routine.


bool GPS::PollForNewMessage( )
	{
	// Read any characters waiting on the serial port, looking
	// for a new sentence updating the status.  Always paste
	// a trailing null only for convenience.

	int c;
	while ( ( c  = GetGpsCharacter( ) ) != -1 )
		{
		// Serial.write( c );
		switch ( c )
			{
			case DollarSign:
				// Start of a sentence.
				Message.p = Message.Buffer;
				*Message.p++ = c;
				*Message.p = 0;
				break;

			case NewLine:
				// End of a sentence.
				// Note that we only parse one message before returning to
				// ensure that every message is seen by the caller.

#				ifdef Debug
					// Reinsert the \r deleted by getchar( ).
					if ( Message.p < Message.Buffer + sizeof( Message.Buffer) - 1 )
						{
						*Message.p++ = CarriageReturn;
						*Message.p = 0;
						}
#				endif

				if ( Message.p < Message.Buffer + sizeof( Message.Buffer) - 1 )
					{
					*Message.p++ = c;
					*Message.p = 0;
					}
				return Parse( );

			default:
				// Add the character if we're in the middle of the sentence
				// and haven't run over the end of the buffer.

				// Other characters that can't be a part of a valid sentence
				// are discarded.

				if ( Message.p > Message.Buffer )
					{
					// When adding, allow for this character + NewLine + null

					if ( Message.p < Message.Buffer + sizeof( Message.Buffer) - 2 )
						{
						*Message.p++ = c;
						*Message.p = 0;
						}
					else
						{
						// Overran the buffer. Begin discarding until we see a
						// new DollarSign signaling the start another possibly
						// valid sentence.
						Message.p = Message.Buffer;
						*Message.p = 0;
						}
					}
			}
		}
	return false;		// No status change.
	}


//
// Simple routine to print GPS status.
//


// Arduino sprintf does not support %f items.

int IntegerPortion( double d )
	{
	int i;
	if ( d >= 0.0 )
		i = ( int )floor( d + 0.0005 );
	else
		i = ( int )ceil( d - 0.0005 );
	return i;
	}


int Thousandths( double d )
	{
	int t;
	if ( d >= 0.0 )
		{
		d += 0.0005;
		t = ( int )floor( 1000.0 * ( d - floor( d ) ) );
		}
	else
		{
		d -= 0.0005;
		t = ( int )floor( 1000.0 * ( ceil( d ) - d ) );
		}
	return t;
	}


#define FloatFormat( d )	IntegerPortion( d ), Thousandths( d )

void GPS::PrintStatus( )
	{
	char buffer[ 256 ];
	sprintf( buffer, ( char * )"%02d/%02d/%04d  %3d:%02d:%02d.%03d UTC\r\n"
				"Latitude  = %3d:%02d:%02d.%03d %c = %4d.%03d\r\n"
				"Longitude = %3d:%02d:%02d.%03d %c = %4d.%03d\r\n"
				"Altitude  = %3d.%03d m\r\n"
				"Course = %d.%03d, Speed = %d.%03d knots = %d.%03d Kmh\r\n"
				"Number of satellites in view = %d, Number used = %d\r\n",

					Date.Month, Date.Day, Date.Year,
					Time.Hours, Time.Minutes,
					FloatFormat( Time.Seconds ),
					Latitude.Degrees, Latitude.Minutes,
					FloatFormat( Latitude.Seconds ),
					Latitude.Location ? Latitude.Location : ' ',
					FloatFormat( Latitude.FloatDegrees ),
					Longitude.Degrees, Longitude.Minutes,
					FloatFormat( Longitude.Seconds ),
					Longitude.Location ? Longitude.Location : ' ',
					FloatFormat( Longitude.FloatDegrees ),
					FloatFormat( Altitude ), FloatFormat( TrueCourse ),
					FloatFormat( SpeedInKnots ), FloatFormat( SpeedInKmh ),
					NumberOfSatellitesInView, NumberOfSatellitesUsedInComputation );

#	ifdef Debug
		puts( buffer );
#	else
		Serial.println( buffer );
#	endif
	}
