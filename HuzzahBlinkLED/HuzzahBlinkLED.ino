// Blink LEDs on Adafruit Huzzah
// For O'Reilly ESP8266 IOT Course
// John C. Shovic, PhD
// SwitchDoc Labs

#define RED_LED 0
#define BLUE_LED 1

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

}

void loop() {

  // put your main code here, to run repeatedly:
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  delay(500);

  digitalWrite(BLUE_LED, HIGH);
  delay(500);
  digitalWrite(BLUE_LED, LOW);
  delay(500);

}
