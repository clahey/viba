/*
 * changeMark.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_CHANGE_MARK_HH_
#define GTK_CHANGE_MARK_HH_

#include <goocanvasmm/group.h>

#include "songState.hh"
#include "timeDelta.hh"
#include "property.hh"

/*
 *
 */
class ChangeMark : public Goocanvas::Group
{
public:
  ChangeMark(SongState* state);
  Property<int> pPosition;
  Property<int> pMinPosition;
  sigc::signal<void, int> requestPosition;

private:
  Glib::RefPtr<Goocanvas::Item> mBar;
  Glib::RefPtr<Goocanvas::Item> mDraggable;

  SongState* mState;
  bool mDragging;

  int mXBase;
  int mXMotion;

  void UpdateDisplay();
  bool OnButtonPressEvent(const Glib::RefPtr<Item>& target,
			     GdkEventButton* event);
  bool OnButtonReleaseEvent(const Glib::RefPtr<Item>& target,
			       GdkEventButton* event);
  bool OnMotionNotifyEvent(const Glib::RefPtr<Item>& target,
			      GdkEventMotion* event);
};

#endif /* GTK_CHANGE_MARK_HH_ */
