#include "OnWin.h"

void OnWin::PushBtn()
{
	this->setState(1);
}

void OnWin::TimeOver()
{
	this->WinDeactivate();
	this->setState(0);
}

void OnWin::TotalTimeUpdateBtn()
{
	this->setState(1);
}
