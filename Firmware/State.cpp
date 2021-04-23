#include "State.h"

void State::setState(int newState)
{
	nextState = newState;
}

int State::getNextState()
{
	return nextState;
}
