/*********************************************************************************************************************************
  defines.h
  
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
 **********************************************************************************************************************************/

#ifndef defines_h
#define defines_h

#define DEBUG_NB_GENERIC_PORT       Serial

// Debug Level from 0 to 5. Level 5 is to print out AT commands and responses
#define _NB_GENERIC_LOGLEVEL_       5

#define SECRET_PINNUMBER     ""

//////////////////////////////////////////////

#if defined(ARDUINO_SAMD_MKRNB1500)

  #define USING_CUSTOMIZED_MKRNB1500      false
  
  #if USING_CUSTOMIZED_MKRNB1500
  
    #define SerialNB      Serial1
    #define NB_RESETN     (3u)
    #define NB_PWR        (2u)
    
  #else
  
    #define SerialNB      SerialSARA
    #define NB_RESETN     SARA_RESETN
    #define NB_PWR        SARA_PWR_ON
    
  #endif
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

// Optional usage of NB_RESETN and NB_DTR. Need to be here only when true. Default is false

//#define UBLOX_USING_RESET_PIN             true
//#define UBLOX_USING_POWER_ON_PIN          true

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
