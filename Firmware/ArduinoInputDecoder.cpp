#include "ArduinoInputDecoder.h"
#include "Arduino.h"
#define noPwr  D5
#define dip1		D7
#define dip2		D6
#define serialDip	D8
#define pushbt		D2
#define sendbt		D6
#define equipmentID "0000"

ArduinoInputDecoder::ArduinoInputDecoder(int yr, int mnth, int dy, int hr, int mnt)
{
  machine = new AirConditionerRelayIno();
  timeStamp = new TimeStamp(yr, mnth, dy, hr, mnt);
  dataList = new UsageDataList();
  mqttPublish = false;
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
      while (digitalRead(pushbt) == HIGH) {
        delay(0);
      }
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
    while (!Serial) { delay(10);  /*Waiting serial connection*/  }
    Serial.println("Press SendButton when ready to transfer: ");
    while (digitalRead(sendbt) == LOW && digitalRead(serialDip) == HIGH) { delay(10); }

    if (digitalRead(sendbt) == HIGH) 
    {
      delay(200);
      while (digitalRead(sendbt) == HIGH) { delay(10); }

      Serial.println("Listing all");
      Serial.println(equipmentID);
      dataList->listAll();

      Serial.println("Press SendButton to delete data and/or switch dip to exit");
      while (digitalRead(serialDip) == HIGH)
      {
        delay(10);
        if (digitalRead(sendbt) == HIGH) 
        {
          delay(200);
          while (digitalRead(sendbt) == HIGH) { delay(10); }
    
          Serial.println("Deleting all");
          dataList->~UsageDataList();
          dataList = new UsageDataList();
          Serial.println("Deleted");
        }
      }
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
    mqttPublish = true;
    timeStamp->advanceTime();
    dataList->insertAfterLast('1', timeStamp->getYr(), timeStamp->getMnth(), timeStamp->getDay(), timeStamp->getHr(), timeStamp->getMin(), timeStamp->getSec());
  }

  if (machine->getState() && (command == 0)) //[0]SOMETHING OPEN
  {
    Serial.println("to list: Something open (turned off)");
    mqttPublish = true;
    timeStamp->advanceTime();
    dataList->insertAfterLast('0', timeStamp->getYr(), timeStamp->getMnth(), timeStamp->getDay(), timeStamp->getHr(), timeStamp->getMin(), timeStamp->getSec());
  }

  machine->HandleEvent(command);
  machine->updateState();
}
