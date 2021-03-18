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
 
  Version: 1.0.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_PIN_GENERIC_IMPL_H_INCLUDED
#define _NB_PIN_GENERIC_IMPL_H_INCLUDED


NBPIN::NBPIN() :
  _pinUsed(false)
{
}

void NBPIN::begin()
{
  MODEM.begin();
}

int NBPIN::isPIN()
{
  String response;

  for (unsigned long start = millis(); (millis() - start) < 1000;) 
  {
    MODEM.send("AT+CPIN?");

    if (MODEM.waitForResponse(10000, &response) == 1) 
    {
      if (response.startsWith("+CPIN: ")) 
      {
        if (response.endsWith("READY")) 
        {
          return 0;
        } 
        else if (response.endsWith("SIM PIN")) 
        {
          return 1;
        } 
        else if (response.endsWith("SIM PUK")) 
        {
          return -1;
        } 
        else 
        {
          return -2;
        }
      }
    }

    delay(100);
  }

  return -2;
}

int NBPIN::checkPIN(String pin)
{
  MODEM.sendf("AT+CPIN=\"%s\"", pin.c_str());
  
  if (MODEM.waitForResponse(10000) == 1) 
  {
    return 0;
  }

  return -1;
}

int NBPIN::checkPUK(String puk, String pin)
{
  MODEM.sendf("AT+CPIN=\"%s\",\"%s\"", puk.c_str(), pin.c_str());
  
  if (MODEM.waitForResponse(10000) == 1) 
  {
    return 0;
  }

  return -1;
}

void NBPIN::changePIN(String old, String pin)
{
  MODEM.sendf("AT+CPWD=\"SC\",\"%s\",\"%s\"", old.c_str(), pin.c_str());
  
  if (MODEM.waitForResponse(10000) == 1) 
  {
    NB_LOGERROR("Pin changed successfully.");  
  } 
  else 
  {
    NB_LOGERROR("ERROR");
  }
}

void NBPIN::switchPIN(String pin)
{
  String response;

  MODEM.send("AT+CLCK=\"SC\",2");
  
  if (MODEM.waitForResponse(180000, &response) != 1) 
  {
    NB_LOGERROR("ERROR");
    return;
  }

  if (response == "+CLCK: 0") 
  {
    MODEM.sendf("AT+CLCK=\"SC\",1,\"%s\"", pin.c_str());
    
    if (MODEM.waitForResponse(180000, &response) == 1) 
    {
      NB_LOGERROR("OK. PIN lock on.");
      _pinUsed = true;
    } 
    else 
    {
      NB_LOGERROR("ERROR");
      _pinUsed = false;
    }
  } 
  else if (response == "+CLCK: 1") 
  {
    MODEM.sendf("AT+CLCK=\"SC\",0,\"%s\"", pin.c_str());
    
    if (MODEM.waitForResponse(180000, &response) == 1) 
    {
      NB_LOGERROR("OK. PIN lock off.");
      _pinUsed = false;
    } 
    else 
    {
      NB_LOGERROR("ERROR");
      _pinUsed = true;
    }
  } 
  else 
  {
    NB_LOGERROR("ERROR");
  }
}

int NBPIN::checkReg()
{
  for (unsigned long start = millis(); (millis() - start) < 10000L;) 
  {
    MODEM.send("AT+CREG?");

    String response = "";

    if (MODEM.waitForResponse(100, &response) == 1) 
    {
      if (response.startsWith("+CREG: ")) 
      {
        if (response.endsWith(",1")) 
        {
          return 0;
        } 
        else if (response.endsWith(",5")) 
        {
          return 1;
        }
      }
    }

    delay(100);
  }

  return -1;
}

bool NBPIN::getPINUsed()
{
  return _pinUsed;
}

void NBPIN::setPINUsed(bool used)
{
  _pinUsed = used;
}

#endif    // _NB_PIN_GENERIC_IMPL_H_INCLUDED
