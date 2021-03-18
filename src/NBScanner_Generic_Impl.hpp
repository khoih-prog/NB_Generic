/*********************************************************************************************************************************
  NBScanner_Generic_Impl.hpp
  
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

#ifndef _NB_SCANNER_GENERIC_IMPL_H_INCLUDED
#define _NB_SCANNER_GENERIC_IMPL_H_INCLUDED

NBScanner::NBScanner(bool trace)
{
  if (trace) 
  {
    MODEM.debug();
  }
}

NB_NetworkStatus_t NBScanner::begin()
{
  MODEM.begin();

  return NB_IDLE;
}

String NBScanner::getCurrentCarrier()
{
  String response;

  MODEM.send("AT+COPS?");
  
  if (MODEM.waitForResponse(180000, &response) == 1) 
  {
    int firstQuoteIndex = response.indexOf('"');
    int lastQuoteIndex = response.lastIndexOf('"');

    if (firstQuoteIndex != -1 && lastQuoteIndex != -1 && firstQuoteIndex != lastQuoteIndex) 
    {
      return response.substring(firstQuoteIndex + 1, lastQuoteIndex);
    }
  }

  return "";
}

String NBScanner::getSignalStrength()
{
  String response;

  MODEM.send("AT+CSQ");
  
  if (MODEM.waitForResponse(100, &response) == 1) 
  {
    int firstSpaceIndex = response.indexOf(' ');
    int lastCommaIndex = response.lastIndexOf(',');

    if (firstSpaceIndex != -1 && lastCommaIndex != -1) 
    {
      return response.substring(firstSpaceIndex + 1, lastCommaIndex);
    }
  }

  return "";
}

String NBScanner::readNetworks()
{
  String response;

  MODEM.send("AT+COPS=?");
  
  if (MODEM.waitForResponse(180000, &response) == 1) 
  {
    String result;
    unsigned int responseLength = response.length();

    for(unsigned int i = 0; i < responseLength; i++) 
    {
      for (; i < responseLength; i++) 
      {
        if (response[i] == '"') 
        {
          result += "> ";
          break;
        }
      }

      for (i++; i < responseLength; i++) 
      {
        if (response[i] == '"') 
        {
          result += '\n';
          break;
        }

        result += response[i];
      }

      for (i++; i < responseLength; i++) 
      {
        if (response[i] == ')') 
        {
          break;
        }
      }
    }

    return result;
  }

  return "";
}

#endif    // _NB_SCANNER_GENERIC_IMPL_H_INCLUDED
