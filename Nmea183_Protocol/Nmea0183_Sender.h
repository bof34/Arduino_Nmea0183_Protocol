#ifndef _Nmea0183_Sender_H_
#define _Nmea0183_Sender_H_

#include <Arduino.h>
#include "Nmea0183_Frame.h"

class Nmea0183_Sender
{
protected:
	HardwareSerial*		hwStream;
	Stream*				stream;
	Nmea0183_Frame		rawFrame;
public:
	Nmea0183_Sender();
	~Nmea0183_Sender();
	void		begin(HardwareSerial &device, unsigned long _baud = 4800);
	void		send(Nmea0183_Frame *rawFrame);
};

#endif
