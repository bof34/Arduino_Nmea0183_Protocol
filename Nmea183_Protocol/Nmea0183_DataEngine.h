#ifndef _Nmea0183_DataEngine_H_
#define _Nmea0183_DataEngine_H_

struct Nmea0183_DataEngine {
	unsigned long lastTime;
	double
		shaftRateOfTurn,
		shaftRpm,
		engineRpm,
		rudder;

	Nmea0183_DataEngine()
	{
		clear();
	}
	void clear()
	{
		shaftRateOfTurn = NAN;
		shaftRpm = NAN;
		engineRpm = NAN;
		rudder = NAN;
		lastTime = millis();
	};
};
#endif

