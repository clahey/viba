/*
 * changeMarkers.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "changeMarkers.hh"

#include <goocanvasmm/polyline.h>
#include <iostream>

#include "gtk/constants.hh"

using namespace std;

ChangeMarkers::ChangeMarkers(SongState *state)
  : Group(),
    mState(state)
{
  TimeDelta delta = mState->mRepeatStart;
  const SongState::TuneList& tunes = mState->pTunes;
  SongState::TuneList::const_iterator it = tunes.begin();
  for (it = tunes.begin(); it != tunes.end(); it++) {
    for (int i = 0; i < it->repeatCount; i++) {
      AddLine(Constants::TimeDeltaToPixels(delta));
      delta += TimeDelta::sBar * TimeDelta::sBarsPerChange;
    }
  }
  AddLine(Constants::TimeDeltaToPixels(delta));
}

void
ChangeMarkers::AddLine(int position)
{
  add_child(Goocanvas::Polyline::create(position, 0,
					position, Constants::sHeight));
}
