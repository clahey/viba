/*
 * changeMark.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "changeMark.hh"

#include <iostream>
#include <goocanvasmm/rect.h>

#include "gtk/constants.hh"

using namespace std;

ChangeMark::ChangeMark(SongState *state)
  : Group(),
    mBar(Goocanvas::Rect::create(-5, 0, 10, 100)),
    mDraggable(Goocanvas::Rect::create(-5, 0, 10, 100)),
    mState(state),
    mDragging(false)
{
  sigc::slot<void> update = sigc::mem_fun(this, &ChangeMark::UpdateDisplay);
  pPosition.changed.connect(update);
  update();
  signal_button_press_event().connect(sigc::mem_fun(this, &ChangeMark::OnButtonPressEvent));
  signal_button_release_event().connect(sigc::mem_fun(this, &ChangeMark::OnButtonReleaseEvent));
  signal_motion_notify_event().connect(sigc::mem_fun(this, &ChangeMark::OnMotionNotifyEvent));
  add_child(mBar);
}

void
ChangeMark::UpdateDisplay()
{
  TimeDelta position;
  TimeDelta repeat = TimeDelta::sBar * TimeDelta::sBarsPerChange;
  position = repeat * pPosition + mState->mRepeatStart;
  if (mDragging) {
    TimeDelta offset = Constants::PixelsToTimeDelta(mXMotion - mXBase);
    if (offset > 0) {
      offset = repeat * int((offset + repeat / 2) / repeat);
    }
    if (offset < 0) {
      offset = repeat * int((offset - repeat / 2) / repeat);
    }
    position += offset;
  }
  mBar->set_simple_transform(Constants::TimeDeltaToPixels(position), 0, 1, 0);
}

bool
ChangeMark::OnButtonPressEvent(const Glib::RefPtr<Goocanvas::Item>& target,
				  GdkEventButton* event)
{
  mXBase = event->x_root;
  mXMotion = event->x_root;
  mDragging = true;
  return true;
}

bool
ChangeMark::OnButtonReleaseEvent(const Glib::RefPtr<Goocanvas::Item>& target,
				    GdkEventButton* event)
{
  mDragging = false;
  mXMotion = event->x_root;

  TimeDelta repeat = TimeDelta::sBar * TimeDelta::sBarsPerChange;
  TimeDelta offset = Constants::PixelsToTimeDelta(mXMotion - mXBase);
  int result = 0;
  if (offset > 0) {
    result = (offset + repeat / 2) / repeat;
  }
  if (offset < 0) {
    result = (offset - repeat / 2) / repeat;
  }
  requestPosition(pPosition + result);
  UpdateDisplay();
  return true;
}

bool
ChangeMark::OnMotionNotifyEvent(const Glib::RefPtr<Goocanvas::Item>& target,
				   GdkEventMotion* event)
{
  if (mDragging) {
    mXMotion = event->x_root;
  }
  UpdateDisplay();
  return true;
}
