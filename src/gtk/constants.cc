#include "constants.hh"

const int Constants::sPixelsPerChange;

int
Constants::TimeDeltaToPixels(TimeDelta delta,
			     int subPixels)
{
  return delta * subPixels * sPixelsPerChange
    / TimeDelta::sBarsPerChange
    / TimeDelta::sBar;
}

TimeDelta
Constants::PixelsToTimeDelta(int pixels,
			     int subPixels)
{
  return TimeDelta::sBar * pixels * TimeDelta::sBarsPerChange
    / Constants::sPixelsPerChange / subPixels;
}

