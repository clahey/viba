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

TimeDelta
FluidOutputSequence::GetCurrentTime()
{
  unsigned int ms_offset = fluid_sequencer_get_tick(mSequencer) - mSequencerBase;
  return sBar * (ms_offset * mBPM / 120000) + mTicksBase;
}

