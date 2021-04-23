//CLASS State.h
	//Contains the state of the FSM
#pragma once
//#include <iostream>
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
