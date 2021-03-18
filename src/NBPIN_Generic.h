/*********************************************************************************************************************************
  NBPIN_Generic.h
  
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
 
  Version: 1.0.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_PIN_GENERIC_H_INCLUDED
#define _NB_PIN_GENERIC_H_INCLUDED

#include <Arduino.h>

#include "NB_Generic_Debug.h"

#include "NB_Type_Generic.h"

class NBPIN 
{

public:

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

  /** Check if modem was registered in NB/GPRS network
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

private:
  bool _pinUsed;
};

#include "NBPIN_Generic_Impl.hpp"

#endif    // _NB_PIN_GENERIC_H_INCLUDED
