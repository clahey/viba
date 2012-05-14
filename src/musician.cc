/*
 * musician.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "musician.hh"

#include "fluidOutputSequence.hh"

void
Musician::GenerateBar(const BarData& bar, FluidOutputSequence* outputSequence, const SongState& state)
{
  std::vector<InstrumentEventPtr> output;
  FillOutput(output, bar, state);
  for (std::vector<InstrumentEventPtr>::iterator it = output.begin(); it != output.end(); it++) {
    InstrumentEventPtr& event = *it;
    if (event->GetOffset() >= bar.mStart + bar.mOffset && event->GetOffset() < bar.mEnd + bar.mOffset) {
      event->Randomize(1.0/512);
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
}
