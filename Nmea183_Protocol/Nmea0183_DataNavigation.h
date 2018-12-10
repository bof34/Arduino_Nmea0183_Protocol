#ifndef _Nmea0183_DataNavigation_H_
#define _Nmea0183_DataNavigation_H_

struct Nmea0183_DataNavigation {
	unsigned long lastTime;
	char
		date[7];
	double
		speed,
		waterSpeed,
		driftSpeed,
		course,
		set,
		headingTrue,
		headingMagn,
		variation,
		altitude,
		time;// Secs since midnight

	Nmea0183_DataNavigation() {
		clear();
	}
	void clear()
	{
		memset(date, 0, sizeof(date));
		altitude = NAN;
		speed = NAN;
		waterSpeed = NAN;
		driftSpeed = NAN;
		course = NAN;
		set = NAN;
		headingTrue = NAN;
		headingMagn = NAN;
		variation = NAN;
		lastTime = millis();
	};
};

#endif


