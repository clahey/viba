/*
 * timeline.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "gtk/timeline.hh"

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
  mChangeMarkers = Glib::RefPtr<ChangeMarkers>(new ChangeMarkers(mState));
  root->add_child(mChangeMarkers);

  sigc::slot<void> resize = sigc::mem_fun(this, &Timeline::Resize);
  
  mChangeMarkers->pWidth.changed.connect(resize);
  mChangeMarkers->pHeight.changed.connect(resize);
  resize();
}

void
Timeline::Resize()
{
  set_bounds(0, 0, mChangeMarkers->pWidth, mChangeMarkers->pHeight);
}
