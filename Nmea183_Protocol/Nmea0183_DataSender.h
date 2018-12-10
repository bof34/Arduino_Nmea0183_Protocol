#ifndef _Nmea0183_DataSender_H_
#define _Nmea0183_DataSender_H_

#include <Arduino.h>
#include "Nmea0183_DataType.h"
#include "Nmea0183_DataEngine.h"
#include "Nmea0183_DataPosition.h"
#include "Nmea0183_DataWeather.h"
#include "Nmea0183_DataWaypoint.h"
#include "Nmea0183_DataNavigation.h"
#include "Nmea0183_Frame.h"

#define SENTENCE_LIST_MAX_LENGTH 40
#define MESSAGE_MAX_LENGTH 84

class Nmea0183_DataSender
{
protected:
	HardwareSerial*		hwStream;
	Stream*				stream;
	char				result[20];
	bool				authorizedSentence(char *sentence);
	char*				cheksum();
	void				clearMessage();
	bool				send();
	char*				convertDouble(double val, int nbDecimal);
public:
	uint16_t period;
	char		message[MESSAGE_MAX_LENGTH];
	char		sentence[4];
	uint16_t	length;
	Nmea0183_DataSender();
	~Nmea0183_DataSender();
	char *validSentence[SENTENCE_LIST_MAX_LENGTH] =
	{
		"AAM" , "APA" , "APB" , "BEC" , "BOD" , "BWC" , "BWR" , "BWW" , "DBK" , "DBS" ,
		"DBT" , "DPT" , "GGA" , "GGL" , "GSA" , "GSV" , "HDG" , "HDM" , "HDT" , "HSC" ,
		"MTW" , "MWD", "MWV" , "OSD" , "ROO" , "RMA" , "RMB" , "RMC" , "ROT" , "RPM" ,
		"RSA" , "VDR" , "VHW" , "VLW" , "VTG" , "VWR" , "WCV" , "WNC" , "---" , "---"
	}; 
	Nmea0183_DataPosition	*dataPosition;
	Nmea0183_DataWaypoint	*dataWaypoint;
	Nmea0183_DataWeather	*dataWeather;
	Nmea0183_DataEngine		*dataEngine;
	Nmea0183_DataNavigation	*dataNavigation;
	bool				begin(HardwareSerial &device, unsigned long _baud = 4800);
	bool				send(Nmea0183_Frame *rawFrame);
	bool				send(Nmea0183_DataType::data_Type type);
	void				_AAM();
	void				_APA();
	void				_APB();
	void				_BEC();
	void				_BOD();
	void				_BWC();
	void				_BWR();
	void				_BWW();
	void				_DBK();
	void				_DBS();
	void				_DBT();
	void				_DPT();
	void				_GGA();
	void				_GLL();
	void				_GSA();
	void				_GSV();
	void				_HDG();
	void				_HDM();
	void				_HDT();
	void				_HSC();
	void				_MTW();
	void				_MWD();
	void				_MWV();
	void				_OSD();
	void				_ROO();
	void				_RMA();
	void				_RMB();
	void				_RMC();
	void				_ROT();
	void				_RPM();
	void				_RSA();
	void				_VDR();
	void				_VHW();
	void				_VLW();
	void				_VTG();
	void				_VWR();
	void				_WCV();
	void				_WNC();
	void				_WPL();
	void				_ZDA();
	void				addSentence(char *sentence);
	void				removeSentence(char *sentence);
	void				clearAllSentence();
	void				printSentenceList();
};

#endif
