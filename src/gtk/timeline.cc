/*
 * timeline.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "timeline.hh"

#include <iostream>

#include "gtk/currentTimeItem.hh"

using namespace std;

Timeline::Timeline(OutputSequence* output)
  : mOutput(output)
{
  Glib::RefPtr<CurrentTimeItem> item = Glib::RefPtr<CurrentTimeItem>(new CurrentTimeItem(mOutput));
  get_root_item()->add_child(item);
}
