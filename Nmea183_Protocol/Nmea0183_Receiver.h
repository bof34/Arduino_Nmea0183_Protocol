#ifndef _Nmea0183_Receiver_H_
#define _Nmea0183_Receiver_H_

#include <Arduino.h>
#include "Nmea0183_Frame.h"

class Nmea0183_Receiver
{
protected:
	HardwareSerial*		hwStream;
	Stream*				stream;
	int					_cursor;
	int					_phase;								// numero du traitement sur le caractere
	int					_checksum;						// somme de controle calculée
	int 				_check;							// checksum inclus avec la trame
	int					convert_hex(char a);
	void				computeHeader(int length);
public:
	Nmea0183_Receiver();
	~Nmea0183_Receiver();
	bool			begin(HardwareSerial &device, unsigned long _baud = 4800);
	bool			getFrame();
	Nmea0183_Frame	frame;
};
#endif

