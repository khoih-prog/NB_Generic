/*********************************************************************************************************************************
  NBModem_Generic_Impl.hpp
  
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

#ifndef _NB_MODEM_GENERIC_IMPL_H_INCLUDED
#define _NB_MODEM_GENERIC_IMPL_H_INCLUDED

NBModem::NBModem()
{
}

int NBModem::begin()
{
  if (!MODEM.begin()) 
  {
    return 0;
  }

  return 1;
}

String NBModem::getIMEI()
{
  String imei;

  imei.reserve(15);

  MODEM.send("AT+CGSN");
  MODEM.waitForResponse(100, &imei);

  return imei;
}

String NBModem::getICCID()
{
  String iccid;

  iccid.reserve(7 + 20);

  MODEM.send("AT+CCID");
  MODEM.waitForResponse(1000, &iccid);

  if (iccid.startsWith("+CCID: ")) 
  {
    iccid.remove(0, 7);
  } 
  else 
  {
    iccid = "";
  }

  return iccid;
}

#endif    // _NB_MODEM_GENERIC_IMPL_H_INCLUDED
