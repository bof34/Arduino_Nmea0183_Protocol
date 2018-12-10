#include "Nmea0183_Receiver.h"

Nmea0183_Receiver::Nmea0183_Receiver()
{
}
Nmea0183_Receiver::~Nmea0183_Receiver()
{
}

bool	Nmea0183_Receiver::begin(HardwareSerial &device, unsigned long _baud = 4800)
{
	hwStream = &device;
	hwStream->begin(_baud);
	while(!hwStream)
	{ }
	return true;
}
bool	Nmea0183_Receiver::getFrame()
{
	while (hwStream->available() > 0)
	{
		char c = hwStream->read();
		if (c == (char)10) // si LF et  CR : _phase=0 ----->case 0
		{
			_phase = 0;
		}
		else if ((c == '!') || (c == '$'))
		{

			frame.clear();
			_checksum = 0;
			frame.message[0] = c;
			_cursor = 1;
			_phase = 1;
			return 0;//valeur de retour FALSE
		}

		// ajout des caractères pour fabriquer la phrase
		switch (_phase)
		{
			case 0:
				// attente des caractères  '!' et '$' 
				break;
			case 1:
				frame.message[_cursor++] = c;
				switch (c)
				{
					case ',':  // separateur de champ
						frame.separator[frame.fieldNumber] = _cursor;//on stocke la position du separateur de champ
						frame.fieldNumber++;//on incremente le nombre de champs
						_checksum ^= c;
						break;
					case '*':  // si fin de phrase ( avant checksum)
						_phase = 2;   // il faut lire le 1er caractère du checksum en case 2
						frame.separator[frame.fieldNumber] = _cursor;//on stocke la position du separateur du dernier champ
						frame.fieldNumber++;//on incremente pour avoir le nombre de champs total
						break;
					default:  // ce n'est pas la fin de la phrase 
						_checksum ^= c;
						break;
				}
				break;
			case 2:
				frame.message[_cursor++] = c; // on lit le 1er caractère du checksum
				_check = (convert_hex(c)) << 4;
				_phase = 3;   // il faut lire le 2eme caractère du checksum en case 3
				break;
			case 3:
				frame.message[_cursor++] = c; // lecture 2eme caractere checksum
				frame.message[_cursor++] = 13;// retour chariot
				frame.message[_cursor++] = 10;// retour ligne
				_check |= (convert_hex(c));
				if (_checksum == _check)
				{
					computeHeader(_cursor);
					_phase = 0;
					_cursor = 0;
					return true;
				}
				else return false;
			default:

				break;
			}
	}
	return false;
}

int	Nmea0183_Receiver::convert_hex(char a) {
	// retourne la valeur en base 16 des caractères
	if (int(a) >= 65) { return int(a) - 55; }
	else { return int(a) - 48; }
}
void	Nmea0183_Receiver::computeHeader(int length) {
	frame.talker[0] = frame.message[1];
	frame.talker[1] = frame.message[2];
	frame.talker[2] = '\0';
	frame.sentence[0] = frame.message[3];
	frame.sentence[1] = frame.message[4];
	frame.sentence[2] = frame.message[5];
	frame.sentence[3] = '\0';
	frame.valid = true;
	frame.length = length;
}
