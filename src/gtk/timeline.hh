/*
 * timeline.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_TIMELINE_HH_
#define GTK_TIMELINE_HH_

#include <goocanvasmm/canvas.h>

#include "types.hh"
#include "gtk/gtkTypes.hh"

/*
 *
 */
class Timeline : public Goocanvas::Canvas
{
public:
  Timeline(OutputSequence* output, SongState* state);

private:
  OutputSequence* mOutput;
  SongState* mState;
  Glib::RefPtr<ChangeMarkers> mChangeMarkers;
  
  void Resize();
};

#endif /* GTK_TIMELINE_HH_ */
