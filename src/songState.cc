
#include "songState.hh"

#include <algorithm>

SongState::BarList
SongState::GetBars(TimeDelta start, TimeDelta end)
  const
{
  BarList retval;
  // Since start < barEnd, start gets larger every time through, and since start and end are integers, this loop ends.
  while (start < end) {
    int bar = static_cast<int>((start - mRepeatStart) / TimeDelta::sBar);
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
      bar = -1;
      barStart = mTuneStart;
      barEnd = mRepeatStart;
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
      bar %= 32;
    }

    TimeDelta thisStart = std::max(start - barStart, TimeDelta(0));
    TimeDelta thisEnd = std::min(end, barEnd) - barStart;
    if (thisStart < thisEnd) {
      retval.push_back(BarData(bar, barStart, thisStart, thisEnd));
    }
    start = barEnd;
  }
  return retval;
}
