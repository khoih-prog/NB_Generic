/*********************************************************************************************************************************
  TestModem.ino

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
 
/*

 This example tests to see if the modem of the
 MKR NB 1500 board is working correctly. You do not need
 a SIM card for this example.

 Circuit:
 * MKR NB 1500 board
 * Antenna

 Created 12 Jun 2012
 by David del Peral
 modified 21 Nov 2012
 by Tom Igoe
*/

#include "defines.h"

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

// modem verification object
NBModem modem;

// BaudRate to communicate to NB-IoT modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

// IMEI variable
String IMEI = "";

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting NBSendSMS on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif

  // start modem test (reset and check response)
  Serial.print("Starting modem test...");
  
  if (modem.begin(baudRateSerialNB)) 
  {
    Serial.println("modem.begin() succeeded");
  } 
  else 
  {
    Serial.println("ERROR, no modem answer.");
  }
}


void loop() {
  // get modem IMEI
  Serial.print("Checking IMEI...");
  IMEI = modem.getIMEI();

  // check IMEI response
  if (IMEI != NULL) 
  {
    // show IMEI in serial monitor
    Serial.println("Modem's IMEI: " + IMEI);
    // reset modem to check booting:
    Serial.print("Resetting modem...");
    modem.begin();
    
    // get and check IMEI one more time
    if (modem.getIMEI() != NULL) 
    {
      Serial.println("Modem is functioning properly");
    } 
    else 
    {
      Serial.println("Error: getIMEI() failed after modem.begin()");
    }
  } 
  else 
  {
    Serial.println("Error: Could not get IMEI");
  }
  
  // do nothing:
  while (true) { delay(1); }
}
