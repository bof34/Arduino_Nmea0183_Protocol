/*
    Name:       Nmea183_Protocol.ino
    Created:	02/12/2018 16:04:41
    Author:     BUPRO\laurent.pares
*/
#include <arduino.h>
#include "Nmea0183_DataReader.h"
#include "Nmea0183_Receiver.h"
#include "Nmea0183_DataSender.h"

Nmea0183_Receiver	nmea183Receiver;
Nmea0183_DataReader nmea183DataReader;
Nmea0183_DataSender	nmea183DataSender;

void setup()
{
	Serial.begin(115200);
	delay(1000);
	Serial.println("Serial ok");
	if (nmea183Receiver.begin(Serial3, 4800))
	{
		Serial.println("serial3 ok");
	}
	if (nmea183DataSender.begin(Serial2, 4800))
	{
		nmea183DataSender.dataWeather = &nmea183DataReader.dataWeather;
		nmea183DataSender.dataNavigation = &nmea183DataReader.dataNavigation;
		nmea183DataSender.dataPosition = &nmea183DataReader.dataPosition;
		nmea183DataSender.dataWaypoint = &nmea183DataReader.dataWaypoint;
		nmea183DataSender.dataEngine = &nmea183DataReader.dataEngine;
		Serial.println("serial2 ok");
	}
	/*nmea183Data.clearAllSentence();
	nmea183Data.addSentence("WPL");
	nmea183Data.printSentence();*/
}

void loop()
{
	if (nmea183Receiver.getFrame())
	{
		if (nmea183DataReader.compute(&nmea183Receiver.frame))
		{
			Serial.print(nmea183Receiver.frame.message);
			//nmea183DataReader.print();
		}
		
		/*if (nmea183DataSender.send(&nmea183Receiver.frame))
		{
			Serial.println("raw frame sent: ok");
		}*/
		if (nmea183DataSender.send(nmea183DataReader.dataType))
		{
			Serial.print("sent: ");
			Serial.println(nmea183DataSender.message);
		}
	}
}
