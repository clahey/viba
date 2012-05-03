
#include "songState.hh"

#include <algorithm>

SongState::SongState()
 : pVolume(1)
{
}

BarList
SongState::GetBars(TimeDelta start, TimeDelta end)
  const
{
  BarList retval;
  // Since start < barEnd, start gets larger every time through, and since start and end are integers, this loop ends.
  while (start < end) {
    int bar = static_cast<int>((start - mRepeatStart) / TimeDelta::sBar);
    int repeat;
    TimeDelta barStart;
    TimeDelta barEnd;
    if (bar < 0) {
      /*
       * bar = floor((start - mRepeatStart) / TimeDelta::sBar)
       * x-1 < floor(x) <= x
       * Because they're integers: floor((start - mRepeatStart) / TimeDelta::sBar) <= -1
       * (start - mRepeatStart) / TimeDelta::sBar - 1 < bar <= -1
       * (start - mRepeatStart) / TimeDelta::sBar < 0
       * start - mRepeatStart < 0
       * start < mRepeatStart
       * start < barEnd
       */
      barStart = mTuneStart;
      barEnd = mRepeatStart;
      repeat = -1;
      bar = -1;
    } else {
      /*
       * bar = floor((start - mRepeatStart) / TimeDelta::sBar)
       * x-1 < floor(x) <= x
       * bar > (start - mRepeatStart) / TimeDelta::sBar - 1
       * bar + 1 > (start - mRepeatStart) / TimeDelta::sBar
       * (TimeDelta::sBar > 0)
       * TimeDelta::sBar * (bar + 1) > start - mRepeatStart
       * mRepeatStart + TimeDelta::sBar * (bar + 1) > start
       * start < barEnd
       */
      barStart = mRepeatStart + TimeDelta::sBar * bar;
      barEnd = mRepeatStart + TimeDelta::sBar * (bar + 1);
      repeat = bar / TimeDelta::sBarsPerChange;
      bar = bar % TimeDelta::sBarsPerChange;
    }

    TuneList::const_iterator it = pTunes.Get().begin();
    int repeatCount = 0;
    for (it = pTunes.Get().begin(); it != pTunes.Get().end(); it++) {
      if (repeat < repeatCount + it->repeatCount) {
	TimeDelta thisStart = std::max(start - barStart, TimeDelta(0));
	TimeDelta thisEnd = std::min(end, barEnd) - barStart;
	bool lastTime = repeat == repeatCount + it->repeatCount - 1;
	if (thisStart < thisEnd) {
	  retval.push_back(BarData(bar, barStart,
				   thisStart, thisEnd,
				   it->tune, lastTime));
	}
	break;
      }
      repeatCount += it->repeatCount;
    }

    start = barEnd;
  }
  return retval;
}
