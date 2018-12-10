#ifndef _Nmea0183_Frame_H_
#define _Nmea0183_Frame_H_

class Nmea0183_Frame
{
private:
	
public:
	char message[84];
	int  separator[20];
	char sentence[4];
	char talker[3];
	int fieldNumber = 0;
	int length = 0;
	bool valid = false;
	Nmea0183_Frame();
	void clear();
};


#endif