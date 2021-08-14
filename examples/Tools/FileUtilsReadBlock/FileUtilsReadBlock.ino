/*********************************************************************************************************************************
  FileUtilsReadBlock.ino

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
/*
  Read large files block-by-block from the NB module filesystem.

  This sketch allows you to read large (max 2.5MB) files from the module's internal filesystem using successive block-by-block reads.

  The contents of the file is printed to Serial port as an hexadecimal string which can be later converted to the original
  content using an external tools, such as 'xxd', eg.

    'xxd -p -r sketck_output.txt data.bin'

  Circuit:
  - NB module

  Created 19 June 2020
  by Giampaolo Mancini

*/

#define DEBUG_NB_GENERIC_PORT       Serial

// Debug Level from 0 to 5. Level 5 is to print out AT commands and responses
#define _NB_GENERIC_LOGLEVEL_       5

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

NBFileUtils fileUtils(false);

// An existing file
constexpr char filename[] = "update.bin";

// Read bloack size
constexpr unsigned int blockSize = 512;

void setup()
{
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting FileUtilsReadBlock on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif  

  fileUtils.begin(baudRateSerialNB);

  uint32_t size = fileUtils.listFile(filename);
  uint32_t cycles = (size / blockSize) + 1;

  uint32_t totalRead = 0;

  for (uint32_t i = 0; i < cycles; i++) 
  {
    uint8_t block[blockSize] = { 0 };
    
    uint32_t read = fileUtils.readBlock(filename, i * blockSize, blockSize, block);
    totalRead += read;
    
    for (uint32_t j = 0; j < read; j++) 
    {
      if (block[j] < 16)
        Serial.print(0);
        
      Serial.print(block[j], HEX);
    }
    
    Serial.println();
  }

  if (totalRead != size) 
  {
    Serial.print("ERROR - File size: ");
    Serial.print(size);
    Serial.print(" Bytes read: ");
    Serial.println(totalRead);
  }
}

void loop()
{
}
