/*********************************************************************************************************************************
  FileUtilsJsonFile.ino

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
  Manage JSON configurations via NB module filesystem.

  This sketch demonstrates how to use the internal filesystem
  of the NB module to store and retrieve data and configurations
  both as JSON objects or C++ structs.

  Circuit:
    MKR NB 1500 board

  created 19 June 2020
  by Giampaolo Mancini
*/

#define DEBUG_NB_GENERIC_PORT       Serial

// Debug Level from 0 to 5. Level 5 is to print out AT commands and responses
#define _NB_GENERIC_LOGLEVEL_       5

#include <Arduino_JSON.h>

//////////////////////////////////////////////

#if defined(ARDUINO_SAMD_MKRNB1500)
  #define SerialNB      SerialSARA
  #define NB_RESETN     SARA_RESETN
  #define NB_PWR        SARA_PWR_ON
#else
  // Override the default (and certainly not good) pins and port
  // Only for boards other than ARDUINO_SAMD_MKRNB1500
  #define NB_RESETN  (10u)
  #define NB_PWR     (11u)

  #if ESP8266
    // Using Software Serial for ESP8266, as Serial1 is TX only
    #define NB_USING_SOFTWARE_SERIAL     true
  #else
    // Optional Software Serial here for other boards, but not advised if HW Serial available
    #define NB_USING_SOFTWARE_SERIAL     false
  #endif
   
  #if NB_USING_SOFTWARE_SERIAL
    #warning Using default SerialNB = SoftwareSerial
    
    #define D8 (15)
    #define D7 (13)
    
    #include <SoftwareSerial.h>
    
    SoftwareSerial swSerial(D7, D8);    // (D7, D8, false, 256); // (RX, TX, false, 256);
    
    #define SerialNB   swSerial
  #else
    #warning Using default SerialNB = HardwareSerial Serial1
    #define SerialNB   Serial1
  #endif    // NB_USING_SOFTWARE_SERIAL

  #warning You must connect the Modem correctly and modify the pins / Serial port here
  
#endif

//////////////////////////////////////////////

// Optional usage of NB_RESETN and NB_DTR. Need to be here only when true. Default is false

//#define UBLOX_USING_RESET_PIN             true
//#define UBLOX_USING_POWER_ON_PIN          true

//////////////////////////////////////////////

#define NB_MODEM_SARAR4                     true

//////////////////////////////////////////////
// Not supported yet
#define NB_MODEM_SIM7000                    false
#define NB_MODEM_XBEE                       false
#define NB_MODEM_SEQUANS_MONARCH            false

//////////////////////////////////////////////


// BaudRate to communicate to NB/GPRS modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

// libraries
#include <NB_Generic_Main.h>

#include "Config.h"

NBFileUtils fileUtils;


void simpleDemo()
{

  Serial.println();
  Serial.println("========================");
  Serial.println("Running simple JSON demo");
  Serial.println();

  JSONVar myObject;

  myObject["hello"] = "world";
  myObject["true"] = true;
  myObject["x"] = 42;

  String jsonString = JSON.stringify(myObject);

  Serial.println("Saving JSON file (test.json): ");
  Serial.println(jsonString);
  Serial.println();
  
  fileUtils.downloadFile("test.json", jsonString);

  printFiles(fileUtils);
  Serial.println();

  String jsonData;
  Serial.println("Reading JSON file (test.json): ");
  
  fileUtils.readFile("test.json", &jsonData);

  Serial.println("File contents:");
  Serial.println(jsonData);
  Serial.println();

  Serial.println("Parsing JSON contents:");
  
  JSONVar myConf = JSON.parse(jsonData);
  Serial.print("myConf[\"hello\"]: ");
  Serial.println(myConf["hello"]);
  Serial.print("myConf[\"true\"]: ");
  Serial.println(myConf["true"]);
  Serial.print("myConf[\"x\"]: ");
  Serial.println(myConf["x"]);
}

void structDemo()
{
  NBModem modem;

  Serial.println();
  Serial.println("========================");
  Serial.println("Running Configuration via struct and JSON demo");
  Serial.println();
  Serial.println("Creating configuration struct:");

  Config conf;
  conf.deviceId = modem.getICCID();
  conf.timestamp = millis();

  Serial.print(conf);

  fileUtils.downloadFile("conf.json", conf.toJson());

  Serial.println();
  printFiles(fileUtils);
  Serial.println();

  Serial.println("Reading configuration file:");

  String jsonConf;
  
  fileUtils.readFile("conf.json", &jsonConf);
  Serial.println(jsonConf);
  Serial.println();

  Serial.println("Reading configuration struct:");
  Config newConf;
  newConf.fromJSON(jsonConf);

  Serial.print(newConf);
}

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting FileUtilsJsonFile on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif  

  Serial.println("Store and Retrieve JSON data to NB module storage.");
  Serial.println();

  fileUtils.begin(baudRateSerialNB);

  simpleDemo();
  structDemo();

  while (true);
}

void loop()
{
}
