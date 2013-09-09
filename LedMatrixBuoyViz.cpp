// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "LedMatrixBuoyViz.h"

#include <ctime>
#include <iostream>


//-----------------------------------------------------------------------------
LedMatrixBuoyViz::LedMatrixBuoyViz(RgbMatrix *m) : RgbMatrixContainer(m)
{
  _red.red = 255;

  _green.green = 255;

  _blue.blue = 255;

  _purple.red = 255;
  _purple.blue = 255;

  _yellow.red = 255;
  _yellow.green = 255;

  _white.red = 255;
  _white.green = 255;
  _white.blue = 255;
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::run()
{
  // Display Various Stats
  //   1: Ground Swell
  //   2: Wind Swell
  //   3: Ground & Wind Swells
  //   4: Tide
  //   5: Wind
  //   6: Time & Sunrise/Sunset
  //   7: Temp: Air / Water
  const int NumStats = 7;
  int curStat = 1;

  //Get the Buoy data from a web-service/API.
  _buoyData = new BuoyData();
  _buoyData->getBuoyData();

  std::cout << "Out!" << std::endl;


  std::cout << "Last Updated at: " <<
               _buoyData->getDate() << " " <<
               _buoyData->getTime() << std::endl
            << "Ground Swell: " <<
               _buoyData->getGroundSwellDirection() << " " <<
               _buoyData->getGroundSwellHeight() << "' @ " <<
               _buoyData->getGroundSwellPeriod() << "s" << std::endl
            << "Wind Swell: " <<
               _buoyData->getWindSwellDirection() << " " <<
               _buoyData->getWindSwellHeight() << "' @ " <<
               _buoyData->getWindSwellPeriod() << "s" << std::endl << std::endl;

  while (!isDone())
  {
    switch (curStat)
    {
      case 1:
        groundSwell();
        break;

      case 2:
        windSwell();
        break;

      case 3:
        groundAndWindSwell();
        break;

      case 4:
        // TODO: Tide
        break;

      case 5:
        // TODO: Wind
        break;

      case 6:
        //TODO: skip for now...
        //sunriseSunset();
        break;

      case 7:
        // TODO: Temp: Air / Water
        break;
    }

    curStat++;
    if (curStat > NumStats) curStat = 1;
  }
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::groundSwell()
{
  _matrix->setTextCursor(1, 6);
  _matrix->setFontColor(_green);
  _matrix->setFontSize(2);

  _matrix->writeChar('G');
  _matrix->writeChar('r');
  _matrix->writeChar('o');
  _matrix->writeChar('u');
  _matrix->writeChar('n');
  _matrix->writeChar('d');

  _matrix->setTextCursor(6, 14);
  _matrix->writeChar('S');
  _matrix->writeChar('w');
  _matrix->writeChar('e');
  _matrix->writeChar('l');
  _matrix->writeChar('l');

  sleep(WaitBefore-2);
  _matrix->fadeRect(0, 6, 31, 6);  //TODO: Font Height
  _matrix->fadeRect(0, 14, 31, 6);


  _matrix->setTextCursor(5, 6);
  std::string work = _buoyData->getGroundSwellDirection();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(5, 14);
  work = _buoyData->getGroundSwellHeight();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('\'');

  _matrix->setTextCursor(5, 22);
  work = _buoyData->getGroundSwellPeriod();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('s');

  sleep(WaitBefore);
  //_matrix->clearDisplay();
  _matrix->fadeDisplay();
  //sleep(WaitAfter);

  //TODO: Calculate Angle and Size from Height, Period, and Direction.
  float gsDir = _buoyData->convertCompassPointToDegrees(_buoyData->getGroundSwellDirection());

  _matrix->drawWedge(16, 11, 12, gsDir - 45, gsDir + 45, _green);
  _matrix->fillCircle(16, 11, 1, _red); //buoy location

  sleep(WaitBefore);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::windSwell()
{
  _matrix->setTextCursor(1, 6);
  _matrix->setFontColor(_blue);
  _matrix->setFontSize(2);

  _matrix->writeChar('W');
  _matrix->writeChar('i');
  _matrix->writeChar('n');
  _matrix->writeChar('d');

  _matrix->setTextCursor(6, 14);
  _matrix->writeChar('S');
  _matrix->writeChar('w');
  _matrix->writeChar('e');
  _matrix->writeChar('l');
  _matrix->writeChar('l');

  sleep(WaitBefore-2);
  _matrix->fadeRect(0, 6, 31, 6);  //TODO: Font Height
  _matrix->fadeRect(0, 14, 31, 6);


  _matrix->setTextCursor(5, 6);
  std::string work = _buoyData->getWindSwellDirection();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(5, 14);
  work = _buoyData->getWindSwellHeight();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('\'');

  _matrix->setTextCursor(5, 22);
  work = _buoyData->getWindSwellPeriod();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('s');

  sleep(WaitBefore);
  _matrix->fadeDisplay();
  //sleep(WaitAfter);

  //TODO: Calculate Angle and Size from Height, Period, and Direction.
  float wsDir = _buoyData->convertCompassPointToDegrees(_buoyData->getWindSwellDirection());


  _matrix->drawWedge(16, 11, 9, wsDir - 45, wsDir + 45, _blue);
  _matrix->fillCircle(16, 11, 1, _red); //buoy location

  sleep(WaitBefore);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::groundAndWindSwell()
{
  //TODO: Calculate Angle and Size from Height, Period, and Direction.
  float wsDir = _buoyData->convertCompassPointToDegrees(_buoyData->getWindSwellDirection());
  _matrix->drawWedge(16, 11, 9, wsDir - 45, wsDir + 45, _blue);

  float gsDir = _buoyData->convertCompassPointToDegrees(_buoyData->getGroundSwellDirection());
  _matrix->drawWedge(16, 11, 12, gsDir - 45, gsDir + 45, _green);

  _matrix->fillCircle(16, 11, 1, _red); //buoy location

  sleep(WaitBefore);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::sunriseSunset()
{
  std::time_t rawTime;
  std::tm *timeInfo;
  char timeBuffer[10];
  int cursorX;

  std::time(&rawTime);
  timeInfo = std::localtime(&rawTime);

  std::strftime(timeBuffer, 10, "%I:%M %p", timeInfo);

  std::string timeString = timeBuffer;

  //std::cout << "String: " << timeString << std::endl;

  std::string hourString = timeString.substr(0, 2);
  std::string minString  = timeString.substr(3, 2);
  std::string amPmString = timeString.substr(6, 2);

  //std::cout << "Substring: " << hourString << ":" << minString << " " << amPmString << std::endl;

  //TODO: skip leading 0.
  cursorX = 0;
  _matrix->putChar(cursorX, 0, hourString[0], 2, _white);

  if (hourString.length() > 1)
  {
    cursorX = SwellFontWidth;
    _matrix->putChar(cursorX, 0, hourString[1], 2, _white);
  }

  cursorX += SwellFontWidth + 1; //mateo: added 1
  _matrix->putChar(cursorX, 0, ':', 2, _white);

  cursorX += SwellFontWidth; //mateo: - 1; // -1 to make closer to ":"
  _matrix->putChar(cursorX, 0, minString[0], 2, _white);
  cursorX += SwellFontWidth;
  _matrix->putChar(cursorX, 0, minString[1], 2, _white);


  cursorX += SwellFontWidth + 2; // +2 to give space to AM/PM
  _matrix->putChar(cursorX, 1, amPmString[0], 1, _white);
  cursorX += SwellFontWidth;
  _matrix->putChar(cursorX, 1, amPmString[1], 1, _white);

  // TODO: Add Sunrise / Sunset

  // * - Sunrise
  _matrix->putChar(4, 12, '*', 3, _yellow);


  // ( - Sunset
  _matrix->putChar(4, 20, '(', 3, _blue);


  sleep(WaitBefore);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}

