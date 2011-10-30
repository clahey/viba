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
#include "fluidOutputSequence.hh"
#include "songState.hh"
#include "tune.hh"

/*
 *
 */
class Musician : public Generator
{
public:
  virtual void SetTune(Tune* tune) { mTune = tune; };
  void Generate(Sequence* dest, TimeDelta start, TimeDelta end, const SongState& state);

protected:
  Tune* mTune;
  virtual void FillOutput(std::vector<InstrumentEvent>& output, const SongState::BarData& bar, const SongState& state) = 0;

private:
  void GenerateBar(const SongState::BarData& bar, FluidOutputSequence* output, const SongState& state);
};

#endif /* MUSICIAN_HH_ */
