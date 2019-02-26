#ifndef Stick_Manager_h
#define Stick_Manager_h

#include "arduino.h"
class Stick_Manager
{
private:
	const int pinClk = 2;
	const int pinDt = 40;
	const int pinSw = 38;
	bool switchedOn = 0; 
	volatile int16_t virtualPosition;
	volatile int16_t step;
public:
	//volatile bool RotationOnLeft = false;
	// bool RotationOnRight = false;
	Stick_Manager();
	~Stick_Manager();
	void RotationEvent();
	void SwitchEvent();
	int GetRotationInterruptPin();
	int GetSwitchInterruptPin();
	void OnEventCallback(void (*dFunction)(bool rotationOnLeft, bool rotationOnRight, bool switchedOn));
	void SetStep(uint16_t _step);
};

#endif