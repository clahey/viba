
/*
 * songState.hh
 *
 *  Created on: Oct 16, 2011
 *      Author: clahey
 */

#ifndef SONGSTATE_HH_
#define SONGSTATE_HH_

struct SongState
{
  bool mLastTime;
  bool operator< (const SongState& other) const {
    return !mLastTime && other.mLastTime;
  }
};

#endif /* SONGSTATE_HH_ */
