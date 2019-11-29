#ifndef ESP_DATE_TIME_DEBUG_H
#define ESP_DATE_TIME_DEBUG_H

#ifdef ESP_DATE_TIME_DEBUG
#define _DTLOG(...) Serial.print(__VA_ARGS__)
#define _DTLOGN(...) Serial.println(__VA_ARGS__)
#define _DTLOGF(...) Serial.printf(__VA_ARGS__)
#else
#define _DTLOG(...)
#define _DTLOGN(...)
#define _DTLOGF(...)
#endif

#endif