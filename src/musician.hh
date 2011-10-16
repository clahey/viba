/*
 * musician.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef MUSICIAN_HH_
#define MUSICIAN_HH_

#include "instrumentSequenceData.hh"
#include "songState.hh"
#include "tune.hh"

/*
 *
 */
class Musician
{
protected:
  Musician();
  virtual ~Musician();
  virtual void GenerateNotes(Tune* tune, InstrumentSequenceData* sequenceData, int barNum, const SongState& state) = 0;
};

#endif /* MUSICIAN_HH_ */
