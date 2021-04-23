#include "ActionsIno.h"
#include "Arduino.h"

void ActionsIno::Activate()
{
	digitalWrite(airCond, HIGH);
	digitalWrite(noPwr, LOW);
	Serial.println("airCond ON");
}

void ActionsIno::Deactivate()
{
	digitalWrite(airCond, LOW);
	digitalWrite(noPwr, HIGH);
	Serial.println("airCond OFF");
}
