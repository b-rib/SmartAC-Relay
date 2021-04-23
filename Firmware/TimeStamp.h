#pragma once
#include <time.h>
using namespace std;

class TimeStamp
{
	int measure;
	int pastMeasure;

	int secondino;
	int minuteino;
	int hourino;
	int dayino;
	int monthino;
	int yearino;
public:
	TimeStamp(int yr, int mnth, int dy, int hr, int mnt);
	void advanceTime();
	int getSec();
	int getMin();
	int getHr();
	int getDay();
	int getMnth();
	int getYr();

};
