/*
 * sequence.cc
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#include "fluidOutputSequence.hh"

#include <glibmm/main.h>
#include <cassert>

const int FluidOutputSequence::sSPerM;
const int FluidOutputSequence::sMSPerS;
const int FluidOutputSequence::sBeatsPerBar;

static void
OnCallback(unsigned int time,
	   fluid_event_t* event,
	   fluid_sequencer_t* seq,
	   void* data)
{
  FluidOutputSequence* sequence = static_cast<FluidOutputSequence*>(data);
  assert(sequence != NULL);
  // Switches to the main loop thread.
  Glib::signal_idle().connect_once(sigc::bind(sigc::mem_fun(sequence, &FluidOutputSequence::OnCallback), time));
}

FluidOutputSequence::FluidOutputSequence()
  : mSequencerBase(0),
    mTicksBase(0),
    mBPM(120),
    mCallbackWaiting(false),
    mPaused(true)
{
  mSettings = new_fluid_settings();
  fluid_settings_setstr(mSettings, "synth.reverb.active", "yes");
  fluid_settings_setstr(mSettings, "synth.chorus.active", "no");
  fluid_settings_setstr(mSettings, "audio.driver", "pulseaudio");
  mSynth = new_fluid_synth(mSettings);
  mSequencer = new_fluid_sequencer2(FALSE);

    // register synth as first destination
    mSynthSeqID = fluid_sequencer_register_fluidsynth(mSequencer, mSynth);

    // put your own path here
    fluid_synth_sfload(mSynth, "/usr/share/sounds/sf2/FluidR3_GM.sf2", 1);
    //    fluid_res = fluid_synth_sfload(mSynth, "/home/clahey/workspace/viba/12ACGUIT.SF2", 1);

    // register myself as second destination
    mMySeqID = fluid_sequencer_register_client(mSequencer, "me", ::OnCallback, this);

    fluid_event_t *evt = new_fluid_event();
    fluid_event_set_source(evt, -1);
    fluid_event_set_dest(evt, mSynthSeqID);
    fluid_event_program_select(evt, 1, 1, 0, 110);
    fluid_sequencer_send_now(mSequencer, evt);
}

FluidOutputSequence::~FluidOutputSequence()
{
  delete_fluid_sequencer(mSequencer);
  if (mDriver != NULL) {
    delete_fluid_audio_driver(mDriver);
  }
  delete_fluid_synth(mSynth);
}

/* Returns true if successful */
bool
FluidOutputSequence::SendInstrumentEvent(InstrumentEventPtr event)
{
//  if (mPaused) {
//    mEventQueue.insert(std::make_pair(event->GetOffset(), event));
//    return true;
//  } else {
    int fluid_res;
    fluid_event_t *evt = new_fluid_event();
    fluid_event_set_source(evt, -1);
    fluid_event_set_dest(evt, mSynthSeqID);
    fluid_event_noteon(evt, event->GetInstrument()->GetChannel(), event->GetNote().GetMidiNote(), event->GetVelocity());
    fluid_res = fluid_sequencer_send_at(mSequencer, evt, TimeDeltaToMS(event->GetOffset()), TRUE);
    //    printf("Note %d starts at %d (%d)\n", event->GetNote().GetMidiNote(), TimeDeltaToMS(event->GetOffset()), event->GetOffset().GetTicks());
    printf("Note %d plays at %f for %f until %f\n", event->GetNote().GetMidiNote(), event->GetOffset() / TimeDelta::sBar, event->GetLength() / TimeDelta::sBar, (event->GetOffset() + event->GetLength()) / TimeDelta::sBar);
    if (fluid_res == FLUID_FAILED) {
      delete_fluid_event(evt);
      return false;
    }
    fluid_event_noteoff(evt, event->GetInstrument()->GetChannel(), event->GetNote().GetMidiNote());
    fluid_res = fluid_sequencer_send_at(mSequencer, evt, TimeDeltaToMS(event->GetOffset() + event->GetLength()), TRUE);
    //    printf("Note %d ends at %d\n", event->GetNote().GetMidiNote(), TimeDeltaToMS(event->GetOffset() + event->GetLength()));
    if (fluid_res == FLUID_FAILED) {
      delete_fluid_event(evt);
      return false;
    }
    delete_fluid_event(evt);
    return true;
    //  }
}

TimeDelta
FluidOutputSequence::GetCurrentTime()
{
  return MSToTimeDelta(fluid_sequencer_get_tick(mSequencer));
}

void
FluidOutputSequence::ScheduleCallback(TimeDelta time, sigc::slot<void> callback)
{
  mCallbackMap.insert(std::make_pair(time, callback));
  ScheduleNextTimeout();
}

void
FluidOutputSequence::ScheduleNextTimeout()
{
  if (!mPaused && !mCallbackMap.empty() && !mCallbackWaiting) {
    //    printf ("Scheduling Next Timeout for %d(%d)\n", TimeDeltaToMS(mCallbackMap.begin()->first), mCallbackMap.begin()->first.GetTicks());
    fluid_event_t *evt = new_fluid_event();
    fluid_event_set_source(evt, -1);
    fluid_event_set_dest(evt, mMySeqID);
    fluid_event_timer(evt, NULL);
    fluid_sequencer_send_at(mSequencer, evt,
			    TimeDeltaToMS(mCallbackMap.begin()->first), 1);
    delete_fluid_event(evt);
    mCallbackWaiting = true;
  }
}

void
FluidOutputSequence::OnCallback(unsigned int time)
{
  //  printf("OnCallback: %d\n", time);
  if (!mPaused) {
    CallbackMap::iterator it = mCallbackMap.begin();
    TimeDelta currentTime = MSToTimeDelta(time);
    while (it != mCallbackMap.end() && it->first <= currentTime) {
      it->second();
      mCallbackMap.erase(it++);
    }
    mCallbackWaiting = false;
    ScheduleNextTimeout();
  } else {
    mCallbackWaiting = false;
  }
}

void
FluidOutputSequence::Clear()
{
  fluid_sequencer_remove_events(mSequencer, -1, mSynthSeqID, FLUID_SEQ_NOTEON);
  fluid_sequencer_remove_events(mSequencer, -1, mSynthSeqID, FLUID_SEQ_NOTEOFF);
}

void
FluidOutputSequence::Refill()
{
//  EventQueue queue;
//  queue.swap(mEventQueue);
//  EventQueue::const_iterator it = queue.begin(); //lower_bound(mTicksBase);
//  for (; it != queue.end(); it++) {
//    SendInstrumentEvent(it->second);
//  }
}

void
FluidOutputSequence::Quiet()
{
  delete_fluid_audio_driver(mDriver);
  mDriver = NULL;
}

void
FluidOutputSequence::Loud()
{
  mDriver = new_fluid_audio_driver(mSettings, mSynth);
}

void
FluidOutputSequence::Play()
{
  if (!mPaused) {
    return;
  }
  Loud();
  //  mSequencerBase = fluid_sequencer_get_tick(mSequencer);
  //  Refill();
  mPaused = false;
  ScheduleNextTimeout();
}

void
FluidOutputSequence::Pause()
{
  if (mPaused) {
    return;
  }
  //  Clear();
  Quiet();
  //  mSequencerBase = fluid_sequencer_get_tick(mSequencer);
  //  mTicksBase = MSToTimeDelta(mSequencerBase);
  mPaused = true;
}

void
FluidOutputSequence::SetBPM(double bpm)
{
  //  Clear();
  mBPM = bpm;
//  unsigned int base = fluid_sequencer_get_tick(mSequencer)
//  mSequencerBase = base;
//  mTicksBase = MSToTimeDelta(base);
  //  Refill();
}

void
FluidOutputSequence::SetPivot(TimeDelta pivot)
{
  mSequencerBase = TimeDeltaToMS(pivot);
  mTicksBase = pivot;
}
