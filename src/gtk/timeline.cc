/*
 * timeline.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "timeline.hh"

#include <iostream>

#include "gtk/changeMarkers.hh"
#include "gtk/currentTimeItem.hh"

using namespace std;

Timeline::Timeline(OutputSequence* output, SongState* state)
  : mOutput(output),
    mState(state)
{
  Glib::RefPtr<Goocanvas::Item> root = get_root_item();
  root->add_child
    (Glib::RefPtr<CurrentTimeItem>(new CurrentTimeItem(mOutput)));
  root->add_child
    (Glib::RefPtr<ChangeMarkers>(new ChangeMarkers(mState)));
}
