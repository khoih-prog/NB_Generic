/*********************************************************************************************************************************
  Modem_SaraR4_Generic.h
  
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

#ifndef _MODEM_SARA_R4_INCLUDED_H
#define _MODEM_SARA_R4_INCLUDED_H

#include <stdarg.h>
#include <stdio.h>

#include <Arduino.h>

#include <IPAddress.h>

#include "NB_Generic_Debug.h"

#include "NB_Type_Generic.h"

#include "NB_Boards_Generic.h"

#include "utility/NBRootCerts_Generic.h"

#if ( NB_USE_SAMD || NB_USE_NRF528XX )

#elif ( NB_USE_SAM_DUE )
  #define Uart      UARTClass
#elif ( NB_USING_SOFTWARE_SERIAL )
  #define Uart      SoftwareSerial
#else
  #define Uart      HardwareSerial
#endif

#if !defined(UBLOX_USING_RESET_PIN)
  #define UBLOX_USING_RESET_PIN    false
  #warning No UBLOX_USING_RESET_PIN
#elif UBLOX_USING_RESET_PIN
  #warning UBLOX_USING_RESET_PIN
#endif

#if !defined(UBLOX_USING_POWER_ON_PIN)
  #define UBLOX_USING_POWER_ON_PIN    false
  #warning No UBLOX_USING_POWER_ON_PIN
#elif UBLOX_USING_POWER_ON_PIN
  #warning UBLOX_USING_POWER_ON_PIN
#endif

/* The NB 1500 does not connect the SARA V_INT pin so that it can be monitored.
   The below constants and associated code enables connecting SARA_VINT to a digital input
   using a level shifter (1.8V to 5V) or simply a MOS transistor, say a 2N7000.
   The code does rudimentary tracking of the on/off state if this is not available.
*/
#define SARA_VINT_OFF       (-1)
#define SARA_VINT_ON        (-2)

#ifndef SARA_VINT
  #define SARA_VINT         SARA_VINT_OFF
#endif

class ModemUrcHandler
{
  public:
    virtual void handleUrc(const String& urc) = 0;
};

//////////////////////////////////////////////////////
// Modem AT Command String definitions.

#define MODEM_AT_COMMAND_START                    "AT"
#define MODEM_AT_COMMAND_END                      "\r\n"

//////////////////////////////////////////////////////
// Modem Response String definitions. To be used with _response.startsWith()

#define MODEM_SOCKET_DATA_START                   "+UUSORD: "
#define MODEM_SOCKET_DATA_END                     ",4294967295"

#define MODEM_READ_SOCKET_RESPONSE_START          "+USORD: "

#define MODEM_CREATE_SOCKET_RESPONSE_START        "+USOCR: "

#define MODEM_DNS_NAME_RESOLVE_RESPONSE_START     "+UDNSRN: \""
#define MODEM_DNS_NAME_RESOLVE_RESPONSE_END       "\""


#define MODEM_GET_CCID_RESPONSE_START             "+CCID: "

#define MODEM_CHECK_REG_RESPONSE_START            "+CREG: "

#define MODEM_CHARSET_CONFIG_RESPONSE_START       "+CSCS: \""

#define MODEM_LIST_SMS_RESPONSE_START             "+CMGL: "

#define MODEM_LIST_SMS_RESPONSE_END               "\r\n+CMGL: "

#define MODEM_PIN_RESPONSE_START                  "+CPIN: "
#define MODEM_PIN_RESPONSE_READY_END              "READY"
#define MODEM_PIN_RESPONSE_SIM_PIN_END            "SIM PIN"
#define MODEM_PIN_RESPONSE_SIM_PUK_END            "SIM PUK"


//////////////////////////////////////////////////////

// End of Modem Response. To be used with indexOf() or lastIndexOf()
#define MODEM_AT_RECEIVING_RESPONSE_OK_END            "OK\r\n"
#define MODEM_AT_RECEIVING_RESPONSE_ERROR_END         "ERROR\r\n"
#define MODEM_AT_RECEIVING_RESPONSE_NO_CARRIER_END    "NO CARRIER\r\n"
#define MODEM_AT_RECEIVING_RESPONSE_CME_ERROR_END     "CME ERROR"

//////////////////////////////////////////////////////

#define MAX_URC_HANDLERS                            8          // 7 sockets + GPRS
#define MODEM_MIN_RESPONSE_OR_URC_WAIT_TIME_MS      20

class ModemClass
{
  public:


    ModemClass(Uart& uart, unsigned long baud, int resetPin = -1, int powerOnPin = -1, int vIntPin = SARA_VINT) :
      _uart(&uart),
      _baud(baud),
      _resetPin(resetPin),
      _powerOnPin(powerOnPin),
      _vIntPin(vIntPin),
      _lastResponseOrUrcMillis(0),
      _atCommandState(AT_COMMAND_IDLE),
      _ready(1),
      _responseDataStorage(NULL)
    {
      _buffer.reserve(64);
    }

    /////////////////////////////////////////

    void setVIntPin(int vIntPin)
    {
      // Allow setting only if unset, used to track state
      if (_vIntPin == SARA_VINT_OFF || _vIntPin == SARA_VINT_ON)
      {
        _vIntPin = vIntPin;
      }
    }

    /////////////////////////////////////////

    int isPowerOn()
    {
      if (_vIntPin == SARA_VINT_OFF)
      {
        return NB_MODEM_START_ERROR;
      }
      else if (_vIntPin == SARA_VINT_ON)
      {
        return NB_MODEM_START_OK;
      }

      return digitalRead(_vIntPin);
    }

    /////////////////////////////////////////

    int begin(unsigned long baud, bool restart = false)
    {
      // datasheet warns not to use _resetPin, this may lead to an unrecoverable state
      digitalWrite(_resetPin, LOW);

      if (restart)
      {
        shutdown();
        end();
      }

      bool newBaud = false;

      if (_baud != baud)
      {
        _baud = ( baud > 115200 ? 115200 : baud );
        newBaud = true;
      }

      NB_LOGDEBUG1(F("begin: UART baud = "), _baud);

      _uart->begin(_baud > 115200 ? 115200 : _baud);

#if UBLOX_USING_POWER_ON_PIN
      // power on module
      if (isPowerOn() == NB_MODEM_START_ERROR)
      {
        NB_LOGDEBUG(F("begin: Reset"));
        digitalWrite(_powerOnPin, HIGH);
        
        delay(150); // Datasheet says power-on pulse should be >=150ms, <=3200ms
        
        digitalWrite(_powerOnPin, LOW);
        
        setVIntPin(SARA_VINT_ON);
        NB_LOGDEBUG(F("begin: Reset done"));
      }
      else
      {
        if (!autosense())
        {
          NB_LOGDEBUG(F("begin: nonrestart autosense error"));
          return NB_MODEM_START_ERROR;
        }
      }
#endif

      NB_LOGDEBUG(F("begin: Check autosense"));

      if (!autosense())
      {
        NB_LOGDEBUG(F("begin: autosense error"));
        return NB_MODEM_START_ERROR;
      }

      NB_LOGDEBUG(F("begin: Check baud"));

      // KH, must always set baud here
      if ( restart || newBaud )
      {
        NB_LOGDEBUG1(F("begin: Set baud = "), _baud);

        sendf("AT+IPR=%ld", _baud);

        if (waitForResponse() != NB_RESPONSE_OK)
        {
          NB_LOGDEBUG(F("begin: Set baud error"));
          return NB_MODEM_START_ERROR;
        }

        _uart->end();
        delay(100);
        _uart->begin(_baud);

        if (!autosense())
        {
          NB_LOGDEBUG(F("begin: Set baud autosense error"));
          return NB_MODEM_START_ERROR;
        }
      }

      return NB_MODEM_START_OK;
    }

    /////////////////////////////////////////

    int begin(bool restart = false)
    {
      return begin(_baud, restart);
    }

    /////////////////////////////////////////

    int shutdown()
    {
      // AT command shutdown
      if (isPowerOn() != NB_MODEM_START_ERROR)
      {
        NB_LOGDEBUG(F("Shutdown: AT+CPWROFF"));
        send("AT+CPWROFF");

        if (waitForResponse(40000) != NB_RESPONSE_OK)
        {
          NB_LOGDEBUG(F("Shutdown Error"));
          return NB_MODEM_START_ERROR;
        }

        setVIntPin(SARA_VINT_OFF);
      }

      return NB_MODEM_START_OK;
    }

    /////////////////////////////////////////

    void end()
    {
      _uart->end();

      // Hardware pin power off
      if (isPowerOn() != NB_MODEM_START_ERROR)
      {
        NB_LOGDEBUG(F("Power Off"));
        digitalWrite(_powerOnPin, HIGH);
        
        delay(1500); // Datasheet says power-off pulse should be >=1500ms
        
        digitalWrite(_powerOnPin, LOW);
        setVIntPin(SARA_VINT_OFF);
      }
    }

    /////////////////////////////////////////

    void hardReset()
    {
      NB_LOGDEBUG(F("HardReset"));
      // Hardware pin reset, only use in EMERGENCY
      digitalWrite(_resetPin, HIGH);
      
      delay(1000); // Datasheet says nothing, so guess we wait one second
      
      digitalWrite(_resetPin, LOW);
      setVIntPin(SARA_VINT_OFF);
    }

    /////////////////////////////////////////

    void debug()
    {
      debug(DBG_PORT_NB);
    }

    /////////////////////////////////////////

    void debug(Print& p)
    {
      _debugPrint = &p;
    }

    /////////////////////////////////////////

    void noDebug()
    {
      _debugPrint = NULL;
    }

    /////////////////////////////////////////

    int autosense(unsigned long timeout = 10000)
    {
      for (unsigned long start = millis(); (millis() - start) < timeout;)
      {
        if (noop())
        {
          return NB_MODEM_START_OK;
        }

        delay(100);
      }

      return NB_MODEM_START_ERROR;
    }

    /////////////////////////////////////////

    bool noop()
    {
      send(MODEM_AT_COMMAND_START);

      return (waitForResponse() == NB_RESPONSE_OK);
    }

    /////////////////////////////////////////

    bool reset()
    {
      NB_LOGDEBUG(F("reset: AT+CFUN=15"));

      send("AT+CFUN=15");

      return (waitForResponse(1000) == NB_RESPONSE_OK);
    }

    /////////////////////////////////////////

    void createTCPSocket()
    {
      send("AT+USOCR=6");
    }

    /////////////////////////////////////////

    void createUDPSocket()
    {
      send("AT+USOCR=17");
    }

    /////////////////////////////////////////

    void closeSocket(int socket)
    {
      sendf("AT+USOCL=%d", socket);
    }

    /////////////////////////////////////////

    void enableSSL(int socket)
    {
      sendf("AT+USOSEC=%d,1,0", socket);
    }

    /////////////////////////////////////////

    /*
      0: certificate validation level; allowed values for <param_val1> (number):
        - 0: level 0 - No validation; the server certificate will not be checked or verified. The server in
      this case is not authenticated.
        - 1: level 1 - Root certificate validation without URL integrity check. The server certificate will
      be verified with a specific trusted certificates or with each of the imported trusted root
      certificates.
        - 2: level 2 - Root certificate validation with URL integrity check. Level 1 validation with an
      additional URL integrity check.
        - 3: level 3 - Root certificate validation with check of certificate validity date. Level 2
      validation with an additional check of certificate validity date.
    */
    void manageSSLProfile(int profile, int validation_level, int param)
    {
      // AT+USECPRF=0,0,1 => Root certificate validation without URL integrity check.
      sendf("AT+USECPRF=%d,%d,%d");
    }

    /////////////////////////////////////////

    void connect(int socket, const char* host, uint16_t port)
    {
      sendf("AT+USOCO=%d,\"%s\",%d", socket, host, port);
    }

    /////////////////////////////////////////

    void connect(int socket, IPAddress ip, uint16_t port)
    {
      sendf("AT+USOCO=%d,\"%d.%d.%d.%d\",%d", socket, ip[0], ip[1], ip[2], ip[3], port);
    }

    /////////////////////////////////////////

    void getSocketError()
    {
      send("AT+USOER");
    }

    /////////////////////////////////////////

    size_t write(uint8_t c)
    {
      return _uart->write(c);
    }

    /////////////////////////////////////////

    size_t write(const uint8_t* buf, size_t size)
    {
      size_t result = _uart->write(buf, size);

      // the R410m echoes the binary data - we don't want it to do so
      size_t ignoreCount = 0;

      while (ignoreCount < result)
      {
        if (_uart->available())
        {
          _uart->read();

          ignoreCount++;
        }
      }

      return result;
    }

    /////////////////////////////////////////

    void send(const char* command)
    {
      // compare the time of the last response or URC and ensure
      // at least 20ms have passed before sending a new command
      unsigned long delta = millis() - _lastResponseOrUrcMillis;

      if (delta < MODEM_MIN_RESPONSE_OR_URC_WAIT_TIME_MS)
      {
        delay(MODEM_MIN_RESPONSE_OR_URC_WAIT_TIME_MS - delta);
      }

      _uart->println(command);
      _uart->flush();
      _atCommandState = AT_COMMAND_IDLE;
      _ready = NB_RESPONSE_IDLE;
    }

    /////////////////////////////////////////

    void send(const String& command)
    {
      send(command.c_str());
    }

    /////////////////////////////////////////

    void sendf(const char *fmt, ...)
    {
      char buf[BUFSIZ];

      va_list ap;
      va_start((ap), (fmt));
      vsnprintf(buf, sizeof(buf) - 1, fmt, ap);
      va_end(ap);

      send(buf);
    }

    /////////////////////////////////////////

    int waitForPrompt(unsigned long timeout = 500)
    {
      for (unsigned long start = millis(); (millis() - start) < timeout;)
      {
        while (_uart->available())
        {
          char c = _uart->read();

          if (_debugPrint)
          {
            _debugPrint->print(c);
          }

          _buffer += c;

          if (_buffer.endsWith(">"))
          {
            return NB_RESPONSE_OK;
          }
        }
      }

      return NB_RESPONSE_TIMEOUT;
    }

    /////////////////////////////////////////

    int waitForResponse(unsigned long timeout = 200, String* responseDataStorage = NULL)
    {
      _responseDataStorage = responseDataStorage;

      for (unsigned long start = millis(); (millis() - start) < timeout;)
      {
        int response = ready();

        if (response != NB_RESPONSE_IDLE)
        {
          _responseDataStorage = NULL;
          return response;
        }
      }

      _responseDataStorage = NULL;
      _buffer = "";

      return NB_RESPONSE_TIMEOUT;
    }

    /////////////////////////////////////////

    int ready()
    {
      poll();

      return _ready;
    }

    /////////////////////////////////////////

    void poll()
    {
      while (_uart->available())
      {
        char c = _uart->read();

        if (_debugPrint)
        {
          _debugPrint->write(c);
        }

        _buffer += c;

        switch (_atCommandState)
        {
          case AT_COMMAND_IDLE:
          default:
            {
              if (_buffer.startsWith(MODEM_AT_COMMAND_START) && _buffer.endsWith(MODEM_AT_COMMAND_END))
              {
                _atCommandState = AT_RECEIVING_RESPONSE;
                _buffer = "";
              }
              else if (_buffer.endsWith(MODEM_AT_COMMAND_END))
              {
                _buffer.trim();

                if (_buffer.length())
                {
                  _lastResponseOrUrcMillis = millis();

                  for (int i = 0; i < MAX_URC_HANDLERS; i++)
                  {
                    if (_urcHandlers[i] != NULL)
                    {
                      _urcHandlers[i]->handleUrc(_buffer);
                    }
                  }
                }

                _buffer = "";
              }

              break;
            }

          case AT_RECEIVING_RESPONSE:
            {
              if (c == '\n')
              {
                _lastResponseOrUrcMillis = millis();
                int responseResultIndex;

                responseResultIndex = _buffer.lastIndexOf(MODEM_AT_RECEIVING_RESPONSE_OK_END);

                if (responseResultIndex != SUBSTRING_NOT_FOUND)
                {
                  _ready = NB_RESPONSE_OK;
                }
                else
                {
                  responseResultIndex = _buffer.lastIndexOf(MODEM_AT_RECEIVING_RESPONSE_ERROR_END);

                  if (responseResultIndex != SUBSTRING_NOT_FOUND)
                  {
                    _ready = NB_RESPONSE_ERROR;
                  }
                  else
                  {
                    responseResultIndex = _buffer.lastIndexOf(MODEM_AT_RECEIVING_RESPONSE_NO_CARRIER_END);

                    if (responseResultIndex != SUBSTRING_NOT_FOUND)
                    {
                      _ready = NB_RESPONSE_NO_CARRIER;
                    }
                    else
                    {
                      responseResultIndex = _buffer.lastIndexOf(MODEM_AT_RECEIVING_RESPONSE_CME_ERROR_END);

                      if (responseResultIndex != SUBSTRING_NOT_FOUND)
                      {
                        _ready = NB_RESPONSE_CME_ERROR;
                      }
                    }
                  }
                }

                if (_ready != NB_RESPONSE_IDLE)
                {
                  if (_responseDataStorage != NULL)
                  {
                    if (_ready > NB_RESPONSE_OK)
                    {
                      _buffer.substring(responseResultIndex);
                    }
                    else
                    {
                      _buffer.remove(responseResultIndex);
                    }

                    _buffer.trim();

                    *_responseDataStorage = _buffer;

                    _responseDataStorage = NULL;
                  }

                  _atCommandState = AT_COMMAND_IDLE;
                  _buffer = "";

                  return;
                }
              }

              break;
            }
        }
      }
    }

    /////////////////////////////////////////

    void setResponseDataStorage(String* responseDataStorage)
    {
      _responseDataStorage = responseDataStorage;
    }

    /////////////////////////////////////////

    void addUrcHandler(ModemUrcHandler* handler)
    {
      for (int i = 0; i < MAX_URC_HANDLERS; i++)
      {
        if (_urcHandlers[i] == NULL)
        {
          _urcHandlers[i] = handler;
          break;
        }
      }
    }

    /////////////////////////////////////////

    void removeUrcHandler(ModemUrcHandler* handler)
    {
      for (int i = 0; i < MAX_URC_HANDLERS; i++)
      {
        if (_urcHandlers[i] == handler)
        {
          _urcHandlers[i] = NULL;
          break;
        }
      }
    }

    /////////////////////////////////////////

    void setBaudRate(unsigned long baud)
    {
      _baud = baud;
    }

    ///////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////
    //From NBFileUtils_Generic.hpp

    void setReportError()
    {
      send("AT+CMEE=2");
    }

    /////////////////////////////////////////

    void listFile()
    {
      send("AT+ULSTFILE=0");
    }

    /////////////////////////////////////////

    void fileSize(const String filename)
    {
      sendf("AT+ULSTFILE=2,\"%s\"", filename.c_str());
    }

    /////////////////////////////////////////

    void freeSpace()
    {
      send("AT+ULSTFILE=1");
    }

    /////////////////////////////////////////

    void deleteFile(const String filename)
    {
      // Delete file +UDELFILE
      sendf("AT+UDELFILE=\"%s\"", filename.c_str());
    }

    /////////////////////////////////////////

    void readBlock(const String filename, const uint32_t offset, const uint32_t len)
    {
      sendf("AT+URDBLOCK=\"%s\",%d,%d", filename.c_str(), offset * 2, len * 2);
    }

    /////////////////////////////////////////

    void downloadFile(const String filename, uint32_t size)
    {
      sendf("AT+UDWNFILE=\"%s\",%d", filename.c_str(), size * 2);
    }

    /////////////////////////////////////////

    void readFile(const String filename)
    {
      sendf("AT+URDFILE=\"%s\"", filename.c_str());
    }

    //////////////////////////////////////////////////////////////////////////////////
    //From NB_Generic_Impl.hpp

    int isAccessAlive()
    {
      String response;

      send("AT+CEREG?");

      if (waitForResponse(100, &response) == NB_RESPONSE_OK)
      {
        int status = response.charAt(response.length() - 1) - '0';

        if (status == NB_NETWORK_HOME || status == NB_NETWORK_ROAMING || status == NB_NETWORK_EMERGENCY_ONLY)
        {
          return 1;
        }
      }

      return 0;
    }

    /////////////////////////////////////////

    int NB_ready(NB_Data& _nbData, String& _response)
    {
      if (_nbData._state == NB_ERROR)
      {
        return NB_RESPONSE_ERROR;
      }

      int modemReady = ready();

      if (modemReady == NB_RESPONSE_IDLE)
      {
        return NB_RESPONSE_IDLE;
      }

      switch (_nbData._readyState)
      {
        case READY_STATE_SET_ERROR_DISABLED:
          {
            send("AT+CMEE=0");
            
            _nbData._readyState = READY_STATE_WAIT_SET_ERROR_DISABLED;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_SET_ERROR_DISABLED:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_MINIMUM_FUNCTIONALITY_MODE;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_MINIMUM_FUNCTIONALITY_MODE:
          {
            send("AT+CFUN=0");
            
            _nbData._readyState = READY_STATE_WAIT_SET_MINIMUM_FUNCTIONALITY_MODE;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_SET_MINIMUM_FUNCTIONALITY_MODE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_CHECK_SIM;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_CHECK_SIM:
          {
            setResponseDataStorage(&_response);
            send("AT+CPIN?");
            
            _nbData._readyState = READY_STATE_WAIT_CHECK_SIM_RESPONSE;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_CHECK_SIM_RESPONSE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              // NB_ERROR => retry
              _nbData._readyState = READY_STATE_CHECK_SIM;
              modemReady = NB_RESPONSE_IDLE;
            }
            else
            {
              if (_response.endsWith(MODEM_PIN_RESPONSE_READY_END))
              {
                _nbData._readyState = READY_STATE_SET_PREFERRED_MESSAGE_FORMAT;
                modemReady = NB_RESPONSE_IDLE;
              }
              else if (_response.endsWith(MODEM_PIN_RESPONSE_SIM_PIN_END))
              {
                _nbData._readyState = READY_STATE_UNLOCK_SIM;
                modemReady = NB_RESPONSE_IDLE;
              }
              else
              {
                _nbData._state = NB_ERROR;
                modemReady = NB_RESPONSE_ERROR;
              }
            }

            break;
          }

        case READY_STATE_UNLOCK_SIM:
          {
            if (_nbData._pin != NULL)
            {
              setResponseDataStorage(&_response);
              sendf("AT+CPIN=\"%s\"", _nbData._pin);

              _nbData._readyState = READY_STATE_WAIT_UNLOCK_SIM_RESPONSE;
              modemReady = NB_RESPONSE_IDLE;
            }
            else
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }

            break;
          }

        case READY_STATE_WAIT_UNLOCK_SIM_RESPONSE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_DETACH_DATA;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_DETACH_DATA:
          {
            send("AT+CGATT=0");
            
            _nbData._readyState = READY_STATE_WAIT_DETACH_DATA;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_DETACH_DATA:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_PREFERRED_MESSAGE_FORMAT;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_PREFERRED_MESSAGE_FORMAT:
          {
            send("AT+CMGF=1");
            
            _nbData._readyState = READY_STATE_WAIT_SET_PREFERRED_MESSAGE_FORMAT_RESPONSE;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_SET_PREFERRED_MESSAGE_FORMAT_RESPONSE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_HEX_MODE;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_HEX_MODE:
          {
            send("AT+UDCONF=1,1");
            
            _nbData._readyState = READY_STATE_WAIT_SET_HEX_MODE_RESPONSE;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_SET_HEX_MODE_RESPONSE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_AUTOMATIC_TIME_ZONE;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_AUTOMATIC_TIME_ZONE:
          {
            send("AT+CTZU=1");
            
            _nbData._readyState = READY_STATE_WAIT_SET_AUTOMATIC_TIME_ZONE_RESPONSE;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_SET_AUTOMATIC_TIME_ZONE_RESPONSE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_APN;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_APN:
          {
            sendf("AT+CGDCONT=1,\"IP\",\"%s\"", _nbData._apn);
            
            _nbData._readyState = READY_STATE_WAIT_SET_APN;
            modemReady = NB_RESPONSE_IDLE;

            break;
          }

        case READY_STATE_WAIT_SET_APN:
          {
            if (modemReady > NB_RESPONSE_OK) 
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_APN_AUTH;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_APN_AUTH:
          {
            if (strlen(_nbData._username) > 0 || strlen(_nbData._password) > 0)
            {
              // CHAP
              sendf("AT+UAUTHREQ=1,2,\"%s\",\"%s\"", _nbData._password, _nbData._username);
            }
            else
            {
              // no auth
              sendf("AT+UAUTHREQ=1,%d", 0);
            }

            _nbData._readyState = READY_STATE_WAIT_SET_APN_AUTH;
            modemReady = NB_RESPONSE_IDLE;

            break;
          }

        case READY_STATE_WAIT_SET_APN_AUTH:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_SET_FULL_FUNCTIONALITY_MODE;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_SET_FULL_FUNCTIONALITY_MODE:
          {
            send("AT+CFUN=1");
            
            _nbData._readyState = READY_STATE_WAIT_SET_FULL_FUNCTIONALITY_MODE;
            modemReady = NB_RESPONSE_IDLE;

            break;
          }

        case READY_STATE_WAIT_SET_FULL_FUNCTIONALITY_MODE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              _nbData._readyState = READY_STATE_CHECK_REGISTRATION;
              modemReady = NB_RESPONSE_IDLE;
            }

            break;
          }

        case READY_STATE_CHECK_REGISTRATION:
          {
            setResponseDataStorage(&_response);
            send("AT+CEREG?");

            _nbData._readyState = READY_STATE_WAIT_CHECK_REGISTRATION_RESPONSE;
            modemReady = NB_RESPONSE_IDLE;
            break;
          }

        case READY_STATE_WAIT_CHECK_REGISTRATION_RESPONSE:
          {
            if (modemReady > NB_RESPONSE_OK)
            {
              _nbData._state = NB_ERROR;
              modemReady = NB_RESPONSE_ERROR;
            }
            else
            {
              int status = _response.charAt(_response.length() - 1) - '0';

              if (status == NB_NETWORK_NOT_REGISTERED_NOT_SEARCHING || status == NB_NETWORK_UNKNOWN)
              {
                _nbData._readyState = READY_STATE_CHECK_REGISTRATION;
                modemReady = NB_RESPONSE_IDLE;
              }
              else if (status == NB_NETWORK_HOME || status == NB_NETWORK_ROAMING || status == NB_NETWORK_EMERGENCY_ONLY)
              {
                _nbData._readyState = READY_STATE_DONE;
                _nbData._state = NB_READY;
                modemReady = NB_RESPONSE_OK;
              }
              else if (status == NB_NETWORK_NOT_REGISTERED_SEARCHING)
              {
                _nbData._readyState = READY_STATE_CHECK_REGISTRATION;
                _nbData._state = NB_CONNECTING;
                modemReady = NB_RESPONSE_IDLE;
              }
              else if (status == NB_NETWORK_REGISTRATION_DENIED)
              {
                _nbData._state = NB_ERROR;
                modemReady = NB_RESPONSE_ERROR;
              }
            }

            break;
          }

        case READY_STATE_DONE:
          break;
      }

      return modemReady;
    }

    /////////////////////////////////////////////////////////////

    unsigned long getTime()
    {
      String response;

      send("AT+CCLK?");

      if (waitForResponse(100, &response) != NB_RESPONSE_OK)
      {
        return 0;
      }

      struct tm now;

      int dashIndex = response.lastIndexOf('-');

      if (dashIndex != SUBSTRING_NOT_FOUND)
      {
        response.remove(dashIndex);
      }

      if (strptime(response.c_str(), "+CCLK: \"%y/%m/%d,%H:%M:%S", &now) != NULL)
      {
        // adjust for timezone offset which is +/- in 15 minute increments

        time_t result = mktime(&now);
        time_t delta = ((response.charAt(26) - '0') * 10 + (response.charAt(27) - '0')) * (15 * 60);

        if (response.charAt(25) == '-')
        {
          result += delta;
        }
        else if (response.charAt(25) == '+')
        {
          result -= delta;
        }

        return result;
      }

      return 0;
    }

    /////////////////////////////////////////////////////////////

    unsigned long getLocalTime()
    {
      String response;

      send("AT+CCLK?");

      if (waitForResponse(100, &response) != NB_RESPONSE_OK)
      {
        return 0;
      }

      struct tm now;

      if (strptime(response.c_str(), "+CCLK: \"%y/%m/%d,%H:%M:%S", &now) != NULL)
      {
        time_t result = mktime(&now);
        return result;
      }

      return 0;
    }

    /////////////////////////////////////////////////////////////

    bool setTime(unsigned long const epoch, int const timezone)
    {
      String hours, date;

      const uint8_t daysInMonth []  = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 };
      unsigned long unix_time       = epoch - 946684800UL; /* Subtract seconds from 1970 to 2000 */

      if (((unix_time  % 86400L) / 3600) < 10 )
      {
        hours = "0";
      }

      hours += String((unix_time  % 86400L) / 3600) + ":";

      if ( ((unix_time  % 3600) / 60) < 10 )
      {
        hours = "0";
      }

      hours += String((unix_time  % 3600) / 60) + ":";

      if ((unix_time % 60) < 10 )
      {
        hours += "0";
      }

      hours += String(unix_time % 60) + "+";

      if (timezone < 10)
      {
        hours += "0";
      }

      hours += String(timezone);

      /* Convert unix_time from seconds to days */
      int days = unix_time / (24 * 3600);
      int leap;
      int year = 0;

      while (1)
      {
        leap = year % 4 == 0;

        if (days < 365 + leap)
        {
          if (year < 10)
          {
            date += "0";
          }

          break;
        }

        days -= 365 + leap;
        year++;
      }

      date += String(year) + "/";

      int month;

      for (month = 1; month < 12; month++)
      {
        uint8_t daysPerMonth = daysInMonth[month - 1];

        if (leap && month == 2)
          daysPerMonth++;

        if (days < daysPerMonth)
        {
          if (month < 10)
          {
            date += "0";
          }

          break;
        }

        days -= daysPerMonth;
      }

      date += String(month) + "/";

      if ((days + 1) < 10)
      {
        date += "0";
      }

      date +=  String(days + 1) + ",";

      send("AT+CCLK=\"" + date + hours + "\"");

      if (waitForResponse(100) != NB_RESPONSE_OK)
      {
        return false;
      }

      return true;
    }

    /////////////////////////////////////////
    //From NBPIN_Generic_Impl.hpp

    int isPIN()
    {
      String response;

      for (unsigned long start = millis(); (millis() - start) < 1000;)
      {
        send("AT+CPIN?");

        if (waitForResponse(10000, &response) == NB_RESPONSE_OK)
        {
          if (response.startsWith(MODEM_PIN_RESPONSE_START))
          {
            if (response.endsWith(MODEM_PIN_RESPONSE_READY_END))
            {
              NB_LOGDEBUG("SIM_PIN_UNLOCKED.");
              
              return SIM_PIN_UNLOCKED;
            }
            else if (response.endsWith(MODEM_PIN_RESPONSE_SIM_PIN_END))
            {
              NB_LOGDEBUG("SIM_PIN_NEED_PIN.");
              
              return SIM_PIN_NEED_PIN;
            }
            else if (response.endsWith(MODEM_PIN_RESPONSE_SIM_PUK_END))
            {
              NB_LOGDEBUG("SIM_PIN_NEED_PUK.");
              
              return SIM_PIN_NEED_PUK;
            }
            else
            {
              NB_LOGDEBUG("isPIN: response error");
              
              return SIM_PIN_ERROR;
            }
          }
        }

        delay(100);
      }

      NB_LOGDEBUG("isPIN: response error timeout");
      
      return SIM_PIN_ERROR;
    }

    ////////////////////////////////////////

    int checkPIN(String pin)
    {
      sendf("AT+CPIN=\"%s\"", pin.c_str());

      if (waitForResponse(10000) == NB_RESPONSE_OK)
      {
        NB_LOGDEBUG("Check PIN OK.");
        
        return 0;
      }
      
      NB_LOGDEBUG("Check PIN Error.");

      return -1;
    }

    ////////////////////////////////////////

    int checkPUK(String puk, String pin)
    {
      sendf("AT+CPIN=\"%s\",\"%s\"", puk.c_str(), pin.c_str());

      if (waitForResponse(10000) == NB_RESPONSE_OK)
      {
        NB_LOGDEBUG("Check PUK OK.");
        
        return 0;
      }
      
      NB_LOGDEBUG("Check PUK Error.");

      return -1;
    }

    ////////////////////////////////////////

    void changePIN(String old, String pin)
    {
      sendf("AT+CPWD=\"SC\",\"%s\",\"%s\"", old.c_str(), pin.c_str());

      if (waitForResponse(10000) == NB_RESPONSE_OK)
      {
        NB_LOGERROR("Pin changed successfully.");
      }
      else
      {
        NB_LOGERROR("ERROR");
      }
    }

    ////////////////////////////////////////

    void switchPIN(String pin, bool& _pinUsed)
    {
      String response;

      NB_LOGDEBUG1(F("NBPIN::switchPIN: pin = "), pin);

      // Facility lock +CLCK => query SIM status
      send("AT+CLCK=\"SC\",2");

      if (waitForResponse(180000, &response) != NB_RESPONSE_OK)
      {
        NB_LOGDEBUG(F("NBPIN::switchPIN: Error no response"));
        return;
      }

      if (response == "+CLCK: 0")
      {
        // Facility lock +CLCK => Lock SIM
        sendf("AT+CLCK=\"SC\",1,\"%s\"", pin.c_str());

        if (waitForResponse(180000, &response) == NB_RESPONSE_OK)
        {
          NB_LOGDEBUG(F("NBPIN::switchPIN: OK. PIN lock on."));
          _pinUsed = true;
        }
        else
        {
          NB_LOGDEBUG(F("NBPIN::switchPIN: Error +CLCK: 0"));
          _pinUsed = false;
        }
      }
      else if (response == "+CLCK: 1")
      {
        // Facility lock +CLCK => Unlock SIM
        sendf("AT+CLCK=\"SC\",0,\"%s\"", pin.c_str());

        if (waitForResponse(180000, &response) == NB_RESPONSE_OK)
        {
          NB_LOGDEBUG(F("NBPIN::switchPIN: OK. PIN lock off."));
          _pinUsed = false;
        }
        else
        {
          NB_LOGDEBUG(F("NBPIN::switchPIN: Error +CLCK: 1"));
          _pinUsed = true;
        }
      }
      else
      {
        NB_LOGDEBUG(F("NBPIN::switchPIN: Error"));
      }
    }

    ////////////////////////////////////////

    int checkReg()
    {
      for (unsigned long start = millis(); (millis() - start) < 10000L;)
      {
        // Network registration status +CREG
        send("AT+CREG?");

        String response = "";

        if (waitForResponse(100, &response) == NB_RESPONSE_OK)
        {
          if (response.startsWith(MODEM_CHECK_REG_RESPONSE_START))
          {
            if (response.endsWith(",1"))
            {
              NB_LOGDEBUG(F("checkReg: NETWORK_HOME."));
              
              return NB_NETWORK_CHECK_IS_HOME;
            }
            else if (response.endsWith(",5"))
            {
              NB_LOGDEBUG(F("checkReg: ROAMING."));
              
              return NB_NETWORK_CHECK_IS_ROAMING;
            }
          }
        }

        delay(100);
      }

      NB_LOGDEBUG(F("checkReg: NETWORK_CHECK_ERROR."));
      
      return NB_NETWORK_CHECK_IS_ERROR;
    }

    ////////////////////////////////////////

    void querySIMStatus()
    {
      send("AT+CLCK=\"SC\",2");
    }

    ////////////////////////////////////////

    void lockSIM_PIN(String pin)
    {
      sendf("AT+CLCK=\"SC\",1,\"%s\"", pin.c_str());
    }

    ////////////////////////////////////////

    void unlockSIM_PIN(String pin)
    {
      sendf("AT+CLCK=\"SC\",0,\"%s\"", pin.c_str());
    }

    ////////////////////////////////////////

    void checkRegStatus()
    {
      // Network registration status +CREG
      send("AT+CREG?");
    }

    ////////////////////////////////////////

    int unlockSim(String pin)
    {
      sendf("AT+CPIN=\"%s\"", pin.c_str());

      if (waitForResponse(10000) == NB_RESPONSE_OK)
      {
        NB_LOGDEBUG(F("unlockSim: SIM_PIN_UNLOCKED."));
        
        return SIM_PIN_UNLOCKED;
      }

      return checkSimStatus();
    }

    ////////////////////////////////////////

    int checkSimStatus()
    {
      return isPIN();
    }

    ///////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////
    //From NBScanner_Generic_Impl.hpp

    String getCurrentCarrier()
    {
      String response;

      send("AT+COPS?");

      if (waitForResponse(180000, &response) == NB_RESPONSE_OK)
      {
        int firstQuoteIndex = response.indexOf('"');
        int lastQuoteIndex  = response.lastIndexOf('"');

        if ( (firstQuoteIndex != SUBSTRING_NOT_FOUND) && (lastQuoteIndex != SUBSTRING_NOT_FOUND) 
          && (firstQuoteIndex != lastQuoteIndex) )
        {
          return response.substring(firstQuoteIndex + 1, lastQuoteIndex);
        }
      }

      return "";
    }

    /////////////////////////////////////////

    String getSignalStrength()
    {
      String response;

      send("AT+CSQ");

      if (waitForResponse(100, &response) == NB_RESPONSE_OK)
      {
        int firstSpaceIndex = response.indexOf(' ');
        int lastCommaIndex  = response.lastIndexOf(',');

        if ( (firstSpaceIndex != SUBSTRING_NOT_FOUND) && (lastCommaIndex != SUBSTRING_NOT_FOUND) )
        {
          return response.substring(firstSpaceIndex + 1, lastCommaIndex);
        }
      }

      return "";
    }

    /////////////////////////////////////////

    String readNetworks()
    {
      String response;

      send("AT+COPS=?");

      if (waitForResponse(180000, &response) == NB_RESPONSE_OK)
      {
        String result;
        unsigned int responseLength = response.length();

        for (unsigned int i = 0; i < responseLength; i++)
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

    ///////////////////////////////////////////////////////////////////////
    //From utility/NB_SMS_Generic_Impl.hpp

    int setCharset(int& inCharset, const char* charset)
    {
      String readcharset(0);

      if ( charset == nullptr )
      {
        if ( inCharset != SMS_CHARSET_NONE )
        {
          return inCharset;
        }
      }
      else
      {
        sendf("AT+CSCS=\"%s\"", charset);

        if (waitForResponse() != NB_RESPONSE_OK)
        {
          return 0;
        }
      }

      sendf("AT+CSCS?");

      if ( (waitForResponse(100, &readcharset) == 1) && readcharset.startsWith(MODEM_CHARSET_CONFIG_RESPONSE_START))
      {
        inCharset = readcharset[8];
        
        return inCharset;
      }

      return 0;
    }

    /////////////////////////////////////////

    int beginSMS(NB_SMS_Data& _smsData, const char* to)
    {
      setCharset(_smsData.charset, nullptr);

      for (const char*iptr = "AT+CMGS=\""; *iptr != 0; write(*iptr++));

      if ( (_smsData.charset == SMS_CHARSET_UCS2) && (*to == '+') )
      {
        write('0');
        write('0');
        write('2');
        write('B');
        to++;
      }

      while (*to != 0)
      {
        if (_smsData.charset == SMS_CHARSET_UCS2)
        {
          write('0');
          write('0');
          write('3');
        }

        write(*to++);
      }

      send("\"");

      if (waitForResponse(100) == NB_RESPONSE_ERROR)
      {
        _smsData.smsTxActive = false;

        return (_smsData.synch) ? 0 : 2;
      }

      _smsData.indexUTF8 = 0;
      _smsData.smsTxActive = true;

      return 1;
    }

    /////////////////////////////////////////

    void listUnreadSMS()
    {
      send("AT+CMGL=\"REC UNREAD\"");
    }

    /////////////////////////////////////////

    int available(NB_SMS_Data& _smsData, String& _incomingBuffer)
    {
      int nextMessageIndex = _incomingBuffer.indexOf(MODEM_LIST_SMS_RESPONSE_START);

      if (nextMessageIndex != SUBSTRING_NOT_FOUND)
      {
        _incomingBuffer.remove(0, nextMessageIndex);
      }
      else
      {
        _incomingBuffer = "";
      }

      if (_incomingBuffer.length() == 0)
      {
        int response;

        if (_smsData.state == SMS_STATE_IDLE)
        {
          setCharset(_smsData.charset, nullptr);
          _smsData.state = SMS_STATE_LIST_MESSAGES;
        }

        if (_smsData.synch)
        {
          unsigned long start = millis();
          while ((response = ready()) == NB_RESPONSE_IDLE && (millis() - start) < 3 * 60 * 1000)
          {
            delay(100);
          }
        }
        else
        {
          response = ready();
        }

        if (response != NB_RESPONSE_OK)
        {
          return 0;
        }
      }

      if (_incomingBuffer.startsWith(MODEM_LIST_SMS_RESPONSE_START))
      {

        _incomingBuffer.remove(0, 7);

        _smsData.smsDataIndex = _incomingBuffer.indexOf('\n') + 1;

        _smsData.smsDataEndIndex = _incomingBuffer.indexOf(MODEM_LIST_SMS_RESPONSE_END, _smsData.smsDataIndex);

        if (_smsData.smsDataEndIndex == SUBSTRING_NOT_FOUND)
        {
          _smsData.smsDataEndIndex = _incomingBuffer.length() - 1;
        }

        return (_smsData.smsDataEndIndex - _smsData.smsDataIndex) + 1;
      }
      else
      {
        _incomingBuffer = "";
      }

      return 0;
    }

    /////////////////////////////////////////

    void flush(NB_SMS_Data& _smsData, String& _incomingBuffer)
    {
      int smsIndexEnd = _incomingBuffer.indexOf(',');

      _smsData.ptrUTF8 = "";

      if (smsIndexEnd != SUBSTRING_NOT_FOUND)
      {
        while (ready() == 0);

        sendf("AT+CMGD=%s", _incomingBuffer.substring(0, smsIndexEnd).c_str());

        if (_smsData.synch)
        {
          waitForResponse(55000);
        }
      }
    }

    /////////////////////////////////////////

    void clear(NB_SMS_Data& _smsData, int flag)
    {
      _smsData.ptrUTF8 = "";

      while (ready() == 0);

      if (flag < 1 || flag > 4)
        flag = 2;

      sendf("AT+CMGD=0,%d", flag);

      if (_smsData.synch)
      {
        waitForResponse(55000);
      }
    }


    ///////////////////////////////////////////////////////////////////////
    //From utility/NBSSLClient_Generic_Impl.hpp

    void loadRootCert(NBRootCert _RCs)
    {
      // load the next root cert
      sendf("AT+USECMNG=0,0,\"%s\",%d", _RCs.name, _RCs.size);
    }

    void removeRootCert(NBRootCert _RCs)
    {
      // remove the next root cert name
      sendf("AT+USECMNG=2,0,\"%s\"", _RCs.name);
    }

    ///////////////////////////////////////////////////////////////////////
    //From utility/NBSocketBuffer_Generic_Impl.hpp

    void readSocketData(int socket, int size)
    {
      sendf("AT+USORD=%d,%d", socket, size);
    }

    /////////////////////////////////////////

    int availableSocketBuffer(int socket, SocketBufferList& buffers)
    {
      if (buffers.length == 0)
      {
        if (buffers.data == NULL)
        {
          buffers.data = buffers.head = (uint8_t*) malloc(NB_SOCKET_BUFFER_SIZE);
          buffers.length = 0;
        }

        String response;

        //Read Socket Data +USORD
        readSocketData(socket, NB_SOCKET_BUFFER_SIZE);

        int status = waitForResponse(10000, &response);

        if (status != NB_RESPONSE_OK)
        {
          return -1;
        }

        if (status != NB_RESPONSE_OK)
        {
          if (status == NB_RESPONSE_ERROR)
          {
            return -1;
          }
          else if (status == NB_RESPONSE_CME_ERROR && response.indexOf("Operation not allowed") != SUBSTRING_NOT_FOUND )
          {
            return -1;
          }
          else
          {
            return 0;
          }
        }

        if (!response.startsWith(MODEM_READ_SOCKET_RESPONSE_START))
        {
          return 0;
        }

        int firstQuoteIndex = response.indexOf("\"");

        response.remove(0, firstQuoteIndex + 1);
        response.remove(response.length() - 1);

        size_t size = response.length() / 2;

        for (size_t i = 0; i < size; i++)
        {
          byte n1 = response[i * 2];
          byte n2 = response[i * 2 + 1];

          n1 = charToInt(n1);
          n2 = charToInt(n2);

          buffers.data[i] = (n1 << 4) | n2;
        }

        buffers.head = buffers.data;
        buffers.length = size;
      }

      return buffers.length;
    }

    ///////////////////////////////////////////////////////////////////////
    //From NBModem_Generic_Impl.hpp

    String getIMEI()
    {
      String imei;

      imei.reserve(15);

      send("AT+CGSN");
      waitForResponse(100, &imei);

      return imei;
    }

    /////////////////////////////////////////

    String getICCID()
    {
      String iccid;

      iccid.reserve(7 + 20);

      send("AT+CCID");
      waitForResponse(1000, &iccid);

      if (iccid.startsWith(MODEM_GET_CCID_RESPONSE_START))
      {
        iccid.remove(0, 7);
      }
      else
      {
        iccid = "";
      }

      return iccid;
    }

    ///////////////////////////////////////////////////////////////////////

  private:

    Uart* _uart;
    unsigned long _baud;
    int _resetPin;
    int _powerOnPin;
    int _vIntPin;
    unsigned long _lastResponseOrUrcMillis;

    enum
    {
      AT_COMMAND_IDLE,
      AT_RECEIVING_RESPONSE
    } _atCommandState;

    int _ready;

    String _buffer;
    String* _responseDataStorage;


    static ModemUrcHandler* _urcHandlers[MAX_URC_HANDLERS];
    static Print* _debugPrint;
};

//////////////////////////////////////////////////////

#if (UBLOX_USING_POWER_ON_PIN && UBLOX_USING_RESET_PIN)
  ModemClass MODEM(SerialNB, 115200, NB_RESETN, NB_PWR, SARA_VINT);
#elif UBLOX_USING_POWER_ON_PIN
  ModemClass MODEM(SerialNB, 115200, -1, NB_PWR, SARA_VINT);
#elif UBLOX_USING_RESET_PIN
  ModemClass MODEM(SerialNB, 115200, NB_RESETN);
#else
  ModemClass MODEM(SerialNB, 115200);
#endif

ModemUrcHandler* ModemClass::_urcHandlers[MAX_URC_HANDLERS] = { NULL };
Print* ModemClass::_debugPrint = NULL;

#include "Modem_SaraR4_Extra_Generic.hpp"

#endif    // _MODEM_SARA_R4_INCLUDED_H
