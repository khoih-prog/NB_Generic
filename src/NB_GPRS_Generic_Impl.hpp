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
 
  Version: 1.0.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_GPRS_GENERIC_IMPL_H_INCLUDED
#define _NB_GPRS_GENERIC_IMPL_H_INCLUDED

GPRS::GPRS() :
  _status(NB_IDLE),
  _timeout(0)
{
}

GPRS::~GPRS()
{
}

NB_NetworkStatus_t GPRS::attachGPRS(bool synchronous)
{
  _state = GPRS_STATE_ATTACH;
  _status = NB_CONNECTING;

  if (synchronous) {
    unsigned long start = millis();

    while (ready() == 0) {
      if (_timeout && !((millis() - start) < _timeout)) {
        _state = NB_ERROR;
        break;
      }
      delay(500);
    }
  } else {
    ready();
  }

  return _status;
}

NB_NetworkStatus_t GPRS::detachGPRS(bool synchronous)
{
  _state = GPRS_STATE_DEATTACH;

  if (synchronous) {
    while (ready() == 0) {
      delay(100);
    }
  } else {
    ready();
  }

  return _status;
}

int GPRS::ready()
{
  int ready = MODEM.ready();

  if (ready == 0) {
    return 0;
  }

  MODEM.poll();

  ready = 0;

  switch (_state) {
    case GPRS_STATE_IDLE:
    default: {
      break;
    }

    case GPRS_STATE_ATTACH: {
      MODEM.send("AT+CGATT=1");
      _state = GPRS_STATE_WAIT_ATTACH_RESPONSE;
      ready = 0;
      break;
    }

    case GPRS_STATE_WAIT_ATTACH_RESPONSE: {
      if (ready > 1) {
        _state = GPRS_STATE_IDLE;
        _status = NB_ERROR;
      } else {
        _state = GPRS_STATE_CHECK_ATTACHED;
        ready = 0;
      }
      break;
    }

    case GPRS_STATE_CHECK_ATTACHED: {
      MODEM.setResponseDataStorage(&_response);
      MODEM.send("AT+CGACT?");
      _state = GPRS_STATE_WAIT_CHECK_ATTACHED_RESPONSE;
      ready = 0;
      break;
    }

    case GPRS_STATE_WAIT_CHECK_ATTACHED_RESPONSE: {
      if (ready > 1) {
        _state = GPRS_STATE_IDLE;
        _status = NB_ERROR;
      } else {
        if (_response.endsWith("1,1")) {
          _state = GPRS_STATE_IDLE;
          _status = GPRS_READY;
          ready = 1;
        } else {
          _state = GPRS_STATE_WAIT_ATTACH_RESPONSE;
          ready = 0;
        }
      }
      break;
    }

    case GPRS_STATE_DEATTACH: {
      MODEM.send("AT+CGATT=0");
      _state = GPRS_STATE_WAIT_DEATTACH_RESPONSE;
      ready = 0;
      break;
    }

    case GPRS_STATE_WAIT_DEATTACH_RESPONSE: {
      if (ready > 1) {
        _state = GPRS_STATE_IDLE;
        _status = NB_ERROR;
      } else {
        _state = GPRS_STATE_IDLE;
        _status = NB_IDLE;
        ready = 1;
      }
      break;
    }
  }

  return ready;
}

IPAddress GPRS::getIPAddress()
{
  String response;

  MODEM.send("AT+CGPADDR=1");
  if (MODEM.waitForResponse(100, &response) == 1) {
    if (response.startsWith("+CGPADDR: 1,")) {
      response.remove(0, 12);
      response.remove(response.length());

      IPAddress ip;

      if (ip.fromString(response)) {
        return ip;
      }
    }
  }

  return IPAddress(0, 0, 0, 0);
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