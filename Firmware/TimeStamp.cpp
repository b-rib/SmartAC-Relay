#include "TimeStamp.h"
#include "Arduino.h"

TimeStamp::TimeStamp(int yr, int mnth, int dy, int hr, int mnt)
{
	secondino = 0;
	minuteino = mnt;
	hourino = hr;
	dayino = dy;
	monthino = mnth;
	yearino = yr;

	pastMeasure = millis();

}

void TimeStamp::advanceTime()
{
	int secondsToAdd;
	measure = millis();

	secondsToAdd = static_cast<int>((measure - pastMeasure)/1000);
  if (secondsToAdd > 0)
  {
    secondino = secondino + secondsToAdd;
    pastMeasure = measure;
    //Serial.println(secondino);  //FOR DEBUGGING
  }
  

	while (secondino >= 60)
	{
		minuteino = minuteino + 1;
		secondino = secondino - 60;
	}

	while (minuteino >= 60)
	{
		hourino = hourino + 1;
		minuteino = minuteino - 60;
	}


	while (hourino >= 24)
	{
		dayino = dayino + 1;
		hourino = hourino - 24;
	}

	//Months treatment
	if (dayino == 32 and (monthino == 1 || monthino == 3 || monthino == 5 || monthino == 7 || monthino == 8 || monthino == 10 || monthino == 12))
	{
		monthino = monthino + 1; //This can tunr month into = 13
		dayino = dayino - 31;
	}

	if (dayino == 31 and (monthino == 4 || monthino == 6 || monthino == 9 || monthino == 11))
	{
		monthino = monthino + 1;
		dayino = dayino - 30;
	}

	if (monthino == 2 )
	{
		if (dayino == 29 and !(yearino == 2024 || yearino == 2028 || yearino == 2032 || yearino == 2036 || yearino == 2040))
		{
			monthino = monthino + 1;
			dayino = dayino - 28;
		}

		if (dayino == 30 and (yearino == 2024 || yearino == 2028 || yearino == 2032 || yearino == 2036 || yearino == 2040))
		{
			monthino = monthino + 1;
			dayino = dayino - 29;
		}
	}

	//Year update
	if (monthino == 13)
	{
		yearino = yearino + 1;
		monthino = monthino - 12;
	}
	
}

int TimeStamp::getSec()
{
	return secondino;
}

int TimeStamp::getMin()
{
	return minuteino;
}

int TimeStamp::getHr()
{
	return hourino;
}

int TimeStamp::getDay()
{
	return dayino;
}

int TimeStamp::getMnth()
{
	return monthino;
}

int TimeStamp::getYr()
{
	return yearino;
}
