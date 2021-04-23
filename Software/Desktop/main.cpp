#pragma once
#include <iostream>
#include <fstream>
#include "Serial.h"
#include "AirConditionerRelayWinApp.h"

using namespace std;

AirConditionerRelayWinApp app;
int choice = 0;
bool leave = false;

void main() 
{
	while (!leave)
	{
		cout << "\n\n[1] Read and store" << endl;
		cout << "[2] Decode file" << endl;
		cout << "[3] Exit" << endl;
		cout << "Choice: ";
		cin >> choice;
		cout << endl;

		switch (choice)
		{
		case 1:
			app.ReadAndStore();
			break;
		case 2:
			app.fileDecode();
			break;
		case 3:
			leave = true;
			break;
		default:
			break;
		}
	}
}