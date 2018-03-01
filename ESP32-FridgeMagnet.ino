#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
//#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <Wire.h>
#include "SSD1306.h"
#include "wifi.h"
#include "time.h"
#include "din1451_16.h"
#include "din1451_36.h"
     
      // CONFIG
      static const char ssid[20] = "YOUR SSID HERE"; 
      static const char pass[20] = "PASSWORD HERE";
      String API_KEY = "b8b8cdd41a9b624ad13b6496b7bb2de7";
      String LOCATION = "Bucharest,RO";
      String API_CALL = "http://api.openweathermap.org/data/2.5/weather";
      String UNITS = "metric";
      const int TIMEOUT = 20; // seconds to shutdown
      
      const char* ntpServer = "pool.ntp.org";
      const long  gmtOffset_sec = 7200;
      const int   daylightOffset_sec = 3600;

String WD[8] = {"-", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
char deg[3] = { 0xC2, 0xB0, 0x00 };
#define BUFFER_SIZE (4000)
#define NUMITEMS(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0])))

#define WiFi_Logo_width 60
#define WiFi_Logo_height 36

int x = 0;
SSD1306  display(0x3c, 5, 4);
HTTPClient http;

const char WiFi_Logo_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF,
  0xFF, 0x03, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0xFF, 0xFF, 0xFF, 0x07, 0xC0, 0x83, 0x01, 0x80, 0xFF, 0xFF, 0xFF,
  0x01, 0x00, 0x07, 0x00, 0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x0C, 0x00,
  0xC0, 0xFF, 0xFF, 0x7C, 0x00, 0x60, 0x0C, 0x00, 0xC0, 0x31, 0x46, 0x7C,
  0xFC, 0x77, 0x08, 0x00, 0xE0, 0x23, 0xC6, 0x3C, 0xFC, 0x67, 0x18, 0x00,
  0xE0, 0x23, 0xE4, 0x3F, 0x1C, 0x00, 0x18, 0x00, 0xE0, 0x23, 0x60, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xE0, 0x03, 0x60, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x07, 0x60, 0x3C, 0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C,
  0xFC, 0x73, 0x18, 0x00, 0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00,
  0xE0, 0x87, 0x70, 0x3C, 0x1C, 0x70, 0x18, 0x00, 0xE0, 0x8F, 0x71, 0x3C,
  0x1C, 0x70, 0x18, 0x00, 0xC0, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x08, 0x00,
  0xC0, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x0C, 0x00, 0x80, 0xFF, 0xFF, 0x1F,
  0x00, 0x00, 0x06, 0x00, 0x80, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0x07, 0x00,
  0x00, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0xF8, 0xFF, 0xFF,
  0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xFF, 0xFF, 0x01, 0x00, 0x00,
  0x00, 0x00, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0xFF,
  0x7F, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xFF, 0x1F, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x80, 0xFF, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  


String GetURL(void) {
  return API_CALL + "?q=" + LOCATION + "&appid=" + API_KEY + "&units=" + UNITS;
}

void showProgress(void) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawXbm(34, 16, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
    display.drawString(64, 0, "Weather in " + LOCATION);
    display.fillRect(x * 4 - 5, 60, x + 10, 4);
    display.display();
    x++;
}

void setup() {
    display.init();
    showProgress();
    delay(200);
    Serial.begin(115200);
    //wifiMulti.addAP(ssid, pass);
    WiFi.begin(ssid, pass);
    showProgress();
    delay(200);
    
    //while((wifiMulti.run() != WL_CONNECTED)) {
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        showProgress();
        delay(200);
    }
    Serial.println("\r\nconnected");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    showProgress();
    Serial.println("NTP OK");
}

void loop() {
    int shown = 0;
    
    // wait for WiFi connection
    //if((wifiMulti.run() == WL_CONNECTED)) {
    if (WiFi.status() == WL_CONNECTED) {
        
        http.begin(GetURL()); //HTTP
        Serial.println("HTTP connected");
        showProgress();
        
        // start connection and send HTTP header
        int httpCode = http.GET();
        Serial.println("HTTP content loaded");
        showProgress();
        
        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                Serial.println(payload);
                DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);
                JsonObject& root = jsonBuffer.parseObject(payload);
                String w1 = root["weather"][0]["main"].as<char*>();
                String w2 = root["weather"][1]["main"].as<char*>();
                String w3 = root["weather"][2]["main"].as<char*>();
                String temp = root["main"]["temp"].as<char*>();
                String humidity = root["main"]["humidity"].as<char*>();
                String unix = root["dt"].as<char*>();
                unsigned long epoch = unix.toInt() + 7200;
                int hour = (epoch % 86400L) / 3600;
                int minute = (epoch % 3600) / 60;
                String h = hour < 10 ? "0" + String(hour) : String(hour);
                String m = minute < 10 ? "0" + String(minute) : String(minute);
                String type = "";
                if(w3.length() > 0) {
                  type = w1 + "/" + w2 + "/" + w3;
                } else if (w2.length() > 0) {
                   type = w1 + "/" + w2;
                } else if (w1.length() > 0) {
                   type = w1;
                }

                String line1 = temp + deg + "C" + ", " + type; //, " + humidity + "%";
                char line2[30] = "Cannot get date";
                char line3[30] = "Cannot get time";
                
                struct tm timeinfo;

                for(shown = 0; shown < TIMEOUT; shown++) {
                    if(getLocalTime(&timeinfo)) {
                        sprintf(line2, " %04d-%02d-%02d", 1900 + timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday);
                        sprintf(line3, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
                    }
                    
                    Serial.println(line1);
                    Serial.println(WD[timeinfo.tm_wday] + line2);
                    Serial.println(line3);
                    
                    display.clear();
                    display.setTextAlignment(TEXT_ALIGN_CENTER);
                    
                    display.setFont(DIN_16);
                    display.drawString(64, 0, line1);
    
                    display.setFont(DIN_36);
                    display.drawString(64, 13, line3);
                    
                    display.setFont(ArialMT_Plain_10);
                    display.drawString(64, 52, WD[timeinfo.tm_wday] + ", " + line2);
                    display.display();
                    
                    delay(1000);
                }
                display.clear();
                display.display();
                
                http.end();
                WiFi.disconnect(true);
                WiFi.mode(WIFI_OFF);
                
                esp_deep_sleep_start();
                
                delay(600 * 1000);
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }   
    }
}
