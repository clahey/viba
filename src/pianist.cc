/*
 * pianist.cc
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#include "pianist.hh"

Pianist::~Pianist()
{
  // TODO Auto-generated destructor stub
}

void
Pianist::GenerateNotes(Tune* tune,
		       InstrumentSequenceData* sequenceData,
		       int barNum,
		       const SongState& state)
{
  if (barNum < 0) {
    return;
  }
  
}
