#ifndef _Nmea0183_DataReader_H_
#define _Nmea0183_DataReader_H_

#include <Arduino.h>
#include <string.h>

#include "Nmea0183_DataType.h"
#include "Nmea0183_DataPosition.h"
#include "Nmea0183_DataWeather.h"
#include "Nmea0183_DataWaypoint.h"
#include "Nmea0183_DataEngine.h"
#include "Nmea0183_DataNavigation.h"
#include "Nmea0183_Frame.h"

#define SENTENCE_LIST_MAX_LENGTH 40

class Nmea0183_DataReader
{

protected:
	char _data[20];
	const double pi = 3.1415926535897932384626433832795;
	const double kmhToms = 1000.0 / 3600.0;
	const double knToms = 1852.0 / 3600.0;
	const double degToRad = pi / 180.0;
	const double radToDeg = 180.0 / pi;
	const double msTokmh = 3600.0 / 1000.0;
	const double msTokn = 3600.0 / 1852.0;
	const double nmTom = 1.852 * 1000;
	const double mToFathoms = 0.546806649;
	const double mToFeet = 3.2808398950131;
	double latLonToDouble(const char *data, const char sign);
	double timeToSeconds(const char *data, uint16_t offsetHour, uint16_t offsetMinute);
	double kmToNm(double *data);
	Nmea0183_Frame *_frame;
	const char *getData(int dataIndex);
	void selectSentenceFunction(char *sentence);
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
	void				printWeather();
	void				printPosition();
	void				printNavigation();
	void				printWaypoint();
	void				printEngine();
public:

	Nmea0183_DataReader();
	~Nmea0183_DataReader();
	char *validSentence[SENTENCE_LIST_MAX_LENGTH] =
	{
		 "AAM" , "APA" , "APB" , "BEC" , "BOD" , "BWC" , "BWR" , "BWW" , "DBK" , "DBS" ,
		 "DBT" , "DPT" , "GGA" , "GGL" , "GSA" , "GSV" , "HDG" , "HDM" , "HDT" , "HSC" ,
		 "MTW" , "MWD", "MWV" , "OSD" , "ROO" , "RMA" , "RMB" , "RMC" , "ROT" , "RPM" ,
		 "RSA" , "VDR" , "VHW" , "VLW" , "VTG" , "VWR" , "WCV" , "WNC" , "---" , "---"
	};
	Nmea0183_DataPosition			dataPosition;
	Nmea0183_DataWaypoint			dataWaypoint;
	Nmea0183_DataWeather			dataWeather;
	Nmea0183_DataEngine				dataEngine;
	Nmea0183_DataNavigation			dataNavigation;
	bool							compute(Nmea0183_Frame * frame);
	Nmea0183_DataType::data_Type	dataType = Nmea0183_DataType::None;
	void							addSentence(char *sentence);
	void							removeSentence(char *sentence);
	void							clearAllSentence();
	void							printSentenceList();
	void							print();
};
#endif
