//CLASS OffIno.h
	//FSM state: OFF (Arduino application)
#pragma once
#include "State.h"
//#include <iostream>
using namespace std;

class OffIno :
	public State
{
	void PushBtn();
	void SomethingOpen();
	void Nothing();
};
