/*********************************************************************************************************************************
  NBFileUtils_Generic_Impl.hpp
  
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

#ifndef _NB_FILE_UTILS_GENERIC_IMPL_H_INCLUDED
#define _NB_FILE_UTILS_GENERIC_IMPL_H_INCLUDED

NBFileUtils::NBFileUtils(bool debug)
  : _count(0)
  , _files("")
  , _debug(debug)
{
}

bool NBFileUtils::begin(unsigned long baud, const bool restart)
{
  int status;

  if (restart)
    MODEM.begin(baud);

  if (_debug)
  {
    MODEM.debug();

    // Report mobile termination error +CMEE
    MODEM.setReportError();

    MODEM.waitForResponse();
  }

  for (unsigned long start = millis(); (millis() - start) < 10000;)
  {
    status = _getFileList();

    if (status == NB_RESPONSE_OK)
    {
      _countFiles();
      return true;
    }

    MODEM.poll();
  }

  return false;
}

int NBFileUtils::_getFileList()
{
  String response;
  int status = NB_RESPONSE_IDLE;

  while (!status)
  {
    MODEM.listFile();

    status = MODEM.waitForResponse(5000, &response);

    if (status == NB_RESPONSE_OK)
    {
      String list = response.substring(11);
      list.trim();
      _files = list;
    }
  }

  return status;
}

int NBFileUtils::existFile(const String filename)
{
  _getFileList();
  _countFiles();

  String files[_count];

  int num = listFiles(files);

  for (int i = 0; i < num; i++)
  {
    if (files[i] == filename)
    {
      return 1;
    }
  }

  return 0;
}

void NBFileUtils::_countFiles()
{
  String list = _files;
  size_t len = 0;

  if (list.length() > 0)
  {
    for (int index = list.indexOf(','); index != SUBSTRING_NOT_FOUND; index = list.indexOf(','))
    {
      list.remove(0, index + 1);
      ++len;
    }

    ++len;
  }

  _count = len;
}

size_t NBFileUtils::listFiles(String files[]) const
{
  String list = _files;
  int index;

  if (_count == 0)
    return 0;

  size_t n = 0;

  for (index = list.indexOf(','); index != SUBSTRING_NOT_FOUND; index = list.indexOf(','))
  {
    String file = list.substring(1, index - 1);
    files[n++] = file;
    list.remove(0, index + 1);
  }

  files[n++] = list.substring(1, list.lastIndexOf("\""));

  return n;
}

uint32_t NBFileUtils::downloadFile(const String filename, const char buf[], uint32_t size, const bool append)
{
  if (!append)
    deleteFile(filename);

  int status = NB_RESPONSE_IDLE;

  while (status == NB_RESPONSE_IDLE)
  {
    //Download file +UDWNFILE
    MODEM.downloadFile(filename, size);

    MODEM.waitForPrompt(20000);

    char hex[size * 2] { 0 };

    for (uint32_t i = 0; i < size; i++)
    {
      byte b = buf[i];

      byte n1 = (b >> 4) & 0x0f;
      byte n2 = (b & 0x0f);

      hex[i * 2] = (char)(n1 > 9 ? 'A' + n1 - 10 : '0' + n1);
      hex[i * 2 + 1] = (char)(n2 > 9 ? 'A' + n2 - 10 : '0' + n2);
    }

    for (auto h : hex)
      MODEM.write(h);

    status = MODEM.waitForResponse(1000);
  }

  auto fileExists = _files.indexOf(filename) > 0;

  if (!fileExists)
  {
    _getFileList();
    _countFiles();
  }

  return size;
}

uint32_t NBFileUtils::createFile(const String filename, const char buf[], uint32_t size)
{
  uint32_t sizeFile;
  sizeFile = listFile(filename);

  if (sizeFile)
  {
    return sizeFile;
  }

  return downloadFile(filename, buf, size, true);
}

uint32_t NBFileUtils::readFile(const String filename, String* content)
{
  String response;

  if (!listFile(filename))
  {
    return 0;
  }

  // Read file +URDFILE
  MODEM.readFile(filename);

  MODEM.waitForResponse(1000, &response);

  size_t skip = 10;
  String _content = response.substring(skip);

  int commaIndex = _content.indexOf(',');
  skip += commaIndex;

  _content = _content.substring(commaIndex + 1);
  commaIndex = _content.indexOf(',');
  skip += commaIndex;

  String sizePart = _content.substring(0, commaIndex);
  uint32_t size = sizePart.toInt() / 2;
  skip += 3;

  String* _data = content;
  (*_data).reserve(size);

  for (uint32_t i = 0; i < size; i++)
  {
    byte n1 = response[skip + i * 2];
    byte n2 = response[skip + i * 2 + 1];

    n1 = charToInt(n1);
    n2 = charToInt(n2);

    (*_data) += (char)((n1 << 4) | n2);
  }

  return (*_data).length();
}

uint32_t NBFileUtils::readFile(const String filename, uint8_t* content)
{
  String response;

  if (listFile(filename) == 0)
  {
    return 0;
  }

  // Read file +URDFILE
  MODEM.readFile(filename);

  MODEM.waitForResponse(1000, &response);

  size_t skip = 10;
  String _content = response.substring(skip);

  int commaIndex = _content.indexOf(',');
  skip += commaIndex;

  _content = _content.substring(commaIndex + 1);
  commaIndex = _content.indexOf(',');
  skip += commaIndex;

  String sizePart = _content.substring(0, commaIndex);
  uint32_t size = sizePart.toInt() / 2;
  skip += 3;

  for (uint32_t i = 0; i < size; i++)
  {
    byte n1 = response[skip + i * 2];
    byte n2 = response[skip + i * 2 + 1];

    n1 = charToInt(n1);
    n2 = charToInt(n2);

    content[i] = (n1 << 4) | n2;
  }

  return size;
}

uint32_t NBFileUtils::readBlock(const String filename, const uint32_t offset, const uint32_t len, uint8_t* content)
{
  String response;

  if (listFile(filename) == 0)
  {
    return 0;
  }

  // Partial read file +URDBLOCK
  MODEM.readBlock(filename, offset, len);


  MODEM.waitForResponse(1000, &response);

  size_t skip = 10;
  String _content = response.substring(skip);

  int commaIndex = _content.indexOf(',');
  skip += commaIndex;

  _content = _content.substring(commaIndex + 1);
  commaIndex = _content.indexOf(',');
  skip += commaIndex;

  String sizePart = _content.substring(0, commaIndex);
  uint32_t size = sizePart.toInt() / 2;
  skip += 3;

  for (uint32_t i = 0; i < size; i++)
  {
    byte n1 = response[skip + i * 2];
    byte n2 = response[skip + i * 2 + 1];

    n1 = charToInt(n1);
    n2 = charToInt(n2);

    content[i] = (n1 << 4) | n2;
  }

  return size;
}

bool NBFileUtils::deleteFile(const String filename)
{
  String response;

  // Delete file +UDELFILE
  MODEM.deleteFile(filename);

  auto status = MODEM.waitForResponse(100, &response);

  if (status == NB_RESPONSE_IDLE)
    return false;

  _getFileList();
  _countFiles();

  return true;
}

int NBFileUtils::deleteFiles()
{
  int n = 0;
  String files[_count];

  //int num = listFiles(files);
  listFiles(files);

  while (_count > 0)
  {
    n += deleteFile(files[_count - 1]);
  }

  return n;
}

uint32_t NBFileUtils::listFile(const String filename) const
{
  String response;
  int res;
  uint32_t size = 0;

  // List files information +ULSTFILE. Size of the specified file
  MODEM.fileSize(filename);

  res = MODEM.waitForResponse(5000, &response);

  if (res == NB_RESPONSE_OK)
  {
    String content = response.substring(11);
    size = content.toInt();
  }

  return size / 2;
}

uint32_t NBFileUtils::freeSpace()
{
  String response;
  int res;
  uint32_t size = 0;

  // List files information +ULSTFILE. Remaining free FS space expressed in bytes
  MODEM.freeSpace();

  res = MODEM.waitForResponse(100, &response);

  if (res == NB_RESPONSE_OK)
  {
    String content = response.substring(11);
    size = content.toInt();
  }

  return size;
}

void printFiles(const NBFileUtils fu)
{
  auto count { fu.fileCount() };
  String files[count];

  DBG_PORT_NB.print(count);
  DBG_PORT_NB.print(count == 1 ? " file" : " files");
  DBG_PORT_NB.println(" found.");

  fu.listFiles(files);

  for (auto f : files)
  {
    DBG_PORT_NB.print("File ");
    DBG_PORT_NB.print(f);
    DBG_PORT_NB.print(" - Size: ");
    DBG_PORT_NB.print(fu.listFile(f));
    DBG_PORT_NB.println();
  }
}

#endif    // _NB_FILE_UTILS_GENERIC_IMPL_H_INCLUDED
