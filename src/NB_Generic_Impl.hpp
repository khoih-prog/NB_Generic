/*********************************************************************************************************************************
  NB_Generic_Impl.hpp
  
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

#ifndef _NB_GENERIC_IMPL_H_INCLUDED
#define _NB_GENERIC_IMPL_H_INCLUDED

#define _XOPEN_SOURCE

#include <time.h>

extern "C"
{
  char *strptime(const char *__restrict, const char *__restrict, struct tm *__restrict);
}

NB::NB(bool debug)
{
  _nbData._state        = NB_ERROR;
  _nbData._readyState   = 0;
  _nbData._pin          = NULL;
  _nbData._apn          = "";
  _nbData._username     = "";
  _nbData._password     = "";
  _nbData._timeout      = 0;

  if (debug)
  {
    MODEM.debug();
  }
}

/////////////////////////////////////////////////////////////

NB_NetworkStatus_t NB::begin(const char* pin, bool restart, bool synchronous)
{
  return begin(pin, "", restart, synchronous);
}

/////////////////////////////////////////////////////////////

NB_NetworkStatus_t NB::begin(const char* pin, const char* apn, bool restart, bool synchronous)
{
  return begin(pin, apn, "", "", restart, synchronous);
}

/////////////////////////////////////////////////////////////

NB_NetworkStatus_t NB::begin(const char* pin, const char* apn, const char* username, const char* password, bool restart, bool synchronous)
{
  if (!MODEM.begin(restart))
  {
    _nbData._state = NB_ERROR;
  }
  else
  {
    _nbData._pin = pin;
    _nbData._apn = apn;
    _nbData._username = username,
    _nbData._password = password;
    _nbData._state = NB_IDLE;
    _nbData._readyState = READY_STATE_SET_ERROR_DISABLED;

    if (synchronous)
    {
      unsigned long start = millis();

      while (ready() == 0)
      {
        if (_nbData._timeout && !((millis() - start) < _nbData._timeout))
        {
          _nbData._state = NB_ERROR;
          
          break;
        }

        delay(100);
      }
    }
    else
    {
      return (NB_NetworkStatus_t) 0;
    }
  }

  return _nbData._state;
}

/////////////////////////////////////////////////////////////

NB_NetworkStatus_t NB::begin(unsigned long baud, const char* pin, bool restart, bool synchronous)
{
  return begin(baud, pin, "", restart, synchronous);
}

/////////////////////////////////////////////////////////////


NB_NetworkStatus_t NB::begin(unsigned long baud, const char* pin, const char* apn, bool restart, bool synchronous)
{
  return begin(baud, pin, apn, "", "", restart, synchronous);
}

/////////////////////////////////////////////////////////////


NB_NetworkStatus_t NB::begin(unsigned long baud, const char* pin, const char* apn, const char* username,
                             const char* password, bool restart, bool synchronous)
{
  if (!MODEM.begin(baud, restart))
  {
    _nbData._state = NB_ERROR;
  }
  else
  {
    _nbData._pin = pin;
    _nbData._apn = apn;
    _nbData._username = username,
    _nbData._password = password;
    _nbData._state = NB_IDLE;
    _nbData._readyState = READY_STATE_SET_ERROR_DISABLED;

    if (synchronous)
    {
      unsigned long start = millis();

      while (ready() == 0)
      {
        if (_nbData._timeout && !((millis() - start) < _nbData._timeout))
        {
          _nbData._state = NB_ERROR;
          
          break;
        }

        delay(100);
      }
    }
    else
    {
      return (NB_NetworkStatus_t) 0;
    }
  }

  return _nbData._state;
}

/////////////////////////////////////////////////////////////

int NB::isAccessAlive()
{
  return MODEM.isAccessAlive();
}

/////////////////////////////////////////////////////////////

bool NB::shutdown()
{
  // Attempt AT command shutdown
  if (_nbData._state == NB_READY && MODEM.shutdown())
  {
    _nbData._state = NB_OFF;
    return true;
  }

  return false;
}

/////////////////////////////////////////////////////////////

bool NB::secureShutdown()
{
  // Hardware power off
  MODEM.end();
  _nbData._state = NB_OFF;
  
  return true;
}

/////////////////////////////////////////////////////////////

int NB::ready()
{
  return MODEM.NB_ready(_nbData, _response);
}

/////////////////////////////////////////////////////////////


void NB::setTimeout(unsigned long timeout)
{
  _nbData._timeout = timeout;
}

/////////////////////////////////////////////////////////////

unsigned long NB::getTime()
{
  return MODEM.getTime();
}

/////////////////////////////////////////////////////////////

unsigned long NB::getLocalTime()
{
  return MODEM.getLocalTime();
}

/////////////////////////////////////////////////////////////

bool NB::setTime(unsigned long const epoch, int const timezone)
{
  return MODEM.setTime(epoch, timezone);
}

/////////////////////////////////////////////////////////////

NB_NetworkStatus_t NB::status()
{
  return _nbData._state;
}

#endif    // _NB_GENERIC_IMPL_H_INCLUDED
