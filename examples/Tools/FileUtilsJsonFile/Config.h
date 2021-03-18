/*********************************************************************************************************************************
  Config.h

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
 **********************************************************************************************************************************/

#pragma once

#ifndef Config_h
#define Config_h

#include <Arduino_JSON.h>


// Make the struct Printable to allow Serial.print-ing
struct Config : public Printable
{
    String deviceId;
    int timestamp;

    String toJson()
    {
      JSONVar conf;

      conf["deviceId"] = deviceId;
      conf["timestamp"] = timestamp;

      return JSON.stringify(conf);
    }

    bool fromJSON(const String buf)
    {
      auto conf = JSON.parse(buf);

      if (!checkProperty(conf, "deviceId", "string"))
        return false;

      if (!checkProperty(conf, "timestamp", "number"))
        return false;

      deviceId = conf["deviceId"];
      timestamp = int { conf["timestamp"] };

      return true;
    }

    virtual size_t printTo(Print& p) const
    {
      size_t written { 0 };

      written += p.print("DeviceId:         ");
      written += p.println(deviceId);
      written += p.print("Timestamp:        ");
      written += p.println(timestamp);

      return written;
    }

  private:
    bool checkProperty(JSONVar json, String property, String type)
    {
      if (!json.hasOwnProperty(property)) 
      {
        Serial.print(property);
        Serial.println(" not found");
        return false;
      }

      if (JSON.typeof(json[property]) != type) 
      {
        Serial.print(property);
        Serial.print(" != \"");
        Serial.print(type);
        Serial.println("\"");
        return false;
      }

      return true;
    }
};

#endif    // Config_h
