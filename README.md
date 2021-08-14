## NB_Generic

[![arduino-library-badge](https://www.ardu-badge.com/badge/NB_Generic.svg?)](https://www.ardu-badge.com/NB_Generic)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/NB_Generic.svg)](https://github.com/khoih-prog/NB_Generic/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/NB_Generic/blob/main/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/NB_Generic.svg)](http://github.com/khoih-prog/NB_Generic/issues)

---
---

## Table of Contents


* [Why do we need this NB_Generic library](#why-do-we-need-this-NB_Generic-library)
  * [Features](#features)
  * [Supported modems](#supported-modems)
  * [To be Supported modems](#to-be-supported-modems)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](#changelog)
  * [Release v1.3.0](#release-v130)
  * [Release v1.2.0](#release-v120)
  * [Major Release v1.1.0](#major-release-v110)
  * [Release v1.0.1](#release-v101)
  * [Release v1.0.0](#release-v100)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Packages' Patches](#packages-patches)
  * [1. For Adafruit nRF52840 and nRF52832 boards](#1-for-adafruit-nRF52840-and-nRF52832-boards)
  * [2. For Teensy boards](#2-for-teensy-boards)
  * [3. For Arduino SAM DUE boards](#3-for-arduino-sam-due-boards)
  * [4. For Arduino SAMD boards](#4-for-arduino-samd-boards)
      * [For core version v1.8.10+](#for-core-version-v1810)
      * [For core version v1.8.9-](#for-core-version-v189-)
  * [5. For Adafruit SAMD boards](#5-for-adafruit-samd-boards)
  * [6. For Seeeduino SAMD boards](#6-for-seeeduino-samd-boards)
  * [7. For STM32 boards](#7-for-stm32-boards) 
    * [7.1. For STM32 boards to use LAN8720](#71-for-stm32-boards-to-use-lan8720)
    * [7.2. For STM32 boards to use Serial1](#72-for-stm32-boards-to-use-serial1)
  * [8. For RP2040-based boards using Earle Philhower arduino-pico core](#8-for-rp2040-based-boards-using-earle-philhower-arduino-pico-core)
    * [8.1. To use BOARD_NAME](#81-to-use-board_name)
    * [8.2. To avoid compile error relating to microsecondsToClockCycles](#82-to-avoid-compile-error-relating-to-microsecondstoclockcycles)
* [Libraries' Patches](#libraries-patches)
  * [1. For Adafruit_MQTT_Library](#1-for-adafruit_mqtt_library)
* [HOWTO Install esp32 core for ESP32-S2 (Saola, AI-Thinker ESP-12K) and ESP32-C3 boards into Arduino IDE](#howto-install-esp32-core-for-esp32-s2-saola-ai-thinker-esp-12k-and-esp32-c3-boards-into-arduino-ide)
  * [1. Save the original esp32 core](#1-save-the-original-esp32-core)
  * [2. Install esp32 core v1.0.6](#2-install-esp32-core-v106)
    * [2.1 Install esp32 core](#21-install-esp32-core)
    * [2.2 Download latest zip with esp32-s2 support](#22-download-latest-zip-with-esp32-s2-support)
    * [2.3 Unzip](#23-unzip)
    * [2.3 Update esp32 core directories](#24-update-esp32-core-directories)
  * [3. Download tools for ESP32-S2](#3-download-tools-for-esp32-s2) 
    * [3.1 Download Toolchain for Xtensa (ESP32-S2) based on GCC](#31-download-toolchain-for-xtensa-esp32-s2-based-on-gcc)
    * [3.2 Download esptool](#32-download-esptool)
    * [3.3 Unzip](#33-unzip)
  * [4. Update tools](#4-update-tools)
    * [4.1 Update Toolchain](#41-update-toolchain)
    * [4.2 Update esptool](#42-update-esptool)
  * [5. Download tools for ESP32-C3](#5-download-tools-for-esp32-c3)
  * [6. esp32-s2 WebServer Library Patch](#6-esp32-s2-webserver-library-patch)
* [Note for Platform IO using ESP32 LittleFS](#note-for-platform-io-using-esp32-littlefs)
* [Reference](#reference)
* [Documentation](#documentation)
  * [1. How it works](#1-how-it-works)
  * [2. Library structure](#2-library-structure)
  * [3. Library main classes](#3-library-main-classes)
    * [NB Class](#nb-class)
    * [NB_SMS Class](#nb_sms-class)
    * [GPRS Class](#gprs-class)
    * [NBClient Class](#nbclient-class)
    * [NBSSLClient Class](#nbsslclient-class)
    * [NBModem Class](#nbmodem-class)
    * [NBScanner Class](#nbscanner-class)
    * [NBPIN Class](#nbpin-class)
    * [NBUDP Class](#nbudp-class)
* [Configuration Notes](#configuration-notes)
  * [1. How to select which NB-IoT/LTE-M/GPRS module to use](#1-how-to-select-which-nb-iotlte-mgprs-module-to-use)
  * [2. How to select which Serial port to communicate to NB module](#2-how-to-select-which-serial-port-to-communicate-to-nb-module)
  * [3. How to select extra pins to control NB module](#3-how-to-select-extra-pins-to-control-nb-module)
* [Examples](#examples)
  * [ 1. GPRSUdpNtpClient](examples/GPRSUdpNtpClient)
  * [ 2. NBSSLWebClient](examples/NBSSLWebClient)
  * [ 3. NBWebClient](examples/NBWebClient)
  * [ 4. ReceiveSMS](examples/ReceiveSMS)
  * [ 5. SendSMS](examples/SendSMS)
* [Tools](#tools)
  * [ 6. ChooseRadioAccessTechnology](examples/Tools/ChooseRadioAccessTechnology)
  * [ 7. NBScanNetworks](examples/Tools/NBScanNetworks)
  * [ 8. PinManagement](examples/Tools/PinManagement)
  * [ 9. SerialNBPassthrough](examples/Tools/SerialNBPassthrough)
  * [10. TestGPRS](examples/Tools/TestGPRS)
  * [11. TestModem](examples/Tools/TestModem)
  * [Advanced Examples](#advanced-examples)
    * [MQTT](#mqtt)
      * [26. NB_MQTT](examples/Advanced/NB_MQTT)
      * [27. NB_MQTTClient_Auth](examples/Advanced/NB_MQTTClient_Auth)
      * [28. NB_MQTTClient_Basic](examples/Advanced/NB_MQTTClient_Basic)
      * [29. NB_MQTT_Medium](examples/Advanced/NB_MQTT_Medium)
      * [30. NB_MQTT_ThingStream](examples/Advanced/NB_MQTT_ThingStream)
    * [Blynk](#blynk)
      * [31. NB_Blynk](examples/Advanced/NB_Blynk)
* [Example NBWebClient](#example-nbwebclient)
  * [1. File NBWebClient.ino](#1-file-nbwebclientino)
  * [2. File defines.h](#2-file-definesh)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Releases](#releases)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---

## Why do we need this [NB_Generic library](https://github.com/khoih-prog/NB_Generic)

### Features

This library is based on, modified, bug-fixed and improved from:

1. [`MKRNB`](https://github.com/arduino-libraries/MKRNB)

to add support to many boards besides `Arduino MKR NB 1500`.

This [NB_Generic library](https://github.com/khoih-prog/NB_Generic) will finally provide these following features (certainly ony if supported by the NB-IoT/LTE-M/GPRS modules)

1. **TCP Client** (HTTP, **HTTPS**, MQTT, Blynk, WebSockets, ...)
2. **UDP** data connections
3. **USSD** (Sending USSD requests and decoding 7,8,16-bit responses)
4. **SMS** (Sending and Receiving)


### Supported modems

- [X] u-blox LTE-M/NB-IoT Modems (SARA-R4xx, SARA-N4xx, _but NOT SARA-N2xx_).

### To be Supported modems

- [ ] SIMCom LTE Modules (SIM7100E, SIM7500E, SIM7500A, SIM7600C, SIM7600E)
- [ ] SIMCom SIM7000E/A/G CAT-M1/NB-IoT Module
- [ ] SIMCom SIM7020E/A/G CAT-M1/NB-IoT Module
- [ ] Sequans Monarch LTE Cat M1/NB1 (VZM20Q)


### Currently Supported Boards

This [**NB_Generic** library](https://github.com/khoih-prog/NB_Generic) currently supports these following boards:

 1. **nRF52 boards**, such as **AdaFruit Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.**
 
 2. **SAM DUE**
 
 3. **SAMD21**
  - Arduino MKR NB 1500
  - Arduino SAMD21: ZERO, MKRs, NANO_33_IOT, etc.
  - Adafruit SAMD21 (M0): ItsyBitsy M0, Feather M0, Feather M0 Express, Metro M0 Express, Circuit Playground Express, Trinket M0, PIRkey, Hallowing M0, Crickit M0, etc.
  - Seeeduino:  LoRaWAN, Zero, Femto M0, XIAO M0, Wio GPS Board, etc.
  
 4. **SAMD51**
 
  - Adafruit SAMD51 (M4): Metro M4, Grand Central M4, ItsyBitsy M4, Feather M4 Express, Trellis M4, Metro M4 AirLift Lite, MONSTER M4SK Express, Hallowing M4, etc.
  - Seeeduino: Wio Terminal, Grove UI Wireless
  
 5. **Teensy (4.1, 4.0, 3.6, 3.5, 3,2, 3.1, 3.0, LC)**
 
 6. **ESP32 including ESP32-S2 (ESP32-S2 Saola, AI-Thinker ESP-12K, etc.)**
 
 7. **ESP8266**
  
 8. **STM32F/L/H/G/WB/MP1 boards (with 32+K Flash)**

  - Nucleo-144 (F429ZI, F767ZI, etc.)
  - Nucleo-64
  - Discovery
  - Generic STM32F0, STM32F1, STM32F2, STM32F3, STM32F4, STM32F7 (with 64+K Flash): x8 and up
  - STM32L0, STM32L1, STM32L4
  - STM32G0, STM32G4
  - STM32H7
  - STM32WB
  - STM32MP1
  - LoRa boards
  - 3-D printer boards
  - Generic Flight Controllers
  - Midatronics boards

  9. RP2040-based boards, such as **NANO_RP2040_CONNECT, RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, using [**Arduino-mbed RP2040** core](https://github.com/arduino/ArduinoCore-mbed) or [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico).
  

---
---

## Changelog

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

---
---

## Prerequisites

 1. [`Arduino IDE 1.8.15+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`Teensy core v1.51`](https://www.pjrc.com/teensy/td_download.html) for Teensy (4.0, 3.6, 3.5, 3,2, 3.1, 3.0) boards and [`Teensy core v1.53+`](https://www.pjrc.com/teensy/td_download.html) for Teensy 4.1 boards using NativeEthernet.
 3. [`Arduino SAM DUE core v1.6.12+`](https://www.arduino.cc/en/Guide/ArduinoDue) for SAM DUE ARM Cortex-M3 boards.
 4. [`Arduino SAMD core 1.8.11+`](https://www.arduino.cc/en/Guide/ArduinoM0) for SAMD ARM Cortex-M0+ boards. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-samd.svg)](https://github.com/arduino/ArduinoCore-samd/releases/latest)
 5. [`Adafruit SAMD core 1.7.2+`](https://www.adafruit.com/) for SAMD ARM Cortex-M0+ and M4 boards (Nano 33 IoT, etc.). [![GitHub release](https://img.shields.io/github/release/adafruit/ArduinoCore-samd.svg)](https://github.com/adafruit/ArduinoCore-samd/releases/latest)
 6. [`Seeeduino SAMD core 1.8.1+`](https://github.com/Seeed-Studio/ArduinoCore-samd) for SAMD21/SAMD51 boards (XIAO M0, Wio Terminal, etc.). [![Latest release](https://img.shields.io/github/release/Seeed-Studio/ArduinoCore-samd.svg)](https://github.com/Seeed-Studio/ArduinoCore-samd/releases/latest/)
 7. [`Adafruit nRF52 v0.24.0+`](https://www.adafruit.com) for nRF52 boards such as Adafruit NRF52840_FEATHER, NRF52832_FEATHER, NRF52840_FEATHER_SENSE, NRF52840_ITSYBITSY, NRF52840_CIRCUITPLAY, NRF52840_CLUE, NRF52840_METRO, NRF52840_PCA10056, PARTICLE_XENON, **NINA_B302_ublox**, etc. [![GitHub release](https://img.shields.io/github/release/adafruit/Adafruit_nRF52_Arduino.svg)](https://github.com/adafruit/Adafruit_nRF52_Arduino/releases/latest)
 8. [`ESP8266 Core 3.0.2+`](https://github.com/esp8266/Arduino) for ESP8266-based boards. [![Latest release](https://img.shields.io/github/release/esp8266/Arduino.svg)](https://github.com/esp8266/Arduino/releases/latest/). To use ESP8266 core 2.7.1+ for LittleFS. 
 9. [`ESP32 Core 1.0.6+`](https://github.com/espressif/arduino-esp32) for ESP32-based boards. [![Latest release](https://img.shields.io/github/release/espressif/arduino-esp32.svg)](https://github.com/espressif/arduino-esp32/releases/latest/)
10. [`ESP32-S2/C3 Core 1.0.6+`](https://github.com/espressif/arduino-esp32) for ESP32-S2/C3-based boards. Must follow [HOWTO Install esp32 core for ESP32-S2 (Saola, AI-Thinker ESP-12K) and ESP32-C3 boards into Arduino IDE](#howto-install-esp32-core-for-esp32-s2-saola-ai-thinker-esp-12k-and-esp32-c3-boards-into-arduino-ide).
11. [`Arduino Core for STM32 v2.0.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32 boards. [![GitHub release](https://img.shields.io/github/release/stm32duino/Arduino_Core_STM32.svg)](https://github.com/stm32duino/Arduino_Core_STM32/releases/latest)
12. [`Arduino mbed_rp2040 core 2.4.1+`](https://github.com/arduino/ArduinoCore-mbed) for Arduino (Use Arduino Board Manager) RP2040-based boards, such as RASPBERRY_PI_PICO. [![GitHub release](https://img.shields.io/github/release/arduino/ArduinoCore-mbed.svg)](https://github.com/arduino/ArduinoCore-mbed/releases/latest)
13. [`Earle Philhower's arduino-pico core v1.9.4+`](https://github.com/earlephilhower/arduino-pico) for RP2040-based boards such as **RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040 and GENERIC_RP2040**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)

---

## Installation

The suggested way to install is to:

### Use Arduino Library Manager
The best way is to use `Arduino Library Manager`. Search for `NB_Generic`, then select / install the latest version. You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/NB_Generic.svg?)](https://www.ardu-badge.com/NB_Generic) for more detailed instructions.

### Manual Install

1. Navigate to [NB_Generic](https://github.com/khoih-prog/NB_Generic) page.
2. Download the latest release `NB_Generic-main.zip`.
3. Extract the zip file to `NB_Generic-main` directory 
4. Copy the whole `NB_Generic-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO:

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**NB_Generic** library](https://platformio.org/lib/show/11912/NB_Generic) by using [Library Manager](https://platformio.org/lib/show/11912/NB_Generic/installation). Search for NB_Generic in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---

### Packages' Patches

#### 1. For Adafruit nRF52840 and nRF52832 boards

**To be able to compile, run and automatically detect and display BOARD_NAME on nRF52840/nRF52832 boards**, you have to copy the whole [nRF52 0.24.0](Packages_Patches/adafruit/hardware/nrf52/0.24.0) directory into Adafruit nRF52 directory (~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0). 

Supposing the Adafruit nRF52 version is 0.24.0. These files must be copied into the directory:
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/0.24.0/cores/nRF5/Udp.h`**

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z
These files must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/platform.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/boards.txt`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B302_ublox/variant.cpp`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.h`
- `~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/variants/NINA_B112_ublox/variant.cpp`
- **`~/.arduino15/packages/adafruit/hardware/nrf52/x.yy.z/cores/nRF5/Udp.h`**

#### 2. For Teensy boards
 
 **To be able to compile and run on Teensy boards**, you have to copy the files in [**Packages_Patches for Teensy directory**](Packages_Patches/hardware/teensy/avr) into Teensy hardware directory (./arduino-1.8.13/hardware/teensy/avr/boards.txt). 

Supposing the Arduino version is 1.8.13. These files must be copied into the directory:

- `./arduino-1.8.13/hardware/teensy/avr/boards.txt`
- `./arduino-1.8.13/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-1.8.13/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-1.8.13/hardware/teensy/avr/cores/teensy4/Stream.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
These files must be copied into the directory:

- `./arduino-x.yy.zz/hardware/teensy/avr/boards.txt`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy3/Stream.h`
- `./arduino-x.yy.zz/hardware/teensy/avr/cores/teensy4/Stream.h`

#### 3. For Arduino SAM DUE boards
 
 **To be able to compile and run on SAM DUE boards**, you have to copy the whole [SAM DUE](Packages_Patches/arduino/hardware/sam/1.6.12) directory into Arduino sam directory (~/.arduino15/packages/arduino/hardware/sam/1.6.12). 

Supposing the Arduino SAM core version is 1.6.12. This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/1.6.12/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/sam/x.yy.zz/platform.txt`

#### 4. For Arduino SAMD boards
 
 ***To be able to compile without error and automatically detect and display BOARD_NAME on Arduino SAMD (Nano-33-IoT, etc) boards***, you have to copy the whole [Arduino SAMD cores 1.8.10](Packages_Patches/arduino/hardware/samd/1.8.10) directory into Arduino SAMD directory (~/.arduino15/packages/arduino/hardware/samd/1.8.10).
 
#### For core version v1.8.10+

Supposing the Arduino SAMD version is 1.8.11. Now only one file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.11/platform.txt`

Whenever a new version is installed, remember to copy this files into the new version directory. For example, new version is x.yy.zz

This file must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.zz/platform.txt`
 
#### For core version v1.8.9-

Supposing the Arduino SAMD version is 1.8.9. These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/1.8.9/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/1.8.9/cores/arduino/Arduino.h`***

Whenever a new version is installed, remember to copy these files into the new version directory. For example, new version is x.yy.z

These files must be copied into the directory:

- `~/.arduino15/packages/arduino/hardware/samd/x.yy.z/platform.txt`
- ***`~/.arduino15/packages/arduino/hardware/samd/x.yy.z/cores/arduino/Arduino.h`***
 
 This is mandatory to fix the ***notorious Arduino SAMD compiler error***. See [Improve Arduino compatibility with the STL (min and max macro)](https://github.com/arduino/ArduinoCore-samd/pull/399)
 
```
 ...\arm-none-eabi\include\c++\7.2.1\bits\stl_algobase.h:243:56: error: macro "min" passed 3 arguments, but takes just 2
     min(const _Tp& __a, const _Tp& __b, _Compare __comp)
```

Whenever the above-mentioned compiler error issue is fixed with the new Arduino SAMD release, you don't need to copy the `Arduino.h` file anymore.

#### 5. For Adafruit SAMD boards
 
 ***To be able to automatically detect and display BOARD_NAME on Adafruit SAMD (Itsy-Bitsy M4, etc) boards***, you have to copy the file [Adafruit SAMD platform.txt](Packages_Patches/adafruit/hardware/samd/1.6.7) into Adafruit samd directory (~/.arduino15/packages/adafruit/hardware/samd/1.6.7). 

Supposing the Adafruit SAMD core version is 1.6.7. This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/1.6.7/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/adafruit/hardware/samd/x.yy.zz/platform.txt`

#### 6. For Seeeduino SAMD boards
 
 ***To be able to automatically detect and display BOARD_NAME on Seeeduino SAMD (XIAO M0, Wio Terminal, etc) boards***, you have to copy the file [Seeeduino SAMD platform.txt](Packages_Patches/Seeeduino/hardware/samd/1.8.1) into Adafruit samd directory (~/.arduino15/packages/Seeeduino/hardware/samd/1.8.1). 

Supposing the Seeeduino SAMD core version is 1.8.1. This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/1.8.1/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/Seeeduino/hardware/samd/x.yy.zz/platform.txt`

#### 7. For STM32 boards

#### 7.1 For STM32 boards to use LAN8720

To use LAN8720 on some STM32 boards 

- **Nucleo-144 (F429ZI, NUCLEO_F746NG, NUCLEO_F746ZG, NUCLEO_F756ZG)**
- **Discovery (DISCO_F746NG)**
- **STM32F4 boards (BLACK_F407VE, BLACK_F407VG, BLACK_F407ZE, BLACK_F407ZG, BLACK_F407VE_Mini, DIYMORE_F407VGT, FK407M1)**

you have to copy the files [stm32f4xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/1.9.0/system/STM32F4xx) and [stm32f7xx_hal_conf_default.h](Packages_Patches/STM32/hardware/stm32/1.9.0/system/STM32F7xx) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system) to overwrite the old files.

Supposing the STM32 stm32 core version is 1.9.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/STM32F4xx/stm32f4xx_hal_conf_default.h` for STM32F4.
- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/STM32F7xx/stm32f7xx_hal_conf_default.h` for Nucleo-144 STM32F7.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F4xx/stm32f4xx_hal_conf_default.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/system/STM32F7xx/stm32f7xx_hal_conf_default.h


#### 7.2 For STM32 boards to use Serial1

**To use Serial1 on some STM32 boards without Serial1 definition (Nucleo-144 NUCLEO_F767ZI, Nucleo-64 NUCLEO_L053R8, etc.) boards**, you have to copy the files [STM32 variant.h](Packages_Patches/STM32/hardware/stm32/1.9.0) into STM32 stm32 directory (~/.arduino15/packages/STM32/hardware/stm32/1.9.0). You have to modify the files corresponding to your boards, this is just an illustration how to do.

Supposing the STM32 stm32 core version is 1.9.0. These files must be copied into the directory:

- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/variants/NUCLEO_F767ZI/variant.h` for Nucleo-144 NUCLEO_F767ZI.
- `~/.arduino15/packages/STM32/hardware/stm32/1.9.0/variants/NUCLEO_L053R8/variant.h` for Nucleo-64 NUCLEO_L053R8.

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz,
theses files must be copied into the corresponding directory:

- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/NUCLEO_F767ZI/variant.h`
- `~/.arduino15/packages/STM32/hardware/stm32/x.yy.zz/variants/NUCLEO_L053R8/variant.h`

#### 8. For RP2040-based boards using [Earle Philhower arduino-pico core](https://github.com/earlephilhower/arduino-pico)

#### 8.1 To use BOARD_NAME

 **To be able to automatically detect and display BOARD_NAME on RP2040-based boards (RASPBERRY_PI_PICO, ADAFRUIT_FEATHER_RP2040, GENERIC_RP2040, etc) boards**, you have to copy the file [RP2040 platform.txt](Packages_Patches/rp2040/hardware/rp2040/1.4.0) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0). 

Supposing the rp2040 core version is 1.4.0. This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/platform.txt`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied into the directory:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/platform.txt`

With core after v1.4.0, this step is not necessary anymore thanks to the PR [Add -DBOARD_NAME="{build.board}" #136](https://github.com/earlephilhower/arduino-pico/pull/136).

#### 8.2 To avoid compile error relating to microsecondsToClockCycles

Some libraries, such as [Adafruit DHT-sensor-library](https://github.com/adafruit/DHT-sensor-library), require the definition of microsecondsToClockCycles(). **To be able to compile and run on RP2040-based boards**, you have to copy the files in [**RP2040 Arduino.h**](Packages_Patches/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h) into rp2040 directory (~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0).

Supposing the rp2040 core version is 1.4.0. This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/1.4.0/cores/rp2040/Arduino.h`

Whenever a new version is installed, remember to copy this file into the new version directory. For example, new version is x.yy.zz
This file must be copied to replace:

- `~/.arduino15/packages/rp2040/hardware/rp2040/x.yy.zz/cores/rp2040/Arduino.h`

With core after v1.4.0, this step is not necessary anymore thanks to the PR [Add defs for compatibility #142](https://github.com/earlephilhower/arduino-pico/pull/142).

---
---

### Libraries' Patches

#### 1. For Adafruit_MQTT_Library

To avoid `dtostrf` compile error in some MQTT-related examples, fix the [`Adafruit_MQTT_Library`](https://github.com/adafruit/Adafruit_MQTT_Library) by copying the following file into the `~/Arduino/libraries/Adafruit_MQTT_Library` directory to overwrite the old file:

- [Adafruit_MQTT_Library.cpp](LibraryPatches/Adafruit_MQTT_Library/Adafruit_MQTT.cpp)

---
---

## HOWTO Install esp32 core for ESP32-S2 (Saola, AI-Thinker ESP-12K) and ESP32-C3 boards into Arduino IDE


These are instructions demonstrating the steps to install esp32-s2/c3 core on Ubuntu machines. For Windows or other OS'es, just follow the the similar principles and steps.

* Windows 10, follows these steps in [Steps to install Arduino ESP32 support on Windows](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/windows.md) 

* Mac OS, follows these steps in [Installation instructions for Mac OS](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/mac.md)

* Fedora, follows these steps in [Installation instructions for Fedora](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/fedora.md)

* openSUSE, follows these steps in [Installation instructions for openSUSE](https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/opensuse.md)

* You can also try to add [package_esp32_dev_index.json](https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json) into Arduino IDE `File - Preferences - Additional Boards Manager URLs` 


```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_dev_index.json
```

and have Board Manager auto-install the **development** esp32 core. For example : esp32 core `v2.0.0-alpha1`


---

If you're already successful in testing the core, after installing by using the above procedures, you don't need to follows the hereafter manual steps.

---

Assuming you already installed Arduino IDE ESP32 core and the installed directory is

`/home/your_account/.arduino15/packages/esp32`


### 1. Save the original esp32 core

First, copy the whole original esp32 core to another safe place. Then delete all the sub-directories of

`/home/your_account/.arduino15/packages/esp32/hardware/esp32/1.0.4`

---


### 2. Install esp32 core v1.0.6

#### 2.1 Install esp32 core

Just use Arduino IDE Board Manager to install [ESP32 Arduino Release 1.0.6 based on ESP-IDF v3.3.5](https://github.com/espressif/arduino-esp32/releases/tag/1.0.6). This official v1.06 core doesn't have esp32-s2/s3 support. You have to download and use the latest master branch.


#### 2.2 Download latest zip with esp32-s2 support

As of **April 16th 2021**, the **esp32-s2/c3** board support has been included in master branch of esp32 core. Download [**esp32 core, master branch**](https://github.com/espressif/arduino-esp32) in the zip format.

#### 2.3 Unzip

<p align="center">
    <img src="https://github.com/khoih-prog/NB_Generic/blob/main/pics/esp32_s2_Core_Unzipped.png">
</p>

#### 2.4 Update esp32 core directories

Copy all subdirectories of esp32 core into `/home/your_account/.arduino15/packages/esp32/hardware/esp32/1.0.6`


---

### 3 Download tools for ESP32-S2


#### 3.1 Download Toolchain for Xtensa (ESP32-S2) based on GCC

Download [**esp32-s2 Toolchain**](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/tools/idf-tools.html#xtensa-esp32s2-elf) corresponding to your environment (linux-amd64, win64, etc.).

For example `xtensa-esp32s2-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz`, then un-archive.


<p align="center">
    <img src="https://github.com/khoih-prog/NB_Generic/blob/main/pics/esp32_s2_Toolchain.png">
</p>

#### 3.2 Download esptool


Download [esptool](https://github.com/espressif/esptool/releases) int the `zip` format:

`esptool-3.0.zip`

#### 3.3 Unzip

<p align="center">
    <img src="https://github.com/khoih-prog/NB_Generic/blob/main/pics/esp32_s2_esptool.png">
</p>

---

### 4. Update tools

#### 4.1 Update Toolchain

Copy whole `xtensa-esp32s2-elf` directory into `/home/your_account/.arduino15/packages/esp32/hardware/esp32/1.0.6/tools`


#### 4.2 Update esptool

Rename `esptool-3.0` directory to `esptool`


Copy whole `esptool` directory into `/home/your_account/.arduino15/packages/esp32/hardware/esp32/1.0.6/tools`


<p align="center">
    <img src="https://github.com/khoih-prog/NB_Generic/blob/main/pics/esp32_s2_tools.png">
</p>


### 5 Download tools for ESP32-C3

Download [**esp32-c3 Toolchain**](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-guides/tools/idf-tools.html#riscv32-esp-elf) corresponding to your environment (linux-amd64, win64, etc.).

For example`riscv32-esp-elf-gcc8_4_0-crosstool-ng-1.24.0-123-g64eb9ff-linux-amd64.tar.gz`, then un-archive.

Then using the similar steps as in

* [3. Download tools for ESP32-S2](#3-download-tools-for-esp32-s2) 
  * [3.1 Download Toolchain for Xtensa (ESP32-S2) based on GCC](#31-download-toolchain-for-xtensa-esp32-s2-based-on-gcc)
  * [3.2 Download esptool](#32-download-esptool)
  * [3.3 Unzip](#33-unzip)
* [4. Update tools](#4-update-tools)
  * [4.1 Update Toolchain](#41-update-toolchain)
  * [4.2 Update esptool](#42-update-esptool)

then copy whole `riscv32-esp-elf` directory into `/home/your_account/.arduino15/packages/esp32/hardware/esp32/1.0.6/tools`

<p align="center">
    <img src="https://github.com/khoih-prog/NB_Generic/blob/main/pics/NB_Generic_ESP32_C3_Support.png">
</p>


### 6. esp32-s2 WebServer Library Patch

#### Necessary only for esp32 core v1.0.6-

If you haven't installed a new version with [WebServer.handleClient delay PR #4350](https://github.com/espressif/arduino-esp32/pull/4350) or haven't applied the above mentioned PR, you have to use the following patch.


**To be able to run Config Portal on ESP32-S2 boards**, you have to copy the files in [esp32-s2 WebServer Patch](esp32s2_WebServer_Patch/) directory into esp32-s2 WebServer library directory (~/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries/WebServer).

Supposing the esp32-s2 version is 1.0.4, these files `WebServer.h/cpp` must be copied into the directory to replace:

- `~/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries/WebServer/src/WebServer.h`
- `~/.arduino15/packages/esp32/hardware/esp32/1.0.4/libraries/WebServer/src/WebServer.cpp`


---

That's it. You're now ready to compile and test for **ESP32-S2 and ESP32-C3** now

---
---

### Note for Platform IO using ESP32 LittleFS

#### Necessary only for esp32 core v1.0.6-

From esp32 core v1.0.6+, [`LittleFS_esp32 v1.0.6`](https://github.com/lorol/LITTLEFS) has been included and this step is not necessary anymore.

In Platform IO, to fix the error when using [`LittleFS_esp32 v1.0`](https://github.com/lorol/LITTLEFS) for ESP32-based boards with ESP32 core v1.0.4- (ESP-IDF v3.2-), uncomment the following line

from

```
//#define CONFIG_LITTLEFS_FOR_IDF_3_2   /* For old IDF - like in release 1.0.4 */
```

to

```
#define CONFIG_LITTLEFS_FOR_IDF_3_2   /* For old IDF - like in release 1.0.4 */
```

It's advisable to use the latest [`LittleFS_esp32 v1.0.5+`](https://github.com/lorol/LITTLEFS) to avoid the issue.

Thanks to [Roshan](https://github.com/solroshan) to report the issue in [Error esp_littlefs.c 'utime_p'](https://github.com/khoih-prog/ESPAsync_WiFiManager/issues/28) 

---
---

### Reference

Currently, check [Reference of MKRNB](https://www.arduino.cc/en/Reference/MKRNB)

---
---

## Documentation

### 1. How it works

This library enable you to do most of the operations you can do with a NB phone: place and receive voice calls, send and receive SMS, and connect to the internet over a GPRS network. The onboard NB-IoT/LTE-M/GPRS module, operates in 3G with a 2G fallback.

The NB-IoT/LTE-M/GPRS modem transfers data, received from a serial port, to the NB-IoT/LTE-M/GPRS network. The modem executes operations via a series of AT commands. The library abstracts low level communications between the modem and SIM card. It relies on the Serial library for communication between the modem and your board.

### 2. Library structure

As the library enables multiple types of functionality, there are a number of different classes.

 1. `NB` class takes care of commands to the radio modem. This handles the connectivity aspects of the module and registers your system in the NB infrastructure. All of your NB-IoT/LTE-M/GPRS programs will need to include an object of this class to handle the necessary low level communication.
 2. `NB_SMS` class manages Sending/Receiving SMS messages.
 3. `GPRS` class manages connecting to the internet.
 4. `NBClient` class includes implementations for a client, similar to the Ethernet and WiFi libraries.
 5. `NBUDP` class enables UDP message to be sent and received
 6. `NBModem` class facilitates diagnostic communication with the modem.
 7. `NBScanner` class provides diagnostic information about the network and carrier.
 8. `NBPIN` class provides utilities for communicating with the SIM card.

---
---

### 3. Library main classes

### NB Class

#### Description

`NB` class is the base class for all NB based functions to prepare communicating with the modem.

#### Syntax

```
NB(bool debug = false);
```

#### Parameters

`debug`: bool (default FALSE) flag for turning on the debug mode. This will print out the AT commands from the modem.

#### Public Functions

```
/** Constructor
    @param debug    Determines debug mode
*/
NB(bool debug = false);

/** Start the NB IoT modem, attaching to the NB IoT or LTE Cat M1 network
    @param pin         SIM PIN number (4 digits in a string, example: "1234"). If
                       NULL the SIM has no configured PIN.
    @param apn         (optional) APN to use
    @param restart     Restart the modem. Default is TRUE. The modem receives
                       a signal through the Ctrl/D7 pin. If it is shut down, it will
                       start-up. If it is running, it will restart. Takes up to 10
                       seconds
    @param synchronous If TRUE the call only returns after the Start is complete
                       or fails. If FALSE the call will return immediately. You have
                       to call repeatedly ready() until you get a result. Default is TRUE.
    @return If synchronous, NB_NetworkStatus_t. If asynchronous, returns 0.
  */
NB_NetworkStatus_t begin(const char* pin = 0, bool restart = true, bool synchronous = true);
NB_NetworkStatus_t begin(const char* pin, const char* apn, bool restart = true, bool synchronous = true);
NB_NetworkStatus_t begin(const char* pin, const char* apn, const char* username, const char* password, bool restart = true, bool synchronous = true);

/** Check network access status
    @return 1 if Alive, 0 if down
 */
int isAccessAlive();

/** Shutdown the modem (power off really)
    @return true if successful
  */
bool shutdown();

/** Secure shutdown the modem (power off really)
    @return always true
  */
bool secureShutdown();

/** Get last command status
    @return returns 0 if last command is still executing, 1 success, >1 error
  */
int ready();

void setTimeout(unsigned long timeout);

unsigned long getTime();
unsigned long getLocalTime();
bool setTime(unsigned long const epoch, int const timezone = 0);

NB_NetworkStatus_t status();
```

---

### NB_SMS Class

#### Description

`NB_SMS` is the base class for all NB functions handling receiving and sending SMS messages.

#### Syntax

```
NB_SMS(bool synch = true);
```

#### Parameters

`synch`: bool (default TRUE) flag for the SMS service to be synchronous

#### Public Functions

```
/** Constructor
    @param synch    Determines sync mode
 */
NB_SMS(bool synch = true);

/** Write a character in SMS message
    @param c      Character
    @return size
  */
size_t write(uint8_t c);

/** Select SMS charset
    @param charset     Character set, one of "IRA" (default), "GSM", or "UCS2", reads from modem if null.
    @return returns first char of charset identifier on success and 0 on error
  */
int setCharset(const char* charset = nullptr);

/** Begin a SMS to send it
    @param to     Destination
    @return error command if it exists
  */
int beginSMS(const char* to);

/** Get last command status
    @return returns 0 if last command is still executing, 1 success, >1 error
 */
int ready();

/** End SMS
    @return error command if it exists
 */
int endSMS();

/** Check if SMS available and prepare it to be read
    @return number of bytes in a received SMS
 */
int available();

/** Read sender number phone
    @param number   Buffer for save number phone
    @param nlength    Buffer length
    @return 1 success, >1 error
 */
int remoteNumber(char* number, int nlength); 

/** Read one char for SMS buffer (advance circular buffer)
    @return byte
 */
int read();

/** Read a byte but do not advance the buffer header (circular buffer)
    @return byte
 */
int peek();

/** Delete the SMS from Modem memory and process answer
 */
void flush();

/** Delete all read and sent SMS from Modem memory and process answer
 */
void clear(int flag = NB_SMS_CLEAR_READ_SENT);
```

---

### GPRS Class

#### Description

`GPRS` is the base class for all GPRS functions and is responsible for including the files that are part of the library that involve TCP, UDP and SSL communication.

#### Syntax

```
GPRS();
```

#### Parameters

none.

#### Public Functions

```
GPRS();
virtual ~GPRS();

/** Attach to GPRS/NB network
    @return connection status
 */
NB_NetworkStatus_t networkAttach()
{
    return attachGPRS();
};

/** Detach GPRS/NB network
    @return connection status
 */
NB_NetworkStatus_t networkDetach(){ return detachGPRS(); };


/** Returns 0 if last command is still executing
    @return 1 if success, >1 if error
 */
int ready();

/** Attach to GPRS service
    @param synchronous  Sync mode
    @return connection status
 */
NB_NetworkStatus_t attachGPRS(bool synchronous = true);

/** Detach GPRS service
    @param synchronous  Sync mode
    @return connection status
 */
NB_NetworkStatus_t detachGPRS(bool synchronous = true);

/** Get actual assigned IP address in IPAddress format
    @return IP address in IPAddress format
 */
IPAddress getIPAddress();
void setTimeout(unsigned long timeout);
NB_NetworkStatus_t status();
```

---

### NBClient Class

#### Description

`NBClient` is the base class for all GPRS client based functions to create clients that can connect to servers and send and receive data.
It is not called directly, but invoked whenever you use a function that relies on it.

#### Syntax

```
NBClient(bool synch = true);
```

#### Parameters

`synch`: bool (default TRUE) flag for the Client using synchronous mode

#### Public Functions

```
/** Constructor
    @param synch    Sync mode
 */
NBClient(bool synch = true);

/** Constructor
    @param socket   Socket
    @param synch    Sync mode
 */
NBClient(int socket, bool synch);

virtual ~NBClient();

/** Get last command status
    @return returns 0 if last command is still executing, 1 success, >1 error
*/
virtual int ready();

/** Connect to server by IP address
    @param (IPAddress)
    @param (uint16_t)
    @return returns 0 if last command is still executing, 1 success, 2 if there are no resources
 */
int connect(IPAddress, uint16_t);
int connectSSL(IPAddress, uint16_t);

/** Connect to server by hostname
    @param host     Hostname
    @param port     Port
    @return returns 0 if last command is still executing, 1 success, 2 if there are no resources
 */
int connect(const char *host, uint16_t port);
int connectSSL(const char *host, uint16_t port);

/** Initialize write in request
    @param sync     Sync mode
 */
void beginWrite(bool sync = false);

/** Write a character in request
    @param c      Character
    @return size
 */
size_t write(uint8_t c);

/** Write a characters buffer in request
    @param buf      Buffer
    @return buffer size
 */
size_t write(const uint8_t *buf);

/** Write a characters buffer with size in request
    @param (uint8_t*) Buffer
    @param (size_t)   Buffer size
    @return buffer size
 */
size_t write(const uint8_t*, size_t);

/** Finish write request
    @param sync     Sync mode
 */
void endWrite(bool sync = false);

/** Check if connected to server
    @return 1 if connected
 */
uint8_t connected();

operator bool();

/** Read from response buffer and copy size specified to buffer
    @param buf      Buffer    
    @param size     Buffer size
    @return bytes read
 */
int read(uint8_t *buf, size_t size);

/** Read a character from response buffer
    @return character
 */
int read();

/** Check if exists a response available
    @return 1 if exists, 0 if not exists
 */
int available();

/** Read a character from response buffer but does not move the pointer.
    @return character
 */
int peek();

/** Flush response buffer
 */
void flush();

/** Stop client
 */
void stop();

virtual void handleUrc(const String& urc);
```

---

### NBSSLClient Class

#### Description

`NBSSLClient` is the base class for all GPRS SSL client based functions to create clients that can connect to servers and send and receive data.
It is not called directly, but invoked whenever you use a function that relies on it.

#### Syntax

```
NBSSLClient(bool synch = true);
```

#### Parameters

`synch`: bool (default TRUE) flag for the SSL Client using synchronous mode

#### Public Functions

```
NBSSLClient(bool synch = true);
NBSSLClient(const NBRootCert* myRCs, int myNumRCs, bool synch = true);
virtual ~NBSSLClient();

virtual int ready();
virtual int iterateCerts();

virtual int connect(IPAddress ip, uint16_t port);
virtual int connect(const char* host, uint16_t port);
```

---

### NBModem Class

#### Description

`NBModem` is the base class for functions facilitating diagnostic communication with the modem.
It is not called directly, but invoked whenever you use a function that relies on it.


#### Syntax

```
NBModem();
```

#### Parameters

none

#### Public Functions

```
/** Constructor */
NBModem();

/** Check modem response and restart it
 */
int begin();

/** Obtain modem IMEI (command AT)
    @return modem IMEI number
 */
String getIMEI();

/** Obtain SIM card ICCID (command AT)
    @return SIM ICCID number
 */
String getICCID();
```

---

### NBScanner Class

#### Description

`NBScanner` is the base class for functions providing diagnostic information about the network and carrier.
It is not called directly, but invoked whenever you use a function that relies on it.


#### Syntax

```
NBScanner(bool trace = false);
```

#### Parameters

`trace`: bool (default FALSE) flag. If TRUE, dumps all AT dialogue to Serial

#### Public Functions

```
/** Constructor
    @param trace    if true, dumps all AT dialogue to Serial
    @return - 
*/
NBScanner(bool trace = false);

/** begin (forces modem hardware restart, so we begin from scratch)
    @return Always returns IDLE status
*/
NB_NetworkStatus_t begin();

/** Read current carrier
    @return Current carrier
 */
String getCurrentCarrier();

/** Obtain signal strength
    @return Signal Strength
 */
String getSignalStrength();

/** Search available carriers
  @return A string with list of networks available
 */
String readNetworks();
```

---

### NBPIN Class

#### Description

`NBPIN` is the base class for all NB based functions that deal with interacting with the PIN on the SIM card.


#### Syntax

```
NBPIN();
```

#### Parameters

none

#### Public Functions

```
/** Constructor */
NBPIN();

/** Check modem response and restart it
 */
void begin();

/** Check if PIN lock or PUK lock is activated
    @return 0 if PIN lock is off, 1 if PIN lock is on, -1 if PUK lock is on, -2 if error exists
 */
int isPIN();

/** Check if PIN code is correct and valid
    @param pin      PIN code
    @return 0 if is correct, -1 if is incorrect
 */
int checkPIN(String pin);

/** Check if PUK code is correct and establish new PIN code
    @param puk      PUK code
    @param pin      New PIN code
    @return 0 if successful, otherwise return -1
 */
int checkPUK(String puk, String pin);

/** Change PIN code
    @param old      Old PIN code
    @param pin      New PIN code
 */
void changePIN(String old, String pin);

/** Change PIN lock status
    @param pin      PIN code
 */
void switchPIN(String pin);

/** Check if modem was registered in NB-IoT/LTE-M/GPRS network
    @return 0 if modem was registered, 1 if modem was registered in roaming, -1 if error exists
 */
int checkReg();

/** Return if PIN lock is used
    @return true if PIN lock is used, otherwise, return false
 */
bool getPINUsed();

/** Set PIN lock status
    @param used     New PIN lock status
 */
void setPINUsed(bool used);
```

---

### NBUDP Class

#### Description

`NBUDP` is the base class for all NB based functions enabling UDP message to be sent and received.


#### Syntax

```
NBUDP();
```

#### Parameters

none

#### Public Functions

```
NBUDP();  // Constructor
virtual ~NBUDP();

virtual uint8_t begin(uint16_t);  // initialize, start listening on specified port. Returns 1 if successful, 0 if there are no sockets available to use
virtual void stop();  // Finish with the UDP socket

// Sending UDP packets

// Start building up a packet to send to the remote host specific in ip and port
// Returns 1 if successful, 0 if there was a problem with the supplied IP address or port
virtual int beginPacket(IPAddress ip, uint16_t port);
// Start building up a packet to send to the remote host specific in host and port
// Returns 1 if successful, 0 if there was a problem resolving the hostname or port
virtual int beginPacket(const char *host, uint16_t port);
// Finish off this packet and send it
// Returns 1 if the packet was sent successfully, 0 if there was an error
virtual int endPacket();
// Write a single byte into the packet
virtual size_t write(uint8_t);
// Write size bytes from buffer into the packet
virtual size_t write(const uint8_t *buffer, size_t size);

using Print::write;

// Start processing the next available incoming packet
// Returns the size of the packet in bytes, or 0 if no packets are available
virtual int parsePacket();
// Number of bytes remaining in the current packet
virtual int available();
// Read a single byte from the current packet
virtual int read();
// Read up to len bytes from the current packet and place them into buffer
// Returns the number of bytes read, or 0 if none are available
virtual int read(unsigned char* buffer, size_t len);
// Read up to len characters from the current packet and place them into buffer
// Returns the number of characters read, or 0 if none are available
virtual int read(char* buffer, size_t len) { return read((unsigned char*)buffer, len); };
// Return the next byte from the current packet without moving on to the next byte
virtual int peek();
virtual void flush();	// Finish reading the current packet

// Return the IP address of the host who sent the current incoming packet
virtual IPAddress remoteIP();
// Return the port of the host who sent the current incoming packet
virtual uint16_t remotePort();

virtual void handleUrc(const String& urc);
```

---
---

### Configuration Notes

#### 1. How to select which NB-IoT/LTE-M/GPRS module to use


Select **one and only one** NB module to use (`true`) as follows:

```cpp
//////////////////////////////////////////////

#if defined(ARDUINO_SAMD_MKRNB1500)
  #define SerialNB      SerialSARA
  #define NB_RESETN     SARA_RESETN
  #define NB_PWR        SARA_PWR_ON
#else
  // Override the default (and certainly not good) pins and port
  // Only for boards other than ARDUINO_SAMD_MKRNB1500
  #define NB_RESETN  (10u)
  #define NB_PWR     (11u)

  #if ESP8266
    // Using Software Serial for ESP8266, as Serial1 is TX only
    #define NB_USING_SOFTWARE_SERIAL     true
  #else
    // Optional Software Serial here for other boards, but not advised if HW Serial available
    #define NB_USING_SOFTWARE_SERIAL     false
  #endif
   
  #if NB_USING_SOFTWARE_SERIAL
    #warning Using default SerialNB = SoftwareSerial
    
    #define D8 (15)
    #define D7 (13)
    
    #include <SoftwareSerial.h>
    
    SoftwareSerial swSerial(D7, D8);    // (D7, D8, false, 256); // (RX, TX, false, 256);
    
    #define SerialNB   swSerial
  #else
    #warning Using default SerialNB = HardwareSerial Serial1
    #define SerialNB   Serial1
  #endif    // NB_USING_SOFTWARE_SERIAL

  #warning You must connect the Modem correctly and modify the pins / Serial port here
  
#endif

//////////////////////////////////////////////

//////////////////////////////////////////////
#define NB_MODEM_SARAR4            true

//////////////////////////////////////////////
// Not supported yet
#define NB_MODEM_SIM7000           false
#define NB_MODEM_XBEE              false
#define NB_MODEM_SEQUANS_MONARCH   false
```

The unselected module `#define` can be deleted or left as is (`false`)

For example

```
// Only one #define with true is enough
#define NB_MODEM_UBLOX             true
```

#### 2. How to select which Serial port to communicate to NB module

Select the corresponding `Serial` port to use to communicate (sending AT conmmands and receiving responces) to NB module
Remember to connect the board and modem correspondingly.

```cpp
#define SerialNB   Serial1
```

#### 3. How to select extra pins to control NB module

For example, the **u-blox SARA U201** requires a **reset pin** and **another pin to control the low-power mode**.
Those pins are named here in this library as `NB_RESETN` and `NB_PWR`.

Default `NB_RESETN` pin is set as 10, and `NB_PWR` as 11. You can override the default pin settings in the sketch as follows:


```
// Optional usage of NB_RESETN and NB_PWR. Need to be here only when true. Default is false

#define UBLOX_USING_RESET_PIN             true
#define UBLOX_USING_LOW_POWER_MODE        true

// Override the default (and certainly not good) pins and port
// Only for boards other than ARDUINO_SAMD_MKRNB1500
#define NB_RESETN  (10u)
#define NB_PWR     (11u)
```

Depending on the NB module, the pins' requirements are different. 
Please follow the instructions for the related NB module.


---
---
 
### Examples

 1. [GPRSUdpNtpClient](examples/GPRSUdpNtpClient)
 2. [NBSSLWebClient](examples/NBSSLWebClient)
 3. [NBWebClient](examples/NBWebClient)
 4. [ReceiveSMS](examples/ReceiveSMS)
 5. [SendSMS](examples/SendSMS)

### Tools

 6. [ChooseRadioAccessTechnology](examples/Tools/ChooseRadioAccessTechnology)
 7. [NBScanNetworks](examples/Tools/NBScanNetworks)
 8. [PinManagement](examples/Tools/PinManagement)
 9. [SerialNBPassthrough](examples/Tools/SerialNBPassthrough)
10. [TestGPRS](examples/Tools/TestGPRS) 
11. [TestModem](examples/Tools/TestModem)

### Advanced Examples

#### MQTT

12. [NB_MQTT](examples/Advanced/NB_MQTT)
13. [NB_MQTTClient_Auth](examples/Advanced/NB_MQTTClient_Auth)
14. [NB_MQTTClient_Basic](examples/Advanced/NB_MQTTClient_Basic)
15. [NB_MQTT_Medium](examples/Advanced/NB_MQTT_Medium)
16. [NB_MQTT_ThingStream](examples/Advanced/NB_MQTT_ThingStream)

#### Blynk

17. [NB_Blynk](examples/Advanced/NB_Blynk)


---
---

### Example [NBWebClient](examples/NBWebClient)

#### 1. File [NBWebClient.ino](examples/NBWebClient/NBWebClient.ino)

```
#include "defines.h"

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

// initialize the library instance
NBClient client;
GPRS gprs;
NB nbAccess;

// URL, path and port (for example: example.org)
char server[] = "arduino.cc";         //"example.org";
char path[]   = "/asciilogo.txt";     //"/";
int port      = 80; // port 80 is the default for HTTP

// BaudRate to communicate to NB-IoT modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting NBWebClient on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif

  // connection state
  boolean connected = false;

  // After starting the modem with NB.begin() using PINNUMBER
  // attach to the GPRS network
  while (!connected)
  {
    if ((nbAccess.begin(PINNUMBER) == NB_READY) && (gprs.attachGPRS() == GPRS_READY))
    {
      connected = true;
    }
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, port))
  {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET ");
    client.print(path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println();
  }
  else
  {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() 
{
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available())
  {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.available() && !client.connected())
  {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    //while(true) { delay(1); }

    delay(1000);
  }
}
```

---

#### 2. File [defines.h](examples/NBWebClient/defines.h)

```
#ifndef defines_h
#define defines_h

#define DEBUG_NB_GENERIC_PORT       Serial

// Debug Level from 0 to 5. Level 5 is to print out AT commands and responses
#define _NB_GENERIC_LOGLEVEL_       5

#define SECRET_PINNUMBER     ""

//////////////////////////////////////////////

#if defined(ARDUINO_SAMD_MKRNB1500)
  #define SerialNB      SerialSARA
  #define NB_RESETN     SARA_RESETN
  #define NB_PWR        SARA_PWR_ON
#else
  // Override the default (and certainly not good) pins and port
  // Only for boards other than ARDUINO_SAMD_MKRNB1500
  #define NB_RESETN  (10u)
  #define NB_PWR     (11u)

  #if ESP8266
    // Using Software Serial for ESP8266, as Serial1 is TX only
    #define NB_USING_SOFTWARE_SERIAL     true
  #else
    // Optional Software Serial here for other boards, but not advised if HW Serial available
    #define NB_USING_SOFTWARE_SERIAL     false
  #endif
   
  #if NB_USING_SOFTWARE_SERIAL
    #warning Using default SerialNB = SoftwareSerial
    
    #define D8 (15)
    #define D7 (13)
    
    #include <SoftwareSerial.h>
    
    SoftwareSerial swSerial(D7, D8);    // (D7, D8, false, 256); // (RX, TX, false, 256);
    
    #define SerialNB   swSerial
  #else
    #warning Using default SerialNB = HardwareSerial Serial1
    #define SerialNB   Serial1
  #endif    // NB_USING_SOFTWARE_SERIAL

  #warning You must connect the Modem correctly and modify the pins / Serial port here
  
#endif

//////////////////////////////////////////////

//////////////////////////////////////////////

#define NB_MODEM_SARAR4                     true

//////////////////////////////////////////////
// Not supported yet
#define NB_MODEM_SIM7000                    false
#define NB_MODEM_XBEE                       false
#define NB_MODEM_SEQUANS_MONARCH            false

//////////////////////////////////////////////

// libraries
#include <NB_Generic_Main.h>


#endif    //defines_h
```

---
---

### Debug Terminal Output Samples



---
---

### Debug

Debug is enabled by default on Serial. Debug Level from 0 to 4. To disable, change the `_NB_Generic_LOGLEVEL_` to 0

```cpp
// Use this to output debug msgs to Serial
#define DEBUG_NB_GENERIC_PORT       Serial

// Debug Level from 0 to 5. Level 5 is to print out AT commands and responses
#define _NB_GENERIC_LOGLEVEL_       5
```

---

## Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the board's core or this library version.

Sometimes, the library will only work if you update the board core to the newer or older version because some function compatibility.

---
---

## Releases

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

1. Initial coding to add support to many boards besides MKRNB 1500 / u-blox NB-IoT / LTE-M modems (Sara R4xx NB-IoT/LTE-M modules (SARA-R404M, SARA-R410M, SARA-R412M, SARA-N410, etc)

---

### Issues ###

Submit issues to: [**NB_Generic issues**](https://github.com/khoih-prog/NB_Generic/issues)

---
---

### TO DO
1. Bug Searching and Killing
2. Support more types of NB-IoT/LTE-M/GPRS modules

---

### DONE

 1. Add support to **Adafruit SAMD21 (Itsy-Bitsy M0, Metro M0, Feather M0 Express, etc.)**.
 2. Add support to **Adafruit SAMD51 (Itsy-Bitsy M4, Metro M4, Grand Central M4, Feather M4 Express, METRO_M4_AIRLIFT_LITE, PYBADGE_AIRLIFT_M4, etc.)**.
 3. Add support to **Adafruit nRF52 ( Feather nRF52832, nRF52840 Express, BlueFruit Sense, Itsy-Bitsy nRF52840 Express, Metro nRF52840 Express, NINA_B302_ublox, NINA_B112_ublox, etc.)**.
 4. Add support to SAM DUE.
 5. Add support to Teensy.
 6. Add support to all **STM32F/L/H/G/WB/MP1 having 64K+** Flash program memory.
 7. Add support to **Seeeduino SAMD21/SAMD51 boards (SEEED_WIO_TERMINAL, SEEED_FEMTO_M0, SEEED_XIAO_M0, Wio_Lite_MG126, WIO_GPS_BOARD, SEEEDUINO_ZERO, SEEEDUINO_LORAWAN, SEEED_GROVE_UI_WIRELESS, etc.)**
 8. Add support to ESP32 (including ESP32-S2) and ESP8266 (only using Hardware Serial)
 9. Support u-blox Sara R4xx NB-IoT/LTE-M modules (SARA-R404M, SARA-R410M, SARA-R412M, SARA-N410, etc)


---
---

### Contributions and Thanks

1. Based on and modified from from [Arduino **MKRNB** library](https://github.com/arduino-libraries/MKRNB).Thanks to the great works of these [MKRNB Library's Contributors](https://github.com/arduino-libraries/MKRNB/graphs/contributors)
2. Thanks to good work of [Miguel Wisintainer](https://github.com/tcpipchip) for initiating, inspriring, working with, developing, debugging, testing and maintaining.


<table>
  <tr>
    <td align="center"><a href="https://github.com/tcpipchip"><img src="https://github.com/tcpipchip.png" width="100px;" alt="tcpipchip"/><br /><sub><b>⭐️ Miguel Wisintainer</b></sub></a><br /></td>
  </tr> 
</table>

---

### Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [The GNU Lesser General Public License (**LGPL-3.0**)](https://github.com/khoih-prog/NB_Generic/blob/main/LICENSE)

---

### Copyright

1. Copyright (c) 2018 Arduino SA. All rights reserved.
2. Copyright 2021- Khoi Hoang



