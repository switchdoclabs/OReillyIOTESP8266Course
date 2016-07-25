// Tests Grove4Ch16BitADC
// Read all four channels single ended
// SwitchDoc Labs
// May 2016
//

#include <Wire.h>
#include "OWMAdafruit_ADS1015.h"


Adafruit_ADS1115 ads;



void setup() {
  Serial.begin(115200);
  Wire.begin();


  Serial.println("------------------------------");
  Serial.println("------------------------------");

  Serial.println("Grove4Ch16BitADC Test");

  Serial.println("------------------------------");
  Serial.println("------------------------------");
  Serial.println();


  ads.begin();


}

void loop() {
  // put your main code here, to run repeatedly:



  Serial.println("------------------------------");
  int16_t ad0 = ads.readADC_SingleEnded(0);
  Serial.print("Channel 0 Voltage =");
  Serial.print(float(ad0) * 0.0001875);
  Serial.print("V ad0=0x");
  Serial.println(ad0, HEX);
  int16_t ad1 = ads.readADC_SingleEnded(1);
  Serial.print("Channel 1 Voltage =");
  Serial.print(float(ad1) * 0.0001875);
  Serial.print("V ad0=0x");
  Serial.println(ad1, HEX);
  int16_t ad2 = ads.readADC_SingleEnded(2);
  Serial.print("Channel 2 Voltage =");
  Serial.print(float(ad2) * 0.0001875);
  Serial.print("V ad2=0x");
  Serial.println(ad2, HEX);
  int16_t ad3 = ads.readADC_SingleEnded(3);
  Serial.print("Channel 3 Voltage =");
  Serial.print(float(ad3) * 0.0001875);
  Serial.print("V ad3=0x");
  Serial.println(ad3, HEX);
  Serial.println("------------------------------");
  delay(1000);
}
