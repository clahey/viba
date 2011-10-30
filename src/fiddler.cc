/*
 * fiddler.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "fiddler.hh"

void
Fiddler::FillOutput(std::vector<InstrumentEvent>& output, const SongState::BarData& bar, const SongState& state)
{
  const NoteSequenceData& noteData = state.mTune->GetNotes(bar.mBarNum, state);
  const std::vector<NoteEvent>& notes = noteData.GetData();
  for (std::vector<NoteEvent>::const_iterator it = notes.begin(); it != notes.end(); it++) {
    const NoteEvent& noteEvent = *it;
    int vel = 80;
    if (bar.mBarNum % 4 == 0 && noteEvent.GetOffset() == 0) {
      vel = 115;
    } else if (bar.mBarNum % 2 == 0 && noteEvent.GetOffset() == 0) {
      vel = 100;
    }
    output.push_back(InstrumentEvent(noteEvent, bar.mOffset, mInstrument, vel));
  }
}
