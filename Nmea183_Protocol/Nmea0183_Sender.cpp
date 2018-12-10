#include "Nmea0183_Sender.h"



Nmea0183_Sender::Nmea0183_Sender()
{
}

Nmea0183_Sender::~Nmea0183_Sender()
{
}
void Nmea0183_Sender::begin(HardwareSerial &device, unsigned long _baud = 4800)
{
	hwStream = &device;
	hwStream->begin(_baud);
}
void Nmea0183_Sender::send(Nmea0183_Frame *rawFrame)
{
	Nmea0183_Frame *_rawFrame = rawFrame;
	int iBuf = 0;
	for (; iBuf < _rawFrame->length; iBuf++) {
		hwStream->write(_rawFrame->message[iBuf]);
	}
}