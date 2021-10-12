/*********************************************************************************************************************************
  NBClient_Generic_Impl.hpp
  
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
 
  Version: 1.3.1
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
  1.1.0    K Hoang     19/03/2021 Rewrite to prepare for supporting more GSM/GPRS modules. Add FileUtils examples.
  1.2.0    K Hoang     28/06/2021 Add support to RP2040-based boards using Arduino mbed or Arduino-pico core
  1.3.0    K Hoang     14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
  1.3.1    K Hoang     10/10/2021 Update `Packages' Patches`
 **********************************************************************************************************************************/

#pragma once

#ifndef _NB_CLIENT_GENERIC_IMPL_H_INCLUDED
#define _NB_CLIENT_GENERIC_IMPL_H_INCLUDED

#include "utility/NBSocketBuffer_Generic.h"


NBClient::NBClient(bool synch) :
  NBClient(-1, synch)
{
}

NBClient::NBClient(int socket, bool synch)
{
  _synch         = synch;
  _socket        = socket;
  _connected      = false;
  _state          = CLIENT_STATE_IDLE;
  _ip             = (uint32_t) 0;
  _host           = NULL;
  _port           = 0;
  _ssl            = false;
  _writeSync      = true;

  MODEM.addUrcHandler(this);
}

NBClient::~NBClient()
{
  MODEM.removeUrcHandler(this);
}

int NBClient::connect(IPAddress ip, uint16_t port)
{
  _ip   = ip;
  _host = NULL;
  _port = port;
  _ssl  = false;

  return connect();
}

int NBClient::connectSSL(IPAddress ip, uint16_t port)
{
  _ip   = ip;
  _host = NULL;
  _port = port;
  _ssl  = true;

  return connect();
}

int NBClient::connect(const char *host, uint16_t port)
{
  _ip   = (uint32_t)0;
  _host = host;
  _port = port;
  _ssl  = false;

  return connect();
}

int NBClient::connectSSL(const char *host, uint16_t port)
{
  _ip   = (uint32_t)0;
  _host = host;
  _port = port;
  _ssl  = true;

  return connect();
}

int NBClient::connect()
{
  if (_socket != -1)
  {
    stop();
  }

  if (_synch)
  {
    while (ready() == NB_RESPONSE_IDLE);
  }
  else if (ready() == NB_RESPONSE_IDLE)
  {
    return 0;
  }

  _state = CLIENT_STATE_CREATE_SOCKET;

  if (_synch)
  {
    while (ready() == NB_RESPONSE_IDLE)
    {
      delay(100);
    }

    if (_socket == -1)
    {
      return 0;
    }
  }

  return 1;
}

void NBClient::beginWrite(bool sync)
{
  _writeSync = sync;
}

size_t NBClient::write(uint8_t c)
{
  return write(&c, 1);
}

size_t NBClient::write(const uint8_t *buf)
{
  return write(buf, strlen((const char*)buf));
}

size_t NBClient::write(const uint8_t* buf, size_t size)
{
  return NBClient_ModemUrcHandler::write(buf, size);
}

void NBClient::endWrite(bool /*sync*/)
{
  _writeSync = true;
}

uint8_t NBClient::connected()
{
  MODEM.poll();

  if (_socket == -1)
  {
    return 0;
  }

  // call available to update socket state
  if (NBSocketBuffer.available(_socket) < 0 || (_ssl && !_connected))
  {
    NB_LOGDEBUG(F("NBClient::connected: no socket"));

    stop();

    return 0;
  }

  NB_LOGDEBUG(F("NBClient::connected: OK"));

  return 1;
}

NBClient::operator bool()
{
  return (_socket != -1);
}

int NBClient::read(uint8_t *buf, size_t size)
{
  if (_socket == -1)
  {
    return 0;
  }

  if (size == 0)
  {
    return 0;
  }

  int avail = available();

  if (avail == 0)
  {
    return 0;
  }

  return NBSocketBuffer.read(_socket, buf, size);
}

int NBClient::read()
{
  byte b;

  if (read(&b, 1) == 1)
  {
    return b;
  }

  return -1;
}

int NBClient::available()
{
  if (_synch)
  {
    while (ready() == NB_RESPONSE_IDLE);
  }
  else if (ready() == NB_RESPONSE_IDLE)
  {
    return 0;
  }

  if (_socket == -1)
  {
    return 0;
  }

  int avail = NBSocketBuffer.available(_socket);

  if (avail < 0)
  {
    stop();

    return 0;
  }

  return avail;
}

int NBClient::peek()
{
  if (available() > 0)
  {
    return NBSocketBuffer.peek(_socket);
  }

  return -1;
}

void NBClient::flush()
{
}

void NBClient::stop()
{
  NBClient_ModemUrcHandler::stop();
}

#endif    // _NB_CLIENT_GENERIC_IMPL_H_INCLUDED
