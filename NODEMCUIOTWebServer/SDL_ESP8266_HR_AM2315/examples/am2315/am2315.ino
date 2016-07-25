#include <Wire.h>
#include <SDL_ESP8266_HR_AM2315.h>

/*************************************************** 
  This is an example for the AM2315 Humidity + Temp sensor
  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> https://www.adafruit.com/products/1293
  These displays use I2C to communicate, 2 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!
  Written by Limor Fried/Ladyada for Adafruit Industries.  
  
  Additional improvements added by Daniel Sandoval/EasternStarGeek  
  8 July, 2013.  (Parent Library: Adafruit_AM2315)

  Additional improvements April 2016, Switchdoc.com
  
  
  This version gets the Temperature and Humidity data with a single read, and is
  believed to solve some prior timing problems in the original.
  
  Be careful with return data array indexing!  Follow this example closely.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Connect RED of the AM2315 sensor to 5.0V
// Connect BLACK to Ground
// Connect WHITE to i2c clock - 
// Connect YELLOW to i2c data - 

// BOF preprocessor bug prevent - insert on top of your arduino-code
#if 1
__asm volatile ("nop");
#endif

// Board options

#pragma GCC diagnostic ignored "-Wwrite-strings"

extern "C" {
#include "user_interface.h"
}

SDL_ESP8266_HR_AM2315 am2315;

float dataAM2315[2];  //Array to hold data returned by sensor.  [0,1] => [Humidity, Temperature]
boolean myOK;  // 1=successful read

void setup() {
  Serial.begin(9600);
  Serial.println("AM2315 Test!");
 }

void loop() {
  myOK = am2315.readData(dataAM2315);
  
  if (myOK) {
  Serial.print("Hum: "); Serial.println(dataAM2315[0]);
  Serial.print("TempF: "); Serial.println(dataAM2315[1]);
  }
else
Serial.println("Sensor not found, check wiring & pullups!");

  delay(1000);
}
