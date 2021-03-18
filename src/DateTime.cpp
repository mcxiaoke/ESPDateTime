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
      ntpServer(_ntpServer),
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
void DateTimeClass::setServer(const char* _server) {
  if (strcmp(_server, ntpServer) == 0) {
    return;
  }
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("setServer to %s\n", _server);
#endif
  ntpServer = _server;
}

bool DateTimeClass::forceUpdate(const unsigned int timeOutMs) {
#ifdef ESP_DATE_TIME_DEBUG
  Serial.printf("forceUpdate,timeZone:%s, server:%s, timeOut:%u\n", timeZone,
                ntpServer, timeOutMs);
#endif
  // esp8266 not support time_zone, just add seconds
  // so strftime %z always +0000
  configTime(timeZone, ntpServer, NTP_SERVER_2, NTP_SERVER_3);
  time_t now = time(nullptr);
  auto startMs = millis();
  unsigned long retryCount = 0;
  while (now < SECS_START_POINT && (startMs + timeOutMs) > millis()) {
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