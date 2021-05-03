/*
	Class: AirConditionerRelayIno

	To be used with "AirConditionerRelay" (polimorphism).
	FSM (states: OnIno and OffIno).
	Developed in: Microsoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

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
