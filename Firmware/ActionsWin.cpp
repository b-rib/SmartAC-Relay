#include "ActionsWin.h"


void ActionsWin::WinActivate()
{
	time_t timer;
	time_t now;
	int seconds = 0;

//	cout << "PUMP ON" << endl;
	
	time(&timer);  // get current time

	while (seconds < 4)
	{
		time(&now);
		seconds = difftime(now, timer);
	}

//	cout << "HEATER ON" << endl;
//	cout << "" << endl;
	
}

void ActionsWin::WinDeactivate()
{
	time_t timer;
	time_t now;
	int seconds = 0;

//	cout << "HEATER OFF" << endl;

	time(&timer);  // get current time

	while (seconds < 4)
	{
		time(&now);
		seconds = difftime(now, timer);
	}

//	cout << "PUMP OFF" << endl;
//	cout << "" << endl;
}
