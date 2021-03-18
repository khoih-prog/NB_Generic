/*********************************************************************************************************************************
  SerialNBAPassthrough.ino

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
 
/*
   SerialSARAPassthrough sketch

   This sketch allows you to send AT commands from the USB CDC serial port
   of the MKR NB 1500 board to the onboard ublox SARA-R410 cellular module.

   For a list of supported AT commands see:
   https://www.u-blox.com/sites/default/files/u-blox-CEL_ATCommands_%28UBX-13002752%29.pdf

   Circuit:
   - MKR NB 1500 board
   - Antenna
   - SIM card

   Make sure the Serial Monitor's line ending is set to "Both NL and CR" or "Carriage Return"

   created 11 December 2017
   Sandeep Mistry
*/

// baud rate used for both Serial ports
unsigned long baud = 115200;

#include "defines.h"

void setup() 
{
  // NEVER EVER use RESET_N
  pinMode(NB_RESETN, OUTPUT);
  digitalWrite(NB_RESETN, LOW);

  // Send Poweron pulse
  pinMode(NB_PWR, OUTPUT);
  digitalWrite(NB_PWR, HIGH);
  delay(150);
  digitalWrite(NB_PWR, LOW);

  Serial.begin(baud);
  SerialNB.begin(baud);
}

void loop() 
{
  if (Serial.available()) 
  {
    SerialNB.write(Serial.read());
  }

  if (SerialNB.available()) 
  {
    Serial.write(SerialNB.read());
  }
}
