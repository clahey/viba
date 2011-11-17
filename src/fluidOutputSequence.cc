/*
 * sequence.cc
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#include "fluidOutputSequence.hh"

#include <glibmm/main.h>

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
  printf("::OnCallback: %d\n", time);
  assert(sequence != NULL);
  Glib::signal_idle().connect_once(sigc::bind(sigc::mem_fun(sequence, &FluidOutputSequence::OnCallback), time));
}

FluidOutputSequence::FluidOutputSequence()
  : mSequencerBase(0),
    mTicksBase(0),
    mBPM(147),
    mCallbackWaiting(false)
{
    fluid_settings_t* settings;
    settings = new_fluid_settings();
    fluid_settings_setstr(settings, "synth.reverb.active", "yes");
    fluid_settings_setstr(settings, "synth.chorus.active", "no");
    fluid_settings_setstr(settings, "audio.driver", "pulseaudio");
    mSynth = new_fluid_synth(settings);
    mDriver = new_fluid_audio_driver(settings, mSynth);
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
  delete_fluid_audio_driver(mDriver);
  delete_fluid_synth(mSynth);
}

/* Returns true if successful */
bool
FluidOutputSequence::SendInstrumentEvent(InstrumentEvent* event)
{
    int fluid_res;
    fluid_event_t *evt = new_fluid_event();
    fluid_event_set_source(evt, -1);
    fluid_event_set_dest(evt, mSynthSeqID);
    fluid_event_noteon(evt, event->GetInstrument()->GetChannel(), event->GetNote().GetMidiNote(), event->GetVelocity());
    fluid_res = fluid_sequencer_send_at(mSequencer, evt, TimeDeltaToMS(event->GetOffset()), TRUE);
    printf("Note %d starts at %d (%d)\n", event->GetNote().GetMidiNote(), TimeDeltaToMS(event->GetOffset()), event->GetOffset().GetTicks());
    if (fluid_res == FLUID_FAILED) {
      delete_fluid_event(evt);
      return false;
    }
    fluid_event_noteoff(evt, event->GetInstrument()->GetChannel(), event->GetNote().GetMidiNote());
    fluid_res = fluid_sequencer_send_at(mSequencer, evt, TimeDeltaToMS(event->GetOffset() + event->GetLength()), TRUE);
    printf("Note %d ends at %d\n", event->GetNote().GetMidiNote(), TimeDeltaToMS(event->GetOffset() + event->GetLength()));
    if (fluid_res == FLUID_FAILED) {
      delete_fluid_event(evt);
      return false;
    }
    delete_fluid_event(evt);
    return true;
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
  if (!mCallbackMap.empty() && !mCallbackWaiting) {
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
  printf ("OnCallback %d\n", time);
  mCallbackWaiting = false;
  CallbackMap::iterator it = mCallbackMap.begin();
  TimeDelta endtime = MSToTimeDelta(time);
  while (it != mCallbackMap.end()) {
    if (it->first < endtime) {
      it->second();
      mCallbackMap.erase(it++);
    } else {
      break;
    }
  }
  ScheduleNextTimeout();
}
