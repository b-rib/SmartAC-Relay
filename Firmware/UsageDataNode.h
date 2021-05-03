/*
	Class: UsageDataNode

	Nodes of a singly linked list.
	Contains: [Event  Timestamp  PointerToNext->]
	Functionallities:
		- Create node
		- Set node's informations (event, timestamp)
		- Set/Get node's next
		- Print node's information to serial port
	Developed in: Arduino IDE (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
using namespace std;

class UsageDataNode
{
	char evnt;

	int timestampYear;
	int timestampMonth;
	int timestampDay;
	int timestampHour;
	int timestampMinute;
	int timestampSecond;

	UsageDataNode* next;

public:

	UsageDataNode(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute, int newSecond, UsageDataNode* nxt);
	void serialPrintNode();
	UsageDataNode* getNext();
	void setNode(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute);
	void setNext(UsageDataNode* nxt);
};
