/*
 ============================================================================
 Name        : viba.cpp
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C++,
 ============================================================================
 */

#include <algorithm>
#include <iostream>
#include <sigc++/sigc++.h>
#include "fluidOutputSequence.hh"
#include "songState.hh"
#include "tune.hh"

using namespace std;

Instrument instrument(2);

void
OutputNote(const NoteEvent& noteEvent, FluidOutputSequence* output, TimeDelta& offset)
{
  InstrumentEvent event(noteEvent, offset, &instrument, 100);
  event = event.Randomize(1.0/2048);
  output->SendInstrumentEvent(&event);
}

void
OutputSequenceData(const NoteSequenceData& data, FluidOutputSequence* output, TimeDelta& offset)
{
  const std::vector<NoteEvent>& notes = data.GetNotes();
  std::for_each(notes.begin(), notes.end(), sigc::bind(sigc::ptr_fun(&OutputNote), output, offset));
  offset += data.GetLength();
}

int main(int argc, char* argv[]) {
  Tune tune;
  tune.Parse(argv[1]);

  FluidOutputSequence* output = new FluidOutputSequence;
  TimeDelta offset = 0;
  const DataSequence* sequence;

  int bar;

  offset = output->GetCurrentTime();

  SongState state;
  state.mLastTime = true;
  for (bar = -1; bar < 32; bar++) {
    OutputSequenceData(tune.GetNotes(bar, state), output, offset);
  }

  sleep(34);

  delete output;
  
  return 0;
}
