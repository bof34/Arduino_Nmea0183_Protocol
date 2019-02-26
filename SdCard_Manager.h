#ifndef SdCard_Manager_h
#define SdCard_Manager_h

#include <SD.h>
#include "Port_DataStatus.h"

class SdCard_Manager
{
private:
	int _cs;
	File _file;
public:
	bool Initialised = false;
	SdCard_Manager();
	~SdCard_Manager();


	void Init(int cs);

	void ReadFile(char* fileName);
	void Get_PortDataStatus(Port_DataStatus* pds);
};

#endif