#include "OnIno.h"
#include "Arduino.h"

void OnIno::PushBtn()
{
	this->setState(1);
}

void OnIno::SomethingOpen()
{
	this->Deactivate();
	this->setState(0);
}


void OnIno::Nothing()
{
  this->setState(1);
}
