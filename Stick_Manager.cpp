#include "Stick_Manager.h"

void(*eventFunction)(bool rotationOnLeft, bool rotationOnRight, bool switchedOn);
void Stick_Manager::OnEventCallback(void(*pFunc)(bool rotationOnLeft, bool rotationOnRight, bool switchedOn))
{
	eventFunction = pFunc;
}
Stick_Manager::Stick_Manager()
{
	pinMode(pinClk, INPUT);
	pinMode(pinDt, INPUT);
	pinMode(pinSw, INPUT); 
	digitalWrite(pinClk, HIGH);
	digitalWrite(pinDt, HIGH);
	digitalWrite(pinSw, HIGH);
	virtualPosition = 0;
	step = 1;
}


Stick_Manager::~Stick_Manager()
{
}

void Stick_Manager::RotationEvent()
{
	int MSB = digitalRead(pinClk);
	int LSB = digitalRead(pinDt);
	int sum = (MSB << 1) | LSB;
	if (sum == 0b01)
	{
		virtualPosition++;
		if (virtualPosition == step)
		{
			virtualPosition = 0;
			if (eventFunction) eventFunction(false, true, switchedOn);
		}
	}
	else if (sum == 0b00)
	{
		virtualPosition--;
		if (virtualPosition == -step)
		{
			virtualPosition = 0;
			if (eventFunction) eventFunction(true, false, switchedOn);
		}
	}
}
void Stick_Manager::SwitchEvent()
{
	switchedOn = digitalRead(pinSw) == 1? 0: 1;
	if (switchedOn)
	{
		if (eventFunction) eventFunction(false, false, switchedOn);
	}
}

int Stick_Manager::GetRotationInterruptPin()
{
	return pinClk;
}
int Stick_Manager::GetSwitchInterruptPin()
{
	return pinSw;
}

void Stick_Manager::SetStep(uint16_t _step)
{
	step = _step;
}
