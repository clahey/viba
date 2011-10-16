/*
 * sequence.cc
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#include "fluidOutputSequence.hh"

FluidOutputSequence::FluidOutputSequence()
{
  mSequencer = new_fluid_sequencer2(TRUE);
}

FluidOutputSequence::~FluidOutputSequence()
{
  delete_fluid_sequencer(mSequencer);
}

