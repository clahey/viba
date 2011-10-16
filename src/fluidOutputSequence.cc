/*
 * sequence.cc
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#include "fluidOutputSequence.hh"

FluidOutputSequence::FluidOutputSequence()
  : mSequencer(new_fluid_sequencer2(TRUE)),
    mSequencerBase(0),
    mTicksBase(0),
    mBPM(120)
{
}

FluidOutputSequence::~FluidOutputSequence()
{
  delete_fluid_sequencer(mSequencer);
}

#define S_PER_M 60
#define MS_PER_S 1000
#define BEATS_PER_BAR 2

TimeDelta
FluidOutputSequence::GetCurrentTime()
{
  unsigned int ms_offset = fluid_sequencer_get_tick(mSequencer) - mSequencerBase;
  return TimeDelta::sBar * (ms_offset * mBPM / (S_PER_M * MS_PER_S * BEATS_PER_BAR)) + mTicksBase;
}

