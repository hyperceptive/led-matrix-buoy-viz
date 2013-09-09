// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "RgbMatrix.h"
#include "RgbMatrixContainer.h"


// Data Visualization of Buoy Data on a 32x32 RGB LED Matrix.
class LedMatrixBuoyViz : public RgbMatrixContainer
{
public:
  LedMatrixBuoyViz(RgbMatrix *m) : RgbMatrixContainer(m) {}

  static const int SwellFontWidth = 4;

  static const int WaitBefore = 3;
  static const int WaitAfter = 1;

  void run();

private:

};
