/*
	main.cpp

	Console App for Windows Desktop computers.
	It receives data from a Smart Relay through serial communication.
	Developed in: Mocrosoft Visual Studio 2017 (Windows 10 environment)
	Developed by: Brenda Ribeiro
*/

#pragma once
#include <iostream>
#include <fstream>
#include "Serial.h"
#include "AirConditionerRelayWinApp.h"

using namespace std;

AirConditionerRelayWinApp app;
int choice = -1;
bool leave = false;

void main() 
{
	while (!leave)
	{
		cout << endl;
		cout << endl;
		if (!isCOMconnected(app))
		{
			cout << "[0] Connect to COM port" << endl;
		}
		cout << "[1] Read and store" << endl;
		cout << "[2] Decode file" << endl;
		cout << "[3] Calculate 'time on' for a given time interval" << endl;
		cout << "[4] Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		cout << endl;

		try
		{
			if (choice >= 0 && choice <= 4) 
			{
				switch (choice)
				{
				case 0:
					app.connectToCOMport();
					break;
				case 1:
					app.ReadAndStore();
					break;
				case 2:
					app.fileDecode();
					break;
				case 3:
					app.countTimeOn();
					break;
				case 4:
					leave = true;
					break;
				}
			}
			else 
			{
				throw (choice);
			}
		}
		catch (...)
		{
			cout << "Please choose a valid option" << endl;
		}
		
	}
}

bool isCOMconnected(AirConditionerRelayWinApp thisApp) {
	if (thisApp.SPort != nullptr)
	{
		return true;
	}
	else
	{
		return false;
	}
}