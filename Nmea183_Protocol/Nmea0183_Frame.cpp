#include "Nmea0183_Frame.h"
#include <string.h>

Nmea0183_Frame::Nmea0183_Frame()
{
	
}

void Nmea0183_Frame::clear()
{
	length = 0;
	memset(message, 0, sizeof(message));
	memset(separator, 0, sizeof(separator));
	memset(sentence, 0, sizeof(sentence));
	memset(talker, 0, sizeof(talker));
	fieldNumber = 0;
	valid = false;
}
