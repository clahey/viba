/*
 * changeMarkers.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "changeMarkers.hh"

#include <goocanvasmm/polyline.h>
#include <iostream>

#include "gtk/changeMark.hh"
#include "gtk/constants.hh"

using namespace std;

ChangeMarkers::ChangeMarkers(SongState *state)
  : Group(),
    mState(state)
{
  sigc::slot<void> build = sigc::mem_fun(this, &ChangeMarkers::Build);
  mState->pTunes.changed.connect(build);
  build();
}

void
ChangeMarkers::Build()
{ 
  while(get_n_children () > 0) {
    remove_child(0);
  }
  mItemVector.clear();

  TimeDelta delta = mState->mRepeatStart;
  const SongState::TuneList& tunes = mState->pTunes;
  SongState::TuneList::const_iterator it = tunes.begin();
  int change = 0;
  int tune = 0;
  for (it = tunes.begin(); it != tunes.end(); it++) {
    for (int i = 0; i < it->repeatCount; i++) {
      AddLine(Constants::TimeDeltaToPixels(delta));
      delta += TimeDelta::sBar * TimeDelta::sBarsPerChange;
      change++;
    }
    Glib::RefPtr<ChangeMark> mark =
      Glib::RefPtr<ChangeMark>(new ChangeMark(mState));
    add_child(mark);
    mark->pPosition = change;
    mItemVector.push_back(mark);
    mark->requestPosition.connect(sigc::bind(sigc::mem_fun(this, &ChangeMarkers::OnRequestPosition), tune));
    tune ++;
  }
  AddLine(Constants::TimeDeltaToPixels(delta));
}

void
ChangeMarkers::AddLine(int position)
{
  add_child(Goocanvas::Polyline::create(position, 0,
					position, Constants::sHeight));
}

void
ChangeMarkers::OnRequestPosition(int position,
				 int tune)
{
  SongState::TuneList tunes = mState->pTunes;
  if (tune == 0) {
    tunes[tune].repeatCount = position;
  } else {
    tunes[tune].repeatCount = position - mItemVector[tune - 1]->pPosition;
  }
  mState->pTunes = tunes;
}
