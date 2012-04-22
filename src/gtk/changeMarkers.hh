/*
 * changeMarkers.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_CHANGE_MARKERS_HH_
#define GTK_CHANGE_MARKERS_HH_

#include <goocanvasmm/group.h>

#include "songState.hh"

/*
 *
 */
class ChangeMarkers : public Goocanvas::Group
{
public:
  ChangeMarkers(SongState* state);

private:
  SongState* mState;

  void AddLine(int position);
};

#endif /* GTK_CHANGE_MARKERS_HH_ */
