/*
 * musician.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef MUSICIAN_HH_
#define MUSICIAN_HH_

#include "generator.hh"
#include "instrumentSequenceData.hh"
#include "songState.hh"
#include "tune.hh"

/*
 *
 */
class Musician : public Generator
{
public:
  virtual void SetTune(Tune* tune) { mTune = tune; };
protected:
  Tune* mTune;
};

#endif /* MUSICIAN_HH_ */
