/*********************************************************************************************************************************
  NBSendSMS.ino

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
  SMS sender

  This sketch, for the MKR NB 1500 board, sends an SMS message
  you enter in the serial monitor. Connect your Arduino with the
  SIM card, open the serial monitor, and wait for
  the "READY" message to appear in the monitor. Next, type a
  message to send and press "return". Make sure the serial
  monitor is set to send a newline when you press return.

  Circuit:
   MKR NB 1500 board
   Antenna
   SIM card that can send SMS

  created 25 Feb 2012
  by Tom Igoe
*/

#include "defines.h"

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

// initialize the library instance
NB nbAccess;
NB_SMS sms;

// BaudRate to communicate to NB-IoT modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

/*
  Read input serial
*/
int readSerial(char result[]) 
{
  int i = 0;
  
  while (1) 
  {
    while (Serial.available() > 0) 
    {
      char inChar = Serial.read();
      
      if (inChar == '\n') 
      {
        result[i] = '\0';
        Serial.flush();
        return 0;
      }
      
      if (inChar != '\r') 
      {
        result[i] = inChar;
        i++;
      }
    }
  }
}

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

  // connection state
  bool connected = false;

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

  Serial.println("NB initialized");
}

void loop() 
{
  Serial.print("Enter a mobile number: ");
  char remoteNum[20];  // telephone number to send SMS
  readSerial(remoteNum);
  Serial.println(remoteNum);

  // SMS text
  Serial.print("Now, enter SMS content: ");
  char txtMsg[200];
  readSerial(txtMsg);
  Serial.println("SENDING");
  Serial.println();
  Serial.println("Message:");
  Serial.println(txtMsg);

  // send the message
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");
}
