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
      int vel = 70;
      if (noteEvent.GetOffset() == 0) {
	if (bar.mBarNum % 16 == 0) {
	  vel = 127;
	} else if (bar.mBarNum % 8 == 0) {
	  vel = 105;
	} else if (bar.mBarNum % 4 == 0) {
	  vel = 90;
	}
      }
    InstrumentEvent event =
      InstrumentEvent(noteEvent, bar.mOffset, mInstrument, vel);
    event.Shorten(TimeDelta::sBar / 512);
    output.push_back(event);
  }
}
