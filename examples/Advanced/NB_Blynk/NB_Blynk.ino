/****************************************************************************************************************************
  NB_Blynk.ino - Dead simple Blynk Client for NB/GPRS shields

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
/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************/

#include "defines.h"

#define USING_NON_BLOCK_BLYNK_CONNECT     true

//////////////////////////////////////////////////////

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

//////////////////////////////////////////////////////

NBClient nbClient;
GPRS gprs;
NB nbAccess;

// connection state
bool connected = false;

bool NB_CONNECT_OK = false;

// BaudRate to communicate to NB/GPRS modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

//////////////////////////////////////////////////////

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void heartBeatPrint()
{
  static int num = 1;

  if (Blynk.connected())
  {
    Serial.print(F("B"));
  }
  else
  {
    Serial.print(F("F"));
  }

  if (num == 40)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

#define STATUS_CHECK_INTERVAL     10000L

  // Send status report every STATUS_REPORT_INTERVAL (60) seconds: we don't need to send updates frequently if there is no status change.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    // report status to Blynk
    heartBeatPrint();

    checkstatus_timeout = millis() + STATUS_CHECK_INTERVAL;
  }
}

bool connectToGPRS()
{
  #define NUMBER_OF_CONNECTION_TRIES      10

  uint8_t tryNumber = 1;
  
  // After starting the modem with NB.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while ((nbAccess.begin(baudRateSerialNB, PINNUMBER) != NB_READY) || (gprs.attachGPRS() != GPRS_READY)) 
  {
    // failed, retry
    Serial.print("Connect GPRS failed, try = "); Serial.println(tryNumber);

    if (++tryNumber > NUMBER_OF_CONNECTION_TRIES)
    {
      Serial.println("\nCan't connected to GPRS.");

      connected = false;
      
      return false;
    }
    
    delay(1000);
  }

  connected = true;

  //Get IP.
  IPAddress LocalIP = gprs.getIPAddress();
  Serial.print("\nConnected to GPRS. IP address = ");
  Serial.println(LocalIP);

  return true;
}

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStart NB_Blynk on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 5) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif  

#if USING_NON_BLOCK_BLYNK_CONNECT
  Blynk.config(nbAccess, gprs, nbClient, auth, blynk_server, BLYNK_HARDWARE_PORT);
  
  NB_CONNECT_OK = connectToGPRS();

  if (NB_CONNECT_OK)
  {
    Serial.println(F("NB_CONNECT_OK. Connecting to Blynk"));
    
    Blynk.connect();
  }
  else
  {
    Serial.println(F("NB_CONNECT_ERROR"));
  }
   
#else
  Blynk.begin(auth, nbAccess, gprs, nbClient, PINNUMBER, GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD);
#endif

  
   Serial.println(F("Checking Blynk now"));

  if (Blynk.connected())
  {
    Serial.println(F("Blynk connected OK"));
  }
  else
  {
    Serial.println(F("Blynk connected error"));
  }
}

void loop()
{
  if (nbAccess.status() != NB_READY || gprs.status() != GPRS_READY) 
  {
    connectToGPRS();
  }

  if (connected)
  {
    Blynk.run();
  }
  else
  {
    delay(1000);
  }

  check_status();
  Serial.print(F("B"));
}
