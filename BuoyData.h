// Copyright (c) 2013 Hyperceptive, LLC
// Use of this source code is governed by The MIT License
// that can be found in the LICENSE file.

#include "Thread.h"

#include <string>


struct BuoyInfo {
    std::string date;
    std::string time;
    std::string groundSwellHeight;
    std::string groundSwellPeriod;
    std::string groundSwellDirection;
    std::string windSwellHeight;
    std::string windSwellPeriod;
    std::string windSwellDirection;
};

// Data Visualization of CDIP Buoy Data on a 32x32 RGB LED Matrix.
class BuoyData : public Thread
{
public:
  BuoyData();

  ~BuoyData();

  void getBuoyData();

  void run();

  // Getters
  std::string getDate() const { return _latestBuoyInfo.date; }
  std::string getTime() const { return _latestBuoyInfo.time; }
  std::string getGroundSwellHeight() const { return _latestBuoyInfo.groundSwellHeight; }
  std::string getGroundSwellPeriod() const { return _latestBuoyInfo.groundSwellPeriod; }
  std::string getGroundSwellDirection() const { return _latestBuoyInfo.groundSwellDirection; }
  std::string getWindSwellHeight() const { return _latestBuoyInfo.windSwellHeight; }
  std::string getWindSwellPeriod() const { return _latestBuoyInfo.windSwellPeriod; }
  std::string getWindSwellDirection() const { return _latestBuoyInfo.windSwellDirection; }

  // Setters
  void setDate(std::string d) { _latestBuoyInfo.date = d; }
  void setTime(std::string t) { _latestBuoyInfo.time = t; }
  void setGroundSwellHeight(std::string gsh) { _latestBuoyInfo.groundSwellHeight = gsh; }
  void setGroundSwellPeriod(std::string gsp) { _latestBuoyInfo.groundSwellPeriod = gsp; }
  void setGroundSwellDirection(std::string gsd) { _latestBuoyInfo.groundSwellDirection = gsd; }
  void setWindSwellHeight(std::string wsh) { _latestBuoyInfo.windSwellHeight = wsh; }
  void setWindSwellPeriod(std::string wsp) { _latestBuoyInfo.windSwellPeriod = wsp; }
  void setWindSwellDirection(std::string wsd) { _latestBuoyInfo.windSwellDirection = wsd; }

  // Conversion Methods
  std::string convertMetersToFeet(std::string meters);
  float convertCompassPointToDegrees(std::string cp);

  std::string round(std::string realNumStr);


private:

  BuoyInfo _latestBuoyInfo;

};

