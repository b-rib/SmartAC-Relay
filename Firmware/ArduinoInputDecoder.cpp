#include "ArduinoInputDecoder.h"
#include "Arduino.h"
#define noPwr  5
#define dip1		7
#define dip2		6
#define serialDip	8
#define pushbt		2
#define sendbt		3


ArduinoInputDecoder::ArduinoInputDecoder()
{
  machine = new AirConditionerRelayIno();
  timeStamp = new TimeStamp(2021, 4, 10, 9, 55);
  dataList = new UsageDataList();
}

void ArduinoInputDecoder::program()
{

  command = 10; //Matches none of the commands (0, 1)

  if (digitalRead(dip1) == HIGH)
  {
    if (digitalRead(pushbt) == HIGH)
    {
      command = 1;
      delay(100);
      Serial.println("Push button pressed");
      while (digitalRead(pushbt) == HIGH){;}
    }
    digitalWrite(noPwr, LOW);
  }

  if (digitalRead(dip1) == LOW)
  {
	  command = 0;
    digitalWrite(noPwr, HIGH);
	  //Serial.println("Something is open");
  }

  //Serial Communication
  while (digitalRead(serialDip) == HIGH)
  {
    int serialAnswer;
	  Serial.begin(19200);
	  while (!Serial) { ; } //Waiting serial connection
	  Serial.println("Press SendButton when ready to transfer: ");
	  while (digitalRead(sendbt) == LOW && digitalRead(serialDip) == HIGH) { ; }
	  if (digitalRead(sendbt) == HIGH) {
      delay(200);
      while (digitalRead(sendbt) == HIGH) { ; }
      
      Serial.println("Listing all");
			dataList->listAll();
        
		  Serial.println("Switch dip to exit");
		  while (digitalRead(serialDip) == HIGH) { ; }
	  }
  }

  timeCount();
}

void ArduinoInputDecoder::timeCount()
{
	timeStamp->advanceTime();


  if (!(machine->getState()) && (command == 1)) //[1]BUTTON PRESSED WHILE MACHINE IS OFF
  {
	Serial.println("to list: Turned on");
	timeStamp->advanceTime();
	dataList->insertAfterLast('1', timeStamp->getYr(), timeStamp->getMnth(), timeStamp->getDay(), timeStamp->getHr(), timeStamp->getMin(), timeStamp->getSec());
  }

  if (machine->getState() && (command == 0)) //[0]SOMETHING OPEN
  {
	Serial.println("to list: Something open (turned off)");
	timeStamp->advanceTime();
	dataList->insertAfterLast('0', timeStamp->getYr(), timeStamp->getMnth(), timeStamp->getDay(), timeStamp->getHr(), timeStamp->getMin(), timeStamp->getSec());
  }
  
  machine->HandleEvent(command);
  machine->updateState();
}
