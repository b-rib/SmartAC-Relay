//CLASS TimeRelayIno.h
	//Used for polymorphism (Arduino application)
#pragma once
#include "AirConditionerRelay.h"
#include "OffIno.h"
#include "OnIno.h"
//#include <iostream>
using namespace std;
class State;

class AirConditionerRelayIno :
	public AirConditionerRelay
{
	State* off;
	State* on;

public:
	AirConditionerRelayIno();
	void updateState();
	bool getState();
};
