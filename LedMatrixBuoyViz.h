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

  static const int BuoyX = 15;
  static const int BuoyY = 15;

  static const int GsMaxRadius = 15;
  static const int GsMaxAngle = 90;  // Half the max angle

  static const int WsMaxRadius = 10;
  static const int WsMaxAngle = 75;  //Half the max angle

  static const int WaitBefore = 4;
  static const int WaitAfter = 1;


  LedMatrixBuoyViz(RgbMatrix *m, DisplayUpdater *d);

  void run();

private:

  Color _red;
  Color _green;
  Color _blue;
  Color _purple;
  Color _turquoise;
  Color _yellow;
  Color _white;

  bool _explainMode;

  BuoyData *_buoyData;

  DisplayUpdater *_updater;  //access to updater to implement fadeIn effect

  void introduceDataViz();
  void groundSwell();
  void windSwell();
  void groundAndWindSwell();

  void sunriseSunset();


  //Helpers
  void displayTitle(Color firstLineColor, Color secondLineColor,
                    std::string firstLine, std::string secondLine);

  void displayGroundSwellViz();
  void displayWindSwellViz();


};
