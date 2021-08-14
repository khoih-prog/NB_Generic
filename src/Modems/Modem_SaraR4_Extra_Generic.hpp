/*********************************************************************************************************************************
  Modem_SaraR4_Extra_Generic.hpp
  
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
 
  Version: 1.3.0
  
  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0    K Hoang     18/03/2021 Initial public release to add support to many boards / modules besides MKRNB 1500 / SARA R4
  1.0.1    K Hoang     18/03/2021 Add Advanced examples (MQTT, Blynk)
  1.1.0    K Hoang     19/03/2021 Rewrite to prepare for supporting more GSM/GPRS modules. Add FileUtils examples.
  1.2.0    K Hoang     28/06/2021 Add support to RP2040-based boards using Arduino mbed or Arduino-pico core
  1.3.0    K Hoang     14/08/2021 Add support to Adafruit nRF52 core v0.22.0+
 **********************************************************************************************************************************/

#pragma once

#ifndef _MODEM_SARA_R4_EXTRA_INCLUDED_H
#define _MODEM_SARA_R4_EXTRA_INCLUDED_H

#include "utility/NBSocketBuffer_Generic.h"

class GPRS_ModemUrcHandler : public ModemUrcHandler
{
  public:

    int ready()
    {
      int ready = MODEM.ready();

      if (ready == NB_RESPONSE_IDLE)
      {
        return NB_RESPONSE_IDLE;
      }

      MODEM.poll();

      ready = NB_RESPONSE_IDLE;

      switch (_state)
      {
        case GPRS_STATE_IDLE:
        default:
          {
            break;
          }

        case GPRS_STATE_ATTACH:
          {
            MODEM.send("AT+CGATT=1");
            _state  = GPRS_STATE_WAIT_ATTACH_RESPONSE;
            ready   = NB_RESPONSE_IDLE;
            break;
          }

        case GPRS_STATE_WAIT_ATTACH_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK)
            {
              _state  = GPRS_STATE_IDLE;
              _status = NB_ERROR;
            }
            else
            {
              _state  = GPRS_STATE_CHECK_ATTACHED;
              ready   = NB_RESPONSE_IDLE;
            }

            break;
          }

        case GPRS_STATE_CHECK_ATTACHED:
          {
            MODEM.setResponseDataStorage(&_response);
            MODEM.send("AT+CGACT?");
            _state  = GPRS_STATE_WAIT_CHECK_ATTACHED_RESPONSE;
            ready   = NB_RESPONSE_IDLE;
            break;
          }

        case GPRS_STATE_WAIT_CHECK_ATTACHED_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK)
            {
              _state  = GPRS_STATE_IDLE;
              _status = NB_ERROR;
            }
            else
            {
              if (_response.endsWith("1,1"))
              {
                _state  = GPRS_STATE_IDLE;
                _status = GPRS_READY;
                ready   = NB_RESPONSE_OK;
              }
              else
              {
                _state  = GPRS_STATE_WAIT_ATTACH_RESPONSE;
                ready   = NB_RESPONSE_IDLE;
              }
            }

            break;
          }

        case GPRS_STATE_DEATTACH:
          {
            MODEM.send("AT+CGATT=0");
            _state  = GPRS_STATE_WAIT_DEATTACH_RESPONSE;
            ready   = NB_RESPONSE_IDLE;

            break;
          }

        case GPRS_STATE_WAIT_DEATTACH_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK)
            {
              _state  = GPRS_STATE_IDLE;
              _status = NB_ERROR;
            }
            else
            {
              _state  = GPRS_STATE_IDLE;
              _status = NB_IDLE;
              ready   = NB_RESPONSE_OK;
            }

            break;
          }
      }

      return ready;
    }

    //////////////////////////////////////////////////////

    IPAddress getIPAddress()
    {
      String response;

      MODEM.send("AT+CGPADDR=1");

      if (MODEM.waitForResponse(100, &response) == NB_RESPONSE_OK)
      {
        if (response.startsWith("+CGPADDR: 1,"))
        {
          response.remove(0, 12);
          response.remove(response.length());

          IPAddress ip;

          if (ip.fromString(response))
          {
            return ip;
          }
        }
      }

      return IPAddress(0, 0, 0, 0);
    }

    //////////////////////////////////////////////////////

    int hostByName(const char* hostname, IPAddress& result)
    {
      String response;

      MODEM.sendf("AT+UDNSRN=0,\"%s\"", hostname);

      if (MODEM.waitForResponse(70000, &response) != NB_RESPONSE_OK)
      {
        return 0;
      }

      if (!response.startsWith(MODEM_DNS_NAME_RESOLVE_RESPONSE_START) ||
          !response.endsWith(MODEM_DNS_NAME_RESOLVE_RESPONSE_END))
      {
        return 0;
      }

      response.remove(0, 10);
      response.remove(response.length() - 1);

      if (result.fromString(response))
      {
        return 1;
      }

      return 0;
    }

    //////////////////////////////////////////////////////

    // Empty now, just for future implementation
    void handleUrc(const String& urc)
    {
      (void) urc;
    }

    //////////////////////////////////////////////////////

  protected:
    // To be used in derived classes
    int                 _state;
    NB_NetworkStatus_t  _status;
    String              _response;
    int                 _pingResult;
};

///////////////////////////////////////////////////////////////////////


class NBClient_ModemUrcHandler : public ModemUrcHandler
{
  public:

    /** Get last command status
      @return returns 0 if last command is still executing, 1 success, >1 error
    */
    virtual int ready()
    {
      int ready = MODEM.ready();

      if (ready == NB_RESPONSE_IDLE)
      {
        return NB_RESPONSE_IDLE;
      }

      switch (_state)
      {
        case CLIENT_STATE_IDLE:
        default:
          {
            break;
          }

        case CLIENT_STATE_CREATE_SOCKET:
          {
            MODEM.setResponseDataStorage(&_response);

            MODEM.createTCPSocket();

            _state = CLIENT_STATE_WAIT_CREATE_SOCKET_RESPONSE;
            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_WAIT_CREATE_SOCKET_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK || !_response.startsWith(MODEM_CREATE_SOCKET_RESPONSE_START))
            {
              _state = CLIENT_STATE_IDLE;
            }
            else
            {
              _socket = _response.charAt(_response.length() - 1) - '0';

              if (_ssl)
              {
                _state = CLIENT_STATE_ENABLE_SSL;
              }
              else
              {
                _state = CLIENT_STATE_CONNECT;
              }

              ready = NB_RESPONSE_IDLE;
            }

            break;
          }

        case CLIENT_STATE_ENABLE_SSL:
          {
            MODEM.enableSSL(_socket);

            _state = CLIENT_STATE_WAIT_ENABLE_SSL_RESPONSE;
            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_WAIT_ENABLE_SSL_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK)
            {
              _state = CLIENT_STATE_CLOSE_SOCKET;
            }
            else
            {
              _state = CLIENT_STATE_MANAGE_SSL_PROFILE;
            }

            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_MANAGE_SSL_PROFILE:
          {
            // AT+USECPRF=0,0,1 => Root certificate validation without URL integrity check.
            MODEM.manageSSLProfile(0, 0, 1);

            _state = CLIENT_STATE_WAIT_MANAGE_SSL_PROFILE_RESPONSE;
            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_WAIT_MANAGE_SSL_PROFILE_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK)
            {
              _state = CLIENT_STATE_CLOSE_SOCKET;
            }
            else
            {
              _state = CLIENT_STATE_CONNECT;
            }

            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_CONNECT:
          {
            if (_host != NULL)
            {
              MODEM.connect(_socket, _host, _port);
            }
            else
            {
              MODEM.connect(_socket, _ip, _port);
            }

            _state = CLIENT_STATE_WAIT_CONNECT_RESPONSE;
            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_WAIT_CONNECT_RESPONSE:
          {
            if (ready > NB_RESPONSE_OK)
            {
              _state = CLIENT_STATE_CLOSE_SOCKET;

              ready = NB_RESPONSE_IDLE;
            }
            else
            {
              _connected = true;
              _state = CLIENT_STATE_IDLE;
            }

            break;
          }

        case CLIENT_STATE_CLOSE_SOCKET:
          {
            MODEM.closeSocket(_socket);

            _state = CLIENT_STATE_WAIT_CLOSE_SOCKET;
            ready = NB_RESPONSE_IDLE;

            break;
          }

        case CLIENT_STATE_WAIT_CLOSE_SOCKET:
          {
            _state = CLIENT_STATE_RETRIEVE_ERROR;
            _socket = -1;
            break;
          }

        case CLIENT_STATE_RETRIEVE_ERROR:
          {
            MODEM.getSocketError();

            _state = CLIENT_STATE_IDLE;
            break;
          }
      }

      return ready;
    }

    //////////////////////////////////////////////////////
    /** Write a characters buffer with size in request
      @param (uint8_t*) Buffer
      @param (size_t)   Buffer size
      @return buffer size
    */
    size_t write(const uint8_t* buf, size_t size)
    {
      if (_writeSync)
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

      size_t written = 0;
      String command;

      command.reserve(19 + (size > 256 ? 256 : size) * 2);

      while (size)
      {
        size_t chunkSize = size;

        if (chunkSize > 256)
        {
          chunkSize = 256;
        }

        command.reserve(19 + chunkSize * 2);

        command = "AT+USOWR=";
        command += _socket;
        command += ",";
        command += chunkSize;
        command += ",\"";

        for (size_t i = 0; i < chunkSize; i++)
        {
          byte b = buf[i + written];

          byte n1 = (b >> 4) & 0x0f;
          byte n2 = (b & 0x0f);

          command += (char)(n1 > 9 ? 'A' + n1 - 10 : '0' + n1);
          command += (char)(n2 > 9 ? 'A' + n2 - 10 : '0' + n2);
        }

        command += "\"";

        MODEM.send(command);

        if (_writeSync)
        {
          String response;
          int status = MODEM.waitForResponse(10000, &response);

          if ( status != NB_RESPONSE_OK)
          {
            if ( (status == NB_RESPONSE_CME_ERROR) && response.indexOf("Operation not allowed") != SUBSTRING_NOT_FOUND )
            {
              stop();
              break;
            }
            else
            {
              break;
            }
          }
        }

        written += chunkSize;
        size    -= chunkSize;
      }

      return written;
    }

    //////////////////////////////////////////////////////

    void stop()
    {
      _state = CLIENT_STATE_IDLE;

      if (_socket < 0)
      {
        return;
      }

      MODEM.closeSocket(_socket);

      MODEM.waitForResponse(10000);

      NBSocketBuffer.close(_socket);

      _socket = -1;
      _connected = false;
    }

    //////////////////////////////////////////////////////

    virtual void handleUrc(const String& urc)
    {
      NB_LOGDEBUG1(F("NBClient::handleUrc: usr = "), urc);

      if (urc.startsWith(MODEM_SOCKET_DATA_START))
      {
        int socket = urc.charAt(9) - '0';

        if (socket == _socket)
        {
          if (urc.endsWith(MODEM_SOCKET_DATA_END))
          {
            _connected = false;
          }
        }
      }
    }

    //////////////////////////////////////////////////////

  protected:
    // To be used in derived classes
    int         _socket;
    int         _connected;

    int         _state;
    IPAddress   _ip;
    const char* _host;
    uint16_t    _port;
    bool        _ssl;

    bool        _writeSync;
    String      _response;
};

///////////////////////////////////////////////////////////////////////

#include "NBClient_Generic.h"


///////////////////////////////////////////////////////////////////////

class NBUDP_ModemUrcHandler : public ModemUrcHandler
{
  public:

    uint8_t begin(uint16_t port)
    {
      String response;

      NB_LOGDEBUG1(F("NBUDP::begin: port = "), port);

      MODEM.send("AT+USOCR=17");

      if (MODEM.waitForResponse(2000, &response) != NB_RESPONSE_OK)
      {
        NB_LOGDEBUG(F("NBUDP::begin: Error no modem response"));

        return 0;
      }

      _socket = response.charAt(response.length() - 1) - '0';

      MODEM.sendf("AT+USOLI=%d,%d", _socket, port);

      if (MODEM.waitForResponse(10000) != NB_RESPONSE_OK)
      {
        stop();
        return 0;
      }

      NB_LOGDEBUG1(F("NBUDP::begin: OK, _socket = "), _socket);

      return 1;
    }

    //////////////////////////////////////////////////////

    void stop()
    {
      if (_socket < 0)
      {
        return;
      }

      MODEM.closeSocket(_socket);

      MODEM.waitForResponse(10000);

      _socket = -1;
    }

    //////////////////////////////////////////////////////

    int endPacket()
    {
      String command;

      if (_txHost != NULL)
      {
        command.reserve(26 + strlen(_txHost) + _txSize * 2);
      }
      else
      {
        command.reserve(41 + _txSize * 2);
      }

      command += "AT+USOST=";
      command += _socket;
      command += ",\"";

      if (_txHost != NULL)
      {
        command += _txHost;
      }
      else
      {
        command += _txIp[0];
        command += '.';
        command += _txIp[1];
        command += '.';
        command += _txIp[2];
        command += '.';
        command += _txIp[3];
      }

      command += "\",";
      command += _txPort;
      command += ",";
      command += _txSize;
      command += ",\"";

      for (size_t i = 0; i < _txSize; i++)
      {
        byte b = _txBuffer[i];

        byte n1 = (b >> 4) & 0x0f;
        byte n2 = (b & 0x0f);

        command += (char)(n1 > 9 ? 'A' + n1 - 10 : '0' + n1);
        command += (char)(n2 > 9 ? 'A' + n2 - 10 : '0' + n2);
      }

      command += "\"";

      MODEM.send(command);

      if (MODEM.waitForResponse() == NB_RESPONSE_OK)
      {
        return 1;
      }
      else
      {
        return 0;
      }
    }

    //////////////////////////////////////////////////////

    int parsePacket()
    {
      MODEM.poll();

      if (_socket < 0)
      {
        return 0;
      }

      if (!_packetReceived)
      {
        return 0;
      }

      _packetReceived = false;

      String response;

      MODEM.sendf("AT+USORF=%d,%d", _socket, sizeof(_rxBuffer));

      if (MODEM.waitForResponse(10000, &response) != NB_RESPONSE_OK)
      {
        return 0;
      }

      if (!response.startsWith("+USORF: "))
      {
        return 0;
      }

      response.remove(0, 11);

      int firstQuoteIndex = response.indexOf('"');

      if (firstQuoteIndex == SUBSTRING_NOT_FOUND)
      {
        return 0;
      }

      String ip = response.substring(0, firstQuoteIndex);
      _rxIp.fromString(ip);

      response.remove(0, firstQuoteIndex + 2);

      int firstCommaIndex = response.indexOf(',');

      if (firstCommaIndex == SUBSTRING_NOT_FOUND)
      {
        return 0;
      }

      String port = response.substring(0, firstCommaIndex);

      _rxPort = port.toInt();
      firstQuoteIndex = response.indexOf("\"");

      response.remove(0, firstQuoteIndex + 1);
      response.remove(response.length() - 1);

      _rxIndex = 0;
      _rxSize = response.length() / 2;

      for (size_t i = 0; i < _rxSize; i++)
      {
        byte n1 = response[i * 2];
        byte n2 = response[i * 2 + 1];

        n1 = charToInt(n1);
        n2 = charToInt(n2);

        _rxBuffer[i] = (n1 << 4) | n2;
      }

      MODEM.poll();

      return _rxSize;
    }

    //////////////////////////////////////////////////////

    virtual void handleUrc(const String& urc)
    {
      NB_LOGDEBUG1(F("NBUDP::handleUrc: urc = "), urc);

      if (urc.startsWith("+UUSORF: "))
      {
        int socket = urc.charAt(9) - '0';

        if (socket == _socket)
        {
          _packetReceived = true;
        }
      }
      else if (urc.startsWith("+UUSOCL: "))
      {
        int socket = urc.charAt(urc.length() - 1) - '0';

        if (socket == _socket)
        {
          // this socket closed
          _socket = -1;
          _rxIndex = 0;
          _rxSize = 0;
        }
      }
    }

    //////////////////////////////////////////////////////

  protected:
    int         _socket;
    bool        _packetReceived;

    IPAddress   _txIp;
    const char* _txHost;
    uint16_t    _txPort;
    size_t      _txSize;
    uint8_t     _txBuffer[NB_UDP_TX_BUFFER_SIZE];

    IPAddress   _rxIp;
    uint16_t    _rxPort;
    size_t      _rxSize;
    size_t      _rxIndex;
    uint8_t     _rxBuffer[NB_UDP_RX_BUFFER_SIZE];
};

///////////////////////////////////////////////////////////////////////

#endif    // _MODEM_SARA_R4_EXTRA_INCLUDED_H
