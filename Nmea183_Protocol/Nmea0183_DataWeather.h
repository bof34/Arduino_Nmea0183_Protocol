#ifndef _Nmea0183_DataWeather_H_
#define _Nmea0183_DataWeather_H_

struct Nmea0183_DataWeather {
	unsigned long lastTime;
	double
		depthKeel,
		depthTransducer,
		depthSurface,
		waterTemp,
		windAngleTrue,
		windAngleRel,
		windSpeed;

	Nmea0183_DataWeather()
	{
		clear();
	}
	void clear()
	{
		depthKeel = NAN;
		depthSurface = NAN;
		depthTransducer = NAN;
		waterTemp = NAN;
		windAngleTrue = NAN;
		windAngleRel = NAN;
		windSpeed = NAN;
		lastTime = millis();
	};
};

#endif
