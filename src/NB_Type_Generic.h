/*********************************************************************************************************************************
  NB_Type_Generic.h
  
  For ESP8266, ESP32, SAMD21/SAMD51, nRF52, SAM DUE, Teensy and STM32 with NB-IoT modules such as u-blox SARA R401
  
  NB_Generic is a library for the ESP8266, ESP32, SAMD21/SAMD51, nRF52, SAM DUE, Teensy and STM32 with GSM modules
  
  Based on and modified from MKRNB Library (https://github.com/arduino-libraries/MKRNB)
  
  Built by Khoi Hoang https://github.com/khoih-prog/NB_Generic
  Licensed under GNU Lesser General Public License
  
  Copyright (C) 2017  Arduino AG (http://www.arduino.cc/)
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

#pragma once

#ifndef _NB_TYPE_GENERIC_H_INCLUDED
#define _NB_TYPE_GENERIC_H_INCLUDED

///////////////////////////////////////////////////////////////
//Global functions and declarations

#include <time.h>

extern "C" 
{
  char *strptime(const char *__restrict, const char *__restrict, struct tm *__restrict);
}

byte charToInt(byte input)
{
  if (input > '9') 
  {
    input = (input - 'A') + 10;
  } 
  else 
  {
    input = (input - '0');
  }
  
  return input;
}

// To use for String.indexOf(), String.lastIndexOf()
#define SUBSTRING_NOT_FOUND       (-1)

///////////////////////////////////////////////////////////////
// Originally from Modem_???_Generic.h

enum
{ 
  NB_MODEM_START_ERROR = 0,
  NB_MODEM_START_OK    = 1
};

enum 
{ 
  NB_RESPONSE_TIMEOUT        = -1, 
  NB_RESPONSE_IDLE           =  0, 
  NB_RESPONSE_OK             =  1, 
  NB_RESPONSE_ERROR          =  2, 
  NB_RESPONSE_NO_CARRIER     =  3,
  NB_RESPONSE_CME_ERROR      =  4 
};

///////////////////////////////////////////////////////////////
// Originally from NB_Generic.h and NB_Generic_Impl.hpp

//enum NB_NetworkStatus_t { ERROR, IDLE, CONNECTING, NB_READY, GPRS_READY, TRANSPARENT_CONNECTED, NB_OFF};

enum NB_NetworkStatus_t 
{ 
  NB_ERROR, 
  NB_IDLE, 
  NB_CONNECTING, 
  NB_READY, 
  GPRS_READY, 
  NB_TRANSPARENT_CONNECTED, 
  NB_OFF 
};

enum 
{
  READY_STATE_SET_ERROR_DISABLED,
  READY_STATE_WAIT_SET_ERROR_DISABLED,
  READY_STATE_SET_MINIMUM_FUNCTIONALITY_MODE,
  READY_STATE_WAIT_SET_MINIMUM_FUNCTIONALITY_MODE,
  READY_STATE_CHECK_SIM,
  READY_STATE_WAIT_CHECK_SIM_RESPONSE,
  READY_STATE_UNLOCK_SIM,
  READY_STATE_WAIT_UNLOCK_SIM_RESPONSE,
  READY_STATE_DETACH_DATA,
  READY_STATE_WAIT_DETACH_DATA,
  READY_STATE_SET_PREFERRED_MESSAGE_FORMAT,
  READY_STATE_WAIT_SET_PREFERRED_MESSAGE_FORMAT_RESPONSE,
  READY_STATE_SET_HEX_MODE,
  READY_STATE_WAIT_SET_HEX_MODE_RESPONSE,
  READY_STATE_SET_AUTOMATIC_TIME_ZONE,
  READY_STATE_WAIT_SET_AUTOMATIC_TIME_ZONE_RESPONSE,
  READY_STATE_SET_APN,
  READY_STATE_WAIT_SET_APN,
  READY_STATE_SET_APN_AUTH,
  READY_STATE_WAIT_SET_APN_AUTH,
  READY_STATE_SET_FULL_FUNCTIONALITY_MODE,
  READY_STATE_WAIT_SET_FULL_FUNCTIONALITY_MODE,
  READY_STATE_CHECK_REGISTRATION,
  READY_STATE_WAIT_CHECK_REGISTRATION_RESPONSE,
  READY_STATE_DONE
};

///////////////////////////////////////////////////////////////
// Originally from NB_GPRS_Generic.h
enum 
{
  GPRS_PING_DEST_UNREACHABLE  = -1,
  GPRS_PING_TIMEOUT           = -2,
  GPRS_PING_UNKNOWN_HOST      = -3,
  GPRS_PING_ERROR             = -4
};

typedef struct 
{
  NB_NetworkStatus_t    _state;
  int                   _readyState;
  const char*           _pin;
  const char*           _apn;
  const char*           _username;
  const char*           _password;
  unsigned long         _timeout;
} NB_Data;

///////////////////////////////////////////////////////////////
// Originally from NB_GPRS_Generic_Impl.hpp
enum {
  GPRS_STATE_IDLE,

  GPRS_STATE_ATTACH,
  GPRS_STATE_WAIT_ATTACH_RESPONSE,
  GPRS_STATE_CHECK_ATTACHED,
  GPRS_STATE_WAIT_CHECK_ATTACHED_RESPONSE,

  GPRS_STATE_DEATTACH,
  GPRS_STATE_WAIT_DEATTACH_RESPONSE
};

///////////////////////////////////////////////////////////////
// Originally from NBClient_Generic_Impl.hpp

enum 
{
  CLIENT_STATE_IDLE,
  CLIENT_STATE_CREATE_SOCKET,
  CLIENT_STATE_WAIT_CREATE_SOCKET_RESPONSE,
  CLIENT_STATE_ENABLE_SSL,
  CLIENT_STATE_WAIT_ENABLE_SSL_RESPONSE,
  CLIENT_STATE_MANAGE_SSL_PROFILE,
  CLIENT_STATE_WAIT_MANAGE_SSL_PROFILE_RESPONSE,
  CLIENT_STATE_CONNECT,
  CLIENT_STATE_WAIT_CONNECT_RESPONSE,
  CLIENT_STATE_CLOSE_SOCKET,
  CLIENT_STATE_WAIT_CLOSE_SOCKET,
  CLIENT_STATE_RETRIEVE_ERROR
};


///////////////////////////////////////////////////////////////
// Originally from NB_SMS_Generic.h

#define NB_SMS_CLEAR_READ             (1)
#define NB_SMS_CLEAR_READ_SENT        (2)
#define NB_SMS_CLEAR_READ_SENT_UNSENT (3)
#define NB_SMS_CLEAR_ALL              (4)

#define SMS_CHARSET_IRA   'I'
#define SMS_CHARSET_GSM   'G'
#define SMS_CHARSET_NONE  'N' 
#define SMS_CHARSET_UCS2  'U'

enum 
{
  SMS_STATE_IDLE,
  SMS_STATE_LIST_MESSAGES,
  SMS_STATE_WAIT_LIST_MESSAGES_RESPONSE
};

#define NB_SMS_UTF8_BUFFER_SZ         (4)

typedef struct
{
  bool        synch;
  int         state;
  int         smsDataIndex;
  int         smsDataEndIndex;
  bool        smsTxActive;
  int         charset;
  char        bufferUTF8[NB_SMS_UTF8_BUFFER_SZ];
  int         indexUTF8;
  const char* ptrUTF8;
} NB_SMS_Data;

// Originally from NB_SSLClient_Generic_Impl.hpp

enum
{
  SSL_CLIENT_STATE_LOAD_ROOT_CERT,
  SSL_CLIENT_STATE_WAIT_LOAD_ROOT_CERT_RESPONSE,
  SSL_CLIENT_STATE_WAIT_DELETE_ROOT_CERT_RESPONSE
};


///////////////////////////////////////////////////////////////
// Originally from NBSocketBuffer_Generic.h

#define NB_SOCKET_BUFFER_SIZE      512
#define NB_SOCKET_BUFFER_NUMBER    7

typedef struct 
{
  uint8_t* data;
  uint8_t* head;
  int length;
} SocketBufferList;
    
///////////////////////////////////////////////////////////////
// SIM PIN related definitions

enum
{
  SIM_PIN_UNLOCKED    =  0, 
  SIM_PIN_NEED_PIN    =  1,
  SIM_PIN_NEED_PUK    = -1,
  SIM_PIN_ERROR       = -2,
  SIM_PIN_WRONG_PIN   = -3
};


enum
{ 
  SIM_PIN_WRITE_SUCCESS   =  1, 
  SIM_PIN_WRITE_ERROR     = -1,
  SIM_PIN_WRITE_WRONG_PIN = -2
};



/*
  Network registration status +CREG
  • 0: not registered, the MT is not currently searching a new operator to register to
  • 1: registered, home network
  • 2: not registered, but the MT is currently searching a new operator to register to
  • 3: registration denied
  • 4: unknown (e.g. out of GERAN/UTRAN/E-UTRAN coverage)
  • 5: registered, roaming
  • 6: registered for "SMS only", home network (applicable only when <AcTStatus>
  indicates E-UTRAN)
  • 7: registered for "SMS only", roaming (applicable only when <AcTStatus> indicates
  E-UTRAN)
  • 8: attached for emergency bearer services only (see 3GPP TS 24.008 [65] and 3GPP
  TS 24.301 [100] that specify the condition when the MS is considered as attached
  for emergency bearer services)
  • 9: registered for "CSFB not preferred", home network (applicable only when
  <AcTStatus> indicates E-UTRAN)
  • 10: registered for "CSFB not preferred", roaming (applicable only when <AcTStatus>
  indicates E-UTRAN)
*/

enum
{ 
  NB_NETWORK_REGISTRATION_ERROR              = -1,
  NB_NETWORK_NOT_REGISTERED_NOT_SEARCHING    =  0, 
  NB_NETWORK_HOME                            =  1,
  NB_NETWORK_NOT_REGISTERED_SEARCHING        =  2,
  NB_NETWORK_REGISTRATION_DENIED             =  3,
  NB_NETWORK_UNKNOWN                         =  4, 
  NB_NETWORK_ROAMING                         =  5,
  NB_NETWORK_HOME_SMS_ONLY                   =  6,
  NB_NETWORK_ROAMING_SMS_ONLY                =  7,
  NB_NETWORK_EMERGENCY_ONLY                  =  8, 
  NB_NETWORK_HOME_CSFB_NOT_PREFERRED         =  9,
  NB_NETWORK_ROAMING_CSFB_NOT_PREFERRED      =  10
};

enum
{ 
  NB_NETWORK_CHECK_IS_ERROR                  = -1,
  NB_NETWORK_CHECK_IS_HOME                   =  0,
  NB_NETWORK_CHECK_IS_ROAMING                =  1,
  NB_NETWORK_CHECK_IS_EMERGENCY_ONLY         =  2,
};
    
    
///////////////////////////////////////////////////////////////
// Originally from NBUDP_Generic.h
// To permit changing from sketch UDP TX/RX buffer size if necessary
// TO be used in Modem_???_Extra_Generic.hpp
#if !defined(NB_UDP_RX_BUFFER_SIZE)
  #define NB_UDP_RX_BUFFER_SIZE      512
#endif

#if !defined(NB_UDP_TX_BUFFER_SIZE)
  #define NB_UDP_TX_BUFFER_SIZE      512
#endif

///////////////////////////////////////////////////////////////
    
#endif    // _NB_TYPE_GENERIC_H_INCLUDED
