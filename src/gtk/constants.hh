/*
 * constants.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_CONSTANTS_HH_
#define GTK_CONSTANTS_HH_

#include "timeDelta.hh"

struct Constants {
  const static int sPixelsPerChange = 100;
  const static int sHeight = 100;

  static int TimeDeltaToPixels(TimeDelta delta, int subPixels = 1);
  static TimeDelta PixelsToTimeDelta(int pixels, int subPixels = 1);
};

#endif /* GTK_CONSTANTS_HH_ */
