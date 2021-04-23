//CLASS OnWin.h
	//FSM state: ON (Windows application)
#pragma once
#include "State.h"
//#include <iostream>
using namespace std;

class OnWin :
	public State
{
	void PushBtn();
	void TimeOver();
	void TotalTimeUpdateBtn();
};
