
/*
 * songState.hh
 *
 *  Created on: Oct 16, 2011
 *      Author: clahey
 */

#ifndef SONGSTATE_HH_
#define SONGSTATE_HH_

#include "tune.hh"

struct SongState
{
  bool mLastTime;
  Tune *tune;
  TimeDelta tuneStart;
  TimeDelta repeatStart;
};

#endif /* SONGSTATE_HH_ */
