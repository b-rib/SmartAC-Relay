#pragma once
#include "Serial.h"
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class AirConditionerRelayWinApp
{
protected:
	Serial* SPort;
public:
	AirConditionerRelayWinApp();
	void ReadAndStore();
	void fileDecode();

};

