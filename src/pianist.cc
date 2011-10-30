/*
 * pianist.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "pianist.hh"

void
Pianist::FillOutput(std::vector<InstrumentEvent>& output, const SongState::BarData& bar, const SongState& state)
{
  if (mPlayMelody) {
    const NoteSequenceData& noteData = state.mTune->GetNotes(bar.mBarNum, state);
    const std::vector<NoteEvent>& notes = noteData.GetData();
    for (std::vector<NoteEvent>::const_iterator it = notes.begin(); it != notes.end(); it++) {
      const NoteEvent& noteEvent = *it;
      int vel = 70;
      if (noteEvent.GetOffset() == 0) {
	if (bar.mBarNum % 16 == 0) {
	  vel = 127;
	} else if (bar.mBarNum % 4 == 0) {
	  vel = 105;
	} else if (bar.mBarNum % 2 == 0) {
	  vel = 90;
	}
      }
      output.push_back(InstrumentEvent(noteEvent, bar.mOffset, mInstrument, vel));
    }
  }

  if (bar.mBarNum >= 0) {
    int i;
    for (i = 0; i < 4; i++) {
      TimeDelta offset = TimeDelta::sBar * i / 4;
      Note octave(36);
      int vel = 70;
      if (bar.mBarNum % 16 == 0 && i == 0) {
	vel = 127;
      } else if (bar.mBarNum % 4 == 0 && i == 0) {
	vel = 105;
      } else if (bar.mBarNum % 2 == 0 && i == 0) {
	vel = 90;
      }
      const Chord& chord = state.mTune->GetChord(offset + TimeDelta::sBar * bar.mBarNum, state);
      if (i % 2 == 0) {
	if (i % 4 == 0 || chord != state.mTune->GetChord(TimeDelta::sBar * bar.mBarNum, state)) {
	  output.push_back(InstrumentEvent(chord.GetBase(octave), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
	  output.push_back(InstrumentEvent(chord.GetBase(octave + 12), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
	} else {
	  output.push_back(InstrumentEvent(chord.GetFifth(octave), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
	  output.push_back(InstrumentEvent(chord.GetFifth(octave + 12), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
	}
      } else {
	output.push_back(InstrumentEvent(chord.GetBase(octave + 24), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
	output.push_back(InstrumentEvent(chord.GetThird(octave + 24), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
	output.push_back(InstrumentEvent(chord.GetFifth(octave + 24), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel));
      }
    }
  }
}
