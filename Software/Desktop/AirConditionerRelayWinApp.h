/*
	Class: AirConditionerRelayWinApp

	This is the main class of the Console App for Windows Desktop computers.
	Functionallities:
		- Establishes connection to a COM port
		- Reads data from serial port and stores it to a .txt file
		- Reads raw data from a .txt file, decodes, and stores it to another .txt file
		- Reads raw data from a .txt file and calculates "total minutes on" for a given time period
	Developed in: Mocrosoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include "Serial.h"
#include <fstream>
#include <string>
#include <sstream>
#include <memory.h>
#include <stdio.h>
#include <time.h>
using namespace std;

class AirConditionerRelayWinApp
{
protected:
	Serial* SPort;
public:
	AirConditionerRelayWinApp();
	void ReadAndStore();
	void fileDecode();
	void countTimeOn();
	void connectToCOMport();
	friend bool isCOMconnected(AirConditionerRelayWinApp thisApp);
	template <class T> void initializeAsZero (T thing)
	{
		memset(&thing, 0, sizeof thing);
	};
};

