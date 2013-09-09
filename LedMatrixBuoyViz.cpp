// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "LedMatrixBuoyViz.h"
#include "BuoyData.h"

#include <ctime>
#include <iostream>


void LedMatrixBuoyViz::run()
{
  Color red;
  red.red = 255;

  Color green;
  green.green = 255;

  Color blue;
  blue.blue = 255;

  Color purple;
  purple.red = 255;
  purple.blue = 255;

  Color yellow;
  yellow.red = 255;
  yellow.green = 255;

  Color white;
  white.red = 255;
  white.green = 255;
  white.blue = 255;

  // Different Stats to Display
  //   1: Primary Swell
  //   2: Seconday Swell
  // TODO: show both swells...
  //   3: Tide
  //   4: Wind
  //   5: Time & Sunrise/Sunset
  //   6: Temp: Air / Water
  const int NumStats = 6;
  int curStat = 1;

  //Get the Buoy data from a web-service/API.
  BuoyData *buoyData = new BuoyData();
  buoyData->getBuoyData();

  std::cout << "Last Updated at: " <<
               buoyData->getDate() << " " <<
               buoyData->getTime() << std::endl
            << "Ground Swell: " <<
               buoyData->getGroundSwellDirection() << " " <<
               buoyData->getGroundSwellHeight() << "' @ " <<
               buoyData->getGroundSwellPeriod() << "s" << std::endl
            << "Wind Swell: " <<
               buoyData->getWindSwellDirection() << " " <<
               buoyData->getWindSwellHeight() << "' @ " <<
               buoyData->getWindSwellPeriod() << "s" << std::endl << std::endl;


  while (!isDone())
  {
    switch (curStat)
    {
      case 1:
      {
        // Primary Swell
        _matrix->setTextCursor(0, 0);
        _matrix->setFontColor(green);
        _matrix->setFontSize(2);

        _matrix->writeChar('G');
        _matrix->writeChar('r');
        _matrix->writeChar('o');
        _matrix->writeChar('u');
        _matrix->writeChar('n');
        _matrix->writeChar('d');

        _matrix->setTextCursor(5, 7);
        _matrix->writeChar('S');
        _matrix->writeChar('w');
        _matrix->writeChar('e');
        _matrix->writeChar('l');
        _matrix->writeChar('l');

        sleep(WaitBefore);
        _matrix->fadeRect(0, 0, 31, 6);  //TODO: Font Height
        _matrix->fadeRect(0, 7, 31, 6);


        _matrix->setTextCursor(5, 10);
        std::string work = buoyData->getGroundSwellDirection();

        for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
        {
          _matrix->writeChar(*itr);
        }

        _matrix->setTextCursor(5, 18);
        work = buoyData->getGroundSwellHeight();

        for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
        {
          _matrix->writeChar(*itr);
        }

        _matrix->writeChar('\'');

        _matrix->setTextCursor(5, 26);
        work = buoyData->getGroundSwellPeriod();

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
        float dir = buoyData->convertCompassPointToDegrees(buoyData->getGroundSwellDirection());
        
        _matrix->drawWedge(16, 11, 12, dir - 45, dir + 45, green);
        _matrix->fillCircle(16, 11, 1, red); //buoy location

        sleep(WaitBefore);
        _matrix->fadeDisplay();
        sleep(WaitAfter);
       break;
      }

      case 2:
      {
        // Secondary Swell
        _matrix->setTextCursor(0, 0);
        _matrix->setFontColor(blue);
        _matrix->setFontSize(1);

        _matrix->writeChar('W');
        _matrix->writeChar('i');
        _matrix->writeChar('n');
        _matrix->writeChar('d');
        //_matrix->writeChar(' ');
        _matrix->writeChar('W');
        _matrix->writeChar('a');
        _matrix->writeChar('v');
        _matrix->writeChar('e');

        _matrix->setFontSize(2);
        _matrix->setTextCursor(5, 10);
        std::string work = buoyData->getWindSwellDirection();

        for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
        {
          _matrix->writeChar(*itr);
        }

        _matrix->setTextCursor(5, 18);
        work = buoyData->getWindSwellHeight();

        for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
        {
          _matrix->writeChar(*itr);
        }

        _matrix->writeChar('\'');

        _matrix->setTextCursor(5, 26);
        work = buoyData->getWindSwellPeriod();

        for (std::string::iterator itr = work.begin(); itr != work.end(); itr++)
        {
          _matrix->writeChar(*itr);
        }

        _matrix->writeChar('s');

        sleep(WaitBefore);
        _matrix->fadeDisplay();
        //sleep(WaitAfter);

        //TODO: Calculate Angle and Size from Height, Period, and Direction.
        _matrix->drawWedge(16, 11, 8, 180, 270, blue);
        _matrix->fillCircle(16, 11, 1, red); //buoy location

        sleep(WaitBefore);
        _matrix->fadeDisplay();
        sleep(WaitAfter);
      }
        break;

      case 3:
        // TODO: Tide
        break;

      case 4:
        // TODO: Wind
        break;

      case 5:
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
        _matrix->putChar(cursorX, 0, hourString[0], 2, white);

        if (hourString.length() > 1)
        {
          cursorX = SwellFontWidth;
          _matrix->putChar(cursorX, 0, hourString[1], 2, white);
        }

        cursorX += SwellFontWidth + 1; //mateo: added 1
        _matrix->putChar(cursorX, 0, ':', 2, white);

        cursorX += SwellFontWidth; //mateo: - 1; // -1 to make closer to ":"
        _matrix->putChar(cursorX, 0, minString[0], 2, white);
        cursorX += SwellFontWidth;
        _matrix->putChar(cursorX, 0, minString[1], 2, white);


        cursorX += SwellFontWidth + 2; // +2 to give space to AM/PM
        _matrix->putChar(cursorX, 1, amPmString[0], 1, white);
        cursorX += SwellFontWidth;
        _matrix->putChar(cursorX, 1, amPmString[1], 1, white);

        // TODO: Add Sunrise / Sunset

        // * - Sunrise
        _matrix->putChar(4, 12, '*', 3, yellow);


        // ( - Sunset
        _matrix->putChar(4, 20, '(', 3, blue);


        sleep(WaitBefore);
        _matrix->fadeDisplay();
        sleep(WaitAfter);

        break;
      }

      case 6:
        // TODO: Temp: Air / Water
        break;
    }

    //TODO: Sleep

    curStat++;
    if (curStat > NumStats) curStat = 1;
  }
}



