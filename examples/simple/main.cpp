
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include "ESPDateTime.h"
#include "config.h"

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
  DateTime.setServer("ntp.aliyun.com");
  DateTime.setTimeZone("CST-8");
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

void showTime() {
  Serial.printf("TimeZone:      %s\n", DateTime.getTimeZone());
  Serial.printf("Up     Time:   %lu seconds\n", millis() / 1000);
  Serial.printf("Now    Time:   %ld\n", DateTime.now());
  Serial.printf("Local  Time:   %s\n",
                DateTime.format(DateFormatter::SIMPLE).c_str());
  Serial.printf("ISO86  Time:   %s\n", DateTime.toISOString().c_str());
  Serial.printf("UTC    Time:   %s\n",
                DateTime.formatUTC(DateFormatter::SIMPLE).c_str());
  Serial.printf("UTC86  Time:   %s\n",
                DateTime.formatUTC(DateFormatter::ISO8601).c_str());

  Serial.println("===========");
  time_t t = time(NULL);
  Serial.printf("OS local:     %s", asctime(localtime(&t)));
  Serial.printf("OS UTC:       %s", asctime(gmtime(&t)));
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  setupWiFi();
  setupDateTime();
  Serial.println(DateTime.now());
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
  Serial.printf("%04d/%02d/%02d %02d:%02d:%02d %ld (%s)\n", p.getYear(),
                p.getMonth(), p.getMonthDay(), p.getHours(), p.getMinutes(),
                p.getSeconds(), p.getTime(), p.getTimeZone());
  Serial.println("--------------------");
  showTime();
}

void loop() {
  if (millis() - ms > 30000) {
    ms = millis();
    Serial.println("--------------------");
    if (!DateTime.isTimeValid()) {
      Serial.println("Failed to get time from server, retry.");
      DateTime.begin();
    } else {
      showTime();
    }
  }
}