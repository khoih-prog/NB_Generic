/*********************************************************************************************************************************
  NB_Boards_Generic.h
  
  For ESP8266, ESP32, SAMD21/SAMD51, nRF52, SAM DUE, Teensy and STM32 with NB modules
  
  NB_Generic is a library for the ESP8266, ESP32, SAMD21/SAMD51, nRF52, SAM DUE, Teensy and STM32 with NB modules
  
  Based on and modified from MKRNB Library (https://github.com/arduino-libraries/MKRNB)
  
  Built by Khoi Hoang https://github.com/khoih-prog/NB_Generic
  Licensed under GNU Lesser General Public License
  
  Copyright (C) 2018  Arduino AG (http://www.arduino.cc/)
  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License 
  as published by the Free Software Foundation, either version 2.1 of the License, or (at your option) any later version.
  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty 
  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
  You should have received a copy of the GNU General Public License along with this program.
  If not, see <https://www.gnu.org/licenses/>.  
 
  Version: 1.3.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
  1.1.0    K Hoang     19/03/2021 Rewrite to prepare for supporting more GSM/GPRS modules. Add FileUtils examples.
  1.2.0    K Hoang     28/06/2021 Add support to RP2040-based boards using Arduino mbed or Arduino-pico core
  1.3.0    K Hoang     14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_BOARDS_GENERIC_H_INCLUDED
#define _NB_BOARDS_GENERIC_H_INCLUDED

#include <Arduino.h>

#if    ( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) || defined(__SAMD21G18A__) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(__SAMD21E18A__) || defined(__SAMD51__) || defined(__SAMD51J20A__) || defined(__SAMD51J19A__) \
      || defined(__SAMD51G19A__) || defined(__SAMD51P19A__) || defined(__SAMD21G18A__) )
  #if defined(NB_USE_SAMD)
    #undef NB_USE_SAMD
  #endif
  #define NB_USE_SAMD      true

#elif (defined(NRF52840_FEATHER) || defined(NRF52832_FEATHER) || defined(NRF52_SERIES) || defined(ARDUINO_NRF52_ADAFRUIT) || \
       defined(NRF52840_FEATHER_SENSE) || defined(NRF52840_ITSYBITSY) || defined(NRF52840_CIRCUITPLAY) || \
       defined(NRF52840_CLUE) || defined(NRF52840_METRO) || defined(NRF52840_PCA10056) || defined(PARTICLE_XENON) || \
       defined(MDBT50Q_RX) || defined(NINA_B302_ublox) || defined(NINA_B112_ublox) )
  #if defined(NB_USE_NRF528XX)
    #undef NB_USE_NRF528XX
  #endif
  #define NB_USE_NRF528XX      true
  
  #include <Adafruit_TinyUSB.h>
 
#elif ( defined(ARDUINO_SAM_DUE) || defined(__SAM3X8E__) )
  #if defined(NB_USE_SAM_DUE)
    #undef NB_USE_SAM_DUE
  #endif
  #define NB_USE_SAM_DUE      true

#elif ( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
  #if defined(NB_USE_STM32)
    #undef NB_USE_STM32
  #endif
  #define NB_USE_STM32      true

#elif ( defined(ARDUINO_NANO_RP2040_CONNECT) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_RASPBERRY_PI_PICO) || \
      defined(ARDUINO_GENERIC_RP2040) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) )
  #if defined(NB_USE_RP2040)
    #undef NB_USE_RP2040
  #endif
  #define NB_USE_RP2040      true
    
#endif

///////////////////////////////////////////

#if defined(NB_USE_SAMD)

  #if !defined(ARDUINO_SAMD_MKRNB1500)
    // Must define these pins for SAMD
    #if !defined(SerialNB)
      #warning Using default SerialNB = Serial1

      #define SerialNB   Serial1
    #else
      #warning Using SerialNB from sketch  
    #endif
    
    #if !( defined(NB_RESETN) && defined(NB_PWR) )
      #warning Using default NB_RESETN(10), NB_PWR(11)
      #define NB_RESETN  (10u)
      #define NB_PWR     (11u)

    #else
      #warning Using NB_RESETN and NB_PWR pins from sketch  
    #endif
    
    #warning Using SAMD different from MKRNB1500
  #endif

  #if ( defined(ARDUINO_SAMD_ZERO) && !defined(SEEED_XIAO_M0) )
    #define BOARD_TYPE      "SAMD Zero"
  #elif defined(ARDUINO_SAMD_MKR1000)
    #define BOARD_TYPE      "SAMD MKR1000"
  #elif defined(ARDUINO_SAMD_MKRWIFI1010)
    #define BOARD_TYPE      "SAMD MKRWIFI1010"
  #elif defined(ARDUINO_SAMD_NANO_33_IOT)
    #define BOARD_TYPE      "SAMD NANO_33_IOT"
  #elif defined(ARDUINO_SAMD_MKRFox1200)
    #define BOARD_TYPE      "SAMD MKRFox1200"
  #elif ( defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) )
    #define BOARD_TYPE      "SAMD MKRWAN13X0"
  #elif defined(ARDUINO_SAMD_MKRGSM1400)
    #define BOARD_TYPE      "SAMD MKRGSM1400"
  #elif defined(ARDUINO_SAMD_MKRNB1500)
    #define BOARD_TYPE      "SAMD MKRNB1500"
  #elif defined(ARDUINO_SAMD_MKRVIDOR4000)
    #define BOARD_TYPE      "SAMD MKRVIDOR4000"
  #elif defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS)
    #define BOARD_TYPE      "SAMD ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS"
  #elif defined(ADAFRUIT_FEATHER_M0_EXPRESS)
    #define BOARD_TYPE      "SAMD21 ADAFRUIT_FEATHER_M0_EXPRESS"
  #elif defined(ADAFRUIT_METRO_M0_EXPRESS)
    #define BOARD_TYPE      "SAMD21 ADAFRUIT_METRO_M0_EXPRESS"
  #elif defined(ADAFRUIT_CIRCUITPLAYGROUND_M0)
    #define BOARD_TYPE      "SAMD21 ADAFRUIT_CIRCUITPLAYGROUND_M0"
  #elif defined(ADAFRUIT_GEMMA_M0)
    #define BOARD_TYPE      "SAMD21 ADAFRUIT_GEMMA_M0"
  #elif defined(ADAFRUIT_TRINKET_M0)
    #define BOARD_TYPE      "SAMD21 ADAFRUIT_TRINKET_M0"
  #elif defined(ADAFRUIT_ITSYBITSY_M0)
    #define BOARD_TYPE      "SAMD21 ADAFRUIT_ITSYBITSY_M0"
  #elif defined(ARDUINO_SAMD_HALLOWING_M0)
    #define BOARD_TYPE      "SAMD21 ARDUINO_SAMD_HALLOWING_M0"
  #elif defined(ADAFRUIT_METRO_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_METRO_M4_EXPRESS"
  #elif defined(ADAFRUIT_GRAND_CENTRAL_M4)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_GRAND_CENTRAL_M4"
  #elif defined(ADAFRUIT_FEATHER_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_FEATHER_M4_EXPRESS"
  #elif defined(ADAFRUIT_ITSYBITSY_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_ITSYBITSY_M4_EXPRESS"
    #define USE_THIS_SS_PIN       10
  #elif defined(ADAFRUIT_TRELLIS_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_TRELLIS_M4_EXPRESS"
  #elif defined(ADAFRUIT_PYPORTAL)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYPORTAL"
  #elif defined(ADAFRUIT_PYPORTAL_M4_TITANO)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYPORTAL_M4_TITANO"
  #elif defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYBADGE_M4_EXPRESS"
  #elif defined(ADAFRUIT_METRO_M4_AIRLIFT_LITE)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_METRO_M4_AIRLIFT_LITE"
  #elif defined(ADAFRUIT_PYGAMER_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYGAMER_M4_EXPRESS"
  #elif defined(ADAFRUIT_PYGAMER_ADVANCE_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYGAMER_ADVANCE_M4_EXPRESS"
  #elif defined(ADAFRUIT_PYBADGE_AIRLIFT_M4)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_PYBADGE_AIRLIFT_M4"
  #elif defined(ADAFRUIT_MONSTER_M4SK_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_MONSTER_M4SK_EXPRESS"
  #elif defined(ADAFRUIT_HALLOWING_M4_EXPRESS)
    #define BOARD_TYPE      "SAMD51 ADAFRUIT_HALLOWING_M4_EXPRESS"
  #elif defined(SEEED_WIO_TERMINAL)
    #define BOARD_TYPE      "SAMD SEEED_WIO_TERMINAL"
  #elif defined(SEEED_FEMTO_M0)
    #define BOARD_TYPE      "SAMD SEEED_FEMTO_M0"
  #elif defined(SEEED_XIAO_M0)
    #define BOARD_TYPE      "SAMD SEEED_XIAO_M0"
  #elif defined(Wio_Lite_MG126)
    #define BOARD_TYPE      "SAMD SEEED Wio_Lite_MG126"
  #elif defined(WIO_GPS_BOARD)
    #define BOARD_TYPE      "SAMD SEEED WIO_GPS_BOARD"
  #elif defined(SEEEDUINO_ZERO)
    #define BOARD_TYPE      "SAMD SEEEDUINO_ZERO"
  #elif defined(SEEEDUINO_LORAWAN)
    #define BOARD_TYPE      "SAMD SEEEDUINO_LORAWAN"
  #elif defined(SEEED_GROVE_UI_WIRELESS)
    #define BOARD_TYPE      "SAMD SEEED_GROVE_UI_WIRELESS"
  #elif defined(__SAMD21E18A__)
    #define BOARD_TYPE      "SAMD21E18A"
  #elif defined(__SAMD21G18A__)
    #define BOARD_TYPE      "SAMD21G18A"
  #elif defined(__SAMD51G19A__)
    #define BOARD_TYPE      "SAMD51G19A"
  #elif defined(__SAMD51J19A__)
    #define BOARD_TYPE      "SAMD51J19A"
  #elif defined(__SAMD51J20A__)
    #define BOARD_TYPE      "SAMD51J20A"
  #elif defined(__SAM3X8E__)
    #define BOARD_TYPE      "SAM3X8E"
  #elif defined(__CPU_ARC__)
    #define BOARD_TYPE      "CPU_ARC"
  #elif defined(__SAMD51__)
    #define BOARD_TYPE      "SAMD51"
  #else
    #define BOARD_TYPE      "SAMD Unknown"
  #endif

#elif (NB_USE_SAM_DUE)

  // Must define these pins for SAM DUE
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial1

    #define SerialNB   Serial1
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif
  
  #warning Using SAM DUE
 
  #define BOARD_TYPE      "SAM DUE"

#elif (NB_USE_NRF528XX)

  // Must define these pins for nRF52
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial1

    #define SerialNB   Serial1
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif
  
  #warning Using nRF52

  #if defined(NRF52840_FEATHER)
    #define BOARD_TYPE      "NRF52840_FEATHER"
  #elif defined(NRF52832_FEATHER)
    #define BOARD_TYPE      "NRF52832_FEATHER"
  #elif defined(NRF52840_FEATHER_SENSE)
    #define BOARD_TYPE      "NRF52840_FEATHER_SENSE"
  #elif defined(NRF52840_ITSYBITSY)
    #define BOARD_TYPE      "NRF52840_ITSYBITSY"
  #elif defined(NRF52840_CIRCUITPLAY)
    #define BOARD_TYPE      "NRF52840_CIRCUITPLAY"
  #elif defined(NRF52840_CLUE)
    #define BOARD_TYPE      "NRF52840_CLUE"
  #elif defined(NRF52840_METRO)
    #define BOARD_TYPE      "NRF52840_METRO"
  #elif defined(NRF52840_PCA10056)
    #define BOARD_TYPE      "NRF52840_PCA10056"
  #elif defined(NINA_B302_ublox)
    #define BOARD_TYPE      "NINA_B302_ublox"
  #elif defined(NINA_B112_ublox)
    #define BOARD_TYPE      "NINA_B112_ublox"
  #elif defined(PARTICLE_XENON)
    #define BOARD_TYPE      "PARTICLE_XENON"
  #elif defined(ARDUINO_NRF52_ADAFRUIT)
    #define BOARD_TYPE      "ARDUINO_NRF52_ADAFRUIT"
  #else
    #define BOARD_TYPE      "nRF52 Unknown"
  #endif

#elif ( defined(CORE_TEENSY) )
  // Must define these pins for Teensy
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial1

    #define SerialNB   Serial1
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif
  
  #warning Using Teensy
  //#error Teensy not supported yet
  
  #if defined(__IMXRT1062__)
    // For Teensy 4.1/4.0
    #if defined(ARDUINO_TEENSY41)
      #define BOARD_TYPE      "TEENSY 4.1"
      // Use true for NativeEthernet Library, false if using other Ethernet libraries
      #define USE_NATIVE_ETHERNET     true
    #elif defined(ARDUINO_TEENSY40)
      #define BOARD_TYPE      "TEENSY 4.0"
    #else
      #define BOARD_TYPE      "TEENSY 4.x"
    #endif      
  #elif defined(__MK66FX1M0__)
    #define BOARD_TYPE "Teensy 3.6"
  #elif defined(__MK64FX512__)
    #define BOARD_TYPE "Teensy 3.5"
  #elif defined(__MKL26Z64__)
    #define BOARD_TYPE "Teensy LC"
  #elif defined(__MK20DX256__)
    #define BOARD_TYPE "Teensy 3.2" // and Teensy 3.1 (obsolete)
  #elif defined(__MK20DX128__)
    #define BOARD_TYPE "Teensy 3.0"
  #elif defined(__AVR_AT90USB1286__)
    #error Teensy 2.0++ not supported yet
  #elif defined(__AVR_ATmega32U4__)
    #error Teensy 2.0 not supported yet
  #else
    // For Other Boards
    #define BOARD_TYPE      "Unknown Teensy Board"
  #endif

#elif ( defined(ESP8266) )

  // Must define these pins for ESP8266
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial => can not use Serial for Debug Terminal

    #define SerialNB   Serial
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif

  #warning Using ESP8266
  
  // For ESP8266
  #warning Use ESP8266 architecture
  #define NB_USE_ESP8266
  #define BOARD_TYPE      ARDUINO_BOARD

#elif ( defined(ESP32) )

  // Must define these pins for ESP32
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial2

    #define SerialNB   Serial2
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif

  #warning Using ESP32
  
  // For ESP32
  #warning Use ESP32 architecture
  #define NB_USE_ESP32
  #define BOARD_TYPE      ARDUINO_BOARD
  
#elif (NB_USE_STM32)

  // Must define these pins for STM32
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial1

    #define SerialNB   Serial1
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif
 
  #warning SerialNB using SERIAL_PORT_HARDWARE, can be Serial or Serial1. See your board variant.h
  #define SerialNB     SERIAL_PORT_HARDWARE

  #if defined(STM32F0)
    #warning STM32F0 board selected
    #define BOARD_TYPE  "STM32F0"
  #elif defined(STM32F1)
    #warning STM32F1 board selected
    #define BOARD_TYPE  "STM32F1"
  #elif defined(STM32F2)
    #warning STM32F2 board selected
    #define BOARD_TYPE  "STM32F2"
  #elif defined(STM32F3)
    #warning STM32F3 board selected
    #define BOARD_TYPE  "STM32F3"
  #elif defined(STM32F4)
    #warning STM32F4 board selected
    #define BOARD_TYPE  "STM32F4"
  #elif defined(STM32F7)
  
    #if defined(ARDUINO_NUCLEO_F767ZI)
      #warning Nucleo-144 NUCLEO_F767ZI board selected, using HardwareSerial Serial1 @ pin D0/RX and D1/TX
      #define BOARD_TYPE  "NUCLEO_F767ZI"
      // RX TX
      HardwareSerial Serial1(D0, D1);
    #else
    
      #warning STM32F7 board selected
      #define BOARD_TYPE  "STM32F7"

    #endif
   
  #elif defined(STM32L0)
  
    #if defined(ARDUINO_NUCLEO_L053R8)
      #warning Nucleo-64 NUCLEO_L053R8 board selected, using HardwareSerial Serial1 @ pin D0/RX and D1/TX

    #else
    
      #warning STM32L0 board selected
      #define BOARD_TYPE  "STM32L0"

    #endif
    
  #elif defined(STM32L1)
    #warning STM32L1 board selected
    #define BOARD_TYPE  "STM32L1"
  #elif defined(STM32L4)
    #warning STM32L4 board selected
    #define BOARD_TYPE  "STM32L4"
  #elif defined(STM32H7)
    #warning STM32H7 board selected
    #define BOARD_TYPE  "STM32H7"
  #elif defined(STM32G0)
    #warning STM32G0 board selected
    #define BOARD_TYPE  "STM32G0"
  #elif defined(STM32G4)
    #warning STM32G4 board selected
    #define BOARD_TYPE  "STM32G4"
  #elif defined(STM32WB)
    #warning STM32WB board selected
    #define BOARD_TYPE  "STM32WB"
  #elif defined(STM32MP1)
    #warning STM32MP1 board selected
    #define BOARD_TYPE  "STM32MP1"
  #else
    #warning STM32 unknown board selected
    #define BOARD_TYPE  "STM32 Unknown"
  #endif  

#elif (NB_USE_RP2040)

  // Must define these pins for RP2040
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial1

    #define SerialNB   Serial1
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_DTR) )
    #warning Using default NB_RESETN(10), NB_DTR(11)
    #define NB_RESETN  (10u)
    #define NB_DTR     (11u)

  #else
    #warning Using NB_RESETN and NB_DTR pins from sketch  
  #endif
   
  #if defined(ARDUINO_ARCH_MBED)

    #warning Using RP2040 and ARDUINO_ARCH_MBED
  
    #if ( defined(ARDUINO_NANO_RP2040_CONNECT)    || defined(ARDUINO_RASPBERRY_PI_PICO) || \
          defined(ARDUINO_GENERIC_RP2040) || defined(ARDUINO_ADAFRUIT_FEATHER_RP2040) )
      // Only undef known BOARD_NAME to use better one
      #undef BOARD_NAME
    #endif
    
    #if defined(ARDUINO_RASPBERRY_PI_PICO)
      #define BOARD_NAME      "MBED RASPBERRY_PI_PICO"
    #elif defined(ARDUINO_ADAFRUIT_FEATHER_RP2040)
      #define BOARD_NAME      "MBED ADAFRUIT_FEATHER_RP2040"
    #elif defined(ARDUINO_GENERIC_RP2040)
      #define BOARD_NAME      "MBED GENERIC_RP2040"
    #elif defined(ARDUINO_NANO_RP2040_CONNECT) 
      #define BOARD_NAME      "MBED NANO_RP2040_CONNECT"
    #else
      // Use default BOARD_NAME if exists
      #if !defined(BOARD_NAME)
        #define BOARD_NAME      "MBED Unknown RP2040"
      #endif
    #endif
   
  #else
  
      #warning Using RP2040 and arduino-pico core
        
  #endif  
  
#else

  // Must define these pins for this board
  #if !defined(SerialNB)
    #warning Using default SerialNB = Serial1

    #define SerialNB   Serial1
  #else
    #warning Using SerialNB from sketch  
  #endif
  
  #if !( defined(NB_RESETN) && defined(NB_PWR) )
    #warning Using default NB_RESETN(10), NB_PWR(11)
    #define NB_RESETN  (10u)
    #define NB_PWR     (11u)

  #else
    #warning Using NB_RESETN and NB_PWR pins from sketch  
  #endif

  //#error Using Unsupported Board
 
  #define BOARD_TYPE            "Unknown"
#endif

#ifndef BOARD_NAME
  #define BOARD_NAME    BOARD_TYPE
#endif


#endif    // _NB_BOARDS_GENERIC_H_INCLUDED
