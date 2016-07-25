Highly Reliable driver for the ESP8266

SwitchDoc Labs   April 2016
www.switchdoc.com

Version 1.1 April 29, 2016 - Changed class names to avoid library conflicts, removed commented code


See the AM2315 ESP8266 posting on www.switchdoc.com

Improved to work with both SwitchDoc Labs AM2315 and Adafruit AM2315

This is a library for the AM2315 Humidity + Temp sensor

Note: Some ESP8266 modules are wired backwards (4 is 5 and 5 is 4). It this doesn't work, try the other way

Wire.begin(4, 5);

OR

Wire.begin(5, 4);

