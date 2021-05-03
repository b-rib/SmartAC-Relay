/*
	Class: OnIno

	FSM state: ON (Arduino/NodeMCU application)
	Developed in: ArduinoIDE (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "State.h"
using namespace std;

class OnIno :
	public State
{
public:
	void PushBtn();
	void SomethingOpen();
	void Nothing();
};
