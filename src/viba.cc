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
#include "tune.hh"

using namespace std;

Instrument instrument;

void
OutputNote(NoteEvent noteEvent, FluidOutputSequence* output, TimeDelta& offset)
{
  InstrumentEvent event(noteEvent, offset, &instrument, 100);
  output->SendInstrumentEvent(&event);
}

void
OutputSequenceData(const SequenceData* data, FluidOutputSequence* output, TimeDelta& offset)
{
  const std::vector<NoteEvent>& notes = dynamic_cast<const NoteSequenceData*>(data)->GetNotes();
  std::for_each(notes.begin(), notes.end(), sigc::bind(sigc::ptr_fun(&OutputNote), output, offset));
  offset += data->GetLength();
}

void
OutputSequence(FluidOutputSequence* output, const DataSequence* sequence, TimeDelta offset)
{
  const std::vector<SequenceData*>& data = sequence->GetData();
  TimeDelta offsetMod = offset;
  std::for_each(data.begin(), data.end(), sigc::bind(sigc::ptr_fun(&OutputSequenceData), output, sigc::ref(offsetMod)));
}

int main(int argc, char* argv[]) {
  Tune tune;
  tune.Parse(argv[1]);

  FluidOutputSequence* output = new FluidOutputSequence();
  TimeDelta offset = 0;
  const DataSequence* sequence;

  sequence = &tune.getIntro();
  OutputSequence(output, sequence, offset);
  offset += sequence->GetLength();
  sequence = &tune.getMain();
  OutputSequence(output, sequence, offset);
  offset += sequence->GetLength();
  sequence = &tune.getOutro();
  OutputSequence(output, sequence, offset);

  sleep(32000);
  
  return 0;
}
