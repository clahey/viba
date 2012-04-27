/*
 * currentTimeItem.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "currentTimeItem.hh"

#include <goocanvasmm/polyline.h>
#include <iostream>

#include "gtk/constants.hh"

using namespace std;

CurrentTimeItem::CurrentTimeItem(OutputSequence* output)
  : Polyline(0, 0, 0, 0),
    mOutput(output)
{
  SetPosition();
}

const static int sSubPixelsPerPixel = 4;

void
CurrentTimeItem::SetPosition()
{
  int position = Constants::TimeDeltaToPixels(mOutput->GetCurrentTime(),
					      sSubPixelsPerPixel);
  Goocanvas::Points points = property_points();
  points.set_coordinate(0, position / (float) sSubPixelsPerPixel, 0);
  points.set_coordinate(1,
			position / (float) sSubPixelsPerPixel,
			Constants::sHeight);
  property_points() = points;
  TimeDelta nextTime = Constants::PixelsToTimeDelta(position + 1,
						    sSubPixelsPerPixel) + 1;
  mOutput->ScheduleCallback(nextTime, 
			    sigc::mem_fun(this, &CurrentTimeItem::SetPosition));
}
