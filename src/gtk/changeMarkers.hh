/*
 * changeMarkers.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_CHANGE_MARKERS_HH_
#define GTK_CHANGE_MARKERS_HH_

#include <goocanvasmm/group.h>
#include <utility>
#include <vector>

#include "gtk/changeMark.hh"
#include "songState.hh"

/*
 *
 */
class ChangeMarkers : public Goocanvas::Group
{
public:
  ChangeMarkers(SongState* state);

private:
  typedef std::vector<Glib::RefPtr<ChangeMark> > ItemVector;

  SongState* mState;
  ItemVector mItemVector;

  void Build();
  void OnRequestPosition(int position, int tune);
  void AddLine(int position);
};

#endif /* GTK_CHANGE_MARKERS_HH_ */
