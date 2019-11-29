#ifndef ESP_DATE_TIME_CLASS_H
#define ESP_DATE_TIME_CLASS_H

#if !defined(ESP8266) && !defined(ESP32)
#error "ESPDateTime only support ESP32 or ESP8266 platform!"
#endif

#include "debug.h"
#include <Arduino.h>
#include <time.h>
#include <sys/time.h>

class DateTimeClass;

constexpr static unsigned long DIFF1900TO1970 = 2208988800UL;
// 2019/01/01 00:00:00 in TimeZone+8
// time() < this means invalid
constexpr static time_t SECS_START_POINT = 1574870400;  // 20191128

struct DateTimeParts {
  // http://www.cplusplus.com/reference/ctime/tm/
  const time_t _ts;
  const int _tz;
  const struct tm* _tm;
  time_t getTime() const { return _ts; }
  int getTimeZone() const { return _tz; }
  int getYear() const { return _tm->tm_year + 1900; }
  int getMonth() const { return _tm->tm_mon; }
  int getYearDay() const { return _tm->tm_yday; }
  int getMonthDay() const { return _tm->tm_mday; }
  int getWeekDay() const { return _tm->tm_wday; }
  int getHours() const { return _tm->tm_hour; }
  int getMinutes() const { return _tm->tm_min; }
  int getSeconds() const { return _tm->tm_sec; }

  String format(const char* fmt) const;

  String toString() const;

  static DateTimeParts from(const time_t timeSecs, const int timeZone = 0);
  static DateTimeParts from(DateTimeClass* dateTime);
};

struct DateFormatter {
  constexpr static const char* ISO8601 = "%FT%T%z";                 // ISO 8601
  constexpr static const char* HTTP = "%a, %d %b %Y %H:%M:%S GMT";  // RFC 1123
  constexpr static const char* SIMPLE = "%F %T";
  constexpr static const char* COMPAT = "%Y%m%d_%H%M%S";
  constexpr static const char* DATE_ONLY = "%F";
  constexpr static const char* TIME_ONLY = "%T";
  String format(const char* fmt,
                const time_t timeSecs,
                const int timeZone = 0) const {
    return DateTimeParts::from(timeSecs, timeZone).format(fmt);
  }
};

class DateTimeClass {
 public:
  constexpr static time_t TIME_ZERO = 0;
  constexpr static int TIMEZONE_UTC = 0;
  constexpr static int TIMEZONE_CHINA = 8;
  constexpr static int DEFAULT_TIMEZONE = TIMEZONE_UTC;  // time zone offset
  constexpr static unsigned int DEFAULT_TIMEOUT = 10 * 1000;  // milliseconds
  constexpr static const char* NTP_SERVER_1 = "ntp.ntsc.ac.cn";
  constexpr static const char* NTP_SERVER_2 = "pool.ntp.org";
  constexpr static const char* NTP_SERVER_3 = "time.windows.com";
  DateTimeClass(const time_t _timeSecs = TIME_ZERO,
                const int _timeZone = DEFAULT_TIMEZONE,
                const char* _ntpServer = NTP_SERVER_1);
  bool setTimeZone(int _timeZone);
  void setServer(const char* _server);
  bool forceUpdate(const unsigned int timeOutMs = DEFAULT_TIMEOUT);
  bool setTime(const time_t timeSecs, bool forceSet = false);
  String format(const char* fmt);
  String formatUTC(const char* fmt);
  // inline functions
  //   void formatTo(const char* fmt, char* dst);
  inline bool begin(const unsigned int timeOutMs = DEFAULT_TIMEOUT) {
    return isTimeValid() || forceUpdate(timeOutMs);
  }
  inline bool isTimeValid() const { return bootTimeSecs > SECS_START_POINT; }
  inline time_t getBootTime() const {
    return bootTimeSecs > SECS_START_POINT
               ? bootTimeSecs
               : TIME_ZERO - (time_t)(millis() / 1000);
  }
  inline time_t now() const { return getTime(); }
  inline time_t getTime() const {
    return bootTimeSecs > SECS_START_POINT
               ? (bootTimeSecs + (time_t)(millis() / 1000))
               : TIME_ZERO;
  }
  inline time_t utcTime() const {
    return bootTimeSecs > SECS_START_POINT ? (getTime() - timeZone * 3600)
                                           : TIME_ZERO;
  }
  inline time_t osTime() const {
    auto t = time(nullptr);
    return t > SECS_START_POINT ? t : TIME_ZERO;
  }
  inline int getTimeZone() const { return timeZone; }
  inline const char* getServer() { return ntpServer; }
  inline DateTimeParts getParts() { return DateTimeParts::from(this); }
  inline String toString() { return format(DateFormatter::SIMPLE); }
  inline String toISOString() { return format(DateFormatter::ISO8601); }
  inline String toUTCString() { return formatUTC(DateFormatter::HTTP); }
  // operator overloads
  DateTimeClass operator+(const time_t timeDeltaSecs) {
    DateTimeClass dt(getTime() + timeDeltaSecs, timeZone, ntpServer);
    return dt;
  }
  DateTimeClass operator-(const time_t timeDeltaSecs) {
    DateTimeClass dt(getTime() - timeDeltaSecs, timeZone, ntpServer);
    return dt;
  }
  DateTimeClass& operator-=(const time_t timeDeltaSecs) {
    bootTimeSecs += timeDeltaSecs;
    return *this;
  }
  DateTimeClass& operator+=(const time_t timeDeltaSecs) {
    bootTimeSecs -= timeDeltaSecs;
    return *this;
  }
  friend bool operator<(const DateTimeClass& lhs, const DateTimeClass& rhs) {
    return lhs.bootTimeSecs + lhs.timeZone * 3600 <
           rhs.bootTimeSecs + rhs.timeZone * 3600;
  }
  friend bool operator>(const DateTimeClass& lhs, const DateTimeClass& rhs) {
    return rhs < lhs;
  }
  friend bool operator<=(const DateTimeClass& lhs, const DateTimeClass& rhs) {
    return !(lhs > rhs);
  }
  friend bool operator>=(const DateTimeClass& lhs, const DateTimeClass& rhs) {
    return !(lhs < rhs);
  }

  friend bool operator==(const DateTimeClass& lhs, const DateTimeClass& rhs) {
    return lhs.bootTimeSecs == rhs.bootTimeSecs && lhs.timeZone == rhs.timeZone;
  }
  friend bool operator!=(const DateTimeClass& lhs, const DateTimeClass& rhs) {
    return !(lhs == rhs);
  }

 private:
  unsigned long bootTimeSecs;
  int timeZone;
  const char* ntpServer;
  bool ntpMode;
};

extern DateTimeClass DateTime;  // define in cpp

#endif
