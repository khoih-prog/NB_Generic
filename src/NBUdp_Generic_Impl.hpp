/*********************************************************************************************************************************
  NBUdp_Generic_Impl.hpp
  
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

#ifndef _NB_UDP_GENERIC_IMPL_H_INCLUDED
#define _NB_UDP_GENERIC_IMPL_H_INCLUDED

NBUDP::NBUDP()
{
  _socket           = -1;
  _packetReceived   = false;
  _txIp             = (uint32_t) 0;
  _txHost           = NULL;
  _txPort           = 0;
  _txSize           = 0;
  _rxIp             = (uint32_t) 0;
  _rxPort           = 0;
  _rxSize           = 0;
  _rxIndex          = 0;

  MODEM.addUrcHandler(this);
}

NBUDP::~NBUDP()
{
  MODEM.removeUrcHandler(this);
}

uint8_t NBUDP::begin(uint16_t port)
{
  return NBUDP_ModemUrcHandler::begin(port);
}

void NBUDP::stop()
{
  NBUDP_ModemUrcHandler::stop();
}

int NBUDP::beginPacket(IPAddress ip, uint16_t port)
{
  if (_socket < 0)
  {
    return 0;
  }

  _txIp = ip;
  _txHost = NULL;
  _txPort = port;
  _txSize = 0;

  return 1;
}

int NBUDP::beginPacket(const char *host, uint16_t port)
{
  if (_socket < 0)
  {
    return 0;
  }

  _txIp = (uint32_t)0;
  _txHost = host;
  _txPort = port;
  _txSize = 0;

  return 1;
}

int NBUDP::endPacket()
{
  return NBUDP_ModemUrcHandler::endPacket();
}

size_t NBUDP::write(uint8_t b)
{
  return write(&b, sizeof(b));
}

size_t NBUDP::write(const uint8_t *buffer, size_t size)
{
  if (_socket < 0)
  {
    return 0;
  }

  size_t spaceAvailable = sizeof(_txBuffer) - _txSize;

  if (size > spaceAvailable)
  {
    size = spaceAvailable;
  }

  memcpy(&_txBuffer[_txSize], buffer, size);
  _txSize += size;

  return size;
}


int NBUDP::parsePacket()
{
  return NBUDP_ModemUrcHandler::parsePacket();
}

int NBUDP::available()
{
  if (_socket < 0)
  {
    return 0;
  }

  return (_rxSize - _rxIndex);
}

int NBUDP::read()
{
  byte b;

  if (read(&b, sizeof(b)) == 1)
  {
    return b;
  }

  return -1;
}

int NBUDP::read(unsigned char* buffer, size_t len)
{
  size_t readMax = available();

  if (len > readMax)
  {
    len = readMax;
  }

  memcpy(buffer, &_rxBuffer[_rxIndex], len);

  _rxIndex += len;

  return len;
}

int NBUDP::peek()
{
  if (available() > 1)
  {
    return _rxBuffer[_rxIndex];
  }

  return -1;
}

void NBUDP::flush()
{
}

IPAddress NBUDP::remoteIP()
{
  return _rxIp;
}

uint16_t NBUDP::remotePort()
{
  return _rxPort;
}

#endif    // _NB_UDP_GENERIC_IMPL_H_INCLUDED
