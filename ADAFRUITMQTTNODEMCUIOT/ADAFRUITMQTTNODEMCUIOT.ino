// Adafruit.io NodeMCU IOT Device
// For O'Reilly ESP8266 IOT Course
// John C. Shovic, PhD
// SwitchDoc Labs

#include <ESP8266WiFi.h>

#include "SDL_Arduino_Grove4Ch16BitADC/OWMAdafruit_ADS1015.h"
#include "SI114X.h"
#include "SDL_ESP8266_HR_AM2315/SDL_ESP8266_HR_AM2315.h"



#include "Adafruit_MQTT_Library/Adafruit_MQTT.h"
#include "Adafruit_MQTT_Library/Adafruit_MQTT_Client.h"

//#include "PubSubClient.h" // https://github.com/knolleary/pubsubclient/releases/tag/v2.3




//
#define IOTVERSION 001

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Faraday_1024"

#define AIO_KEY         "640aeb48c0e64a66beae70e95dc82987"

// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/


// three feeds
//
#define FEEDTEMPERATURE  AIO_USERNAME "/feeds/iottemperature"
#define FEEDHUMIDITY AIO_USERNAME "/feeds/iothumidity"
#define FEEDAQS AIO_USERNAME "/feeds/iotaqs"

// Setup a feed called 'temperature' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char TEMPERATURE_FEED[] PROGMEM = FEEDTEMPERATURE;
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, TEMPERATURE_FEED);

// Setup a feed called 'humidity' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char HUMIDITY_FEED[] PROGMEM = FEEDHUMIDITY;
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, HUMIDITY_FEED);

// Setup a feed called 'Air Quality Sensor' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char AQS_FEED[] PROGMEM = FEEDAQS;
Adafruit_MQTT_Publish aqs = Adafruit_MQTT_Publish(&mqtt, AQS_FEED);



Adafruit_ADS1115 ads1115;

SI114X SI1145 = SI114X();

SDL_ESP8266_HR_AM2315 am2315;

float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]
boolean myOK;  // 1=successful read


#include "AirQualityLibrary.h"

const char* ssid = "gracie";
const char* password = "1831faraday";

WiFiServer server(80);


// state variables
long currentAirQuality;
int currentAQSValue;
String EnvironmentalData;


void setup() {
  Serial.begin(115200);
  Serial.println("---------Adafruit MQTT Starting--------");
  delay(10);

  digitalWrite(D0, HIGH);
  pinMode(D0, OUTPUT);

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
uint32_t x = 0;
void loop() {


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


  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  /*
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) {
      if (subscription == &onoffbutton) {
        Serial.print(F("Got: "));
        Serial.println((char *)onoffbutton.lastread);
      }
    }
  */
  myOK = am2315.readData(dataAM2315);

  if (myOK) {
    Serial.print("Hum: "); Serial.println(dataAM2315[0]);
    Serial.print("TempC: "); Serial.println(dataAM2315[1]);
  }
  else
    Serial.println("AM2315 Sensor not found, check wiring & pullups!");
  // Now we can publish stuff!
  Serial.print(F("\nSending humdity val "));

  Serial.print("...");

  if (! humidity.publish(dataAM2315[0])) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  Serial.print(F("\nSending temperature val "));

  Serial.print("...");
  if (! temperature.publish(dataAM2315[1])) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  digitalWrite(D0, HIGH);
  delay(200);
  digitalWrite(D0, LOW);
  delay(200);
  digitalWrite(D0, HIGH);
  delay(200);

  Serial.print(F("\nSending air qualityl "));

  Serial.print("...");
  if (! aqs.publish(currentAQSValue)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }


  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
    if(! mqtt.ping()) {
    mqtt.disconnect();
    }
  */

  delay(5000);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}


