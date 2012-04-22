/*
 * changeMarkers.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_CHANGE_MARKERS_HH_
#define GTK_CHANGE_MARKERS_HH_

#include <goocanvasmm/itemgroup.h>

#include "timeDelta.hh"
#include "outputSequence.hh"

/*
 *
 */
class ChangeMarkers : public Goocanvas::Group
{
public:
  ChangeMarkers(SongState* state);

private:
  SongState* mState;
};

#endif /* GTK_CHANGE_MARKERS_HH_ */
