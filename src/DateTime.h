#ifndef ESP_DATE_TIME_CLASS_H
#define ESP_DATE_TIME_CLASS_H

/**
 * @file DateTime.h
 * @author Zhang Xiaoke (github@mcxiaoke.com)
 * @brief ESPDateTime header
 *
 */

#if !defined(ESP8266) && !defined(ESP32)
#error "ESPDateTime only support ESP32 or ESP8266 platform!"
#endif

/**
 * @brief Default TimeZone Offset GMT+0
 *
 */
#define DEFAULT_TIMEZONE "UTC0"

#include <Arduino.h>
#include <TZ.h>
#include <sys/time.h>
#include <time.h>

class DateTimeClass;

/**
 * @brief DateTime Parts struct, similar to struct tm in <time.h>, containing a
 * calendar date and time broken down into its components, but more readable,
 * include some useful getter methods.
 *
 */
struct DateTimeParts {
  // http://www.cplusplus.com/reference/ctime/tm/
  const time_t _ts; /**< timestamp variable, internal */
  const char* _tz;  /**< timezone variable, internal */
  /**
   * @brief Get internal timestamp, in seconds
   *
   * @return time_t timestamp, in seconds
   */
  time_t getTime() const { return _ts; }
  /**
   * @brief Get internal timezone offset
   *
   * @return int timezone offset
   */
  const char* getTimeZone() const { return _tz; }
  /**
   * @brief Get the year (format: 19xx, 20xx)
   *
   * @return int year value
   */
  int getYear() const {
    struct tm* t = localtime(&_ts);
    return t->tm_year + 1900;
  }
  /**
   * @brief Get months since January (0-11)
   *
   * @return int month value
   */
  int getMonth() const {
    struct tm* t = localtime(&_ts);
    return t->tm_mon;
  }
  /**
   * @brief Get days since January 1 (0-365)
   *
   * @return int day of year
   */
  int getYearDay() const {
    struct tm* t = localtime(&_ts);
    return t->tm_yday;
  }
  /**
   * @brief Get day of the month (1-31)
   *
   * @return int month day
   */
  int getMonthDay() const {
    struct tm* t = localtime(&_ts);
    return t->tm_mday;
  }
  /**
   * @brief Get days since Sunday (0-6)
   *
   * @return int day of week
   */
  int getWeekDay() const {
    struct tm* t = localtime(&_ts);
    return t->tm_wday;
  }
  /**
   * @brief Get hours since midnight (0-23)
   *
   * @return int hours
   */
  int getHours() const {
    struct tm* t = localtime(&_ts);
    return t->tm_hour;
  }
  /**
   * @brief Get minutes after the hour (0-59)
   *
   * @return int minutes
   */
  int getMinutes() const {
    struct tm* t = localtime(&_ts);
    return t->tm_min;
  }
  /**
   * @brief Get seconds after the minute (0-60)
   *
   * @return int seconds
   */
  int getSeconds() const {
    struct tm* t = localtime(&_ts);
    return t->tm_sec;
  }

  /**
   * @brief Foramt current time to string representation
   *
   * @param fmt format string for strftime
   * @return String string representation of current time
   */
  String format(const char* fmt) const;

  /**
   * @brief Foramt utc time to string representation
   *
   * @param fmt format string for strftime
   * @return String string representation of current time
   */
  String formatUTC(const char* fmt) const;
  /**
   * @brief Get string representation of current time
   *
   * @return String string representation of current time
   */
  String toString() const;

  /**
   * @brief factory method for constructing DateTimeParts from timestamp and
   * timezone.
   *
   * @param timeSecs timestamp in seconds since 1970
   * @param timeZone  timezone offset (-11,+13)
   * @return DateTimeParts DateTimeParts object
   */
  static DateTimeParts from(const time_t timeSecs,
                            const char* timeZone = DEFAULT_TIMEZONE);
  /**
   * @brief factory method for constructing DateTimeParts from DateTimeClass
   * object.
   *
   * @param dateTime DateTimeClass object
   * @return DateTimeParts DateTimeParts object
   */
  static DateTimeParts from(DateTimeClass* dateTime);
};

/**
 * @brief DateTime Formatter constants and static format methods.
 *
 * Details in http://www.cplusplus.com/reference/ctime/strftime/
 *
 */
struct DateFormatter {
  /**
   * @brief ISO8601 date time string format (2019-11-29T23:29:55+0800).
   *
   */
  constexpr static const char* ISO8601 = "%FT%T%z";
  /**
   * @brief RFC1123 date time string format (Fri, 29 Nov 2019 15:29:55 GMT)
   *
   */
  constexpr static const char* HTTP = "%a, %d %b %Y %H:%M:%S GMT";
  /**
   * @brief Simple date time string format (2019-11-29 23:29:55).
   *
   */
  constexpr static const char* SIMPLE = "%F %T";
  /**
   * @brief Compat date time string format (20191129_232955).
   *
   */
  constexpr static const char* COMPAT = "%Y%m%d_%H%M%S";
  /**
   * @brief Date Only date time string format (2019-11-29).
   *
   */
  constexpr static const char* DATE_ONLY = "%F";
  /**
   * @brief Time Only date time string format (23:29:55).
   *
   */
  constexpr static const char* TIME_ONLY = "%T";
  /**
   * @brief utility method for formatting time using fmt.
   *
   * @param fmt date time format string
   * @param timeSecs timestamp value
   * @param timeZone  timezone offset (-11,13)
   * @return String string representation of timeSecs
   */
  inline static String format(const char* fmt, const time_t timeSecs,
                              const char* timeZone = DEFAULT_TIMEZONE) {
    return DateTimeParts::from(timeSecs, timeZone).format(fmt);
  }
};

/**
 * @brief DateTime Library Main Class, include time get/set/format methods.
 *
 */
class DateTimeClass {
 public:
  /**
   * @brief Valid min timestamp value 1609459200 (2021/01/01 00:00:00).
   *
   * return value from time(null) < 1609459200 means system time invalid
   *
   */
  constexpr static time_t SECS_START_POINT = 1609459200;  // 20191128
  /**
   * @brief Unix Time Zero constant (1970-01-01 00:00:00)
   *
   */
  constexpr static time_t TIME_ZERO = 0;
  /**
   * @brief NTP Request default timeout: 10 seconds
   *
   */
  constexpr static unsigned int DEFAULT_TIMEOUT = 10 * 1000;  // milliseconds
  /**
   * @brief NTP Server 1
   *
   */
  constexpr static const char* NTP_SERVER_1 = "pool.ntp.org";
  /**
   * @brief NTP Server 2
   *
   */
  constexpr static const char* NTP_SERVER_2 = "time.apple.com";
  /**
   * @brief NTP Server 3
   *
   */
  constexpr static const char* NTP_SERVER_3 = "time.windows.com";
  /**
   * @brief Construct a new DateTimeClass object.
   *
   * @param _timeSecs set initialize timestamp
   * @param _timeZone set initialize timezone offset
   * @param _ntpServer set initialize ntp server
   */
  DateTimeClass(const time_t _timeSecs = TIME_ZERO,
                const char* _timeZone = DEFAULT_TIMEZONE,
                const char* _ntpServer = NTP_SERVER_1);
  /**
   * @brief Set the TimeZone offset
   *
   * @param _timeZone time zone offset value
   * @return true if time zone valid and changed
   * @return false if time zone not valid or not changed
   */
  bool setTimeZone(const char* _timeZone);
  /**
   * @brief Set the NTP Server
   *
   * @param _server ntp server domain name or ip address
   */
  void setServer(const char* _server);
  /**
   * @brief Force NTP Sync to update system timestamp for internal use, please *
   * using begin() instead.
   *
   * @param timeOutMs ntp request timeout
   * @return true if timestamp updated and valid
   * @return false if timestamp not valid
   */
  bool forceUpdate(const unsigned int timeOutMs = DEFAULT_TIMEOUT);
  /**
   * @brief Set the timestamp from outside, for test only
   *
   * @param timeSecs timestamp in seconds
   * @param forceSet ignore valid check, force set
   * @return true if timestamp valid
   * @return false if timestamp not valid
   */
  bool setTime(const time_t timeSecs, bool forceSet = false);
  /**
   * @brief Format current local time to string
   *
   * Attention: ESP8266 does not support real timezone, it just add timeZone
   * 3600 seconds to original timestamp, so %z format is always +0000
   *
   * @param fmt date time format
   * @return String string representation of local time
   */
  String format(const char* fmt);
  /**
   * @brief Format current utc time to string
   *
   * @param fmt date time format
   * @return String String string representation of utc time
   */
  String formatUTC(const char* fmt);
  // inline functions
  //   void formatTo(const char* fmt, char* dst);
  /**
   * @brief Begin ntp sync to update system time
   *
   * @param timeOutMs ntp request timeout
   * @return true if timestamp updated and valid
   * @return false if timestamp not valid
   */
  inline bool begin(const unsigned int timeOutMs = DEFAULT_TIMEOUT) {
    return isTimeValid() || forceUpdate(timeOutMs);
  }
  /**
   * @brief Check current timestamp is or not valid time
   *
   * @return true if time valid
   * @return false if time not valid
   */
  inline bool isTimeValid() const { return bootTimeSecs > SECS_START_POINT; }
  /**
   * @brief Get system boot timestamp in seconds
   *
   * @return time_t boot timestamp
   */
  inline time_t getBootTime() const {
    return bootTimeSecs > SECS_START_POINT ? bootTimeSecs : TIME_ZERO;
  }
  /**
   * @brief Get current local timestamp, alias of getTime()
   *
   * @return time_t timestamp
   */
  inline time_t now() const { return getTime(); }
  /**
   * @brief Get current local timestamp
   *
   * @return time_t timestamp
   */
  inline time_t getTime() const {
    auto t = time(nullptr);
    return t > SECS_START_POINT ? t : TIME_ZERO;
  }
  /**
   * @brief Get current timezone offset
   *
   * @return int time zone offset
   */
  inline const char* getTimeZone() const { return timeZone; }
  /**
   * @brief Get current ntp server address
   *
   * @return const char* ntp server
   */
  inline const char* getServer() { return ntpServer; }
  /**
   * @brief Get DateTimeParts object
   *
   * @return DateTimeParts DateTimeParts object
   */
  inline DateTimeParts getParts() { return DateTimeParts::from(this); }
  /**
   * @brief String simple string representation of local time
   *
   * @return String string representation
   */
  inline String toString() { return format(DateFormatter::SIMPLE); }
  /**
   * @brief String ISO8601 representation of local time
   *
   * @return String string representation
   */
  inline String toISOString() { return format(DateFormatter::ISO8601); }
  /**
   * @brief String RFC1123 representation of local time
   *
   * @return String string representation
   */
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
    return lhs.bootTimeSecs < rhs.bootTimeSecs;
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
  /**
   * @brief Boot timestamp seconds.
   *
   */
  unsigned long bootTimeSecs;
  /**
   * @brief Time zone offset
   *
   */
  const char* timeZone;
  /**
   * @brief First ntp server address.
   *
   */
  const char* ntpServer;
  bool ntpMode;
};

/**
 * @brief Global DateTimeClass object.
 *
 */
extern DateTimeClass DateTime;  // define in cpp

#endif
