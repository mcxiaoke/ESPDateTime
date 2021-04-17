#include "DateTime.h"

// static time_t getCurrentTime() {
// need #include <chrono>
//   using std::chrono::system_clock;
//   auto now = system_clock::now();
//   std::time_t now_time_t = system_clock::to_time_t(now);
// }

static time_t validateTime(const time_t timeSecs) {
  auto bootSecs = timeSecs - (time_t)(millis() / 1000);
  return bootSecs > DateTimeClass::SECS_START_POINT ? bootSecs
                                                    : DateTimeClass::TIME_ZERO;
}

String DateTimeParts::format(const char* fmt) const {
  char buf[64];
  struct tm* t = localtime(&_ts);
  strftime(buf, sizeof(buf), fmt, t);
  return String(buf);
}

String DateTimeParts::formatUTC(const char* fmt) const {
  char buf[64];
  struct tm* t = gmtime(&_ts);
  strftime(buf, sizeof(buf), fmt, t);
  return String(buf);
}

String DateTimeParts::toString() const {
  return format(DateFormatter::ISO8601);
}

DateTimeParts DateTimeParts::from(const time_t timeSecs, const char* timeZone) {
  return {timeSecs, timeZone};
}

DateTimeParts DateTimeParts::from(DateTimeClass* dateTime) {
  return from(dateTime->getTime(), dateTime->getTimeZone());
}

DateTimeClass::DateTimeClass(const time_t _timeSecs, const char* _timeZone,
                             const char* _ntpServer)
    : bootTimeSecs(validateTime(_timeSecs)),
      timeZone(_timeZone),
      ntpServer1(_ntpServer),
      ntpMode(bootTimeSecs == TIME_ZERO) {}

bool DateTimeClass::setTimeZone(const char* _timeZone) {
  if (strcmp(timeZone, _timeZone) == 0) {
    return false;
  }
  timeZone = _timeZone;
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("setTimeZone to %s\n", _timeZone);
#endif
  return true;
}
void DateTimeClass::setServer(const char* _server1, const char* _server2,
                              const char* _server3) {
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("setServer to %s,%s,%s\n", _server1, _server2, _server3);
#endif
  ntpServer1 = _server1;
  ntpServer2 = _server2;
  ntpServer3 = _server3;
}

bool DateTimeClass::forceUpdate(const unsigned int timeOutMs) {
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("forceUpdate,timeZone:%s, server:%s, timeOut:%u\n", timeZone,
                ntpServer1, timeOutMs);
#endif
// esp8266 not support time_zone, just add seconds
// so strftime %z always +0000
#if defined(ESP8266)
  configTime(timeZone, ntpServer1, ntpServer2, ntpServer3);
#elif defined(ESP32)
  configTzTime(timeZone, ntpServer1, ntpServer2, ntpServer3);
#endif
  time_t now = time(nullptr);
  auto startMs = millis();
  unsigned long retryCount = 0;
  while (now < SECS_START_POINT && (millis() - startMs < timeOutMs)) {
    delay(50 + 50 * retryCount++);
    now = time(nullptr);
  }
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("forceUpdate,now:%ld\n", now);
#endif
  ntpMode = true;
  setTime(time(nullptr));
  return isTimeValid();
}

time_t DateTimeClass::ntpTime(const unsigned int timeOutMs) {
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("ntpTime,timeZone:%s, server:%s, timeOut:%u\n", timeZone,
                ntpServer1, timeOutMs);
#endif
  time_t now = time(nullptr);
  auto startMs = millis();
  unsigned long retryCount = 0;
  while (millis() - startMs < timeOutMs) {
    delay(50 + 50 * retryCount++);
    now = time(nullptr);
  }
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("ntpTime,now:%ld\n", now);
#endif
  return now;
}

bool DateTimeClass::setTime(const time_t timeSecs, bool forceSet) {
  if (forceSet || timeSecs > SECS_START_POINT) {
    bootTimeSecs = timeSecs - (time_t)(millis() / 1000);
  }
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("setTime,timeSecs:%ld, bootTimeSecs:%ld\n", timeSecs,
                bootTimeSecs);
#endif
  return isTimeValid();
}

String DateTimeClass::format(const char* fmt) { return getParts().format(fmt); }

String DateTimeClass::formatUTC(const char* fmt) {
  return getParts().formatUTC(fmt);
}

DateTimeClass DateTime;