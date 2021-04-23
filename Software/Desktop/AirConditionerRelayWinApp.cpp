#include "AirConditionerRelayWinApp.h"

AirConditionerRelayWinApp::AirConditionerRelayWinApp()
{
	printf("TIME RELAY: WINDOWS APP FOR SERIAL DATA ACQUIREMENT\n\n");
	
	char portName[5];

	printf("Insert port name (COMx): ");
	cin >> portName;
	SPort = new Serial(portName);

	if (SPort->IsConnected())
	{
		printf("Connecting...\nConnected to %s\n\n", portName);
	}
		
}

void AirConditionerRelayWinApp::ReadAndStore()
{
	char incomingData[256] = "";	// Buffer. Pre-allocated memory.
	int dataLength = 255;
	int readResult = 0;

	bool read = true;
	bool store = false;
	char id[8] = "";

	printf("Insert equipment ID as filename (max. 4 characters 'xxxx.txt'): ");
	cin >> id;

	ofstream file(id, ios_base::app | ios_base::ate);

	while (SPort->IsConnected() && read)
	{
		readResult = SPort->ReadData(incomingData, dataLength);
		// printf("Bytes read: (0 means no data available) %i\n",readResult);
		incomingData[readResult] = 0;

		if (incomingData[13] == '1' || incomingData[13] == '0')
		{
			printf("----STARTED-----\n");
			store = true;
		}

		int endOfData;
		for (size_t i = 13; i < 256; i++)
		{
			if (incomingData[i] == 'S')
			{
				endOfData = i - 1;
			}
		}

		printf("%s", incomingData);
		//printf("%s", incomingData[12]);

		if (store)
		{
			for (size_t i = 12; i < endOfData; i++)
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
	char idIn[8] = "";
	char idOut[15] = "";

	printf("\nInsert the name of the input file: (xxxx.txt): ");
	cin >> idIn;
	ifstream inFile(idIn);

	printf("\nInsert the name of the output file: (suggestion: xxxxDecoded.txt): ");
	cin >> idOut;
	ofstream outFile(idOut, ios_base::app | ios_base::ate);

	stringstream lineStream;
	string line;
	string word;

	outFile.open(idOut, ios_base::app | ios_base::ate);
	outFile.close();
	/*outFile.open(idOut, ios_base::app | ios_base::ate);
	outFile << "Event,				YYYY/MM/DD,	HH:MM:SS" << endl;
	outFile.close();*/

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
