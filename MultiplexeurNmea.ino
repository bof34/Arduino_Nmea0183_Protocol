/*
    Name:       MultiplexeurNmea.ino
    Created:	20/11/2018 09:36:50
    Author:     laurent.pares
*/
#include "Port_Manager.h"
#include "SdCard_Manager.h"
#include "Oled_Manager.h"
#include "Stick_Manager.h";

Port_Manager portMng;
SdCard_Manager sdcardMng;
Oled_Manager oledMng;
Stick_Manager stickMng;
void setup()
{
	Serial.begin(115200);
	delay(1000);
	Serial.println("Start program");

	stickMng.SetStep(2);
	stickMng.OnEventCallback(OnStickEvent);
	attachInterrupt(digitalPinToInterrupt(stickMng.GetRotationInterruptPin()), stickMngRotationEvent, FALLING);
	attachInterrupt(digitalPinToInterrupt(stickMng.GetSwitchInterruptPin()), stickMngSwitchEvent, CHANGE);
	
	sdcardMng.Init(26);
	if (sdcardMng.Initialised)
	{
		sdcardMng.Get_PortDataStatus(portMng.PortDataStatusList);
		for (int i = 0; i < 6; i++)
		{
			Serial.print("Name: ");
			Serial.print(portMng.PortDataStatusList[i].Name);
			Serial.print(" - Input: ");
			Serial.print(portMng.PortDataStatusList[i].Input);
			Serial.print(" - Ouput: ");
			Serial.print(portMng.PortDataStatusList[i].Output);
			Serial.print(" - Active: ");
			Serial.print(portMng.PortDataStatusList[i].Active);
			Serial.print(" - Type: ");
			Serial.println(portMng.PortDataStatusList[i].DataType);
		}
		//oledMng.ShowGlobalMenu(1);
	}
	pinMode(SDA, INPUT_PULLUP);
	pinMode(SCL, INPUT_PULLUP);
	oledMng.Init(Oled_Manager::Screen2);
	char adr[5];
	itoa(oledMng.GetAddress(), adr, 16);
	oledMng.TestScreen(adr); 
	oledMng.Init(Oled_Manager::Screen1);
	itoa(oledMng.GetAddress(), adr, 16);
	oledMng.TestScreen(adr);
	delay(2000);
	oledMng.SetPortDataStatusList(portMng.PortDataStatusList);
	//oledMng.ShowGlobalMenu(Oled_Manager::Screen1);
}

void loop()
{
}

void OnStickEvent(bool rotationOnLeft, bool rotationOnRight, bool switchedOn)
{
	Serial.print(rotationOnLeft);
	Serial.print(rotationOnRight);
	Serial.println(switchedOn);
	if (!switchedOn)
	{
		if (oledMng.GetMenu() == Oled_Manager::Config)
		{
			oledMng.ChangeMenu();
		}
		if (rotationOnLeft)
		{
			oledMng.ShowGlobalMenu(Oled_Manager::Screen2);
			oledMng.TestScreen(Oled_Manager::Screen1, "");
		}
		else if (rotationOnRight)
		{
			oledMng.ShowGlobalMenu(Oled_Manager::Screen1);
			oledMng.TestScreen(Oled_Manager::Screen2,"");
		}
		else
		{
			oledMng.ShowGlobalMenu(Oled_Manager::Screen1);
			oledMng.TestScreen(Oled_Manager::Screen2, "");
		}
	}
	else
	{
		if (oledMng.GetMenu() == Oled_Manager::Ops)
		{
			oledMng.ChangeMenu();
		}
		if (rotationOnLeft)
		{
			
		}
		else if (rotationOnRight)
		{
			
		}
		else
		{
			oledMng.TestScreen(Oled_Manager::Screen2, "CONFIG");
			oledMng.TestScreen(Oled_Manager::Screen1, "");
		}

	}
}

void stickMngRotationEvent()
{
	stickMng.RotationEvent();
}
void stickMngSwitchEvent()
{
	stickMng.SwitchEvent();
}
