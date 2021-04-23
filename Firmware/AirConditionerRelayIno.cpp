#include "AirConditionerRelayIno.h"
#include "OffIno.h"
#include "OnIno.h"

AirConditionerRelayIno::AirConditionerRelayIno()
{
	off = new OffIno();
	on = new OnIno();
	MachineState = off; //Beginning state
}

void AirConditionerRelayIno::updateState()
{
	int newState = MachineState->getNextState();

	switch (newState)
	{
	case 0: //OFF
		MachineState = off;
		break;
	case 1: //ON
		MachineState = on;
		break;
	default:
		break;
	}
}

bool AirConditionerRelayIno::getState()
{
	if (MachineState == on)
	{
		return true;
	}
	if (MachineState == off)
	{
		return false;
	}
}
