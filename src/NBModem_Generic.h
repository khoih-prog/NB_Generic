/*********************************************************************************************************************************
  NBModem_Generic.h
  
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

#ifndef _NB_MODEM_GENERIC_H_INCLUDED
#define _NB_MODEM_GENERIC_H_INCLUDED

#include <Arduino.h>

#include "NB_Generic_Debug.h"

#include "NB_Type_Generic.h"

class NBModem
{

  public:

    /** Constructor */
    NBModem();

    /** Check modem response and restart it
    */
    int begin(unsigned long baud = 115200);

    /** Obtain modem IMEI (command AT)
        @return modem IMEI number
    */
    String getIMEI();

    /** Obtain SIM card ICCID (command AT)
        @return SIM ICCID number
    */
    String getICCID();
};

#include "NBModem_Generic_Impl.hpp"

#endif    // _NB_MODEM_GENERIC_H_INCLUDED
