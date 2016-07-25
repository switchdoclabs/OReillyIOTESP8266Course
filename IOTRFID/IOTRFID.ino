

/*
    SwitchDoc Labs Code for IOT RFID
    IOT RFID uses publish subscribe to commmunicate to Raspberry Pi
    January 2016
*/

// BOF preprocessor bug prevent - insert on top of your arduino-code
#if 1
__asm volatile ("nop");
#endif

// Board options

#pragma GCC diagnostic ignored "-Wwrite-strings"

extern "C" {
#include "user_interface.h"
}
#include <ESP8266WiFi.h>
#include "PubSubClient.h" // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

#include <SoftwareSerial.h>
#include "seeedRFID.h"
SoftwareSerial SoftSerial(4, 5);


char RFIDbuffer[64]; // buffer array for data receive over serial port
int count = 0;   // counter for buffer array

//  Variables

int blinkPin = 0;                // pin to blink led at each reception of RFID code

#include "Utils.h"


//----------------------------------------------------------------------
//Local WiFi Variables

const char* ssid = "gracie";
//const char ssid[] = {"gracie"};
const char* password = "faraday01";

#define IOTRFIDVERSION 005


// Raspberry Pi Information

#define ORG "switchdoc"
#define DEVICE_TYPE "IOTRFID-01"
#define DEVICE_ID "1"
#define TOKEN "ul!fjH!y8y0gDREmsA"


// setup for IOT Raspberry Pi


char server[] = "192.168.1.60";
char topic[] = "ENVIROIOT";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "IOTRFID";

// for PN532 Board RFID Reader


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("callback invoked from IOT RFID");
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);


void setup() {
  // put your setup code here, to run once:

  // signal start of code - three quick blinks
  blinkLED(3, 250);

  Serial.begin(9600);             // we agree to talk fast!


  Serial.println("----------------");
  Serial.println("IOTRFID publish/subscribe Inventory");
  Serial.println("----------------");







  Serial.print("Connecting to ");
  Serial.print(ssid);
  if (strcmp (WiFi.SSID().c_str(), ssid) != 0) {
    WiFi.begin(ssid, password);
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.print("Local WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:


  clearBufferArray();              // call clearBufferArray function to clear the stored data from the array
  count = 0;


  if (!!!client.connected()) {
    Serial.print("Reconnecting client to ");
    Serial.println(server);
    //while (!!!client.connect(clientId, authMethod, token)) {
    while (!!!client.connect(clientId)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }

  // Check for RFID available

  String payload;


  // Sending payload

  payload = "{\"d\":{\"IOTRFID\":\"IR1\",";
  payload += "\"VER\":\"";
  payload += IOTRFIDVERSION;
  payload += "\",\"RFID_ID\":\"";
  payload += String("hello");
  payload += "\"";
  payload += "}}";


  // check for message


  clearBufferArray();              // call clearBufferArray function to clear the stored data from the array
  count = 0;

  Serial.print("Sending IOTRFID payload: ");
  Serial.println(payload);

  if (client.publish(topic, (char*) payload.c_str())) {
    Serial.println("IOTRFID Publish ok");

  } else {
    Serial.println("IOTRFID Publish failed");

  }


  yield();
}
