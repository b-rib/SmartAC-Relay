#include "UsageDataNode.h"
#include "Arduino.h"

UsageDataNode::UsageDataNode(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute, int newSecond, UsageDataNode* nxt)
{
	evnt = newEvent;
	timestampYear = newYear;
	timestampMonth = newMonth;
	timestampDay = newDay;
	timestampHour = newHour;
	timestampMinute = newMinute;
	timestampSecond = newSecond;
	next = nxt;
}

void UsageDataNode::serialPrintNode()
{
	Serial.print(evnt);
	Serial.print(",");
	Serial.print(timestampYear);
	Serial.print(",");
	Serial.print(timestampMonth);
	Serial.print(",");
	Serial.print(timestampDay);
	Serial.print(",");
	Serial.print(timestampHour);
	Serial.print(",");
	Serial.print(timestampMinute);
	Serial.print(",");
	Serial.print(timestampSecond);
	Serial.println();
}

UsageDataNode* UsageDataNode::getNext()
{
	return next;
}

void UsageDataNode::setNode(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute)
{
	evnt = newEvent;
	timestampYear = newYear;
	timestampMonth = newMonth;
	timestampDay = newDay;
	timestampHour = newHour;
	timestampMinute = newMinute;
}

void UsageDataNode::setNext(UsageDataNode * nxt)
{
	next = nxt;
}
