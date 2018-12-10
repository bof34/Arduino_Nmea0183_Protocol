#ifndef _Protocol_Nmea183_H_
#define _Protocol_Nmea183_H_

#include <Arduino.h>
#include "Nmea0183_Frame.h"

class Protocol_Nmea183
{
private:
	HardwareSerial* hwStream;
	Stream* stream;
	int		 n;
	int		_w;								// numero du traitement sur le caractere
	int		_checksum;						// somme de controle calculée
	char*	_extract_parameter(short&  startCursor, int rank);	//extrait le parametre contenu au rang precise de la trame
	int 	_check;							// checksum inclus avec la trame
	int		_convert_hex(char a);			//conversion en base 16
	void	_compute_frame();			//extrait les parametres contenus dans la trame

	int convert_hex(char a);
public:
	Protocol_Nmea183();
	~Protocol_Nmea183();
	void			begin(HardwareSerial &device, unsigned long _baud = 4800);
	bool			getFrame();
	Nmea0183_Frame	rawFrame;
#endif