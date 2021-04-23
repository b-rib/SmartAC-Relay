//CLASS ArduinoInputDecoder.h
	//Decodes Arduino inputs
#pragma once
#include "AirConditionerRelay.h"
#include "AirConditionerRelayIno.h"
#include "TimeStamp.h"
#include "UsageDataList.h"
//#include <iostream>

#define dip1	7
#define dip2	6
#define pushbt	2
#define sendbt	3
using namespace std;

class ArduinoInputDecoder
{
public:
	AirConditionerRelay* machine;
	TimeStamp* timeStamp;
	UsageDataList* dataList;


	int command;

	ArduinoInputDecoder();

	void program();
	void timeCount();
};
