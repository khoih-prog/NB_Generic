/*********************************************************************************************************************************
  NBScanner_Generic.h
  
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
 
  Version: 1.0.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_SCANNER_GENERIC_H_INCLUDED
#define _NB_SCANNER_GENERIC_H_INCLUDED

#include "NB_Generic_Debug.h"

#include "NB_Type_Generic.h"

#include "NB_Generic.h"

class NBScanner 
{

public:
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
};

#include "NBScanner_Generic_Impl.hpp"

#endif    // _NB_SCANNER_GENERIC_H_INCLUDED
