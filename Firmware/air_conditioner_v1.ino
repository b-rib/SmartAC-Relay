/*
  Smart Relay (for AC units) Firmware

  Developed in: ArduinoIDE (Windows 10 environment)
  Needs: Esp8266 NodeMCU core (open source)
  Developed by: Brenda Ribeiro
*/

// Type here the Wi-Fi information____________________
#define wifiName "BrendaAP-2.4Ghz"                 //|
#define wifiPswd "redeBrendaAP"                    //|

// Type here the MQTT broker__________________________
#define mqttServer "broker.hivemq.com"             //|

// Type here the timestamp settings___________________
#define setYear   2021                             //|
#define setMonth  4                                //|
#define setDay    30                               //|
#define setHour   10                               //|
#define setMin    10                               //|
//__________________________________________________/|
// Thecnical information, from this point forward, do not change
//(ESP8266 NodeMCU I/O ports)
#define airCond     D4
#define noPwr       D5
#define dip1    D7
#define dip2    D6
#define serialDip  D8
#define pushbt   D2
#define sendbt  D6


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ArduinoInputDecoder.h"

const char* ssid = wifiName;
const char* password = wifiPswd;
const char* mqtt_server = mqttServer;
char msg[2] = "";

WiFiClient espClient;
PubSubClient client(espClient);

ArduinoInputDecoder ESPboard {setYear, setMonth, setDay, setHour, setMin};

//-----Initializations, executed once-----
void setup() {
  //Pin initialization
  pinMode(airCond, OUTPUT);
  pinMode(noPwr, OUTPUT);
  pinMode(dip1, INPUT);
  pinMode(dip2, INPUT);
  pinMode(serialDip, INPUT);
  pinMode(pushbt, INPUT);
  pinMode(sendbt, INPUT);

  //Serial initialization
  Serial.begin(19200);

  //Wifi initialization
  Serial.println("Connecting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.status());
    delay(5000);
  }
  randomSeed(micros());
  Serial.print("Connected to Wifi. Status: ");
  Serial.println(WiFi.status());

  //MQTT initialization
  client.setServer(mqtt_server, 1883);
}


//-----Main program, executed in loop-----
void loop() {
  ESPboard.program();
  
  //Publish to MQTT broker
  mqttPublish();
}




//-----MQTT functions, used in loop()-----
void mqttConnect() {
  while (!client.connected())
  {
    // Attempt to connect
    if (client.connect("AC0000"))
    {
      Serial.println("Connected to mqtt broker");
    } else
    {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      delay(5000);
    }
  }
  client.loop();
}

void mqttPublish() {
  if (ESPboard.mqttPublish == true)
  {
    mqttConnect();

    sprintf(msg, "%d", ESPboard.command);
    client.publish("ACsmartRelay/device0000", msg, true);
    delay(2000);
    ESPboard.mqttPublish = false;
    Serial.print("Posted to mqtt topic: ");
    Serial.println(msg);
  }
}
