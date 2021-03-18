/****************************************************************************************************************************
  BlynkSimpleNB_Generic.h - Dead simple Blynk Client for NB/GPRS shields

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
/**
   Based on and modified from
   
   @file       BlynkSimpleNB_Generic.h
   @author     Riccardo Rizzo
   @license    This project is released under the MIT License (MIT)
   @date       Dec 2018
   @brief
*/

// To be copied into Blynk library

#ifndef BlynkSimpleNB_Generic_h
#define BlynkSimpleNB_Generic_h

#ifndef BLYNK_INFO_CONNECTION
  #define BLYNK_INFO_CONNECTION  "NB_GENERIC"
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION  "MKRNB"
#endif

#ifndef BLYNK_HEARTBEAT
#define BLYNK_HEARTBEAT 60
#endif

#ifndef BLYNK_TIMEOUT_MS
#define BLYNK_TIMEOUT_MS 6000
#endif

#define BLYNK_SEND_ATOMIC

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>

class BlynkSIM
  : public BlynkProtocol<BlynkArduinoClient>
{
    typedef BlynkProtocol<BlynkArduinoClient> Base;
  public:
    BlynkSIM(BlynkArduinoClient& transp)
      : Base(transp)
    {}

    bool connectNetwork(NB& modem, GPRS& gprs, const char* pin , const char* apn, const char* user, const char* pass)
    {
      BLYNK_LOG1(BLYNK_F("Modem init..."));

      if (!modem.begin(pin))
      {
        BLYNK_FATAL(BLYNK_F("Cannot init"));
        return false;
      }

      BLYNK_LOG3(BLYNK_F("Connecting to "), apn, BLYNK_F(" ..."));

      if (!gprs.attachGPRS())
      {
        BLYNK_FATAL(BLYNK_F("Connect GPRS failed"));
        return false;
      }

      BLYNK_LOG1(BLYNK_F("Connected to GPRS"));
      return true;
    }

    void config(NB&         nb,
                GPRS&       gprs,
                NBClient&   nbclient,
                const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
      Base::begin(auth);
      modem = &nb;
      modemGPRS = &gprs;

      this->conn.setClient(&nbclient);
      this->conn.begin(domain, port);
    }

    void begin(const char* auth,
               NB&         nb,
               GPRS&       gprs,
               NBClient&   nbclient,
               const char* pin,
               const char* apn,
               const char* user,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT)
    {
      config(nb, gprs, nbclient, auth, domain, port);
      connectNetwork(nb, gprs, pin, apn, user, pass);
      while (this->connect() != true) {}
    }

  private:

    NB*        modem;
    GPRS*      modemGPRS;
    NBClient*  nbclient;
};

static BlynkArduinoClient _blynkTransport;
BlynkSIM Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
