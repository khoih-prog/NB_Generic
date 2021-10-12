/****************************************************************************************************************************
  NB_MQTT.ino - Dead simple MQTT Client for NB/GPRS shields

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
/****************************************************************************************************************************
  You have to modify file ./libraries/Adafruit_MQTT_Library/Adafruit_MQTT.cpp as follows to avoid dtostrf error, if exists
   
  #ifdef __cplusplus
    extern "C" {
  #endif
  extern char* itoa(int value, char *string, int radix);
  extern char* ltoa(long value, char *string, int radix);
  extern char* utoa(unsigned value, char *string, int radix);
  extern char* ultoa(unsigned long value, char *string, int radix);
  #ifdef __cplusplus
    } // extern "C"
  #endif
  //#if defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_ARCH_SAMD)
  #if !( ESP32 || ESP8266 || defined(CORE_TEENSY) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3) || defined(STM32F4) || defined(STM32F7) || \
       ( defined(ARDUINO_ARCH_RP2040) && !defined(ARDUINO_ARCH_MBED) ) || ARDUINO_ARCH_SEEED_SAMD || ( defined(SEEED_WIO_TERMINAL) || defined(SEEED_XIAO_M0) || \
         defined(SEEED_FEMTO_M0) || defined(Wio_Lite_MG126) || defined(WIO_GPS_BOARD) || defined(SEEEDUINO_ZERO) || defined(SEEEDUINO_LORAWAN) || defined(WIO_LTE_CAT) || \
         defined(SEEED_GROVE_UI_WIRELESS) ) ) 
  static char *dtostrf(double val, signed char width, unsigned char prec, char *sout)
  {
    char fmt[20];
    sprintf(fmt, "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
  }
  #endif
 *****************************************************************************************************************************/
 
#include "defines.h"

#define LOCAL_DEBUG       true  //false

//////////////////////////////////////////////////////

// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// PIN Number
const char PINNUMBER[]     = SECRET_PINNUMBER;

//////////////////////////////////////////////////////

#include "Adafruit_MQTT.h"                //https://github.com/adafruit/Adafruit_MQTT_Library
#include "Adafruit_MQTT_Client.h"         //https://github.com/adafruit/Adafruit_MQTT_Library

#define AIO_SERVER_LEN       20
#define AIO_SERVERPORT_LEN   6
#define AIO_USERNAME_LEN     20
#define AIO_KEY_LEN          40

// 
#define AIO_PUB_TOPIC_LEN    40
#define AIO_SUB_TOPIC_LEN    40

char AIO_SERVER     [AIO_SERVER_LEN + 1]        = "io.adafruit.com";
char AIO_SERVERPORT [AIO_SERVERPORT_LEN + 1]    = "1883";     //1883, or 8883 for SSL

char AIO_USERNAME   [AIO_USERNAME_LEN + 1]      = "private";
char AIO_KEY        [AIO_KEY_LEN + 1]           = "private";

char AIO_PUB_TOPIC  [AIO_PUB_TOPIC_LEN + 1]     = "/feeds/Temperature";
char AIO_SUB_TOPIC  [AIO_SUB_TOPIC_LEN + 1]     = "/feeds/LED_Control";

//////////////////////////////////////////////////////

// initialize the library instance
NBClient *client   = NULL;
GPRS gprs;
NB nbAccess;

// connection state
bool connected = false;

// BaudRate to communicate to NB/GPRS modem. If be limit to max 115200 inside modem
unsigned long baudRateSerialNB  = 115200;

//////////////////////////////////////////////////////

Adafruit_MQTT_Client    *mqtt         = NULL;
Adafruit_MQTT_Publish   *Temperature  = NULL;
Adafruit_MQTT_Subscribe *LED_Control  = NULL;

// You have to get from a sensor. Here is just an example
uint32_t measuredTemp = 5;

#ifdef LED_BUILTIN
  #define LED_PIN     LED_BUILTIN
#else
  #define LED_PIN     13
#endif

//////////////////////////////////////////////////////

void heartBeatPrint()
{
  static int num = 1;

  if (connected)
    Serial.print("G");        // W means connected to NB/GPRS
  else
    Serial.print("N");        // N means not connected to NB

  if (num == 40)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 5 == 0)
  {
    Serial.print(" ");
  }
}

void publishMQTT()
{
  MQTT_connect();

  if (Temperature->publish(measuredTemp))
  {
    Serial.print(F("T"));        // T means publishing OK
  }
  else
  {
    Serial.print(F("F"));        // F means publishing failure
  }
}

void subscribeMQTT()
{
  Adafruit_MQTT_Subscribe *subscription;

  MQTT_connect();

  while ((subscription = mqtt->readSubscription(5000)))
  {
    if (subscription == LED_Control)
    {
      Serial.print(F("\nGot: "));
      Serial.println((char *)LED_Control->lastread);

      if (!strcmp((char*) LED_Control->lastread, "ON"))
      {
        digitalWrite(LED_PIN, HIGH);
      }
      else
      {
        digitalWrite(LED_PIN, LOW);
      }
    }
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    60000L
  // Print NB hearbeat, Publish MQTT Topic every HEARTBEAT_INTERVAL (60) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    if (connected)
    {
      // MQTT related jobs
      publishMQTT();
      subscribeMQTT();
    }
    
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}

void deleteOldInstances()
{
  // Delete previous instances
  if (mqtt)
  {
    delete mqtt;
    mqtt = NULL;
    Serial.println(F("Deleting old MQTT object"));
  }

  if (Temperature)
  {
    delete Temperature;
    Temperature = NULL;
    Serial.println(F("Deleting old Temperature object"));
  }
}

#define USE_GLOBAL_TOPIC    true

#if USE_GLOBAL_TOPIC
  String completePubTopic;
  String completeSubTopic;
#endif

void createNewInstances()
{
  if (!client)
  {
    client = new NBClient;

    if (client)
    {
      Serial.println(F("\nCreating new NBClient client object OK"));
    }
    else
      Serial.println(F("\nCreating new NBClient client object failed"));
  }

  // Create new instances from new data
  if (!mqtt)
  {
    // Setup the MQTT client class by passing in the NB client and MQTT server and login details.
    mqtt = new Adafruit_MQTT_Client(client, AIO_SERVER, atoi(AIO_SERVERPORT), AIO_USERNAME, AIO_KEY);

    if (mqtt)
    {
      Serial.println(F("Creating new MQTT object OK"));
      Serial.print(F("AIO_SERVER = ")); Serial.print(AIO_SERVER); Serial.print(F(", AIO_SERVERPORT = ")); Serial.print(AIO_SERVERPORT);
      Serial.print(F("AIO_USERNAME = ")); Serial.print(AIO_USERNAME); Serial.print(F(", AIO_KEY = ")); Serial.print(AIO_KEY);
    }
    else
      Serial.println(F("Creating new MQTT object failed"));
  }

  if (!Temperature)
  {
#if USE_GLOBAL_TOPIC
    completePubTopic = String(AIO_USERNAME) + String(AIO_PUB_TOPIC);
#else
    // Must be static or global
    static String completePubTopic = String(AIO_USERNAME) + String(AIO_PUB_TOPIC);
#endif

    Temperature = new Adafruit_MQTT_Publish(mqtt, completePubTopic.c_str());
    Serial.print(F("Creating new MQTT_Pub_Topic,  Temperature = ")); Serial.println(completePubTopic);

    if (Temperature)
    {
      Serial.println(F("Creating new Temperature object OK"));
      Serial.print(F("Temperature MQTT_Pub_Topic = ")); Serial.println(completePubTopic);
    }
    else
      Serial.println(F("Creating new Temperature object failed"));
  }

  if (!LED_Control)
  {
#if USE_GLOBAL_TOPIC
    completeSubTopic = String(AIO_USERNAME) + String(AIO_SUB_TOPIC);
#else
    // Must be static or global
    static String completeSubTopic = String(AIO_USERNAME) + String(AIO_SUB_TOPIC);
#endif

    LED_Control = new Adafruit_MQTT_Subscribe(mqtt, completeSubTopic.c_str());

    Serial.print(F("Creating new AIO_SUB_TOPIC, LED_Control = ")); Serial.println(completeSubTopic);

    if (LED_Control)
    {
      Serial.println(F("Creating new LED_Control object OK"));
      Serial.print(F("LED_Control AIO_SUB_TOPIC = ")); Serial.println(completeSubTopic);

      mqtt->subscribe(LED_Control);
    }
    else
      Serial.println(F("Creating new LED_Control object failed"));
  }
}

void MQTT_connect()
{
  int8_t ret;

  createNewInstances();

  // Return if already connected
  if (mqtt->connected())
  {
    return;
  }

#if LOCAL_DEBUG
  Serial.println(F("\nConnecting to NB MQTT (3 attempts)..."));
#endif

  uint8_t attempt = 3;

  while ( (ret = mqtt->connect()) )
  {
    // connect will return 0 for connected
    Serial.println(mqtt->connectErrorString(ret));

#if LOCAL_DEBUG
    Serial.println(F("Another attemtpt to connect to MQTT in 5 seconds..."));
#endif

    mqtt->disconnect();
    delay(5000);  // wait 5 seconds
    attempt--;

    if (attempt == 0)
    {
      Serial.println(F("NB MQTT connection failed. Continuing with program..."));
      return;
    }
  }

#if LOCAL_DEBUG
  Serial.println(F("NB MQTT connection successful!"));
#endif
}

void connectToGPRS()
{
  // After starting the modem with NB.begin()
  // attach the shield to the GPRS network with the APN, login and password
  while (!connected) 
  {
    if ((nbAccess.begin(baudRateSerialNB, PINNUMBER) == NB_READY) && (gprs.attachGPRS() == GPRS_READY))
    {
      connected = true;

      //Get IP.
      IPAddress LocalIP = gprs.getIPAddress();
      Serial.print("Connected to GPRS. IP address = ");
      Serial.println(LocalIP);
    } 
    else 
    {
      Serial.print(".");
      delay(1000);
    }
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_PIN, OUTPUT);

  delay(200);

  Serial.print(F("\nStart NB_MQTT on ")); Serial.println(BOARD_NAME);
  Serial.println(NB_GENERIC_VERSION);

#if ( defined(DEBUG_NB_GENERIC_PORT) && (_NB_GENERIC_LOGLEVEL_ > 5) )
  MODEM.debug(DEBUG_NB_GENERIC_PORT);
#endif  

  connectToGPRS();
}

void loop()
{
  if (connected)
  {
    check_status();
  }
  else
  {
    connectToGPRS();
  }
}
