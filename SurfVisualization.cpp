// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "DisplayUpdater.h"
#include "LedMatrixBuoyViz.h"
#include "RgbMatrix.h"
#include "RgbMatrixContainer.h"

#include <cstdio>


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

  printf("Press <RETURN> to suspend updater thread.\n");
  getchar();
  updater->suspend();

  printf("Press <RETURN> to resume updater thread.\n");
  getchar();
  updater->resume();

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
