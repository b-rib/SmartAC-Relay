/*
	Class: OffIno

	FSM state: OFF (Arduino/NodeMCU application)
	Developed in: ArduinoIDE (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "State.h"
using namespace std;

class OffIno :
	public State
{
	void PushBtn();
	void SomethingOpen();
	void Nothing();
};
