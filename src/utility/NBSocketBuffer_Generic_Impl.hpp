/*********************************************************************************************************************************
  NBSocketBuffer_Generic_Impl.hpp
  
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

#ifndef _NBSOCKET_BUFFER_IMPL_H_INCLUDED
#define _NBSOCKET_BUFFER_IMPL_H_INCLUDED

#include <stdlib.h>
#include <string.h>

#define NB_SOCKET_NUM_BUFFERS     (sizeof(_buffers) / sizeof(_buffers[0]))

NBSocketBufferClass::NBSocketBufferClass()
{
  memset(&_buffers, 0x00, sizeof(_buffers));
}

NBSocketBufferClass::~NBSocketBufferClass()
{
  for (unsigned int i = 0; i < NB_SOCKET_NUM_BUFFERS; i++) 
  {
    close(i);
  }
}

void NBSocketBufferClass::close(int socket)
{
  if (_buffers[socket].data) 
  {
    free(_buffers[socket].data);
    _buffers[socket].data = _buffers[socket].head = NULL;
    _buffers[socket].length = 0;
  }
}

int NBSocketBufferClass::available(int socket)
{
  return MODEM.availableSocketBuffer(socket, _buffers[socket]);
}

int NBSocketBufferClass::peek(int socket)
{
  if (!available(socket)) 
  {
    return -1;
  }

  return *_buffers[socket].head;
}

int NBSocketBufferClass::read(int socket, uint8_t* data, size_t length)
{
  uint16_t avail = available(socket);

  if (!avail) 
  {
    return 0;
  }

  if (avail < (uint16_t)length) 
  {
    length = avail;
  }

  memcpy(data, _buffers[socket].head, (uint16_t) length);
  _buffers[socket].head += length;
  _buffers[socket].length -= length;

  return length;
}

NBSocketBufferClass NBSocketBuffer;

#endif    // _NBSOCKET_BUFFER_IMPL_H_INCLUDED
