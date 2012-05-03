/*
 * changeMarkers.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "changeMarkers.hh"

#include <goocanvasmm/polyline.h>
#include <goocanvasmm/text.h>
#include <iostream>
#include <sstream>

#include "gtk/changeMark.hh"
#include "gtk/constants.hh"
#include "tune.hh"

using namespace std;

ChangeMarkers::ChangeMarkers(SongState *state)
  : Group(),
    pHeight(100),
    pWidth(10),
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
    AddText(Constants::TimeDeltaToPixels(delta), it->tune->GetName(), true, .5);
    for (int i = 0; i < it->repeatCount; i++) {
      AddLine(Constants::TimeDeltaToPixels(delta));
      AddText(Constants::TimeDeltaToPixels(delta), change + 1);
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
  pWidth = Constants::TimeDeltaToPixels(delta) + 6;
}

void
ChangeMarkers::AddLine(int position)
{
  add_child(Goocanvas::Polyline::create(position, 0,
					position, Constants::sHeight));
}

void
ChangeMarkers::AddText(int position, int number, bool top, double scale)
{
  ostringstream output;
  output << number;
  AddText(position, output.str(), top, scale);
}

static const int sPadding = 7;

void
ChangeMarkers::AddText(int position,
		       const Glib::ustring& string,
		       bool top,
		       double scale)
{
  Glib::RefPtr<Goocanvas::Text> text = Goocanvas::Text::create(string);
  if (top) {
    text->translate(position + sPadding, 0);
  } else {
    text->property_anchor() = Gtk::ANCHOR_SOUTH_WEST;
    text->translate(position + sPadding, 100);
  }

  text->property_width() = (100 - sPadding * 2) / scale;
  text->property_ellipsize() = Pango::ELLIPSIZE_END;
  //  text->property_wrap() = Pango::WRAP_WORD;
  text->scale(scale, scale);
  add_child(text);
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
