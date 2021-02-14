# ESPDateTime

This library provides a simple class `DateTimeClass` for sync system timestamp vis ntp and a struct `DateFormatter` to format date time to string, works on **ESP8266** and **ESP32** platform.

Current Version: **v0.2.0** [![Build Status](https://travis-ci.org/mcxiaoke/ESPDateTime.svg?branch=master)](https://travis-ci.org/mcxiaoke/ESPDateTime)

# Install

## Using PlatformIO

This libarary is published to [PlatformIO](https://platformio.org/lib/show/6871/ESPDateTime), PlatformIO IDE has built-in PIO Home: Library Manager, you can search `ESPDateTime` in Library Manager and click to install this library.

Or you can install using platformio cli:

```bash
# Using library Id
 platformio lib install 6871
# or Using library Name
 platformio lib install "ESPDateTime"
```

Add dependency to your `platformio.ini` file:

```ini
lib_deps =
     # Using library Name
     mcxiaoke/ESPDateTime @ ^0.2.1
     # or You can use the latest git version
     https://github.com/mcxiaoke/ESPDateTime.git
```

## Using Arduino IDE

I will publish this library to Arduino Library soon. (TODO)

## Manual Install

Clone this repo or download source code at [release](https://github.com/mcxiaoke/ESPDateTime/releases/latest) page, then copy all files in src to your project source directory.

# Getting Started

## Include the Header

```cpp
// if you just want to use DateTime:
#include <DateTime.h>
// or if you want to use DateTimeClass and TimeElapsed:
#include <ESPDateTime.h>
```

## Config DateTime

`DateTime` is a global [`DateTimeClass`](https://github.com/mcxiaoke/ESPDateTime/blob/master/src/DateTime.h#L58) object for use in your code.

```cpp
void setupDateTime() {
  // setup this after wifi connected
  // you can use custom timeZone,server and timeout
  // DateTime.setTimeZone(-4);
  // DateTime.setServer("asia.pool.ntp.org");
  // DateTime.begin(15 * 1000);
  DateTime.setTimeZone(8);
  // this method config ntp and wait for time sync
  // default timeout is 10 seconds
  DateTime.begin(/* timeout param */);
  if (!DateTime.isTimeValid()) {
    Serial.println("Failed to get time from server.");
  }
}
```

## DateTime Functions

You can use [`DateTime`](https://github.com/mcxiaoke/ESPDateTime/blob/master/src/DateTime.h#L58) to get current time and format time to string, `format` function internal using `strftime` function in `<time.h>`.

```cpp
// alias for getTime()
time_t  DateTime.now()
// get current timestap in seconds
time_t  DateTime.getTime()
// get utc timestamp in seconds
time_t  DateTime.utcTime()
// get current timezone
int     DateTime.getTimeZone()
// get formatted string of time
String  DateTime.toString ()
// format time to string, using strftime
// http://www.cplusplus.com/reference/ctime/strftime/
String DateTime.format(const char* fmt);
```

## Classes

- [**DateTimeClass**](https://github.com/mcxiaoke/ESPDateTime/blob/master/src/DateTime.h#L58) - Main Class for get current timestamp and format time to string, class of global `DateTime` object.
- [**DateTimeParts**](https://github.com/mcxiaoke/ESPDateTime/blob/master/src/DateTime.h#L20) - Struct for get year/month/day/week part of time struct.
- [**DateFormatter**](https://github.com/mcxiaoke/ESPDateTime/blob/master/src/DateTime.h#L44) - Class for format timestamp to string, include some format constants.
- [**TimeElapsed**](https://github.com/mcxiaoke/ESPDateTime/blob/master/src/TimeElapsed.h) - Class for calculate elapsed time in milliseconds, original code is from [elapsedMillis](https://github.com/pfeerick/elapsedMillis).

## Examples

See [examples](https://github.com/mcxiaoke/ESPDateTime/tree/master/examples/) folder in this project, to run example on your device, WiFi ssid and password must be set in source code.

## Documents

See [API Reference](https://blog.mcxiaoke.com/ESPDateTime/).

# License

    Copyright 2019-2021 github@mcxiaoke.com

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
