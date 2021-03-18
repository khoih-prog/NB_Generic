/*********************************************************************************************************************************
  NB_Generic_Main.h
  
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

#pragma once

#ifndef _NB_GENERIC_MAIN_H_INCLUDED
#define _NB_GENERIC_MAIN_H_INCLUDED

#define NB_GENERIC_VERSION     "NB_Generic v1.1.0"

#include <stdarg.h>
#include <stdio.h>
#include "Arduino.h"

#include "NB_Generic_Debug.h"

#include "NB_Boards_Generic.h"

////////////////////////////////////////

#if ( NB_MODEM_SARAR4 )
  #warning Using NB_MODEM_SARAR4
  #include "Modems/Modem_SaraR4_Generic.h"
#else
  #error No NB-IoT Modem selected yet
#endif  

////////////////////////////////////////

#include "NB_Generic.h"
#include "NB_SMS_Generic.h"
#include "NB_GPRS_Generic.h"
#include "NBClient_Generic.h"
#include "NBModem_Generic.h"
#include "NBScanner_Generic.h"
#include "NBPIN_Generic.h"

#include "NBSSLClient_Generic.h"
#include "NBUdp_Generic.h"

#include "NBFileUtils_Generic.h"

#endif    // _NB_GENERIC_MAIN_H_INCLUDED
