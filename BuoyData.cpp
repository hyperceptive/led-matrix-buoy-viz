// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "BuoyData.h"
#include "HttpRequest.h"

#include <cstdio>
#include <math.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>


template <typename T>
T **newArray2D(int rows, int cols)
{
  T **dynamicArray;

  dynamicArray = new T*[rows];

  for (int i=0; i < rows; i++)
  {
    dynamicArray[i] = new T[cols];
  }

  return dynamicArray;
}

template <typename T>
void deleteArray2D(T **array)
{
  delete[] *array;
  delete[] array;
}


int payloadSize;  
std::string payload;
std::string **buoyInfoData;


//-----------------------------------------------------------------------------
void headersReady(const HttpResponse *response, void *additionalParams)
{
  printf("HTTP Status: %d - %s\n", response->getStatus(), response->getReason());
  payloadSize = 0;
}


//-----------------------------------------------------------------------------
void receiveData(const HttpResponse *response, void *additionalParams, const unsigned char *data, int sizeOfData)
{
  payloadSize += sizeOfData;

  for (int i=0; i < sizeOfData; i++)
  {
    char c = data[i];
    payload += c;
  }
}


//-----------------------------------------------------------------------------
void responseComplete(const HttpResponse *response, void *additionalParams)
{
  std::string onlyData;
  //For CDIP service: unsigned posStart, posEnd, length;

  //Param is an instance of the BuoyData class.
  BuoyData *buoyData = static_cast<BuoyData*>(additionalParams);

  // First, get the RAW data.
  printf("Data Size: %d bytes\n\n", payloadSize);
  //For CDIP service: posStart = payload.find("YYYYMMDDHHMM");
  //For CDIP service: posEnd   = payload.find("</pre>");
  //For CDIP service: length   = posEnd - posStart;
  //For CDIP service: onlyData = payload.substr(posStart, length);
  onlyData = payload;

  // Next, replace spaces with TAB and count the number of rows & cols.

  std::string tableData;

  int rows = 0;
  int cols = 1;

  for (std::string::iterator itr = onlyData.begin(); itr != onlyData.end(); itr++)
  {
    if (*itr == ' ')
    {
      if (rows == 0) cols++;
      tableData += '\t';

      while (*itr && *itr == ' ')
      {
        itr++; // Skip additional spaces
      }

      if (*itr && *itr != ' ') itr--;
    }
    else if (*itr == '\n')
    {
      rows++;
      tableData += *itr;
    }
    else
    {
      tableData += *itr;
    }
  }

  // Then, put the data into a Table (2D Array).

  buoyInfoData = newArray2D<std::string>(rows, cols);
  std::string currentValue;
  int currRow = 0;
  int currCol = 0;

  for (std::string::iterator itr = tableData.begin(); itr != tableData.end(); itr++)
  {
    if (*itr == '\t')
    {
      buoyInfoData[currRow][currCol] = currentValue;
      currentValue.clear();
      currCol++;
    }
    else if (*itr == '\n')
    {
      buoyInfoData[currRow][currCol] = currentValue;
      currentValue.clear();
      currRow++;
      currCol = 0;
    }
    else
    {
      currentValue += *itr;
    }
  }

/*
  std::cout << "Table Data: " << std::endl << tableData << std::endl
            << "rows: " << rows<< ", cols: " << cols << std::endl << std::endl;

  std::cout << "buoyInfoData[1][0]: " << buoyInfoData[1][0] << std::endl
            << "buoyInfoData[4][4]: " << buoyInfoData[4][4] << std::endl;
*/

  buoyData->setDate(buoyInfoData[2][0] + buoyInfoData[2][1] + buoyInfoData[2][2]); //YYYYMMDD
  buoyData->setTime(buoyInfoData[2][3] + buoyInfoData[2][4]); //HHMM
  buoyData->setGroundSwellHeight(buoyData->convertMetersToFeet(buoyInfoData[2][6]));
  buoyData->setGroundSwellPeriod(buoyInfoData[2][7]);
  buoyData->setGroundSwellDirection(buoyInfoData[2][10]);
  buoyData->setWindSwellHeight(buoyData->convertMetersToFeet(buoyInfoData[2][8]));
  buoyData->setWindSwellPeriod(buoyInfoData[2][9]);
  buoyData->setWindSwellDirection(buoyInfoData[2][11]);


  //TODO: setup a timer to request this info again.
  
}



//-----------------------------------------------------------------------------
BuoyData::BuoyData()
{	
}
 
BuoyData::~BuoyData()
{
  deleteArray2D(buoyInfoData);
}


//-----------------------------------------------------------------------------
void BuoyData::getBuoyData()
{
  const char *headers[] =
  {
    "Connection", "close",
    "Content-type", "application/x-www-form-urlencoded",
    "Accept", "text/plain",
    0  //null terminator
  };

/*
  // Swell Energy
  HttpRequest request("cdip.ucsd.edu", 80);

  int buoyEnergy = 0;
  request.initCallbacks(headersReady, receiveData, responseComplete, &buoyEnergy);
  request.sendRequest("POST", "/data_access/justdar.cdip?142+de", headers);

  while(request.responsesPending())
  {
    request.processRequest();
  }

  // Swell Direction
  HttpRequest request2("cdip.ucsd.edu", 80);

  int buoyDirection = 1;
  request2.initCallbacks(headersReady, receiveData, responseComplete, &buoyDirection);
  request2.sendRequest("POST", "/data_access/justdar.cdip?142+dd", headers);

  while(request2.responsesPending())
  {
    request2.processRequest();
  }
*/


  //Try NOAA web-service - still accesses CDIP buoy info, just via NOAA.
  //http://www.ndbc.noaa.gov/data/realtime2/46237.spec
  
  //#YY  MM DD hh mm WVHT  SwH  SwP  WWH  WWP SwD WWD  STEEPNESS  APD MWD
  //#yr  mo dy hr mn    m    m  sec    m  sec  -  degT     -      sec degT
  //2013 09 08 17 01  1.0  0.5 11.1  0.9  9.9   W WNW    AVERAGE  6.5 285

  // YY  MM DD 
  // hh mm 
  // WVHT (m)  - Significant Wave Height
  // SwH (m)   - Ground Swell Height
  // SwP (s)   - Ground Swell Period
  // WWH (m)   - Wind Swell Height
  // WWP (s)   - Wind Swell Period
  // SwD       - Ground Swell Direction
  // WWD       - Wind Swell Direction
  // STEEPNESS
  // APD (s)   - Average Wave Period
  // MWD (deg) - Dominant Wave Direction

  // 
  HttpRequest request("www.ndbc.noaa.gov", 80);

  request.initCallbacks(headersReady, receiveData, responseComplete, this);
  request.sendRequest("POST", "/data/realtime2/46237.spec", headers);

  while(request.responsesPending())
  {
    request.processRequest();
  }

  //http://www.ndbc.noaa.gov/data/realtime2/46237.txt
  //#YY  MM DD hh mm WDIR WSPD GST  WVHT   DPD   APD MWD   PRES  ATMP  WTMP  DEWP  VIS PTDY  TIDE
  //#yr  mo dy hr mn degT m/s  m/s     m   sec   sec degT   hPa  degC  degC  degC  nmi  hPa    ft
  //2013 09 08 17 01  MM   MM   MM   1.0    10   6.5 285     MM    MM  15.0    MM   MM   MM    MM

  // YY  MM DD 
  // hh mm 
  // WVHT (m)  -- see above
  // DPD (s)   -- Dominant Wave Period
  // APD (s)   -- also above
  // MWD (deg) -- also above
  // WTMP (C)
  
}



//-----------------------------------------------------------------------------
std::string BuoyData::convertMetersToFeet(std::string meters)
{
  float feet = atof(meters.c_str()) * 3.2808;
  feet = floorf(feet * 5 + 0.5) / 5;

  std::ostringstream ss;
  ss << feet;
  return ss.str();
}

//-----------------------------------------------------------------------------
float BuoyData::convertCompassPointToDegrees(std::string cp)
{
  if (cp == "N")
    return 270;
  else if (cp == "NNE")
    return 292.5;
  else if (cp == "NE")
    return 315;
  else if (cp == "ENE")
    return 337.5;
  else if (cp == "E")
    return 0;
  else if (cp == "ESE")
    return 22.5;
  else if (cp == "SE")
    return 45;
  else if (cp == "SSE")
    return 67.5;
  else if (cp == "S")
    return 90;
  else if (cp == "SSW")
    return 112.5;
  else if (cp == "SW")
    return 135;
  else if (cp == "WSW")
    return 157.5;
  else if (cp == "W")
    return 180;
  else if (cp == "WNW")
    return 202.5;
  else if (cp == "NW")
    return 225;
  else if (cp == "NNW")
    return 247.5;

  return 0;
}

