/*
 * pianist.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "pianist.hh"

void
Pianist::GenerateBar(const SongState::BarData& bar, FluidOutputSequence* output, const SongState& state)
{
  const NoteSequenceData& noteData = state.mTune->GetNotes(bar.mBarNum, state);

  const std::vector<NoteEvent>& notes = noteData.GetData();
  std::vector<NoteEvent>::const_iterator it;
  for (it = notes.begin(); it != notes.end(); it++) {
    const NoteEvent& noteEvent = *it;
    if (noteEvent.GetOffset() >= bar.mStart && noteEvent.GetOffset() < bar.mEnd) {
      InstrumentEvent event(noteEvent, bar.mOffset, mInstrument, 100);
      event.Randomize(1.0/2048);
      output->SendInstrumentEvent(&event);
    }
  }
}

void
Pianist::Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state)
{
  FluidOutputSequence* output = dynamic_cast<FluidOutputSequence*>(dest);
  assert(output != NULL);
  SongState::BarList bars = state.GetBars(start, end);
  std::for_each (bars.begin(), bars.end(), sigc::bind(sigc::mem_fun(this, &Pianist::GenerateBar), output, sigc::ref(state)));
}
