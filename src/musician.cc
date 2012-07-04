/*
 * musician.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "musician.hh"

#include "fluidOutputSequence.hh"
#include "random.hh"
#include <cassert>

void
Musician::GenerateBar(const BarData& bar, FluidOutputSequence* outputSequence, const SongState& state)
{
  std::vector<InstrumentEventPtr> output;
  FillOutput(output, bar, state);
  for (std::vector<InstrumentEventPtr>::iterator it = output.begin(); it != output.end(); it++) {
    InstrumentEventPtr& event = *it;
    if (event->GetOffset() >= bar.mStart + bar.mOffset && event->GetOffset() < bar.mEnd + bar.mOffset) {
      const TimeSlip& startSlip = GetTimeSlip(event->GetOffset());
      const TimeSlip& endSlip =
	GetTimeSlip(event->GetOffset() + event->GetLength());
      TimeDelta startDelay = TimeDelta::sBar * GetNormalDistribution(1.0 / 2048, 0);
      TimeDelta endDelay = TimeDelta::sBar * GetNormalDistribution(1.0 / 2048, 0);
      event->SetOffset(event->GetOffset() + startSlip.first + startDelay);
      // Move the end by the end slip offset and subtract the end slip gap.
      event->SetLength(event->GetLength()/2 - startSlip.first - startDelay
		       + endSlip.first - endSlip.second - endDelay);
      outputSequence->SendInstrumentEvent(event);
    }
  }
}

void
Musician::Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state)
{
  FluidOutputSequence* output = dynamic_cast<FluidOutputSequence*>(dest);
  assert(output != NULL);
  BarList bars = state.GetBars(start, end);
  std::for_each (bars.begin(), bars.end(), sigc::bind(sigc::mem_fun(this, &Musician::GenerateBar), output, sigc::ref(state)));
  CleanTimeSlipMap(start - TimeDelta::sBar * 32);
}

const Musician::TimeSlip&
Musician::GetTimeSlip(TimeDelta time)
  const
{
  const static double gap = 1.0/1024;
  TimeSlipMap::iterator it = mTimeSlipMap.find(time);
  if (it == mTimeSlipMap.end()) {
    mTimeSlipMap[time] =
      std::make_pair(TimeDelta::sBar * GetNormalDistribution(1.0 / 512),
		     TimeDelta::sBar * (GetNormalDistribution(gap, gap) - gap));
  }
  return mTimeSlipMap[time];
}

void
Musician::CleanTimeSlipMap(TimeDelta time)
{
  mTimeSlipMap.erase(mTimeSlipMap.begin(), mTimeSlipMap.upper_bound(time));
}
