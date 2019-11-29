#include "DateTime.h"

// static time_t getCurrentTime() {
// need #include <chrono>
//   using std::chrono::system_clock;
//   auto now = system_clock::now();
//   std::time_t now_time_t = system_clock::to_time_t(now);
// }

static time_t validateTime(const time_t timeSecs) {
  return timeSecs > SECS_START_POINT ? (timeSecs - millis() / 1000) : 0;
}

String DateTimeParts::format(const char* fmt) const {
  char buf[64];
  strftime(buf, sizeof(buf), fmt, _tm);
  return String(buf);
}

String DateTimeParts::toString() const {
  return format(DateTimeFormat::ISO8601);
}

DateTimeParts DateTimeParts::from(const time_t timeSecs, const int timeZone) {
  struct tm* time_info = localtime(&timeSecs);
  return {timeSecs, timeZone, time_info};
}

DateTimeParts DateTimeParts::from(DateTimeClass* dateTime) {
  return from(dateTime->getTime(), dateTime->getTimeZone());
}

DateTimeClass::DateTimeClass(const time_t _timeSecs,
                             const int _timeZone,
                             const char* _ntpServer)
    : bootTimeSecs(validateTime(_timeSecs)),
      timeZone(_timeZone),
      ntpServer(_ntpServer) {}

bool DateTimeClass::setTimeZone(int _timeZone) {
  if (_timeZone == timeZone) {
    return false;
  }
  if (timeZone >= -11 || timeZone <= 13) {
    timeZone = timeZone;
    // config changed, update
    forceUpdate(DEFAULT_TIMEOUT);
    return true;
  }
  return false;
}
void DateTimeClass::setServer(const char* _server) {
  if (strcmp(_server, ntpServer) == 0) {
    return;
  }
  ntpServer = _server;
  // config changed, update
  forceUpdate(DEFAULT_TIMEOUT);
}

bool DateTimeClass::forceUpdate(const unsigned int timeOutMs) {
  Serial.printf("forceUpdate: %d - %s\n", timeZone, ntpServer);
  // esp8266 not support time_zone, just add seconds
  // so strftime %z always +0000
  configTime(timeZone * 3600, 0, ntpServer, NTP_SERVER_2, NTP_SERVER_3);
  time_t now = time(nullptr);
  auto startMs = millis();
  unsigned long retryCount = 0;
  while (now < SECS_START_POINT && (startMs + timeOutMs) > millis()) {
    delay(50 + 50 * retryCount++);
    now = time(nullptr);
  }
  setTime(time(nullptr));
  Serial.println(timeIsSet());
  return timeIsSet();
}

bool DateTimeClass::setTime(const time_t timeSecs, bool forceSet) {
  if (forceSet || timeSecs > SECS_START_POINT) {
    bootTimeSecs = timeSecs - millis() / 1000;
  }
  return timeIsSet();
}

String DateTimeClass::format(const char* fmt) {
  return getParts().format(fmt);
}

String DateTimeClass::formatUTC(const char* fmt) {
  return DateTimeParts::from(utcTime(), 0).format(fmt);
}

DateTimeClass DateTime;