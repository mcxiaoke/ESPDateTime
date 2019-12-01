#ifndef ESP_DATE_TIME_TIME_ELAPSED_H
#define ESP_DATE_TIME_TIME_ELAPSED_H

/**
 * @file TimeElapsed.h
 * @author Zhang Xiaoke (github@mcxiaoke.com)
 * @brief ESPDateTime header
 *
 */

#include <Arduino.h>

/**
 * @brief TimeElapsed class
 *
 */

class TimeElapsed {
 private:
  unsigned long ms;

 public:
  TimeElapsed() : ms(millis()) {}
  TimeElapsed(unsigned long val) { ms = millis() - val; }
  TimeElapsed(const TimeElapsed& rhs) { ms = rhs.ms; }
  operator unsigned long() const { return millis() - ms; }
  TimeElapsed& operator=(const TimeElapsed& rhs) {
    ms = rhs.ms;
    return *this;
  }
  TimeElapsed& operator=(unsigned long val) {
    ms = millis() - val;
    return *this;
  }
  TimeElapsed& operator-=(unsigned long val) {
    ms += val;
    return *this;
  }
  TimeElapsed& operator+=(unsigned long val) {
    ms -= val;
    return *this;
  }
  TimeElapsed operator-(int val) const {
    TimeElapsed r(*this);
    r.ms += val;
    return r;
  }
  TimeElapsed operator-(unsigned int val) const {
    TimeElapsed r(*this);
    r.ms += val;
    return r;
  }
  TimeElapsed operator-(long val) const {
    TimeElapsed r(*this);
    r.ms += val;
    return r;
  }
  TimeElapsed operator-(unsigned long val) const {
    TimeElapsed r(*this);
    r.ms += val;
    return r;
  }
  TimeElapsed operator+(int val) const {
    TimeElapsed r(*this);
    r.ms -= val;
    return r;
  }
  TimeElapsed operator+(unsigned int val) const {
    TimeElapsed r(*this);
    r.ms -= val;
    return r;
  }
  TimeElapsed operator+(long val) const {
    TimeElapsed r(*this);
    r.ms -= val;
    return r;
  }
  TimeElapsed operator+(unsigned long val) const {
    TimeElapsed r(*this);
    r.ms -= val;
    return r;
  }
};

#endif