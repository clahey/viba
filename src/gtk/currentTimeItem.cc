/*
 * currentTimeItem.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "currentTimeItem.hh"

#include <goocanvasmm/polyline.h>
#include <iostream>

using namespace std;

CurrentTimeItem::CurrentTimeItem(OutputSequence* output)
  : Polyline(0, 0, 0, 0),
    mOutput(output)
{
  SetPosition();
}

const static int sSubPixelsPerPixel = 4;
const static int sPixelsPerChange = 100;

void
CurrentTimeItem::SetPosition()
{
  int position = mOutput->GetCurrentTime()
    * sSubPixelsPerPixel * sPixelsPerChange / TimeDelta::sBarsPerChange
    / TimeDelta::sBar;
  Goocanvas::Points points = property_points();
  points.set_coordinate(0, position / (float) sSubPixelsPerPixel, 0);
  points.set_coordinate(1, position / (float) sSubPixelsPerPixel, 100);
  property_points() = points;
  TimeDelta nextTime = TimeDelta::sBar * (position + 1)
    * TimeDelta::sBarsPerChange / sPixelsPerChange / sSubPixelsPerPixel + 1;
  mOutput->ScheduleCallback(nextTime, 
			    sigc::mem_fun(this, &CurrentTimeItem::SetPosition));
}
