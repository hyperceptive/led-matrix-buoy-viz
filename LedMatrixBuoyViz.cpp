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

    //Dominant Swell
    _matrix->putChar(0, 0, 'W', 2, green);

    _matrix->putChar(7,  0, '1',  2, green);
    _matrix->putChar(11, 0, '0',  2, green);
    _matrix->putChar(15, 0, '\'', 2, green);

    _matrix->putChar(20, 0, '1', 2, green);
    _matrix->putChar(24, 0, '2', 2, green);
    _matrix->putChar(29, 1, 's', 1, green);


    //Secondary Swell
    _matrix->putChar(0, 26, 'N', 2, blue);
    _matrix->putChar(5, 26, 'W', 2, blue);

    _matrix->putChar(14, 26, '3',  2, blue);
    _matrix->putChar(18, 26, '\'', 2, blue);

    _matrix->putChar(24, 26, '5', 2, blue);
    _matrix->putChar(29, 27, 's', 1, blue);


/*
    _matrix->setTextCursor(0, 27);
    _matrix->setFontColor(blue);
    _matrix->writeLetter('N');
    _matrix->writeLetter('W');
    _matrix->writeLetter(' ');
    _matrix->writeLetter('3');
    _matrix->writeLetter('\'');
    _matrix->writeLetter(' ');
    _matrix->writeLetter('5');
    _matrix->writeLetter('s');
*/

    _matrix->drawWedge(16, 16, 8, 180, 270, blue);
    _matrix->drawWedge(16, 16, 12, 135, 225, green);
/*
    Color white;
    white.red = 80;
    white.green = 80;
    white.blue = 80;

    _matrix->drawLine(3, 16, 16, 16, white);
*/
    _matrix->fillCircle(16, 16, 1, red);
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

  // Stop threads and wait for them to join.
  if (display) delete display;
  if (updater) delete updater;

  // Clear and refresh the display.
  m->clearDisplay();
  m->updateDisplay();

  delete m;

  return 0;
}

