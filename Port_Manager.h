#ifndef Port_Manager_h
#define Port_Manager_h


#include "Port_DataStatus.h"

class Port_Manager
{
public:
	Port_Manager();
	~Port_Manager();
	Port_DataStatus PortDataStatusList[6]{
		Port_DataStatus(),
		Port_DataStatus(),
		Port_DataStatus(),
		Port_DataStatus(),
		Port_DataStatus(),
		Port_DataStatus()
	};

};

#endif