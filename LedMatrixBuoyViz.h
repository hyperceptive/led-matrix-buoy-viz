// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "BuoyData.h"
#include "DisplayUpdater.h"
#include "RgbMatrix.h"
#include "RgbMatrixContainer.h"


// Data Visualization of Buoy Data on a 32x32 RGB LED Matrix.
class LedMatrixBuoyViz : public RgbMatrixContainer
{
public:

  static const int SwellFontWidth = 4;

  static const int WaitBefore = 4;
  static const int WaitAfter = 1;


  LedMatrixBuoyViz(RgbMatrix *m, DisplayUpdater *d);

  void run();

private:

  Color _red;
  Color _green;
  Color _blue;
  Color _purple;
  Color _yellow;
  Color _white;

  BuoyData *_buoyData;

  DisplayUpdater *_updater;  //access to updater to implement fadeIn effect

  void groundSwell();
  void windSwell();
  void groundAndWindSwell();

  void sunriseSunset();

};
