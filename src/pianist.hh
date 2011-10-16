/*
 * pianist.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef PIANIST_HH_
#define PIANIST_HH_

#include "musician.hh"

/*
 *
 */
class Pianist : public Musician
{
protected:
  Pianist();
  virtual ~Pianist();
  virtual void GenerateNotes(Tune* tune, InstrumentSequenceData* sequenceData, int barNum, const SongState& state);
};

#endif /* PIANIST_HH_ */
