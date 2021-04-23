#include "AirConditionerRelay.h"
#include "Arduino.h"


void AirConditionerRelay::HandleEvent(int newEvent)
{
	switch (newEvent)
	{
	case 0:
		MachineState->SomethingOpen(); //Will perform arctions according to current machine state
		break;
	case 1:
		MachineState->PushBtn();
		break;
	default:
		MachineState->Nothing();
		break;
	}
}
