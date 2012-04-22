/*
 * timeline.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef GTK_TIMELINE_HH_
#define GTK_TIMELINE_HH_

#include <goocanvasmm/canvas.h>

#include "timeDelta.hh"
#include "songState.hh"
#include "outputSequence.hh"

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
};

#endif /* GTK_TIMELINE_HH_ */
