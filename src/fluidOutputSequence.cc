/*
 * sequence.cc
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#include "fluidOutputSequence.hh"

const int FluidOutputSequence::sSPerM;
const int FluidOutputSequence::sMSPerS;
const int FluidOutputSequence::sBeatsPerBar;

FluidOutputSequence::FluidOutputSequence()
  : mSequencerBase(0),
    mTicksBase(0),
    mBPM(120)
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

    int fluid_res;
    // put your own path here
    fluid_res = fluid_synth_sfload(mSynth, "/usr/share/sounds/sf2/FluidR3_GM.sf2", 1);

    // register myself as second destination
    //    mMySeqID = fluid_sequencer_register_client(sequencer, "me", seq_callback, NULL);
    mMySeqID = -1;
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
}
