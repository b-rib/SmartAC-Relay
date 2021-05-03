/*
	Class: AirConditionerRelayWinApp

	Check header file for more details
*/

#include "AirConditionerRelayWinApp.h"
#include <cstdio>

AirConditionerRelayWinApp::AirConditionerRelayWinApp()
{
	printf("TIME RELAY: WINDOWS APP FOR SERIAL DATA ACQUIREMENT\n\n");
		
}

void AirConditionerRelayWinApp::ReadAndStore()
{
	char incomingData[2000] = "";	// Buffer. Pre-allocated memory.
	int dataLength = 1999;
	int readResult = 0;

	bool read = true;
	bool store = false;
	char id[9] = "";

	int beginOfName = 0;
	int endOfData = 0;

	ofstream file;

	while (SPort->IsConnected() && read)
	{
		Sleep(2000);
		readResult = SPort->ReadData(incomingData, dataLength);
		incomingData[readResult] = 0;

		//Making filename
		for (size_t i = 0; i < (dataLength + 1); i++)
		{
			if (incomingData[i] == 'l' && incomingData[i-1] == 'l')
			{
				beginOfName = i + 3;
			}
		}
		if (beginOfName > 0)
		{
			printf("----STARTED-----\n");
			store = true;
			id[0] = incomingData[beginOfName];
			id[1] = incomingData[beginOfName + 1];
			id[2] = incomingData[beginOfName + 2];
			id[3] = incomingData[beginOfName + 3];
			id[4] = '.';
			id[5] = 't';
			id[6] = 'x';
			id[7] = 't';
			id[8] = '\0';
			file = ofstream(id, ios_base::app | ios_base::ate);
		}
		//ofstream file(id, ios_base::app | ios_base::ate);

		//Finding where data ends
		for (size_t i = 13; i < (dataLength + 1); i++)
		{
			if (incomingData[i] == 'P')
			{
				endOfData = i - 1;
			}
		}

		printf("%s", incomingData);

		if (store && endOfData > 0)
		{
			for (int i = (beginOfName + 4); i < endOfData; i++)
			{
				if (incomingData[i] != '\r')
				{
					file.open(id, ios_base::ate | ios_base::app);
					file << incomingData[i];
					file.close();
				}
			}
		}

		if (incomingData[readResult - 3] == 't')
		{
			printf("-----FINISHED-----\n");
			read = false;
			store = false;
		}

		Sleep(500);
	}
}

void AirConditionerRelayWinApp::fileDecode()
{
	char idIn[9] = "";
	char idOut[22] = "";

	printf("\nInsert equipment ID (xxxx): ");
	cin >> idIn;

	sprintf_s(idOut, "%sDecoded.txt", idIn);
	ofstream outFile(idOut, ios_base::app | ios_base::ate);

	//idIn[4] = '.';	idIn[5] = 't';	idIn[6] = 'x';	idIn[7] = 't';	idIn[8] = '\0';
	sprintf_s(idIn, "%s.txt", idIn);
	ifstream inFile(idIn);

	stringstream lineStream;
	string line;
	string word;

	outFile.open(idOut, ios_base::app | ios_base::ate);
	outFile.close();

	int i;
	
	while (getline(inFile, line, '\n')) //Reading each line
	{
		lineStream.str(line);
		//cout << lineStream.str();
		//cout << line;
		i = 0;
		while (getline(lineStream, word, ',') || i < 7) //Reading each word
		{
			//cout << word;
			if (i == 0) //Then its the 1st word of the line
			{
				if (word[0] == '0')
				{
					outFile.open(idOut, ios_base::app | ios_base::ate);
					outFile << "Something open (turned OFF)" << ",	";
					outFile.close();
				}
				if (word[0] == '1')
				{
					outFile.open(idOut, ios_base::app | ios_base::ate);
					outFile << "Button pressed (turned ON)" << ",	";
					outFile.close();
				}
			}

			if (word.size() == 1 && word != "\r" && word != "\n")
			{
				word = '0' + word;
			}

			if (word.size() < 6 && word.size() > 1)
			{
				if (i == 1 || i == 2) //Then its the 2nd or 3rd word of the line
				{
					outFile.open(idOut, ios_base::app);
					outFile << word << "/";
					outFile.close();
				}

				if (i == 3) //Then its the 4th word of the line
				{
					outFile.open(idOut, ios_base::app);
					outFile << word << ",	";
					outFile.close();
				}

				if (i == 4 || i == 5) //Then its the 5th or 6th word of the line
				{
					outFile.open(idOut, ios_base::app);
					outFile << word << ":";
					outFile.close();
				}

				if (i == 6) //Then its the 7th word of the line (LAST)
				{
					outFile.open(idOut, ios_base::app);
					outFile << word << endl;
					outFile.close();
				}
			}
			i++;
		}
		lineStream.clear();//clear any bits set
		lineStream.str(std::string());//Flushing
	}
	
	inFile.close();
	cout << "\nFile decoded.";

}

void AirConditionerRelayWinApp::countTimeOn()
{
	char idIn[9] = "";
	char timeIn[11] = "";

	struct tm startingTime {};
	//memset(&startingTime, 0, sizeof startingTime);
	initializeAsZero(startingTime);
	struct tm endingTime {};
	//memset(&endingTime, 0, sizeof endingTime);
	initializeAsZero(endingTime);

	int lastPointEvent = 2;
	struct tm lastPointTime {};
	//memset(&lastPointTime, 0, sizeof lastPointTime);
	initializeAsZero(lastPointTime);

	int thisPointEvent = 2;
	struct tm thisPointTime {};
	//memset(&thisPointTime, 0, sizeof thisPointTime);
	initializeAsZero(thisPointTime);

	double minutesOn = 0;

	printf("\nInsert equipment ID (xxxx): ");
	cin >> idIn;

	printf("\nInsert starting time (dd/mm/yyyy): ");
	cin >> timeIn;
	sscanf_s(timeIn, "%d/%d/%d", &startingTime.tm_mday, &startingTime.tm_mon, &startingTime.tm_year);
	startingTime.tm_year -= 1900;
	startingTime.tm_mon--;
	//memset(timeIn, 0, sizeof timeIn);
	initializeAsZero(timeIn);

	printf("\nInsert ending time (dd/mm/yyyy): ");
	cin >> timeIn;
	sscanf_s(timeIn, "%d/%d/%d", &endingTime.tm_mday, &endingTime.tm_mon, &endingTime.tm_year);
	endingTime.tm_year -= 1900;
	endingTime.tm_mon--;

	sprintf_s(idIn, "%s.txt", idIn);
	ifstream inFile(idIn);

	string line;

	while (getline(inFile, line, '\n')) //Reading each line
	{
		char * lineAsChar = new char[line.length() + 1];
		strcpy_s(lineAsChar, (line.length() + 1), line.c_str());

		sscanf_s(lineAsChar, "%d,%d,%d,%d,%d,%d,%d", &thisPointEvent, &thisPointTime.tm_year, &thisPointTime.tm_mon, &thisPointTime.tm_mday, &thisPointTime.tm_hour, &thisPointTime.tm_min, &thisPointTime.tm_sec);
		thisPointTime.tm_year -= 1900;
		thisPointTime.tm_mon--;


		if (thisPointEvent == 0)
		{
			if (lastPointEvent == 1 &&
				mktime(&lastPointTime) < mktime(&startingTime) &&
				mktime(&thisPointTime) < mktime(&endingTime) &&
				mktime(&thisPointTime) > mktime(&startingTime))
			{
				minutesOn = minutesOn + difftime(mktime(&thisPointTime), mktime(&startingTime));
			}

			if (lastPointEvent == 1 &&
				mktime(&lastPointTime) > mktime(&startingTime) &&
				mktime(&lastPointTime) < mktime(&endingTime) &&
				mktime(&thisPointTime) > mktime(&endingTime))
			{
				minutesOn = minutesOn + difftime(mktime(&endingTime), mktime(&lastPointTime));
			}

			if (lastPointEvent == 1 &&
				mktime(&lastPointTime) > mktime(&startingTime) &&
				mktime(&lastPointTime) < mktime(&endingTime) &&
				mktime(&thisPointTime) > mktime(&startingTime) &&
				mktime(&thisPointTime) < mktime(&endingTime))
			{
				minutesOn = minutesOn + difftime(mktime(&thisPointTime), mktime(&lastPointTime));
			}
		}
		lastPointTime = thisPointTime;
		lastPointEvent = thisPointEvent;
	}
	minutesOn = minutesOn / 60;
	inFile.close();
	printf("\nFor the given time interval, the total time on is: %.2f minutes.\n", minutesOn);
}

void AirConditionerRelayWinApp::connectToCOMport()
{
	char portName[5];

	printf("Insert port name (COMx): ");
	cin >> portName;
	SPort = new Serial(portName);

	if (SPort->IsConnected())
	{
		printf("Connected to %s\n\n", portName);
	}
}
