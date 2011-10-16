/*
 * outputSequence.hh
 *
 *  Created on: Oct 2, 2011
 *      Author: clahey
 */

#ifndef FLUIDOUTPUTSEQUENCE_HH_
#define FLUIDOUTPUTSEQUENCE_HH_


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
  TimeDelta GetLength() { return 0; };
  TimeDelta GetCurrentTime();
private:
  fluid_sequencer_t* mSequencer;
};

#endif /* OUTPUTSEQUENCE_HH_ */
