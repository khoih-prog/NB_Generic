/*********************************************************************************************************************************
  TestGPRS.ino

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

  This sketch tests the MKR NB 1500 board's ability to connect to a
  GPRS network. It asks for APN information through the
  serial monitor and tries to connect to example.org.

  Circuit:
   MKR NB 1500 board
   Antenna
   SIM card with data plan

  Created 18 Jun 2012
  by David del Peral
*/

#include "defines.h"

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

// initialize the library instance
NB nbAccess;        // NB access: include a 'true' parameter for debug enabled
GPRS gprsAccess;    // GPRS access
NBClient client;    // Client service for TCP connection

// messages for serial monitor response
String oktext     = "OK";
String errortext  = "ERROR";

// URL and path (for example: example.org)
char url[]      = "example.org";
char urlproxy[] = "http://example.org";
char path[]     = "/";

// variable to save obtained response
String response = "";

// use a proxy
boolean use_proxy = false;

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

  Serial.print(F("\nStarting TestGPRS on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 4) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif
}

void loop()
{
  use_proxy = false;

  // start module
  // if your SIM has PIN, pass it as a parameter of begin() in quotes
  Serial.print("Connecting NB IoT / LTE Cat M1 network...");

  if (nbAccess.begin(PINNUMBER) != NB_READY)
  {
    Serial.println(errortext);
    while (true);
  }

  Serial.println(oktext);

  // attach GPRS

  if (gprsAccess.attachGPRS() != GPRS_READY)
  {
    Serial.println(errortext);
  }
  else
  {
    Serial.println(oktext);

    // read proxy introduced by user
    char proxy[100];
    Serial.print("If your carrier uses a proxy, enter it, if not press enter: ");
    readSerial(proxy);
    Serial.println(proxy);

    // if user introduced a proxy, asks them for proxy port
    int pport;

    if (proxy[0] != '\0')
    {
      // read proxy port introduced by user
      char proxyport[10];
      Serial.print("Enter the proxy port: ");
      readSerial(proxyport);
      // cast proxy port introduced to integer
      pport = (int) proxyport;
      use_proxy = true;
      Serial.println(proxyport);
    }

    // connection with example.org and realize HTTP request
    Serial.print("Connecting and sending GET request to example.org...");
    int res_connect;

    // if use a proxy, connect with it
    if (use_proxy)
    {
      res_connect = client.connect(proxy, pport);
    }
    else
    {
      res_connect = client.connect(url, 80);
    }

    if (res_connect)
    {
      // make a HTTP 1.0 GET request (client sends the request)
      client.print("GET ");

      // if using a proxy, the path is example.org URL
      if (use_proxy)
      {
        client.print(urlproxy);
      }
      else
      {
        client.print(path);
      }

      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(url);
      client.println("Connection: close");
      client.println();
      Serial.println(oktext);
    }
    else
    {
      // if you didn't get a connection to the server
      Serial.println(errortext);
    }

    Serial.print("Receiving response...");

    boolean test = true;

    while (test)
    {
      // if there are incoming bytes available
      // from the server, read and check them
      if (client.available())
      {
        char c = client.read();
        response += c;

        // cast response obtained from string to char array
        char responsechar[response.length() + 1];
        response.toCharArray(responsechar, response.length() + 1);

        // if response includes a "200 OK" substring
        if (strstr(responsechar, "200 OK") != NULL)
        {
          Serial.println(oktext);
          Serial.println("TEST COMPLETE!");
          test = false;
        }
      }

      // if the server's disconnected, stop the client:
      if (!client.connected())
      {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        test = false;
      }
    }
  }
}
