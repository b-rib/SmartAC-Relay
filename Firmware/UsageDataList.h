/*
	Class: UsageDataList

	A singly linked list.
	Functionallities:
		- Create/delete list
		- Insert node in the end
		- Insert node in the beginning
		- Remove node from beginning
		- List all (prints to serial port)
	Developed in: ArduinoIDE (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "UsageDataNode.h"
using namespace std;

class UsageDataList
{
	UsageDataNode* head;
	UsageDataNode* tail;
public:
	UsageDataList();
	~UsageDataList();
	void insertBeforeFirst(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute, int newSecond);
	void insertAfterLast(char newEvent, int newYear, int newMonth, int newDay, int newHour, int newMinute, int newSecond);
	void removeFirst();
	void listAll();
};

