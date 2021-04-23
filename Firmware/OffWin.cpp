#include "OffWin.h"

void OffWin::PushBtn()
{
	this->WinActivate();
	this->setState(1);
}

void OffWin::TimeOver()
{
	this->setState(0);
}

void OffWin::TotalTimeUpdateBtn()
{
	this->setState(0);
}
