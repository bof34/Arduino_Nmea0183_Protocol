#include "Nmea0183_DataSender.h"

Nmea0183_DataSender::Nmea0183_DataSender()
{
}

Nmea0183_DataSender::~Nmea0183_DataSender()
{
}
bool Nmea0183_DataSender::begin(HardwareSerial &device, unsigned long _baud = 4800)
{
	hwStream = &device;
	hwStream->begin(_baud); 
	while (!hwStream)
	{
	}
	return true;
}
bool Nmea0183_DataSender::send(Nmea0183_Frame *rawFrame)
{
	if (authorizedSentence(rawFrame->sentence))
	{
		int iBuf = 0;
		for (; iBuf < rawFrame->length; iBuf++) {
			hwStream->write(rawFrame->message[iBuf]);
		}
		return true;
	}
	return false;
}
bool Nmea0183_DataSender::send()
{
	if (authorizedSentence(sentence))
	{
		int iBuf = 0;
		for (; iBuf < MESSAGE_MAX_LENGTH; iBuf++) {
			hwStream->write(message[iBuf]);
			if (message[iBuf] == 10) break;
		}
		return true;
	}
	return false;
}
bool Nmea0183_DataSender::authorizedSentence(char *sentence)
{
	for (uint16_t i = 0; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		if (!strcmp("---", validSentence[i]))
		{
			break;
		}
		else
		{
			if (!strcmp(sentence, validSentence[i]))
			{
				return true;
			}
		}
	}
	return false;
}
bool Nmea0183_DataSender::send(Nmea0183_DataType::data_Type type)
{
	bool result = false;
	switch (type)
	{
		case Nmea0183_DataType::Waypoint:

			break;
		case Nmea0183_DataType::Position:

			break;
		case Nmea0183_DataType::Navigation:

			break;
		case Nmea0183_DataType::Weather:
			if (!isnan(dataWeather->depthTransducer))
			{
				_DBT();
				if (!isnan(dataWeather->depthSurface))
				{
					_DBS();
					_DPT();
				}
				if (!isnan(dataWeather->depthKeel))
				{
					_DBK();
					_DPT();
				}
				result = true;
			}
			break;
		case Nmea0183_DataType::Engine:

			break;
		case Nmea0183_DataType::PositionNavigation:

			break;
	}
	return result;
}

void Nmea0183_DataSender::addSentence(char *sentence)
{
	//memccpy(sentence, (const char *)sentence, 4, sizeof(sentence));
	for (uint16_t i = 0; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		if (!strcmp("---", validSentence[i]))
		{
			validSentence[i] = sentence;
			break;
		}
	}
}
void Nmea0183_DataSender::removeSentence(char *sentence)
{
	bool sentenceDeleted = false;
	for (uint16_t i = 0; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		if (!sentenceDeleted)
		{
			if (!strcmp(sentence, validSentence[i]))
			{
				if (i == SENTENCE_LIST_MAX_LENGTH - 1)
				{
					validSentence[i] = "---";
				}
				else
				{
					sentenceDeleted = true;
					validSentence[i] = validSentence[i + 1];
				}
			}
		}
		else
		{
			if (i == SENTENCE_LIST_MAX_LENGTH - 1)
			{
				validSentence[i] = "---";
			}
			else
			{
				validSentence[i] = validSentence[i + 1];
			}
		}
	}
}
void Nmea0183_DataSender::clearAllSentence()
{
	for (uint16_t i = 0; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		if (!strcmp(validSentence[i], "---"))
		{
			break;
		}
		else
		{
			validSentence[i] = "---";
		}
	}
}
void Nmea0183_DataSender::printSentenceList()
{
	Serial.println("Active Sentence list:");
	for (uint16_t i = 0; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		if (!strcmp(validSentence[i], "---"))
		{
			break;
		}
		else
		{
			Serial.println(validSentence[i]);
		}

	}
}

void Nmea0183_DataSender::clearMessage()
{
	memset(message, 0, sizeof(message));
}

void Nmea0183_DataSender::_AAM()
{
	
}
void Nmea0183_DataSender::_APA()
{
	
}
void Nmea0183_DataSender::_APB()
{
	
}
void Nmea0183_DataSender::_BEC()
{
	
}
void Nmea0183_DataSender::_BOD()
{
	
}
void Nmea0183_DataSender::_BWC()
{
	
}
void Nmea0183_DataSender::_BWR()
{
	
}
void Nmea0183_DataSender::_BWW()
{
	
}
void Nmea0183_DataSender::_DBK()
{
	clearMessage();
	strcpy(sentence, "DBK");
	strcpy(message, "$SDDBK,,f,");
	strcat(message, (const char *)(convertDouble(dataWeather->depthKeel, 1)));
	strcat(message, ",M,,F*");
	strcat(message, cheksum());
	strcat(message, "\r");
	strcat(message, "\n");
	send();
}
void Nmea0183_DataSender::_DBS()
{
	clearMessage();
	strcpy(sentence, "DBS");
	strcpy(message, "$SDDBS,,f,");
	strcat(message, (const char *)(convertDouble(dataWeather->depthSurface, 1)));
	strcat(message, ",M,,F*");
	strcat(message, cheksum());
	strcat(message, "\r");
	strcat(message, "\n");
	send();
}
void Nmea0183_DataSender::_DBT()
{
	clearMessage();
	strcpy(sentence, "DBT");
	strcpy(message, "$SDDBT,,f,");
	strcat(message, (const char *)(convertDouble(dataWeather->depthTransducer, 1)));
	strcat(message, ",M,,F*");
	strcat(message, cheksum());
	strcat(message, "\r");
	strcat(message, "\n");
	send();
}
void Nmea0183_DataSender::_DPT()
{
	clearMessage();
	strcpy(sentence, "DPT");
	strcpy(message, "$SDDPT,");
	strcat(message, (const char *)(convertDouble(dataWeather->depthTransducer, 1)));
	strcat(message, ",");
	strcat(message, (const char *)(convertDouble(dataWeather->depthSurface - dataWeather->depthTransducer, 1)));
	strcat(message, "*");
	strcat(message, cheksum());
	strcat(message, "\r");
	strcat(message, "\n");
	send();
}
void Nmea0183_DataSender::_GGA()
{
	
}
void Nmea0183_DataSender::_GLL()
{
	
}
void Nmea0183_DataSender::_GSA()
{
	
}
void Nmea0183_DataSender::_GSV()
{
	
}
void Nmea0183_DataSender::_HDG()
{
	
}
void Nmea0183_DataSender::_HDM()
{
	
}
void Nmea0183_DataSender::_HDT()
{
	
}
void Nmea0183_DataSender::_HSC()
{
	
}
void Nmea0183_DataSender::_MTW()
{
	
}
void Nmea0183_DataSender::_MWD()
{
	
}
void Nmea0183_DataSender::_MWV()
{
	
}
void Nmea0183_DataSender::_OSD()
{
	
}
void Nmea0183_DataSender::_ROO()
{
}
void Nmea0183_DataSender::_RMA()
{
	
}
void Nmea0183_DataSender::_RMB()
{
	
}
void Nmea0183_DataSender::_RMC()
{
	
}
void Nmea0183_DataSender::_ROT()
{
	
}
void Nmea0183_DataSender::_RPM()
{
	
}
void Nmea0183_DataSender::_RSA()
{
	
}
void Nmea0183_DataSender::_VDR()
{
	
}
void Nmea0183_DataSender::_VHW()
{
	
}
void Nmea0183_DataSender::_VLW()
{
	
}
void Nmea0183_DataSender::_VTG()
{
	
}
void Nmea0183_DataSender::_VWR()
{
	
}
void Nmea0183_DataSender::_WCV()
{
	
}
void Nmea0183_DataSender::_WNC()
{
	
}
void Nmea0183_DataSender::_WPL()
{
	
}
void Nmea0183_DataSender::_ZDA()
{
	
}

char *Nmea0183_DataSender::cheksum()
{
	memset(result, 0, sizeof(result));
	int _checksum = 0;
	for (uint16_t i = 1; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		char c = message[i];
		if (c == '*')
		{
			break;
		}
		else
		{
			_checksum ^= c;
		}
	}
	int first = floor(_checksum / 16);
	int second = _checksum - (first * 16);
	result[0] = (char)first + (first > 9 ? 55 : 48);
	result[1] = (char)second + (second > 9 ? 55 : 48);
	result[2] = '\0';
	return result;
}
char *Nmea0183_DataSender::convertDouble(double val, int nbDecimal)
{
	memset(result, 0, sizeof(result));
	dtostrf(val, sizeof(result) - 1, nbDecimal, result);
	while (result[0] == ' ')
	{
		for (uint16_t i = 1; i < sizeof(result); i++)
		{
			result[i - 1] = result[i];
			if (result[i] == '\0') break;
		}
	}
	return result;
}