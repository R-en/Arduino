// GPS.h   Arduino GPS NMEA message driver

// Nicole Hamilton, nicole@nicolehamilton.com, Copyright 2013.


// This driver parses NMEA messages from the GPS module and collects
// status in a GPS class.
//
//	These are the public routines:
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
// Actual status can be read directly from other public members of the class.
// (I have deliberately not bothered with "accessor" routines.)


// References for NMEA messages:
//
// 1. Monte Variakojis, "NMEA Parser Design", November 1, 2002,
//		http://www.visualgps.net/whitepapers/nmeaparser/nmeaparserdesign.html
//
// 2. Ger Rietman, "How to calculate the NMEA checksum", September 25, 2008,
//		http://rietman.wordpress.com/2008/09/25/how-to-calculate-the-nmea-checksum/
//
// 3. "GPS - NMEA sentence information", http://aprs.gids.nl/nmea/


#ifdef	Debug

	// Scaffolding to allow testing on Windows

#	include	<stdio.h>
#	include	<stdlib.h>

	typedef	unsigned char			uint8_t;

	class	SoftwareSerial
		{
		public:
			int read( )
				{
				// Note that getchar( ) deletes CarriageReturns.
				return getchar( );
				}
			SoftwareSerial( uint8_t rx, uint8_t tx )
				{
				}
			void begin( long r )
				{
				}
		};

#else

#	include	<SoftwareSerial.h>
#	include	<Arduino.h>


#endif

#include <math.h>

#define	MaxNmeaMessageLength			83		// max 82 in the message + trailing
														// null
#define	MinNmeaMessageLength			12		// $GPxxx,*xx\r\n = 12

#define	TwoDigitRolloverYear			85		// Interpret 00 to 84 = 2000 to 2084,
														// 85 to 99 = 1985 to 1999

#define	MaxNumberOfSatellites		12

#define	DollarSign						'$'
#define	CarriageReturn					'\r'
#define	NewLine							'\n'
#define	Comma								','
#define	Asterisk							'*'


// Default pins the GPS is attached to.

#define	SoftRxPin 3
#define	SoftTxPin 4


// Various structs used to collect status.

typedef struct NmeaBufferStr
	{
	char	Buffer[ MaxNmeaMessageLength ],
			*p;	// Next position to fill.
	}	NmeaBufferObj;


typedef struct UtcTimeStr
	{
	unsigned Hours,
				Minutes;
	double	Seconds;
	}	UtcTimeObj;


typedef struct UtcDateStr
	{
	unsigned Month,
				Day,
				Year;
	}	UtcDateObj;


typedef struct CoordinateStr
	{
	unsigned Degrees,
				Minutes;
	double	Seconds,
				FloatDegrees;
	char		Location;
	}	CoordinateObj;


typedef struct SatelliteStr
	{
	unsigned ID,
				Elevation,
				Azimuth,
				SignalToNoiseRatio;
	}	SatelliteObj;


// The GPS class inherits from the SoftwareSerial class to operate the
// serial connection to the GPS device.


class GPS : SoftwareSerial
	{
	public:

		// Last message

		NmeaBufferObj	Message;					// If PollForNewMessage( ) returns
														// true, contains the raw sentence.
														// Otherwise, it contains only as
														// many characters as have been
														// collected toward the next
														// sentence.

		// Basic status:	time, date, position and speed

		UtcTimeObj		Time;						// GPGGA and GPRMC
		UtcDateObj		Date;						// GPRMC

		CoordinateObj	Latitude,				// GPGGA and GPRMC
							Longitude;				// GPGGA and GPRMC

		double			Altitude,				// GPGGA:  Altitude is in meters
							TrueCourse,				// GPRMC and GPVTG
							MagneticCourse,		// GPVTG
							SpeedInKnots,			// GPRMC and GPVTG
							SpeedInKmh,				// GPVTG
							MagneticVariation;	// GPRMC

		// Satellites in view and used in computation

		unsigned			NumberOfSatellitesInView;	// GPGGA and GPGSV
		SatelliteObj	SatellitesInView[ MaxNumberOfSatellites ];	// GPGSV

		unsigned			NumberOfSatellitesUsedInComputation,			// GPGSA
							SatellitesUsedInComputation[ MaxNumberOfSatellites ];

		// Detailed status

		unsigned			FixQuality,				// GPGGA:  0 = Invalid, 1 = GPS,
														//         2 = DGPS
							PositionMode;			// GPGSA:  1 = Fix n/a, 2 = 2D,
														//         3 = 3D

		double			HorizontalDilutionOfPrecision,	// GPGGA and GPGSA
							VerticalDilutionOfPrecision,		// GPGSA
							PositionDilutionOfPrecision,		// GPGSA
							HeightOfGeoid,							// GPGGA:  in meters
							SecondsSinceLastDgpsUpdate;		// GPGGA

		unsigned			DgpsReferenceStationID,				// GPGGA
							NumberOfGpgsvMessages,				// GPGSV
							LastGpgsvMessageNumber;				// GPGSV

		char				AcquisitionMode,		// GPGSA and GPGSV:	M = Manual,
														// A = Automatic

							Validity,				// GPRMC:  A = Valid, V = Invalid

							PositionFix,			// GPRMC and GPVTG:	N = Not valid,
														// A = Autonomous, D = Differential,
														// P = Precise,
														// R = Realtime kinematic with ints,
														// F = Realtime kinematic with
														//     floats,
														// E = Estimated (dead reckoning),
														// M = Manual, S = Simulator

							NavigationStatus,		// GPRMC:  S = Safe, C = Caution,
														// U = Unsafe, V = Not valid

							TrueCourseIndicator,			// GPVTG:  Always T
							MagneticCourseIndicator,	// GPVTG:  Always M
							SpeedInKnotsIndicator,		// GPVTG:  Always N
							SpeedInKmhIndicator;			// GPVTG:  Always K


														// Simple constructor takes pin
														// assignments for the GPS device
														// and passes them through to the
														// SoftwareSerial driver.

		GPS( uint8_t rxPin = SoftRxPin, uint8_t txPin = SoftTxPin ) :
				SoftwareSerial( rxPin, txPin )
			{
			}

		bool	PollForNewMessage( );			// Returns true if a valid new
														// message was received and
														// processed.  Only one message
														// is processed to ensure that the
														// caller never misses any.

		void	Initialize( ),						// Zero out all status fields and
														// open the GpsSerial SoftwareSerial
														// port at 9600 baud.
				PrintStatus( );					// Dump basic GPS status to Serial
														// out.


	private:

		bool	ParseGPGGA( ),						// Parse a $GPGGA message.
				ParseGPGSA( ),						// Parse a $GPGSA message.
				ParseGPGSV( ),						// Parse a $GPGSV message.
				ParseGPRMC( ),						// Parse a $GPRMC message.
				ParseGPVTG( ),						// Parse a $GPVTG message.
				Parse( ),							// Parse the supported NMEA messages.
				IsGoodChecksum( );				// Test that the message appears to
														// be properly formatted with a good
														// checksum.
	};
	
	
int	IntegerPortion( double d ),			// sprintf on Arduino does not
														// support %f items, so these are
														// workaround routines to extract
														// the integer portion and the
														// number of thousandths as
														// integers.
		Thousandths( double d );
