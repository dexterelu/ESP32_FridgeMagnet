# ESP32_FridgeMagnet

Video here: https://www.youtube.com/watch?v=0v66mTCL1lw

**HARDWARE REQUIREMENTS**

- WEMOS LOLIN32-compatible Board with integrated SSD1306 OLED 

**OR**

- Generic ESP32 Board
- SSD1306 I2C OLED

**SOFTWARE REQUIREMENTS**
- Arduino IDE
- ESP32 Board Config with integrated WiFi Library
- Adafruit SSD1306 Library


**CONFIG**

- SSID - your wifi router's SSID

- PASS - password for above

- API_KEY - get your API KEY from here: https://home.openweathermap.org/users/sign_up

- LOCATION - city, country code (more info here: https://openweathermap.org/api )

- UNITS - metric or imperial

- gmtOffset_sec - how many seconds before or after GMT your location requires

- daylightOffset_sec - if your location uses daylight savings, this is where you enter the offset in seconds


**NEW FEATURES**

- TIMEOUT - auto-shutdown after a number of seconds



That's it! Config done, just compile and have fun!
