#include "SdCard_Manager.h"

SdCard_Manager::SdCard_Manager()
{
	
}

SdCard_Manager::~SdCard_Manager()
{
}

void SdCard_Manager::Init(int cs)
{
	_cs = cs;
	if (!SD.begin(_cs)) {
		Serial.println("SDCard ko");
		Initialised = false;
	}
	else
	{
		Serial.println("SDCard ok");
		Initialised = true;
	}
}

void SdCard_Manager::ReadFile(char* fileName)
{
	Serial.print("File ");
	Serial.print(fileName);
	if (SD.exists(fileName))
	{
		_file = SD.open(fileName, FILE_READ);
		if (_file)
		{
			Serial.println(" ok");
			int n = 0;
			int i = 0;
			char data[4];
			while (_file.available() > 0) {
				char c = _file.read();
				if (c != (char)35) {
					data[i] = c;
					i++;
				}
				else
				{
					n++;
					i = 0;
					memset(data, 0, sizeof(data));
				}
			}
			_file.close();
		}
		else {
			Serial.println(" ko");
		}
	}
	else
	{
		Serial.println(" not found");
	}
}

void SdCard_Manager::Get_PortDataStatus(Port_DataStatus* pdsList)
{
	char fileName[] = "PDS.txt";
	Serial.print("File PDS");
	if (SD.exists(fileName))
	{
		File dataStatusFile = SD.open(fileName, FILE_READ);
		if (dataStatusFile)
		{
			Serial.println(" ok");
			int i = 0;
			int item = 0;
			int index = 0;
			char data[10];
			Serial.println("Start read RDS");
			memset(data, 0, sizeof(data));
			while (dataStatusFile.available() > 0) {
				char c = dataStatusFile.read();
				if (c == (char)59) {
					switch(item)
					{
					 case 0:
						 strcpy(pdsList[index].Name, data);
					 case 1:
						pdsList[index].Input = data[0] == '0'? false : true;
					 case 2:
						pdsList[index].Output = data[0] == '0'? false: true;
					 case 3:
						pdsList[index].Active = data[0] == '0' ? false : true;
					}
					i = 0;
					memset(data, 0, sizeof(data));
					item++;
				}
				else if (c == (char)13) {
				}
				else if (c == (char)10) {
					strcpy(pdsList[index].DataType, data);
					index++;
					item = 0;
					i = 0;
					memset(data, 0, sizeof(data));
				}
				else
				{
					data[i] = c;
					i++;
				}
			}
			strcpy(pdsList[index].DataType, data);
			Serial.println("End read RDS");
			_file.close();
		}
		else 
		{
			Serial.println(" ko");
		}
	}
	else
	{
		Serial.println(" not found");
	}
}
