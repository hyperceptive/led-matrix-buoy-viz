// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "DisplayUpdater.h"
#include "RgbMatrix.h"
#include "RgbMatrixContainer.h"
#include "Thread.h"

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>


// Data Visualization of CDIP Buoy Data on a 32x32 RGB LED Matrix.
class LedMatrixBuoyViz : public RgbMatrixContainer
{
public:
  LedMatrixBuoyViz(RgbMatrix *m) : RgbMatrixContainer(m) {}

  static const int SwellFontSize = 2;
  static const int SwellFontWidth = 4;

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

    // Different Stats to Display
    //   1: Primary Swell
    //   2: Seconday Swell
    //   3: Tide
    //   4: Wind
    //   5: Time & Sunrise/Sunset
    //   6: Temp: Air / Water
    const int NumStats = 6;
    int curStat = 1;

    while (!isDone())
    {
      switch (curStat)
      {
        case 1:
          // Dominant Swell
          _matrix->putChar(0, 26, 'W', 2, green);

          _matrix->putChar(7,  26, '1',  2, green);
          _matrix->putChar(11, 26, '0',  2, green);
          _matrix->putChar(15, 26, '\'', 2, green);

          _matrix->putChar(20, 26, '1', 2, green);
          _matrix->putChar(24, 26, '2', 2, green);
          _matrix->putChar(29, 27, 's', 1, green);

          _matrix->drawWedge(16, 11, 12, 135, 225, green);
          _matrix->fillCircle(16, 11, 1, red); //buoy location


          sleep(5);
          _matrix->fadeDisplay();
          sleep(1);

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

          sleep(5);
          _matrix->fadeDisplay();
          sleep(1);

          break;

        case 3:
          // TODO: Tide
          break;

        case 4:
          // TODO: Wind
          break;

        case 5:
          // TODO: Time & Sunrise / Sunset
          _matrix->setTextCursor(0, 0);
          _matrix->setFontSize(2);
          _matrix->setFontColor(white);

          _matrix->writeChar('9');
          _matrix->writeChar(':');
          _matrix->writeChar('5');
          _matrix->writeChar('2');

          _matrix->setTextCursor(21, 1);
          _matrix->setFontSize(1);
          _matrix->writeChar('P');
          _matrix->writeChar('M'); 

          sleep(5);
          _matrix->fadeDisplay();
          sleep(1);

          break;

        case 6:
          // TODO: Temp: Air / Water
          break;
      }

      //TODO: Sleep

      curStat++;
      if (curStat > NumStats) curStat = 1;
    }
  }

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

