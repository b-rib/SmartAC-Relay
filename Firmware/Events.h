/*
	Class: Events

	Virtual class.
	Contains the events (inputs) of the FSM.
	Developed in: Microsoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
using namespace std;

class Events
{
public:
	virtual void SomethingOpen() { return; }   	//-0
	virtual void PushBtn() { return; }		   	//-1
	virtual void Nothing()  { return; }			//-10
};
