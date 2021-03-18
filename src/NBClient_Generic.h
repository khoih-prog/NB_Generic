/*********************************************************************************************************************************
  NBClient_Generic.h
  
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

#ifndef _NB_CLIENT_GENERIC_H_INCLUDED
#define _NB_CLIENT_GENERIC_H_INCLUDED

#include "NB_Generic_Debug.h"

#include "NB_Type_Generic.h"

#include "NBModem_Generic.h"

#include <Client.h>

class NBClient : public Client, public NBClient_ModemUrcHandler
{

  public:

    /** Constructor
        @param synch    Sync mode
    */
    NBClient(bool synch = true);

    /** Constructor
        @param socket   Socket
        @param synch    Sync mode
    */
    NBClient(int socket, bool synch);

    virtual ~NBClient();


    /** Connect to server by IP address
        @param (IPAddress)
        @param (uint16_t)
        @return returns 0 if last command is still executing, 1 success, 2 if there are no resources
    */
    int connect(IPAddress, uint16_t);
    int connectSSL(IPAddress, uint16_t);

    /** Connect to server by hostname
        @param host     Hostname
        @param port     Port
        @return returns 0 if last command is still executing, 1 success, 2 if there are no resources
    */
    int connect(const char *host, uint16_t port);
    int connectSSL(const char *host, uint16_t port);

    /** Initialize write in request
        @param sync     Sync mode
    */
    void beginWrite(bool sync = false);

    /** Write a character in request
        @param c      Character
        @return size
    */
    size_t write(uint8_t c);

    /** Write a characters buffer in request
        @param buf      Buffer
        @return buffer size
    */
    size_t write(const uint8_t *buf);

    /** Write a characters buffer with size in request
        @param (uint8_t*) Buffer
        @param (size_t)   Buffer size
        @return buffer size
    */
    size_t write(const uint8_t*, size_t);

    /** Finish write request
        @param sync     Sync mode
    */
    void endWrite(bool sync = false);

    /** Check if connected to server
        @return 1 if connected
    */
    uint8_t connected();

    operator bool();

    /** Read from response buffer and copy size specified to buffer
        @param buf      Buffer
        @param size     Buffer size
        @return bytes read
    */
    int read(uint8_t *buf, size_t size);

    /** Read a character from response buffer
        @return character
    */
    int read();

    /** Check if exists a response available
        @return 1 if exists, 0 if not exists
    */
    int available();

    /** Read a character from response buffer but does not move the pointer.
        @return character
    */
    int peek();

    /** Flush response buffer
    */
    void flush();


    /** Stop client
    */
    void stop();


  private:

    int connect();

    bool _synch;
};

#include "NBClient_Generic_Impl.hpp"

#endif    // _NB_CLIENT_GENERIC_H_INCLUDED
