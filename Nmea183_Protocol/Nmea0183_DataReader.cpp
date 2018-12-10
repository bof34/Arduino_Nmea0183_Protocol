#include "Nmea0183_DataReader.h"

#include <string.h>

Nmea0183_DataReader::Nmea0183_DataReader()
{	
}
Nmea0183_DataReader::~Nmea0183_DataReader()
{
}

double Nmea0183_DataReader::latLonToDouble(const char *data, const char sign) {
	double val = atof(data);
	double deg = floor(val / 100.0);
	double ss = (val - deg*100.0) / 60.0;

	val = deg + ss;
	if (sign == 'S' || sign == 'W') val = -val;

	return val;
}
double Nmea0183_DataReader::kmToNm(double *data)
{
	return (*data / 1.852);
}
//*****************************************************************************
double Nmea0183_DataReader::timeToSeconds(const char *data, uint16_t offsetHour, uint16_t offsetMinute) {
	double val = atof(data);
	double hh = floor(val / 10000.0) + offsetHour;
	double mm = floor((val - hh * 10000.0) / 100.0) + offsetMinute;
	double ss = floor(val - hh * 10000.0 - mm * 100.0);

	val = hh * 3600.0 + mm * 60.0 + ss;
	return val;
}

const char *Nmea0183_DataReader::getData(int dataIndex)
{
	uint16_t start = _frame->separator[dataIndex - 1];//position de la virgule avant la donnée
	uint16_t end = _frame->separator[dataIndex];//position de la virgule après la donnée
	//const uint16_t _sizeOfData = end - start;//on calcule la longueur de la données
	//char _data[_sizeOfData] = {};//on initialise un tableau de stockage de la donnée
	memset(_data, 0, sizeof(_data));
	uint16_t idx = 0;
	for (uint16_t j = start; j < end - 1; j++)//on stocke chaque caractere dans le tableau de donnée
	{
		_data[idx] = _frame->message[j];
		idx++;
	}
	_data[idx] = '\0';//fin de la donnée
	return _data;
}
bool Nmea0183_DataReader::compute(Nmea0183_Frame * frame)
{
	bool result = false;
	dataType = Nmea0183_DataType::None;
	_frame = frame;
	for (uint16_t i = 0; i < SENTENCE_LIST_MAX_LENGTH; i++)
	{
		if (!strcmp("---", validSentence[i]))
		{
			break;
		}
		else
		{
			if (!strcmp(_frame->sentence, validSentence[i]))
			{
				selectSentenceFunction(_frame->sentence);
				result = true;
				break;
			}
		}
	}
	return result;
}
void Nmea0183_DataReader::selectSentenceFunction(char *sentence)
{
	if		(!strcmp(sentence, "AAM")) _AAM();
	else if (!strcmp(sentence, "APA")) _APA();
	else if (!strcmp(sentence, "APB")) _APB();
	else if (!strcmp(sentence, "BEC")) _BEC();
	else if (!strcmp(sentence, "BOD")) _BOD();
	else if (!strcmp(sentence, "BWC")) _BWC();
	else if (!strcmp(sentence, "BWR")) _BWR();
	else if (!strcmp(sentence, "BWW")) _BWW();
	else if (!strcmp(sentence, "DBK")) _DBK();
	else if (!strcmp(sentence, "DBS")) _DBS();
	else if (!strcmp(sentence, "DBT")) _DBT();
	else if (!strcmp(sentence, "DPT")) _DPT();
	else if (!strcmp(sentence, "GGA")) _GGA();
	else if (!strcmp(sentence, "GLL")) _GLL();
	else if (!strcmp(sentence, "GSA")) _GSA();
	else if (!strcmp(sentence, "GSV")) _GSV();
	else if (!strcmp(sentence, "HDG")) _HDG();
	else if (!strcmp(sentence, "HDM")) _HDM();
	else if (!strcmp(sentence, "HDT")) _HDT();
	else if (!strcmp(sentence, "HSC")) _HSC();
	else if (!strcmp(sentence, "MTW")) _MTW();
	else if (!strcmp(sentence, "MWD")) _MWD();
	else if (!strcmp(sentence, "MWV")) _MWV();
	else if (!strcmp(sentence, "OSD")) _OSD();
	else if (!strcmp(sentence, "ROO")) _ROO();
	else if (!strcmp(sentence, "RMA")) _RMA();
	else if (!strcmp(sentence, "RMB")) _RMB();
	else if (!strcmp(sentence, "RMC")) _RMC();
	else if (!strcmp(sentence, "ROT")) _ROT();
	else if (!strcmp(sentence, "RPM")) _RPM();
	else if (!strcmp(sentence, "RSA")) _RSA();
	else if (!strcmp(sentence, "VDR")) _VDR();
	else if (!strcmp(sentence, "VHW")) _VHW();
	else if (!strcmp(sentence, "VLW")) _VLW();
	else if (!strcmp(sentence, "VTG")) _VTG();
	else if (!strcmp(sentence, "VWR")) _VWR();
	else if (!strcmp(sentence, "WCV")) _WCV();
	else if (!strcmp(sentence, "WNC")) _WNC();
	else if (!strcmp(sentence, "WPL")) _WPL();
	else if (!strcmp(sentence, "ZDA")) _ZDA();
}
void Nmea0183_DataReader::_AAM()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,2,3,5 };//numero des données a exploiter dans le message
	dataWaypoint.arrivalCircleEntered = getData(indexData[0])[0] == 'A' ? 1 : 0;
	dataWaypoint.cpa = getData(indexData[1])[0] == 'A' ? 1 : 0;
	dataWaypoint.arrivalCircleRadius = atof(getData(indexData[2]));
	strncpy(dataWaypoint.toId,getData(indexData[3]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_APA()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 3,4,5,6,7,8,9,10 };
	double crossTrackError = atof(getData(indexData[0]));
	if (getData(indexData[2])[0] == 'M')dataWaypoint.crossTrackError = crossTrackError;
	else dataWaypoint.crossTrackError = kmToNm(&crossTrackError);
	dataWaypoint.directionToSteer = getData(indexData[1])[0];
	dataWaypoint.arrivalCircleEntered = getData(indexData[3])[0] == 'A' ? 1 : 0;
	dataWaypoint.cpa = getData(indexData[4])[0] == 'A' ? 1 : 0; 
	char type = getData(indexData[6])[0];
	if (type == 'M') dataWaypoint.bearingOriginDestinationMagn = atof(getData(indexData[5]));
	else if (type == 'T') dataWaypoint.bearingOriginDestinationTrue = atof(getData(indexData[5]));
	strncpy(dataWaypoint.toId, getData(indexData[7]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_APB()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 3,4,5,6,7,8,9,10,11,12,13,14 };
	double crossTrackError = atof(getData(indexData[0]));
	if (getData(indexData[2])[0] == 'M')dataWaypoint.crossTrackError = crossTrackError;
	else dataWaypoint.crossTrackError = kmToNm(&crossTrackError);
	dataWaypoint.directionToSteer = getData(indexData[1])[0];
	dataWaypoint.arrivalCircleEntered = getData(indexData[3])[0] == 'A' ? 1 : 0;
	dataWaypoint.cpa = getData(indexData[4])[0] == 'A' ? 1 : 0;
	char type = getData(indexData[6])[0];
	if (type == 'M') dataWaypoint.bearingOriginDestinationMagn = atof(getData(indexData[5]));
	else if (type == 'T') dataWaypoint.bearingOriginDestinationTrue = atof(getData(indexData[5]));
	strncpy(dataWaypoint.toId, getData(indexData[7]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	type = getData(indexData[9])[0];
	if (type == 'M') dataWaypoint.bearingMagn = atof(getData(indexData[8]));
	else if (type == 'T') dataWaypoint.bearingTrue = atof(getData(indexData[8]));
	type = getData(indexData[11])[0];
	if (type == 'M') dataWaypoint.headingToSteerMagn = atof(getData(indexData[10]));
	else if (type == 'T') dataWaypoint.headingToSteerTrue = atof(getData(indexData[10]));
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_BEC()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,2,3,4,5,6,8,10,12 };
	dataWaypoint.time = timeToSeconds(getData(indexData[0]), 0, 0);
	dataWaypoint.latitude = latLonToDouble(getData(indexData[1]), (const char)getData(indexData[2]));
	dataWaypoint.longitude = latLonToDouble(getData(indexData[3]), (const char)getData(indexData[4]));
	dataWaypoint.bearingTrue = atof(getData(indexData[5]));
	dataWaypoint.bearingMagn = atof(getData(indexData[6]));
	dataWaypoint.distance = atof(getData(indexData[7]));
	strncpy(dataWaypoint.toId, getData(indexData[8]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_BOD()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,3,5,6 };
	dataWaypoint.bearingTrue = atof(getData(indexData[0]));
	dataWaypoint.bearingMagn = atof(getData(indexData[1]));
	strncpy(dataWaypoint.toId, getData(indexData[2]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	strncpy(dataWaypoint.fromId, getData(indexData[3]), sizeof(dataWaypoint.fromId) / sizeof(char));
	dataWaypoint.fromId[sizeof(dataWaypoint.fromId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_BWC()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,2,3,4,5,6,8,10,12 };
	dataWaypoint.time = timeToSeconds(getData(indexData[0]), 0, 0);
	dataWaypoint.latitude = latLonToDouble(getData(indexData[1]), (const char)getData(indexData[2]));
	dataWaypoint.longitude = latLonToDouble(getData(indexData[3]), (const char)getData(indexData[4]));
	dataWaypoint.bearingTrue = atof(getData(indexData[5]));
	dataWaypoint.bearingMagn = atof(getData(indexData[6]));
	dataWaypoint.distance = atof(getData(indexData[7]));
	strncpy(dataWaypoint.toId, getData(indexData[8]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_BWR()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,2,3,4,5,6,8,10,12 };
	dataWaypoint.time = timeToSeconds(getData(indexData[0]), 0, 0);
	dataWaypoint.latitude = latLonToDouble(getData(indexData[1]), (const char)getData(indexData[2]));
	dataWaypoint.longitude = latLonToDouble(getData(indexData[3]), (const char)getData(indexData[4]));
	dataWaypoint.bearingTrue = atof(getData(indexData[5]));
	dataWaypoint.bearingMagn = atof(getData(indexData[6]));
	dataWaypoint.distance = atof(getData(indexData[7]));
	strncpy(dataWaypoint.toId, getData(indexData[8]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_BWW()
{
	dataWaypoint.clear();
	int indexData[] = { 1,3,5,6 };
	dataWaypoint.bearingTrue = atof(getData(indexData[0]));
	dataWaypoint.bearingMagn = atof(getData(indexData[1]));
	strncpy(dataWaypoint.fromId, getData(indexData[2]), sizeof(dataWaypoint.fromId) / sizeof(char));
	dataWaypoint.fromId[sizeof(dataWaypoint.fromId) / sizeof(char) - 1] = '\0';
	strncpy(dataWaypoint.toId, getData(indexData[3]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_DBK()
{
	dataWeather.clear();
	int indexData[] = { 3 };//numero des données a exploiter dans le message
	dataWeather.depthKeel = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_DBS()
{
	dataWeather.clear();
	int indexData[] = { 3 };//numero des données a exploiter dans le message
	dataWeather.depthSurface = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_DBT()
{
	dataWeather.clear();
	int indexData[] = { 3 };//numero des données a exploiter dans le message
	dataWeather.depthTransducer = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_DPT()
{
	dataWeather.clear();
	int indexData[] = { 1,2 };//numero des données a exploiter dans le message
	double dpt = atof(getData(indexData[0]));
	double delta = atof(getData(indexData[1]));
	dataWeather.depthTransducer = dpt;
	if (delta >= 0)
	{
		dataWeather.depthSurface = (dpt + delta);
		dataWeather.depthKeel = NAN;
	}
	else
	{
		dataWeather.depthSurface = NAN;
		dataWeather.depthKeel = (dpt + delta);
	}
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_GGA()
{
	dataPosition.clear();
	dataNavigation.clear();
	uint16_t indexData[] = { 1,2,3,4,5,7,8,9 };
	dataNavigation.time = timeToSeconds(getData(indexData[0]), 0, 0);
	dataPosition.latitude = latLonToDouble(getData(indexData[1]), (const char)getData(indexData[2]));
	dataPosition.longitude = latLonToDouble(getData(indexData[3]), (const char)getData(indexData[4]));
	dataPosition.satelliteCount = atoi(getData(indexData[5]));
	dataPosition.hDop = atof(getData(indexData[6]));
	dataNavigation.altitude = atof(getData(indexData[7]));
	dataType = Nmea0183_DataType::PositionNavigation;
}
void Nmea0183_DataReader::_GLL()
{
	dataPosition.clear();
	dataNavigation.clear();
	uint16_t indexData[] = { 1,2,3,4,5,6 };//numero des données a exploiter dans le message
	if (getData(indexData[5])[0] == 'A')
	{
		dataPosition.latitude = latLonToDouble(getData(indexData[0]), (const char)getData(indexData[1]));
		dataPosition.longitude = latLonToDouble(getData(indexData[2]), (const char)getData(indexData[3]));
		dataNavigation.time = timeToSeconds(getData(indexData[4]), 0, 0);
		dataType = Nmea0183_DataType::PositionNavigation;
	}
}
void Nmea0183_DataReader::_GSA()
{
	dataPosition.clear();
	uint16_t indexData[] = { 15,16,17 };//numero des données a exploiter dans le message
	dataPosition.pDop = atof(getData(indexData[0]));
	dataPosition.hDop = atof(getData(indexData[1]));
	dataPosition.vDop = atof(getData(indexData[2]));
	dataType = Nmea0183_DataType::Position;
}
void Nmea0183_DataReader::_GSV()
{
	dataPosition.clear();
	uint16_t indexData[] = { 3 };
	dataPosition.satelliteCount = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Position;
}
void Nmea0183_DataReader::_HDG()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1,4,5 };
	dataNavigation.headingMagn = atof(getData(indexData[0]));
	double variation = atof(getData(indexData[1]));
	char type = getData(indexData[2])[0];
	if (type == 'E') dataNavigation.variation = variation;
	else if (type == 'W') dataNavigation.variation = -variation;
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_HDM()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1 };
	dataNavigation.headingMagn = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_HDT()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1 };
	dataNavigation.headingTrue = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_HSC()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1,3 };
	dataNavigation.headingTrue = atof(getData(indexData[0]));
	dataNavigation.headingMagn = atof(getData(indexData[1]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_MTW()
{
	dataWeather.clear();
	uint16_t indexData[] = { 1 };
	dataWeather.waterTemp = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_MWD()
{
	dataWeather.clear();
	uint16_t indexData[] = { 1,3,5 };
	dataWeather.windAngleTrue = atof(getData(indexData[0]));
	dataWeather.windAngleRel = atof(getData(indexData[1]));
	dataWeather.windSpeed = atof(getData(indexData[2]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_MWV()
{
	dataWeather.clear();
	uint16_t indexData[] = { 1,2,3 };
	if(getData(indexData[1])[0] == 'R') dataWeather.windAngleRel = atof(getData(indexData[0]));
	else if (getData(indexData[1])[0] == 'T') dataWeather.windAngleTrue = atof(getData(indexData[0]));
	dataWeather.windSpeed = atof(getData(indexData[2]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_OSD()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1,4,5,7,8 };
	dataNavigation.headingTrue = atof(getData(indexData[0]));
	dataNavigation.course = atof(getData(indexData[1]));
	dataNavigation.speed = atof(getData(indexData[2]));
	dataNavigation.set = atof(getData(indexData[3]));
	dataNavigation.driftSpeed = atof(getData(indexData[4]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_ROO()
{}
void Nmea0183_DataReader::_RMA()
{
	dataPosition.clear();
	dataNavigation.clear();
	uint16_t indexData[] = { 2,3,4,5,8,10,11 };
	dataPosition.latitude = latLonToDouble(getData(indexData[0]), (const char)getData(indexData[1]));
	dataPosition.longitude = latLonToDouble(getData(indexData[2]), (const char)getData(indexData[3]));
	dataNavigation.speed = atof(getData(indexData[4]));
	dataNavigation.variation = atof(getData(indexData[5]));
	if (*getData(indexData[6]) == 'W') dataNavigation.variation = -dataNavigation.variation;
	dataType = Nmea0183_DataType::PositionNavigation;
}
void Nmea0183_DataReader::_RMB()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 2,3,4,5,6,7,8,9,10,11,12,13 };
	dataWaypoint.crossTrackError = atof(getData(indexData[0]));
	dataWaypoint.directionToSteer = getData(indexData[1])[0];
	strncpy(dataWaypoint.toId, getData(indexData[2]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	strncpy(dataWaypoint.fromId, getData(indexData[3]), sizeof(dataWaypoint.fromId) / sizeof(char));
	dataWaypoint.fromId[sizeof(dataWaypoint.fromId) / sizeof(char) - 1] = '\0';
	dataWaypoint.latitude = latLonToDouble(getData(indexData[4]), (const char)getData(indexData[5]));
	dataWaypoint.longitude = latLonToDouble(getData(indexData[6]), (const char)getData(indexData[7]));
	dataWaypoint.distance = atof(getData(indexData[8]));
	dataWaypoint.bearingTrue = atof(getData(indexData[9]));
	dataWaypoint.closingSpeed = atof(getData(indexData[10]));
	dataWaypoint.arrivalCircleEntered = getData(indexData[11])[0] == 'A' ? 1 : 0;
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_RMC()
{
	dataPosition.clear();
	dataNavigation.clear();
	uint16_t indexData[] = { 1,2,3,4,5,6,7,9,10,11 };
	if (getData(indexData[1])[0] == 'A')
	{
		dataNavigation.time = timeToSeconds(getData(indexData[0]), 0, 0);
		dataPosition.latitude = latLonToDouble(getData(indexData[2]), (const char)getData(indexData[3]));
		dataPosition.longitude = latLonToDouble(getData(indexData[4]), (const char)getData(indexData[5]));
		dataNavigation.speed = atof(getData(indexData[6]));
		strncpy(dataNavigation.date, getData(indexData[7]), sizeof(dataNavigation.date) / sizeof(char));
		dataNavigation.date[sizeof(dataNavigation.date) / sizeof(char) - 1] = '\0';
		dataNavigation.variation = atof(getData(indexData[8]));
		if (*getData(indexData[9]) == 'W') dataNavigation.variation = -dataNavigation.variation;
	}
	dataType = Nmea0183_DataType::PositionNavigation;
}
void Nmea0183_DataReader::_ROT()
{
	dataEngine.clear();
	uint16_t indexData[] = { 1 };
	dataEngine.shaftRateOfTurn = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Engine;
}
void Nmea0183_DataReader::_RPM()
{
	dataEngine.clear();
	uint16_t indexData[] = { 1,3,5 };
	if (getData(indexData[2])[0] == 'A')
	{
		char type = getData(indexData[0])[0];
		double rpm = atof(getData(indexData[1]));
		if (type == 'E') dataEngine.engineRpm = rpm;
		else if (type == 'S') dataEngine.shaftRpm = rpm;
	}
	dataType = Nmea0183_DataType::Engine;
	//$VWRPM,E,1,0,50,A*62
}
void Nmea0183_DataReader::_RSA()
{
	dataEngine.clear();
	uint16_t indexData[] = { 1 };
	dataEngine.rudder = atof(getData(indexData[0]));
	dataType = Nmea0183_DataType::Engine;
}
void Nmea0183_DataReader::_VDR()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1,3,5 };
	dataNavigation.headingTrue = atof(getData(indexData[0]));
	dataNavigation.headingMagn = atof(getData(indexData[1]));
	dataNavigation.driftSpeed = atof(getData(indexData[2]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_VHW()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1,3,5 };
	dataNavigation.headingTrue = atof(getData(indexData[0]));
	dataNavigation.headingMagn = atof(getData(indexData[1]));
	dataNavigation.waterSpeed = atof(getData(indexData[2]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_VLW()
{
	dataPosition.clear();
	uint16_t indexData[] = { 1,3 };
	dataPosition.distanceTotal = atof(getData(indexData[0]));
	dataPosition.distance = atof(getData(indexData[1]));
	dataType = Nmea0183_DataType::Position;
}
void Nmea0183_DataReader::_VTG()
{
	dataNavigation.clear();
	uint16_t indexData[] = { 1,3,5 };
	dataNavigation.headingTrue = atof(getData(indexData[0]));
	dataNavigation.headingMagn = atof(getData(indexData[1]));
	dataNavigation.speed = atof(getData(indexData[2]));
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::_VWR()
{
	dataWeather.clear();
	uint16_t indexData[] = { 1,3 };
	dataWeather.windAngleTrue = atof(getData(indexData[0]));
	dataWeather.windSpeed = atof(getData(indexData[1]));
	dataType = Nmea0183_DataType::Weather;
}
void Nmea0183_DataReader::_WCV()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,3 };
	dataWaypoint.closingSpeed = atof(getData(indexData[0]));
	strncpy(dataWaypoint.toId, getData(indexData[1]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_WNC()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,5,6 };
	dataWaypoint.distance = atof(getData(indexData[0]));
	strncpy(dataWaypoint.toId, getData(indexData[1]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	strncpy(dataWaypoint.fromId, getData(indexData[1]), sizeof(dataWaypoint.fromId) / sizeof(char));
	dataWaypoint.fromId[sizeof(dataWaypoint.fromId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_WPL()
{
	dataWaypoint.clear();
	uint16_t indexData[] = { 1,2,3,4,5 };
	dataWaypoint.latitude = latLonToDouble(getData(indexData[0]), (const char)getData(indexData[1]));
	dataWaypoint.longitude = latLonToDouble(getData(indexData[2]), (const char)getData(indexData[3]));
	strncpy(dataWaypoint.toId, getData(indexData[4]), sizeof(dataWaypoint.toId) / sizeof(char));
	dataWaypoint.toId[sizeof(dataWaypoint.toId) / sizeof(char) - 1] = '\0';
	dataType = Nmea0183_DataType::Waypoint;
}
void Nmea0183_DataReader::_ZDA()
{
	dataPosition.clear();
	uint16_t indexData[] = { 1,2,3,4,5,6 };
	uint16_t offsettHour = atoi(getData(indexData[4]));
	uint16_t offsettMin = atoi(getData(indexData[5]));
	dataNavigation.time = timeToSeconds(getData(indexData[0]), offsettHour, offsettMin);
	char date[9];
	strcpy(date, getData(indexData[1]));
	strcat(date, getData(indexData[2]));
	strcat(date, getData(indexData[3]));
	strcpy(dataNavigation.date,date);
	dataType = Nmea0183_DataType::Navigation;
}
void Nmea0183_DataReader::addSentence(char *sentence)
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
void Nmea0183_DataReader::removeSentence(char *sentence)
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
void Nmea0183_DataReader::clearAllSentence()
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
void Nmea0183_DataReader::printSentenceList()
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
void Nmea0183_DataReader::print()
{
	switch (dataType)
	{
	case Nmea0183_DataType::Waypoint:
		printWaypoint();
		break;
	case Nmea0183_DataType::Navigation:
		printNavigation();
		break;
	case Nmea0183_DataType::Position:
		printPosition();
		break;
	case Nmea0183_DataType::Weather:
		printWeather();
		break;
	case Nmea0183_DataType::Engine:
		printEngine();
		break;
	case Nmea0183_DataType::PositionNavigation:
		printPosition();
		printNavigation();
		break;
	}
}
void Nmea0183_DataReader::printEngine()
{
		if (!isnan(dataEngine.shaftRateOfTurn)) { Serial.print("shaftRateOfTurn\t"); Serial.println(dataEngine.shaftRateOfTurn); }
		if (!isnan(dataEngine.shaftRpm)) { Serial.print("shaftRpm\t\t"); Serial.println(dataEngine.shaftRpm); }
		if (!isnan(dataEngine.engineRpm)) { Serial.print("engineRpm\t"); Serial.println(dataEngine.engineRpm); }
		if (!isnan(dataEngine.rudder)) { Serial.print("rudder\t\t"); Serial.println(dataEngine.rudder); }
}
void Nmea0183_DataReader::printNavigation()
{
	if (!isnan(dataNavigation.course)) { Serial.print("course\t\t"); Serial.println(dataNavigation.course); }
	if (!isnan(dataNavigation.speed)) { Serial.print("speed\t\t"); Serial.println(dataNavigation.speed); }
	if (!isnan(dataNavigation.altitude)) { Serial.print("altitude\t\t"); Serial.println(dataNavigation.altitude); }
	if (!isnan(dataNavigation.driftSpeed)) { Serial.print("driftSpeed\t"); Serial.println(dataNavigation.driftSpeed); }
	if (!isnan(dataNavigation.headingMagn)) { Serial.print("headingMagn\t"); Serial.println(dataNavigation.headingMagn); }
	if (!isnan(dataNavigation.headingTrue)) { Serial.print("headingTrue\t"); Serial.println(dataNavigation.headingTrue); }
	if (!isnan(dataNavigation.variation)) { Serial.print("variation\t\t"); Serial.println(dataNavigation.variation); }
	if (!isnan(dataNavigation.waterSpeed)) { Serial.print("waterSpeed\t"); Serial.println(dataNavigation.waterSpeed); }
	if (!isnan(dataNavigation.set)) { Serial.print("set\t\t"); Serial.println(dataNavigation.set); }
	if (!isnan(dataNavigation.time)) { Serial.print("time\t\t"); Serial.println(dataNavigation.time, 6); }
	if (strcmp(dataNavigation.date, "")) { Serial.print("date\t\t"); Serial.println(dataNavigation.date); }
}
void Nmea0183_DataReader::printPosition()
{
	if (!isnan(dataPosition.latitude)) { Serial.print("latitude\t\t"); Serial.println(dataPosition.latitude, 6); }
	if (!isnan(dataPosition.longitude)) { Serial.print("longitude\t\t"); Serial.println(dataPosition.longitude, 6); }
	if (dataPosition.satelliteCount != -1) { Serial.print("satelliteCount\t"); Serial.println(dataPosition.satelliteCount); }
	if (dataPosition.quality != -1) { Serial.print("quality\t\t"); Serial.println(dataPosition.quality); }
	if (!isnan(dataPosition.vDop)) { Serial.print("vDop\t\t"); Serial.println(dataPosition.vDop); }
	if (!isnan(dataPosition.hDop)) { Serial.print("hDop\t\t"); Serial.println(dataPosition.hDop); }
	if (!isnan(dataPosition.pDop)) { Serial.print("pDop\t\t"); Serial.println(dataPosition.pDop); }
	if (dataPosition.distanceTotal != -1) { Serial.print("distanceTotal\t"); Serial.println(dataPosition.distanceTotal); }
	if (dataPosition.distance != -1) { Serial.print("distance\t\t"); Serial.println(dataPosition.distance); }
}
void Nmea0183_DataReader::printWeather()
{
	if (!isnan(dataWeather.depthTransducer)) { Serial.print("depthTransducer\t"); Serial.println(dataWeather.depthTransducer); }
	if (!isnan(dataWeather.depthKeel)) { Serial.print("depthKeel\t"); Serial.println(dataWeather.depthKeel); }
	if (!isnan(dataWeather.depthSurface)) { Serial.print("depthSurface\t"); Serial.println(dataWeather.depthSurface); }
	if (!isnan(dataWeather.waterTemp)) { Serial.print("waterTemp\t"); Serial.println(dataWeather.waterTemp); }
	if (!isnan(dataWeather.windAngleTrue)) { Serial.print("windAngle\t"); Serial.println(dataWeather.windAngleTrue); }
	if (!isnan(dataWeather.windAngleRel)) { Serial.print("windAngle\t"); Serial.println(dataWeather.windAngleRel); }
	if (!isnan(dataWeather.windSpeed)) { Serial.print("windSpeed\t"); Serial.println(dataWeather.windSpeed); }
}
void Nmea0183_DataReader::printWaypoint()
{
	if (dataWaypoint.arrivalCircleEntered != -1) { Serial.print("arrivalCircleEntered\t"); Serial.println(dataWaypoint.arrivalCircleEntered); }
	if (!isnan(dataWaypoint.arrivalCircleRadius)) { Serial.print("arrivalCircleRadius\t"); Serial.println(dataWaypoint.arrivalCircleRadius); }
	if (!isnan(dataWaypoint.bearingMagn)) { Serial.print("bearingMagn\t"); Serial.println(dataWaypoint.bearingMagn); }
	if (!isnan(dataWaypoint.bearingOriginDestinationMagn)) { Serial.print("bearingOriginDestinationMagn\t"); Serial.println(dataWaypoint.bearingOriginDestinationMagn); }
	if (!isnan(dataWaypoint.bearingOriginDestinationTrue)) { Serial.print("bearingOriginDestinationTrue\t"); Serial.println(dataWaypoint.bearingOriginDestinationTrue); }
	if (!isnan(dataWaypoint.bearingTrue)) { Serial.print("bearingTrue\t"); Serial.println(dataWaypoint.bearingTrue); }
	if (!isnan(dataWaypoint.closingSpeed)) { Serial.print("closingSpeed\t"); Serial.println(dataWaypoint.closingSpeed); }
	if (dataWaypoint.cpa != -1) { Serial.print("cpa\t\t"); Serial.println(dataWaypoint.cpa); }
	if (!isnan(dataWaypoint.crossTrackError)) { Serial.print("crossTrackError\t"); Serial.println(dataWaypoint.crossTrackError); }
	if (dataWaypoint.directionToSteer != "*") { Serial.print("directionToSteer\t"); Serial.println(dataWaypoint.directionToSteer); }
	if (!isnan(dataWaypoint.distance)) { Serial.print("distance\t\t"); Serial.println(dataWaypoint.distance); }
	if (strcmp(dataWaypoint.fromId, 0)) { Serial.print("fromId\t\t"); Serial.println(dataWaypoint.fromId); }
	if (strcmp(dataWaypoint.toId, 0)) { Serial.print("toId\t\t"); Serial.println(dataWaypoint.toId); }
	if (!isnan(dataWaypoint.headingToSteerMagn)) { Serial.print("headingToSteerMagn\t"); Serial.println(dataWaypoint.headingToSteerMagn); }
	if (!isnan(dataWaypoint.headingToSteerTrue)) { Serial.print("headingToSteerTrue\t"); Serial.println(dataWaypoint.headingToSteerTrue); }
	if (!isnan(dataWaypoint.latitude)) { Serial.print("latitude\t\t"); Serial.println(dataWaypoint.latitude, 6); }
	if (!isnan(dataWaypoint.longitude)) { Serial.print("longitude\t\t"); Serial.println(dataWaypoint.longitude, 6); }
	if (!isnan(dataWaypoint.time)) { Serial.print("time\t\t"); Serial.println(dataWaypoint.time, 6); }
}