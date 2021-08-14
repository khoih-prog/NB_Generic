/*********************************************************************************************************************************
  NB_Generic.h
  
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

#ifndef _NB_GENERIC_H_INCLUDED
#define _NB_GENERIC_H_INCLUDED

#include <Arduino.h>

#include "NB_Generic_Debug.h"

#include "NB_Type_Generic.h"


class NB
{

  public:
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

    NB_NetworkStatus_t begin(unsigned long baud = 115200, const char* pin = 0, bool restart = true, bool synchronous = true);
    NB_NetworkStatus_t begin(unsigned long baud, const char* pin, const char* apn, bool restart = true, bool synchronous = true);
    NB_NetworkStatus_t begin(unsigned long baud, const char* pin, const char* apn, const char* username,
                             const char* password, bool restart = true, bool synchronous = true);

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

  private:

    NB_Data  _nbData;

    unsigned long _timeout;

    String _response;
};

#include "NB_Generic_Impl.hpp"

#endif    // _NB_GENERIC_H_INCLUDED
