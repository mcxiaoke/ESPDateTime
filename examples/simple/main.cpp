
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
// #include "config.h"
#include "ESPDateTime.h"

// don't forget to change this to real ssid/password
// or set these in config.h and include it
#ifndef WIFI_SSID
#define WIFI_SSID "Change to your WiFi SSID"
#define WIFI_PASS "Change to your WiFi Password"
#endif

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

void setupDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone(-4);
  //   DateTime.setServer("asia.pool.ntp.org");
  //   DateTime.begin(15 * 1000);
  DateTime.setTimeZone(8);
  DateTime.begin();
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
}

/**
 * More examples and docs see :
 * https://github.com/mcxiaoke/ESPDateTime
 *
 */

void setup() {
  delay(1000);
  Serial.begin(115200);
  setupWiFi();
  setupDateTime();
  Serial.println(DateTime.now());
  Serial.println(DateTime.getTime());
  Serial.println(DateTime.utcTime());
  Serial.println("--------------------");
  Serial.println(DateTime.toString());
  Serial.println(DateTime.toISOString());
  Serial.println(DateTime.toUTCString());
  Serial.println("--------------------");
  Serial.println(DateTime.format(DateFormatter::COMPAT));
  Serial.println(DateTime.format(DateFormatter::DATE_ONLY));
  Serial.println(DateTime.format(DateFormatter::TIME_ONLY));
  Serial.println("--------------------");
  DateTimeParts p = DateTime.getParts();
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d %ld %+05d\n", p.getYear(),
                p.getMonth(), p.getMonthDay(), p.getHours(), p.getMinutes(),
                p.getSeconds(), p.getTime(), p.getTimeZone());
  Serial.println("--------------------");
  time_t t = DateTime.now();
  Serial.println(DateFormatter::format("%Y/%m/%d %H:%M:%S", t));
  Serial.println(DateFormatter::format("%x - %I:%M %p", t));
  Serial.println(DateFormatter::format("Now it's %F %I:%M%p.", t));
}

void loop() {
  if (millis() - ms > 5000) {
    ms = millis();
    Serial.println("--------------------");
    if (!DateTime.isTimeValid()) {
      Serial.println("Failed to get time from server, retry.");
      DateTime.begin();
    } else {
      Serial.printf("Up     Time:   %lu seconds\n", millis() / 1000);
      Serial.printf("Local  Time:   %ld\n", DateTime.now());
      Serial.printf("Local  Time:   %s\n", DateTime.toString().c_str());
      Serial.printf("UTC    Time:   %ld\n", DateTime.utcTime());
      Serial.printf("UTC    Time:   %s\n",
                    DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
    }
  }
}