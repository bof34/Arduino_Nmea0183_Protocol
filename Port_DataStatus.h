#ifndef Port_DataStatus_h
#define Port_DataStatus_h

class Port_DataStatus
{
public:
	Port_DataStatus();
	~Port_DataStatus();
	char Name[10];
	bool Input;
	bool Output;
	bool Active;
	char DataType[10];
};

#endif
