//CLASS OnIno.h
	//FSM state: ON (Arduino application)
#pragma once
#include "State.h"
//#include <iostream>
using namespace std;

class OnIno :
	public State
{
public:
	void PushBtn();
	void SomethingOpen();
	void Nothing();
};
