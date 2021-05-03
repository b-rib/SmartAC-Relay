/*
	Class: OnWin

	FSM state: ON (Windows application)
	Developed in: Microsoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "State.h"
using namespace std;

class OnWin :
	public State
{
	void PushBtn();
	void TimeOver();
	void TotalTimeUpdateBtn();
};
