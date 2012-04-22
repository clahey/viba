
/*
 * songState.hh
 *
 *  Created on: Oct 16, 2011
 *      Author: clahey
 */

#ifndef SONGSTATE_HH_
#define SONGSTATE_HH_

#include "property.hh"
#include "tune.hh"

struct BarData {
  BarData(int barNum, TimeDelta offset, TimeDelta start, TimeDelta end,
	  Tune* tune, bool lastTime)
    : mBarNum(barNum),
      mOffset(offset),
      mStart(start),
      mEnd(end),
      mTune(tune),
      mLastTime(lastTime)
  {};

  int mBarNum;
  TimeDelta mOffset;
  TimeDelta mStart;
  TimeDelta mEnd;
  Tune* mTune;
  bool mLastTime;
};

typedef std::vector<BarData> BarList;

struct SongState
{

  struct TuneChange
  {
    TuneChange(Tune* tune, int repeatCount)
      : tune(tune),
	repeatCount(repeatCount) {};
    Tune* tune;
    int repeatCount;

    bool operator==(const TuneChange& other) const {
      return tune == other.tune && repeatCount == other.repeatCount;
    }
  };
  typedef std::vector<TuneChange> TuneList;

  Property<TuneList> pTunes;

  TimeDelta mTuneStart;
  TimeDelta mRepeatStart;

  BarList GetBars(TimeDelta start, TimeDelta end) const;
};



#endif /* SONGSTATE_HH_ */
