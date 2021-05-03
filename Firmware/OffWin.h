/*
	Class: OffWin

	FSM state: OFF (Windows application)
	Developed in: Microsoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "State.h"
using namespace std;

class OffWin :
	public State
{
public:
	void PushBtn();
	void TimeOver();
	void TotalTimeUpdateBtn();
};
