// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "BuoyData.h"
#include "DisplayUpdater.h"
#include "HttpRequest.h"
#include "RgbMatrix.h"
#include "RgbMatrixContainer.h"
#include "Thread.h"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

#include <stdio.h>
#include <unistd.h>



// Data Visualization of CDIP Buoy Data on a 32x32 RGB LED Matrix.
class LedMatrixBuoyViz : public RgbMatrixContainer
{
public:
  LedMatrixBuoyViz(RgbMatrix *m) : RgbMatrixContainer(m) {}

  static const int SwellFontSize = 2;
  static const int SwellFontWidth = 4;

  static const int WaitBefore = 3;
  static const int WaitAfter = 1;

  void run()
  {
    Color red;
    red.red = 255;

    Color green;
    green.green = 255;

    Color blue;
    blue.blue = 255;

    Color white;
    white.red = 255;
    white.green = 255;
    white.blue = 255;

    Color yellow;
    yellow.red = 255;
    yellow.green = 255;

    // Different Stats to Display
    //   1: Primary Swell
    //   2: Seconday Swell
    //   3: Tide
    //   4: Wind
    //   5: Time & Sunrise/Sunset
    //   6: Temp: Air / Water
    const int NumStats = 6;
    int curStat = 1;

    //Get the Buoy data from a web-service/API.
    BuoyData *buoyData = new BuoyData();    
    buoyData->getBuoyData();

    while (!isDone())
    {
      switch (curStat)
      {
        case 1:
          // Primary Swell

          //TODO: Get the largest swell from dataTable...
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


          _matrix->putChar(0, 26, 'W', 2, green);

          _matrix->putChar(7,  26, '1',  2, green);
          _matrix->putChar(11, 26, '0',  2, green);
          _matrix->putChar(15, 26, '\'', 2, green);

          _matrix->putChar(20, 26, '1', 2, green);
          _matrix->putChar(24, 26, '2', 2, green);
          _matrix->putChar(29, 27, 's', 1, green);

          _matrix->drawWedge(16, 11, 12, 135, 225, green);
          _matrix->fillCircle(16, 11, 1, red); //buoy location

          sleep(WaitBefore);
          _matrix->fadeDisplay();
          sleep(WaitAfter);

         break;

        case 2:
	  // Secondary Swell
	  _matrix->putChar(0, 26, 'N', 2, blue);
	  _matrix->putChar(5, 26, 'W', 2, blue);

	  _matrix->putChar(14, 26, '3',  2, blue);
	  _matrix->putChar(18, 26, '\'', 2, blue);

	  _matrix->putChar(24, 26, '5', 2, blue);
	  _matrix->putChar(29, 27, 's', 1, blue);

	  _matrix->drawWedge(16, 11, 8, 180, 270, blue);
	  _matrix->fillCircle(16, 11, 1, red); //buoy location

          sleep(WaitBefore);
          _matrix->fadeDisplay();
          sleep(WaitAfter);

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

          cursorX += SwellFontWidth;
          _matrix->putChar(cursorX, 0, ':', 2, white);

          cursorX += SwellFontWidth - 1; // -1 to make closer to ":"
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


private:

};


//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  GpioProxy io;

  if (!io.initialize())
    return 1;

  RgbMatrix *m = NULL;

  m = new RgbMatrix(&io);

  RgbMatrixContainer *display = NULL;
  RgbMatrixContainer *updater = NULL;

  display = new LedMatrixBuoyViz(m);
  updater = new DisplayUpdater(m);

  printf("\nRunning the Buoy Data Visualization.\n\n");

  updater->start(10);
  display->start();

  printf("Press <RETURN> when done.\n");
  getchar();

  printf("Signaling visualization to stop.\n\n");

  // Stop threads and wait for them to join.
  if (display) delete display;
  if (updater) delete updater;

  // Clear and refresh the display.
  m->clearDisplay();
  m->updateDisplay();

  delete m;

  return 0;
}

