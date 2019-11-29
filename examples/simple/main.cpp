
#include <Arduino.h>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include "config.h"
#include "ESPDateTime.h"

unsigned long lastMs = 0;

unsigned long ms = millis();

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println(millis());
  Serial.print("WiFi Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  setupWiFi();
  Serial.println(DateTime.toString());
  Serial.println(DateTime.toISOString());
  Serial.println(DateTime.toUTCString());
  Serial.println(millis());
  Serial.println("Time Syncing");
  DateTime.setTimeZone(DateTimeClass::TIMEZONE_CHINA);
  DateTime.begin();
  time_t now = DateTime.now();
  Serial.println(ctime(&now));
  DateTimeClass dt;
}

void loop() {
  if (millis() - ms > 10000) {
    ms = millis();
    Serial.println(DateTime.format(DateFormatter::COMPAT));
    Serial.println(DateTime.toString());
    Serial.println(DateTime.toISOString());
    Serial.println(DateTime.toUTCString());
    // DateTimeClass a(1574953106, 8);
    // DateTimeClass b(1574955106, 8);
    // auto c = a - 3600 * 10;
    // auto d = b + 3600 * 24;
    // Serial.println(a.toString());
    // Serial.println(b.toString());
    // Serial.println(c.toString());
    // Serial.println(d.toString());
    // Serial.println(a < b);
    Serial.println("----------");
  }
}