// NodeMCU Simple IOT Device and Webserver
// For O'Reilly ESP8266 IOT Course
// John C. Shovic, PhD
// SwitchDoc Labs

#include <ESP8266WiFi.h>


#include "SDL_ESP8266_HR_AM2315/SDL_ESP8266_HR_AM2315.h"



SDL_ESP8266_HR_AM2315 am2315;

float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]
boolean myOK;  // 1=successful read

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


  // set up Grove AM2315 Temp/Humidity Sensor



  myOK = am2315.readData(dataAM2315);

  if (myOK) {
    Serial.print("Hum: "); Serial.println(dataAM2315[0]);
    Serial.print("TempC: "); Serial.println(dataAM2315[1]);
  }
  else
    Serial.println("AM2315 Sensor not found, check wiring & pullups!");


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


  buildIOTWebPage(client);

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");


  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

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

  HeaderPage = "<!DOCTYPE HTML><html><h1>NodeMCU ESP8266 Simple IOT Device Web Server</h1>";
  ButtonsWebPage = "<p>On-Board Red LED <a href=\"/led/1\"><button>ON</button></a>&nbsp;<a href=\"/led/0\"><button>OFF</button></a></p>";
  ButtonsWebPage +=  "<p>Grove Blue LED <a href=\"/ledblue/1\"><button>ON</button></a>&nbsp;<a href=\"/ledblue/0\"><button>OFF</button></a></p>";
  BottomWebPage += "</html>";

  // build data response section


  // Read Switch State

  DataResponsePage += "<p>Switch State: ";
  if (digitalRead(SWITCH) == HIGH)
  {
    DataResponsePage += "ON";
  }
  else
  {
    DataResponsePage += "OFF";
  }
  DataResponsePage += "</p>";


  myOK = am2315.readData(dataAM2315);

  if (myOK) {
    Serial.print("Hum: "); Serial.println(dataAM2315[0]);
    Serial.print("TempC: "); Serial.println(dataAM2315[1]);
    DataResponsePage += "<p>Temperature: " + String(dataAM2315[1]) + "C</pr>";
    DataResponsePage += "<p>Humidity: " + String(dataAM2315[0]) + "%</pr>";

  }
  else
    Serial.println("AM2315 Sensor not found, check wiring & pullups!");
  client.println(HeaderPage + ButtonsWebPage + DataResponsePage + BottomWebPage);

}


