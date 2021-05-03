/*
	Class: ArduinoInputDecoder

	Main class of the firmware.
	Decodes the system inputs and generates outputs.
	Developed in: ArduinoIDE (Windows 10 environment)
	Needs: Esp8266 NodeMCU core (open source)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "AirConditionerRelay.h"
#include "AirConditionerRelayIno.h"
#include "TimeStamp.h"
#include "UsageDataList.h"
//#include <iostream>

#define dip1	D7
#define dip2	D6
#define pushbt	D2
#define sendbt	D3
using namespace std;

class ArduinoInputDecoder
{
public:
	AirConditionerRelay* machine;
	TimeStamp* timeStamp;
	UsageDataList* dataList;

  bool mqttPublish;
	int command;

	ArduinoInputDecoder(int yr, int mnth, int dy, int hr, int mnt);

	void program();
	void timeCount();
};
