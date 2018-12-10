#ifndef _Nmea0183_DataWaypoint_H_
#define _Nmea0183_DataWaypoint_H_

struct Nmea0183_DataWaypoint {
	unsigned long lastTime;
	char
		*directionToSteer;
	char
		fromId[20],
		toId[20];
	int
		arrivalCircleEntered,
		cpa;
	double
		arrivalCircleRadius,
		bearingMagn,
		bearingTrue,
		bearingOriginDestinationMagn,
		bearingOriginDestinationTrue,
		headingToSteerMagn,
		headingToSteerTrue,
		crossTrackError,
		distance,
		closingSpeed,
		latitude,
		longitude,
		time;

	Nmea0183_DataWaypoint() {
		clear();
	};
	void clear()
	{
		arrivalCircleEntered = -1;
		arrivalCircleRadius = NAN;
		directionToSteer = "*";
		cpa = -1;
		bearingMagn = NAN;
		bearingTrue = NAN;
		bearingOriginDestinationMagn = NAN;
		bearingOriginDestinationTrue = NAN;
		headingToSteerMagn = NAN;
		headingToSteerTrue = NAN;
		crossTrackError = NAN;
		time = NAN;
		latitude = NAN;
		longitude = NAN;
		distance = NAN;
		memset(fromId, 0, sizeof(fromId));
		memset(toId, 0, sizeof(toId));
		closingSpeed = NAN;
		lastTime = millis();
	};
};

#endif