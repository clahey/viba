/*
 * musician.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "musician.hh"

void
Musician::GenerateBar(const SongState::BarData& bar, FluidOutputSequence* outputSequence, const SongState& state)
{
  std::vector<InstrumentEvent> output;
  FillOutput(output, bar, state);
  for (std::vector<InstrumentEvent>::iterator it = output.begin(); it != output.end(); it++) {
    InstrumentEvent& event = *it;
    if (event.GetOffset() >= bar.mStart + bar.mOffset && event.GetOffset() < bar.mEnd + bar.mOffset) {
      event.Randomize(1.0/2048);
      outputSequence->SendInstrumentEvent(&event);
    }
  }
}

void
Musician::Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state)
{
  FluidOutputSequence* output = dynamic_cast<FluidOutputSequence*>(dest);
  assert(output != NULL);
  SongState::BarList bars = state.GetBars(start, end);
  std::for_each (bars.begin(), bars.end(), sigc::bind(sigc::mem_fun(this, &Musician::GenerateBar), output, sigc::ref(state)));
}
