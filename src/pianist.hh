/*
 * pianist.hh
 *
 *  Created on: Oct 8, 2011
 *      Author: clahey
 */

#ifndef PIANIST_HH_
#define PIANIST_HH_

#include "fluidOutputSequence.hh"
#include "musician.hh"

/*
 *
 */
class Pianist : public Musician
{
public:
  Pianist(Instrument* instrument) : mInstrument(instrument) {};
  void Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state);
private:
  Instrument* mInstrument;
  void GenerateBar(const SongState::BarData& bar, FluidOutputSequence* output, const SongState& state);
};

#endif /* PIANIST_HH_ */
