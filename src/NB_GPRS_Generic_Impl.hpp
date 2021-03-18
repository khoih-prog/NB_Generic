/*********************************************************************************************************************************
  NB_GPRS_Generic_Impl.hpp
  
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
 
  Version: 1.1.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
  1.1.0    K Hoang     19/03/2021 Rewrite to prepare for supporting more GSM/GPRS modules. Add FileUtils examples.
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_GPRS_GENERIC_IMPL_H_INCLUDED
#define _NB_GPRS_GENERIC_IMPL_H_INCLUDED

GPRS::GPRS()
{
  _status   = NB_IDLE;
  _timeout  = 0;

  // Empty now, just for future implementation
  MODEM.addUrcHandler(this);
}

GPRS::~GPRS()
{
  // Empty now, just for future implementation
  MODEM.removeUrcHandler(this);
}

NB_NetworkStatus_t GPRS::attachGPRS(bool synchronous)
{
  _state = GPRS_STATE_ATTACH;
  _status = NB_CONNECTING;

  if (synchronous)
  {
    unsigned long start = millis();

    while (ready() == NB_RESPONSE_IDLE)
    {
      if (_timeout && !((millis() - start) < _timeout))
      {
        _state = NB_ERROR;

        break;
      }

      delay(500);
    }
  }
  else
  {
    ready();
  }

  return _status;
}

NB_NetworkStatus_t GPRS::detachGPRS(bool synchronous)
{
  _state = GPRS_STATE_DEATTACH;

  if (synchronous)
  {
    while (ready() == NB_RESPONSE_IDLE)
    {
      delay(100);
    }
  }
  else
  {
    ready();
  }

  return _status;
}

void GPRS::setTimeout(unsigned long timeout)
{
  _timeout = timeout;
}

NB_NetworkStatus_t GPRS::status()
{
  MODEM.poll();

  return _status;
}

#endif    // _NB_GPRS_GENERIC_IMPL_H_INCLUDED
