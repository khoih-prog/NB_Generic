/*********************************************************************************************************************************
  NB_Generic_Debug.h
  
  For ESP8266, ESP32, SAMD21/SAMD51, nRF52, SAM DUE, Teensy and STM32 with NB-IoT modules such as u-blox SARA R401
  
  NB_Generic is a library for the ESP8266, ESP32, SAMD21/SAMD51, nRF52, SAM DUE, Teensy and STM32 with GSM modules
  
  Based on and modified from MKRNB Library (https://github.com/arduino-libraries/MKRNB)
  
  Built by Khoi Hoang https://github.com/khoih-prog/NB_Generic
  Licensed under GNU Lesser General Public License
  
  Copyright (C) 2017  Arduino AG (http://www.arduino.cc/)
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

#ifndef _NB_GENERIC_DEBUG_H_INCLUDED
#define _NB_GENERIC_DEBUG_H_INCLUDED

#ifdef DEBUG_NB_GENERIC_PORT
  #define DBG_PORT_NB       DEBUG_NB_GENERIC_PORT
#else
  #define DBG_PORT_NB       Serial
#endif

// Change _NB_GENERIC_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _NB_GENERIC_LOGLEVEL_
  #define _NB_GENERIC_LOGLEVEL_       0
#endif

const char NB_MARK[] = "[NB] ";

#define NB_PRINT_MARK   DBG_PORT_NB.print(NB_MARK)

#define NB_PRINT        DBG_PORT_NB.print
#define NB_PRINTLN      DBG_PORT_NB.println


///////////////////////////////////////////

#define NB_LOGERROR0(x)              if(_NB_GENERIC_LOGLEVEL_>0) { NB_PRINT(x); }
#define NB_LOGERROR(x)               if(_NB_GENERIC_LOGLEVEL_>0) { NB_PRINT_MARK; NB_PRINTLN(x); }
#define NB_LOGERROR1(x,y)            if(_NB_GENERIC_LOGLEVEL_>0) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINTLN(y); }
#define NB_LOGERROR2(x,y,z)          if(_NB_GENERIC_LOGLEVEL_>0) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINTLN(z); }
#define NB_LOGERROR3(x,y,z,w)        if(_NB_GENERIC_LOGLEVEL_>0) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINTLN(w); }
#define NB_LOGERROR5(x,y,z,w,xx,yy)  if(_NB_GENERIC_LOGLEVEL_>0) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINT(w); NB_PRINT(xx); NB_PRINTLN(yy); }

///////////////////////////////////////////

#define NB_LOGWARN0(x)             if(_NB_GENERIC_LOGLEVEL_>1) { NB_PRINT(x); }
#define NB_LOGWARN(x)              if(_NB_GENERIC_LOGLEVEL_>1) { NB_PRINT_MARK; NB_PRINTLN(x); }
#define NB_LOGWARN1(x,y)           if(_NB_GENERIC_LOGLEVEL_>1) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINTLN(y); }
#define NB_LOGWARN2(x,y,z)         if(_NB_GENERIC_LOGLEVEL_>1) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINTLN(z); }
#define NB_LOGWARN3(x,y,z,w)       if(_NB_GENERIC_LOGLEVEL_>1) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINTLN(w); }
#define NB_LOGWARN5(x,y,z,w,xx,yy) if(_NB_GENERIC_LOGLEVEL_>1) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINT(w); NB_PRINT(xx); NB_PRINTLN(yy); }

///////////////////////////////////////////

#define NB_LOGINFO0(x)             if(_NB_GENERIC_LOGLEVEL_>2) { NB_PRINT(x); }
#define NB_LOGINFO(x)              if(_NB_GENERIC_LOGLEVEL_>2) { NB_PRINT_MARK; NB_PRINTLN(x); }
#define NB_LOGINFO1(x,y)           if(_NB_GENERIC_LOGLEVEL_>2) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINTLN(y); }
#define NB_LOGINFO2(x,y,z)         if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINTLN(z); }
#define NB_LOGINFO3(x,y,z,w)       if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINTLN(w); }
#define NB_LOGINFO5(x,y,z,w,xx,yy) if(_NB_GENERIC_LOGLEVEL_>2) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINT(w); NB_PRINT(xx); NB_PRINTLN(yy); }

///////////////////////////////////////////

#define NB_LOGDEBUG0(x)              if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT(x); }
#define NB_LOGDEBUG(x)               if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINTLN(x); }
#define NB_LOGDEBUG1(x,y)            if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINTLN(y); }
#define NB_LOGDEBUG2(x,y,z)          if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINTLN(z); }
#define NB_LOGDEBUG3(x,y,z,w)        if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINTLN(w); }
#define NB_LOGDEBUG5(x,y,z,w,xx,yy)  if(_NB_GENERIC_LOGLEVEL_>3) { NB_PRINT_MARK; NB_PRINT(x); NB_PRINT(y); NB_PRINT(z); NB_PRINT(w); NB_PRINT(xx); NB_PRINTLN(yy); }

///////////////////////////////////////////

#endif    // _NB_GENERIC_DEBUG_H_INCLUDED
