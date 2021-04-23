#include "OffIno.h"
#include "Arduino.h"

void OffIno::PushBtn()
{
	this->Activate();
	this->setState(1);
}

void OffIno::SomethingOpen()
{
	this->setState(0);
}

void OffIno::Nothing()
{
  this->setState(0);
}
