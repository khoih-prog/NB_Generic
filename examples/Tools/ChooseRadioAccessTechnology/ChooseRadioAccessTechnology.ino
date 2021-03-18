/*********************************************************************************************************************************
  ChooseRadioAccessTechnology.ino

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
  Radio Access Technology selection for Arduino MKR NB 1500

  This sketch allows you to select your preferred 4G
  Narrowband Radio Access Technology (RAT).

  You can choose among CAT-M1, NB-IoT or a combination of both.

  Selecting JUST ONE technology will speed up the modem's network
  registration A LOT!

  The chosen configuration will be saved to modem's internal memory
  and will be preserved through MKR NB 1500 sketch uploads.

  In order to change the RAT, you will need to run this sketch again.

  Circuit:
  - MKR NB 1500 board
  - Antenna
  - SIM card

  Created 26 November 2018
  by Giampaolo Mancini

*/

#include "defines.h"

bool setRAT(String choice)
{
  String response;

  Serial.print("Disconnecting from network: ");

  MODEM.sendf("AT+COPS=2");
  MODEM.waitForResponse(2000);

  Serial.println("done.");

  Serial.print("Setting Radio Access Technology: ");

  MODEM.sendf("AT+URAT=%s", choice.c_str());
  MODEM.waitForResponse(2000, &response);

  Serial.println("done.");

  return true;
}

bool apply()
{
  Serial.print("Applying changes and saving configuration: ");
  
  MODEM.send("AT+CFUN=15");
  MODEM.waitForResponse(5000);
  
  delay(5000);

  do
  {
    delay(1000);
    MODEM.noop();
  } while (MODEM.waitForResponse(1000) != 1);

  Serial.println("done.");

  return true;
}

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ChooseRadioAccessTechnology on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif

  MODEM.begin();

  while (!MODEM.noop());

  for (int i = 0; i < 80; i++)
    Serial.print("*");

  Serial.println();
  Serial.println("This sketch allows you to select your preferred");
  Serial.println("4G Narrowband Radio Access Technology (RAT).");
  Serial.println();
  Serial.println("You can choose among CAT-M1, NB-IoT or a combination of both.");
  Serial.println();
  Serial.println("Selecting JUST ONE technology will speed up the modem's network registration A LOT!");
  Serial.println();
  Serial.println("The chosen configuration will be saved to modem's internal memory");
  Serial.println("and will be preserved through MKR NB 1500 sketch uploads.");
  Serial.println();
  Serial.println("In order to change the RAT, you will need to run this sketch again.");

  for (int i = 0; i < 80; i++)
    Serial.print("*");

  Serial.println();
  Serial.println();
  Serial.println("Please choose your Radio Access Technology:");
  Serial.println();
  Serial.println("    0 - CAT M1 only");
  Serial.println("    1 - NB IoT only");
  Serial.println("    2 - CAT M1 preferred, NB IoT as failover (default)");
  Serial.println("    3 - NB IoT preferred, CAT M1 as failover");
  Serial.println();
}

void loop()
{
  String uratChoice;

  Serial.print("> ");

  Serial.setTimeout(-1);

  while (Serial.available() == 0);

  String uratInput = Serial.readStringUntil('\n');
  uratInput.trim();
  int urat = uratInput.toInt();
  Serial.println(urat);

  switch (urat)
  {
    case 0:
      uratChoice = "7";
      break;
    case 1:
      uratChoice = "8";
      break;
    case 2:
      uratChoice = "7,8";
      break;
    case 3:
      uratChoice = "8,7";
      break;
    default:
      Serial.println("Invalid input. Please, retry.");
      return;
  }

  setRAT(uratChoice);
  apply();

  Serial.println();
  Serial.println("Radio Access Technology selected.");
  Serial.println("Now you can upload your 4G application sketch.");

  while (true);
}
