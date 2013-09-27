// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "LedMatrixBuoyViz.h"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <math.h>


//-----------------------------------------------------------------------------
LedMatrixBuoyViz::LedMatrixBuoyViz(RgbMatrix *m, DisplayUpdater *d)
  : RgbMatrixContainer(m)
{
  _explainMode = true;

  _updater = d;

  _red.red = 180;

  _green.green = 180;

  _blue.blue = 180;

  _purple.red = 180;
  _purple.blue = 180;

  _yellow.red = 180;
  _yellow.green = 180;

  _turquoise.green = 180;
  _turquoise.blue = 180;

  _white.red = 180;
  _white.green = 180;
  _white.blue = 180;
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

  _explainCount = 0;

  //Get the Buoy data from a web-service/API.
  _buoyData = new BuoyData();
  _buoyData->getBuoyData();
  _buoyData->start();  //start thread to auto-update every X minutes

  introduceDataViz();

  while (!isDone())
  {
    //In Explain mode, re-introduce the data viz from time to time.
    if (_explainMode && _explainCount > 6)
    {
      introduceDataViz();
      _explainCount = 0;
    }

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
        // TODO: Wind Speed and Direction
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

    if (curStat > NumStats)
    {
      curStat = 1;
      _explainCount++;
    }
  }
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::introduceDataViz()
{
  _matrix->setFontSize(2);
  _matrix->setWordWrap(false);
  _matrix->setFontColor(_white);

  _matrix->setTextCursor(1, 0);
  std::string textLine = "SF";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(13, 0);
  textLine = "Bay";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(6, 8);
  textLine = "Area";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(3, 16);
  textLine = "Surf";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(6, 24);
  textLine = "State";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  sleep(WaitBefore-2);
  _matrix->fadeRect(0, 0, 31, 7);  //TODO: Use SwellFontWidth...
  _matrix->fadeRect(0, 8, 31, 7);
  _matrix->fadeRect(0, 16, 31, 7);
  _matrix->fadeRect(0, 24, 31, 7);

  //Buoy Location
  _matrix->setFontColor(_white);

  _matrix->setTextCursor(1, 0);
  textLine = "using";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(6, 8);
  textLine = "SF";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(18, 8);
  textLine = "Bar";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(3, 16);
  textLine = "Buoy";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(6, 24);
  textLine = "Data";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  sleep(WaitBefore-2);
  _matrix->fadeRect(0, 0, 32, 7);  //TODO: Use SwellFontWidth...
  _matrix->fadeRect(0, 8, 32, 7);
  _matrix->fadeRect(0, 16, 32, 7);
  _matrix->fadeRect(0, 24, 32, 7);
  sleep(1);

  //Buoy Location
  _matrix->setFontColor(_red);

  _matrix->setTextCursor(1, 0);
  textLine = "The";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(6, 7);
  textLine = "Buoy";

  for (std::string::iterator itr = textLine.begin(); itr != textLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->fillCircle(BuoyX, BuoyY, 1, _red); //Add Buoy Location

  sleep(2);
  _matrix->fadeRect(0, 0, 32, 6);  //TODO: Use SwellFontWidth...
  _matrix->fadeRect(0, 7, 32, 7);

  sleep(WaitBefore-2);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::groundSwell()
{
  //Only display Title in "Explain" mode.
  if (_explainMode && _explainCount == 0)
  {
    displayTitle(_green, _green, "Ground", "Swell");
  }

  //Buoy Info
  _updater->suspend(); // Stop updating display in order to fade-in

  _matrix->setFontColor(_green);
  _matrix->setTextCursor(0, 6);
  std::string work = _buoyData->getGroundSwellDirection();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(3, 14);
  work = _buoyData->getGroundSwellHeight();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('\'');
  _matrix->writeChar('@');

  _matrix->setTextCursor(6, 22);
  work = _buoyData->getGroundSwellPeriod();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('s');

  // Fade In the content 
  _matrix->setupFadeIn();
  _updater->resume();
  _matrix->fadeIn();

  sleep(WaitBefore);
  _matrix->wipeDown();
  
  displayGroundSwellViz();

  _matrix->fillCircle(BuoyX, BuoyY, 1, _red); //Add Buoy Location

  sleep(WaitBefore);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::windSwell()
{
  //Only display Title in "Explain" mode.
  if (_explainMode && _explainCount == 0)
  {
    displayTitle(_blue, _blue, "Wind", "Swell");
  }

  //Buoy Info
  _updater->suspend(); // Stop updating display in order to fade-in

  _matrix->setFontColor(_blue);
  _matrix->setTextCursor(0, 6);
  std::string work = _buoyData->getWindSwellDirection();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(3, 14);
  work = _buoyData->getWindSwellHeight();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->writeChar('\'');
  _matrix->writeChar('@');

  _matrix->setTextCursor(6, 22);
  work = _buoyData->getWindSwellPeriod();

  for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  // Fade In the content 
  _matrix->setupFadeIn();
  _updater->resume();
  _matrix->fadeIn();

  _matrix->writeChar('s');

  sleep(WaitBefore);
  _matrix->wipeDown();

  displayWindSwellViz();

  _matrix->fillCircle(BuoyX, BuoyY, 1, _red); //Add Buoy Location

  sleep(WaitBefore);
  _matrix->fadeDisplay();
  sleep(WaitAfter);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::groundAndWindSwell()
{
  //Only display Title in "Explain" mode.
  if (_explainMode && _explainCount == 0)
  {
    displayTitle(_turquoise, _turquoise, "Combo", "Swell");
  }

  displayWindSwellViz();
  displayGroundSwellViz();

  _matrix->fillCircle(BuoyX, BuoyY, 1, _red); //Add Buoy Location

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




//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::displayTitle(Color firstLineColor,
                                    Color secondLineColor,
                                    std::string firstLine,
                                    std::string secondLine)
{
  _matrix->setFontSize(2);
  _matrix->setWordWrap(false);

  _matrix->setTextCursor(1, 6);
  _matrix->setFontColor(firstLineColor);

  for (std::string::iterator itr = firstLine.begin(); itr != firstLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  _matrix->setTextCursor(6, 14);
  _matrix->setFontColor(secondLineColor);

  for (std::string::iterator itr = secondLine.begin(); itr != secondLine.end(); itr++)
  {
    _matrix->writeChar(*itr);
  }

  sleep(WaitBefore-2);
  _matrix->fadeRect(0, 6, 31, 6);  //TODO: Use SwellFontWidth...
  _matrix->fadeRect(0, 14, 31, 6);
  sleep(1);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::displayGroundSwellViz()
{
  // Calculate Angle and Size from Height, Period, and Direction.
  float gsHeight = atof(_buoyData->getGroundSwellHeight().c_str()); //radius
  float gsPeriod = atof(_buoyData->getGroundSwellPeriod().c_str()); //arc angle
  float gsDir = _buoyData->convertCompassPointToDegrees(_buoyData->getGroundSwellDirection());

  float gsRadius = std::min((float)GsMaxRadius, std::max((float)(GsMaxRadius/2), gsHeight));
  float gsAngle  = std::min((float)GsMaxAngle, gsPeriod*GsMaxAngle/25);

  float startAngle = gsDir - gsAngle;
  float endAngle = gsDir + gsAngle;

  //DEBUG
  //time_t now = time(0);
  //std::cout << ctime(&now) << "  Green Wedge - radius: " << gsRadius << 
  //             ", angle: " << gsAngle << ", start: " << startAngle << " end: " << endAngle << std::endl
  //          << "  Height: " << gsHeight << ", Period: " << gsPeriod << std::endl << std::endl;

  //TODO: Change x,y to cos, sin funtions to put wedge at edge of radius.
  _matrix->drawWedge(BuoyX, BuoyY, (int)gsRadius, startAngle, endAngle, _green);
}


//-----------------------------------------------------------------------------
void LedMatrixBuoyViz::displayWindSwellViz()
{
  // Calculate Angle and Size from Height, Period, and Direction.
  float wsHeight = atof(_buoyData->getWindSwellHeight().c_str()); //radius
  float wsPeriod = atof(_buoyData->getWindSwellPeriod().c_str()); //arc angle
  float wsDir = _buoyData->convertCompassPointToDegrees(_buoyData->getWindSwellDirection());

  float wsRadius = std::min((float)WsMaxRadius, std::max((float)(WsMaxRadius/2), wsHeight));
  float wsAngle  = std::min((float)WsMaxAngle, wsPeriod*WsMaxAngle/25);

  float wsStartAngle = wsDir - wsAngle;
  float wsEndAngle = wsDir + wsAngle;

  //DEBUG
  //time_t now = time(0);
  //std::cout << ctime(&now) << "  Blue Wedge - radius: " << wsRadius << 
  //             ", angle: " << wsAngle << ", start: " << wsStartAngle << " end: " << wsEndAngle << std::endl
  //          << "  Height: " << wsHeight << ", Period: " << wsPeriod << std::endl << std::endl;

  _matrix->drawWedge(BuoyX, BuoyY, (int)wsRadius, wsStartAngle, wsEndAngle, _blue);
}

