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
      int vel = 80;
      if (bar.mBarNum % 4 == 0 && noteEvent.GetOffset() == 0) {
	vel = 115;
      } else if (bar.mBarNum % 2 == 0 && noteEvent.GetOffset() == 0) {
	vel = 100;
      }
      InstrumentEvent event(noteEvent, bar.mOffset, mInstrument, vel);
      event.Randomize(1.0/2048);
      output->SendInstrumentEvent(&event);
    }
  }
  if (bar.mBarNum >= 0) {
    int i;
    for (i = 0; i < 4; i++) {
      TimeDelta offset = TimeDelta::sBar * i / 4;
      if (offset >= bar.mStart && offset < bar.mEnd) {
	Note octave(60);
	int vel = 80;
	if (bar.mBarNum % 4 == 0 && i == 0) {
	  vel = 115;
	} else if (bar.mBarNum % 2 == 0 && i == 0) {
	  vel = 100;
	}
	const Chord& chord = state.mTune->GetChord(offset + TimeDelta::sBar * bar.mBarNum, state);
	if (i % 2 == 0) {
	  InstrumentEvent event(chord.GetBase(octave), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel);
	  event.Randomize(1.0/2048);
	  output->SendInstrumentEvent(&event);
	} else {
	  InstrumentEvent event(chord.GetThird(octave), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel);
	  event.Randomize(1.0/2048);
	  output->SendInstrumentEvent(&event);
	  event = InstrumentEvent(chord.GetFifth(octave), TimeDelta::sBar / 4, offset + bar.mOffset, mInstrument, vel);
	  event.Randomize(1.0/2048);
	  output->SendInstrumentEvent(&event);
	}
      }
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
