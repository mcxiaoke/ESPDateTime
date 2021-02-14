#line 2 "test_date_time.cpp"
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <unity.h>
#include <DateTime.h>
#include "config.h"
#include "test_config.h"

// https://github.com/bxparks/AUnit
using namespace aunit;

test(T001ZeroConstructorA) {
  af(DateTime.isTimeValid());
  ae(DateTimeClass::TIME_ZERO,
     DateTime.getBootTime() + (time_t)(millis() / 1000));
  ae(DateTimeClass::DEFAULT_TIMEZONE, DateTime.getTimeZone());
  ae(DateTimeClass::TIME_ZERO, DateTime.now());
  ae(DateTimeClass::TIME_ZERO, DateTime.getTime());
  ae(DateTimeClass::TIME_ZERO, DateTime.utcTime());
  ae(DateTimeClass::NTP_SERVER_1, DateTime.getServer());
}

test(T002ZeroConstructorB) {
  af(DateTime.isTimeValid());
  ae("1970-01-01 00:00:00", DateTime.toString());
  ae("1970-01-01T00:00:00+0000", DateTime.toISOString());
  ae("Thu, 01 Jan 1970 00:00:00 GMT", DateTime.toUTCString());
}

test(T003NewConstructorA) {
  DateTimeClass d;
  af(d.isTimeValid());
  ae(DateTimeClass::TIME_ZERO, d.getBootTime() + (time_t)(millis() / 1000));
  ae(DateTimeClass::DEFAULT_TIMEZONE, d.getTimeZone());
  ae(DateTimeClass::TIME_ZERO, d.now());
  ae(DateTimeClass::TIME_ZERO, d.getTime());
  ae(DateTimeClass::TIME_ZERO, d.utcTime());
  ae(DateTimeClass::NTP_SERVER_1, d.getServer());
}

test(T004NewConstructorB) {
  DateTimeClass d;
  af(d.isTimeValid());
  ae("1970-01-01 00:00:00", d.toString());
  ae("1970-01-01T00:00:00+0000", d.toISOString());
  ae("Thu, 01 Jan 1970 00:00:00 GMT", d.toUTCString());
}

test(T005CustomConstructorA) {
  // 1574956800 = 20191129000000
  time_t t = 1574956800;
  int z = 8;
  const char* s = "time.apple.com";
  DateTimeClass d(t, z, s);
  at(d.isTimeValid());
  ae(t, d.getBootTime() + (time_t)(millis() / 1000));
  ae(z, d.getTimeZone());
  ae(t, d.now());
  ae(t, d.getTime());
  ae(t - 8 * 3600, d.utcTime());
  ae(s, d.getServer());
}

test(T006CustomConstructorB) {
  // 1574956800 = 20191129000000
  time_t t = 1574956800;
  int z = 8;
  const char* s = "time.apple.com";
  DateTimeClass d(t, z, s);
  at(d.isTimeValid());
  ae("2019-11-28 16:00:00", d.toString());
  ae("2019-11-28T16:00:00+0000", d.toISOString());
  ae("Thu, 28 Nov 2019 08:00:00 GMT", d.toUTCString());
}

test(T007NTPDateTime) {
  // 1574956800 = 20191129000000
  DateTime.setTimeZone(8);
  DateTime.setServer("time.apple.com");
  af(DateTime.isTimeValid());
  ae(8, DateTime.getTimeZone());
  ae("time.apple.com", DateTime.getServer());
  DateTime.begin();
  Serial.printf("%s (%s, %d)\n", DateTime.toISOString().c_str(), __FUNCTION__,
                __LINE__);
  at(DateTime.isTimeValid());
  // 1574956800 < timestamp
  ale(1574956800L, DateTime.now());
  ale(1574956800L, DateTime.getTime());
  ale(1574956800L - 8 * 3600, DateTime.utcTime());
}

test(T008NTPDateTimeClass) {
  // 1574956800 = 20191129000000
  DateTimeClass d(0, 8);
  af(d.isTimeValid());
  ae(8, d.getTimeZone());
  ae(DateTimeClass::NTP_SERVER_1, d.getServer());
  d.begin();
  Serial.printf("%s (%s, %d)\n", d.toISOString().c_str(), __FUNCTION__,
                __LINE__);
  at(d.isTimeValid());
  // 1574956800 < timestamp
  ale(1574956800L, d.now());
  ale(1574956800L, d.getTime());
  ale(1574956800L - 8 * 3600, d.utcTime());
}

test(T009DateTimeParts) {
  // 1613277037 = 202102141230
  int z = 8;
  DateTimeClass d;
  d.setTimeZone(8);
  d.begin();
  at(d.isTimeValid());
  Serial.printf("%s (%s, %d)\n", d.toISOString().c_str(), __FUNCTION__,
                __LINE__);
  auto p = d.getParts();
  ae(z, p._tz);
  // real time test
  // replace with cur date
  ae(2021, p.getYear());
  ae(2, p.getMonth());
  ae(14, p.getMonthDay());
  ae(6, p.getWeekDay());
  //   ae(0, p.getHours());
  //   ae(0, p.getMinutes());
  //   ae(0, p.getSeconds());
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  Serial.println("WiFi Connected");
}

void setup() {
  delay(1000);
  Serial.begin(115200);
  setupWiFi();
  TestRunner::setTimeout(120);
  TestRunner::setVerbosity(Verbosity::kTestFailed | Verbosity::kTestRunSummary);
}

void loop() {
  // Should get:
  // TestRunner summary:
  //    3 passed, 1 failed, 0 skipped, 0 timed out, out of 8 test(s).
  TestRunner::run();
}