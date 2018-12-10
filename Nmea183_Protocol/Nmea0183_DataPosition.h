#ifndef _Nmea0183_DataPosition_H_
#define _Nmea0183_DataPosition_H_

struct Nmea0183_DataPosition {
	unsigned long lastTime;
	double
		latitude,
		longitude,
		pDop,
		hDop,
		vDop;
	int 
		quality, 
		satelliteCount,
		distanceTotal,
		distance;

	Nmea0183_DataPosition()
	{
		clear();
	}
	void clear()
	{
		latitude = NAN;
		longitude = NAN;
		pDop = NAN;
		hDop = NAN;
		vDop = NAN;
		quality = -1;
		satelliteCount = -1;
		distanceTotal = -1;
		distance = -1;
		lastTime = millis();
	};
};

#endif

