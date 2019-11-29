#include "DateTime.h"

// static time_t getCurrentTime() {
// need #include <chrono>
//   using std::chrono::system_clock;
//   auto now = system_clock::now();
//   std::time_t now_time_t = system_clock::to_time_t(now);
// }

static time_t validateTime(const time_t timeSecs) {
  auto bootSecs = timeSecs - (time_t)(millis() / 1000);
  return bootSecs > SECS_START_POINT ? bootSecs : DateTimeClass::TIME_ZERO;
}

String DateTimeParts::format(const char* fmt) const {
  char buf[64];
  strftime(buf, sizeof(buf), fmt, _tm);
  return String(buf);
}

String DateTimeParts::toString() const {
  return format(DateFormatter::ISO8601);
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
      ntpServer(_ntpServer),
      ntpMode(bootTimeSecs == TIME_ZERO) {}

bool DateTimeClass::setTimeZone(int _timeZone) {
  if (_timeZone == timeZone) {
    return false;
  }
  _DTLOGF("setTimeZone to %d\n", _timeZone);
  if (timeZone >= -11 || timeZone <= 13) {
    timeZone = _timeZone;
    return true;
  }
  return false;
}
void DateTimeClass::setServer(const char* _server) {
  if (strcmp(_server, ntpServer) == 0) {
    return;
  }
  _DTLOGF("setServer to %s\n", _server);
  ntpServer = _server;
}

bool DateTimeClass::forceUpdate(const unsigned int timeOutMs) {
  _DTLOGF("forceUpdate,timeZone:%d, server:%s, timeOut:%u\n", timeZone,
          ntpServer, timeOutMs);
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
  _DTLOGF("forceUpdate,now:%ld\n", now);
  ntpMode = true;
  setTime(time(nullptr));
  return isTimeValid();
}

bool DateTimeClass::setTime(const time_t timeSecs, bool forceSet) {
  if (forceSet || timeSecs > SECS_START_POINT) {
    bootTimeSecs = timeSecs - (time_t)(millis() / 1000);
  }
  _DTLOGF("setTime,timeSecs:%ld, bootTimeSecs:%ld\n", timeSecs, bootTimeSecs);
  return isTimeValid();
}

String DateTimeClass::format(const char* fmt) {
  return getParts().format(fmt);
}

String DateTimeClass::formatUTC(const char* fmt) {
  return DateTimeParts::from(utcTime(), 0).format(fmt);
}

DateTimeClass DateTime;