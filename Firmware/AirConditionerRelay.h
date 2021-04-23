//CLASS TimeRelay.h
	//Used for polymorphism (abstract)
#pragma once
#include "State.h"

//#include <iostream>
using namespace std;
class State;

class AirConditionerRelay
{
protected:
	State* MachineState;
public:


	void HandleEvent(int newEvent); //Event handler 0-SensorOpen 1-PushBtn
	virtual void updateState() { return; }
	virtual bool getState() { return false; }
};
