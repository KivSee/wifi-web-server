Library Description
===================
# wifi-web-server

This library enables playing controlling LED animations by the FastLED library over WiFi on an ESP8266 device.

Over The Air (OTA) reprogramming of the device is enabled once an initial serial configuration was done with WiFi credentials.

Testing was done using WS2812 led strips on a NodeMCU 3.0 board.

NOTE: The library is under development and major changes are expected.

## Dependencies
Arduino core for ESP8266 (if using Arduino IDE) https://github.com/esp8266/Arduino

FastLED library coryking fork, https://github.com/coryking/FastLED

ArduinoJson

RemoteDebug

## Configuration file

Library usage requires creating a `KivseeConf.h` file under the `src` directory, a `KivseeConfTemplate.h` file is included to be used as a template,
simply change the required parameters in the template file and save it as `KivseeConf.h`.


## Atom IDE

Development of this library is recommended using Atom IDE ( https://atom.io/ ) with PlatformIO plugin ( https://platformio.org/ )
The IDE project files are included with the library as a reference for ease of use.

The Atom IDE Library Dependency Finder (LDF) automates the process of fetching dependency libraries and installing them.

The PlatformIO plugin adds support for ESP8266 dev boards as well as software libraries for the esp8266 platform