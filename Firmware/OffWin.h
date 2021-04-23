//CLASS OffWin.h
	//FSM state: OFF (Windows application)
#pragma once
#include "State.h"
//#include <iostream>
using namespace std;

class OffWin :
	public State
{
public:
	void PushBtn();
	void TimeOver();
	void TotalTimeUpdateBtn();
};
