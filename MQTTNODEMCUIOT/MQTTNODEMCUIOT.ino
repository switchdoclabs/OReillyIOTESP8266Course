// NodeMCU IOT Device and Webserver
// For O'Reilly ESP8266 IOT Course
// John C. Shovic, PhD
// SwitchDoc Labs

#include <ESP8266WiFi.h>

#include "SDL_Arduino_Grove4Ch16BitADC/OWMAdafruit_ADS1015.h"
#include "SI114X.h"
#include "SDL_ESP8266_HR_AM2315/SDL_ESP8266_HR_AM2315.h"

#include "PubSubClient.h" // https://github.com/knolleary/pubsubclient/releases/tag/v2.3

// MQTT on Raspberry Pi
// Raspberry Pi Information


#define IOTVERSION 001

#define ORG "switchdoc"
#define DEVICE_TYPE "ENVIROIOT-01"
#define DEVICE_ID "1"


// setup for IOT Raspberry Pi


char serverIP[] = "192.168.1.60";
char topic[] = "ENVIROIOT";
char clientId[] = "ENVIROIOT";

Adafruit_ADS1115 ads1115;

SI114X SI1145 = SI114X();

SDL_ESP8266_HR_AM2315 am2315;

float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]
boolean myOK;  // 1=successful read


#include "AirQualityLibrary.h"


const char* ssid = "gracie";
const char* password = "1831faraday";

WiFiServer server(80);

// note different numbering for NodeMCU
#define RED_LED D0
#define BLUE_LED D3
#define SWITCH D8

// state variables
long currentAirQuality;
int currentAQSValue;
String EnvironmentalData;


void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);

  pinMode(BLUE_LED, OUTPUT);
  digitalWrite(BLUE_LED, LOW);

  pinMode(SWITCH, INPUT);


  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  Wire.begin();
  // Grove4Ch16BitADC Setup
  ads1115.begin();

  // Grove SI114X Setup
  while (!SI1145.Begin()) {
    Serial.println("Si1145 is not ready!");
    delay(1000);
  }
  Serial.println("Si1145 is ready!");


  // set up Grove AM2315 Temp/Humidity Sensor



  myOK = am2315.readData(dataAM2315);

  if (myOK) {
    Serial.print("Hum: "); Serial.println(dataAM2315[0]);
    Serial.print("TempC: "); Serial.println(dataAM2315[1]);
  }
  else
    Serial.println("AM2315 Sensor not found, check wiring & pullups!");


}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("callback invoked from ENVIRON IOT");
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }





  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  Serial.print("req=");
  Serial.println(req);
  // Match the request
  int val = -1; // We'll use 'val' to keep track of both the
  int blueval = -1;

  // request type (read/set) and value if set.
  if (req.indexOf("/led/0") != -1)
    val = 0; // Will write LED low
  else if (req.indexOf("/led/1") != -1)
    val = 1; // Will write LED high

  // request type (read/set) and value if set.
  if (req.indexOf("/ledblue/0") != -1)
    blueval = 0; // Will write LED low
  else if (req.indexOf("/ledblue/1") != -1)
    blueval = 1; // Will write LED high

  // set the LEDs
  Serial.print("val=");
  Serial.println(val);
  Serial.print("blueval=");
  Serial.println(blueval);

  if (val == 0)
  {
    digitalWrite(RED_LED, HIGH);   // high means off with RED LED

  }

  else if (val == 1)
  {
    digitalWrite(RED_LED, LOW);   // low means off with RED LED

  }

  if (blueval == 0)
  {
    digitalWrite(BLUE_LED, LOW);   // low means off with RED LED

  }
  else if (blueval == 1)
  {
    digitalWrite(BLUE_LED, HIGH);   // High means off with RED LED

  }

  Serial.println("------------------------------");
  Serial.println("Air Quality Sensor Read");
  Serial.println("------------------------------");
  currentAirQuality = getAirQuality();
  Serial.print("currentAirQuality=");
  Serial.println(currentAirQuality);
  currentAQSValue = getAQSSensorValue();
  Serial.print("currentAQSValue=");
  Serial.println(currentAQSValue);

  EnvironmentalData = String(currentAirQuality) + ",";
  EnvironmentalData += String(currentAQSValue) + ",";


  buildIOTWebPage(client);



  sendMQTT();

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");


  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}


void sendMQTT()
{

  WiFiClient wifiClient;
  PubSubClient MQTTclient(serverIP, 1883, callback,  wifiClient);

  if (!!!MQTTclient.connected()) {
    Serial.print("Reconnecting MQTTclient to ");
    Serial.println(serverIP);
    //while (!!!NQTTclient.connect(clientId, authMethod, token)) {
    while (!!!MQTTclient.connect(clientId)) {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
  }
  // Sending payload
  String payload;

  payload = "{\"d\":{\"ENVIROIOT\":\"EIOT-01\",";
  payload += "\"VER\":\"";
  payload += IOTVERSION;
  payload += "\",\"ENVIRODATA\":\"";
  payload += EnvironmentalData;
  payload += "\"";
  payload += "}}";

  Serial.print("Sending ENVIROIOT payload: ");
  Serial.println(payload);

  if (MQTTclient.publish(topic, (char*) payload.c_str())) {
    Serial.println("ENVIROIOT Publish ok");

  } else {
    Serial.println("ENVIROIOT Publish failed");

  }



}
void buildIOTWebPage(WiFiClient client)
{
  String HeaderPage;
  String ButtonsWebPage;
  String DataResponsePage;
  String BottomWebPage;

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one

  HeaderPage = "<!DOCTYPE HTML><html><h1>NodeMCU ESP8266 IOT Device Web Server</h1>";
  ButtonsWebPage = "<p>On-Board Red LED <a href=\"/led/1\"><button>ON</button></a>&nbsp;<a href=\"/led/0\"><button>OFF</button></a></p>";
  ButtonsWebPage +=  "<p>Grove Blue LED <a href=\"/ledblue/1\"><button>ON</button></a>&nbsp;<a href=\"/ledblue/0\"><button>OFF</button></a></p>";
  BottomWebPage += "</html>";

  // build data response section

  // Air Quality Sensor
  DataResponsePage = "<p>Air Quality State: " + getAirQualityName(currentAQSValue) + "</p>";
  DataResponsePage += "<p>Air Quality Sensor Value: " + String(currentAQSValue) + "</p>";


  // Read Switch State

  DataResponsePage += "<p>Switch State: ";
  if (digitalRead(SWITCH) == HIGH)
  {
    DataResponsePage += "ON";
    EnvironmentalData += String("ON") + ",";

  }
  else
  {
    DataResponsePage += "OFF";
    EnvironmentalData += String("OFF") + ",";
  }
  DataResponsePage += "</p>";

  // Read sunlight sensor
  float uV;
  int Vis;
  int IR;

  //the real UV value must be div 100 from the reg value , datasheet for more information.

  uV = (float)SI1145.ReadUV() / 100;
  Vis = SI1145.ReadVisible();
  IR = SI1145.ReadIR();
    EnvironmentalData += String(uV) + ",";
    EnvironmentalData += String(Vis) + ",";
    EnvironmentalData += String(IR) + ",";
  Serial.print("//--------------------------------------//\r\n");
  Serial.print("Vis: "); Serial.println(Vis);
  Serial.print("IR: "); Serial.println(IR);
  Serial.print("UV: ");  Serial.println(uV);

  DataResponsePage += "<p>Sunlight Visible: " + String(Vis) + "</pr>";
  DataResponsePage += "<p>Sunlight Infrared: " + String(IR) + "</pr>";
  DataResponsePage += "<p>Sunlight Ultra Violet: " + String(uV) + "</pr>";

  myOK = am2315.readData(dataAM2315);

  if (myOK) {
    Serial.print("Hum: "); Serial.println(dataAM2315[0]);
    Serial.print("TempC: "); Serial.println(dataAM2315[1]);
    DataResponsePage += "<p>Temperature: " + String(dataAM2315[1]) + "C</pr>";
    DataResponsePage += "<p>Humidity: " + String(dataAM2315[0]) + "%</pr>";
    EnvironmentalData += String(dataAM2315[1]) + ",";
    EnvironmentalData += String(dataAM2315[0]);
  }
  else
    Serial.println("AM2315 Sensor not found, check wiring & pullups!");
  client.println(HeaderPage + ButtonsWebPage + DataResponsePage + BottomWebPage);

  client.stop();
}




