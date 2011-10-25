
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
  struct BarData {
    BarData(int barNum, TimeDelta offset, TimeDelta start, TimeDelta end)
      : mBarNum(barNum), mOffset(offset), mStart(start), mEnd(end) {};
    int mBarNum;
    TimeDelta mOffset;
    TimeDelta mStart;
    TimeDelta mEnd;
  };
  typedef std::vector<BarData> BarList;

  bool mLastTime;
  Tune *mTune;
  TimeDelta mTuneStart;
  TimeDelta mRepeatStart;

  BarList GetBars(TimeDelta start, TimeDelta end) const;
};



#endif /* SONGSTATE_HH_ */
