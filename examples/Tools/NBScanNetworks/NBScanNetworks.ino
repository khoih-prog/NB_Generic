/*********************************************************************************************************************************
  NBScanNetworks.ino

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
 
/*

 NB Scan Networks

 This example prints out the IMEI number of the modem,
 then checks to see if it's connected to a carrier.
 Then it scans for nearby networks and prints out their signal strengths.

 Circuit:
 * MKR NB 1500 board
 * Antenna
 * SIM card

 Created 8 Mar 2012
 by Tom Igoe, implemented by Javier Carazo
 Modified 4 Feb 2013
 by Scott Fitzgerald
*/

#include "defines.h"

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[] = SECRET_PINNUMBER;

// initialize the library instance
NB nbAccess;     // include a 'true' parameter to enable debugging
NBScanner scannerNetworks;
NBModem modemTest;

// BaudRate to communicate to NB-IoT modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

// Save data variables
String IMEI = "";

// serial monitor result messages
String errortext = "ERROR";

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting NBScanNetworks on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif

  Serial.println("NB IoT/LTE Cat M1 networks scanner");
  scannerNetworks.begin();

  // connection state
  boolean connected = false;

  // Start NB connection
  while (!connected)
  {
    if (nbAccess.begin(baudRateSerialNB, PINNUMBER) == NB_READY)
    {
      connected = true;
    }
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  // get modem parameters
  // IMEI, modem unique identifier
  Serial.print("Modem IMEI: ");
  
  IMEI = modemTest.getIMEI();
  IMEI.replace("\n", "");
  
  if (IMEI != NULL) 
  {
    Serial.println(IMEI);
  }
}

void loop() 
{
  // currently connected carrier
  Serial.print("Current carrier: ");
  Serial.println(scannerNetworks.getCurrentCarrier());

  // returns strength and BER
  // signal strength in 0-31 scale. 31 means power > 51dBm
  // BER is the Bit Error Rate. 0-7 scale. 99=not detectable
  Serial.print("Signal Strength: ");
  Serial.print(scannerNetworks.getSignalStrength());
  Serial.println(" [0-31]");

  // scan for existing networks, displays a list of networks
  Serial.println("Scanning available networks. May take some seconds.");
  Serial.println(scannerNetworks.readNetworks());
  // wait ten seconds before scanning again
  delay(10000);
}
