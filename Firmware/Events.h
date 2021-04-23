//CLASS Events.h
	//Virtual class.
	//Contains the events of the FSM
#pragma once
//#include <iostream>
using namespace std;

class Events
{
public:
	virtual void SomethingOpen() { return; }   	//-0
	virtual void PushBtn() { return; }		   	//-1
	virtual void Nothing()  { return; }			//-10
};
