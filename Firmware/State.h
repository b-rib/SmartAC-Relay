/*
	Class: State

	Handles the state of the FSM.
	Developed in: Microsoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
using namespace std;

#include "Events.h"
#include "ActionsWin.h"
#include "ActionsIno.h"

class State :
	public Events, public ActionsWin, public ActionsIno
{
	int nextState;
public:
	void setState(int newState);
	int getNextState();
};
