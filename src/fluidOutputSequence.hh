/*
 * outputSequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef FLUIDOUTPUTSEQUENCE_HH_
#define FLUIDOUTPUTSEQUENCE_HH_

#include <map>

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
  // Overrides
  FluidOutputSequence();
  ~FluidOutputSequence();
  TimeDelta GetLength() const { return 0; };
  TimeDelta GetCurrentTime();
  void ScheduleCallback(TimeDelta offset, sigc::slot<void> callback);

  // New methods
  bool SendInstrumentEvent(InstrumentEventPtr event);

  // For internal use from C code.
  void OnCallback(unsigned int time);

  virtual void SetBPM(double newBPM);
  virtual double GetBPM() { return mBPM; };
  virtual void SetPivot(TimeDelta pivot);

  void Play();
  void Pause();

private:
  typedef std::multimap<TimeDelta, InstrumentEventPtr> EventQueue;

  FluidOutputSequence(const FluidOutputSequence& other) {};
  static const int sSPerM = 60;
  static const int sMSPerS = 1000;
  static const int sBeatsPerBar = 2;

  TimeDelta MSToTimeDelta(unsigned int ms) {
    int ms_offset = ms - mSequencerBase;
    return TimeDelta::sBar * (ms_offset * mBPM / (sSPerM * sMSPerS * sBeatsPerBar)) + mTicksBase;
  };
  unsigned int TimeDeltaToMS(TimeDelta time) {
    TimeDelta time_offset = time - mTicksBase;
    return static_cast<unsigned int>((time_offset / TimeDelta::sBar) / mBPM * (sSPerM * sMSPerS * sBeatsPerBar)) + mSequencerBase;
  };
  void ScheduleNextTimeout();
  void Clear();
  void Refill();
  void Quiet();
  void Loud();

  fluid_settings_t* mSettings;
  fluid_sequencer_t* mSequencer;
  fluid_synth_t* mSynth;
  fluid_audio_driver_t* mDriver;
  short mSynthSeqID;
  short mMySeqID;
  unsigned int mSequencerBase;
  TimeDelta mTicksBase;
  double mBPM;
  typedef std::multimap<TimeDelta, sigc::slot<void> > CallbackMap;
  CallbackMap mCallbackMap;
  bool mCallbackWaiting;
  bool mPaused;
  EventQueue mEventQueue;
};

#endif /* OUTPUTSEQUENCE_HH_ */
