/*********************************************************************************************************************************
  NBSSLClient_Generic_Impl.hpp
  
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

#ifndef _NB_SSL_CLIENT_GENERIC_IMPL_H_INCLUDED
#define _NB_SSL_CLIENT_GENERIC_IMPL_H_INCLUDED

bool NBSSLClient::_defaultRootCertsLoaded = false;

NBSSLClient::NBSSLClient(bool synch) :
  NBClient(synch),
  _RCs(NB_ROOT_CERTS),
  _numRCs(NB_NUM_ROOT_CERTS),
  _customRootCerts(false)
{
}

NBSSLClient::NBSSLClient(const NBRootCert* myRCs, int myNumRCs, bool synch) :
  NBClient(synch),
  _RCs(myRCs),
  _numRCs(myNumRCs),
  _customRootCerts(true),
  _customRootCertsLoaded(false)
{
}

NBSSLClient::~NBSSLClient()
{
}

int NBSSLClient::ready()
{
  if ((!_customRootCerts && _defaultRootCertsLoaded) ||
      (_customRootCerts && (_numRCs == 0 || _customRootCertsLoaded)))
  {
    // root certs loaded already, continue to regular NBClient
    return NBClient::ready();
  }

  int ready = MODEM.ready();

  if (ready == 0)
  {
    // a command is still running
    return 0;
  }

  switch (_state)
  {
    case SSL_CLIENT_STATE_LOAD_ROOT_CERT:
      {
        if (_RCs[_certIndex].size)
        {
          // load the next root cert
          MODEM.loadRootCert(_RCs[_certIndex]);

          if (MODEM.waitForPrompt() != 1)
          {
            // failure
            ready = -1;
          }
          else
          {
            // send the cert contents
            MODEM.write(_RCs[_certIndex].data, _RCs[_certIndex].size);
            
            _state = SSL_CLIENT_STATE_WAIT_LOAD_ROOT_CERT_RESPONSE;
            ready = 0;
          }
        }
        else
        {
          // remove the next root cert name
          MODEM.removeRootCert(_RCs[_certIndex]);

          _state = SSL_CLIENT_STATE_WAIT_DELETE_ROOT_CERT_RESPONSE;
          ready = 0;
        }

        break;
      }

    case SSL_CLIENT_STATE_WAIT_LOAD_ROOT_CERT_RESPONSE:
      {
        if (ready > 1)
        {
          // error
        }
        else
        {
          ready = iterateCerts();
        }

        break;
      }

    case SSL_CLIENT_STATE_WAIT_DELETE_ROOT_CERT_RESPONSE:
      {
        // ignore ready response, root cert might not exist
        ready = iterateCerts();

        break;
      }
  }

  return ready;
}

int NBSSLClient::connect(IPAddress ip, uint16_t port)
{
  _certIndex = 0;
  _state = SSL_CLIENT_STATE_LOAD_ROOT_CERT;

  return connectSSL(ip, port);
}

int NBSSLClient::connect(const char* host, uint16_t port)
{
  _certIndex = 0;
  _state = SSL_CLIENT_STATE_LOAD_ROOT_CERT;

  return connectSSL(host, port);
}

int NBSSLClient::iterateCerts()
{
  _certIndex++;

  if (_certIndex == _numRCs)
  {
    // all certs loaded
    if (_customRootCerts)
    {
      _customRootCertsLoaded = true;
    }
    else
    {
      _defaultRootCertsLoaded = true;
    }
  }
  else
  {
    // load next
    _state = SSL_CLIENT_STATE_LOAD_ROOT_CERT;
  }

  return 0;
}

#endif    // _NB_SSL_CLIENT_GENERIC_IMPL_H_INCLUDED
