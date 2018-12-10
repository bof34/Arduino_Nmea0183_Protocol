#include "Protocol_Nmea183.h"

Protocol_Nmea183::Protocol_Nmea183()
{
}
Protocol_Nmea183::~Protocol_Nmea183()
{
}

void Protocol_Nmea183::begin(HardwareSerial &device, unsigned long _baud = 4800)
{
	hwStream = &device;
	hwStream->begin(_baud);
}
bool Protocol_Nmea183::getFrame()
{
	bool result = false;
	while (hwStream->available() > 0 && !result)
	{
		char c = hwStream->read();
		if ((c == (char)10) || (c == (char)13)) // si LF et  CR : _w=0 ----->case 0
		{ 
			_w = 0; 
		}
		else if ((c == '!') || (c == '$'))
		{

			memset(rawFrame.message, 0, sizeof(rawFrame.message));
			_checksum = 0;
			rawFrame.message[0] = c;
			n = 1;
			_w = 1;
			return 0;//valeur de retour FALSE
		}

		// ajout des caractères pour fabriquer la phrase
		switch (_w)
		{
			case 0:
				// attente des caractères  '!' et '$' 
				break;
			case 1:
				rawFrame.message[n++] = c;
				switch (c)
				{
				case '*':  // si fin de phrase ( avant checksum)
					_w = 2;   // il faut lire le 1er caractère du checksum en case 2
					break;
				default:  // ce n' est pas la fin de la phrase 
					_checksum = _checksum ^ c;
					//_w = 1;
					break;
				}
				break;
			case 2:
				rawFrame.message[n++] = c; // on lit le 1er caractère du checksum
				_check = (16 * convert_hex(c));
				_w = 3;   // il faut lire le 2eme caractère du checksum en case 3
				break;
			case 3:
				rawFrame.message[n++] = c; // lecture 2eme caractere checksum
				rawFrame.message[n++] = 13;// retour chariot
				rawFrame.message[n++] = 10;// retour ligne
								//_frame[n++] = 0;
				_check = _check + convert_hex(c);
				if (_checksum == _check)
				{
					_compute_frame();
					_w = 0;
					return 1;
				}// phrase acceptée , valeur de retour TRUE pour 1
				else return false; // valeur de retour FALSE pour 0
			default:

				break;
		}
		return false;
	}
	return result;
}
int Protocol_Nmea183::convert_hex(char a) {
	// retourne la valeur en base 16 des caractères
	if (int(a) >= 65) { return int(a) - 55; }
	else { return int(a) - 48; }
}