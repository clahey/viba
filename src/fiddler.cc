/*
 * fiddler.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "fiddler.hh"

#include "tune.hh"

void
Fiddler::FillOutput(std::vector<InstrumentEventPtr>& output, const BarData& bar, const SongState& state)
{
  const NoteSequenceData& noteData = bar.mTune->GetNotes(bar, state);
  const std::vector<NoteEvent>& notes = noteData.GetData();
  for (std::vector<NoteEvent>::const_iterator it = notes.begin(); it != notes.end(); it++) {
    const NoteEvent& noteEvent = *it;
    TimeDelta offset = bar.mOffset + noteEvent.GetOffset();
    TimeDelta duration = noteEvent.GetLength()/2;
    int vel = 85;
    
    // emphasize the offbeat
    if (noteEvent.GetOffset().GetTicks()/720 % 4 == 2) {
      vel = 127;
      offset -= 30;
    }

    vel *= state.pVolume.Get(bar.mStart + offset);
      InstrumentEventPtr event = InstrumentEvent::create(noteEvent.GetNote(), duration, offset, mInstrument, vel);
    output.push_back(event);
  }
}
