/*
	Class: ActionsIno

	Contains the actions of the implemented FSM.
	Developed in: ArduinoIDE (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#define airCond	D4
#define noPwr	D5
using namespace std;

class ActionsIno
{
public:
	void Activate();
	void Deactivate();
};
