## NB_Generic

[![arduino-library-badge](https://www.ardu-badge.com/badge/NB_Generic.svg?)](https://www.ardu-badge.com/NB_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/NB_Generic.svg)](https://github.com/khoih-prog/NB_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/NB_Generic/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/NB_Generic.svg)](http://github.com/khoih-prog/NB_Generic/issues)

---
---

## Table of Contents


* [Changelog](#changelog)
  * [Release v1.3.1](#release-v131)
  * [Release v1.3.0](#release-v130)
  * [Release v1.2.0](#release-v120)
  * [Major Release v1.1.0](#major-release-v110)
  * [Release v1.0.1](#release-v101)
  * [Release v1.0.0](#release-v100)


---
---

## Changelog

### Release v1.3.1

1. Update `Packages' Patches`

### Release v1.3.0

1. Add support to Adafruit nRF52 core v0.22.0+
2. Add support to Raytac MDBT50Q_RX Dongle

### Release v1.2.0

1. Add support to RP2040-based boards, such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Earle Philhower's arduino-pico core**](https://github.com/earlephilhower/arduino-pico).
2. Add support to RP2040-based boards, such as **NANO_RP2040_CONNECT, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed).

### Major Release v1.1.0

1. Rewrite to isolate AT-command hardware-specific code to prepare for supporting more NB-IoT/LTE-M/GPRS modems
2. Add FileUtils-related examples
3. Make `NB_RESETN`, `NB_PWR` optional for u-blox NB-IoT/LTE-M/GPRS modems
4. Permit configure baudrate of NB-IoT/LTE-M/GPRS Modem
5. Reduce hardcoding
6. Add `Bug Report` and `Feature Request` templates

### Release v1.0.1

1. Add Advanced examples (MQTT, Blynk)

### Release v1.0.0

1. Initial coding to add support to many boards besides MKRNB 1500 / u-blox NB-IoT/LTE-M/GPRS modems (Sara R4xx NB-IoT/LTE-M modules (SARA-R404M, SARA-R410M, SARA-R412M, SARA-N410, etc)



