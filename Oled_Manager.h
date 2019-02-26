#ifndef Oled_Manager_h
#define Oled_Manager_h

#include <U8glib.h>
#include "Port_DataStatus.h"
//#include <Arduino.h>
extern uint8_t I2C_SLA;

class Oled_Manager
{
private:
	Port_DataStatus* portDataStatusList;
	bool menuOps = true;
public:
	enum Screen{ Screen1, Screen2 };
	enum Menu { Ops, Config };
	Oled_Manager();
	~Oled_Manager();

	void SetPortDataStatusList(Port_DataStatus _portDataStatusList[]);
	void Init(Screen screen);
	void Init();
	void SelectScreen(Screen screen);
	void TestScreen(char label[]);
	void TestScreen(Screen screen, char label[]);
	void ClearScreen(Screen screen);
	void ClearScreen();
	void ShowGlobalMenu(Screen screen);
	void ShowGlobalMenu();
	uint8_t GetAddress();
	void ChangeMenu(Menu menu);
	void ChangeMenu();
	Menu GetMenu();
};

#endif