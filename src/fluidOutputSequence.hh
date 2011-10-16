/*
 * outputSequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef FLUIDOUTPUTSEQUENCE_HH_
#define FLUIDOUTPUTSEQUENCE_HH_


#include "instrumentEvent.hh"
#include "outputSequence.hh"

extern "C" {
#include <fluidsynth.h>
#include <fluidsynth/seq.h>
};

/*
 *
 */
class FluidOutputSequence : public OutputSequence
{
public:
  FluidOutputSequence();
  ~FluidOutputSequence();
  TimeDelta GetLength() const { return 0; };
  TimeDelta GetCurrentTime() { return MSToTimeDelta(fluid_sequencer_get_tick(mSequencer)); };
  bool SendInstrumentEvent(InstrumentEvent* event);

private:
  static const int sSPerM = 60;
  static const int sMSPerS = 1000;
  static const int sBeatsPerBar = 2;

  TimeDelta MSToTimeDelta(unsigned int ms) {
    unsigned int ms_offset = ms - mSequencerBase;
    return TimeDelta::sBar * (ms_offset * mBPM / (sSPerM * sMSPerS * sBeatsPerBar)) + mTicksBase;
  };
  unsigned int TimeDeltaToMS(TimeDelta time) {
    TimeDelta time_offset = time - mTicksBase;
    return static_cast<unsigned int>((time_offset / TimeDelta::sBar) / mBPM * (sSPerM * sMSPerS * sBeatsPerBar)) + mSequencerBase;
  };
  fluid_sequencer_t* mSequencer;
  fluid_synth_t* mSynth;
  fluid_audio_driver_t* mDriver;
  short mSynthSeqID;
  short mMySeqID;
  int mSequencerBase;
  TimeDelta mTicksBase;
  double mBPM;
};

#endif /* OUTPUTSEQUENCE_HH_ */
