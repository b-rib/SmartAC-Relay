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

