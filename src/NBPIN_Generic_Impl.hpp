/*********************************************************************************************************************************
  NBPIN_Generic_Impl.hpp
  
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
 
  Version: 1.2.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
  1.1.0    K Hoang     19/03/2021 Rewrite to prepare for supporting more GSM/GPRS modules. Add FileUtils examples.
  1.2.0    K Hoang     28/06/2021 Add support to RP2040-based boards using Arduino mbed or Arduino-pico core
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_PIN_GENERIC_IMPL_H_INCLUDED
#define _NB_PIN_GENERIC_IMPL_H_INCLUDED


NBPIN::NBPIN() :
  _pinUsed(false)
{
}

void NBPIN::begin(unsigned long baud)
{
  MODEM.begin(baud);
}

int NBPIN::isPIN()
{
  return MODEM.isPIN();
}

int NBPIN::checkPIN(String pin)
{
  return MODEM.checkPIN(pin);
}

int NBPIN::checkPUK(String puk, String pin)
{
  return MODEM.checkPUK(puk, pin);
}

void NBPIN::changePIN(String oldPIN, String newPIN)
{
  MODEM.changePIN(oldPIN, newPIN);
}

void NBPIN::switchPIN(String pin)
{
  MODEM.switchPIN(pin, _pinUsed);
}

int NBPIN::checkReg()
{
  return MODEM.checkReg();
}

bool NBPIN::getPINUsed()
{
  NB_LOGDEBUG1(F("GSMPIN::getPINUsed: _pinUsed = "), _pinUsed);

  return _pinUsed;
}

void NBPIN::setPINUsed(bool used)
{
  _pinUsed = used;

  NB_LOGDEBUG1(F("GSMPIN::getPINUsed: _pinUsed = "), _pinUsed);
}

#endif    // _NB_PIN_GENERIC_IMPL_H_INCLUDED
