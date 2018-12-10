#ifndef _Nmea0183_DataType_H_
#define _Nmea0183_DataType_H_
class Nmea0183_DataType
{
public:
	enum data_Type { None, Position, Waypoint, Navigation, PositionNavigation, Weather, Engine };
};
#endif
